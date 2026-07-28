/*
 * uart.h
 *
 *      Author: sikor
 */

#ifndef UART_H_
#define UART_H_

// USART1 -> PA9, wired to the HC-12 radio
void uart_init(void);
void uart_send_byte( uint8_t byte);
void uart_send_string (const char *string);
void uart_send_temp(int16_t raw);

// USART2 -> PA2, wired to the ST-LINK virtual COM port
void uart2_init(void);
void uart2_send_byte( uint8_t byte);
void uart2_send_string (const char *string);
void uart2_send_temp(int16_t raw);

#endif /* UART_H_ */
