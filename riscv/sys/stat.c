
#include "sys/stat.h"

#include "stdio.h"
#include <sys/stat.h>

int fstat(int filedes, struct stat *buf) {
  fprintf(stderr, "fstat not implemented file %d\n", filedes);
  return 0;
}

int mkdir(const char *pathname, mode_t mode) {
  fprintf(stderr, "mkdir not implemented path %s\n", pathname);
  return 0;
}