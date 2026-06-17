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
void struct_string_del(struct string *this);

DECL_SINGLE_WORD_TYPE(struct string *, struct_string_p);
DARRAY_DECL(struct_string_p);
DARRAY_DECL_NEW(struct_string_p);
DARRAY_DECL_DEL(struct_string_p);

struct sstring {
	struct metadata hdr;
	char *const buff;
};
#define sstring_new(_str)                                                      \
	(struct sstring) {                                                         \
		.hdr = {.cap = sizeof _str - 1, .len = 0}, .buff = _str                \
	}

struct lstring {
	struct metadata hdr;
	const char *buff;
};
#define lstring_new(_str)                                                      \
	(struct lstring) {                                                         \
		.hdr = {.cap = sizeof _str - 1, .len = sizeof _str - 1}, .buff = _str  \
	}

#define string_new(_str)                                                       \
	_Generic(_str, const char *: lstring_new(_str), char *: sstring_new(_str))
