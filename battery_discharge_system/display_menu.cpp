// This file is dedicated for display menu driving

#include <stdint.h>
#include "esp32-hal.h"

#include "buttons.h"
#include "display.h"
#include "display_menu.h"
#include "voltages_structure.h"

#define PERIOD_DISPLAY_MS 500

typedef enum { PRESSED, RELEASED } state_t;
typedef enum { BTN_UP, BTN_DOWN, BTN_SELECT, BTN_RESET } btn_type_t;
typedef enum { SHORT_PRESS, LONG_PRESS } type_of_press_t;
static struct {
    state_t state;
    btn_type_t btn_type;
    type_of_press_t type_of_press;
} btn;


typedef enum { CHANGING, NOT_CHANGING } v_is_changing_t;
static struct {
    uint8_t cur_line;   // This var takes values from 1 to 4 (they are like definitions of lines: R_1_LINE=1, R_2_LINE=2...)
    v_is_changing_t v_is_changing;
} display_state;

static uint8_t old_voltage_not_catched = 1U;
static int old_voltage;
static int cur_voltage;

// These functions are handlers that are called when certain buttons are pressed by certain way
void up_short_handler(void);
void up_long_handler(void);
void down_short_handler(void);
void down_long_handler(void);
void select_handler(void);
void reset_handler(void);

static int * get_structure_voltage_mv(uint8_t line);



void init_display_menu()
{
    // Buttons initialization
    init_btns();
    register_handlers(up_short_handler, up_long_handler,
                      down_short_handler, down_long_handler,
                      select_handler, reset_handler);
    btn.state = RELEASED;
    btn.btn_type = BTN_UP;
    btn.type_of_press = SHORT_PRESS;

    // display initialization
    init_display();
    set_bat_voltage(((float) voltages->battery_mv) / 1000.);
    set_line_voltage(R_1_LINE, ((float) voltages->relay_1_mv) / 1000.);
    set_line_voltage(R_2_LINE, ((float) voltages->relay_2_mv) / 1000.);
    set_line_voltage(R_3_LINE, ((float) voltages->relay_3_mv) / 1000.);
    set_line_voltage(H_LINE,   ((float) voltages->hysteresis_mv) / 1000.);
    display_state.cur_line = R_1_LINE;
    display_state.v_is_changing = NOT_CHANGING;

    highlight_line(display_state.cur_line);
}

void handle_display_menu()
{
    if (btn.state == PRESSED)
    {
        if (display_state.v_is_changing == CHANGING)
            switch (btn.btn_type) {
                case BTN_UP:
                    if (btn.type_of_press == SHORT_PRESS)
                    {
                        if (old_voltage_not_catched == 1U)
                        {
                            old_voltage = *get_structure_voltage_mv(display_state.cur_line);
                            cur_voltage = old_voltage;
                            old_voltage_not_catched = 0U;
                        }
                        cur_voltage += 10;
                        if (cur_voltage > 3150)
                            cur_voltage = 3150;
                        set_line_voltage(display_state.cur_line, ((float) cur_voltage) / 1000.);
                    }
                    else if (btn.type_of_press == LONG_PRESS)
                    {
                        if (old_voltage_not_catched == 1U)
                        {
                            old_voltage = *get_structure_voltage_mv(display_state.cur_line);
                            cur_voltage = old_voltage;
                            old_voltage_not_catched = 0U;
                        }
                        cur_voltage += 100;
                        if (cur_voltage > 3150)
                            cur_voltage = 3150;
                        set_line_voltage(display_state.cur_line, ((float) cur_voltage) / 1000.);
                    }
                    break;
                case BTN_DOWN:
                    if (btn.type_of_press == SHORT_PRESS)
                    {
                        if (old_voltage_not_catched == 1U)
                        {
                            old_voltage = *get_structure_voltage_mv(display_state.cur_line);
                            cur_voltage = old_voltage;
                            old_voltage_not_catched = 0U;
                        }
                        cur_voltage -= 10;
                        if (cur_voltage < 0)
                            cur_voltage = 0;
                        set_line_voltage(display_state.cur_line, ((float) cur_voltage) / 1000.);
                    }
                    else if (btn.type_of_press == LONG_PRESS)
                    {
                        if (old_voltage_not_catched == 1U)
                        {
                            old_voltage = *get_structure_voltage_mv(display_state.cur_line);
                            cur_voltage = old_voltage;
                            old_voltage_not_catched = 0U;
                        }
                        cur_voltage -= 100;
                        if (cur_voltage < 0)
                            cur_voltage = 0;
                        set_line_voltage(display_state.cur_line, ((float) cur_voltage) / 1000.);
                    }
                    break;
                case BTN_SELECT:
                    // put new voltage to the array
                    // unset flag for old voltage
                    *get_structure_voltage_mv(display_state.cur_line) = cur_voltage;
                    old_voltage_not_catched = 1U;
                    unchoose_line(display_state.cur_line);
                    highlight_line(display_state.cur_line);
                    display_state.v_is_changing = NOT_CHANGING;
                    break;
                case BTN_RESET:
                    // unset flag for old voltage
                    old_voltage_not_catched = 1U;
                    set_line_voltage(display_state.cur_line, ((float) old_voltage) / 1000.);
                    unchoose_line(display_state.cur_line);
                    highlight_line(display_state.cur_line);
                    display_state.v_is_changing = NOT_CHANGING;
                    break;
            }
        else if (display_state.v_is_changing == NOT_CHANGING)
        {
            switch (btn.btn_type) {
                case BTN_UP:
                    dehighlight_line(display_state.cur_line);
                    if (display_state.cur_line == R_1_LINE)
                        display_state.cur_line = H_LINE;
                    else
                        display_state.cur_line--;
                    highlight_line(display_state.cur_line);
                    break;
                case BTN_DOWN:
                    dehighlight_line(display_state.cur_line);
                    if (display_state.cur_line == H_LINE)
                        display_state.cur_line = R_1_LINE;
                    else
                        display_state.cur_line++;
                    highlight_line(display_state.cur_line);
                    break;
                case BTN_SELECT:
                    dehighlight_line(display_state.cur_line);
                    choose_line(display_state.cur_line);
                    display_state.v_is_changing = CHANGING;
                    break;
                case BTN_RESET:
                    // do nothing in this case
                    break;
            }
        }
        btn.state = RELEASED;
    }

    // Here it's needed to update the display every period (500ms)
    static unsigned long lastTimeStamp = 0U;
    if (millis() - lastTimeStamp > PERIOD_DISPLAY_MS)
    {
        set_bat_voltage(((float) voltages->battery_mv) / 1000.);
        for (uint8_t line = R_1_LINE; line <= H_LINE; ++line)
        {
            if (display_state.cur_line == line && display_state.v_is_changing == CHANGING)
                continue;
            set_line_voltage(line, ((float) *get_structure_voltage_mv(line)) / 1000.);
        }
        lastTimeStamp = millis();
    }
}


static int * get_structure_voltage_mv(uint8_t line)
{
    switch(line) {
        case R_1_LINE:
            return &voltages->relay_1_mv;
        case R_2_LINE:
            return &voltages->relay_2_mv;
        case R_3_LINE:
            return &voltages->relay_3_mv;
        case H_LINE:
            return &voltages->hysteresis_mv;
    }
}


void up_short_handler(void)
{
    if (btn.state == PRESSED)
        return;
    
    btn.state = PRESSED;
    btn.btn_type = BTN_UP;
    btn.type_of_press = SHORT_PRESS;
}

void up_long_handler(void)
{
    if (btn.state == PRESSED)
        return;
    
    btn.state = PRESSED;
    btn.btn_type = BTN_UP;
    btn.type_of_press = LONG_PRESS;
}

void down_short_handler(void)
{
    if (btn.state == PRESSED)
        return;
    
    btn.state = PRESSED;
    btn.btn_type = BTN_DOWN;
    btn.type_of_press = SHORT_PRESS;
}

void down_long_handler(void)
{
    if (btn.state == PRESSED)
        return;
    
    btn.state = PRESSED;
    btn.btn_type = BTN_DOWN;
    btn.type_of_press = LONG_PRESS;
}

void select_handler(void)
{
    if (btn.state == PRESSED)
        return;
    
    btn.state = PRESSED;
    btn.btn_type = BTN_SELECT;
}

void reset_handler(void)
{
    if (btn.state == PRESSED)
        return;
    
    btn.state = PRESSED;
    btn.btn_type = BTN_RESET;
}

