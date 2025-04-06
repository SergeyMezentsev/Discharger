// This file represents structure that saves current voltages for the whole system

#include "voltages_structure.h"

// Here we have start meanings for all voltages in the system
static voltages_structure_p _voltages_structure = {
                                                .battery_mv = 0,
                                                .relay_1_mv = 2500,
                                                .relay_2_mv = 1700,
                                                .relay_3_mv = 800,
                                                .hysteresis_mv = 300};
voltages_structure_p *voltages = &(_voltages_structure);
