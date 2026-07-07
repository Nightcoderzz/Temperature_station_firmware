#include "stm32c0xx.h"

void gpio_init(void){

	// PA2 setting for DS18B20
	GPIOA->MODER &= ~(0b11 << 4); // clear any previous data
	GPIOA->MODER |= (0b01 << 4);  // set as output
	GPIOA->OTYPER |= (1U << 2); // open drain configuration

	// PA3 setting for Battery indicator
	GPIOA->MODER &= ~(0b11 << 6); // clear any previous
	GPIOA->MODER |= (0b11 << 6); // Analog pin



}
