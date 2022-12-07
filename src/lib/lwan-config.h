/*
 * lwan - web server
 * Copyright (c) 2013 L. A. F. Pereira <l@tia.mat.br>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#define ONE_MINUTE 60
#define ONE_HOUR (ONE_MINUTE * 60)
#define ONE_DAY (ONE_HOUR * 24)
#define ONE_WEEK (ONE_DAY * 7)
#define ONE_MONTH (ONE_DAY * 31)
#define ONE_YEAR (ONE_MONTH * 12)

#include <stdio.h>
#include <stdbool.h>

#include "lwan-strbuf.h"

enum config_line_type {
    CONFIG_LINE_TYPE_LINE,
    CONFIG_LINE_TYPE_SECTION,
    CONFIG_LINE_TYPE_SECTION_END
};

struct config_line {
    char *key;
    char *value;

    enum config_line_type type;
};

struct config;

struct config *config_open(const char *path);
void config_close(struct config *conf);
bool config_error(struct config *conf, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
const struct config_line *config_read_line(struct config *conf);

const char *config_last_error(struct config *conf);
int config_cur_line(struct config *conf);

struct config *config_isolate_section(struct config *current_conf,
                                      const struct config_line *current_line);
bool config_skip_section(struct config *conf, const struct config_line *line);

bool parse_bool(const char *value, bool default_value);
long parse_long(const char *value, long default_value);
long long parse_long_long(const char *value, long long default_value);
int parse_int(const char *value, int default_value);
unsigned int parse_time_period(const char *str, unsigned int default_value);

#if defined(FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION)
#include <stdint.h>
struct config *config_open_for_fuzzing(const uint8_t *data, size_t len);
#endif

#if defined(__cplusplus)
}
#endif
