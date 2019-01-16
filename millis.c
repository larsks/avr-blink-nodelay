#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

volatile unsigned long timer_millis = 0;

ISR(TIMER0_COMPA_vect) {
    timer_millis++;
}

// based on https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf
void init_millis() {
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
}

unsigned long millis() {
    unsigned long _millis;
    // Updating a 16 bit value is not an atomic operation.
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        _millis = timer_millis;
    }
    
    return _millis;
}                                                
