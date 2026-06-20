#include "metadata.h"
#include "struct_array.h"
#include "struct_string.h"
#include <direct.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define litncmp(buff, lit) _litncmp(buff, lit, sizeof lit - 1)
[[nodiscard]] static inline bool _litncmp(const char *const restrict buff,
										  const char *const restrict lit,
										  const size_t lit_size) {
	return strncmp(buff, lit, lit_size) == 0;
}

#define err(ERR, ...)                                                          \
	do {                                                                       \
		perror("\x1B[31m" ERR "\x1B[39m");                                     \
		exit_status = errno == 0 ? 1 : errno;                                  \
		goto cleanup;                                                          \
	} while (0)

int main([[maybe_unused]] const int argc,
		 [[maybe_unused]] const char *const argv[const]) {
	constexpr size_t BUFF_SIZE = 4096;
	int exit_status = 0;
	darray(struct_string_p) *args = nullptr;

	if (argc <= 1) {
		err("Require more arguments");
	}

	args = darray_new(struct_string_p, (size_t)argc - 1);
	// TODO: Function to append arguments.
	for (size_t index = 0; index < (size_t)argc - 1; ++index) {
		darray(struct_string_p) *const res =
			darray_add(struct_string_p, args, dstring_new(argv[index + 1]));
		if (res == nullptr) {
			err("Couldn't allocate all arguments");
		}
		args = res;
	}
	//
	// This `const` qualifier won't let us free `template_dir` since
	// `template_dir` is acting as a readable string only.
	const struct string *template_dir = nullptr;
	for (size_t index = 0; index < len(args); ++index) {
		if (litncmp(args->buff[index]->buff, "--help") ||
			litncmp(args->buff[index]->buff, "-h")) {
			// TODO: Generate the help menu.
			printf("The help menu will be available soon.\n");
			goto cleanup;
		} else if (litncmp(args->buff[index]->buff, "--version") ||
				   litncmp(args->buff[index]->buff, "-v")) {
			printf("Currently at version 0.0.1.\n");
			goto cleanup;
		} else if (litncmp(args->buff[index]->buff, "-d")) {
			if (index + 1 == len(args)) {
				err("\x1B[37m-d\x1B[39m has to be followed with a directory "
					"name or '.'");
			}

			++index;
			// We are adding a default name.
			if (litncmp(args->buff[index]->buff, ".")) {
				printf("This ran\n");
				del(args->buff[index]);
				args->buff[index] = dstring_new("cproject");
				if (args->buff[index] == nullptr) {
					err("Couldn't change '.' to cproject");
				}
			}

			template_dir = args->buff[index];
		}
	}
	// Necessary to know for upcoming operations.
	if (template_dir == nullptr) {
		err("Invalid arguments provided");
	}

	// TODO: We are limited to _MAX_PATH characters, how to escape this
	// limitation.

	// We are going to have `_` prefixed c string elements to convert them to
	// our string type for future use.
	char _abs_template_dir[BUFF_SIZE] = "";
	struct sstring abs_template_dir = string_new(_abs_template_dir);
	if (struct_sstring_update(&abs_template_dir, GetFullPathName,
							  template_dir->buff, BUFF_SIZE,
							  abs_template_dir.buff, nullptr) >= BUFF_SIZE) {
		err("Buffer allocated to store directory path is too small");
	}

	switch (_mkdir(abs_template_dir.buff)) {
		case EEXIST: {
			err("Directory already exists");
		}
		case ENOENT: {
			err("Directory path can't be found, some parent folders may be "
				"missing");
		}
		default: {
			printf("Directory created successfully at %.*s\n",
				   (int)len(&abs_template_dir), abs_template_dir.buff);
		}
	}

cleanup:
	// Cleanup.
	del(args);

	return exit_status;
}
