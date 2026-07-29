#include "stm32c0xx.h"
#include "timers.h"


#define TIM3_CLK_EN  (1 << 1)


void timer_init (void){

// Picked general purpose timer TIM3

	// Set clock (tim3 sits at APB1)
   RCC->APBENR1 |= (TIM3_CLK_EN);

   //Prescaler PSC+1
   TIM3->PSC = 47; // divide by 2+1=3. SYSCLK=3MHz/3=1us clock

   // Top count value 65535
   TIM3->ARR = 0xFFFF;

   // Timer modification apply. It trigger UG that implements setting change
   TIM3->EGR = (1 << 0);

   //Fire the counter up ENABLE
   TIM3->CR1 |= (1 << 0);

}



