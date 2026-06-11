#include "struct_string.h"
#include <direct.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define len_from_sizeof(arr) sizeof arr / sizeof arr[0]
#define lit_strncmp(buff, lit) strncmp(buff, lit, sizeof lit - 1) == 0
#define error() printf("\x1B[31mSome error occurred\n\x1B[39m");

int main(const int argc, const char **argv) {
	if (argc <= 1) {
		printf("Require more arguments.\n");
		return 1;
	}

	struct string *args[argc - 1];
	// Initializing array independently.
	// This is so that when we try to create strings, if we have an error, the
	// memory isn't corrupted because we freed random addresses.
	for (size_t index = 0; index < len_from_sizeof(args); ++index) {
		args[index] = nullptr;
	}

	for (size_t index = 0; index < len_from_sizeof(args); ++index) {
		const size_t curr_arg_len = strnlen_s(argv[index + 1], SIZE_MAX);
		if (curr_arg_len == SIZE_MAX) {
			// TODO: Handle errors here.
			error();
			return 1;
		}

		args[index] = struct_string_new(argv[index + 1], curr_arg_len);
		if (args[index] == nullptr) {
			goto cleanup;
		}
	}

	// This `const` qualifier won't let us free `template_dir` since
	// `template_dir` is acting as a readable string only.
	const struct string *template_dir = nullptr;
	for (size_t index = 0; index < len_from_sizeof(args); ++index) {
		if (lit_strncmp(args[index]->buff, "--help") ||
			lit_strncmp(args[index]->buff, "-h")) {
			// TODO: Generate the help menu.
			printf("The help menu will be available soon.\n");
			break;
		} else if (lit_strncmp(args[index]->buff, "--version") ||
				   lit_strncmp(args[index]->buff, "-v")) {
			printf("Currently at version 0.0.1.\n");
			break;
		} else if (lit_strncmp(args[index]->buff, "-d")) {
			if (index + 1 == len_from_sizeof(args)) {
				// TODO: Error here.
				error();
				goto cleanup;
			}

			template_dir = args[index + 1];
			break;
		}
	}
	// Necessary to know for upcoming operations.
	if (template_dir == nullptr) {
		error();
		goto cleanup;
	}
	if (memchr(template_dir->buff, '.', len(template_dir)) != nullptr) {
		// TODO: Error here.
		// The given name represents a file name not a directory name.
		error();
		goto cleanup;
	}

	// TODO: We are limited to _MAX_PATH characters, how to escape this
	// limitation.

	// We are going to have `_` prefixed c string elements to convert them to
	// our string type for future use.
	char _abs_template_dir[_MAX_PATH] = "";
	struct sstring abs_template_dir = string_new(_abs_template_dir);
	if ((*ref_len(&abs_template_dir) =
			 GetFullPathName(template_dir->buff, _MAX_PATH,
							 abs_template_dir.buff, nullptr)) >= _MAX_PATH) {
		// TODO: Error.
		goto cleanup;
	}

	switch (_mkdir(abs_template_dir.buff)) {
		case EEXIST:
		case ENOENT: {
			// TODO: Error here.
			error();
			goto cleanup;
		}
		default:
	}

cleanup:
	// Cleanup.
	for (size_t index = 0; index < sizeof args / sizeof args[0]; ++index) {
		struct_string_del(args[index]);
	}

	return 0;
}
