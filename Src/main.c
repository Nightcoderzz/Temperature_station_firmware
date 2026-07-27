#include "stm32c0xx.h"
#include "clock.h"
#include "timers.h"
#include "gpio.h"
#include "adc.h"
#include "uart.h"
#include "one_wire.h"

int main () {

//Clock
	sys_clk_to_3MHz ();
	peripheral_clk_init ();
	rtc_init();

// Timers
	timer_init ();

// ADC
	adc_init();

//Gpio
	gpio_init();

// Uart
	uart_init();


	while (1){


		int16_t t = read_temp_1w();
		uart_send_temp(t);

		delay_ms(1000);
	}
}



