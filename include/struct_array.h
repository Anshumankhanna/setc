#ifndef ARRAY_H
#define ARRAY_H

#include "metadata.h"
#include <stddef.h>

#define decl_single_word_type(TYPE, NAME) typedef TYPE NAME

#define darray(TYPE) struct TYPE##_darray
#define darray_decl(TYPE)                                                      \
	darray(TYPE) {                                                             \
		struct metadata hdr;                                                   \
		TYPE buff[];                                                           \
	}

#define darray_def_new(TYPE)                                                   \
	[[nodiscard]] darray(TYPE) *                                        \
		_##TYPE##_darray_new(const size_t len, const size_t elem_size) {       \
		darray(TYPE) *new = malloc(sizeof(darray(TYPE)) + elem_size * len);    \
		if (new == nullptr) {                                                  \
			return new;                                                        \
		}                                                                      \
                                                                               \
		*ref_cap(new) = *ref_len(new) = len;                                   \
		memset(new->buff, 0, elem_size * len);                                 \
                                                                               \
		return new;                                                            \
	}
#define darray_decl_new(TYPE)                                                  \
	[[nodiscard]] darray(TYPE) *                                 \
		_##TYPE##_darray_new(const size_t len, const size_t elem_size)
#define darray_new(TYPE, len) _##TYPE##_darray_new(len, sizeof(TYPE))

#define darray_def_del(TYPE)                                                   \
	inline void _##TYPE##_darray_del(darray(TYPE) * this) {                    \
		free(this);                                                            \
	}
#define darray_decl_del(TYPE)                                                  \
	extern inline void _##TYPE##_darray_del(darray(TYPE) * this)
#define darray_del(TYPE, this) _##TYPE##_darray_del(this)

#define sarray(TYPE) struct TYPE##_sarray
#define sarray_decl(TYPE)                                                      \
	sarray(TYPE) {                                                             \
		struct metadata hdr;                                                   \
		TYPE *buff;                                                            \
	}

#define const_sarray(TYPE) struct const_##TYPE##_sarray
#define const_sarray_decl(TYPE)                                                \
	const_sarray(TYPE) {                                                       \
		struct metadata hdr;                                                   \
		TYPE *const buff;                                                      \
	}

#endif
