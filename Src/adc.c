#include "stm32c0xx.h"
#include "timers.h"


#define ADC_ASY 		(0b10 << 30)  // Asynchronous CLK, CLK HSI
#define ADC_OVERSAMPLE  (1 << 0)
#define ADC_OVER16		(0b011 << 2)  // Oversample by 16
#define ADC_SHIFT4      (0b0100 << 5) // divide by 16
#define ADC_ADVREGEN_EN	(1U << 28)    // Enable internal ADC Voltage regulator
#define ADC_CAL_START   (1U << 31)	  // Start calibration
#define AUTOFF			(1 << 15)     // adc off after conversion
#define SMP2_PRESET		(0b111 << 0)  // 160.5 cycles x 48MHZ =50us x 16 samples
#define ADC_RDY_FLAG	(1U << 0)	  // Clear ready ADC ENABLE flag
#define ADC_EN			(1U << 0)	  // ADC enable


// timer_init() function must be called before this
void adc_init(void){


	// clock setup- HSI kernel clock at 48MHz

	RCC->CCIPR |= (ADC_ASY);

	//CFGR2 reg configuration

	// Oversampler enable
	ADC1->CFGR2 |= (ADC_OVERSAMPLE);
	//Oversample ratio
	ADC1->CFGR2 |= (ADC_OVER16);
	// Divide by 16
	ADC1->CFGR2 |= (ADC_SHIFT4);
	// Enable asynchronous, bit 30=0b00
	ADC1->CFGR2 &= ~(0b11 << 30);

	// Enable voltage regulator
	ADC1->CR |= (ADC_ADVREGEN_EN);
	delay_us(30);
	// Start to calibrate ADC
	ADC1->CR |= (ADC_CAL_START);

	while (ADC1->CR & ADC_CAL_START);  // Wait till calibration bit will be 0

	// autooff
	ADC1->CFGR1 |= (AUTOFF);

	// Select sampling time (internal cap charge time) and chanel
	ADC1->SMPR |= (SMP2_PRESET);

	// Clear ready flag
	ADC1->ISR |= (ADC_RDY_FLAG);

	// Enable ADC
	ADC1->CR |= (ADC_EN);

	while (!(ADC1->ISR & ADC_RDY_FLAG)); // Wait untill ADC ready flag is 0 (ADC ready)

}
