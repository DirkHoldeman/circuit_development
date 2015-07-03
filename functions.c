#include <teensy/common.h>
#include "cores.h"

uint16_t time_to_seconds(uint8_t hour_digit, uint8_t minute_digit_one, uint8_t minute_digit_two) {
    uint16_t minute_total = minute_digit_one * 10 + minute_digit_two;
    minute_total += hour_digit * 60;
    return minute_total * 60;
}

void wait_ms(uint16_t ms) {
    for(volatile uint32_t wait_num = (ms * 6600); wait_num; wait_num--);
}

void wait_us(uint16_t us) {
    for(volatile uint16_t wait_num = (us * 7); wait_num; wait_num--);
}

void lcd_init(void) {
    wait_ms(15);
    lcd_command(0x30); //function set
    wait_ms(5);
    lcd_command(0x30); //function set
    wait_us(100);
    lcd_command(0x30); //function set
    lcd_command(0x30); //final function set
    lcd_command(0x08); //turn off display
    lcd_command(0x01); //clear display
    lcd_command(0x06); //entry mode set
    lcd_command(0x0c); //turn on display
}

void lcd_check(void) {
    wait_ms(5);
    GPIOC_PCOR = (0xFF);
    GPIOC_PDDR = (0x00);
    GPIOD_PSOR = (1<<5);
    GPIOD_PCOR = (1<<4);
    GPIOC_PDDR = (0xFF);
    GPIOD_PCOR = (1<<5);
}

void lcd_command(char command) {
    lcd_check();
    GPIOC_PSOR = (command);
    GPIOD_PSOR = (1<<6);
    wait_us(1);
    GPIOD_PCOR = (1<<6);
}

void display_char(char character) {
    lcd_check();
    GPIOD_PSOR = (1<<4);
    GPIOC_PSOR = (character);
    GPIOD_PSOR = (1<<6);
    wait_us(1);
    GPIOD_PCOR = (1<<6);
}

void display_string(char *string) {
    while(*string > 0) {
        display_char(*string++);
    }
}

void adc_init() {
    ADC0_CFG1 = (ADC_CFG1_ADIV(0x00) | ADC_CFG1_ADICLK(0x01)) + ADC_CFG1_MODE(0x02);
    ADC0_SC3 = ADC_SC3_CAL_MASK;
    volatile uint32_t filler;
    while(ADC0_SC3 & ADC_SC3_CAL_MASK) {
	filler++;
    }
    uint16_t cal_sum = ADC0_CLPS + ADC0_CLP4 + ADC0_CLP3 + ADC0_CLP2 + ADC0_CLP1 + ADC0_CLP0;
    cal_sum = (cal_sum / 2) | 0x8000;
    ADC0_PG = cal_sum;
    cal_sum = ADC0_CLMS + ADC0_CLM4 + ADC0_CLM3 + ADC0_CLM2 + ADC0_CLM1 + ADC0_CLM0;
    cal_sum = (cal_sum / 2) | 0x8000;
    ADC0_MG = cal_sum;
}

uint16_t adc_convert() {
    ADC0_SC1A = ADC_SC1_ADCH(0x08);
    while(1) {
	if(ADC0_SC1A & ADC_SC1_COCO_MASK) {
	    uint16_t adc_result = (ADC0_RA & ADC_R_D(0x08));
	    return adc_result;
	}
    }
}

uint8_t number_of_digits(uint16_t num) {
    uint8_t digit_count = 0;
    while(num > 0) {
        digit_count++;
        num /= 10;
    }
    return digit_count;
}

char *int_to_string(uint16_t num) {
    uint8_t digit_count = number_of_digits(num);
    char string[digit_count];
    string[digit_count] = 0;
    while(num != 0) {
        string[digit_count - 1] = num % 10 + '0';
        num = num / 10;
        digit_count--;
    }
    return string;
}

uint32_t voltage_to_ohms(uint16_t volts) {
    return (14000000 / ((volts * 1000) / 3000) - 14000);
}

uint8_t digital_read(uint8_t port, uint8_t pin) {
    uint32_t result;
    switch(port) {
	case 0:
	    result = GPIOA_PDIR & (1<<pin);
	    break;
	case 1:
	    result = GPIOB_PDIR & (1<<pin);
	    break;
	case 2:
	    result = GPIOC_PDIR & (1<<pin);
	    break;
	case 3:
	    result = GPIOD_PDIR & (1<<pin);
	    break;
	case 4:
	    result = GPIOE_PDIR & (1<<pin);
	    break;
	default:
	    break;
    }
    return result;
}

uint8_t debounce_read(uint8_t port, uint8_t pin) {
    uint16_t switch_release_confidence = 0;
    uint16_t switch_press_confidence = 0;
    uint16_t switch_pressed = 0;
    while(1) {
	if(digital_read(port, pin)) {
	    switch_release_confidence++;
	    switch_press_confidence = 0;
	    if(switch_release_confidence > 500) {
		if(switch_pressed == 1) {
		    return 1;
		}
		else {
		    return 0;
		}
	    }
	}
	else {
	    switch_press_confidence++;
	    switch_release_confidence = 0;
	    if(switch_press_confidence > 500) {
		switch_pressed = 1;
	    }
	}
    }
}

void alarm_setup(void) {
    SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;
    RTC_CR = RTC_CR_SWR_MASK;
    RTC_CR &= ~RTC_CR_SWR_MASK;
    if(RTC_SR & RTC_SR_TIF_MASK) {
         RTC_TSR = 0x00;
    }
    uint8_t digit_switch = 0;
    uint8_t twilight_hour = 0;
    uint8_t twilight_minute_one = 0;
    uint8_t twilight_minute_two = 0;
    uint8_t startup_hour = 0;
    uint8_t startup_minute_one = 0;
    uint8_t startup_minute_two = 0;
    uint8_t loop_run = 1;
    while(loop_run) {
	if(GPIOA_PDIR & (1<<13)) {
            if(debounce_read(0, 12)) {
		digit_switch++;
            }
            if(debounce_read(1, 2)) {
		if(digit_switch == 0) {
		    twilight_hour++;
		    display_char(twilight_hour + '0');
		}
		else if(digit_switch == 1) {
		    twilight_minute_one++;
		    display_char(twilight_minute_one + '0');
		}
		else if(digit_switch == 2) {
		    twilight_minute_two++;
		    display_char(twilight_minute_two + '0');
		}
		else {
		    digit_switch = 0;
		}
            }
	}
        else {
            if(debounce_read(0, 12)) {
		digit_switch++;
	    }
            if(debounce_read(1, 2)) {
		if(digit_switch == 0) {
		    startup_hour++;
		    display_char(startup_hour + '0');
		}
		else if(digit_switch == 1) {
		    startup_minute_one++;
		    display_char(startup_minute_one + '0');
		}
		else if(digit_switch == 2) {
		    startup_minute_two++;
		    display_char(startup_minute_two + '0');
		}
		else {
		    loop_run = 0;
		}
            }
        }
        if(loop_run == 0) {
            twilight = time_to_seconds(twilight_hour, twilight_minute_one, twilight_minute_two);
            startup_time = time_to_seconds(startup_hour, startup_minute_one, startup_minute_two);
            RTC_TAR = RTC_TAR_TAR(twilight-startup_time);
            RTC_CR |= RTC_CR_OSCE_MASK;
            RTC_SR |= RTC_SR_TCE_MASK;
        }
    }
}
