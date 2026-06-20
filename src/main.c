#include "metadata.h"
#include "struct_array.h"
#include "struct_string.h"
#include <direct.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#ifndef unreachable
#define unreachable() __builtin_unreachable()
#endif

#define err(ERR)                                                               \
	do {                                                                       \
		perror("\x1B[31m" ERR "\x1B[39m");                                     \
		exit_status = errno == 0 ? 1 : errno;                                  \
		goto cleanup;                                                          \
	} while (0)

int main(const int argc, const char *const argv[const restrict]) {
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
		if (strncmp(args->buff[index]->buff, "--help", sizeof "--help") == 0 ||
			strncmp(args->buff[index]->buff, "-h", sizeof "-h") == 0) {
			// TODO: Generate the help menu.
			printf("The help menu will be available soon.\n");
			goto cleanup;
		} else if (strncmp(args->buff[index]->buff, "--version",
						   sizeof "--version") == 0 ||
				   strncmp(args->buff[index]->buff, "-v", sizeof "-v") == 0) {
			printf("Currently at version 0.0.1.\n");
			goto cleanup;
		} else if (strncmp(args->buff[index]->buff, "-d", sizeof "-d") == 0) {
			if (index + 1 == len(args)) {
				err("\x1B[37m-d\x1B[39m has to be followed with a directory "
					"name or '.'");
			}

			++index;
			if (args->buff[index]->buff[0] == '-') {
				// if curr == "--" and index + 1 < len(&args) and next[0] == '-'
				if (strncmp(args->buff[index]->buff, "--", sizeof "--") != 0 ||
					index + 1 == len(args) ||
					args->buff[index + 1]->buff[0] != '-') {
					err("Invalid arguments provided");
				}
				++index;
			}
			// We are adding a default name.
			if (strncmp(args->buff[index]->buff, ".", sizeof ".") == 0) {
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

	constexpr size_t BUFF_SIZE = 4096;
	// We are going to have `_` prefixed c string elements to convert them to
	// our string type for future use.
	char _abs_template_dir[BUFF_SIZE] = "";
	struct sstring abs_template_dir = string_new(_abs_template_dir);
	*ref_len(&abs_template_dir) = GetFullPathName(
		template_dir->buff, BUFF_SIZE, abs_template_dir.buff, nullptr);
	if (len(&abs_template_dir) >= BUFF_SIZE) {
		err("Buffer allocated to store directory path is too small");
	}
	if (len(&abs_template_dir) == 0) {
		err();
	}

	switch (_mkdir(abs_template_dir.buff)) {
		case -1: {
			switch (errno) {
				case EEXIST: {
					err("Directory already exists");
				}
				case ENOENT: {
					err("Invalid directory path");
				}
				default: {
					err("Unknown error in creating directory");
				}
			}
		}
		case 0: {
			printf("Directory created successfully at %.*s\n",
				   (int)len(&abs_template_dir), abs_template_dir.buff);
			break;
		}
		default: {
			unreachable();
		}
	}

cleanup:
	// Cleanup.
	del(args);

	return exit_status;
}
