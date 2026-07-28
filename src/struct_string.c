#include "struct_string.h"
#include "array_def.h"
#include "metadata.h"
#include "utilities.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TYPES
struct dstring {
	metadata hdr;
	char8_t buff[];
};
struct sstring {
	metadata_const_cap hdr;
	char8_t *buff;
};
struct lstring {
	metadata hdr;
	const char8_t *buff;
};

// CONSTRUCTORS
[[nodiscard]] dstring *new_dstring(const char8_t *const restrict raw_bytes,
								   const size_t raw_bytes_len, [[maybe_unused]] lstring *const restrict err) {
	if (raw_bytes == nullptr) {
		*err = new_lstring(u8"raw_bytes is nullptr.\n");
	}

	struct dstring *const new = malloc(sizeof(struct dstring) +
									   sizeof *raw_bytes * (raw_bytes_len + 1));
	if (new == nullptr) {
		*err = new_lstring(u8"Couldn't allocate more memory\n");
	}

	set_cap(new, raw_bytes_len);
	set_len(new, raw_bytes_len);
	memcpy(new->buff, raw_bytes, sizeof *raw_bytes * len(new));
	new->buff[len(new)] = '\0';

	return new;
}
[[nodiscard]] sstring _new_sstring(const char8_t *const restrict raw_bytes,
								   const size_t write_len);
[[nodiscard]] lstring _new_lstring(const char8_t *const restrict raw_bytes,
								   const size_t write_len);

// GETTERS
// HDR GETTER
[[nodiscard]] const metadata *
hdr_c_dstring(const dstring *const restrict this) {
	return &this->hdr;
}
[[nodiscard]] metadata *hdr_dstring(dstring *const restrict this) {
	return &this->hdr;
}
[[nodiscard]] const metadata_const_cap *
hdr_c_sstring(const sstring *const restrict this) {
	return &this->hdr;
}
[[nodiscard]] metadata_const_cap *hdr_sstring(sstring *const restrict this) {
	return &this->hdr;
}
[[nodiscard]] const metadata *hdr_lstring(const lstring *const restrict this) {
	return &this->hdr;
}

[[nodiscard]] struct dstring *
struct_dstring_new_char_arr(const char8_t *const restrict raw_string) {
}
[[nodiscard]] struct dstring *
struct_dstring_new_length(const size_t len, const char8_t character) {
	struct dstring *const new =
		malloc(sizeof(struct dstring) + sizeof character * (len + 1));
	if (new == nullptr) {
		// TODO: Handle error.
		return new;
	}

	set_cap(new, len);
	set_len(new, len);
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

DARRAY_DECL(struct_dstring_p);
DARRAY_DEF_NEW(struct_dstring_p)
DARRAY_DEF_DEL(struct_dstring_p)
DARRAY_DEF_ADD(struct_dstring_p)

// STATIC STRING
struct sstring {
	struct metadata hdr;
	char8_t *const buff;
};

[[nodiscard, unsequenced]] const struct metadata *
hdr_sstring(const struct sstring *const restrict str) {
	return &str->hdr;
}
[[nodiscard]] struct sstring
struct_sstring_new(const size_t cap, char8_t static_string[const cap]) {
	return (struct sstring){.hdr = {.cap = cap, .len = 0},
							.buff = static_string};
}

// LITERAL STRING
struct lstring {
	const struct metadata hdr;
	const char8_t *buff;
};

[[nodiscard, unsequenced]] const struct metadata *
hdr_lstring(const struct lstring *const restrict str) {
	return &str->hdr;
}
[[nodiscard]] struct lstring
struct_lstring_new(const size_t cap, const char8_t *const literal_string) {
	return (struct lstring){.hdr = {.cap = cap, .len = cap},
							.buff = literal_string};
}
