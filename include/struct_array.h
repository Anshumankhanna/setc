#ifndef ARRAY_H
#define ARRAY_H

#define decl_single_word_type(TYPE, NAME) typedef TYPE NAME
#define array(TYPE) struct TYPE##_array

#define array_decl(TYPE)                                                       \
	struct array(TYPE) {                                                       \
		struct metadata hdr;                                                   \
		TYPE *buff;                                                            \
	}

#define array_new(TYPE)                                                        \
	[[nodiscard]] TYPE##_array *TYPE##_array_new(                              \
		const TYPE *const restrict _buff, const size_t _len) {                 \
		array(TYPE) *new =                                                     \
			malloc(sizeof(struct metadata) + sizeof(TYPE) * _len);             \
		if (new == nullptr) {                                                  \
			return new;                                                        \
		}                                                                      \
                                                                               \
		*ref_cap(new) = *ref_len(new) = _len;                                  \
		if (_buff != nullptr || _len == 0) {                                   \
			memcpy(new->buff, _buff, _len);                                    \
		}                                                                      \
                                                                               \
		return new;                                                            \
	}
#define array_del(TYPE)                                                        \
	static inline void TYPE##_array_del(array(TYPE) * _ptr) {                  \
		if (_ptr != nullptr) {                                                 \
			free_sized(_ptr,                                                   \
					   sizeof(struct metadata) + sizeof(TYPE) * _ptr->cap);    \
		}                                                                      \
	}

#endif
