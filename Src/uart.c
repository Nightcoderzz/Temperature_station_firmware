#include "stm32c0xx.h"

#define GPIOA_PA9_ALT  (0b0001 << 4)  // PA9 seta as alternate pin for uart
#define USART1_EN	   (1U << 14)	  // Enable usart1 clock
#define USART_EN	   (1 << 0)		  // USART enable
#define TRANS_EN	   (1 << 3)       // Transmitter enable bit
#define SYS_CLK		   3000000		  // 3MHz
#define BAUD		   9600			  // Baud rate
#define TXE		       (1 << 7)		  // TDR empty flag

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

void uart_send_byte( uint8_t byte){

	while (!(USART1->ISR & TXE));

	USART1->TDR=byte;
}

void uart_send_string (const char *string) {

	while (*string != '\0'){

		uart_send_byte(*string);
		*string++;
	}
}
