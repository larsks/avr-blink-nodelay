#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h> 

volatile unsigned long timer_millis;
unsigned long last;

ISR(TIMER0_COMPA_vect)
{
    timer_millis++;
    PINB = 1<<PB3;
}

// based on https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf
int main(void) 
{
    DDRB |= 1<<PB0 | 1<<PB3;

    // flash PB0
    PORTB |= 1<<PB0;
    _delay_ms(100);
    PINB = 1<<PB0;
    _delay_ms(100);
    PINB = 1<<PB0;

    TCCR0A = 1<<WGM01;
    TCCR0B = 3<<CS00;
    OCR0A = (F_CPU/1000) / 64;
    TIMSK |= 1<<OCIE0A;
    SREG |= 1<<7;

    while (1)
    {
        unsigned long now;

        ATOMIC_BLOCK(ATOMIC_FORCEON) {
            now = timer_millis;
        }

        if (now - last > 1000) {
            PINB = 1<<PB0;
            last = now;
        }
    }                                                
    return (0);                           
}
