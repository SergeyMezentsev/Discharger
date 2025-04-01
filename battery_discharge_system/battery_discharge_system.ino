

#include "rele.h"
// #include "buttons.cpp"




void setup() {
    
    init_rele();
    init_btns();
    


}

void loop() {


#ifdef 0
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




    // change_rele_state(RELE_3, RELE_UP);
    // delay(500);
    // change_rele_state(RELE_3, RELE_DOWN);
    // delay(500);

}
