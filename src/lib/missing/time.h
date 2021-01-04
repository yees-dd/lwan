/*
 * lwan - simple web server
 * Copyright (c) 2012 Leandro A. F. Pereira <leandro@hardinfo.org>
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

#include_next <time.h>

#ifndef MISSING_TIME_H
#define MISSING_TIME_H

#ifndef HAVE_CLOCK_GETTIME
typedef int clockid_t;
int clock_gettime(clockid_t clk_id, struct timespec *ts);
#endif

#if !defined(CLOCK_MONOTONIC_COARSE) && defined(CLOCK_MONOTONIC_FAST)
#  define CLOCK_MONOTONIC_COARSE CLOCK_MONOTONIC_FAST /* FreeBSD */
#elif !defined(CLOCK_MONOTONIC_COARSE) && defined(CLOCK_MONOTONIC_RAW_APPROX)
#  define CLOCK_MONOTONIC_COARSE CLOCK_MONOTONIC_RAW_APPROX /* New-ish macOS */
#elif !defined(CLOCK_MONOTONIC_COARSE)
#  if defined(CLOCK_MONOTONIC)
#    define CLOCK_MONOTONIC_COARSE CLOCK_MONOTONIC
#  else
#    define CLOCK_MONOTONIC_COARSE 0xbebac0ca /* Old macOS, usually */
#  endif
#endif

#if !defined(CLOCK_MONOTONIC)
#define CLOCK_MONOTONIC 0xbebacafe
#endif

#endif /* MISSING_TIME_H */
