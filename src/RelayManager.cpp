#include <avr/io.h>
#include "RelayManager.h"

RelayManager::RelayManager() = default;

void RelayManager::configurePWM() {
  	// Timer 1 will turn off the relay
	// It is only enabled when the relay is turned on (by setting CS12 to TCCR1B)
#ifdef DEBUG_RELAY_TIMER
	DDRB |= _BV(DDB1); // temp: enable output on tpidata
    TCCR1A = _BV(COM1A0); // temp: turns timer on tpidata
#endif
    TCCR1B = _BV(WGM12);
    OCR1A = delayTimer;
    OCR1B = delayTimer;
}

void RelayManager::configureInterrupts() {
    // Timer mask for OCIE1A (Compare timer 1) - counts down to turn off relay
    TIMSK |= _BV(OCIE1A);
}

void RelayManager::configureOutputs() {
    DDRA |= _BV(relayPin) | _BV(indicatorLedPin);
}

void RelayManager::handleIRDetected() {
	TCCR1B &= ~_BV(CS12); // disable timer
    PORTA |= _BV(relayPin) | _BV(indicatorLedPin);
}

void RelayManager::handleIRCleared(unsigned long const ticks) volatile {
    if (ticks < 250) {
        ticksRemaining = 250;
    } else {
        ticksRemaining = ticks;
    }

    TCCR1B |= _BV(CS12); // enable timer, prescaler factor CLK_io/256
	PORTA &= ~_BV(indicatorLedPin);
}

void RelayManager::tick() volatile {
  	if (ticksRemaining == 0) {
	    TCCR1B &= ~_BV(CS12) & ~_BV(CS11) & ~_BV(CS10); // disable timer
	    PORTA &= ~_BV(indicatorLedPin) & ~_BV(relayPin);
		return;
	}

	ticksRemaining = ticksRemaining - 1;
}
