#ifndef VOLTAGES_STRUCTURE_H
#define VOLTAGES_STRUCTURE_H

typedef struct {
    int battery_mv;
    int relay_1_mv;
    int relay_2_mv;
    int relay_3_mv;
    int hysteresis_mv;
} voltages_structure_p;

extern voltages_structure_p *voltages;

#endif  // VOLTAGES_STRUCTURE_H