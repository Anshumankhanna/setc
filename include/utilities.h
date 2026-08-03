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
		dstring: del_dstring,                                                  \
		darray(dstring_p): del_darray(dstring_p),                              \
		default: free)(_ptr)

// STRINGS (char to char8_t)
#ifndef char8_t
typedef unsigned char char8_t;
#endif

// CAST
#define cast(TYPE, val) (TYPE) val
// To convert `char8_t` based string types to `char` based string types
#define byte_arr(val)                                                       \
	_Generic(val, const char *: (const char8_t *)val, char *: (char8_t *)val)
#define char_arr(val)                                                       \
	_Generic(val, const char8_t *: (const char *)val, char8_t *: (char *)val)

#endif
