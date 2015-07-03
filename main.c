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
    PORTD_PCR7 = PORT_PCR_MUX(0x1); //debug
    PORTD_PCR4 = PORT_PCR_MUX(0x1); //lcd RS
    PORTD_PCR5 = PORT_PCR_MUX(0x1); //lcd R/W
    PORTD_PCR6 = PORT_PCR_MUX(0x1); //lcd E
    GPIOD_PDDR = (0x7F);
    lcd_init();
    adc_init();
    while(1) {
	wait_ms(1000);
	lcd_command(0x01); //clear display
	uint16_t adc_value = adc_convert();
	char *adc_string = int_to_string(adc_value);
	display_string(adc_string);
    }
}
