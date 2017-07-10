// Texture module
// Copyright (C) 2015 Legimet + fgalliat (Xtase)
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
#include <stdbool.h>
#include <libndls.h>


// =================================================
#include "framebuffer.h"
// =================================================


static duk_ret_t nsp_texture_constructor(duk_context *ctx) {
	int width = duk_require_int(ctx, 0);
	int height = duk_require_int(ctx, 1);
	if (width < 1 || height < 1) {
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "Width and height must be positive");
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
	if (width != 320 || height != 240 || !duk_is_null(ctx, -1))
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "must have dimensions 230x240 without transparency");
	duk_pop(ctx);
	duk_get_prop_string(ctx, -1, "bitmap");
	size_t size;
	uint16_t *bitmap;
	bitmap = duk_get_buffer(ctx, -1, &size);
	if (bitmap == NULL || size != 320 * 240 * 2)
		return duk_error(ctx, DUK_ERR_ERROR, "bitmap buffer does not match with dimensions");

	if (has_colors)
		lcd_blit(bitmap, SCR_320x240_565);
	else {
		uint8_t buf[240*320/2];
		uint8_t r[2], g[2], b[2], c[2];
		for (unsigned i = 0; i < 240; i++) {
			for (unsigned j = 0; j < 320/2; j++) {
				for (unsigned k = 0; k < 2; k++) {
					r[k] = bitmap[i*320+2*j+k] >> 12;
					g[k] = (bitmap[i*320+2*j+k] >> 7) & 0xF;
					b[k] = (bitmap[i*320+2*j+k] >> 1) & 0xF;
					c[k] = (30*r[k] + 59*g[k] + 11*b[k]) / 100;
				}
				buf[i*320/2+j] = (c[0] << 4) | c[1];
			}
		}
		lcd_blit(buf, SCR_320x240_4);
	}

	return 0;
}

duk_ret_t nsp_texture_fill(duk_context *ctx) {
	uint16_t color = duk_require_int(ctx, 0);
	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, "bitmap");
	size_t size;
	uint16_t *bitmap = duk_get_buffer(ctx, -1, &size);
	if (bitmap == NULL)
		return duk_error(ctx, DUK_ERR_ERROR, "bitmap pointer is NULL");
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

	if (w <= 0 || h <= 0)
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "width and height must be positive");

	duk_get_prop_string(ctx, -1, "bitmap");
	size_t size;
	uint16_t *bitmap = duk_get_buffer(ctx, -1, &size);
	if (bitmap == NULL)
		return duk_error(ctx, DUK_ERR_ERROR, "bitmap pointer is NULL");

	if (0 <= x && x < w && 0 <= y && y < h && size >= (size_t)(w * h * 2)) {
		duk_push_int(ctx, bitmap[w * y + x]);
		return 1;
	} else
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "coordinates out of range");
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

	if (w <= 0 || h <= 0)
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "width and height must be positive");

	duk_get_prop_string(ctx, -1, "bitmap");
	size_t size;
	uint16_t *bitmap = duk_get_buffer(ctx, -1, &size);
	if (bitmap == NULL)
		return duk_error(ctx, DUK_ERR_ERROR, "bitmap pointer is NULL");

	if (0 <= x && x < w && 0 <= y && y < h && size >= (size_t)(w * h * 2)) {
		bitmap[w * y + x] = (uint16_t)color;
		return 0;
	} else
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "coordinates out of range");
}

// ====== Xtase drawing routines ======
duk_ret_t nsp_texture_draw_line(duk_context *ctx) {
	int x1 = duk_require_int(ctx, 0);
	int y1 = duk_require_int(ctx, 1);

	int x2 = duk_require_int(ctx, 2);
	int y2 = duk_require_int(ctx, 3);

	uint16_t color = duk_require_int(ctx, 4);


	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, "width");
	int w = duk_require_int(ctx, -1);
	duk_pop(ctx);
	duk_get_prop_string(ctx, -1, "height");
	int h = duk_require_int(ctx, -1);
	duk_pop(ctx);

	if (w <= 0 || h <= 0)
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "width and height must be positive");

	duk_get_prop_string(ctx, -1, "bitmap");
	size_t size;
	uint16_t *bitmap = duk_get_buffer(ctx, -1, &size);
	if (bitmap == NULL)
		return duk_error(ctx, DUK_ERR_ERROR, "bitmap pointer is NULL");

	FbDev* fb = (FbDev*)malloc( 1 * sizeof(FbDev) );
	fb->width = w;
	fb->height = h;
	fb->fb_size = w * h;
	fb->fb = bitmap;
	drawLine(x1,y1,x2,y2,color,fb);
	free(fb);
	return 0;
}



duk_ret_t nsp_texture_draw_rect(duk_context *ctx) {
	int x = duk_require_int(ctx, 0);
	int y = duk_require_int(ctx, 1);

	int w_ = duk_require_int(ctx, 2);
	int h_ = duk_require_int(ctx, 3);

	uint16_t color = duk_require_int(ctx, 4);


	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, "width");
	int w = duk_require_int(ctx, -1);
	duk_pop(ctx);
	duk_get_prop_string(ctx, -1, "height");
	int h = duk_require_int(ctx, -1);
	duk_pop(ctx);

	if (w <= 0 || h <= 0)
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "width and height must be positive");

	duk_get_prop_string(ctx, -1, "bitmap");
	size_t size;
	uint16_t *bitmap = duk_get_buffer(ctx, -1, &size);
	if (bitmap == NULL)
		return duk_error(ctx, DUK_ERR_ERROR, "bitmap pointer is NULL");

	FbDev* fb = (FbDev*)malloc( 1 * sizeof(FbDev) );
	fb->width = w;
	fb->height = h;
	fb->fb_size = w * h;
	fb->fb = bitmap;
	drawRect(x,y,w_,h_,color,fb);
	free(fb);
	return 0;
}

duk_ret_t nsp_texture_fill_rect(duk_context *ctx) {
	int x = duk_require_int(ctx, 0);
	int y = duk_require_int(ctx, 1);

	int w_ = duk_require_int(ctx, 2);
	int h_ = duk_require_int(ctx, 3);

	uint16_t color = duk_require_int(ctx, 4);


	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, "width");
	int w = duk_require_int(ctx, -1);
	duk_pop(ctx);
	duk_get_prop_string(ctx, -1, "height");
	int h = duk_require_int(ctx, -1);
	duk_pop(ctx);

	if (w <= 0 || h <= 0)
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "width and height must be positive");

	duk_get_prop_string(ctx, -1, "bitmap");
	size_t size;
	uint16_t *bitmap = duk_get_buffer(ctx, -1, &size);
	if (bitmap == NULL)
		return duk_error(ctx, DUK_ERR_ERROR, "bitmap pointer is NULL");

	FbDev* fb = (FbDev*)malloc( 1 * sizeof(FbDev) );
	fb->width = w;
	fb->height = h;
	fb->fb_size = w * h;
	fb->fb = bitmap;
	fillRect(x,y,w_,h_,color,fb);
	free(fb);
	return 0;
}

int* getIntArray(duk_context *ctx, int stackIndex) {
	static int* result = NULL;

	if(duk_is_array(ctx, stackIndex)) {
		int resultLen = duk_get_length(ctx, stackIndex);

		result = (int*)malloc( resultLen * sizeof(int) );
		memset(result,0,resultLen);

		duk_enum(ctx, stackIndex, DUK_ENUM_ARRAY_INDICES_ONLY);


		// NOT stackIndex because waits enumIndex that is -1
		int idx=0;
		while (duk_next(ctx, -1, 1)) {
			// in JS/duktape toto[1] <=> toto["1"]
			// that's why keys are strings
			int v = duk_to_int(ctx, -1);
			//printf("key=%s, value=%d\n", k, v);
			result[idx++] = v;
			duk_pop_2(ctx);
		}
		duk_pop(ctx); // duk_enum

	} else {
		printf("Found NO array\n");
	}

	return result;
}



duk_ret_t nsp_texture_draw_polyLines(duk_context *ctx) {

	duk_require_object_coercible(ctx, 0);
	int* xys = getIntArray(ctx,0);

	int nbPts = duk_require_int(ctx, 1);

	uint16_t color = duk_require_int(ctx, 2);


	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, "width");
	int w = duk_require_int(ctx, -1);
	duk_pop(ctx);

	duk_get_prop_string(ctx, -1, "height");
	int h = duk_require_int(ctx, -1);
	duk_pop(ctx);

	if (w <= 0 || h <= 0)
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "width and height must be positive");

	duk_get_prop_string(ctx, -1, "bitmap");
	size_t size;
	uint16_t *bitmap = duk_get_buffer(ctx, -1, &size);
	if (bitmap == NULL)
		return duk_error(ctx, DUK_ERR_ERROR, "bitmap pointer is NULL");

	FbDev* fb = (FbDev*)malloc( 1 * sizeof(FbDev) );
	fb->width = w;
	fb->height = h;
	fb->fb_size = w * h;
	fb->fb = bitmap;
	drawPolyLines( xys, nbPts, color, fb );
	free(fb);
	return 0;
}

duk_ret_t nsp_texture_fill_polygon(duk_context *ctx) {

	duk_require_object_coercible(ctx, 0);
	int* xys = getIntArray(ctx,0);

	int nbPts = duk_require_int(ctx, 1);

	uint16_t color = duk_require_int(ctx, 2);


	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, "width");
	int w = duk_require_int(ctx, -1);
	duk_pop(ctx);

	duk_get_prop_string(ctx, -1, "height");
	int h = duk_require_int(ctx, -1);
	duk_pop(ctx);

	if (w <= 0 || h <= 0)
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "width and height must be positive");

	duk_get_prop_string(ctx, -1, "bitmap");
	size_t size;
	uint16_t *bitmap = duk_get_buffer(ctx, -1, &size);
	if (bitmap == NULL)
		return duk_error(ctx, DUK_ERR_ERROR, "bitmap pointer is NULL");

	FbDev* fb = (FbDev*)malloc( 1 * sizeof(FbDev) );
	fb->width = w;
	fb->height = h;
	fb->fb_size = w * h;
	fb->fb = bitmap;
	fillPolygon( xys, nbPts, color, fb );
	free(fb);
	return 0;
}

duk_ret_t nsp_texture_draw_circle(duk_context *ctx) {
	int x = duk_require_int(ctx, 0);
	int y = duk_require_int(ctx, 1);

	int radius = duk_require_int(ctx, 2);
	uint16_t color = duk_require_int(ctx, 3);


	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, "width");
	int w = duk_require_int(ctx, -1);
	duk_pop(ctx);
	duk_get_prop_string(ctx, -1, "height");
	int h = duk_require_int(ctx, -1);
	duk_pop(ctx);

	if (w <= 0 || h <= 0)
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "width and height must be positive");

	duk_get_prop_string(ctx, -1, "bitmap");
	size_t size;
	uint16_t *bitmap = duk_get_buffer(ctx, -1, &size);
	if (bitmap == NULL)
		return duk_error(ctx, DUK_ERR_ERROR, "bitmap pointer is NULL");

	FbDev* fb = (FbDev*)malloc( 1 * sizeof(FbDev) );
	fb->width = w;
	fb->height = h;
	fb->fb_size = w * h;
	fb->fb = bitmap;
	drawCircle(x,y,radius,color,fb);
	free(fb);
	return 0;
}

duk_ret_t nsp_texture_fill_circle(duk_context *ctx) {
	int x = duk_require_int(ctx, 0);
	int y = duk_require_int(ctx, 1);

	int radius = duk_require_int(ctx, 2);
	uint16_t color = duk_require_int(ctx, 3);


	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, "width");
	int w = duk_require_int(ctx, -1);
	duk_pop(ctx);
	duk_get_prop_string(ctx, -1, "height");
	int h = duk_require_int(ctx, -1);
	duk_pop(ctx);

	if (w <= 0 || h <= 0)
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "width and height must be positive");

	duk_get_prop_string(ctx, -1, "bitmap");
	size_t size;
	uint16_t *bitmap = duk_get_buffer(ctx, -1, &size);
	if (bitmap == NULL)
		return duk_error(ctx, DUK_ERR_ERROR, "bitmap pointer is NULL");

	FbDev* fb = (FbDev*)malloc( 1 * sizeof(FbDev) );
	fb->width = w;
	fb->height = h;
	fb->fb_size = w * h;
	fb->fb = bitmap;
	fillCircle(x,y,radius,color,fb);
	free(fb);
	return 0;
}

// ======/Xtase drawing routines/======


static const duk_function_list_entry nsp_texture_methods[] = {
	{"display", nsp_texture_display, 0},
	{"fill", nsp_texture_fill, 1},
	{"getPx", nsp_texture_get_pixel, 2},
	{"setPx", nsp_texture_set_pixel, 3},

	// Xtase drawing routines
	{"drawLine", nsp_texture_draw_line, 5},
	{"drawRect", nsp_texture_draw_rect, 5},
	{"fillRect", nsp_texture_fill_rect, 5},

	{"drawPolyLines", nsp_texture_draw_polyLines, 3},
	{"fillPolygon", nsp_texture_fill_polygon, 3},

	{"drawCircle", nsp_texture_draw_circle, 4},
	{"fillCircle", nsp_texture_fill_circle, 4},

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
