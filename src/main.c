#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct_string.h"

#define lit_strncmp(buff, lit) strncmp(buff, lit, sizeof lit - 1) == 0

int main(const int argc, const char **argv) {
	if (argc <= 1) {
		printf("Require more arguments.\n");
		return 1;
	}

	struct string *args[argc - 1];
	for (int index = 1; index < argc; ++index) {
		const size_t curr_arg_len = strnlen_s(argv[index], SIZE_MAX);
		if (curr_arg_len == SIZE_MAX) {
			return 1;
		}

		args[index - 1] = struct_string_new(argv[index], curr_arg_len);
	}

	for (size_t index = 0; index < sizeof args / sizeof args[0]; ++index) {
		if (lit_strncmp(args[index] -> buff, "--help") || lit_strncmp(args[index] -> buff, "-h")) {
			// TODO: Generate the help menu.
			printf("The help menu will be available soon.\n");
			break;
		} else if (lit_strncmp(args[index] -> buff, "--version") || lit_strncmp(args[index] -> buff, "-v")) {
			printf("Currently at version 0.0.1.\n");
			break;
		}
		// TODO: Create the last input for taking actual directory names.
	}

	// Cleanup.
	for (size_t index = 0; index < sizeof args / sizeof args[0]; ++index) {
		struct_string_del(args[index]);
	}

	return 0;
}
