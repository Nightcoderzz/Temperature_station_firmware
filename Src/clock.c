#include "stm32c0xx.h"

#define GPIOA_EN (1 << 0)
#define USART1_EN (1 << 14)
#define ADC_EN (1 << 20)

// -------------RTC DEFINITIONS ----------------------

// RCC->CSR2 bits
#define LSI_ON          (1 << 0)    // Turn on LSI oscillator
#define LSI_READY       (1 << 1)    // LSI oscillator is stable

// RCC->APBENR1 bits
#define RTC_BUS_EN      (1 << 10)   // RTC bus clock enable
#define PWR_EN          (1 << 28)   // Power controller clock enable

// PWR->CR1 bits
#define BACKUP_UNLOCK   (1 << 8)    // Disable backup domain protection

// RCC->BDCR bits
#define RTC_CLK_LSI     (1 << 9)    // Select LSI as RTC clock (RTCSEL bits [9:8] = 10)
#define RTC_ON          (1 << 15)   // Enable the RTC

// RTC->ICSR bits
#define RTC_INIT_MODE   (1 << 7)    // Enter init mode
#define RTC_INIT_READY  (1 << 6)    // Init mode is ready


void peripheral_clk_init (void){

	// Initialise GPOI
	//Initialise UART
	// Initialise ADC
	// Initialise RTC


	RCC->IOPENR  |= GPIOA_EN;   // Enable GPIOA clock
	RCC->APBENR2 |= USART1_EN; // Enable USART1
	RCC->APBENR2 |= ADC_EN;	   // Enable ADC

}


void rtc_init(void) {

    // 1. Start LSI oscillator
    RCC->CSR2 |= LSI_ON;
    while (!(RCC->CSR2 & LSI_READY));

    // 2. Enable RTC bus clock
    RCC->APBENR1 |= RTC_BUS_EN;

    // 3. Enable power controller and unlock backup domain
    RCC->APBENR1 |= PWR_EN;
    PWR->CR1 	  |= BACKUP_UNLOCK;

    // 4. Select LSI as RTC clock source and enable RTC
    RCC->CSR1 |= RTC_CLK_LSI;
    RCC->CSR1 |= RTC_ON;

    // 5. Unlock RTC registers (two magic keys, must be in this order)
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    // 6. Enter init mode
    RTC->ICSR |= RTC_INIT_MODE;
    while (!(RTC->ICSR & RTC_INIT_READY));

    // 7. Set prescaler: ~32000 / 128 / 250 = 1 Hz
    RTC->PRER = (127U << 16) | 249U;

    // 8. Exit init mode and re-lock
    RTC->ICSR &= ~RTC_INIT_MODE;
    RTC->WPR = 0xFF;
}


