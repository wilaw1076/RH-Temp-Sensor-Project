#ifndef USART_HPP
#define USART_HPP

#include <stdint.h>


void usart_init(uint32_t baud);
void usart_write_byte(uint8_t data);
void usart_printString(const char* text);
void usart_printInt(uint32_t value);


#endif // USART_HPP