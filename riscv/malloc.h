#ifndef __MALLOC__
#define __MALLOC__

#include <stdint-gcc.h>

typedef uint32_t size_t;

#ifndef NULL
#define NULL 0
#endif

void *malloc(size_t __size);
void *realloc(void * __ptr, size_t __size);

#endif /* __MALLOC__ */
