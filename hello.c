#include "duktape.h"

duk_ret_t hello_world(duk_context *ctx) {
    duk_push_global_object(ctx);
    duk_get_prop_string(ctx, -1, "print");
    duk_push_string(ctx, "Hello world!");
    duk_call(ctx, 1);
    duk_pop_2(ctx);
    return 0;
}

const duk_function_list_entry hello_funcs[] = {
    {"helloWorld", hello_world, 0},
    {NULL, NULL, 0}
};

duk_ret_t dukopen_hello(duk_context *ctx) {
    duk_push_object(ctx);
    duk_put_function_list(ctx, -1, hello_funcs);
    return 1;
}
