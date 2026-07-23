#ifndef ARRAY_H
#define ARRAY_H

#define DECL_SINGLE_WORD_TYPE(TYPE, NAME) typedef TYPE NAME
#define darray(TYPE) struct TYPE##_darray

#define DARRAY_DECL_NEW(TYPE)                                                  \
	[[nodiscard]] darray(TYPE) *                                               \
		TYPE##_darray_new(const size_t len, const size_t elem_size)
#define darray_new(TYPE, len) TYPE##_darray_new(len, sizeof(TYPE))

#define DARRAY_DECL_DEL(TYPE) void TYPE##_darray_del(darray(TYPE) *const this)
#define darray_del(TYPE) TYPE##_darray_del

#define DARRAY_DECL_ADD(TYPE)                                                  \
	[[nodiscard]] darray(TYPE) *                                               \
		TYPE##_darray_add(darray(TYPE) * this, TYPE elem)
#define darray_add(TYPE, this, elem) TYPE##_darray_add(this, elem)

#endif
