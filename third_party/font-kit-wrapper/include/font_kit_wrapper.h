#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct FontBuffer {
  uint8_t *data;
  uintptr_t len;
} FontBuffer;

void free_font_buffer(struct FontBuffer buffer);

struct FontBuffer find_system_font(const char *font_name_raw);
