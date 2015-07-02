#ifndef _CORES_H
#define _CORES_H

#include <teensy/mk20d7.h>
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00
#define CPU_8MHz        0x01
#define CPU_4MHz        0x02
#define CPU_2MHz        0x03
#define CPU_1MHz        0x04
#define CPU_500kHz      0x05
#define CPU_250kHz      0x06
#define CPU_125kHz      0x07
#define CPU_62kHz       0x08
#define LED_PIN 1<<5

uint16_t startup_time;
uint16_t twilight;
uint16_t voltage;
uint32_t ohms;
uint8_t k, l, result;
uint8_t switch_pressed;
uint8_t pressed;
uint8_t digit_switch;
uint8_t twilight_hour;
uint8_t twilight_minute_one;
uint8_t twilight_minute_two;
uint8_t startup_hour;
uint8_t startup_minute_one;
uint8_t startup_minute_two;
uint16_t switch_press_confidence;
uint16_t press_confidence;
uint16_t switch_release_confidence;
uint16_t release_confidence;
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
