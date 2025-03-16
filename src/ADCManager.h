#ifndef ADC_MANAGER_H
#define ADC_MANAGER_H
#include <avr/io.h>

class ADCManager {
public:
    /**
     * Pin which variable resistor is hooked to
     */
    static constexpr uint8_t adcPin = PA5;

    /**
     * MUX channel for the ADC (in this case, equals the pin's number in decimal)
     * See datasheet pages 105-106
     */
    static constexpr uint8_t adcChannel = _BV(MUX2) | _BV(MUX0); // datasheet pgs. 105-106

    /**
     * Configures ADC IO for just this pin, expects there are no other ADCs in use
     */
    static void configureADC();

    /**
     * Reads the value from the preconfigured ADC.
     * Does not disable interrupts.
     * @return the ADC value between 0 and 1023, inclusively
     */
    static uint16_t readADC();
};

#endif //ADC_MANAGER_H
