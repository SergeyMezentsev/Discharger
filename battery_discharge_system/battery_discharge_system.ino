
#include "ADC.h"
#include "local_server.h"
#include "relay.h"
#include "display_menu.h"


void setup()
{
    // DEBUG_START
    Serial.begin(115200);
    // DEBUG_END

    init_relay();
    init_ADC();
    init_display_menu();
    init_server("DISCHARGER");
}

void loop()
{
    handle_ADC();
    handle_relay();
    handle_display_menu();
    handle_server();
}
