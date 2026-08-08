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

// Timers
	timer_init ();

// ADC
	adc_init();

//Gpio
	gpio_init();

// Uart
	uart_init();   // USART1 on PA9  -> HC-12
	uart2_init();  // USART2 on PA2  -> ST-LINK VCP

	uart_send_string("Temp_station up\r\n");

// Rtc
	rtc_init1();
	alarm_a_init();
	rtc_exti_init();


	while (1){

		// stop mode time day vs night
		if( ldr_read ()){
			for (uint8_t i=0; i<3; i++){ // 3 minuted for day
				uart_send_string("It's day \r\n");
				stop_mode_enter();
			}
		}
			else {
				for (uint8_t i=0; i<9; i++){ // 10 minutes for night
					uart_send_string("It's night \r\n");
					stop_mode_enter();
				}
	}



	}
}



