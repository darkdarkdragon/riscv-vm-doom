#ifndef __STDLIB__
#define __STDLIB__

#include <stdint-gcc.h>

#define NULL (void *)0

typedef uint32_t size_t;

void *malloc(size_t __size);
void *memset(void *dest, int byte, size_t len);
void *memcpy(void *dest, const void *src, size_t len);
int atoi(const char *str);
long atol(const char *str);
void exit(int);
char *getenv(const char *);
int abs(int x);
void _init(int cid, int nc);

#endif /* __STDLIB__ */
