// This is the header for buttons driving
#ifndef BUTTONS_H
#define BUTTONS_H

void init_btns();
void register_handlers( void (* btn_up_short_handler)     (void),
                        void (* btn_up_long_handler)      (void),
                        void (* btn_down_short_handler)   (void),
                        void (* btn_down_long_handler)    (void),
                        void (* btn_select_handler)       (void),
                        void (* btn_reset_handler)        (void)
                   );

#endif  // BUTTONS_H