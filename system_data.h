#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include <stdint.h>
#include <stdbool.h>

/* ================= INVERTER ================= */

typedef struct
{
    float voltage;
    float current;
    float power;
    float frequency;

    bool online;

} inverter_data_t;

/* ================= BATTERY ================= */

typedef struct
{
    float voltage;
    float current;

    float soc;
    float soh;

    float temperature;

    float max_cell_voltage;
    float min_cell_voltage;

    float max_charge_current;
    float max_discharge_current;

    uint16_t warning_code;
    uint16_t error_code;
    uint16_t status_code;

    bool charge_enable;
    bool discharge_enable;

    bool online;

} battery_data_t;

/* ================= DC CONTROLLER ================= */

typedef struct
{
    float dc_voltage;
    float dc_current;
    float dc_power;

    bool online;

} dc_data_t;

/* ================= AC CONTROLLER ================= */

typedef struct
{
    float ac_voltage;
    float ac_current;
    float ac_power;
    float frequency;

    bool online;

} ac_data_t;

/* ================= EV DATA ================= */

typedef struct
{
    float charger_voltage;
    float charger_current;
    float charger_power;

    bool charging;

    bool online;

} ev_data_t;
/* ================= MAIN SYSTEM DATA ================= */

typedef struct
{
    inverter_data_t inverter;

    battery_data_t battery;

    dc_data_t dc;

    ac_data_t ac;

    ev_data_t ev;

} system_data_t;

/* GLOBAL VARIABLE */

extern system_data_t system_data;

#endif