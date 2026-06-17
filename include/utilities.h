#include <stddef.h>

#define _max(a, b) ((a) > (b) ? (a) : (b))
[[nodiscard]] inline size_t max_size_t(const size_t a, const size_t b) {
	return a > b ? a : b;
}

#define max(a, b) _Generic(a, typeof(b): _Generic(b, size_t: max_size_t(a, b), default: _max(a, b)))
