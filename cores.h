#ifndef _CORES_H
#define _CORES_H

#include <teensy/mk20d7.h>

uint16_t time_to_seconds(uint8_t hour_digit, uint8_t minute_digit_one, uint8_t minute_digit_two);
void wait_ms(uint16_t ms);
void wait_us(uint16_t us);
void adc_init();
uint8_t digital_read(uint8_t port, uint8_t pin);
uint8_t debounce_read(uint8_t port, uint8_t pin);
uint16_t adc_convert();
uint32_t voltage_to_ohms(uint16_t volts);
uint8_t number_of_digits(uint16_t num);
char *int_to_string(uint16_t num);
void lcd_check(void);
void lcd_init(void);
void lcd_command(char command);
void display_char(char character);
void display_string(char *string);
void alarm_setup(void);

#endif
