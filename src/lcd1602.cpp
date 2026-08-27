#include "lcd1602.hpp"
#include "pcf8574.hpp"
#include "timer.hpp"
#include <stdlib.h>

static const uint8_t LCD_RS = 0;
static const uint8_t LCD_E  = 2;
static const uint8_t LCD_BL = 3;


static void lcd_send_nibble(uint8_t nibble, bool rs)
{
    // Keep only the lower 4 bits, then move them to P4-P7
    uint8_t value = (nibble & 0x0F) << 4;

    // Turn backlight on
    value |= (1 << LCD_BL);

    // RS = 1 for character data, 0 for commands
    if (rs)
    {
        value |= (1 << LCD_RS);
    }

    // E HIGH
    value |= (1 << LCD_E);
    pcf8574_write(value);

    timer_delay_us(1);

    // E LOW
    value &= ~(1 << LCD_E);
    pcf8574_write(value);

    timer_delay_us(50);
}


static void lcd_send_byte(uint8_t value, bool rs)
{
    // Upper four bits first
    lcd_send_nibble(value >> 4, rs);

    // Lower four bits second
    lcd_send_nibble(value & 0x0F, rs);
}


static void lcd_command(uint8_t command)
{
    lcd_send_byte(command, false);
}


void lcd_write_char(char c)
{
    lcd_send_byte((uint8_t)c, true);
}

void lcd_init()
{
    pcf8574_init(0x27);

    // Wait for LCD power-up
    timer_delay_ms(50);

    // Force LCD into a known 8-bit startup state
    // We only have D4-D7 connected, so these are sent as nibbles.
    lcd_send_nibble(0x03, false);
    timer_delay_ms(5);

    lcd_send_nibble(0x03, false);
    timer_delay_us(150);

    lcd_send_nibble(0x03, false);
    timer_delay_us(150);

    // Switch to 4-bit mode
    lcd_send_nibble(0x02, false);
    timer_delay_us(150);

    // 4-bit mode, 2 lines, 5x8 font
    lcd_command(0x28);

    // Display OFF while configuring
    lcd_command(0x08);

    // Clear display
    lcd_command(0x01);
    timer_delay_ms(2);

    // Cursor moves right after each character
    lcd_command(0x06);

    // Display ON, cursor OFF, blink OFF
    lcd_command(0x0C);

    timer_delay_ms(2);
}

void lcd_print(const char* text)
{
     while(*text != '\0')
     {
         lcd_write_char(*text++);
     }
}

void lcd_set_cursor(uint8_t col, uint8_t row)
{
    if(row == 0)
    {
        lcd_command(0x80 + col);
    }
    else if(row == 1)
    {
        lcd_command(0xC0 + col);
    }
}

void lcd_print_int(uint16_t value)
{
    char buffer[6];
    ultoa(value, buffer, 10);
    lcd_print(buffer);
}