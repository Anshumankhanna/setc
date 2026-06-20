#ifndef ARRAY_H
#define ARRAY_H

#include "metadata.h"
#include "utilities.h"

#define DECL_SINGLE_WORD_TYPE(TYPE, NAME) typedef TYPE NAME

#define darray(TYPE) struct TYPE##_darray
#define DARRAY_DECL(TYPE)                                                      \
	darray(TYPE) {                                                             \
		struct metadata hdr;                                                   \
		TYPE buff[];                                                           \
	}

#define DARRAY_DECL_NEW(TYPE)                                                  \
	[[nodiscard]] darray(TYPE) *                                               \
		TYPE##_darray_new(const size_t len, const size_t elem_size)
#define DARRAY_DEF_NEW(TYPE)                                                   \
	[[nodiscard]] darray(TYPE) *                                               \
		TYPE##_darray_new(const size_t len, const size_t elem_size) {          \
		darray(TYPE) *const new =                                              \
			malloc(sizeof(darray(TYPE)) + elem_size * len);                    \
		if (new == nullptr) {                                                  \
			return new;                                                        \
		}                                                                      \
                                                                               \
		*ref_cap(new) = len;                                                   \
		*ref_len(new) = 0;                                                     \
		memset(new->buff, 0, cap(new));                                        \
                                                                               \
		return new;                                                            \
	}
#define darray_new(TYPE, len) TYPE##_darray_new(len, sizeof(TYPE))

#define DARRAY_DECL_DEL(TYPE) void TYPE##_darray_del(darray(TYPE) *const this)
#define DARRAY_DEF_DEL(TYPE)                                                   \
	void TYPE##_darray_del(darray(TYPE) *const this) {                         \
		for (size_t index = 0; index < len(this); ++index) {                   \
			del(this->buff[index]);                                            \
		}                                                                      \
		free(this);                                                             \
	}
#define darray_del(TYPE) TYPE##_darray_del

#define DARRAY_DECL_ADD(TYPE)                                                  \
	[[nodiscard]] darray(TYPE) *                                               \
		TYPE##_darray_add(darray(TYPE) * this, TYPE elem)
#define DARRAY_DEF_ADD(TYPE)                                                   \
	[[nodiscard]] darray(TYPE) *                                               \
		TYPE##_darray_add(darray(TYPE) * this, TYPE elem) {                    \
		if (cap(this) < len(this) + 1) {                                       \
			const size_t new_cap = cap(this) >= SIZE_MAX >> 1                  \
									   ? SIZE_MAX                              \
									   : max(1ull, cap(this)) * 2ull;          \
			darray(TYPE) *const new =                                          \
				realloc(this, sizeof(darray(TYPE)) + new_cap);                 \
			if (new == nullptr) {                                              \
				return new;                                                    \
			}                                                                  \
                                                                               \
			this = new;                                                        \
			*ref_cap(this) = new_cap;                                          \
		}                                                                      \
                                                                               \
		this->buff[(*ref_len(this))++] = elem;                                 \
                                                                               \
		return this;                                                           \
	}
#define darray_add(TYPE, this, elem) TYPE##_darray_add(this, elem)

#define sarray(TYPE) struct TYPE##_sarray
#define sarray_decl(TYPE)                                                      \
	sarray(TYPE) {                                                             \
		struct metadata hdr;                                                   \
		TYPE *buff;                                                            \
	}

#define const_sarray(TYPE) struct constTYPE##_sarray
#define const_sarray_decl(TYPE)                                                \
	const_sarray(TYPE) {                                                       \
		struct metadata hdr;                                                   \
		TYPE *const buff;                                                      \
	}

#endif
