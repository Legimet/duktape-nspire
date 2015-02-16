#include "duktape.h"
#include "module.h"
#include <stdbool.h>
#include <stdio.h>

duk_ret_t module_search(duk_context *ctx) {
    const char *id = duk_require_string(ctx, 0);
    bool found = false;

    for (int i = 0; i < c_module_count; i++) {
	if (!strcmp(c_module_list[i].name, id)) {
	    duk_push_c_function(ctx, c_module_list[i].init_func, 0);
	    duk_call(ctx, 0);
	    duk_enum(ctx, -1, 0);
	    while(duk_next(ctx, -1, 1)) {
		duk_put_prop(ctx, 2);
	    }
	    duk_pop_2(ctx);
	    found = true;
	    break;
	}
    }

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
    if (!found) {
	duk_push_string(ctx, "module not found:");
	duk_throw(ctx);
    }

    return 0;
}
