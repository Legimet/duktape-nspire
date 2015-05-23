// Module search function
// Copyright (C) 2015 Legimet
//
// This file is part of Duktape-nspire.
//
// Duktape-nspire is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Duktape-nspire is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Duktape-nspire. If not, see <http://www.gnu.org/licenses/>.

#ifndef MODULE_H
#define MODULE_H

#include "duktape.h"
#include "fs.h"
#include "nsp_keys.h"
#include "nsp_texture.h"

struct c_module {
    const char *name;
    duk_ret_t (*init_func)(duk_context*);
};

static const struct c_module c_module_list[] = {
    {"fs", dukopen_fs},
    {"nsp/keys", dukopen_nsp_keys},
    {"nsp/texture", dukopen_nsp_texture}
};

static const int c_module_count = (sizeof(c_module_list) / sizeof(c_module_list[0]));

duk_ret_t module_search(duk_context *ctx);

#endif
