#ifndef __MALLOC__
#define __MALLOC__

#include <stdint-gcc.h>

typedef uint32_t size_t;

void *malloc(size_t __size);
void *realloc(void * __ptr, size_t __size);

#endif /* __MALLOC__ */
