#ifndef IR_MANAGER_H
#define IR_MANAGER_H

#include <avr/io.h>

#ifndef IR_HZ
#define IR_HZ 38000L
#endif

#if IR_HZ >= 15626L
constexpr uint8_t irEmitterTimer = F_CPU / (2 * IR_HZ) - 1;
#else
constexpr uint8_t irEmitterTimer = F_CPU / (2 * 15626L) - 1;
#endif

class IRManager {
    bool state = false;
public:
    enum State {
        NO_CHANGE,
        DETECTED,
        CLEARED,
    };
    static constexpr uint8_t irSensorPin = PA5;
    static constexpr uint8_t irSensorInterrupt = PCINT5;
    IRManager();
    static void configurePWM();
    static void configureInterrupts();
    static bool isIRCurrentlyDetected();
    State updateIRState() volatile;
};

#endif //IR_MANAGER_H
