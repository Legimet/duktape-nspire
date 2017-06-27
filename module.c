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

#include "duktape.h"
#include "module.h"
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>

// Duktape.modSearch function, needed for loading modules with require()
duk_ret_t module_search(duk_context *ctx) {
	const char *id = duk_require_string(ctx, 0);

	// C modules: add functions to exports variable (3rd argument) and return undefined
	for (int i = 0; i < c_module_count; i++) {
		if (!strcmp(c_module_list[i].name, id)) {
			duk_push_c_function(ctx, c_module_list[i].init_func, 0);
			duk_call(ctx, 0);
			duk_enum(ctx, -1, 0);
			while(duk_next(ctx, -1, 1)) {
				duk_put_prop(ctx, 2);
			}
			duk_pop_2(ctx);
			return 0;
		}
	}

	// JS modules: return source code as a string
	// Read from file "modname.js.tns"
	int module_filename_len = strlen(id) + strlen(".js.tns") + 1;
	char *module_filename = malloc(module_filename_len);
	if (!module_filename) goto error;
	snprintf(module_filename, module_filename_len, "%s.js.tns", id);
	FILE *module_file = fopen(module_filename, "r");
	free(module_filename);
	if (!module_file) goto error;
	if (fseek(module_file, 0, SEEK_END) != 0) goto error;
	long module_file_size = ftell(module_file);
	if (module_file_size == -1) goto error;
	rewind(module_file);
	char *src = malloc(module_file_size);
	if (!src) goto error;
	fread(src, 1, module_file_size, module_file);
	if (ferror(module_file)) goto error;
	fclose(module_file);

	duk_push_lstring(ctx, src, module_file_size);
	free(src);

	return 1;

error:
	return duk_error(ctx, DUK_ERR_ERROR, "module %s not found: %s", id, strerror(errno));
}
