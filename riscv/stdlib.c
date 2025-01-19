
#include "stdlib.h"

void *memset(void *dest, int byte, size_t len) {
  if ((((uintptr_t)dest | len) & (sizeof(uintptr_t) - 1)) == 0) {
    uintptr_t word = byte & 0xFF;
    word |= word << 8;
    word |= word << 16;
    word |= word << 16 << 16;

    uintptr_t *d = dest;
    while (d < (uintptr_t *)(dest + len))
      *d++ = word;
  } else {
    char *d = dest;
    while (d < (char *)(dest + len))
      *d++ = byte;
  }
  return dest;
}

void *memcpy(void *dest, const void *src, size_t len) {
  if ((((uintptr_t)dest | (uintptr_t)src | len) & (sizeof(uintptr_t) - 1)) ==
      0) {
    const uintptr_t *s = src;
    uintptr_t *d = dest;
    uintptr_t *end = dest + len;
    while (d + 8 < end) {
      uintptr_t reg[8] = {s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]};
      d[0] = reg[0];
      d[1] = reg[1];
      d[2] = reg[2];
      d[3] = reg[3];
      d[4] = reg[4];
      d[5] = reg[5];
      d[6] = reg[6];
      d[7] = reg[7];
      d += 8;
      s += 8;
    }
    while (d < end)
      *d++ = *s++;
  } else {
    const char *s = src;
    char *d = dest;
    while (d < (char *)(dest + len))
      *d++ = *s++;
  }
  return dest;
}

long atol(const char *str) {
  long res = 0;
  int sign = 0;

  while (*str == ' ')
    str++;

  if (*str == '-' || *str == '+') {
    sign = *str == '-';
    str++;
  }

  while (*str) {
    res *= 10;
    res += *str++ - '0';
  }

  return sign ? -res : res;
}

char *getenv(const char *) { return 0; }

int my_abs(int x) { return x < 0 ? -x : x; }
