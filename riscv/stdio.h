
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

#endif /* __STDIO__ */
