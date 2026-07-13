#include "stm32c0xx.h"
#include "timers.h"


#define TIM3_CLK_EN  (1 << 0)


void timer_init (void){

// Picked general purpose timer TIM3

	// Set clock (tim3 sits at APB1)
   RCC->APBENR1 |= (TIM3_CLK_EN);

   //Prescaler PSC+1
   TIM3->PSC = (0b10 << 0); // divide by 2+1=3. SYSCLK=3MHz/3=1us clock

   // Top count value 65535
   TIM3->ARR = 0xFFFF;

   // Timer modification apply. It trigger UG that implements setting change
   TIM3->EGR = (1 << 0);

   //Fire the counter up
   TIM3->CR1 |= (1 << 0);
}


void delay_us (uint16_t delay_val){

	TIM3->CNT = 0; // start counting from 0
	while (TIM3->CNT < delay_val);

	return;
}


void delay_ms (uint16_t delay_val){

	for (uint16_t i =0; i< delay_val; i++) {
		delay_us(1000);
	}

}
