#ifndef ADC_MANAGER_H
#define ADC_MANAGER_H
#include <avr/io.h>

class ADCManager {
public:
    static constexpr uint8_t adcPin = PA1;
    static constexpr uint8_t adcChannel = _BV(MUX0);
    static void configureADC();
    static uint16_t readADC();
};

#endif //ADC_MANAGER_H
