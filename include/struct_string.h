#include "metadata.h"
#include "struct_array.h"
#include "utilities.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct string {
	struct metadata hdr;
	char8_t buff[];
};

[[nodiscard]] struct string *
struct_string_new_char_arr(const char8_t *const restrict raw_string);
[[nodiscard]] struct string *struct_string_new_length(const size_t _len,
													  const char8_t character);
[[nodiscard]] struct string *
struct_string_new_string(const struct string *const restrict str);
#define dstring_new(arg1, ...)                                                 \
	_Generic(arg1,                                                             \
		size_t: struct_string_new_length,                                      \
		struct string *: struct_string_new_string,                             \
		const struct string *: struct_string_new_string,                       \
		char8_t *: struct_string_new_char_arr,                                 \
		const char8_t *: struct_string_new_char_arr)(arg1 __VA_OPT__(, )       \
														 __VA_ARGS__)
void struct_string_del(struct string *const this);
[[nodiscard]] struct string *
struct_string_cat(const struct string *const restrict first,
				  const struct string *const restrict second);

DECL_SINGLE_WORD_TYPE(struct string *, struct_string_p);
DARRAY_DECL(struct_string_p);
DARRAY_DECL_NEW(struct_string_p);
DARRAY_DECL_DEL(struct_string_p);
DARRAY_DECL_ADD(struct_string_p);

// Static string i.e. a string struct that has a static buffer i.e. `char8_t[]`.
struct sstring {
	struct metadata hdr;
	char8_t *const buff;
};
[[nodiscard]] inline struct sstring
struct_sstring_new(const size_t cap, char8_t static_string[const cap]) {
	return (struct sstring){.hdr = {.cap = cap, .len = 0},
							.buff = static_string};
}

// Literal string i.e. a string struct that has a string literal buffer i.e.
// `const char8_t *`.
struct lstring {
	const struct metadata hdr;
	const char8_t *buff;
};
[[nodiscard]] inline struct lstring
struct_lstring_new(const size_t cap, const char8_t *const literal_string) {
	return (struct lstring){.hdr = {.cap = cap, .len = cap},
							.buff = literal_string};
}

#define string_new(_str)                                                       \
	_Generic(_str,                                                             \
		char8_t *: struct_sstring_new,                                         \
		const char8_t *: struct_lstring_new)(sizeof _str - 1, _str)
