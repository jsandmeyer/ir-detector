
#include "ADCManager.h"

void ADCManager::configureADC() {
    // Pin A1 has the variable resistor
    DDRA &= ~_BV(adcPin);

    // Right half of ADMUX selects which channel to read, leave left half alone
    ADMUX &= 0xF0;
    ADMUX |= adcChannel; // select channel 1

    // Disable digital input on ADC1
    DIDR0 = _BV(ADC1D);

    // Enable the ADC
    ADCSRA |= _BV(ADEN);
    ADCSRA |= _BV(ADSC);
}

uint16_t ADCManager::readADC() {
    ADMUX &= 0xF0;
    ADMUX |= adcChannel;
    ADCSRA |= _BV(ADSC);

    while (ADCSRA & _BV(ADSC)) {
        /* wait for ADSC to return to 0 */
    }

    // read ADCH last, once you do, ADCL disappears
    return (static_cast<uint16_t>(ADCL) + (static_cast<uint16_t>(ADCH) << 8u)) & 0x03FF;
}
