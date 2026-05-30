#include "modbus.h"
#include <Arduino.h>
#include <string.h>
#include "battery_data.h"
#include "dc_data.h"
#include "ac_data.h"
#include "inverter_data.h"

/* =====================================================
   BMS SETTINGS
===================================================== */

#define BMS_ID            0x01
#define START_REGISTER    0x0015
#define TOTAL_REGISTERS   12

/* =====================================================
   STATUS
===================================================== */

bool bms_online = false;

/* =====================================================
   EXTERNAL FUNCTIONS
===================================================== */

extern void rs485_send(uint8_t *data, int len);
extern int rs485_available(void);
extern uint8_t rs485_read(void);
extern void debug_print(const char *msg);

/* =====================================================
   MODBUS CRC FUNCTION
===================================================== */

static uint16_t modbus_crc( uint8_t *data, int length)
{
    uint16_t crc = 0xFFFF;
    for(int i = 0; i < length; i++)
    {
        crc ^= data[i];
        for(int j = 0; j < 8; j++)
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

/* =====================================================
   READ MODBUS REGISTERS
===================================================== */

static bool modbus_read_registers(
    uint8_t slave_id,
    uint16_t start_register,
    uint16_t register_count,
    uint16_t *register_buffer)
{
    /* -----------------------------
       CREATE REQUEST FRAME
    ----------------------------- */

    uint8_t tx_frame[8];

    tx_frame[0] = slave_id;

    tx_frame[1] = 0x03;

    tx_frame[2] = start_register >> 8;

    tx_frame[3] = start_register & 0xFF;

    tx_frame[4] = register_count >> 8;

    tx_frame[5] = register_count & 0xFF;

    uint16_t crc = modbus_crc(tx_frame, 6);

    tx_frame[6] = crc & 0xFF;

    tx_frame[7] = crc >> 8;

    /* -----------------------------
       SEND REQUEST
    ----------------------------- */

    rs485_send(tx_frame, 8);

    /* -----------------------------
       RECEIVE RESPONSE
    ----------------------------- */

    uint8_t rx_frame[64];
    int rx_index = 0;
    unsigned long start_time = millis();
    while(millis() - start_time < 20)
    {
        while(rs485_available())
        {
            if(rx_index < sizeof(rx_frame))
            {
                rx_frame[rx_index] =
                    rs485_read();

                rx_index++;
            }
        }

        /* COMPLETE RESPONSE RECEIVED */

        if(rx_index >= (5 + register_count * 2))
        {
            break;
        }
    }

    /* -----------------------------
       CHECK TIMEOUT
    ----------------------------- */

    if(rx_index < 5)
    {
        debug_print("MODBUS TIMEOUT");

        return false;
    }

    /* -----------------------------
       CHECK CRC
    ----------------------------- */

    uint16_t received_crc =
        (rx_frame[rx_index - 1] << 8) |
         rx_frame[rx_index - 2];

    uint16_t calculated_crc =
        modbus_crc(
            rx_frame,
            rx_index - 2
        );

    if(received_crc != calculated_crc)
    {
        debug_print("CRC ERROR");

        return false;
    }

    /* -----------------------------
       COPY REGISTER DATA
    ----------------------------- */

    for(int i = 0; i < register_count; i++)
    {
        register_buffer[i] =
            (rx_frame[3 + (i * 2)] << 8) |
             rx_frame[4 + (i * 2)];
    }

    return true;
}

/* =====================================================
   MODBUS INIT
===================================================== */

void modbus_init(void)
{
    debug_print("MODBUS STARTED");
}

/* =====================================================
   UPDATE REAL DATA
===================================================== */

bool modbus_update_data(void)
{
    uint16_t reg[TOTAL_REGISTERS];

    bool status =modbus_read_registers(
            BMS_ID,
            START_REGISTER,
            TOTAL_REGISTERS,
            reg
        );

    /* =================================================
       IF DATA RECEIVED SUCCESSFULLY
    ================================================= */

    if(status)
    {
        bms_online = true;

        /* ---------------------------------------------
           REGISTER MAPPING
        ---------------------------------------------

           reg[0]  = SOC
           reg[1]  = BATTERY VOLTAGE
           reg[2]  = BATTERY CURRENT
           reg[3]  = TEMPERATURE
           reg[11] = SOH

        --------------------------------------------- */

        /* =================================================
           BATTERY DATA
        ================================================= */

        battery_data.soc =  reg[0];
        battery_data.voltage =   reg[1] * 0.01f;
        battery_data.current = ((int16_t)reg[2]) * 0.01f;
        battery_data.temperature = reg[3];
        battery_data.soh = reg[11];
        strcpy( battery_data.fault, "NORMAL" );

        /* =================================================
           DC DATA
        ================================================= */

        dc_data.voltage = battery_data.voltage;
        dc_data.current = battery_data.current;
        dc_data.power =  dc_data.voltage * dc_data.current;

        /* =================================================
           INVERTER DATA
        ================================================= */

        inverter_data.voltage = battery_data.voltage;
        inverter_data.current = battery_data.current;
        inverter_data.power  inverter_data.voltage * inverter_data.current;
        inverter_data.soc = battery_data.soc;
        inverter_data.soh = battery_data.soh;
        inverter_data.frequency = 50.0f;

        /* =================================================
           AC DATA
        ================================================= */

        ac_data.voltage = 230.0f;
        ac_data.current = 5.0f;
        ac_data.frequency = 50.0f;
        ac_data.active_power = (ac_data.voltage * ac_data.current) / 1000.0f;
        ac_data.reactive_power =0.5f;
        ac_data.pf = 0.95f;
        ac_data.energy += 0.01f;
        ac_data.reactive_energy +=0.005f;
        debug_print("BMS DATA OK");
        return true;
    }

    /* =================================================
       IF COMMUNICATION FAILED
    ================================================= */

    bms_online = false;
    strcpy( battery_data.fault,"COMM FAIL");
    debug_print("BMS OFFLINE");
    return false;
}