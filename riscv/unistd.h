
#ifndef __UNISTD__
#define __UNISTD__

#include <stdint-gcc.h>

/* access function */
#define F_OK 0        /* test for existence of file */
#define X_OK (1 << 0) /* test for execute or search permission */
#define W_OK (1 << 1) /* test for write permission */
#define R_OK (1 << 2) /* test for read permission */

typedef int32_t off_t;

int access(const char *pathname, int mode);
off_t lseek(int fildes, off_t offset, int whence);

#endif
