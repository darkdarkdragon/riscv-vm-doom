#include "unistd.h"
#include "stdio.h"
#include <stdio.h>

int access(const char *pathname, int mode) {
  // fprintf(stderr, "access: unimplemented pathname %s mode %d\n", pathname, mode);
  fprintf(stderr, "access: pathname %s mode %d\n", pathname, mode);

  return syscall(SYS_access, (uint32_t)pathname, mode, 0, 0, 0, 0, 0);
}

off_t lseek(int fildes, off_t offset, int whence) {
  fprintf(stderr, "lseek: unimplemented fildes %d\n", fildes);
  return 0;
}
