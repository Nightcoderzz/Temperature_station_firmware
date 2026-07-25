#include "stm32c0xx.h"
#include "gpio.h"
#include "timers.h"


#define PA2LOW				(1U << 18) // This resets bit 18 PA2 in GPOIA->BSRR
#define PA2HIGH				(1U << 2)	// PA2 high

// repetition for bytes access



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


// every write must be at least 60us long

void write_bit_1w (uint8_t bit){

	if (bit){ 					// write bit high
		GPIOA->BSRR = PA2LOW;
		delay_us(6);
		GPIOA->BSRR = PA2HIGH;
		delay_us(57);
	}
	else {						// write bit low
		GPIOA->BSRR = PA2LOW;
		delay_us(64);
		GPIOA->BSRR = PA2HIGH;
		delay_us(10);
	}
}

void write_byte_1w (uint8_t cmd){

	for (uint8_t i=0; i<8; i++){

		uint8_t bit = cmd & 1U;
		write_bit_1w (bit);
		cmd= cmd >> 1; // shift cmd 1 slot right
	}

}

uint8_t read_bit_1w (void){

	// master initiates read time slot
	GPIOA->BSRR = PA2LOW;
	delay_us(6);
	GPIOA->BSRR = PA2HIGH;
	delay_us(9);
	uint8_t read_pin = (GPIOA->IDR >> 2) & 1U;
	delay_us(55);

	return read_pin;
}

int16_t read_temp_1w (void){ // for reading 2 bytes repeats=2

uint8_t read_scr_pad =0xBE;
uint8_t temp_lsb=0;
uint8_t temp_msb=0;

	one_wire_reset ();
	write_byte_1w (read_scr_pad);

	for (uint8_t i =0; i<2; i++){

		if (!i){

			for(uint8_t k=0; k<8; k++){
				uint8_t temp = read_bit_1w();
				temp_lsb |= (temp << k);
			}
		}
		if(i){
			for(uint8_t k=0; k<8; k++){
				uint8_t temp = read_bit_1w();
				temp_msb |= (temp << k);

			}
		}
	}

int16_t combined_temp = (temp_msb << 7) | temp_lsb;

return combined_temp;

}
