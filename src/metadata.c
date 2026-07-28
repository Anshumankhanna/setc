#include "metadata.h"

[[nodiscard, unsequenced]] size_t extern inline
_cap(const metadata *const restrict hdr);
[[nodiscard, unsequenced]] size_t extern inline
_len(const metadata *const restrict hdr);

[[unsequenced]] void extern inline _set_cap(metadata *const restrict hdr, const size_t new_cap);
[[unsequenced]] void extern inline _set_len(metadata *const restrict hdr, const size_t new_len);
