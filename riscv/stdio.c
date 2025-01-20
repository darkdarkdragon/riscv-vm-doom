
#include <stdarg.h>

#include "stdio.h"
#include "string.h"

#define SYS_write 64
#define CHAR_BIT 8
// typedef uint32_t uintptr_t;

// static uint32_t syscall(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t arg5, uint32_t arg6, uint32_t
// which) {
//   return 0;
// }
static uint32_t syscall(uint32_t which, uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t arg5,
                        uint32_t arg6) {
  uint32_t result;
  asm volatile("mv a0, %1\n"  // Load arg0 into a0
               "mv a1, %2\n"  // Load arg1 into a1
               "mv a2, %3\n"  // Load arg2 into a2
               "mv a3, %4\n"  // Load arg3 into a3
               "mv a4, %5\n"  // Load arg4 into a4
               "mv a5, %6\n"  // Load arg5 into a5
               "mv a6, %7\n"  // Load arg6 into a6
               "mv a7, %8\n"  // Load which (system call number) into a7
               "ecall\n"      // Make the system call
               "mv %0, a0\n"  // Retrieve the result from a0
               : "=r"(result) // Output: result
               : "r"(arg0), "r"(arg1), "r"(arg2), "r"(arg3), "r"(arg4), "r"(arg5), "r"(arg6), "r"(which) // Inputs
               : "memory", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"                                // Clobbered registers
  );
  return result;
}

// void printstr(const char *s) { syscall(SYS_write, 1, (uintptr_t)s, strlen(s)); }

int putchar(int ch) {
  static __thread char buf[64] __attribute__((aligned(64)));
  static __thread int buflen = 0;

  buf[buflen++] = ch;

  if (ch == '\n' || buflen == sizeof(buf)) {
    syscall(SYS_write, stdout_fd, (uintptr_t)buf, buflen, 0, 0, 0, 0);
    buflen = 0;
  }

  return 0;
}

int putchar_err(int ch) {
  static __thread char buf[64] __attribute__((aligned(64)));
  static __thread int buflen = 0;

  buf[buflen++] = ch;

  if (ch == '\n' || buflen == sizeof(buf)) {
    syscall(SYS_write, stderr_fd, (uintptr_t)buf, buflen, 0, 0, 0, 0);
    buflen = 0;
  }

  return 0;
}

/*
void printhex(uint64_t x) {
  char str[17];
  int i;
  for (i = 0; i < 16; i++) {
    str[15 - i] = (x & 0xF) + ((x & 0xF) < 10 ? '0' : 'a' - 10);
    x >>= 4;
  }
  str[16] = 0;

  printstr(str);
}
*/

static inline void printnum(void (*putch)(int, void **), void **putdat, unsigned long long num, unsigned base, int width, int padc) {
  unsigned digs[sizeof(num) * CHAR_BIT];
  int pos = 0;

  while (1) {
    digs[pos++] = num % base;
    if (num < base)
      break;
    num /= base;
  }

  while (width-- > pos)
    putch(padc, putdat);

  while (pos-- > 0)
    putch(digs[pos] + (digs[pos] >= 10 ? 'a' - 10 : '0'), putdat);
}

static unsigned long long getuint(va_list *ap, int lflag) {
  if (lflag >= 2)
    return va_arg(*ap, unsigned long long);
  else if (lflag)
    return va_arg(*ap, unsigned long);
  else
    return va_arg(*ap, unsigned int);
}

static long long getint(va_list *ap, int lflag) {
  if (lflag >= 2)
    return va_arg(*ap, long long);
  else if (lflag)
    return va_arg(*ap, long);
  else
    return va_arg(*ap, int);
}

static void vprintfmt(void (*putch)(int, void **), void **putdat, const char *fmt, va_list ap) {
  register const char *p;
  const char *last_fmt;
  register int ch, err;
  unsigned long long num;
  int base, lflag, width, precision, altflag;
  char padc;

  while (1) {
    while ((ch = *(unsigned char *)fmt) != '%') {
      if (ch == '\0')
        return;
      fmt++;
      putch(ch, putdat);
    }
    fmt++;

    // Process a %-escape sequence
    last_fmt = fmt;
    padc = ' ';
    width = -1;
    precision = -1;
    lflag = 0;
    altflag = 0;
  reswitch:
    switch (ch = *(unsigned char *)fmt++) {

    // flag to pad on the right
    case '-':
      padc = '-';
      goto reswitch;

    // flag to pad with 0's instead of spaces
    case '0':
      padc = '0';
      goto reswitch;

    // width field
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      for (precision = 0;; ++fmt) {
        precision = precision * 10 + ch - '0';
        ch = *fmt;
        if (ch < '0' || ch > '9')
          break;
      }
      goto process_precision;

    case '*':
      precision = va_arg(ap, int);
      goto process_precision;

    case '.':
      if (width < 0)
        width = 0;
      goto reswitch;

    case '#':
      altflag = 1;
      goto reswitch;

    process_precision:
      if (width < 0)
        width = precision, precision = -1;
      goto reswitch;

    // long flag (doubled for long long)
    case 'l':
      lflag++;
      goto reswitch;

    // character
    case 'c':
      putch(va_arg(ap, int), putdat);
      break;

    // string
    case 's':
      if ((p = va_arg(ap, char *)) == NULL)
        p = "(null)";
      if (width > 0 && padc != '-')
        for (width -= strnlen(p, precision); width > 0; width--)
          putch(padc, putdat);
      for (; (ch = *p) != '\0' && (precision < 0 || --precision >= 0); width--) {
        putch(ch, putdat);
        p++;
      }
      for (; width > 0; width--)
        putch(' ', putdat);
      break;

    // (signed) decimal
    case 'd':
      num = getint(&ap, lflag);
      if ((long long)num < 0) {
        putch('-', putdat);
        num = -(long long)num;
      }
      base = 10;
      goto signed_number;

    // unsigned decimal
    case 'u':
      base = 10;
      goto unsigned_number;

    // (unsigned) octal
    case 'o':
      // should do something with padding so it's always 3 octits
      base = 8;
      goto unsigned_number;

    // pointer
    case 'p':
      _Static_assert(sizeof(long) == sizeof(void *));
      lflag = 1;
      putch('0', putdat);
      putch('x', putdat);
      /* fall through to 'x' */

    // (unsigned) hexadecimal
    case 'x':
      base = 16;
    unsigned_number:
      num = getuint(&ap, lflag);
    signed_number:
      printnum(putch, putdat, num, base, width, padc);
      break;

    // escaped '%' character
    case '%':
      putch(ch, putdat);
      break;

    // unrecognized escape sequence - just print it literally
    default:
      putch('%', putdat);
      fmt = last_fmt;
      break;
    }
  }
}

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  vprintfmt((void *)putchar, 0, fmt, ap);

  va_end(ap);
  return 0; // incorrect return value, but who cares, anyway?
}

int sprintf(char *str, const char *fmt, ...) {
  va_list ap;
  char *str0 = str;
  va_start(ap, fmt);

  void sprintf_putch(int ch, void **data) {
    char **pstr = (char **)data;
    **pstr = ch;
    (*pstr)++;
  }

  vprintfmt(sprintf_putch, (void **)&str, fmt, ap);
  *str = 0;

  va_end(ap);
  return str - str0;
}

// int fprintf(FILE *__restrict, const char *__restrict, ...)
int fprintf(FILE *stream, const char *fmt, ...) {
  if (stream != stdout && stream != stderr) {
    fprintf(stderr, "fprintf: invalid stream %d\n", stream);
    return 0;
  }
  va_list ap;
  va_start(ap, fmt);

  void *pc = (void *)putchar;
  if (stream == stderr) {
    pc = (void *)putchar_err;
  }

  vprintfmt(pc, 0, fmt, ap);

  va_end(ap);
  return 0; // incorrect return value, but who cares, anyway?
}

int feof(FILE *file) {
  fprintf(stderr, "feof: unimplemented file %d\n", file);
  return 0;
}

int fscanf(FILE *stream, const char *format, ...) {
  fprintf(stderr, "fscanf: unimplemented stream %d\n", stream);
  return 0;
}

int sscanf(const char *__restrict str, const char *__restrict format, ...) {
  fprintf(stderr, "sscanf: unimplemented string %s\n", str);
  return 0;
}

void setbuf(FILE *restrict stream, char *restrict buffer) { fprintf(stderr, "setbuf: unimplemented stream %d\n", stream); }

int getchar(void) {
  fprintf(stderr, "getchar: unimplemented\n");
  return 0;
}

FILE *fopen(const char *__restrict __filename, const char *__restrict __mode) {
  fprintf(stderr, "fopen: unimplemented filename %s\n", __filename);
  return 0;
}

int fclose(FILE *f) {
  fprintf(stderr, "fclose: unimplemented file %d\n", f);
  return 0;
}

size_t fread(void *__restrict __ptr, size_t __size, size_t __nitems, FILE *__restrict __stream) {
  fprintf(stderr, "fread: unimplemented file %d\n", __stream);
  return 0;
}

long ftell(FILE *f) {
  fprintf(stderr, "ftell: unimplemented file %d\n", f);
  return 0;
}

int fseek(FILE *f, long a1, int a2) {
  fprintf(stderr, "fseek: unimplemented file %d\n", f);
  return 0;
}

int fflush(FILE *stream) {
  fprintf(stderr, "fflush: unimplemented stream %d\n", stream);
  return 0;
}