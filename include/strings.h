#include "arrays.h"
#include "metadata.h"
#include "utilities.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// TYPES
typedef struct {
	metadata hdr;
	char8_t buff[];
} dstring;
typedef struct {
	metadata hdr;
	char8_t *buff;
} sstring;
typedef struct {
	metadata hdr;
	const char8_t *buff;
} lstring;

DECL_SINGLE_WORD_TYPE(dstring *, dstring_p);
DARRAY_DECL(dstring_p);

// CONSTRUCTORS
[[nodiscard]] dstring *new_dstring_str(const char8_t *const restrict raw_bytes);
[[nodiscard]] dstring *new_dstring_len(const size_t write_len,
								   const char8_t *const restrict raw_bytes);
#define new_dstring(arg, ...) _Generic(arg, size_t: new_dstring_len, const char8_t *: new_dstring_str, char8_t *: new_dstring_str)(arg __VA_OPT__(,) __VA_ARGS__)

#define new_sstring(len)                                                       \
	(sstring){.hdr = reserve(len), .buff = (char8_t[len + 1]){0}};
#define new_lstring(str)                                                       \
	_Generic(str,                                                              \
		const char8_t *: (lstring){.hdr = fill(sizeof str - sizeof ""),        \
								   .buff = str})

DARRAY_DECL_NEW(dstring_p);

// DESTRUCTORS
void del_dstring(dstring *const this);
DARRAY_DECL_DEL(dstring_p);

// GETTERS

// SETTERS

// APPEND
DARRAY_DECL_ADD(dstring_p);

// CONCATENATIONS
