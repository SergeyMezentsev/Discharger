

// #include "rele.h"
// #include "buttons.h"
// #include "display.h"


// Example for work with buttons
//
// void up_short_handler(void)
// {
//     counter = 1U;
//     flag = 1U;
//     change_rele_state(RELE_3, RELE_UP);
// }

// void up_long_handler(void)
// {
//     counter = 2U;
//     flag = 1U;
//     change_rele_state(RELE_3, RELE_DOWN);
// }

// void down_short_handler(void)
// {
//     counter = 3U;
//     flag = 1U;
//     change_rele_state(RELE_2, RELE_UP);
// }

// void down_long_handler(void)
// {
//     counter = 4U;
//     flag = 1U;
//     change_rele_state(RELE_2, RELE_DOWN);
// }

// void select_handler(void)
// {
//     counter = 5U;
//     flag = 1U;
//     change_rele_state(RELE_1, RELE_UP);
// }

// void reset_handler(void)
// {
//     counter = 6U;
//     flag = 1U;
//     change_rele_state(RELE_1, RELE_DOWN);
// }


// void setup() {
    
//     init_rele();

//     init_btns();
//     register_handlers(up_short_handler, up_long_handler,
//                       down_short_handler, down_long_handler,
//                       select_handler, reset_handler);


// }

// void loop() {


// }


#include "ADC.h"
#include "local_server.h"


void setup()
{
    // DEBUG_START
    Serial.begin(115200);
    // DEBUG_END


    init_ADC();
    init_server("DISCHARGER");
}

void loop()
{
    handle_ADC();
    handle_server();
}





















