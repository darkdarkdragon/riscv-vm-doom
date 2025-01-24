
#include "sys/stat.h"

#include "stdio.h"
#include <sys/stat.h>

int fstat(int filedes, struct stat *buf) {
  uint32_t res = syscall(SYS_fstat, (uint32_t)filedes, (uint32_t)buf, 0, 0, 0, 0, 0);
  // fprintf(stderr, "fstat file %d res %d size %d\n", filedes, (int)res, buf->st_size);
  printf("fstat file %d res %d size %d\n", filedes, (int)res, buf->st_size);
  return (int)res;
}

int mkdir(const char *pathname, mode_t mode) {
  fprintf(stderr, "mkdir not implemented path %s\n", pathname);
  return 0;
}