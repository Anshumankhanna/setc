#define _CRT_SECURE_NO_WARNINGS

#include "arrays.h"
#include "metadata.h"
#include "string.h"
#include "strings.h"
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
		exit_status = errno == 0 ? 1wb : (u(8))errno;                          \
		goto cleanup;                                                          \
	} while (0)

static enum {
	CRDIR_EXIST = EEXIST,		 // Directory exists
	CRDIR_INVALID_PATH = ENOENT, // Invalid path
	CRDIR_ERROR = -1,			 // Some error
	CRDIR_SUCCESS = 0			 // Success
} create_dir([[maybe_unused]] const void *path) {
	errno = 0;

	const char *buff = nullptr;
	// switch (path.type) {
	// 	case TAG_DSTRING: {
	// 		buff = (const char *)path.val.dstr->buff;
	// 		break;
	// 	}
	// 	case TAG_SSTRING: {
	// 		buff = (const char *)path.val.sstr->buff;
	// 		break;
	// 	}
	// 	case TAG_LSTRING: {
	// 		buff = (const char *)path.val.lstr->buff;
	// 		break;
	// 	}
	// 	default: unreachable();
	// }

	switch (_mkdir(buff)) {
		case -1: {
			switch (errno) {
				case EEXIST: return CRDIR_EXIST;
				case ENOENT: return CRDIR_INVALID_PATH;
				default: return CRDIR_ERROR;
			}
		}
		case 0: return CRDIR_SUCCESS;
		default: unreachable();
	}
}

// static inline bool create_file(const struct string *const restrict path,
// 							   const struct string *const restrict data) {
// 	FILE *new = fopen((const char *)path->buff, "wx");
// 	if (new == nullptr) {
// 		return false;
// 	}
//
// 	const size_t written_chars =
// 		fwrite(data->buff, sizeof *data->buff, len(data), new);
// 	if (written_chars < len(data) || written_chars == 0) {
// 		return false;
// 	}
//
// 	return true;
// }

int main(const int argc, const char *const argv[const]) {
	constexpr size_t BUFF_SIZE = (size_t)4096;
	u(8) exit_status = 0uwb;

	darray(dstring_p) *args = nullptr;
	struct dstring *src = nullptr;
	struct dstring *include = nullptr;

	if (argc <= 1) {
		err("Require more arguments");
	}

	args = new_darray(dstring_p, (size_t)argc - 1);
	for (size_t index = 0; index < (size_t)argc - 1; ++index) {
		darray(dstring_p) *const res =
			add_darray(dstring_p, args,
					   new_dstring(cast(const char8_t *, argv[index + 1])));
		if (res == nullptr) {
			err("Couldn't allocate all arguments");
		}
		args = res;
	}

	// This `const` qualifier won't let us free `template_dir` since
	// `template_dir` is acting as a readable string only.
	const dstring *template_dir = nullptr;
	for (size_t index = 0; index < len(args); ++index) {
		if (strncmp(char_arr(args->buff[index]->buff), "--help",
					sizeof "--help") == 0 ||
			strncmp(char_arr(args->buff[index]->buff), "-h", sizeof "-h") ==
				0) {
			// TODO: Generate the help menu.
			printf("The help menu will be available soon.\n");
			goto cleanup;
		} else if (strncmp(char_arr(args->buff[index]->buff), "--version",
						   sizeof "--version") == 0 ||
				   strncmp(char_arr(args->buff[index]->buff), "-v",
						   sizeof "-v") == 0) {
			printf("Currently at version 0.0.1.\n");
			goto cleanup;
		} else if (strncmp(char_arr(args->buff[index]->buff), "-d",
						   sizeof "-d") == 0) {
			if (index + 1 == len(args)) {
				err("\x1B[37m-d\x1B[39m has to be followed with a directory "
					"name or '.'");
			}

			++index;
			if (args->buff[index]->buff[0] == '-') {
				// if curr == "--" and index + 1 < len(&args) and next[0] == '-'
				if (strncmp(char_arr(args->buff[index]->buff), "--",
							sizeof "--") != 0 ||
					index + 1 == len(args) ||
					args->buff[index + 1]->buff[0] != '-') {
					err("Invalid arguments provided");
				}
				++index;
			}
			// We are adding a default name.
			if (strncmp(char_arr(args->buff[index]->buff), ".", sizeof ".") ==
				0) {
				del(args->buff[index]);
				args->buff[index] = new_dstring(u8"cproject");
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
	sstring abs_template_dir = new_sstring(BUFF_SIZE);
	set_len(&abs_template_dir,
			GetFullPathName((const char *)template_dir->buff, BUFF_SIZE,
							char_arr(abs_template_dir.buff), nullptr));

	if (len(&abs_template_dir) >= BUFF_SIZE) {
		err("Buffer allocated to store directory path is too small");
	}
	if (len(&abs_template_dir) == 0) {
		err();
	}

	switch (create_dir(&abs_template_dir)) {
		case CRDIR_EXIST:
		case CRDIR_INVALID_PATH:
		case CRDIR_ERROR: goto cleanup;
		case CRDIR_SUCCESS: {
			printf("Created template directory\n");
			break;
		}
		default: unreachable();
	}

	// 	constexpr char8_t _gitignore[] = {
	// #embed "../.gitignore" suffix(, )
	// 		'\0'};
	// 	[[maybe_unused]] const struct lstring gitignore =
	// string_new(_gitignore);
	//
	// 	constexpr char8_t _makefile[] = {
	// #embed "../makefile" suffix(, )
	// 		'\0'};
	// 	[[maybe_unused]] const struct lstring makefile = string_new(_makefile);
	//
	// 	constexpr char8_t _clangd[] = {
	// #embed "../.clangd" suffix(, )
	// 		'\0'};
	// 	[[maybe_unused]] const struct lstring clangd = string_new(_clangd);
	//
	// 	constexpr char8_t _clang_format[] = {
	// #embed "../.clang-format" suffix(, )
	// 		'\0'};
	// 	[[maybe_unused]] const struct lstring clang_format =
	// 		string_new(_clang_format);

cleanup:
	// Cleanup.
	del(include);
	del(src);
	del(args);

	return (int)exit_status;
}
