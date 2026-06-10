// We have to enable this guard since we are enabling `inline` functions and they require having definitions in the same file that they are declared.
#ifndef METADATA_H
#define METADATA_H

#include <stddef.h>

struct metadata {
	size_t cap;
	size_t len;
};

#define DEFINE_PRIVATE_FIELD_FUNCS(FIELD) \
	[[nodiscard]] inline size_t _##FIELD(const struct metadata *const restrict hdr) {\
		return hdr -> FIELD;\
	}\
	[[nodiscard]] inline size_t *_ref_##FIELD(struct metadata *const restrict hdr) {\
		return &hdr -> FIELD;\
	}
#define DECLARE_PRIVATE_FIELD_FUNCS(FIELD) \
	[[nodiscard]] extern inline size_t _##FIELD(const struct metadata *const restrict hdr);\
	[[nodiscard]] extern inline size_t *_ref_##FIELD(struct metadata *const restrict hdr);

#define const_metadata_const_cast(ptr) (const struct metadata *const) ptr
#define metadata_const_cast(ptr) (struct metadata *const) ptr

DEFINE_PRIVATE_FIELD_FUNCS(cap)
#define cap(ptr) _cap(const_metadata_const_cast(ptr))
#define ref_cap(ptr) _ref_cap(metadata_const_cast(ptr))

DEFINE_PRIVATE_FIELD_FUNCS(len)
#define len(ptr) _len(const_metadata_const_cast(ptr))
#define ref_len(ptr) _ref_len(metadata_const_cast(ptr))

#endif
