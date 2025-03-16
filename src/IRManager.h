#ifndef IR_MANAGER_H
#define IR_MANAGER_H

#include <avr/io.h>

#ifndef IR_HZ
#define IR_HZ 38000L
#endif

/**
 * Selection of frequency below 15626 results in overflow for timer (with no prescaler)
 */
#if IR_HZ >= 15626L
constexpr uint8_t irEmitterTimer = F_CPU / (2 * IR_HZ) - 1;
#else
constexpr uint8_t irEmitterTimer = F_CPU / (2 * 15626L) - 1;
#endif

/**
 * Manages both IR emission and detection components/functions
 */
class IRManager {
    /**
     * internal storage, last known state of IR sensor
     */
    bool state = false;

    /**
     * Determine whether IR is currently detected, without changing the "most recent" state in variable `state`
     * @return whether IR is currently detected
     */
    static bool isIRCurrentlyDetected();
public:
    /**
     * Returned state value of IR sensor
     */
    enum State {
        NO_CHANGE,
        DETECTED,
        CLEARED,
    };

    /**
     * IR sensor pin
     * low = detected
     */
    static constexpr uint8_t irSensorPin = PA6;

    /**
     * IR interrupt pin ID
     */
    static constexpr uint8_t irSensorInterrupt = PCINT6;

    /**
     * Create a new IRManager instance
     */
    IRManager();

    /**
     * Configure TIMER0 for the IR emitter
     */
    static void configurePWM();

    /**
     * Configure interrupts and IO for the IR sensor pin
     */
    static void configureInterrupts();

    /**
     * Determines whether IR has been changed.
     * Updates internal variable `state`.
     * @return the current state of the IR sensor, compared to its previous state
     */
    State getIRState() volatile;
};

#endif //IR_MANAGER_H
