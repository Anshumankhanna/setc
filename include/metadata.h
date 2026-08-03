#ifndef METADATA_H
#define METADATA_H

#include <stddef.h>

typedef struct {
	size_t cap;
	size_t len;
} metadata ;


inline void _set_cap(metadata *const restrict hdr, const size_t new_cap) {
	hdr->cap = new_cap;
}
inline void _set_len(metadata *const restrict hdr, const size_t new_len) {
	hdr->len = new_len;
}

[[nodiscard]] inline size_t _cap(const metadata *const restrict hdr) {
	return hdr->cap;
}
[[nodiscard]] inline size_t _len(const metadata *const restrict hdr) {
	return hdr->len;
}

[[nodiscard]] inline metadata reserve(const size_t size) {
	return (metadata) { .cap = size, .len = 0 };
}
[[nodiscard]] inline metadata fill(const size_t size) {
	return (metadata) { .cap = size, .len = size };
}

#define set_cap(ptr, size) _Generic(&(ptr)->hdr, metadata *: _set_cap)(&(ptr)->hdr, size)
#define set_len(ptr, size) _Generic(&(ptr)->hdr, metadata *: _set_len)(&(ptr)->hdr, size)

#define cap(ptr) _Generic(&(ptr)->hdr, const metadata *: _cap, metadata *: _cap)(&(ptr)->hdr)
#define len(ptr) _Generic(&(ptr)->hdr, const metadata *: _len, metadata *: _len)(&(ptr)->hdr)

#endif
