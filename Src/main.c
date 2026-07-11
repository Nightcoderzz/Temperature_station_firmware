#include "stm32c0xx.h"
#include "clock.h"
#include "gpio.h"


int main () {

	// clock
	sys_clk_to_3MHz ();
	peripheral_clk_init ();
	rtc_init();

	// gpio
	gpio_init();


	while (1){

	for(;;) {}

	}
}



