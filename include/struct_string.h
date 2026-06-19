#include "metadata.h"
#include "struct_array.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct string {
	struct metadata hdr;
	char buff[];
};

[[nodiscard]] struct string *
struct_string_new_value(const char *const restrict raw_string);
[[nodiscard]] struct string *struct_string_new_length(const size_t _len,
													  const char character);
#define PICK(a1, ...) a1
#define dstring_new(arg1, ...)                                                 \
	_Generic(arg1,                                                             \
		size_t: struct_string_new_length(                                      \
				 (size_t)(const void *)arg1,                                   \
				 PICK(__VA_ARGS__ __VA_OPT__(, ) '\0')),                       \
		char *: struct_string_new_value((const char *)(const void *)arg1),     \
		const char *: struct_string_new_value(                                 \
				 (const char *)(const void *)arg1))
void struct_string_del(struct string *this);

DECL_SINGLE_WORD_TYPE(struct string *, struct_string_p);
DARRAY_DECL(struct_string_p);
DARRAY_DECL_NEW(struct_string_p);
DARRAY_DECL_DEL(struct_string_p);
DARRAY_DECL_ADD(struct_string_p);

// Static string i.e. a string struct that has a static buffer i.e. `char[]`.
struct sstring {
	struct metadata hdr;
	char *const buff;
};
[[nodiscard]] inline struct sstring
struct_sstring_new(const size_t cap, char static_string[const cap]) {
	return (struct sstring){.hdr = {.cap = cap, .len = 0},
							.buff = static_string};
}
// Macro takes an updater func that modifies the buffer and returns length.
#define struct_sstring_update(_this, _updater, ...)                            \
	(*ref_len(_this) = _updater(__VA_ARGS__))

// Literal string i.e. a string struct that has a string literal buffer i.e.
// `const char *`.
struct lstring {
	const struct metadata hdr;
	const char *const buff;
};
[[nodiscard]] inline struct lstring
struct_lstring_new(const size_t cap, const char *const literal_string) {
	return (struct lstring){.hdr = {.cap = cap, .len = cap},
							.buff = literal_string};
}

#define string_new(_str)                                                       \
	_Generic(_str,                                                             \
		char *: struct_sstring_new,                                            \
		const char *: struct_lstring_new)(sizeof _str - 1, _str)
