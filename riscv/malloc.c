
#include "malloc.h"

#define MALLOC_TRACE 0

static uint32_t total_allocated = 0;

struct MemChunk {
  struct MemChunk *prev;
  struct MemChunk *next;
  uint32_t size;
};

static struct MemChunk *small_heap_start;
// static struct MemChunk small_heap_end;

// static struct MemChunk big_heap_start;
// static struct MemChunk big_heap_end;

// Static initialization function
// void init_function(void) __attribute__((constructor));
void malloc_init(void) {
  // printf("Initialization function is running before main.\n");
  small_heap_start = (struct MemChunk *)HEAP_START;
  memset(small_heap_start, 0, sizeof(struct MemChunk));
  total_allocated = 0;
  // small_heap_start.next = NULL;
  // small_heap_start.prev = NULL;
  // small_heap_end.prev = &small_heap_start;
  // small_heap_end.next = NULL;
  // big_heap_start.next = &big_heap_end;
  // big_heap_start.prev = NULL;
  // big_heap_end.prev = &big_heap_start;
  // big_heap_end.next = NULL;
}

struct MemChunk *findHole(size_t size) {
  struct MemChunk *res = small_heap_start;
  struct MemChunk *next = NULL;
  while (1) {
#if MALLOC_TRACE
    printf("findHole: %p\n", res);
#endif
    if (res->next == NULL) {
      return res;
    }
    next = res->next;
    int hole = (uint8_t *)next - (uint8_t *)res - sizeof(struct MemChunk) - res->size - 4;
    if (hole >= (int)size) {
      // printf("hole found from %p next %p hole %d\n", res, next, hole);
      return res;
    }
    res = res->next;
  }
}

void *align4(void *ptr) {
  uint32_t addr = (uint32_t)ptr;
  uint32_t aligned = (addr + 3) & ~3;
  return (void *)aligned;
}

void *malloc(size_t __size) {
  // fprintf(stderr, "malloc: unimplemented size %d\n", __size);
  fprintf(stderr, "malloc size %d total %d\n", __size, total_allocated);
  struct MemChunk *before = findHole(__size);
  if (before == NULL) {
    fprintf(stderr, "malloc: no hole found\n");
    return NULL;
  }
  // printf("before %p bs %d memsiezof %d\n", before, before->size, sizeof(struct MemChunk));
  // struct MemChunk *new = align4(before + sizeof(struct MemChunk) + before->size);
  // struct MemChunk *new = (struct MemChunk *)((uint8_t *)before + sizeof(struct MemChunk) + before->size);
  struct MemChunk *new = (struct MemChunk *)(align4((uint8_t *)before + sizeof(struct MemChunk) + before->size));
  if (before->next != NULL) {
    before->next->prev = new;
  }
  new->next = before->next;
  before->next = new;
  new->prev = before;
  new->size = __size;
  void *res = ((uint8_t *)new) + sizeof(struct MemChunk);
  // printf("malloc: %p\n", res);
  total_allocated += __size;
#if MALLOC_TRACE
  printf("malloc res: %p size %d total %d\n", res, __size, total_allocated);
#endif
  return res;
}

void *realloc(void *__ptr, size_t __size) {
  // fprintf(stderr, "realloc: unimplemented size %d ptr %p\n", __size, __ptr);
  fprintf(stderr, "realloc size %d ptr %p\n", __size, __ptr);
  if (__ptr == NULL) {
    return malloc(__size);
  }
  struct MemChunk *cur = (struct MemChunk *)((uint8_t *)__ptr - sizeof(struct MemChunk));
#if MALLOC_TRACE
  printf("realloc cur %p cur size %d\n", cur, cur->size);
#endif
  if (cur->next == NULL) {
    cur->size = __size;
#if MALLOC_TRACE
    printf("realloc left 1\n");
#endif
    return __ptr;
  }
  int gap = (uint8_t *)cur->next - (uint8_t *)cur - sizeof(struct MemChunk) - cur->size - 4;
#if MALLOC_TRACE
  printf("realloc gap %d\n", gap);
#endif
  if (gap >= (int)__size) {
    cur->size = __size;
#if MALLOC_TRACE
    printf("realloc left 2\n");
#endif
    return __ptr;
  }
  void *new = malloc(__size);
  if (new == NULL) {
    return NULL;
  }
  memcpy(new, __ptr, cur->size);
  // struct MemChunk *newChunnk = new-sizeof(struct MemChunk);
  // free current
  cur->prev->next = cur->next;
  cur->next->prev = cur->prev;
  // total_allocated += ;
  total_allocated += __size;
  return new;
}
