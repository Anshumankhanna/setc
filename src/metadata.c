#include "metadata.h"

// Since we have defined these functions to be inline in `metadata.h` but they may not be inlined at low optimization, we need a file to provide global symbols to call,
// This is the point of this file,
// We declare an `extern` function and include the definition from `metadata.h` by importing it.
DECLARE_PRIVATE_FIELD_FUNCS(cap)
DECLARE_PRIVATE_FIELD_FUNCS(len)
