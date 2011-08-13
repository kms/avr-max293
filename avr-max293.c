/* avr-max293
 * Adjustable frequency generator for MAX293.
 *
 * (c) Karl-Martin Skontorp <kms@skontorp.net> ~ http://22pf.org/
 * Licensed under the GNU GPL 2.0 or later.
 */

#define F_CPU 4.8E6

#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

volatile uint8_t f;

ISR(SIG_ADC) {
    if (PINB & _BV(PINB4)) {
        f = 0xff;
    } else {
        uint16_t p;

        p = ADCL;
        p |= (ADCH << 8);

        f = p >> 2;
    }

    OCR0A = f;
}

int main(void) {
    /* Watchdog */
    wdt_reset();
    wdt_disable();

    /* System clock */
    clock_prescale_set(clock_div_1);

    /* Ports */
    PORTB = _BV(PINB2) | _BV(PINB3) | _BV(PINB4);
    DDRB = _BV(PINB0) | _BV(PINB1);

    /* Power saving */
    set_sleep_mode(SLEEP_MODE_IDLE);

    /* Timer */
    TCCR0A = _BV(COM0A0) | _BV(WGM01);
    TCCR0B = _BV(CS00);

    /* ADC */
    ADMUX = _BV(MUX0) | _BV(MUX1);
    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADIE) | _BV(ADATE);
    ADCSRA |= _BV(ADPS2) | _BV(ADPS1);

    for (;;) {
        uint8_t i;

        sei();

        PORTB &= ~(_BV(PINB1));
        for (i = 0; i < 15; i++) {
            _delay_ms(10);
        }
        PORTB |= _BV(PINB1);

        cli();

        for (i = 0; i < f; i++) {
            _delay_ms(5);
        }
    }
}
