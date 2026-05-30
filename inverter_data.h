#ifndef INVERTER_DATA_H
#define INVERTER_DATA_H

typedef struct
{
    float voltage;
    float current;
    float power;
    float frequency;
    float soc;
    float soh;

} Inverter_Data_t;

extern Inverter_Data_t inverter_data;

#endif