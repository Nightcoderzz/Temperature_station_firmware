#include "stm32c0xx.h"
#include "clock.h"


#define LSI_ON			  (1 <<0)     // LSI on
#define RTC_CLK_ENABLE	  (1 << 10)	  // APB1 bus clock
#define RTC_CLK_SOURCE    (0b10 << 8) // LSI selected
#define RTC_INIT_MODE	  (1 << 7)
#define RTC_ENABLED		  (1 << 15)

#define SHADOW_FLAG       (1 << 5)
#define ALARM_FLAG_UP     (1 << 0)
#define ALARM_FLAG_CLEAR  (1 << 0)
#define ALARM_A_INTERRUPT (1UL << 12)



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
	while (!((RTC->ICSR) & (1 << 6))); // Calendar registers update is allowed

	RTC->PRER  = 0x007F0000; // 127 +1
	RTC->PRER |= (249U << 0); // 249 +1

	RTC->ICSR &= ~(RTC_INIT_MODE); // Clear INIT to exit

	while ( !((RTC->ICSR) & SHADOW_FLAG)); // Wait for shadow register

	RTC->WPR = 0xFF; // lock- anything than key will lock it

}

void alarm_a_init (void){

	// Write protection key
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	RTC->CR   &= ~(1 << 8); // disable ALARM A

	RTC->ALRMAR &= ~(0xFFFFFFFF); // clear register
	RTC->ALRMAR |= (1U << 31) | (1 << 23) | (1 << 15); // date, hours, minutes
	RTC->SCR 	= (ALARM_FLAG_CLEAR);

	RTC->CR   |= (1 << 8); // ENABLE ALARM A

	RTC->CR 	|= (ALARM_A_INTERRUPT); // let the signal leave RTC ALRAF

	RTC->WPR = 0xFF; // lock- anything than key will lock it
}


void rtc_exti_init(void){

    EXTI->EMR1 |= (1U << 19);   /* state register -> |= is right */
}


void stop_mode_enter(void)
{
    /* LPMS in PWR_CR1 already reads 000 = Stop from reset, so there is
       nothing to configure in PWR for this design. */

    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;     /* deep, not light */

    /* The core's event register is a single sticky bit. If something set
       it earlier, WFE returns instantly and we spin at full current.
       SEV sets one deliberately; the first WFE consumes it; the second
       WFE is guaranteed to sleep. This is the standard idiom. */
    __SEV();
    __WFE();
    __WFE();                               /* <<< stops here for ~60 s */


    /* ---------- woken ---------- */

    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

    RTC->WPR = 0xCAU;
    RTC->WPR = 0x53U;
    RTC->SCR = RTC_SCR_CALRAF;             /* or the next WFE returns at once */
    RTC->WPR = 0xFFU;
}
