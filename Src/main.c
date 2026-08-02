#include "stm32c0xx.h"
#include "clock.h"
#include "timers.h"
#include "gpio.h"
#include "adc.h"
#include "uart.h"
#include "one_wire.h"
#include "stop_mode.h"

int main () {

//Clock
	sys_clk_48MHz ();
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

// Rtc
	 rtc_init1 ();

while (1){



	}
}



