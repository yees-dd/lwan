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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#pragma once

#include "lwan.h"

struct lwan_lua_settings {
    const char *default_type;
    const char *script_file;
    const char *script;
    unsigned int cache_period;
};

LWAN_MODULE_FORWARD_DECL(lua);

#define LUA(default_type_) \
    .module = LWAN_MODULE_REF(lua), \
    .args = ((struct lwan_lua_settings[]) {{ \
        .default_type = default_type_ \
    }}), \
    .flags = 0
