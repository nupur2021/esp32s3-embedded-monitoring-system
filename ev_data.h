#ifndef EV_DATA_H
#define EV_DATA_H
#include <stdint.h>
typedef struct
{
     float power;        // kW or W
    uint16_t time_min;  // minutes
    uint8_t soc;        // %
    uint8_t is_charging; // 0 = stop, 1 = charging
    char status[20];    // "Charging", "Stopped", etc.
} ev_data_t;

extern ev_data_t ev_data;
#endif