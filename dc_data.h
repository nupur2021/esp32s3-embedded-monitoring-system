#ifndef DC_DATA_H
#define DC_DATA_H

typedef struct
{
    float voltage;
    float current;
    float power;
    float frequency;

} DC_Data_t;

extern DC_Data_t dc_data;

#endif