/*
 * one_wire.h
 *
 *      Author: sikor
 */

#ifndef ONE_WIRE_H_
#define ONE_WIRE_H_

// GPIOA pin carrying the DS18B20 data line.
// Moved off PA2 because PA2 is now USART2_TX (ST-LINK VCP).
// Change this one number if you wire the sensor to a different GPIOA pin.
#define OW_PIN	1	// PA1

uint8_t one_wire_reset (void);
uint8_t one_wire_presence(void);
void write_bit_1w (uint8_t bit);
void write_byte_1w (uint8_t cmd);
uint8_t read_bit_1w (void);
int16_t read_temp_1w (void);

#endif /* ONE_WIRE_H_ */
