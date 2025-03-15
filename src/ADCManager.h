#ifndef ADC_MANAGER_H
#define ADC_MANAGER_H
#include <avr/io.h>

class ADCManager {
public:
    static constexpr uint8_t adcPin = PA5;
    static constexpr uint8_t adcChannel = _BV(MUX2) | _BV(MUX0); // datasheet pgs. 105-106
    static void configureADC();
    static uint16_t readADC();
};

#endif //ADC_MANAGER_H
