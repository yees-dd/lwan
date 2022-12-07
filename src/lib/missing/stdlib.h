/*
 * lwan - web server
 * Copyright (c) 2017 L. A. F. Pereira <l@tia.mat.br>
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

#include_next <stdlib.h>

#ifndef MISSING_STDLIB_H
#define MISSING_STDLIB_H

#if defined(__GLIBC__)
#include <features.h>
#if defined(__GLIBC_PREREQ) && __GLIBC_PREREQ(2, 17)
#define LWAN_HAVE_SECURE_GETENV
#endif
#if defined(__GLIBC_PREREQ) && __GLIBC_PREREQ(2, 19)
#define LWAN_HAVE_MKOSTEMPS
#endif
#if defined(__GLIBC_PREREQ) && __GLIBC_PREREQ(2, 7)
#define LWAN_HAVE_CORRECT_UMASK_TMPFILE
#endif
#endif

#if !defined(LWAN_HAVE_SECURE_GETENV)
static inline char *secure_getenv(const char *name) { return getenv(name); }
#endif

#if !defined(LWAN_HAVE_MKOSTEMP)
int mkostemp(char *tmpl, int flags);
#endif

#if defined(LWAN_HAVE_CORRECT_UMASK_TMPFILE)
#define umask_for_tmpfile(mask_)                                               \
    ({                                                                         \
        (void)(mask_);                                                         \
        0U;                                                                    \
    })
#else
#define umask_for_tmpfile(mask_) umask(mask_)
#endif

#if !defined(LWAN_HAVE_REALLOCARRAY)
void *reallocarray(void *optr, size_t nmemb, size_t size);
#endif

#endif /* MISSING_STDLIB_H */
