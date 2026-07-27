#include "stm32c0xx.h"
#include "gpio.h"
#include "timers.h"
#include "uart.h"


#define PA2LOW				(1U << 18)  // This resets bit 18 PA2 in GPOIA->BSRR
#define PA2HIGH				(1U << 2)	// PA2 high
#define RST_LOW				600			// this how long master pull low at reset
#define RST_HIGH			70			// time before sampling sensors pull low
#define RESET_ATTEMPTS		3			// times that presence will be checked if problem occurs




// reset sequence needed every communication
uint8_t one_wire_reset (void){

	//Reset sequence
	GPIOA->BSRR = PA2LOW;
	delay_us(RST_LOW); // PA2 pulls bus low for 600us

	GPIOA->BSRR = PA2HIGH; //DS18B20 waits min 15us.
	delay_us(RST_HIGH); // PA2 high and waits for DS18B20 sample

	uint8_t pin_low = !(GPIOA->IDR & (1 << 2)); // check if the sensor responds

	delay_us(600); // DS18B20 wire low- presence pulse

	return pin_low; // reset completed
}

uint8_t one_wire_presence(void){   // checks if presensce pulse is there

    for (uint8_t i = 0; i < RESET_ATTEMPTS; i++) {
        if (one_wire_reset()) {     //<- the call
            return 1;
        }
        delay_ms(5);
    }
    return 0;
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

uint8_t read_byte_1w (void) {

  uint8_t temp=0;
	for (uint8_t i=0; i<8; i++){
		temp |= (read_bit_1w() << i);
	}
  return temp;
}

int16_t read_temp_1w (void){ // returns combined_temp as a reading.

// command list

 uint8_t skip_rom_cmd=0xCC;
 uint8_t convert_temp_cmd=0x44;
 uint8_t read_scratchpad_cmd=0xBE;


 uint8_t temp_lsb=0;
 uint8_t temp_msb=0;

 uint8_t check=one_wire_presence();
 	 if(!check){
 		uart_send_string ("sensor does not respond");
 		return 0;
 	 }

// flowchart datasheet p.13-14
 write_byte_1w (skip_rom_cmd);
 write_byte_1w (convert_temp_cmd);
 delay_ms(750); // wait for 750ms second

 one_wire_presence();
 write_byte_1w (skip_rom_cmd);
 write_byte_1w (read_scratchpad_cmd);

 temp_lsb = read_byte_1w();
 temp_msb = read_byte_1w();

 uint16_t raw = ((uint16_t)temp_msb << 8) | temp_lsb;// combines lsb and msb
 int16_t  temperature = (int16_t)raw;

return temperature;

}
