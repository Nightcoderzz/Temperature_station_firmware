#include "stm32c0xx.h"
#include "clock.h"
#include "timers.h"
#include "gpio.h"
#include "adc.h"
#include "uart.h"

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

	for(;;) {}

	}
}



