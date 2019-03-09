#include <am.h>
#include <amdev.h>

#define SIDE 8

static inline void puts(const char *s) {
  for (; *s; s++) _putc(*s);
}
