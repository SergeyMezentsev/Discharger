// This file provides diplay API

#ifndef DISPLAY_H
#define DISPLAY_H

#define BAT_LINE 0
#define R_1_LINE 1
#define R_2_LINE 2
#define R_3_LINE 3
#define G_LINE   4

void init_display();
void set_line_voltage(uint8_t line, float voltage);
void set_bat_voltage(float voltage);
void highlight_line(uint8_t line);
void dehighlight_line(uint8_t line);
void choose_line(uint8_t line);
void unchoose_line(uint8_t line);

#endif  // DISPLAY_H