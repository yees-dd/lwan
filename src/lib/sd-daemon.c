/*-*- Mode: C; c-basic-offset: 8; indent-tabs-mode: nil -*-*/

/***
  This file is based on sd-daemon.c from systemd. Unused code has been
  removed.

  Copyright 2010 Lennart Poettering

  systemd is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  systemd is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with systemd; If not, see <http://www.gnu.org/licenses/>.
***/

#define _GNU_SOURCE
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "sd-daemon.h"
#include "lwan-config.h"

static void unsetenv_listen_vars(void) {
        unsetenv("LISTEN_PID");
        unsetenv("LISTEN_FDS");
        unsetenv("LISTEN_FDNAMES");
}

int sd_listen_fds(int unset_environment) {
        int n, l, r, fd;
        const char *e;

        e = getenv("LISTEN_PID");
        if (!e) {
                r = 0;
                goto finish;
        }

        l = parse_int(e, -1);
        if (l <= 0) {
                r = -EINVAL;
                goto finish;
        }

        /* Is this for us? */
        if (getpid() != (pid_t)l) {
                r = 0;
                goto finish;
        }

        e = getenv("LISTEN_FDS");
        if (!e) {
                r = 0;
                goto finish;
        }

        n = parse_int(e, -1);
        if (!n) {
                r = 0;
                goto finish;
        }

        static_assert(SD_LISTEN_FDS_START < INT_MAX, "");
        if (n < 0 || n > INT_MAX - SD_LISTEN_FDS_START) {
                r = -EINVAL;
                goto finish;
        }

        for (fd = SD_LISTEN_FDS_START; fd < SD_LISTEN_FDS_START + (int)n; fd++) {
                int flags;

                flags = fcntl(fd, F_GETFD);
                if (flags < 0) {
                        r = -errno;
                        goto finish;
                }

                if (flags & FD_CLOEXEC)
                        continue;

                if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) < 0) {
                        r = -errno;
                        goto finish;
                }
        }

        r = n;

finish:
        if (unset_environment)
                unsetenv_listen_vars();

        return r;
}

/* Both strv_extend_n() and strv_split() aren't simple copies of the
 * same functions from systemd.  These are simplified versions of those
 * functions, used only in the sd_listen_fds_with_names() ported from
 * newer versions of systemd.
 */
static int strv_extend_n(char ***p, const char *s, int n) {
        if (!p)
                return -EINVAL;
        if (!s)
                return -EINVAL;

        *p = calloc((size_t)n, sizeof(char *));
        if (!p)
                return -ENOMEM;

        size_t s_size = strlen(s) + 1;
        char *copies = calloc((size_t)(n + 1), s_size);
        if (!copies) {
                free(*p);
                return -ENOMEM;
        }
        for (int i = 0; i < n; i++) {
                char *copy = &copies[(size_t)i * s_size];
                *p[i] = memcpy(copy, s, s_size);
        }        

        return 0;
}

static int strv_split(char ***p, const char *value, const char separator) {
        char *copy = strdup(value);
        int n_split = 0;

        if (!copy)
                return -ENOMEM;

        for (char *c = copy; *c; ) {
                char *sep_pos = strchr(c, separator);
                if (!sep_pos)
                        break;

                n_split++;
                c = sep_pos + 1;
        }        

        if (!n_split)
                return 0;

        *p = calloc((size_t)(n_split + 1), sizeof(char *));
        if (!*p) {
                free(copy);
                return -ENOMEM;
        }

        int i = 0;
        for (char *c = copy; *c; ) {
                char *sep_pos = strchr(c, separator);
                if (!sep_pos)
                        break;

                *sep_pos = '\0';
                *p[i++] = c;
                c = sep_pos + 1;
        }        

        return n_split;
}

int sd_listen_fds_with_names(int unset_environment, char ***names) {
        char **l = NULL;
        bool have_names;
        int n_names = 0, n_fds;
        const char *e;
        int r;

        if (!names)
                return sd_listen_fds(unset_environment);

        e = getenv("LISTEN_FDNAMES");
        if (e) {
                n_names = strv_split(&l, e, ':');
                if (n_names < 0) {
                        if (unset_environment)
                                unsetenv_listen_vars();
                        return n_names;
                }

                have_names = true;
        } else {
                have_names = false;
        }

        n_fds = sd_listen_fds(unset_environment);
        if (n_fds <= 0) {
                r = n_fds;
                goto fail;
        }

        if (have_names) {
                if (n_names != n_fds) {
                        r = -EINVAL;
                        goto fail;
                }
        } else {
                r = strv_extend_n(&l, "unknown", n_fds);
                if (r < 0)
                        goto fail;
        }

        *names = l;

        return n_fds;

fail:
        free(l);
        return r;
}

static int sd_is_socket_internal(int fd, int type, int listening) {
        struct stat st_fd;

        if (fd < 0)
                return -EBADF;

        if (type < 0)
                return -EINVAL;

        if (fstat(fd, &st_fd) < 0)
                return -errno;

        if (!S_ISSOCK(st_fd.st_mode))
                return 0;

        if (type != 0) {
                int other_type = 0;
                socklen_t l = sizeof(other_type);

                if (getsockopt(fd, SOL_SOCKET, SO_TYPE, &other_type, &l) < 0)
                        return -errno;

                if (l != sizeof(other_type))
                        return -EINVAL;

                if (other_type != type)
                        return 0;
        }

        if (listening >= 0) {
                int accepting = 0;
                socklen_t l = sizeof(accepting);

                if (getsockopt(fd, SOL_SOCKET, SO_ACCEPTCONN, &accepting, &l) < 0)
                        return -errno;

                if (l != sizeof(accepting))
                        return -EINVAL;

                if (!accepting != !listening)
                        return 0;
        }

        return 1;
}

union sockaddr_union {
        struct sockaddr sa;
        struct sockaddr_in in4;
        struct sockaddr_in6 in6;
};

int sd_is_socket_inet(int fd, int family, int type, int listening, uint16_t port) {
        union sockaddr_union sockaddr = {};
        socklen_t l = sizeof(sockaddr);
        int r;

        if (family != 0 && family != AF_INET && family != AF_INET6)
                return -EINVAL;

        r = sd_is_socket_internal(fd, type, listening);
        if (r <= 0)
                return r;

        if (getsockname(fd, &sockaddr.sa, &l) < 0)
                return -errno;

        if (l < sizeof(sa_family_t))
                return -EINVAL;

        if (sockaddr.sa.sa_family != AF_INET &&
            sockaddr.sa.sa_family != AF_INET6)
                return 0;

        if (family > 0)
                if (sockaddr.sa.sa_family != family)
                        return 0;

        if (port > 0) {
                if (sockaddr.sa.sa_family == AF_INET) {
                        if (l < sizeof(struct sockaddr_in))
                                return -EINVAL;

                        return htons(port) == sockaddr.in4.sin_port;
                } else {
                        if (l < sizeof(struct sockaddr_in6))
                                return -EINVAL;

                        return htons(port) == sockaddr.in6.sin6_port;
                }
        }

        return 1;
}
