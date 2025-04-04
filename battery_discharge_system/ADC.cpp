// File for ADC driving

#include <esp32-hal-adc.h>
#include "ADC.h"

#define ADC_PIN 15
#define ADC_RESOLUTION 12

void init_ADC()
{
    analogReadResolution(ADC_RESOLUTION);
}

unsigned int get_bat_voltage_mv()
{
    return (unsigned int) analogReadMilliVolts(ADC_PIN);
}
