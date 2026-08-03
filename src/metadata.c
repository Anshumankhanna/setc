#include "metadata.h"

extern inline void _set_cap(metadata *const restrict hdr, const size_t new_cap);
extern inline void _set_len(metadata *const restrict hdr, const size_t new_len);

[[nodiscard]] extern inline size_t _cap(const metadata *const restrict hdr);
[[nodiscard]] extern inline size_t _len(const metadata *const restrict hdr);


[[nodiscard]] extern inline metadata reserve(const size_t size);
[[nodiscard]] extern inline metadata fill(const size_t size);
