#ifndef ARRAY_H
#define ARRAY_H

#include "metadata.h"
#include <stddef.h>

#define DECL_SINGLE_WORD_TYPE(TYPE, NAME) typedef TYPE NAME

#define darray(TYPE) struct TYPE##_darray
#define DARRAY_DECL(TYPE)                                                      \
	darray(TYPE) {                                                             \
		struct metadata hdr;                                                   \
		void (*del)(TYPE);                                                     \
		TYPE buff[];                                                           \
	}

#define DARRAY_DECL_NEW(TYPE)                                                  \
	[[nodiscard]] darray(TYPE) * _##TYPE##_darray_new(const size_t len,        \
													  const size_t elem_size,  \
													  void (*const del)(TYPE))
#define DARRAY_DEF_NEW(TYPE)                                                   \
	[[nodiscard]] darray(TYPE) *                                               \
		_##TYPE##_darray_new(const size_t len, const size_t elem_size,         \
							 void (*const del)(TYPE)) {                        \
		darray(TYPE) *new = malloc(sizeof(darray(TYPE)) + elem_size * len);    \
		if (new == nullptr) {                                                  \
			return new;                                                        \
		}                                                                      \
                                                                               \
		new->del = del;                                                        \
		*ref_cap(new) = len;                                                   \
		*ref_len(new) = 0;                                                     \
                                                                               \
		return new;                                                            \
	}
#define darray_new(TYPE, len, del) _##TYPE##_darray_new(len, sizeof(TYPE), del)

#define DARRAY_DECL_DEL(TYPE) void _##TYPE##_darray_del(darray(TYPE) * this)
#define DARRAY_DEF_DEL(TYPE)                                                   \
	void _##TYPE##_darray_del(darray(TYPE) * this) {                           \
		if (this->del != nullptr) {                                            \
			for (size_t index = 0; index < len(this); ++index) {               \
				this->del(this->buff[index]);                                  \
			}                                                                  \
		}                                                                      \
		free(this);                                                            \
	}
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
