#ifndef ARRAY_H
#define ARRAY_H

#define DECL_SINGLE_WORD_TYPE(TYPE, NAME) typedef TYPE NAME
#define darray(TYPE) darray_##TYPE
#define DARRAY_DECL(TYPE)                                                      \
	typedef struct {                                                           \
		metadata hdr;                                                          \
		TYPE buff[];                                                           \
	} darray(TYPE)
#define DARRAY_DECL_NEW(TYPE)                                                  \
	[[nodiscard]] darray(TYPE) *                                               \
		new_darray_##TYPE(const size_t len, const size_t elem_size)
#define new_darray(TYPE, len) new_darray_##TYPE(len, sizeof(TYPE))

#define DARRAY_DECL_DEL(TYPE) void del_darray_##TYPE(darray(TYPE) *const this)
#define del_darray(TYPE) del_darray_##TYPE

#define DARRAY_DECL_ADD(TYPE)                                                  \
	[[nodiscard]] darray(TYPE) *                                               \
		add_darray_##TYPE(darray(TYPE) * this, TYPE elem)
#define add_darray(TYPE, this, elem) add_darray_##TYPE(this, elem)

#endif
