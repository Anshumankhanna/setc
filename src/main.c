#define _CRT_SECURE_NO_WARNINGS

#include "metadata.h"
#include "struct_array.h"
#include "struct_string.h"
#include "utilities.h"

#include <direct.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define err(ERR)                                                               \
	do {                                                                       \
		printf("\x1B[31m" ERR "\n");                                           \
		if (errno != 0) {                                                      \
			perror(nullptr);                                                   \
		}                                                                      \
		printf("\x1B[39m");                                                    \
                                                                               \
		exit_status = errno == 0 ? 1 : (_BitInt(8))errno;                      \
		goto cleanup;                                                          \
	} while (0)

static inline _BitInt(8) create_dir(const struct string *const restrict path) {
	switch (_mkdir((const char *)path->buff)) {
		case -1: {
			return (_BitInt(8))errno;
			// switch (errno) {
			// 	case EEXIST: {
			// 		err("Directory already exists");
			// 	}
			// 	case ENOENT: {
			// 		err("Invalid directory path");
			// 	}
			// 	default: {
			// 		err("Unknown error in creating directory");
			// 	}
			// }
		}
		case 0: {
			// printf("Directory created successfully at %.*s\n",
			// (int)len(path), 	   path->buff);
			return 0;
		}
		default: {
			unreachable();
		}
	}
}
static inline bool create_file(const struct string *const restrict path,
							   const struct string *const restrict data) {
	FILE *new = fopen((const char *)path->buff, "wx");
	if (new == nullptr) {
		return false;
	}

	const size_t written_chars =
		fwrite(data->buff, sizeof *data->buff, len(data), new);
	if (written_chars < len(data) || written_chars == 0) {
		return false;
	}

	return true;
}

int main(const int argc, const char *const *const argv) {
	constexpr size_t BUFF_SIZE = 4096;
	_BitInt(8) exit_status = 0;

	darray(struct_string_p) *args = nullptr;
	struct string *src = nullptr;
	struct string *include = nullptr;

	if (argc <= 1) {
		err("Require more arguments");
	}

	args = darray_new(struct_string_p, (size_t)argc - 1);
	// TODO: Function to append arguments.
	for (size_t index = 0; index < (size_t)argc - 1; ++index) {
		darray(struct_string_p) *const res =
			darray_add(struct_string_p, args,
					   dstring_new((const char8_t *)argv[index + 1]));
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
		if (ncmp(args->buff[index]->buff, u8"--help", sizeof "--help") ||
			ncmp(args->buff[index]->buff, u8"-h", sizeof "-h")) {
			// TODO: Generate the help menu.
			printf("The help menu will be available soon.\n");
			goto cleanup;
		} else if (ncmp(args->buff[index]->buff, u8"--version",
						sizeof "--version") ||
				   ncmp(args->buff[index]->buff, u8"-v", sizeof "-v")) {
			printf("Currently at version 0.0.1.\n");
			goto cleanup;
		} else if (ncmp(args->buff[index]->buff, u8"-d", sizeof "-d")) {
			if (index + 1 == len(args)) {
				err("\x1B[37m-d\x1B[39m has to be followed with a directory "
					"name or '.'");
			}

			++index;
			if (args->buff[index]->buff[0] == '-') {
				// if curr == "--" and index + 1 < len(&args) and next[0] == '-'
				if (ncmp(args->buff[index]->buff, u8"--", sizeof "--") != 0 ||
					index + 1 == len(args) ||
					args->buff[index + 1]->buff[0] != '-') {
					err("Invalid arguments provided");
				}
				++index;
			}
			// We are adding a default name.
			if (ncmp(args->buff[index]->buff, u8".", sizeof ".")) {
				del(args->buff[index]);
				args->buff[index] = dstring_new(u8"cproject");
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

	// We are going to have `_` prefixed c string elements to convert them to
	// our string type for future use.
	char8_t _abs_template_dir[BUFF_SIZE] = u8"";
	struct sstring abs_template_dir = string_new(_abs_template_dir);
	ref_len(&abs_template_dir) =
		GetFullPathName((const char *)template_dir->buff, BUFF_SIZE,
						(char *)abs_template_dir.buff, nullptr);
	if (len(&abs_template_dir) >= BUFF_SIZE) {
		err("Buffer allocated to store directory path is too small");
	}
	if (len(&abs_template_dir) == 0) {
		err();
	}

	switch (_mkdir((const char *)abs_template_dir.buff)) {
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

	const struct lstring src_literal = string_new(u8"/src");
	src = struct_string_cat((const struct string *)&abs_template_dir,
							(const struct string *)&src_literal);
	if (src != nullptr) {
		switch (_mkdir((const char *)src->buff)) {
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
					   (int)len(src), src->buff);
				break;
			}
			default: {
				unreachable();
			}
		}
	}

	const struct lstring include_literal = string_new(u8"/include");
	include = struct_string_cat((const struct string *)&abs_template_dir,
								(const struct string *)&include_literal);
	if (include != nullptr) {
		switch (_mkdir((const char *)include->buff)) {
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
					   (int)len(include), include->buff);
				break;
			}
			default: {
				unreachable();
			}
		}
	}

	constexpr char8_t _gitignore[] = {
#embed "../.gitignore" suffix(, )
		'\0'};
	[[maybe_unused]] const struct lstring gitignore = string_new(_gitignore);

	constexpr char8_t _makefile[] = {
#embed "../makefile" suffix(, )
		'\0'};
	[[maybe_unused]] const struct lstring makefile = string_new(_makefile);

	constexpr char8_t _clangd[] = {
#embed "../.clangd" suffix(, )
		'\0'};
	[[maybe_unused]] const struct lstring clangd = string_new(_clangd);

	constexpr char8_t _clang_format[] = {
#embed "../.clang-format" suffix(, )
		'\0'};
	[[maybe_unused]] const struct lstring clang_format =
		string_new(_clang_format);

cleanup:
	// Cleanup.
	del(include);
	del(src);
	del(args);

	return (int)exit_status;
}
