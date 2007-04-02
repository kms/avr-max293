/*
 * $Id$
 * Karl-Martin Skontorp <kms@skontorp.net>
 */

#define F_CPU 4.8E6

#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

ISR(SIG_ADC) {
    uint32_t f = ADCH * 18750;
    uint32_t o = (9600000 / (2 * 1 * f)) - 1;
    OCR0A = (uint8_t) o;
}

int main(void) {
    /* Watchdog */
    wdt_reset();
    wdt_disable();

    /* System clock */
    //CLKPR = _BV(CLKPCE);
    //CLKPR = _BV(CLKPS3);

    /* Ports */
    PORTB = _BV(PINB2) | _BV(PINB3) | _BV(PINB4);
    DDRB = _BV(PINB0) | _BV(PINB1);

    /* Power saving */
    set_sleep_mode(SLEEP_MODE_IDLE);

    /* Timer */
    TCCR0A = _BV(COM0A0) | _BV(WGM01);
    TCCR0B = _BV(CS00);

    /* ADC */
    ADMUX = _BV(ADLAR) | _BV(MUX0);
    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADIE) | _BV(ADATE);
    ADCSRA |= _BV(ADPS2) | _BV(ADPS1);

    sei();

    for (;;) {
	sleep_mode();
    }
}
