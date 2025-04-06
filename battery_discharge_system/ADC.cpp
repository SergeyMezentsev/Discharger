// File for ADC driving

#include "esp32-hal.h"
#include <esp32-hal-adc.h>
#include "ADC.h"
#include "voltages_structure.h"

// DEBUG_START
#include <HardwareSerial.h>
// DEBUG_END



#define ADC_PIN        33
#define ADC_RESOLUTION 12
#define PERIOD_ADC_MS  1000

void init_ADC()
{
    analogReadResolution(ADC_RESOLUTION);
}

void handle_ADC()
{
    static unsigned long lastTimeStamp = 0U;
    if (millis() - lastTimeStamp > PERIOD_ADC_MS)
    {
        voltages->battery_mv = (int) analogReadMilliVolts(ADC_PIN);
        lastTimeStamp = millis();

        // DEBUG_START
        Serial.printf("ADC = %d\n", voltages->battery_mv);
        // DEBUG_END
    }

}

