#ifndef AC_DATA_H
#define AC_DATA_H

typedef struct
{
    float voltage;
    float current;
    float frequency;

    float active_power;
    float reactive_power;
    float pf;

    float energy;
    float reactive_energy;

} AC_Data_t;

extern AC_Data_t ac_data;

#endif