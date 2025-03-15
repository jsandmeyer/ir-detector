#include <avr/io.h>
#include "RelayManager.h"

RelayManager::RelayManager() = default;

void RelayManager::configurePWM() {
  	// Timer 1 will turn off the relay
	// It is only enabled when the relay is turned on (by setting CS12 to TCCR1B)
	// DDRB |= _BV(DDB1);
    TCCR1B = _BV(WGM12);
    OCR1A = delayTimer;
    OCR1B = delayTimer;
}

void RelayManager::configureInterrupts() {
    // Timer mask for OCIE1A (Compare timer 1) - counts down to turn off relay
    TIMSK |= _BV(OCIE1A);
}

void RelayManager::configureOutputs() {
    DDRA |= _BV(relayPin); // Relay
    DDRB |= _BV(indicatorLedPin); // Detected Indicator LED
}

void RelayManager::handleIRDetected() {
	TCCR1B &= ~_BV(CS12); // disable timer
    PORTA |= _BV(relayPin); // turn on relay
    PORTB |= _BV(indicatorLedPin); // turn on LED
}

void RelayManager::handleIRCleared(unsigned long const ticks) volatile {
    if (ticks < 250) {
        ticksRemaining = 250;
    } else {
        ticksRemaining = ticks;
    }

    TCCR1B |= _BV(CS12); // enable timer
	PORTB &= ~_BV(indicatorLedPin); // turn off LED
}

void RelayManager::tick() volatile {
  	if (ticksRemaining == 0) {
	    TCCR1B &= ~_BV(CS12); // disable timer
	    PORTB &= ~_BV(indicatorLedPin); // turn off LED
	    PORTA &= ~_BV(relayPin); // turn off relay
		return;
	}

	ticksRemaining = ticksRemaining - 1;
}
