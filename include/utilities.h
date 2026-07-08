#ifndef UTILITIES_H
#define UTILITIES_H

#include <string.h>

#define i(N) _BitInt(N)
#define u(N) unsigned i(N)

#ifndef unreachable
#define unreachable() __builtin_unreachable()
#endif

#ifndef nullptr_t
typedef typeof(nullptr) nullptr_t;
#endif

// MAX FUNCTIONS
#define _MAX(TYPE) max_##TYPE
#define DECL_MAX(TYPE)                                                         \
	[[nodiscard]] extern inline TYPE _MAX(TYPE)(const TYPE a, const TYPE b)
#define DEF_MAX(TYPE)                                                          \
	[[nodiscard]] inline TYPE _MAX(TYPE)(const TYPE a, const TYPE b) {         \
		return a > b ? a : b;                                                  \
	}

DEF_MAX(size_t)

#define max(a, b) _Generic(a, typeof(b): _Generic(b, size_t: _MAX(size_t)))(a, b)

// DEL FUNCTIONS
#define del(_ptr)                                                              \
	_Generic(*_ptr,                                                            \
		struct string: struct_string_del,                                      \
		darray(struct_string_p): darray_del(struct_string_p),                  \
		default: free)(_ptr)

// STRINGS (char to char8_t)
#ifndef char8_t
typedef unsigned char char8_t;
#endif

[[nodiscard]] inline bool ncmp(const char8_t *const restrict str1,
							   const char8_t *const restrict str2,
							   const size_t size) {
	return strncmp((const char *)str1, (const char *)str2, size) == 0;
}

#endif
