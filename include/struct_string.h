#include "metadata.h"
#include "struct_array.h"
#include "utilities.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct dstring;

[[nodiscard]] struct dstring *
struct_dstring_new_char_arr(const char8_t *const restrict raw_string);
[[nodiscard]] struct dstring *
struct_dstring_new_length(const size_t _len, const char8_t character);
[[nodiscard]] struct dstring *
struct_dstring_new_string(const struct dstring *const restrict str);
#define dstring_new(arg1, ...)                                                 \
	_Generic(arg1,                                                             \
		size_t: struct_dstring_new_length,                                     \
		struct dstring *: struct_dstring_new_string,                           \
		const struct dstring *: struct_dstring_new_string,                     \
		char8_t *: struct_dstring_new_char_arr,                                \
		const char8_t *: struct_dstring_new_char_arr)(arg1 __VA_OPT__(, )      \
														  __VA_ARGS__)
void struct_dstring_del(struct dstring *const this);
[[nodiscard]] struct dstring *
struct_dstring_cat(const struct dstring *const restrict first,
				   const struct dstring *const restrict second);

DECL_SINGLE_WORD_TYPE(struct dstring *, struct_dstring_p);
darray(struct_dstring_p);
DARRAY_DECL_NEW(struct_dstring_p);
DARRAY_DECL_DEL(struct_dstring_p);
DARRAY_DECL_ADD(struct_dstring_p);

// Static dstring i.e. a dstring struct that has a static buffer i.e.
// `char8_t[]`.

struct sstring;
[[nodiscard]] inline struct sstring
struct_sstring_new(const size_t cap, char8_t static_string[const cap]) {
	return (struct sstring){.hdr = {.cap = cap, .len = 0},
							.buff = static_string};
}

// Literal dstring i.e. a dstring struct that has a dstring literal buffer i.e.
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

union string_u {
	const struct dstring *dstr;
	const struct sstring *sstr;
	const struct lstring *lstr;
};
enum string_e { TAG_DSTRING, TAG_SSTRING, TAG_LSTRING };
struct string {
	enum string_e type : sizeof(union string_u);
	union string_u val;
};
#define string(_str)                                                           \
	_Generic(_str,                                                             \
		const struct dstring *: (                                              \
				 struct string){.type = TAG_DSTRING,                           \
								.val = {.dstr =                                \
											(const struct dstring *)(_str)}},  \
		struct dstring *: (                                                    \
				 struct string){.type = TAG_DSTRING,                           \
								.val = {.dstr =                                \
											(const struct dstring *)(_str)}},  \
		const struct sstring *: (                                              \
				 struct string){.type = TAG_SSTRING,                           \
								.val = {.sstr =                                \
											(const struct sstring *)(_str)}},  \
		struct sstring *: (                                                    \
				 struct string){.type = TAG_SSTRING,                           \
								.val = {.sstr =                                \
											(const struct sstring *)(_str)}},  \
		const struct lstring *: (                                              \
				 struct string){.type = TAG_LSTRING,                           \
								.val = {.lstr =                                \
											(const struct lstring *)(_str)}},  \
		struct lstring *: (struct string){                                     \
			.type = TAG_LSTRING,                                               \
			.val = {.lstr = (const struct lstring *)(_str)}})
