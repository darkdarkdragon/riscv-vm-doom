#ifndef __FCNTL__
#define __FCNTL__

#include "stdio.h"

/* open-only flags */
#define O_RDONLY 0x0000  /* open for reading only */
#define O_WRONLY 0x0001  /* open for writing only */
#define O_RDWR 0x0002    /* open for reading and writing */
#define O_ACCMODE 0x0003 /* mask for above modes */

#define O_SHLOCK 0x00000010   /* open with shared file lock */
#define O_EXLOCK 0x00000020   /* open with exclusive file lock */
#define O_ASYNC 0x00000040    /* signal pgrp when data ready */
#define O_FSYNC O_SYNC        /* source compatibility: do not use */
#define O_NOFOLLOW 0x00000100 /* don't follow symlinks */

#define O_CREAT 0x00000200 /* create if nonexistant */
#define O_TRUNC 0x00000400 /* truncate to zero length */
#define O_EXCL 0x00000800  /* error if already exists */

int open(const char *, int, ...);
int close(int);
size_t read(int, void *, size_t);
size_t write(int __fd, const void *__buf, size_t __nbyte);

#endif /* __FCNTL__ */
