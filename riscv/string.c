
#include "string.h"
#include <stdio.h>


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

int my_toupper(int c) {
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A'); // Convert lowercase to uppercase
    }
    return c; // Return unchanged if not lowercase
}

void strupr(char *str) {
    while (*str) {
        *str = my_toupper(*str);
        str++;
    }
}


char *strncpy(char *dest, const char *src, size_t n) {
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    // Fill the remaining space with null bytes if `src` is shorter than `n`.
    for (; i < n; i++) {
        dest[i] = '\0';
    }

    return dest;
}



int my_tolower(int c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A'); // Convert uppercase to lowercase
    }
    return c; // Return the character unchanged if it's not uppercase
}

int strcasecmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        char c1 = my_tolower((unsigned char)*s1);
        char c2 = my_tolower((unsigned char)*s2);

        if (c1 != c2) {
            return (unsigned char)c1 - (unsigned char)c2;
        }

        s1++;
        s2++;
    }

    // Compare the final characters (accounting for string length differences)
    return (unsigned char)my_tolower((unsigned char)*s1) - (unsigned char)my_tolower((unsigned char)*s2);
}

// Custom strncasecmp function
int strncasecmp(const char *s1, const char *s2, size_t n) {
    if (n == 0) {
        return 0; // No characters to compare
    }

    while (n-- > 0 && *s1 && *s2) {
        char c1 = my_tolower((unsigned char)*s1);
        char c2 = my_tolower((unsigned char)*s2);

        if (c1 != c2) {
            return (unsigned char)c1 - (unsigned char)c2;
        }

        s1++;
        s2++;
    }

    // If one string ends before the other
    if (n != (size_t)-1) {
        return (unsigned char)my_tolower((unsigned char)*s1) -
               (unsigned char)my_tolower((unsigned char)*s2);
    }

    return 0;
}


int strncmp(const char *s1, const char *s2, size_t n) {
    if (n == 0) {
        return 0; // No characters to compare
    }

    while (n-- > 0) {
        if (*s1 != *s2 || *s1 == '\0' || *s2 == '\0') {
            return (unsigned char)*s1 - (unsigned char)*s2;
        }

        s1++;
        s2++;
    }

    return 0; // Strings are equal for the first n characters
}
