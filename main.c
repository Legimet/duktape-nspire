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
#include "module.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libndls.h>

duk_context *ctx;

// Push stack of Error object at -1, remove the Error object
duk_ret_t get_error_stack(duk_context *ctx, __attribute__((unused)) void *udata) {
	if (duk_is_error(ctx, -1) && duk_has_prop_string(ctx, -1, "stack")) {
		duk_get_prop_string(ctx, -1, "stack");
		duk_remove(ctx, -2);
	}
	return 1;
}

// Print stack of Error, remove the Error object
void print_pop_error(void) {
	duk_safe_call(ctx, get_error_stack, NULL, 1, 1);
	fprintf(stderr, "%s\n", duk_safe_to_string(ctx, -1));
	duk_pop(ctx);
}

// Run JS file
int handle_file(char *path) {
	if (duk_pcompile_file(ctx, 0, path)) {
		return 1;
	}
	duk_push_global_object(ctx);
	if (duk_pcall_method(ctx, 0)) {
		return 1;
	}
	return 0;
}

// REPL
int handle_repl(void) {
	char input[256];

	puts("Duktape-nspire " VERSION "\nBuilt on " BUILD_DATE ", using Duktape " DUK_GIT_DESCRIBE);

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
			print_pop_error();
			continue;
		}
		duk_push_global_object(ctx);
		if (duk_pcall_method(ctx, 0)) {
			print_pop_error();
			continue;
		}
		puts(duk_safe_to_string(ctx, -1));
	}
	return 0;
}

// Cleanup at end
void cleanup(void) {
	duk_destroy_heap(ctx);
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
	ctx = duk_create_heap_default();
	if (atexit(cleanup)) {
		cleanup();
		return EXIT_FAILURE;
	}

	// Add modSearch function for module loading support
	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Duktape");
	duk_push_c_function(ctx, module_search, 4);
	duk_put_prop_string(ctx, -2, "modSearch");
	duk_pop_2(ctx);

	if (argc <= 1) {
		if (handle_repl()) {
			wait_key_pressed();
			return EXIT_FAILURE;
		}
	} else {
		for (int i = 1; i < argc; i++) {
			duk_push_string(ctx, argv[i]);
			if (handle_file(argv[i])) {
				print_pop_error();
				wait_key_pressed();
				return EXIT_FAILURE;
			}
		}
	}

	return 0;
}
