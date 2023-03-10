#ifndef _UART_LIB_H
#define _UART_LIB_H
#include "stm32f1xx_hal.h"
typedef enum
{
	not_success = 0,
	success
} uart_receive_state;
typedef enum
{
	freq_dht = 0,
	freq_gas,
	tem_str,
	hum_str,
	gas_str,
	get_tem_str,
	get_hum_str,
	get_gas_str,
	set_mode1,
	set_mode2,
	error_str
} enum_string;
typedef enum
{
	mode_dht = 0,
	mode_gas,
	mode_display
} mode_button;
void float_to_string(float number,char*string);
uint8_t check_number(char *number_string);
void tem_string_handle(enum_string string_handle,uint8_t *dt);
void uart_receive_handle(uint8_t *dt);
void threshold_handle(void);
void send_message_to_computer(void);
void send_message_to_LCD(void);
void get_temperature(void);
void get_humidity(void);
void get_gas(void);
#endif
