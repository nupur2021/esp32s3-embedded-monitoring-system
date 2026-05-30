#ifndef COMM_MANAGER_H
#define COMM_MANAGER_H

typedef enum
{
    COMM_NONE = 0,
    COMM_RS485,
    COMM_CAN

} comm_mode_t;

extern comm_mode_t active_comm;

void comm_manager_init(void);

void comm_manager_update(void);

#endif