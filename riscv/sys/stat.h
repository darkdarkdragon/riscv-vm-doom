
#ifndef __SYSSTAT__
#define __SYSSTAT__

#include <stdint-gcc.h>
#include <stdlib.h>

typedef uint16_t mode_t;

struct stat {
  size_t st_size;           /* [XSI] file size, in bytes */
};

int fstat(int filedes, struct stat *buf);
int mkdir(const char *pathname, mode_t mode);

#endif // __SYSSTAT__
