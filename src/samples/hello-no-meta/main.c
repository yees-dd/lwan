/*
 * lwan - web server
 * Copyright (c) 2018 L. A. F. Pereira <l@tia.mat.br>
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

#include "lwan.h"

/* Defining a handler like this will make it invisible for configuration
 * files, but it can be referenced by a url_map and will work just the
 * same.  */
static enum lwan_http_status hello_world(struct lwan_request *request
                                         __attribute__((unused)),
                                         struct lwan_response *response,
                                         void *data __attribute__((unused)))
{
    static const char message[] = "Hello, World!";

    response->mime_type = "text/plain";
    lwan_strbuf_set_static(response->buffer, message, sizeof(message) - 1);

    return HTTP_OK;
}

int main(void)
{
    const struct lwan_url_map default_map[] = {
        {.prefix = "/", .handler = hello_world},
        {.prefix = NULL}
    };
    struct lwan l;

    lwan_init(&l);

    lwan_set_url_map(&l, default_map);
    lwan_main_loop(&l);

    lwan_shutdown(&l);

    return 0;
}
