// This file is dedicated for buttons driving

#include <Arduino.h>
#include <esp32-hal.h>
#include <esp32-hal-gpio.h>
#include <esp32-hal-timer.h>

#include "buttons.h"

#define BTN_UP     0
#define BTN_DOWN   1
#define BTN_SELECT 2
#define BTN_RESET  3

#define BTN_UP_PIN        16
#define BTN_DOWN_PIN      17
#define BTN_SELECT_PIN    18
#define BTN_RESET_PIN     19

#define TIM_PSC                     20000   // prescaler for both timers
#define DEBOUNCE_TIME_MS            70      // time for debounce (for the debounce timer)
#define ONE_COUNT_TIME_MS           50      // time for single count for duration timer
#define THRESHOLD_SLOW_MS                1000    // threshold that defines difference between short and long press for UP and DOWN btns
#define THRESHOLD_FAST_MS                200     // threshold that defines difference between short and long press for SELECT and RESET btns
#define COEFF_MS_TO_TIM_TICKS       4       // coefficient to convert ms to ticks by multiplying

// Structure to save info about current pressed btn
static volatile struct {
    uint8_t btn;
    uint8_t btn_pin;
    uint32_t duration_count;
} pressed_btn;

// Structure for saving handlers for btns
static volatile struct {
    void (* btn_up_short_handler)     (void);
    void (* btn_up_long_handler)      (void);
    void (* btn_down_short_handler)   (void);
    void (* btn_down_long_handler)    (void);
    void (* btn_select_handler)       (void);
    void (* btn_reset_handler)        (void);
} btn_handlers;

// pointers to use timers
static hw_timer_t *debounce_timer = NULL;
static hw_timer_t *duration_timer = NULL;


void IRAM_ATTR  debounce_timer_isr();
void IRAM_ATTR  duration_timer_isr();
static void call_handler();
static void attachAllBtnsInterrupts();
static void detachAllBtnsInterrupts();
static uint32_t getThreshold();

void ARDUINO_ISR_ATTR btn_up_isr();
void ARDUINO_ISR_ATTR btn_down_isr();
void ARDUINO_ISR_ATTR btn_select_isr();
void ARDUINO_ISR_ATTR btn_reset_isr();


void init_btns()
{
    pinMode(BTN_UP_PIN, INPUT);
    pinMode(BTN_DOWN_PIN, INPUT);
    pinMode(BTN_SELECT_PIN, INPUT);
    pinMode(BTN_RESET_PIN, INPUT);

    attachAllBtnsInterrupts();

    // debounce timer settings
    // Timer frequency before psc: 80 MHz
    debounce_timer = timerBegin(0, TIM_PSC, true);
    timerStop(debounce_timer);
    timerWrite(debounce_timer, 0);
    timerAttachInterrupt(debounce_timer, &debounce_timer_isr, true);
    timerAlarmWrite(debounce_timer, COEFF_MS_TO_TIM_TICKS * DEBOUNCE_TIME_MS, true);
    timerAlarmEnable(debounce_timer);

    // duration timer settings
    // Timer frequency before psc: 80 MHz
    duration_timer = timerBegin(1, TIM_PSC, true);
    timerStop(duration_timer);
    timerWrite(duration_timer, 0);
    timerAttachInterrupt(duration_timer, &duration_timer_isr, true);
    timerAlarmWrite(duration_timer, COEFF_MS_TO_TIM_TICKS * ONE_COUNT_TIME_MS, true);
    timerAlarmEnable(duration_timer);

    // Reset whole btn-press structure
    pressed_btn.btn = BTN_UP;
    pressed_btn.btn_pin = BTN_UP_PIN;
    pressed_btn.duration_count = 0U;

}

void register_handlers( void (* btn_up_short_handler_p)     (void),
                        void (* btn_up_long_handler_p)      (void),
                        void (* btn_down_short_handler_p)   (void),
                        void (* btn_down_long_handler_p)    (void),
                        void (* btn_select_handler_p)       (void),
                        void (* btn_reset_handler_p)        (void)
                   )
{
    btn_handlers.btn_up_short_handler = btn_up_short_handler_p;
    btn_handlers.btn_up_long_handler = btn_up_long_handler_p;

    btn_handlers.btn_down_short_handler = btn_down_short_handler_p;
    btn_handlers.btn_down_long_handler = btn_down_long_handler_p;

    btn_handlers.btn_select_handler = btn_select_handler_p;
    btn_handlers.btn_reset_handler = btn_reset_handler_p;
}


static void call_handler(uint8_t press_is_long)
{
    switch (pressed_btn.btn) {
        case BTN_UP:
            if (press_is_long == 1U)
                btn_handlers.btn_up_long_handler();
            else
                btn_handlers.btn_up_short_handler();
            break;
        case BTN_DOWN:
            if (press_is_long == 1U)
                btn_handlers.btn_down_long_handler();
            else
                btn_handlers.btn_down_short_handler();
            break;
        case BTN_SELECT:
            btn_handlers.btn_select_handler();
            break;
        case BTN_RESET:
            btn_handlers.btn_reset_handler();
            break;
        default:
            break;
    }
}

static void attachAllBtnsInterrupts()
{
    attachInterrupt(BTN_UP_PIN, btn_up_isr, RISING);
    attachInterrupt(BTN_DOWN_PIN, btn_down_isr, RISING);
    attachInterrupt(BTN_SELECT_PIN, btn_select_isr, RISING);
    attachInterrupt(BTN_RESET_PIN, btn_reset_isr, RISING);
}

static void detachAllBtnsInterrupts()
{
    detachInterrupt(BTN_UP_PIN);
    detachInterrupt(BTN_DOWN_PIN);
    detachInterrupt(BTN_SELECT_PIN);
    detachInterrupt(BTN_RESET_PIN);
}

static uint32_t getThreshold()
{
    static uint32_t threshold_slow = THRESHOLD_SLOW_MS * 80000 / (TIM_PSC * COEFF_MS_TO_TIM_TICKS * ONE_COUNT_TIME_MS);
    static uint32_t threshold_fast = THRESHOLD_FAST_MS * 80000 / (TIM_PSC * COEFF_MS_TO_TIM_TICKS * ONE_COUNT_TIME_MS);

    if (pressed_btn.btn == BTN_UP || pressed_btn.btn == BTN_DOWN)
    {
        return threshold_slow;
    } else {
        return threshold_fast;
    }
}


// Interrupt service routines for all btns
void ARDUINO_ISR_ATTR btn_up_isr()
{
    pressed_btn.btn = BTN_UP;
    pressed_btn.btn_pin = BTN_UP_PIN;
    detachAllBtnsInterrupts();
    timerStart(debounce_timer);
}

void ARDUINO_ISR_ATTR btn_down_isr()
{
    pressed_btn.btn = BTN_DOWN;
    pressed_btn.btn_pin = BTN_DOWN_PIN;
    detachAllBtnsInterrupts();
    timerStart(debounce_timer);
}

void ARDUINO_ISR_ATTR btn_select_isr()
{
    pressed_btn.btn = BTN_SELECT;
    pressed_btn.btn_pin = BTN_SELECT_PIN;
    detachAllBtnsInterrupts();
    timerStart(debounce_timer);
}

void ARDUINO_ISR_ATTR btn_reset_isr()
{
    pressed_btn.btn = BTN_RESET;
    pressed_btn.btn_pin = BTN_RESET_PIN;
    detachAllBtnsInterrupts();
    timerStart(debounce_timer);
}


// IRS for debounce timer
void IRAM_ATTR debounce_timer_isr()
{
    timerStop(debounce_timer);
    timerWrite(debounce_timer, 0U);

    if (digitalRead(pressed_btn.btn_pin) == HIGH)
    {
        timerStart(duration_timer);
    } else
    {
        attachAllBtnsInterrupts();
    }
}


void IRAM_ATTR duration_timer_isr()
{
    if (digitalRead(pressed_btn.btn_pin) == HIGH && pressed_btn.duration_count <= getThreshold())
    {
        ++pressed_btn.duration_count;
        timerStop(duration_timer);
        timerWrite(duration_timer, 0U);
        timerStart(duration_timer);
    } else {
        timerStop(duration_timer);
        timerWrite(duration_timer, 0U);

        if (pressed_btn.duration_count > getThreshold())
            call_handler(1U);   // Call long version of handler
        else
            call_handler(0U);   // Call short version of handler

        // Reset whole btn-press structure
        pressed_btn.btn = BTN_UP;
        pressed_btn.btn_pin = BTN_UP_PIN;
        pressed_btn.duration_count = 0U;

        attachAllBtnsInterrupts();
    }
}
