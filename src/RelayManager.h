#ifndef RELAY_MANAGER_H
#define RELAY_MANAGER_H

#ifndef DELAY_MAX_S
#define DELAY_MAX_S 30UL // typical values 30 or 60
#endif

/**
 * Has functions and constants for managing the relay and related output pins
 */
class RelayManager {
    volatile unsigned long ticksRemaining = 0;
public:
    /**
     * Maximum possible value for 10-bit ADC
     */
    static constexpr uint32_t maxADC = 1024UL;

    /**
     * Desired output of Timer 1 for relay shutoff (125Hz)
     */
    static constexpr uint32_t relayHz = 125UL;

    /**
     * Used to determine how many "ticks" until relay shutoff.  Multiply by ADC value.
     * Timer triggers on toggle (looks like sawtooth wave in docs), so you need to wait 2 * Hz ticks
     */
    static constexpr uint32_t delayMultiplier = 2 * relayHz * DELAY_MAX_S; // timer triggers interrupt on toggle (top/bottom), so runs at 2*relayHz

    /**
     * Calculates the value for OCR1A / OCR1B based on the formula at the top of page 85 describing CTC mode
     * The value 256 comes from use of CS12 clock source, which uses 1/256 prescaler
     */
    static constexpr uint8_t delayTimer = F_CPU / (256 * 2 * relayHz) - 1;

    /**
     * Pin for indicator LED.
     * The LED is on while something is detected.
     */
    static constexpr uint8_t indicatorLedPin = PA1;

    /**
     * Pin for relay.
     * The relay is on starting at detection time, and through a time specified by the ADC (with minimum).
     */
    static constexpr uint8_t relayPin = PA4;

    /**
     * Construct a RelayManager instance
     */
    RelayManager();

    /**
     * Sets up TIMER1 for relay countdown
     */
    static void configurePWM();

    /**
     * Sets up timer interrupts for handling the countdown timer
     */
    static void configureInterrupts();

    /**
     * Sets up outputs for the relay and status LED
     */
    static void configureOutputs();

    /**
     * Should be called when IR is detected.
     * Should only be called upon state change.
     */
    static void handleIRDetected();

    /**
     * Should be called when IR is no longer detected.
     * Should only be called upon state change.
     * @param ticks number of ticks to count down before turning off relay
     */
    void handleIRCleared(unsigned long ticks) volatile;

    /**
     * Should be called by timer interrupt to decrement relay counter.
     * Once the relay counter hits 0 (runs out of time), the relay and timer are turned off.
     */
    void tick() volatile;
};

#endif //RELAY_MANAGER_H
