#include "stm32c0xx.h"
#include "timers.h"


#define RCC_ADCEN		(1 << 20)	  // Enable ADC clock
#define ADC_ASY 		(0b10 << 30)  // Asynchronous CLK, CLK HSI
#define ADC_OVERSAMPLE  (1 << 0)
#define ADC_OVER16		(0b011 << 2)  // Oversample by 16
#define ADC_SHIFT4      (0b0100 << 5) // divide by 16
#define ADC_ADVREGEN_EN	(1U << 28)    // Enable internal ADC Voltage regulator
#define ADC_CAL_START   (1U << 31)	  // Start calibration
#define AUTOFF			(1 << 15)     // adc off after conversion
#define SMP1_PRESET		(0b111 << 0)  // 160.5 cycles x 48MHZ =50us x 16 samples
#define ADC_RDY_FLAG	(1U << 0)	  // Clear ready ADC ENABLE flag
#define ADC_EN			(1U << 0)	  // ADC enable


// timer_init() function must be called before this
void adc_init(void){

	RCC->APBENR2 |= RCC_APBENR2_ADCEN;

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

	// Select sampling time (internal cap charge time) and channel
	ADC1->SMPR |= (SMP1_PRESET);

	// Clear ready flag
	ADC1->ISR |= (ADC_RDY_FLAG);

	// Enable ADC
	ADC1->CR |= (ADC_EN);

	while (!(ADC1->ISR & ADC_RDY_FLAG)); // Wait until ADC ready flag is 0 (ADC ready)

	// autooff
	ADC1->CFGR1 |= (AUTOFF);
}





// battery level is adc_in3
// ldr is adc_in4

uint16_t adc_read( uint8_t channel){

uint16_t data = 0;

	ADC1->CHSELR = (1U << channel); // channel selection
	ADC1->CR |= (1 << 2);			//	ADC start

	while(!(ADC1->ISR & (1 << 2)));	// wait for conversion to finish

	data = ADC1->DR;	// ADC conversion data

	return data;
}

uint8_t batt_voltage_read (void){

uint8_t battery_level=0;

uint16_t adc_data = adc_read(3); // read channel 3 (PA3)

	uint16_t batt_voltage = (( (uint32_t) adc_data * 3300)/4095) *2;

	if (batt_voltage >= 4000){
		battery_level=5;
	}
	else if (batt_voltage >= 3800){
		battery_level=4;
	}
	else if (batt_voltage >= 3600){
		battery_level=3;
	}
	else if( batt_voltage >= 3400){
		battery_level=2;
	}
	else if (batt_voltage <= 3400){
		battery_level=1;
	}
	return battery_level;
}

uint8_t ldr_read(void){

	GPIOA->BSRR = (1 << 5); // switching ON PA5 to drive LDR voltage divider

	delay_ms(5); // give cap time to charge

	// assume that day is 40kOhms and below
	uint16_t adc_data = adc_read(4); // read channel 4 (PA4)
	GPIOA->BSRR = (1 << 21); // Pin 5 OFF. Light sample taken.

	uint16_t daynight = 2600; // value calculated at 40kOhms as cutoff
	uint16_t pin_volt = (( (uint32_t) adc_data * 3300)/4095);

	if (daynight > pin_volt )
		return 1; // day
	else
		return 0; // night
}
