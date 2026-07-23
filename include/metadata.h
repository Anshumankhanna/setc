#ifndef METADATA_H
#define METADATA_H

struct metadata;

[[nodiscard, unsequenced]] size_t
_cap(const struct metadata *const restrict hdr);
[[nodiscard, unsequenced]] size_t
_len(const struct metadata *const restrict hdr);

[[nodiscard]] size_t *_ref_cap(struct metadata *const restrict hdr);
[[nodiscard]] size_t *_ref_len(struct metadata *const restrict hdr);

#define hdr(ptr)                                                               \
	_Generic(ptr,                                                              \
		const struct dstring *: hdr_string,                                    \
		const struct sstring *: hdr_string,                                    \
		const struct lstring *: hdr_string,                                    \
		const darray(struct_dstring_p) *: hdr_darray,                          \
                                                                               \
		struct dstring *: hdr_string,                                          \
		struct sstring *: hdr_string,                                          \
		struct lstring *: hdr_string,                                          \
		darray(struct_dstring_p) *: hdr_darray)(ptr)

#define cap(ptr) _cap(hdr(ptr))
#define len(ptr) _len(hdr(ptr))

#define ref_cap(ptr) _ref_cap(hdr(ptr))
#define ref_len(ptr) _ref_len(hdr(ptr))

#endif
