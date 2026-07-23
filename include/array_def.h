#ifndef ARRAY_DEF_H
#define ARRAY_DEF_H

#define DARRAY_DECL(TYPE)                                                      \
	darray(TYPE) {                                                             \
		struct metadata hdr;                                                   \
		TYPE buff[];                                                           \
	}

#define DARRAY_DEF_NEW(TYPE)                                                   \
	[[nodiscard]] darray(TYPE) *                                               \
		TYPE##_darray_new(const size_t len, const size_t elem_size) {          \
		darray(TYPE) *const new =                                              \
			malloc(sizeof(darray(TYPE)) + elem_size * len);                    \
		if (new == nullptr) {                                                  \
			return new;                                                        \
		}                                                                      \
                                                                               \
		ref_cap(new) = len;                                                    \
		ref_len(new) = 0;                                                      \
		memset(new->buff, 0, cap(new));                                        \
                                                                               \
		return new;                                                            \
	}

#define DARRAY_DEF_DEL(TYPE)                                                   \
	void TYPE##_darray_del(darray(TYPE) *const this) {                         \
		if (this == nullptr) {                                                 \
			return;                                                            \
		}                                                                      \
                                                                               \
		for (size_t index = 0; index < len(this); ++index) {                   \
			del(this->buff[index]);                                            \
		}                                                                      \
		free(this);                                                            \
	}
#define DARRAY_DEF_ADD(TYPE)                                                   \
	[[nodiscard]] darray(TYPE) *                                               \
		TYPE##_darray_add(darray(TYPE) * this, TYPE elem) {                    \
		if (this == nullptr) {                                                 \
			return nullptr;                                                    \
		}                                                                      \
                                                                               \
		if (cap(this) < len(this) + 1) {                                       \
			const size_t new_cap =                                             \
				cap(this) >= SIZE_MAX >> 1                                     \
					? SIZE_MAX                                                 \
					: max((size_t)1, cap(this)) * (size_t)2;                   \
			darray(TYPE) *const new =                                          \
				realloc(this, sizeof(darray(TYPE)) + new_cap);                 \
			if (new == nullptr) {                                              \
				return new;                                                    \
			}                                                                  \
                                                                               \
			this = new;                                                        \
			ref_cap(this) = new_cap;                                           \
		}                                                                      \
                                                                               \
		this->buff[(ref_len(this))++] = elem;                                  \
                                                                               \
		return this;                                                           \
	}

#define sarray_decl(TYPE)                                                      \
	sarray(TYPE) {                                                             \
		struct metadata hdr;                                                   \
		TYPE *buff;                                                            \
	}

#define const_sarray_decl(TYPE)                                                \
	const_sarray(TYPE) {                                                       \
		struct metadata hdr;                                                   \
		TYPE *const buff;                                                      \
	}

#endif
