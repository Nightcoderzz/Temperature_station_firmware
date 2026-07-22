#include "stm32c0xx.h"

void gpio_init(void){

	//Initial pin states

	GPIOA->BSRR = (1 << 2); // PA2 DS18B20 data -> high (release 1-Wire bus)

	GPIOA->BSRR = (1 << 21); // Pin PA5 LDR_SW reset
	GPIOA->BSRR = (1 << 22); // Pin PA6 DS18B20_SW reset
	GPIOA->BSRR = (1 << 23); // Pin PA7 HC-12_SW reset

	// PA2 setting for DS18B20 data
	GPIOA->MODER &= ~(0b11 << 4); // clear any previous data
	GPIOA->MODER |=  (0b01 << 4);  // set as output
	GPIOA->OTYPER |= (1U << 2); // open drain configuration

	// PA3 setting for Battery indicator ANALOG IN
	GPIOA->MODER &= ~(0b11 << 6); // clear any previous
	GPIOA->MODER |= (0b11 << 6); // Analog pin

	// PA4 setting for LDR ANALOG IN
	GPIOA->MODER &= ~(0b11 << 8); // clear any previous
	GPIOA->MODER |=  (0b11 << 8); // Analog pin

	// PA5 setting for LDR SWITCH
	GPIOA->MODER &= ~(0b11 << 10); // clear any previous
	GPIOA->MODER |=  (0b01 << 10); // output pin

	// PA6 setting for DS18B20 SWITCH
	GPIOA->MODER &= ~(0b11 << 12); // clear any previous
	GPIOA->MODER |=  (0b01 << 12); // output pin

	// PA7 setting for HC-12 SWITCH
	GPIOA->MODER &= ~(0b11 << 14); // clear any previous
	GPIOA->MODER |=  (0b01 << 14); // output pin

}
