#include "metadata.h"
#include <stdlib.h>
#include <string.h>
#include "struct_string.h"

[[nodiscard]] struct string *struct_string_new(const char *const restrict raw_string, const size_t raw_string_len) {
	struct string *new = malloc(sizeof(struct metadata) + sizeof(char) * (raw_string_len + 1));
	if (new == nullptr) {
		return new;
	}

	*ref_cap(new) = raw_string_len;
	// In case we have been provided a length but not a string, we just create an empty string with a great capacity.
	if (raw_string != nullptr) {
		strncpy_s(new -> buff, new -> hdr.cap + 1, raw_string, raw_string_len);
	} else {
		new -> buff[0] = '\0';
	}
	// We are doing it like this since this is the only way to truly capture the length of the string,
	// If `raw_string[n] == '\0'` for `0 <= n < raw_string_len`, then the length of the string copied is varied,
	// We could have ignored this but it would create an issue in the program where `len` doesn't represent the real length.
	*ref_len(new) = strnlen_s(new -> buff, new -> hdr.cap);

	return new;
}

void struct_string_del(struct string *ptr) {
	free(ptr);
	ptr = nullptr;
}
