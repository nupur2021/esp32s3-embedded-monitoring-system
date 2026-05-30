#include "modbus.h"

#include <Arduino.h>
#include <string.h>

#include "battery_data.h"
#include "dc_data.h"
#include "ac_data.h"
#include "inverter_data.h"

/* ================= SLAVE ID ================= */

#define BMS_ID     0x01

/* ================= STATUS ================= */

bool bms_online = false;

/* ================= CRC16 ================= */

static uint16_t modbus_crc(uint8_t *buf, int len)
{
    uint16_t crc = 0xFFFF;

    for(int pos = 0; pos < len; pos++)
    {
        crc ^= (uint16_t)buf[pos];

        for(int i = 0; i < 8; i++)
        {
            if(crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}

/* ================= MODBUS READ ================= */

static bool modbus_read_registers(
    uint8_t slave,
    uint16_t startReg,
    uint16_t count,
    uint16_t *buffer
)
{
    uint8_t tx[8];

    tx[0] = slave;

    tx[1] = 0x03;

    tx[2] = startReg >> 8;

    tx[3] = startReg & 0xFF;

    tx[4] = count >> 8;

    tx[5] = count & 0xFF;

    uint16_t crc =
        modbus_crc(tx, 6);

    tx[6] = crc & 0xFF;

    tx[7] = crc >> 8;

    /* SEND DATA */

    rs485_send(tx, 8);

    uint8_t rx[64];

    int index = 0;

    unsigned long start =
        millis();

    // while(millis() - start < 300)
    // {
    //     while(rs485_available())
    //     {
    //         rx[index++] =
    //             rs485_read();
    //     }
    // }

    while(millis() - start < 100)
{
    while(rs485_available())
    {
        if(index < sizeof(rx))
        {
            rx[index++] = rs485_read();
        }
    }

    /* response complete */
    if(index >= (5 + count * 2))
    {
        break;
    }

    delay(1);
}

    /* TIMEOUT */

    if(index < 5)
    {
        debug_print(
            "MODBUS TIMEOUT"
        );

        return false;
    }

    /* CRC CHECK */

    uint16_t rx_crc =
        (rx[index - 1] << 8) |
        rx[index - 2];

    uint16_t calc_crc =
        modbus_crc(
            rx,
            index - 2
        );

    if(rx_crc != calc_crc)
    {
        debug_print(
            "CRC FAIL"
        );

        return false;
    }

    /* COPY DATA */

    for(int i = 0; i < count; i++)
    {
        buffer[i] =
            (rx[3 + i * 2] << 8) |
            rx[4 + i * 2];
    }

    return true;
}

/* ================= INIT ================= */

void modbus_init(void)
{
    debug_print(
        "MODBUS STARTED"
    );
}

/* ================= UPDATE ================= */

void modbus_update_data(void)
{
    uint16_t reg[20];

    bool ok =
        modbus_read_registers(
            BMS_ID,
            0x0015,
            10,
            reg
        );

    if(ok)
    {   
        bms_online = true;
        debug_print("MODBUS SUCCESS");

        /* ================= BATTERY ================= */

        battery_data.soc =
            reg[0];

        battery_data.voltage =
            reg[1] * 0.01f;

        battery_data.current =
            ((int16_t)reg[2]) * 0.01f;

        battery_data.temperature =
            (int16_t)reg[3];

        battery_data.soh =
            reg[5];

        strcpy(
            battery_data.fault,
            "NORMAL"
        );

        /* ================= DC ================= */

        dc_data.voltage =
            battery_data.voltage;

        dc_data.current =
            battery_data.current;

        dc_data.power =
            dc_data.voltage *
            dc_data.current;

        /* ================= INVERTER ================= */

        inverter_data.voltage =
            battery_data.voltage;

        inverter_data.current =
            battery_data.current;

        inverter_data.power =
            inverter_data.voltage *
            inverter_data.current;

        inverter_data.soc = 

        inverter_data.soh =
            battery_data.soh;

        inverter_data.frequency =
            50.0f;

        /* ================= AC ================= */

        ac_data.voltage =
            230.0f;

        ac_data.current =
            5.0f;

        ac_data.frequency =
            50.0f;

        ac_data.active_power =
            (ac_data.voltage *
             ac_data.current) / 1000.0f;

        ac_data.reactive_power =
            0.5f;

        ac_data.pf =
            0.95f;

        ac_data.energy +=
            0.01f;

        ac_data.reactive_energy +=
            0.005f;

        /* ================= DEBUG ================= */

        debug_print(
            "BMS DATA RECEIVED"
        );
    }
    else
    {
        bms_online = false;

        strcpy(
            battery_data.fault,
            "COMM FAIL"
        );

        debug_print(
            "BMS OFFLINE"
        );
    }
}