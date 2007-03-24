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

void startFan(void);

void startFan() {
    uint16_t i;
    OCR0A = 0x00;
    for (i = 0; i < 400; i++) {
	_delay_ms(25);
    }
}

ISR(SIG_ADC) {
    OCR0A = 0xff - ADCH;
//    OCR0A = 0x7f;
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
    TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(WGM01) | _BV(WGM00);
    TCCR0B = _BV(CS00);

    /* ADC */
    ADMUX = _BV(ADLAR) | _BV(MUX0);
    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADIE) | _BV(ADATE);
    ADCSRA |= _BV(ADPS2) | _BV(ADPS1);

    startFan();

    sei();

    for (;;) {
	sleep_mode();
    }
}
