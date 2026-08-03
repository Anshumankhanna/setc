#include "strings.h"
#include "_arrays.h"
#include "metadata.h"
#include "utilities.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CONSTRUCTORS
[[nodiscard]] dstring *new_dstring_str(const char8_t *const restrict raw_bytes) {
	auto const raw_bytes_len = strnlen(cast(const char *, raw_bytes), SIZE_MAX);
	if (raw_bytes_len == SIZE_MAX) {
		return nullptr;
	}

	return new_dstring_len(raw_bytes_len, raw_bytes);
}
[[nodiscard]] dstring *new_dstring_len(const size_t write_len, const char8_t *const restrict raw_bytes) {
	if (raw_bytes == nullptr) {
		return nullptr;
	}

	dstring *const new =
		malloc(sizeof(dstring) + sizeof *raw_bytes * (write_len + 1));
	if (new == nullptr) {
		return nullptr;
	}

	set_cap(new, write_len);
	set_len(new, write_len);
	memcpy(new->buff, raw_bytes, sizeof *raw_bytes * len(new));
	new->buff[len(new)] = '\0';

	return new;
}
DARRAY_DEF_NEW(dstring_p)

// DESTRUCTORS
void del_dstring(dstring *const this) {
	free(this);
}
DARRAY_DEF_DEL(dstring_p)

// GETTERS
// HDR GETTER

// [[nodiscard]] struct dstring *
// struct_dstring_cat(const struct dstring *const restrict first,
// 				   const struct dstring *const restrict second) {
// 	if (first == nullptr) {
// 		return dstring_new(second);
// 	}
// 	if (second == nullptr) {
// 		return dstring_new(first);
// 	}
//
// 	struct dstring *const new =
// 		malloc(sizeof(struct dstring) +
// 			   sizeof(char8_t) * (len(first) + len(second) + 1));
// 	if (new == nullptr) {
// 		return nullptr;
// 	}
//
// 	ref_cap(new) = ref_len(new) = len(first) + len(second);
// 	memcpy(new->buff, first, len(first));
// 	memcpy(new->buff + len(first), second, len(second));
// 	new->buff[len(new)] = '\0';
//
// 	return new;
// }

// ARRAYS
DARRAY_DEF_ADD(dstring_p)
