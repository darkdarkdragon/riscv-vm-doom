
#ifndef __STDIO__
#define __STDIO__

// #include <stdint.h>
#include "stdlib.h"
#include <stdint-gcc.h>

// typedef int size_t;
typedef int32_t FILE;

#define stdout_fd 1
#define stdin_fd 0
#define stderr_fd 2

#define stdout ((FILE *)1)
#define stdin ((FILE *)0)
#define stderr ((FILE *)2)

#define SEEK_SET 0 /* set file offset to offset */
#define SEEK_CUR 1 /* set file offset to current plus offset */
#define SEEK_END 2 /* set file offset to EOF plus offset */

#define SYS_write 64
#define SYS_exit 93
#define SYS_access 100

int printf(const char *fmt, ...);
int sprintf(char *str, const char *fmt, ...);

void setbuf(FILE *restrict stream, char *restrict buffer);
int getchar(void);

FILE *fopen(const char *__restrict __filename, const char *__restrict __mode);
int fclose(FILE *);
size_t fread(void *__restrict __ptr, size_t __size, size_t __nitems, FILE *__restrict __stream);
long ftell(FILE *);
int fseek(FILE *, long, int);
int fprintf(FILE *__restrict, const char *__restrict, ...);
int feof(FILE *);
int fscanf(FILE *__restrict, const char *__restrict, ...);
int sscanf(const char *__restrict, const char *__restrict, ...);
int fflush(FILE *stream);
uint32_t syscall(uint32_t which, uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t arg5, uint32_t arg6);

#endif /* __STDIO__ */
