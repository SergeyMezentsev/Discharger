#include "esp32-hal-gpio.h"

// This file is dedicated for rele driving

#include "rele.h"

#define RELE_1_PIN 13
#define RELE_2_PIN 12
#define RELE_3_PIN 14


void init_rele()
{
    pinMode(RELE_1_PIN, OUTPUT);
    pinMode(RELE_2_PIN, OUTPUT);
    pinMode(RELE_3_PIN, OUTPUT);

    digitalWrite(RELE_1_PIN, LOW);
    digitalWrite(RELE_2_PIN, LOW);
    digitalWrite(RELE_3_PIN, LOW);
}

void change_rele_state(uint8_t rele, uint8_t state)
{
    uint8_t pin = RELE_1_PIN;

    switch (rele)
    {
        case RELE_1:
            pin = RELE_1_PIN;
            break;
        case RELE_2:
            pin = RELE_2_PIN;
            break;
        case RELE_3:
            pin = RELE_3_PIN;
            break;
        default:
            pin = RELE_1_PIN;
    }

    if (state == RELE_UP)
        digitalWrite(pin, HIGH);
    else
        digitalWrite(pin, LOW);
}

