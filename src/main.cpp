/*
 * IR Detector V17
 * Send to attiny20 via tpi
 *
 * Created: 2022-07-29 20:26:16
 * Author : John Sandmeyer
 *
 * Optional external defines:
 * DELAY_MAX_S: max delay of relay timer, default 30s
 * IR_HZ: speed of IR LED to match sensor (default 38000L)
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include "IRManager.h"
#include "RelayManager.h"
#include "ADCManager.h"

static volatile IRManager irManager; // NOSONAR
static volatile RelayManager relayManager; // NOSONAR

int main() {
    // disable prescaler - set CCP to disable configuration change protection
    // enforces a clock source and disables prescaler
    CCP = 0xD8;
    CLKMSR = 0;
    CCP = 0xD8;
    CLKPSR = 0;

    IRManager::configurePWM();
    IRManager::configureInterrupts();

    RelayManager::configurePWM();
    RelayManager::configureInterrupts();
    RelayManager::configureOutputs();

    ADCManager::configureADC();

	// wait 1.5 sec before enabling interrupts
	_delay_ms(1500);
	sei();

    // ReSharper disable once CppDFAEndlessLoop : loop is to keep MCU running while interrupts operate
    while (true) {
		/* loop forever, let interrupts do the work */
	}
}

/**
 * Invoked when something in PORTA is changed
 * Check to see if state on the pin we care about chagned, since it could fire for other reasons
 */
ISR(PCINT0_vect) {
    const auto state = irManager.getIRState();

	if (state == IRManager::State::DETECTED) {
        RelayManager::handleIRDetected();
	} else if (state == IRManager::State::CLEARED) {
	    const auto adcValue = static_cast<unsigned long>(ADCManager::readADC());

#if DEBUG_RELAY_TIMER == 1
	    // in debug mode, IR LED is adjusted based on ADC value for easy testing
	    // use formula HZ = 8MHz / (2 * (1 + OCR0A)) or OCR0A = 4000/KHz - 1
	    // or adcValue = ((4000/KHz) - 1)(1024/255)
	    // adcValue should be between 0 and 1023 (inclusive)
	    // for verification, ADC value 0 -> 4MHz, value 1023 -> 15.686KHz
	    OCR0A = static_cast<uint8_t>(adcValue * 255UL / 1024UL);
	    OCR0B = static_cast<uint8_t>(adcValue * 255UL / 1024UL);
#endif

	    // get number of ticks based on ADC data, send to relay manager
	    const auto ticks = (adcValue * RelayManager::delayMultiplier) / RelayManager::maxADC;
	    relayManager.handleIRCleared(ticks);
	}
}

/**
 * Runs when TIMER1 compare matches
 * Use to decrement relay manager's timer, to turn off the relay eventually
 */
ISR(TIM1_COMPA_vect) {
    relayManager.tick();
}
