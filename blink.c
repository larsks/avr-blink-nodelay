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
    // set PB0 and PB3 as output.  PB0 will flash 1/second, and PB3 will flash
    // whenever the ISR fires.
    DDRB |= 1<<PB0 | 1<<PB3;

    // flash PB0
    PORTB |= 1<<PB0;
    _delay_ms(100);
    PINB = 1<<PB0;
    _delay_ms(100);
    PINB = 1<<PB0;

    // Enable CTC mode
    TCCR0A = 1<<WGM01;

    // Select /64 scale, since this will let us operate at 16Mhz without
    // overflowing our 8-bit TCNT0 register.
    TCCR0B = 3<<CS00;

    // (F_CPU/1000) is number of clock cycles/ms, and we are using the
    // /64 scaler.
    OCR0A = (F_CPU/1000) / 64;

    // Enable timer compare interrupt
    TIMSK |= 1<<OCIE0A;

    // Enable interrupts globally
    SREG |= 1<<7;

    while (1)
    {
        unsigned long now;

        // Without this, updating a 16 bit value can
        // be interrupted by the ISR.
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
