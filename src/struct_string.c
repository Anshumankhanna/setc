#include "struct_string.h"
#include "struct_array.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

[[nodiscard]] struct string *
struct_string_new_value(const char *const restrict raw_string) {
	if (raw_string == nullptr) {
		// TODO: Handle error.
		return nullptr;
	}

	const size_t raw_string_len = strnlen(raw_string, SIZE_MAX);
	if (raw_string_len == SIZE_MAX) {
		// TODO: Handle error.
		return nullptr;
	}

	struct string *new = malloc(sizeof(struct string) +
								sizeof *raw_string * (raw_string_len + 1));
	if (new == nullptr) {
		// TODO: Handle error.
		return new;
	}

	*ref_cap(new) = *ref_len(new) = raw_string_len;
	memcpy(new->buff, raw_string, sizeof *raw_string * len(new));
	new->buff[len(new)] = '\0';

	return new;
}
[[nodiscard]] struct string *struct_string_new_length(const size_t len,
													  const char character) {
	struct string *new =
		malloc(sizeof(struct string) + sizeof character * (len + 1));
	if (new == nullptr) {
		// TODO: Handle error.
		return new;
	}

	*ref_cap(new) = len;
	*ref_len(new) = 0;
	new->buff[len(new)] = '\0';

	return new;
}
void struct_string_del(struct string *this) {
	printf("Removing something\n");
	free(this);
}

DARRAY_DEF_NEW(struct_string_p)
DARRAY_DEF_DEL(struct_string_p)
