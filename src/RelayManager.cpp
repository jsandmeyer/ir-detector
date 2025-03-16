#include <avr/io.h>
#include "RelayManager.h"

RelayManager::RelayManager() = default;

void RelayManager::configurePWM() {
  	// Timer 1 will turn off the relay
	// It is only enabled when the relay is turned on (by setting CS12 to TCCR1B)
#if DEBUG_RELAY_TIMER == 1
    // in debug mode, relay timer output is sent to TPIDATA pin to verify relayHz
	DDRB |= _BV(DDB1); // temp: enable output on tpidata
    TCCR1A = _BV(COM1A0); // temp: turns timer on tpidata
#endif
    // Set up in CTC mode, will restart timer on compare (when counter reaches OCR1A)
    TCCR1B = _BV(WGM12);

    // Number of cycles to count up - sets frequency
    OCR1A = delayTimer;
    OCR1B = delayTimer;
}

void RelayManager::configureInterrupts() {
    // On compare match for TIMER1, run interrupt subroutine
    TIMSK |= _BV(OCIE1A);
}

void RelayManager::configureOutputs() {
    DDRA |= _BV(relayPin) | _BV(indicatorLedPin);
}

void RelayManager::handleIRDetected() {
	TCCR1B &= ~_BV(CS12) & ~_BV(CS11) & ~_BV(CS10); // disable timer - only turn on once IR is no longer detected
    PORTA |= _BV(relayPin) | _BV(indicatorLedPin);
}

void RelayManager::handleIRCleared(unsigned long const ticks) volatile {
    // Must be on for 1sec
    if (ticks < relayHz * 2) {
        ticksRemaining = relayHz * 2;
    } else {
        ticksRemaining = ticks;
    }

    TCCR1B |= _BV(CS12); // enable timer, prescaler factor CLK_io/256
	PORTA &= ~_BV(indicatorLedPin); // turn off indicator LED because it's only for detection, not relay
}

void RelayManager::tick() volatile {
    // turn off relay and timer if 0 ticks are left
  	if (ticksRemaining == 0) {
	    TCCR1B &= ~_BV(CS12) & ~_BV(CS11) & ~_BV(CS10); // disable timer
	    PORTA &= ~_BV(indicatorLedPin) & ~_BV(relayPin);
		return;
	}

	ticksRemaining = ticksRemaining - 1;
}
