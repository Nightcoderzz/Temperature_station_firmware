
void uart_init(void);
void uart_send_byte( uint8_t byte);
void uart_send_string (const char *string);
void uart_send_temp(int16_t raw);

void uart2_init(void);
void uart2_send_byte( uint8_t byte);
void uart2_send_string (const char *string);
void uart2_send_temp(int16_t raw);
