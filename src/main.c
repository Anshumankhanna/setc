#include "metadata.h"
#include "struct_array.h"
#include "struct_string.h"
#include <direct.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define lit_strncmp(buff, lit) _lit_strncmp(buff, lit, sizeof lit - 1)
[[nodiscard]] static inline bool _lit_strncmp(const char *const restrict buff,
											  const char *const restrict lit,
											  const size_t lit_size) {
	return strncmp(buff, lit, lit_size) == 0;
}

static inline void error() {
	printf("\x1B[31mSome error occurred\n\x1B[39m");
}

int main(const int argc, const char *const argv[const]) {
	if (argc <= 1) {
		printf("Require more arguments.\n");
		return 1;
	}

	darray(struct_string_p) *args =
		darray_new(struct_string_p, (size_t)argc - 1, struct_string_del);
	// TODO: Function to append arguments.
	for (size_t index = 0; index < len(args); ++index) {
		args->buff[index] = struct_string_new_value(argv[index + 1]);
		if (args->buff[index] == nullptr) {
			goto cleanup;
		}
	}

	// This `const` qualifier won't let us free `template_dir` since
	// `template_dir` is acting as a readable string only.
	const struct string *template_dir = nullptr;
	for (size_t index = 0; index < len(args); ++index) {
		printf("Running index %zu\n", index);
		if (lit_strncmp(args->buff[index]->buff, "--help") ||
			lit_strncmp(args->buff[index]->buff, "-h")) {
			// TODO: Generate the help menu.
			printf("The help menu will be available soon.\n");
			break;
		} else if (lit_strncmp(args->buff[index]->buff, "--version") ||
				   lit_strncmp(args->buff[index]->buff, "-v")) {
			printf("Currently at version 0.0.1.\n");
			break;
		} else if (lit_strncmp(args->buff[index]->buff, "-d")) {
			if (index + 1 == len(args)) {
				// TODO: Error here.
				error();
				goto cleanup;
			}

			template_dir = args->buff[index + 1];
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
		error();
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
	darray_del(struct_string_p, args);

	return 0;
}
