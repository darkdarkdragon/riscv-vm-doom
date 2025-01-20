#include "fcntl.h"
#include <stdio.h>

int open(const char *name, int mode, ...) {
  fprintf(stderr, "open: unimplemented name %s\n", name);
  return 0;
}

int close(int fd) {
  fprintf(stderr, "close: unimplemented fd %d\n", fd);

  return 0;
}

size_t read(int fd, void *buf, size_t size) {
  fprintf(stderr, "read: unimplemented fd %d\n", fd);
  return 0;
}

size_t write(int __fd, const void *__buf, size_t __nbyte) {
  fprintf(stderr, "write: unimplemented fd %d\n", __fd);
  return 0;
}
