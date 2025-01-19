
#include "string.h"


size_t strlen(const char *s)
{
  const char *p = s;
  while (*p)
    p++;
  return p - s;
}

size_t strnlen(const char *s, size_t n)
{
  const char *p = s;
  while (n-- && *p)
    p++;
  return p - s;
}

int strcmp(const char* s1, const char* s2)
{
  unsigned char c1, c2;

  do {
    c1 = *s1++;
    c2 = *s2++;
  } while (c1 != 0 && c1 == c2);

  return c1 - c2;
}

char* strcpy(char* dest, const char* src)
{
  char* d = dest;
  while ((*d++ = *src++))
    ;
  return dest;
}

void strupr(char *str) {
    while (*str) {
        *str = toupper(*str);
        str++;
    }
}

char *strncpy(char *dest, const char *src, size_t n) {
    // @TODO
    return NULL;
}


int strncasecmp(const char *, const char *, size_t) {
    // @TODO
    return -1;
}

int strcasecmp(const char *, const char *) {
    // @TODO
    return -1;
}

int strncmp(const char *__s1, const char *__s2, size_t __n) {
    // @TODO
    return -1;
}
