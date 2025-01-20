
#include "malloc.h"
#include <stdio.h>

void *malloc(size_t __size) {
  fprintf(stderr, "malloc: unimplemented size %d\n", __size);
  return NULL;
}

void *realloc(void *__ptr, size_t __size) {
  fprintf(stderr, "realloc: unimplemented size %d ptr %p\n", __size, __ptr);
  return NULL;
}
