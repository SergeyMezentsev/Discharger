// This file is dedicated for relay driving

#include "esp32-hal.h"
#include "esp32-hal-gpio.h"
#include <stdint.h>

#include "relay.h"
#include "voltages_structure.h"

#define RELAY_1_PIN 13
#define RELAY_2_PIN 12
#define RELAY_3_PIN 14

#define RELAY_1 0
#define RELAY_2 1
#define RELAY_3 2

#define RELAY_UP     HIGH
#define RELAY_DOWN   LOW

#define PERIOD_RELAY_MS  500

typedef struct {
    uint8_t pin;
    uint8_t state;
    int threshold;
} relay_t;
static relay_t relays[3];

static void update_relays_tresholds()
{
    relays[RELAY_1].threshold = voltages->relay_1_mv;
    relays[RELAY_2].threshold = voltages->relay_2_mv;
    relays[RELAY_3].threshold = voltages->relay_3_mv;
}

void init_relay()
{
    pinMode(RELAY_1_PIN, OUTPUT);
    pinMode(RELAY_2_PIN, OUTPUT);
    pinMode(RELAY_3_PIN, OUTPUT);

    digitalWrite(RELAY_1_PIN, LOW);
    digitalWrite(RELAY_2_PIN, LOW);
    digitalWrite(RELAY_3_PIN, LOW);

    relays[RELAY_1].state = RELAY_DOWN;
    relays[RELAY_2].state = RELAY_DOWN;
    relays[RELAY_3].state = RELAY_DOWN;
    relays[RELAY_1].pin = RELAY_1_PIN;
    relays[RELAY_2].pin = RELAY_2_PIN;
    relays[RELAY_3].pin = RELAY_3_PIN;

    update_relays_tresholds();
}

void handle_relay()
{
    static unsigned long lastTimeStamp = 0U;
    if (millis() - lastTimeStamp > PERIOD_RELAY_MS)
    {
        for (uint8_t relay = 0U; relay < 3U; ++relay)
        {
            if (relays[relay].state == RELAY_UP)
            {
                if (voltages->battery_mv < relays[relay].threshold)
                {
                    relays[relay].state = RELAY_DOWN;
                    digitalWrite(relays[relay].pin, RELAY_DOWN);
                }
            }
            else if (relays[relay].state == RELAY_DOWN)
            {
                if (voltages->battery_mv > (relays[relay].threshold + voltages->hysteresis_mv) )
                {
                    relays[relay].state = RELAY_UP;
                    digitalWrite(relays[relay].pin, RELAY_UP);
                }
            }
        }

        update_relays_tresholds();
        lastTimeStamp = millis();
    }
}







