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

DEFINE_PRIVATE_FIELD_FUNCS(cap)
#define cap(ptr) _cap((struct metadata *) ptr)
#define ref_cap(ptr) _ref_cap((struct metadata *) ptr)

DEFINE_PRIVATE_FIELD_FUNCS(len)
#define len(ptr) _len((struct metadata *) ptr)
#define ref_len(ptr) _ref_len((struct metadata *) ptr)

#endif
