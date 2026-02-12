#include <stdint.h>

#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define WHITE 0x3

#define X_WIDTH 320
#define Y_HEIGHT 240

bool init_graphics();

void prepare_for_data_tx();
void do_data_tx();

void fill_screen(uint8_t color);
void draw_pixel(uint16_t x, uint16_t y, uint8_t color);
void draw_horz_line(uint16_t x_start, uint16_t y_start, uint16_t length, uint8_t color);
void fill_rect(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, uint8_t color);
