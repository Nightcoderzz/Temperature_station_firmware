

uint8_t one_wire_reset (void);
uint8_t one_wire_presence(void);
void write_bit_1w (uint8_t bit);
void write_byte_1w (uint8_t cmd);
uint8_t read_bit_1w (void);
int16_t read_temp_1w (void);
