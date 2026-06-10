#include "metadata.h"
#include <stddef.h>

struct string {
	struct metadata hdr;
	char buff[];
};

// Dynamically creates and returns the pointer to an object of type `struct
// string`, If `raw_string == nullptr || raw_string_len == 0`, then `hdr.cap ==
// hdr.len == 0` and `buff[0] = '\0'`, i.e. returns an empty string (`hdr.len ==
// 0`) but with `hdr.cap == raw_string_len`. Otherwise, `hdr.cap == hdr.len ==
// strlen(raw_string)` and `strncmp(buff, raw_string, strlen(raw_string)) == 0`,
// i.e. we create a new string with the data from `raw_string`.
// In case of `ENOMEM` the returned value is `nullptr`,
// If `raw_string[index] == '\0'` for `0 <= index < raw_string_len`, then
// `hdr.len == strnlen_s(raw_string, raw_string_len)`, which has to be handled
// at the user's side.
[[nodiscard]] struct string *struct_string_new(const char *const raw_string,
											   const size_t raw_string_len);
// Frees the memory allocated to `ptr` and makes `ptr == nullptr`.
// Any error ocurred is UB the same as `free()`.
void struct_string_del(struct string *ptr);

struct sstring {
	struct metadata hdr;
	char *buff;
};
#define sstring_new(_str) (struct sstring) { .hdr = { .cap = sizeof _str - 1, .len = 0 }, .buff = _str }

struct lstring {
	struct metadata hdr;
	const char *buff;
};
#define lstring_new(_str) (struct lstring) { .hdr = { .cap = sizeof _str - 1, .len = sizeof _str - 1 }, .buff = _str }

#define string_new(_str) _Generic(_str, const char *: lstring_new(_str), char *: sstring_new(_str))
