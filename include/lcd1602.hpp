#ifndef LCD1602_HPP
#define LCD1602_HPP

#include <stdint.h>

void lcd_init();
void lcd_clear();
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_write_char(char c);
void lcd_print(const char* text);
void lcd_print_int(uint16_t value);

#endif // LCD1602_HPP