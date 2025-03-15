#ifndef RELAY_MANAGER_H
#define RELAY_MANAGER_H

#ifndef DELAY_MAX_S
#define DELAY_MAX_S 30UL // typical values 30 or 60
#endif

class RelayManager {
    volatile unsigned long ticksRemaining = 0;
public:
    static constexpr uint32_t maxADC = 1024UL;
    static constexpr uint32_t relayHz = 125UL;
    static constexpr uint32_t delayMultiplier = 2 * relayHz * DELAY_MAX_S; // runs twice per hz
    static constexpr uint8_t delayTimer = F_CPU / (256 * 2 * relayHz) - 1; // 256 comes from using CS12 (by itself), see docs pg. 99
    static constexpr uint8_t indicatorLedPin = PA1;
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
