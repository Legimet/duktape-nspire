// Keys module
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
#include <libndls.h>

struct nsp_key {
    const char *key_name;
    const t_key *key_val;
};

#define KEY(a) {#a, &a}

static const struct nsp_key nsp_key_consts[] = {
    KEY(KEY_NSPIRE_RET),
    KEY(KEY_NSPIRE_ENTER),
    KEY(KEY_NSPIRE_SPACE),
    KEY(KEY_NSPIRE_NEGATIVE),
    KEY(KEY_NSPIRE_Z),
    KEY(KEY_NSPIRE_PERIOD),
    KEY(KEY_NSPIRE_Y),
    KEY(KEY_NSPIRE_0),
    KEY(KEY_NSPIRE_X),
    KEY(KEY_NSPIRE_THETA),
    KEY(KEY_NSPIRE_COMMA),
    KEY(KEY_NSPIRE_PLUS),
    KEY(KEY_NSPIRE_W),
    KEY(KEY_NSPIRE_3),
    KEY(KEY_NSPIRE_V),
    KEY(KEY_NSPIRE_2),
    KEY(KEY_NSPIRE_U),
    KEY(KEY_NSPIRE_1),
    KEY(KEY_NSPIRE_T),
    KEY(KEY_NSPIRE_eEXP),
    KEY(KEY_NSPIRE_PI),
    KEY(KEY_NSPIRE_QUES),
    KEY(KEY_NSPIRE_QUESEXCL),
    KEY(KEY_NSPIRE_MINUS),
    KEY(KEY_NSPIRE_S),
    KEY(KEY_NSPIRE_6),
    KEY(KEY_NSPIRE_R),
    KEY(KEY_NSPIRE_5),
    KEY(KEY_NSPIRE_Q),
    KEY(KEY_NSPIRE_4),
    KEY(KEY_NSPIRE_P),
    KEY(KEY_NSPIRE_TENX),
    KEY(KEY_NSPIRE_EE),
    KEY(KEY_NSPIRE_COLON),
    KEY(KEY_NSPIRE_MULTIPLY),
    KEY(KEY_NSPIRE_O),
    KEY(KEY_NSPIRE_9),
    KEY(KEY_NSPIRE_N),
    KEY(KEY_NSPIRE_8),
    KEY(KEY_NSPIRE_M),
    KEY(KEY_NSPIRE_7),
    KEY(KEY_NSPIRE_L),
    KEY(KEY_NSPIRE_SQU),
    KEY(KEY_NSPIRE_II),
    KEY(KEY_NSPIRE_QUOTE),
    KEY(KEY_NSPIRE_DIVIDE),
    KEY(KEY_NSPIRE_K),
    KEY(KEY_NSPIRE_TAN),
    KEY(KEY_NSPIRE_J),
    KEY(KEY_NSPIRE_COS),
    KEY(KEY_NSPIRE_I),
    KEY(KEY_NSPIRE_SIN),
    KEY(KEY_NSPIRE_H),
    KEY(KEY_NSPIRE_EXP),
    KEY(KEY_NSPIRE_GTHAN),
    KEY(KEY_NSPIRE_APOSTROPHE),
    KEY(KEY_NSPIRE_CAT),
    KEY(KEY_NSPIRE_FRAC),
    KEY(KEY_NSPIRE_G),
    KEY(KEY_NSPIRE_RP),
    KEY(KEY_NSPIRE_F),
    KEY(KEY_NSPIRE_LP),
    KEY(KEY_NSPIRE_E),
    KEY(KEY_NSPIRE_VAR),
    KEY(KEY_NSPIRE_D),
    KEY(KEY_NSPIRE_DEL),
    KEY(KEY_NSPIRE_LTHAN),
    KEY(KEY_NSPIRE_FLAG),
    KEY(KEY_NSPIRE_CLICK),
    KEY(KEY_NSPIRE_C),
    KEY(KEY_NSPIRE_HOME),
    KEY(KEY_NSPIRE_B),
    KEY(KEY_NSPIRE_MENU),
    KEY(KEY_NSPIRE_A),
    KEY(KEY_NSPIRE_ESC),
    KEY(KEY_NSPIRE_BAR),
    KEY(KEY_NSPIRE_TAB),
    KEY(KEY_NSPIRE_EQU),
    KEY(KEY_NSPIRE_UP),
    KEY(KEY_NSPIRE_UPRIGHT),
    KEY(KEY_NSPIRE_RIGHT),
    KEY(KEY_NSPIRE_RIGHTDOWN),
    KEY(KEY_NSPIRE_DOWN),
    KEY(KEY_NSPIRE_DOWNLEFT),
    KEY(KEY_NSPIRE_LEFT),
    KEY(KEY_NSPIRE_LEFTUP),
    KEY(KEY_NSPIRE_SHIFT),
    KEY(KEY_NSPIRE_CTRL),
    KEY(KEY_NSPIRE_DOC),
    KEY(KEY_NSPIRE_TRIG),
    KEY(KEY_NSPIRE_SCRATCHPAD),
    KEY(KEY_84_DOWN),
    KEY(KEY_84_LEFT),
    KEY(KEY_84_RIGHT),
    KEY(KEY_84_UP),
    KEY(KEY_84_ENTER),
    KEY(KEY_84_PLUS),
    KEY(KEY_84_MINUS),
    KEY(KEY_84_MULTIPLY),
    KEY(KEY_84_DIVIDE),
    KEY(KEY_84_EXP),
    KEY(KEY_84_CLEAR),
    KEY(KEY_84_NEGATIVE),
    KEY(KEY_84_3),
    KEY(KEY_84_6),
    KEY(KEY_84_9),
    KEY(KEY_84_RP),
    KEY(KEY_84_TAN),
    KEY(KEY_84_VARS),
    KEY(KEY_84_PERIOD),
    KEY(KEY_84_2),
    KEY(KEY_84_5),
    KEY(KEY_84_8),
    KEY(KEY_84_LP),
    KEY(KEY_84_COS),
    KEY(KEY_84_PRGM),
    KEY(KEY_84_STAT),
    KEY(KEY_84_0),
    KEY(KEY_84_1),
    KEY(KEY_84_4),
    KEY(KEY_84_7),
    KEY(KEY_84_COMMA),
    KEY(KEY_84_SIN),
    KEY(KEY_84_APPS),
    KEY(KEY_84_X),
    KEY(KEY_84_STO),
    KEY(KEY_84_LN),
    KEY(KEY_84_LOG),
    KEY(KEY_84_SQU),
    KEY(KEY_84_INV),
    KEY(KEY_84_MATH),
    KEY(KEY_84_ALPHA),
    KEY(KEY_84_GRAPH),
    KEY(KEY_84_TRACE),
    KEY(KEY_84_ZOOM),
    KEY(KEY_84_WIND),
    KEY(KEY_84_YEQU),
    KEY(KEY_84_2ND),
    KEY(KEY_84_MODE),
    KEY(KEY_84_DEL)
};

static duk_ret_t nsp_keys_any_key_pressed(__attribute__((unused)) duk_context *ctx) {
    duk_push_boolean(ctx, any_key_pressed());
    return 1;
}

static duk_ret_t nsp_keys_is_key_pressed(duk_context *ctx) {
    int k = duk_require_int(ctx, 0);
    if (k < 0 || (unsigned int)k >= sizeof(nsp_key_consts) / sizeof(struct nsp_key)) {
	duk_push_error_object(ctx, DUK_ERR_ERROR, "invalid key: %d", k);
	duk_throw(ctx);
    }
    duk_push_boolean(ctx, isKeyPressed(*(nsp_key_consts[k].key_val)));
    return 1;
}

static duk_ret_t nsp_keys_on_key_pressed(__attribute__((unused)) duk_context *ctx) {
    duk_push_boolean(ctx, on_key_pressed());
    return 1;
}

static duk_ret_t nsp_keys_wait_key_pressed(__attribute__((unused)) duk_context *ctx) {
    wait_key_pressed();
    return 0;
}

static duk_ret_t nsp_keys_wait_no_key_pressed(__attribute__((unused)) duk_context *ctx) {
    wait_no_key_pressed();
    return 0;
}

static const duk_function_list_entry nsp_keys_funcs[] = {
    {"anyKeyPressed", nsp_keys_any_key_pressed, 0},
    {"isKeyPressed", nsp_keys_is_key_pressed, 1},
    {"onKeyPressed", nsp_keys_on_key_pressed, 0},
    {"waitKeyPressed", nsp_keys_wait_key_pressed, 0},
    {"waitNoKeyPressed", nsp_keys_wait_no_key_pressed, 0},
    {NULL, NULL, 0 }
};

duk_ret_t dukopen_nsp_keys(duk_context *ctx) {
    duk_idx_t idx = duk_push_object(ctx);
    duk_put_function_list(ctx, idx, nsp_keys_funcs);
    for (unsigned int i = 0; i < sizeof(nsp_key_consts) / sizeof(struct nsp_key); i++) {
	duk_push_int(ctx, i);
	duk_put_prop_string(ctx, idx, nsp_key_consts[i].key_name);
    }
    return 1;
}
