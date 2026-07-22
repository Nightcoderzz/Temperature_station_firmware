#include "stm32c0xx.h"
#include "gpio.h"
#include "timers.h"


#define PA2LOW				(1U << 18) // This resets bit 18 PA2 in GPOIA->BSRR
#define PA2HIGH				(1U << 2)	// PA2 high


// reset sequence needed every communication
uint8_t one_wire_reset (void){

	//Reset sequence
	GPIOA->BSRR = PA2LOW;
	delay_us(600); // PA2 pulls bus low for 600us

	GPIOA->BSRR = PA2HIGH;
	delay_us(60); // PA2 high and waits for DS18B20

	uint8_t pin_low = !(GPIOA->IDR & (1 << 2));

	delay_us(600); // DS18B20 wire low- presence pulse

	return pin_low; // reset completed
}


void write_bit (uint8_t bit){

	if (bit){
		GPIOA->BSRR = PA2LOW;
		delay_us(6);
		GPIOA->BSRR = PA2HIGH;
		delay_us(64);
	}
	else {
		GPIOA->BSRR = PA2LOW;
		delay_us(60);
		GPIOA->BSRR = PA2HIGH;
		delay_us(10);
	}
}

void write_byte (uint8_t cmd){

	for (uint8_t i=0; i<8; i++){

		uint8_t bit = cmd & 1U;
		write_bit (bit);
		cmd= cmd >> 1; // shift cmd 1 slot right
	}

}
