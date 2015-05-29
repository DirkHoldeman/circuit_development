#include <teensy/common.h>
#include "cores.h"

int main(void) {
    PORTC_PCR0 = PORT_PCR_MUX(0x1); //lcd port 0
    PORTC_PCR1 = PORT_PCR_MUX(0x1); //lcd port 1
    PORTC_PCR2 = PORT_PCR_MUX(0x1); //lcd port 2
    PORTC_PCR3 = PORT_PCR_MUX(0x1); //lcd port 3
    PORTC_PCR4 = PORT_PCR_MUX(0x1); //lcd port 4
    PORTC_PCR5 = PORT_PCR_MUX(0x1); //lcd port 5
    PORTC_PCR6 = PORT_PCR_MUX(0x1); //lcd port 6
    PORTC_PCR7 = PORT_PCR_MUX(0x1); //lcd port 7
    PORTB_PCR2 = PORT_PCR_MUX(0x1); //receives button presses for startup_time and twilight
    PORTA_PCR12 = PORT_PCR_MUX(0x1); //switches digits for B2
    PORTA_PCR13 = PORT_PCR_MUX(0x1); //switches between startup_time and twilight
    PORTD_PCR7 = PORT_PCR_MUX(0x1); //debug led
    PORTD_PCR4 = PORT_PCR_MUX(0x1); //lcd RS
    PORTD_PCR5 = PORT_PCR_MUX(0x1); //lcd R/W
    PORTD_PCR6 = PORT_PCR_MUX(0x1); //lcd E
    GPIOD_PDDR = (0xFF);

//    alarm_setup();

//    while(1) {
//	if(RTC_SR & RTC_SR_TAF_MASK) {
//	    jklmnop = 254;
//	}
/*	    if(voltage) {
		GPIOB_PSOR = (1<<0);
		for(volatile uint16_t a = 0; a != 65535; a++);
	    }
	    else {
		voltage = read_adc();
		ohms = voltage_to_ohms(voltage);
		char *resistor_value = int_to_string(ohms);
	    }
	}

	if(GPIOB_PDIR & (1<<1)) {
	    GPIOB_PCOR = (1<<0);
	    for(volatile uint16_t b = 0; b != 65535; b++);
	    l = 1;
	}
	else {
	    if(l) {d
		l = 0;
*/
		lcd_init();
		adc_init();
		while(1) {
		    wait_ms(1000);
		    lcd_command(0x01); //clear display
		    display_string("beep 5");
		    wait_ms(1000);
//		    uint16_t adc_value = adc_convert();
//		    char *adc_string = int_to_string(adc_value);
//		    display_string(adc_string);
		}
/*		display_string(resistor_value);
	    }
	}
    }*/
}
