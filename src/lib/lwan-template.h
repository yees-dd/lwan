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
#pragma once

#include "lwan-coro.h"
#include "lwan-strbuf.h"
#include <stddef.h>

enum lwan_tpl_flag { LWAN_TPL_FLAG_CONST_TEMPLATE = 1 << 0 };

struct lwan_var_descriptor {
    const char *name;
    const off_t offset;

    void (*append_to_strbuf)(struct lwan_strbuf *buf, void *ptr);
    bool (*get_is_empty)(void *ptr);

    coro_function_t generator;
    const struct lwan_var_descriptor *list_desc;
};

#define TPL_VAR_SIMPLE(var_, append_to_lwan_strbuf_, get_is_empty_)            \
    {                                                                          \
        .name = #var_, .offset = offsetof(TPL_STRUCT, var_),                   \
        .append_to_strbuf = append_to_lwan_strbuf_,                            \
        .get_is_empty = get_is_empty_                                          \
    }

#define TPL_VAR_SEQUENCE(var_, generator_, seqitem_desc_)                      \
    {                                                                          \
        .name = #var_, .offset = offsetof(TPL_STRUCT, var_.generator),         \
        .generator = generator_, .list_desc = seqitem_desc_                    \
    }

#define TPL_VAR_INT(var_)                                                      \
    TPL_VAR_SIMPLE(var_, lwan_append_int_to_strbuf, lwan_tpl_int_is_empty)

#define TPL_VAR_DOUBLE(var_)                                                   \
    TPL_VAR_SIMPLE(var_, lwan_append_double_to_strbuf, lwan_tpl_double_is_empty)

#define TPL_VAR_STR(var_)                                                      \
    TPL_VAR_SIMPLE(var_, lwan_append_str_to_strbuf, lwan_tpl_str_is_empty)

#define TPL_VAR_STR_ESCAPE(var_)                                               \
    TPL_VAR_SIMPLE(var_, lwan_append_str_escaped_to_strbuf,                    \
                   lwan_tpl_str_is_empty)

#define TPL_VAR_SENTINEL                                                       \
    {                                                                          \
        NULL, 0, NULL, NULL, NULL, NULL                                        \
    }

/*
 * These functions are not meant to be used directly. We do need a pointer to
 * them, though, that's why they're exported. Eventually this will move to
 * something more opaque.
 */
void lwan_append_int_to_strbuf(struct lwan_strbuf *buf, void *ptr);
bool lwan_tpl_int_is_empty(void *ptr);
void lwan_append_str_to_strbuf(struct lwan_strbuf *buf, void *ptr);
void lwan_append_str_escaped_to_strbuf(struct lwan_strbuf *buf, void *ptr);
bool lwan_tpl_str_is_empty(void *ptr);
void lwan_append_double_to_strbuf(struct lwan_strbuf *buf, void *ptr);
bool lwan_tpl_double_is_empty(void *ptr);

struct lwan_tpl *
lwan_tpl_compile_string_full(const char *string,
                             const struct lwan_var_descriptor *descriptor,
                             enum lwan_tpl_flag flags);
struct lwan_tpl *
lwan_tpl_compile_string(const char *string,
                        const struct lwan_var_descriptor *descriptor);
struct lwan_tpl *
lwan_tpl_compile_file(const char *filename,
                      const struct lwan_var_descriptor *descriptor);
struct lwan_strbuf *lwan_tpl_apply(struct lwan_tpl *tpl, void *variables);
bool lwan_tpl_apply_with_buffer(struct lwan_tpl *tpl,
                                struct lwan_strbuf *buf,
                                void *variables);
void lwan_tpl_free(struct lwan_tpl *tpl);
