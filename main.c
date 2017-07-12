// Main function
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
#include "duk_module_node.h"
#include "duk_print_alert.h"
#include "duk_console.h"
#include "module.h"
#include "misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libndls.h>

static duk_context *ctx;

// Push stack of Error object at -1, remove the Error object
static duk_ret_t get_error_stack(duk_context *ctx, __attribute__((unused)) void *udata) {
	if (duk_is_error(ctx, 0) && duk_has_prop_string(ctx, 0, "stack")) {
		duk_get_prop_string(ctx, 0, "stack");
		duk_remove(ctx, -2);
	}
	return 1;
}

// Print stack of Error
static void print_error(void) {
	duk_safe_call(ctx, get_error_stack, NULL, 1, 1);
	fprintf(stderr, "%s\n", duk_safe_to_string(ctx, -1));
	duk_pop(ctx);
}

// Run JS file
static int handle_file(char *path) {
	if (push_file_contents_safe(ctx, path)) {
		print_error();
		return 1;
	}
	if (duk_module_node_peval_main(ctx, path)) {
		print_error();
		return 1;
	}
	return 0;
}

// REPL
static int handle_repl(void) {
	char input[256];

	puts("Duktape-nspire " VERSION "\n"
	     "Built on " BUILD_DATE " with Duktape " DUK_GIT_DESCRIBE);

	while (true) {
		printf("> ");
		fflush(stdout);
		if (!fgets(input, 256, stdin)) {
			if (ferror(stdin)) {
				fprintf(stderr, "unable to read input\n");
				return 1;
			} else {
				return 0;
			}
		}
		if (!strcmp(input, ".exit\n")) {
			return 0;
		}
		if (duk_pcompile_string(ctx, 0, input)) {
			print_error();
			continue;
		}
		duk_push_global_object(ctx);
		if (duk_pcall_method(ctx, 0)) {
			print_error();
			continue;
		}
		puts(duk_safe_to_string(ctx, -1));
	}
	return 0;
}

static void fatal_error_handler(__attribute__((unused)) void *udata, const char *msg) {
	fputs("*** FATAL ERROR", stderr);
	if (msg) {
		fputs(": ", stderr);
		fputs(msg, stderr);
	}
	fputc('\n', stderr);
	wait_no_key_pressed();
	abort();
}

int main(int argc, char **argv) {
	enable_relative_paths(argv); // Enable relative paths

	// Register .js file extension
	if (argc > 0) {
		char *ptr = strrchr(argv[0], '/');
		if (ptr++) {
			char *dot = strrchr(ptr, '.');
			if (dot && dot != ptr && !strcmp(dot, ".tns")) {
				*dot = '\0';
				cfg_register_fileext("js", ptr);
				*dot = '.';
			}
		}
	}

	// Initialize Duktape heap and register cleanup with atexit
	ctx = duk_create_heap(NULL, NULL, NULL, NULL, fatal_error_handler);
	if (!ctx) {
		fputs("Failed to create heap\n", stderr);
		return EXIT_FAILURE;
	}

	// Module system - register the resolve and load functions
	duk_push_object(ctx);
	duk_push_c_function(ctx, cb_resolve_module, DUK_VARARGS);
	duk_put_prop_string(ctx, -2, "resolve");
	duk_push_c_function(ctx, cb_load_module, DUK_VARARGS);
	duk_put_prop_string(ctx, -2, "load");
	duk_module_node_init(ctx);

	duk_print_alert_init(ctx, 0); // Needed for print/alert
	duk_console_init(ctx, 0); // console binding

	int ret = EXIT_SUCCESS;
	if (argc <= 1) {
		if (handle_repl()) {
			wait_key_pressed();
			ret = EXIT_FAILURE;
		}
	} else {
		for (int i = 1; i < argc; i++) {
			if (handle_file(argv[i])) {
				wait_key_pressed();
				ret = EXIT_FAILURE;
				break;
			}
		}
	}

	duk_destroy_heap(ctx);

	return ret;
}
