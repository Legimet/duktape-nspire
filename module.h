#ifndef MODULE_H
#define MODULE_H

#include "duktape.h"
#include "hello.h"

struct c_module {
    const char *name;
    duk_ret_t (*init_func)(duk_context*);
    const char **deps;
    const int deps_count;
};

static const struct c_module c_module_list[] = {
    {"hello", dukopen_hello, NULL, 0},
    {NULL, NULL, NULL, 0}
};

static const int c_module_count = (sizeof(c_module_list) / sizeof(c_module_list[0])) - 1;

duk_ret_t module_search(duk_context *ctx);

#endif
