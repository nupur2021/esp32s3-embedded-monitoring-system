#include "comm_manager.h"
#include "modbus.h"

extern bool canbus_update_data(void);

/* =====================================================
   ACTIVE MODE
===================================================== */

comm_mode_t active_comm =COMM_RS485;

/* =====================================================
   FAIL COUNTER
===================================================== */

static uint32_t fail_counter = 0;

/* =====================================================
   INIT
===================================================== */

void comm_manager_init(void)
{
    active_comm =  COMM_RS485;
    fail_counter = 0;
}

/* =====================================================
   COMMUNICATION MANAGER
===================================================== */

void comm_manager_update(void)
{
    bool ok = false;

    switch(active_comm)
    {
        /* =============================================
           RS485 MODE
        ============================================= */

        case COMM_RS485: ok = modbus_update_data();
            if(ok)
            {
                fail_counter = 0;
            }
            else
            {
                fail_counter++;

                /* SWITCH TO CAN */

                if(fail_counter >= 3)
                {
                    active_comm = COMM_CAN;
                    fail_counter = 0;
                }
            }

            break;

        /* =============================================
           CAN MODE
        ============================================= */

        case COMM_CAN:ok =canbus_update_data();
            if(ok)
            {
                fail_counter = 0;
            }
            else
            {
                fail_counter++;

                /* SWITCH TO RS485 */

                if(fail_counter >= 3)
                {
                    active_comm =    COMM_RS485;
                    fail_counter = 0;
                }
            }

            break;

        /* =============================================
           DEFAULT
        ============================================= */

        default:

            active_comm = COMM_RS485;
            fail_counter = 0;

            break;
    }
}