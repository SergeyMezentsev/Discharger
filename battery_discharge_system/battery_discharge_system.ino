

#include "rele.h"
#include "buttons.h"

volatile uint32_t counter = 0U;
volatile uint32_t flag = 0U;


void up_short_handler(void)
{
    counter = 1U;
    flag = 1U;
    change_rele_state(RELE_3, RELE_UP);
}

void up_long_handler(void)
{
    counter = 2U;
    flag = 1U;
    change_rele_state(RELE_3, RELE_DOWN);
}

void down_short_handler(void)
{
    counter = 3U;
    flag = 1U;
    change_rele_state(RELE_2, RELE_UP);
}

void down_long_handler(void)
{
    counter = 4U;
    flag = 1U;
    change_rele_state(RELE_2, RELE_DOWN);
}

void select_handler(void)
{
    counter = 5U;
    flag = 1U;
    change_rele_state(RELE_1, RELE_UP);
}

void reset_handler(void)
{
    counter = 6U;
    flag = 1U;
    change_rele_state(RELE_1, RELE_DOWN);
}


void setup() {
    
    init_rele();

    init_btns();
    register_handlers(up_short_handler, up_long_handler,
                      down_short_handler, down_long_handler,
                      select_handler, reset_handler);


}

void loop() {


#if 0
    if (digitalRead(BTN_UP_PIN) == HIGH)
       change_rele_state(RELE_1, RELE_UP);
    else
     change_rele_state(RELE_1, RELE_DOWN);


    if (digitalRead(BTN_DOWN_PIN) == HIGH)
       change_rele_state(RELE_2, RELE_UP);
    else
     change_rele_state(RELE_2, RELE_DOWN);
    

    if (digitalRead(BTN_SELECT_PIN) == HIGH)
       change_rele_state(RELE_3, RELE_UP);
    else
     change_rele_state(RELE_3, RELE_DOWN);
    

    if (digitalRead(BTN_RESET_PIN) == HIGH)
       change_rele_state(RELE_1, RELE_UP);
    else
     change_rele_state(RELE_1, RELE_DOWN);

#endif



    if (flag == 1U)
    {
        Serial.printf("counter = %u\n\r", counter);
        flag = 0U;
    }



}
