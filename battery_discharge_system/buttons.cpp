#include "esp32-hal-gpio.h"

// This file is dedicated for buttons driving

#define BTN_UP_PIN        16
#define BTN_DOWN_PIN      17
#define BTN_SELECT_PIN    18
#define BTN_RESET_PIN     19



void init_btns()
{
    pinMode(BTN_UP_PIN, INPUT);
    pinMode(BTN_DOWN_PIN, INPUT);
    pinMode(BTN_SELECT_PIN, INPUT);
    pinMode(BTN_RESET_PIN, INPUT);
}





