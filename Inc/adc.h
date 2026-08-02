/*
 * adc.h
 *
 *  Created on: 7 Jul 2026
 *      Author: sikor
 */

#ifndef ADC_H_
#define ADC_H_

void adc_init(void);
uint16_t adc_read( uint8_t channel);
uint8_t batt_voltage_read (void);
uint8_t ldr_read(void);





#endif /* ADC_H_ */
