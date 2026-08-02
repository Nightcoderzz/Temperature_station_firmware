#include "stm32c0xx.h"

#define LSI_ON			(1 <<0)    // LSI on
#define RTC_CLK_ENABLE	(1 << 10)	// APB1 bus clock
#define RTC_CLK_SOURCE  (0b10 << 8) // LSI selected
#define RTC_INIT_MODE	(1 << 7)
#define RTC_ENABLED		(1 << 15)

#define SHADOW_FLAG     (1 << 5)


void rtc_init1 (void){

	RCC->CSR2 	 |= (LSI_ON);
	while ( !((RCC->CSR2) & (1 << 1))); // Wait

	RCC->APBENR1 |= (RTC_CLK_ENABLE);
	RCC->CSR1    |= (RTC_CLK_SOURCE); // Selected LSI
	RCC->CSR1 |= (RTC_ENABLED);

	// Write protection key
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	RTC->ICSR |= (RTC_INIT_MODE);
	while (!((RTC->ICSR) & (1 << 6)));

	RTC->PRER  = 0x007F0000; // 127 +1
	RTC->PRER |= (249U << 0); // 249 +1

	RTC->ICSR &= ~(RTC_INIT_MODE); // Clear INIT to exit

	while ( !((RTC->ICSR) & SHADOW_FLAG)); // Wait for shadow register

	RTC->WPR = 0xFF; // lock- anything than key will lock it

}

