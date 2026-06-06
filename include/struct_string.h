#include <stddef.h>

struct string {
	size_t cap;
	size_t len;
	char buff[];
};
struct string_literal {
	size_t cap; // not `const` since we may assign multiple string literals to the same variable.
	size_t len;
	const char *buff; // `const` since we will assign a string literal value and they cannot be modified in C.
};

#define string_literal_new(str) \
	(struct string_literal) { .cap = sizeof str - 1, .len = sizeof str - 1, .buff = str }
