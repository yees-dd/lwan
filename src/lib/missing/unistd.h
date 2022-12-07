/*
 * lwan - web server
 * Copyright (c) 2012 L. A. F. Pereira <l@tia.mat.br>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include_next <unistd.h>

#ifndef MISSING_UNISTD_H
#define MISSING_UNISTD_H

#ifndef LWAN_HAVE_PIPE2
int pipe2(int pipefd[2], int flags);
#endif

#if defined(__APPLE__)
int setresuid(uid_t ruid, uid_t euid, uid_t suid)
    __attribute__((warn_unused_result));
int setresgid(gid_t rgid, gid_t egid, gid_t sgid)
    __attribute__((warn_unused_result));
int getresuid(uid_t *ruid, uid_t *euid, uid_t *suid)
    __attribute__((warn_unused_result));
int getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid)
    __attribute__((warn_unused_result));
#endif

#if !defined(LWAN_HAVE_GET_CURRENT_DIR_NAME)
char *get_current_dir_name(void);
#endif

#if defined(__APPLE__)
/* getrandom() is defined in <sys/random.h> on macOS */
#include <sys/random.h>
#endif

#endif /* MISSING_UNISTD_H */
