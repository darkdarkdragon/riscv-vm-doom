#ifndef __MALLOC__
#define __MALLOC__

#include <stdint-gcc.h>
#include <stdio.h>

typedef uint32_t size_t;

#ifndef NULL
#define NULL 0
#endif

// 1MB
#define HEAP_START 1048576
#define HEAP_SIZE 1048576

#define TOTAL_MEMORY 1048576 * 32

void malloc_init(void);
void *malloc(size_t __size);
void *realloc(void *__ptr, size_t __size);

#endif /* __MALLOC__ */
