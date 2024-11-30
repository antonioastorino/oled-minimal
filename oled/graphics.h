#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <Arduino.h>
#define DISPLAY_ROWS (32)
#define DISPLAY_COLS (128)
#define DISPLAY_PAGES (DISPLAY_ROWS / 8)

void graphics_add_pixel(uint8_t x, uint8_t y);
void graphics_clear_buffer();
uint8_t (*graphics_get_bitmap())[DISPLAY_COLS][DISPLAY_PAGES];
void graphics_add_inline_symbol(uint8_t cursor_position, uint8_t page, uint8_t symbol[8]);
#endif /* GRAPHICS_H */
