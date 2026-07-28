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
	uart_init();   // USART1 on PA9  -> HC-12
	uart2_init();  // USART2 on PA2  -> ST-LINK VCP

	uart2_send_string("Temp_station up\r\n");

	while (1){

		uart2_send_string("START\r\n");

		int16_t t = read_temp_1w();
		uart_send_temp(t);
		uart2_send_temp(t);
		uart2_send_string("Temp_station up\r\n");

		delay_ms(1000);

	}
}



