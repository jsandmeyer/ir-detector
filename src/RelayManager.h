#ifndef RELAY_MANAGER_H
#define RELAY_MANAGER_H

#ifndef DELAY_MAX_S
#define DELAY_MAX_S 30UL // typical values 30 or 60
#endif

class RelayManager {
    volatile unsigned long ticksRemaining = 0;
public:
    static constexpr uint8_t delayTimer = 1;
    static constexpr uint32_t maxADC = 1024L;
    static constexpr uint32_t delayMultiplier = 250UL * DELAY_MAX_S;
    static constexpr uint8_t indicatorLedPin = PB1;
    static constexpr uint8_t relayPin = PA4;
    RelayManager();
    static void configurePWM();
    static void configureInterrupts();
    static void configureOutputs();
    static void handleIRDetected();
    void handleIRCleared(unsigned long ticks) volatile;
    void tick() volatile;
};



#endif //RELAY_MANAGER_H
