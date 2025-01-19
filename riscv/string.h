#ifndef __STRING__
#define __STRING__

#include "stdlib.h"
#include <ctype.h>

size_t strlen(const char *s);
size_t strnlen(const char *s, size_t n);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
// char toupper(const char c);
char *strcat(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
int strcasecmp(const char *, const char *);
int strncasecmp(const char *, const char *, size_t);
int strncmp(const char *__s1, const char *__s2, size_t __n);
void strupr(char *str);

#endif /* __STRING__ */
