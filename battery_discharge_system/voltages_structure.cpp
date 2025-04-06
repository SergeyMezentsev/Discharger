

#include "voltages_structure.h"

static voltages_structure_p _voltages_structure = {
                                                .battery_mv = 0,
                                                .relay_1_mv = 2200,
                                                .relay_2_mv = 1700,
                                                .relay_3_mv = 1300,
                                                .hysteresis_mv = 200};
voltages_structure_p *voltages = &(_voltages_structure);
