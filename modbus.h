#ifndef MODBUS_H
#define MODBUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

void modbus_init(void);

//void modbus_update_data(void);
bool modbus_update_data(void);

extern bool bms_online;

#ifdef __cplusplus
}
#endif

#endif