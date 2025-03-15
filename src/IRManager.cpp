#include "IRManager.h"

#include <avr/sfr_defs.h>

IRManager::IRManager() = default;

void IRManager::configureInterrupts() {
    // Pin A5 has the IR sensor - set up interrupts
    DDRA &= ~_BV(irSensorPin);
    GIMSK |= _BV(PCIE0);
    PCMSK0 |= _BV(irSensorInterrupt);
}

void IRManager::configurePWM() {
    // Count UP on OC0A/OC0B from 0 to OCR0A/OCR0B, then toggle states
    // From docs page 85, F_OCnA = F_clkIO / (2 * N * (1 + OCRnA) where N is prescaler (1)

    // Timer 0 counts UP on compare-mode to OCR0A/OCR0B, then state toggles
    // This controls the IR LED
    DDRB |= _BV(PB2);
    TCCR0A = _BV(COM0A0) | _BV(WGM01);
    TCCR0B = _BV(CS00);
    OCR0A = irEmitterTimer;
    OCR0B = irEmitterTimer;
}

bool IRManager::isIRCurrentlyDetected() {
    return bit_is_clear(PINA, irSensorPin);
}

IRManager::State IRManager::updateIRState() volatile {
    const auto newState = isIRCurrentlyDetected();

    if (newState == state) {
        return NO_CHANGE;
    }

    state = newState;

    if (newState) {
        return DETECTED;
    }

    return CLEARED;
}
