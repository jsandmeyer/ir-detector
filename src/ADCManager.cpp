
#include "ADCManager.h"

void ADCManager::configureADC() {
    // Pin A1 has the variable resistor
    DDRA &= ~_BV(adcPin);

    ADMUX &= 0xF0;
    ADMUX |= adcChannel; // select channel 1
    DIDR0 = _BV(ADC1D); // disable digital input on ADC1
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

    return (static_cast<uint16_t>(ADCL) + (static_cast<uint16_t>(ADCH) << 8u)) & 0x03FF;
}
