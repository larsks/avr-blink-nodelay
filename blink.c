#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h> 

#include "millis.h"

void init_pins() {
	DDRB = 1<<PB0;
}

// based on https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf
int main(void) 
{
    unsigned long last = 0;

    init_millis();
    init_pins();

    while (1)
    {
        unsigned long now = millis();

        if (now - last > 1000) {
            PINB = 1<<PB0;
            last = now;
        }
    }                                                
    return (0);                           
}
