#include "metadata.h"
#include "metadata_def.h"

[[nodiscard, unsequenced]] size_t
_cap(const struct metadata *const restrict hdr) {
	return hdr->cap;
}
[[nodiscard, unsequenced]] size_t
_len(const struct metadata *const restrict hdr) {
	return hdr->len;
}
[[nodiscard]] size_t *_ref_cap(struct metadata *const restrict hdr) {
	return &hdr->cap;
}

[[nodiscard]] size_t *_ref_len(struct metadata *const restrict hdr) {
	return &hdr->len;
}
