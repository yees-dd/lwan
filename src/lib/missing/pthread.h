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

#include_next <pthread.h>

#ifndef MISSING_PTHREAD_H
#define MISSING_PTHREAD_H

#ifndef LWAN_HAVE_PTHREADBARRIER
typedef int pthread_barrierattr_t;
typedef struct pthread_barrier {
    unsigned int count;
    unsigned int in;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} pthread_barrier_t;

int pthread_barrier_init(pthread_barrier_t *restrict barrier,
                         const pthread_barrierattr_t *restrict attr,
                         unsigned int count);
int pthread_barrier_destroy(pthread_barrier_t *barrier);
int pthread_barrier_wait(pthread_barrier_t *barrier);
#endif

#if defined(__FreeBSD__) || defined(__OpenBSD__)
#include <pthread_np.h>
#endif

#ifndef LWAN_HAVE_PTHREAD_SET_NAME_NP
int pthread_set_name_np(pthread_t thread, const char *name);
#endif

#endif /* MISSING_PTHREAD_H */
