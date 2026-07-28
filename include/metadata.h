#ifndef METADATA_H
#define METADATA_H

typedef struct {
	size_t cap;
	size_t len;
} metadata;
typedef struct {
	const size_t cap;
	size_t len;
} metadata_const_cap;

[[nodiscard, unsequenced]] size_t _cap(const metadata *const restrict hdr);
[[nodiscard, unsequenced]] size_t _len(const metadata *const restrict hdr);

[[unsequenced]] void inline _set_cap(metadata *const restrict hdr, const size_t new_cap) {
	hdr->cap = new_cap;
}
[[unsequenced]] void inline _set_len(metadata *const restrict hdr, const size_t new_len) {
	hdr->len = new_len;
}

#define get_hdr(ptr)                                                           \
	_Generic(ptr,                                                              \
		const struct dstring *: hdr_c_dstring,                                 \
		const struct sstring *: hdr_c_sstring,                                 \
		const struct lstring *: hdr_lstring,                                   \
		struct dstring *: hdr_dstring,                                         \
		struct sstring *: hdr_sstring)(ptr)

#define cap(ptr) _cap(get_hdr(ptr))
#define len(ptr) _len(get_hdr(ptr))

#define set_cap(ptr, len) _set_cap(get_hdr(ptr), len)
#define set_len(ptr, len) _set_len(get_hdr(ptr), len)

#endif
