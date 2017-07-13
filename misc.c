// Miscellaneous functions
// Copyright (C) 2017 Legimet
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
#include <errno.h>
#include <string.h>
#include <stdio.h>

void push_file_contents(duk_context *ctx, const char *filename) {
	FILE *file = fopen(filename, "rb");
	if (!file) goto error;
	if (fseek(file, 0, SEEK_END)) goto error;
	long size = ftell(file);
	if (size < 0) goto error;
	if (size == 0) {
		fclose(file);
		duk_push_lstring(ctx, NULL, 0);
		return;
	}
	if (fseek(file, 0, SEEK_SET)) goto error;
	char *buf = duk_push_fixed_buffer(ctx, size);
	if (fread(buf, size, 1, file) != 1) {
		duk_pop(ctx);
		goto error;
	}
	fclose(file);
	duk_buffer_to_string(ctx, -1);
	return;

error:
	if (file)
		fclose(file);
	(void) duk_error(ctx, DUK_ERR_TYPE_ERROR, "Unable to read '%s': %s", filename, strerror(errno));
}

static duk_ret_t push_file_contents_wrapper(duk_context *ctx, void *filename) {
	push_file_contents(ctx, filename);
	return 1;
}

duk_ret_t push_file_contents_safe(duk_context *ctx, char *filename) {
	return duk_safe_call(ctx, push_file_contents_wrapper, filename, 0, 1);
}

struct args {const char *str; duk_size_t len;};

static duk_ret_t safe_push_lstring(duk_context *ctx, void *udata) {
	duk_push_lstring(ctx, ((struct args*)udata)->str, ((struct args*)udata)->len);
	return 1;
}

duk_ret_t console_readline(duk_context *ctx) {
	size_t n = 0;
	char *line = NULL;
	ssize_t len;
	if ((len = __getline(&line, &n, stdin)) == -1) {
		free(line);
		goto failure;
	}
	struct args udata = {line, len - 1};
	if (duk_safe_call(ctx, safe_push_lstring, &udata, 0, 1)) {
		free(line);
		return duk_throw(ctx);
	}
	return 1;

failure:
	return duk_generic_error(ctx, "Unable to read line: %s", strerror(errno));
}
