#include "fcntl.h"
#include <stdio.h>

int open(const char *name, int mode, ...) {
  uint32_t res = syscall(SYS_open, (uint32_t)name, (uint32_t)mode, 0, 0, 0, 0, 0);
  // fprintf(stderr, "open: name %s mode %d\n", name, mode, (int)res);
  return (int)res;
}

int close(int fd) {
  uint32_t res = syscall(SYS_close, (uint32_t)fd, 0, 0, 0, 0, 0, 0);
  // fprintf(stderr, "close: fd %d res %d\n", fd, (int)res);
  return (int)res;
}

size_t read(int fd, void *buf, size_t size) {
  uint32_t res = syscall(SYS_read, (uint32_t)fd, (uint32_t)buf, size, 0, 0, 0, 0);
  // fprintf(stderr, "read: fd %d buf %p size %d res %d\n", fd, buf, size, (int)res);
  return (int)res;
}

size_t write(int __fd, const void *__buf, size_t __nbyte) {
  fprintf(stderr, "write: unimplemented fd %d\n", __fd);
  return 0;
}
