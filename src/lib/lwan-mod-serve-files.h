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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#pragma once

#if defined (__cplusplus)
extern "C" {
#endif

#include "lwan.h"

#define SERVE_FILES_READ_AHEAD_BYTES (128 * 1024)
#define SERVE_FILES_CACHE_FOR 5

struct lwan_serve_files_settings {
  const char *root_path;
  const char *index_html;
  const char *directory_list_template;
  size_t read_ahead;
  time_t cache_for;
  bool serve_precompressed_files;
  bool auto_index;
  bool auto_index_readme;
};

LWAN_MODULE_FORWARD_DECL(serve_files);

#define SERVE_FILES_SETTINGS(root_path_, index_html_, serve_precompressed_files_) \
  .module = LWAN_MODULE_REF(serve_files), \
  .args = ((struct lwan_serve_files_settings[]) {{ \
    .read_ahead = SERVE_FILES_READ_AHEAD_BYTES, \
    .root_path = root_path_, \
    .index_html = index_html_, \
    .serve_precompressed_files = serve_precompressed_files_, \
    .directory_list_template = NULL, \
    .auto_index = true, \
    .auto_index_readme = true, \
    .cache_for = SERVE_FILES_CACHE_FOR, \
  }}), \
  .flags = (enum lwan_handler_flags)0

#define SERVE_FILES(root_path) \
  SERVE_FILES_SETTINGS(root_path, NULL, true)

#if defined (__cplusplus)
}
#endif
