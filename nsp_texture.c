// Texture module
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
#include <limits.h>
#include <stdbool.h>
#include <libndls.h>

static duk_ret_t nsp_texture_constructor(duk_context *ctx) {
    int width = duk_require_int(ctx, 0);
    int height = duk_require_int(ctx, 1);
    if (width < 1 || height < 1) {
	duk_push_error_object(ctx, DUK_ERR_RANGE_ERROR, "Width and height must be positive");
	duk_throw(ctx);
    }
    bool has_transparency;
    uint16_t transparent_color;
    if ((has_transparency = duk_is_number(ctx, 2))) {
	transparent_color = (uint16_t)duk_get_int(ctx, 2);
    }
    duk_push_this(ctx);
    duk_push_fixed_buffer(ctx, width * height * 2);
    duk_put_prop_string(ctx, -2, "bitmap");
    duk_push_int(ctx, width);
    duk_put_prop_string(ctx, -2, "width");
    duk_push_int(ctx, height);
    duk_put_prop_string(ctx, -2, "height");
    if (has_transparency) {
	duk_push_int(ctx, transparent_color);
    } else {
	duk_push_null(ctx);
    }
    duk_put_prop_string(ctx, -2, "transparentColor");
    return 0;
}

duk_ret_t nsp_texture_display(duk_context *ctx) {
    duk_push_this(ctx);
    duk_get_prop_string(ctx, -1, "width");
    int width = duk_get_int(ctx, -1);
    duk_pop(ctx);
    duk_get_prop_string(ctx, -1, "height");
    int height = duk_get_int(ctx, -1);
    duk_pop(ctx);
    duk_get_prop_string(ctx, -1, "transparentColor");
    if (width != 320 || height != 240 || !duk_is_null(ctx, -1)) {
	duk_push_error_object(ctx, DUK_ERR_RANGE_ERROR, "must have dimensions 230x240 without transparency");
	duk_throw(ctx);
    }
    duk_pop(ctx);
    duk_get_prop_string(ctx, -1, "bitmap");
    size_t size;
    uint16_t *bitmap;
    bitmap = duk_get_buffer(ctx, -1, &size);
    if (bitmap == NULL || size != 320 * 240 * 2) {
	duk_push_error_object(ctx, DUK_ERR_ERROR, "bitmap buffer does not match with dimensions");
	duk_throw(ctx);
    }
    memcpy(SCREEN_BASE_ADDRESS, bitmap, 320 * 240 * 2);
    return 0;
}

duk_ret_t nsp_texture_fill(duk_context *ctx) {
    uint16_t color = duk_require_int(ctx, 0);
    duk_push_this(ctx);
    duk_get_prop_string(ctx, -1, "bitmap");
    size_t size;
    uint16_t *bitmap = duk_get_buffer(ctx, -1, &size);
    if (bitmap == NULL) {
	duk_push_error_object(ctx, DUK_ERR_ERROR, "bitmap pointer is NULL");
	duk_throw(ctx);
    }
    for (size_t i = 0; i < size / 2; i++) {
	bitmap[i] = (uint16_t)color;
    }

    return 0;
}

duk_ret_t nsp_texture_get_pixel(duk_context *ctx) {
    int x = duk_require_int(ctx, 0);
    int y = duk_require_int(ctx, 1);
    duk_push_this(ctx);
    duk_get_prop_string(ctx, -1, "width");
    int w = duk_require_int(ctx, -1);
    duk_pop(ctx);
    duk_get_prop_string(ctx, -1, "height");
    int h = duk_require_int(ctx, -1);
    duk_pop(ctx);

    if (w <= 0 || h <= 0) {
	duk_push_error_object(ctx, DUK_ERR_RANGE_ERROR, "width and height must be positive");
	duk_throw(ctx);
    }

    duk_get_prop_string(ctx, -1, "bitmap");
    size_t size;
    uint16_t *bitmap = duk_get_buffer(ctx, -1, &size);
    if (bitmap == NULL) {
	duk_push_error_object(ctx, DUK_ERR_ERROR, "bitmap pointer is NULL");
	duk_throw(ctx);
    }

    if (0 <= x && x < w && 0 <= y && y < h && size >= (size_t)(w * h * 2)) {
	duk_push_int(ctx, bitmap[w * y + x]);
	return 1;
    } else {
	duk_push_error_object(ctx, DUK_ERR_RANGE_ERROR, "coordinates out of range");
	duk_throw(ctx);
    }
}

duk_ret_t nsp_texture_set_pixel(duk_context *ctx) {
    int x = duk_require_int(ctx, 0);
    int y = duk_require_int(ctx, 1);
    uint16_t color = duk_require_int(ctx, 2);
    duk_push_this(ctx);
    duk_get_prop_string(ctx, -1, "width");
    int w = duk_require_int(ctx, -1);
    duk_pop(ctx);
    duk_get_prop_string(ctx, -1, "height");
    int h = duk_require_int(ctx, -1);
    duk_pop(ctx);

    if (w <= 0 || h <= 0) {
	duk_push_error_object(ctx, DUK_ERR_RANGE_ERROR, "width and height must be positive");
	duk_throw(ctx);
    }

    duk_get_prop_string(ctx, -1, "bitmap");
    size_t size;
    uint16_t *bitmap = duk_get_buffer(ctx, -1, &size);
    if (bitmap == NULL) {
	duk_push_error_object(ctx, DUK_ERR_ERROR, "bitmap pointer is NULL");
	duk_throw(ctx);
    }

    if (0 <= x && x < w && 0 <= y && y < h && size >= (size_t)(w * h * 2)) {
	bitmap[w * y + x] = (uint16_t)color;
	return 0;
    } else {
	duk_push_error_object(ctx, DUK_ERR_RANGE_ERROR, "coordinates out of range");
	duk_throw(ctx);
    }
}

static const duk_function_list_entry nsp_texture_methods[] = {
    {"display", nsp_texture_display, 0},
    {"fill", nsp_texture_fill, 1},
    {"getPx", nsp_texture_get_pixel, 2},
    {"setPx", nsp_texture_set_pixel, 3},
    {NULL, NULL, 0}
};

duk_ret_t dukopen_nsp_texture(duk_context *ctx) {
    duk_idx_t idx = duk_push_object(ctx);
    duk_idx_t stats_constr = duk_push_c_function(ctx, nsp_texture_constructor, DUK_VARARGS);
    duk_idx_t stats_prot = duk_push_object(ctx);
    duk_put_function_list(ctx, stats_prot, nsp_texture_methods);
    duk_put_prop_string(ctx, stats_constr, "prototype");
    duk_put_prop_string(ctx, idx, "Texture");
    return 1;
}
