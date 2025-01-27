
#include "stdlib.h"
#include "stdio.h"

int __attribute__((weak)) main(int argc, char** argv)
{
  // single-threaded programs override this function.
//   printstr("Implement main(), foo!\n");
  return -1;
}

uintptr_t __attribute__((weak)) handle_trap(uintptr_t cause, uintptr_t epc, uintptr_t regs[32])
{
//  tohost_exit(1337);
}

void *memset(void *dest, int byte, size_t len) {
  if ((((uintptr_t)dest | len) & (sizeof(uintptr_t) - 1)) == 0) {
    uintptr_t word = byte & 0xFF;
    word |= word << 8;
    word |= word << 16;
    word |= word << 16 << 16;

    uintptr_t *d = dest;
    while (d < (uintptr_t *)(dest + len))
      *d++ = word;
  } else {
    char *d = dest;
    while (d < (char *)(dest + len))
      *d++ = byte;
  }
  return dest;
}

void *memcpy(void *dest, const void *src, size_t len) {
  if ((((uintptr_t)dest | (uintptr_t)src | len) & (sizeof(uintptr_t) - 1)) == 0) {
    const uintptr_t *s = src;
    uintptr_t *d = dest;
    uintptr_t *end = dest + len;
    while (d + 8 < end) {
      uintptr_t reg[8] = {s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]};
      d[0] = reg[0];
      d[1] = reg[1];
      d[2] = reg[2];
      d[3] = reg[3];
      d[4] = reg[4];
      d[5] = reg[5];
      d[6] = reg[6];
      d[7] = reg[7];
      d += 8;
      s += 8;
    }
    while (d < end)
      *d++ = *s++;
  } else {
    const char *s = src;
    char *d = dest;
    while (d < (char *)(dest + len))
      *d++ = *s++;
  }
  return dest;
}

long atol(const char *str) {
  long res = 0;
  int sign = 0;

  while (*str == ' ')
    str++;

  if (*str == '-' || *str == '+') {
    sign = *str == '-';
    str++;
  }

  while (*str) {
    res *= 10;
    res += *str++ - '0';
  }

  return sign ? -res : res;
}

char *getenv(const char *) { return 0; }

int my_abs(int x) { return x < 0 ? -x : x; }

void exit(int code) {
  syscall(SYS_exit, (uint32_t)code, 0, 0, 0, 0, 0, 0);
  while (1)
    ;
}

int atoi(const char *str) {
    int result = 0;
    int sign = 1;

    // Skip leading whitespace
    while (isspace_custom(*str)) {
        str++;
    }

    // Handle optional sign
    if (*str == '+' || *str == '-') {
        if (*str == '-') {
            sign = -1;
        }
        str++;
    }

    // Convert digits to integer
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }

    return result * sign;
}


void _init(int cid, int nc) {
//   init_tls();
//   thread_entry(cid, nc);

  // only single-threaded programs should ever get here.
  int ret = main(0, 0);

//   char buf[NUM_COUNTERS * 32] __attribute__((aligned(64)));
//   char *pbuf = buf;
//   for (int i = 0; i < NUM_COUNTERS; i++)
//     if (counters[i])
//       pbuf += sprintf(pbuf, "%s = %d\n", counter_names[i], counters[i]);
//   if (pbuf != buf)
//     printstr(buf);

  exit(ret);
}
