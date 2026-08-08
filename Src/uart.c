#include "stm32c0xx.h"

#define GPIOA_PA9_ALT  (0b0001 << 4)  // PA9 seta as alternate pin for uart
#define GPIOA_PA2_ALT  (0b0001 << 8)  // PA2 set as AF1 -> USART2_TX (ST-LINK VCP)
#define USART1_EN	   (1U << 14)	  // Enable usart1 clock (APBENR2)
#define USART2_EN	   (1U << 17)	  // Enable usart2 clock (APBENR1)
#define USART_EN	   (1 << 0)		  // USART enable
#define TRANS_EN	   (1 << 3)       // Transmitter enable bit
#define SYS_CLK		   48000000		  // 48MHz
#define BAUD		   9600			  // Baud rate
#define TXE		       (1 << 7)		  // TDR empty flag
#define TRANS_COMPLETE (1 << 6)
void uart_init(void){

// UART clock enable
	RCC->APBENR2 |= (USART1_EN);

// PA9 → alternate function, USART1_TX
    GPIOA->MODER &= ~(0b11 << (9 * 2));  //clear
    GPIOA->MODER |=  (0b10 << (9 * 2));  // set as alternate mode
    GPIOA->AFR[1] &= ~(0xF << 4);        // clear
	GPIOA->AFR[1] |= (GPIOA_PA9_ALT);

// Set baud rate
	USART1->BRR = SYS_CLK / BAUD; //PCLK/baud

// Enable USART
	USART1->CR1 |= (USART_EN);

// Set TE bit as per instructions in RM490 p.753
	USART1->CR1 |= (TRANS_EN);

}

void uart2_init(void){

// UART clock enable - USART2 sits on APB1, not APB2 like USART1
	RCC->APBENR1 |= (USART2_EN);

// PA2 → alternate function, USART2_TX (routed to the ST-LINK VCP)
	GPIOA->MODER  &= ~(0b11 << (2 * 2));  // clear
	GPIOA->MODER  |=  (0b10 << (2 * 2));  // set as alternate mode
	GPIOA->OTYPER &= ~(1U << 2);          // push-pull, TX must drive both levels
	GPIOA->AFR[0] &= ~(0xF << 8);         // clear (PA2 lives in AFR[0], bits 11:8)
	GPIOA->AFR[0] |=  (GPIOA_PA2_ALT);

// Set baud rate
	USART2->BRR = SYS_CLK / BAUD; //PCLK/baud

// Enable USART
	USART2->CR1 |= (USART_EN);

// Set TE bit as per instructions in RM490 p.753
	USART2->CR1 |= (TRANS_EN);

}

// The three senders below are shared by both ports, the public uart_*/uart2_*
// wrappers just pick which USART the bytes go out of.

static void usart_send_byte (USART_TypeDef *usart, uint8_t byte){

	while (!(usart->ISR & TXE));

	usart->TDR=byte;
}

static void usart_send_string (USART_TypeDef *usart, const char *string) {

	while (*string != '\0'){

		usart_send_byte(usart, *string);
		string++;
	while (!((USART1->ISR) & TRANS_COMPLETE ));
	}
}

static void usart_send_temp (USART_TypeDef *usart, int16_t raw)
{
    char buf[6];
    uint8_t count = 0;

    if (raw < 0) {
        usart_send_byte(usart, '-');
        raw = -raw;
    }

    uint16_t whole = raw / 16;
    uint16_t frac  = raw % 16;

    do {
        buf[count] = (whole % 10) + '0';
        count++;
        whole = whole / 10;
    } while (whole > 0);

    while (count > 0) {
        count--;
        usart_send_byte(usart, buf[count]);
    }

    usart_send_byte(usart, '.');
    usart_send_byte(usart, (frac * 10 / 16) + '0');
    usart_send_string(usart, "\r\n");

	while (!((USART1->ISR) & TRANS_COMPLETE ));

}

// ---------------- USART1 (PA9, HC-12) ----------------

void uart_send_byte( uint8_t byte){

	usart_send_byte(USART1, byte);
	while (!((USART1->ISR) & TRANS_COMPLETE ));

}

void uart_send_string (const char *string) {

	usart_send_string(USART1, string);
	while (!((USART1->ISR) & TRANS_COMPLETE ));

}

void uart_send_temp(int16_t raw)
{
	usart_send_temp(USART1, raw);
}

// ---------------- USART2 (PA2, ST-LINK VCP) ----------------

void uart2_send_byte( uint8_t byte){

	usart_send_byte(USART2, byte);
}

void uart2_send_string (const char *string) {

	usart_send_string(USART2, string);
}

void uart2_send_temp(int16_t raw)
{
	usart_send_temp(USART2, raw);
}
