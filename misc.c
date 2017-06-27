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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

duk_ret_t push_file_contents(duk_context *ctx, void *filename) {
	FILE *file = fopen(filename, "rb");
	if (!file) goto error;
	if (fseek(file, 0, SEEK_END)) goto error;
	long size = ftell(file);
	if (size < 0) goto error;
	if (size == 0) {
		fclose(file);
		duk_push_lstring(ctx, NULL, 0);
		return 1;
	}
	if (fseek(file, 0, SEEK_SET)) goto error;
	char *buf = duk_push_fixed_buffer(ctx, size);
	if (fread(buf, size, 1, file) != 1) {
		duk_pop(ctx);
		goto error;
	}
	fclose(file);
	duk_buffer_to_string(ctx, -1);
	return 1;

error:
	if (file)
		fclose(file);
	return duk_error(ctx, DUK_ERR_TYPE_ERROR, "Unable to read %s: %s", filename, strerror(errno));
}
