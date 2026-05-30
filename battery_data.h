#ifndef BATTERY_DATA_H
#define BATTERY_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    float voltage;
    float current;
    float soc;
    float soh;
    float temperature;

    char fault[32];

} battery_data_t;

extern battery_data_t battery_data;

#ifdef __cplusplus
}
#endif

#endif