#include "metadata.h"
#include "struct_array.h"
#include "utilities.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// TYPES
typedef struct dstring dstring;
typedef struct sstring sstring;
typedef struct lstring lstring;

// CONSTRUCTORS
[[nodiscard]] dstring *new_dstring(const char8_t *const restrict raw_bytes, const size_t write_len, [[maybe_unused]] lstring *restrict err);
[[nodiscard]] sstring _new_sstring(const char8_t *const restrict raw_bytes, const size_t write_len);
#define new_sstring(raw_bytes) _new_sstring(raw_bytes, sizeof raw_bytes)
[[nodiscard]] lstring _new_lstring(const char8_t *const restrict raw_bytes, const size_t write_len);
#define new_lstring(raw_bytes) _new_lstring(raw_bytes, sizeof raw_bytes)

// DESTRUCTORS
void del_dstring(dstring *const this);

// GETTERS
// HDR GETTER
[[nodiscard]] const metadata *hdr_c_dstring(const dstring *const restrict this);
[[nodiscard]] metadata *hdr_dstring(dstring *const restrict this);
[[nodiscard]] const metadata_const_cap *hdr_c_sstring(const sstring *const restrict this);
[[nodiscard]] metadata_const_cap *hdr_sstring(sstring *const restrict this);
[[nodiscard]] const metadata *hdr_lstring(const lstring *const restrict this);

// SETTERS
// CONCATENATIONS

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
[[nodiscard]] struct sstring
struct_sstring_new(const size_t cap, char8_t static_string[const cap]);

// Literal dstring i.e. a dstring struct that has a dstring literal buffer i.e.
// `const char8_t *`.

struct lstring;
[[nodiscard]] extern inline struct lstring
struct_lstring_new(const size_t cap, const char8_t *const literal_string);

#define string_new(_str)                                                       \
	_Generic(_str,                                                             \
		char8_t *: struct_sstring_new,                                         \
		const char8_t *: struct_lstring_new)(sizeof _str - 1, _str)

union string_u {
	const dstring *dstr;
	const sstring *sstr;
	const lstring *lstr;
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
