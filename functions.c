#include <teensy/common.h>
#include "cores.h"

uint16_t time_to_seconds(uint8_t hour_digit, uint8_t minute_digit_one, uint8_t minute_digit_two) {
    uint16_t minute_total = minute_digit_one * 10 + minute_digit_two;
    minute_total += hour_digit * 60;
    return minute_total * 60;
}

void wait_ms(uint16_t ms) {
    //Needs work
    for(volatile uint32_t wait_num = (ms * 6600); wait_num; wait_num--);
}

void wait_us(uint16_t us) {
    //Needs work
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
    wait_ms(5); //needs to be changed to real busy check
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
    /*ADC0_CFG1*/uint32_t var = (ADC_CFG1_ADIV(0x00) | ADC_CFG1_ADICLK(0x01)) + ADC_CFG1_MODE(0x02);
    char *stringvar = int_to_string(var);
    display_string(stringvar);
    wait_ms(500);
    lcd_command(0x01);
    /*ADC0_SC3*/uint32_t vartwo = ADC_SC3_CAL_MASK;
    char *stringvartwo = int_to_string(vartwo);
    display_string(stringvartwo);
    wait_ms(500);
    lcd_command(0x01);
    volatile uint32_t filler = 1;
    while(/*ADC0_SC3 & ADC_SC3_CAL_MASK*/filler < 10) {
	filler++;
    display_string("beep!");
    wait_ms(750);
    lcd_command(0x01);
    }
    display_string("beep2!");
    wait_ms(1000);
    lcd_command(0x01);
    //char *fillerstring = int_to_string(filler);
    //display_string(fillerstring);
    //uint16_t cal_sum = ADC0_CLPS + ADC0_CLP4 + ADC0_CLP3 + ADC0_CLP2 + ADC0_CLP1 + ADC0_CLP0;
    //cal_sum = (cal_sum / 2) | 0x8000;
    //ADC0_PG = cal_sum;
    //cal_sum = ADC0_CLMS + ADC0_CLM4 + ADC0_CLM3 + ADC0_CLM2 + ADC0_CLM1 + ADC0_CLM0;
    //cal_sum = (cal_sum / 2) | 0x8000;
    //ADC0_MG = cal_sum;
    if(varthree) {
	varthree++;
    }
    else {
	varthree = 1;
    }
    char *stringvarthree = int_to_string(varthree);
    display_string(stringvarthree);
    wait_ms(1000);
    lcd_command(0x01);
}

uint16_t adc_convert() {
    ADC0_SC1A = ADC_SC1_ADCH(0x08);
    uint8_t u = 1;
    while(u) {
	if(ADC0_SC1A & ADC_SC1_COCO_MASK) {
	    u = 0;
	    uint16_t result = (ADC0_RA & ADC_R_D(0x08));
	    return result;
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

uint8_t debounce_read(uint8_t pin, uint8_t port){
    switch(port) {
	case 0:
	    uint32_t *input_port = GPIOA_PDIR;
	    break;
	case 1:
	    uint32_t *input_port = GPIOB_PDIR;
	    break;
	case 2:
	    uint32_t *input_port = GPIOC_PDIR;
	    break;
	case 3:
	    uint32_t *input_port = GPIOD_PDIR;
	    break;
	case 4:
	    uint32_t *input_port = GPIOE_PDIR;
	    break;
	default:
	    break;
    }
    varname = 1;
    while(varname) {
	if(input_port & (1<<pin)) {
	    
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
    switch_release_confidence = 0;
    switch_press_confidence = 0;
    switch_pressed = 0;
    digit_switch = 0;
    pressed = 0;
    release_confidence = 0;
    press_confidence = 0;
    twilight_hour = 0;
    twilight_minute_one = 0;
    twilight_minute_two = 0;
    startup_hour = 0;
    startup_minute_one = 0;
    startup_minute_two = 0;
    k = 1;
    while(k) {
        if(GPIOA_PDIR & (1<<13)) {
            if(GPIOA_PDIR & (1<<12)) {
                switch_release_confidence++;
                switch_press_confidence = 0;
                if(switch_release_confidence > 500) {
                    switch_pressed = 0;
                    switch_release_confidence = 0;
                }
            }
            else {
                switch_press_confidence++;
                switch_release_confidence = 0;
                if(switch_press_confidence > 500) {
                    if(switch_pressed == 0){
                        digit_switch++;
                        switch_pressed = 1;
                        switch_press_confidence = 0;
                    }
                }
            }
            if(GPIOB_PDIR & (1<<2)) {
                release_confidence++;
                press_confidence = 0;
                if(release_confidence > 500) {
                    pressed = 0;
                    release_confidence = 0;
                }
            }
            else {
                press_confidence++;
                release_confidence = 0;
                if(press_confidence > 500) {
                    if(pressed == 0) {
                        pressed = 1;
                        press_confidence = 0;
                        if(digit_switch == 0) {
                            twilight_hour++;
                            lcd_command(0x30);
                            lcd_command(0x0c);
                            lcd_command(0x01);
                            display_char(twilight_hour + '0');
                        }
                        else if(digit_switch == 1) {
                            twilight_minute_one++;
                            lcd_command(0x30);
                            lcd_command(0x0c);
                            lcd_command(0x01);
                            display_char(twilight_minute_one + '0');
                        }
                        else if(digit_switch == 2) {
                            twilight_minute_two++;
                            lcd_command(0x30);
                            lcd_command(0x0c);
                            lcd_command(0x01);
                            display_char(twilight_minute_two + '0');
                        }
                        else {
                            digit_switch = 0;
                        }
                    }
                }
            }
        }
        else {
            if(GPIOA_PDIR & (1<<12)) {
                switch_release_confidence++;
                switch_press_confidence = 0;
                if(switch_release_confidence > 500) {
                    switch_pressed = 0;
                    switch_release_confidence = 0;
                }
            }
            else {
                switch_press_confidence++;
                switch_release_confidence = 0;
                if(switch_press_confidence > 500) {
                    if(switch_pressed == 0){
                        digit_switch++;
                        switch_pressed = 1;
                        switch_press_confidence = 0;
                    }
                }
            }
            if(GPIOB_PDIR & (1<<2)) {
                release_confidence++;
                press_confidence = 0;
                if(release_confidence > 500) {
                    pressed = 0;
                    release_confidence = 0;
                }
            }
            else {
                press_confidence++;
                release_confidence = 0;
                if(press_confidence > 500) {
                    if(pressed == 0) {
                        pressed = 1;
                        press_confidence = 0;
                        if(digit_switch == 0) {
                            startup_hour++;
                            lcd_command(0x30);
                            lcd_command(0x0c);
                            lcd_command(0x01);
                            display_char(startup_hour + '0');
                        }
                        else if(digit_switch == 1) {
                            startup_minute_one++;
                            lcd_command(0x30);
                            lcd_command(0x0c);
                            lcd_command(0x01);
                            display_char(startup_minute_one + '0');
                        }
                        else if(digit_switch == 2) {
                            startup_minute_two++;
                            lcd_command(0x30);
                            lcd_command(0x0c);
                            lcd_command(0x01);
                            display_char(startup_minute_two + '0');
                        }
                        else {
                            k = 0;
                        }
                    }
                }
            }
        }
        if(k == 0) {
            twilight = time_to_seconds(twilight_hour, twilight_minute_one, twilight_minute_two);
            startup_time = time_to_seconds(startup_hour, startup_minute_one, startup_minute_two);
            RTC_TAR = RTC_TAR_TAR(60);
            RTC_CR |= RTC_CR_OSCE_MASK;
            RTC_SR |= RTC_SR_TCE_MASK;
            k = 0;
        }
    }
}
