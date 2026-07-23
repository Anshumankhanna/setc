#include "struct_string.h"
#include "metadata.h"
#include "metadata_def.h"
#include "array_def.h"
#include "utilities.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dstring {
	struct metadata hdr;
	char8_t buff[];
};

[[nodiscard]] struct dstring *
struct_dstring_new_char_arr(const char8_t *const restrict raw_string) {
	if (raw_string == nullptr) {
		// TODO: Handle error.
		return nullptr;
	}

	const size_t raw_string_len = strnlen((const char *)raw_string, SIZE_MAX);
	if (raw_string_len == SIZE_MAX) {
		// TODO: Handle error.
		return nullptr;
	}

	struct dstring *const new = malloc(
		sizeof(struct dstring) + sizeof *raw_string * (raw_string_len + 1));
	if (new == nullptr) {
		// TODO: Handle error.
		return new;
	}

	ref_cap(new) = ref_len(new) = raw_string_len;
	memcpy(new->buff, raw_string, sizeof *raw_string * len(new));
	new->buff[len(new)] = '\0';

	return new;
}
[[nodiscard]] struct dstring *
struct_dstring_new_length(const size_t len, const char8_t character) {
	struct dstring *const new =
		malloc(sizeof(struct dstring) + sizeof character * (len + 1));
	if (new == nullptr) {
		// TODO: Handle error.
		return new;
	}

	ref_cap(new) = ref_len(new) = len;
	memset(new->buff, character, cap(new));
	new->buff[len(new)] = '\0';

	return new;
}
[[nodiscard]] struct dstring *
struct_dstring_new_string(const struct dstring *const restrict str) {
	if (str == nullptr) {
		return nullptr;
	}

	struct dstring *const new =
		malloc(sizeof *str + sizeof *str->buff * (len(str) + 1));
	if (new == nullptr) {
		return nullptr;
	}

	ref_cap(new) = ref_len(new) = len(str);
	memcpy(new->buff, str->buff, len(str));
	new->buff[0] = '\0';

	return new;
}

void struct_dstring_del(struct dstring *const this) {
	free(this);
}

[[nodiscard]] struct dstring *
struct_dstring_cat(const struct dstring *const restrict first,
				   const struct dstring *const restrict second) {
	if (first == nullptr) {
		return dstring_new(second);
	}
	if (second == nullptr) {
		return dstring_new(first);
	}

	struct dstring *const new =
		malloc(sizeof(struct dstring) +
			   sizeof(char8_t) * (len(first) + len(second) + 1));
	if (new == nullptr) {
		return nullptr;
	}

	ref_cap(new) = ref_len(new) = len(first) + len(second);
	memcpy(new->buff, first, len(first));
	memcpy(new->buff + len(first), second, len(second));
	new->buff[len(new)] = '\0';

	return new;
}

DARRAY_DEF_NEW(struct_dstring_p)
DARRAY_DEF_DEL(struct_dstring_p)
DARRAY_DEF_ADD(struct_dstring_p)

struct sstring {
	struct metadata hdr;
	char8_t *const buff;
};
[[nodiscard]] extern inline struct sstring
struct_sstring_new(const size_t cap, char8_t static_string[const cap]);

[[nodiscard]] extern inline struct lstring
struct_lstring_new(const size_t cap, const char8_t *const literal_string);
