#include <stdio.h>
#include "ui.h"
#include "ac_data.h"
#include "dc_data.h"
#include "inverter_data.h"
#include "battery_data.h"
#include "ev_data.h"
#include "modbus.h"

/* ================= AC UPDATE ================= */
void ui_update_common_values(void)
{
    char buf[32];

    if(uic_ac_voltage_value)
    {
        sprintf(buf, "%.2f V", ac_data.voltage);
        lv_label_set_text(uic_ac_voltage_value, buf);
    }

    if(uic_ac_currentvalue)
    {
        sprintf(buf, "%.2f A", ac_data.current);
        lv_label_set_text(uic_ac_currentvalue, buf);
    }

    if(uic_frequencyvalue)
    {
        sprintf(buf, "%.2f Hz", ac_data.frequency);
        lv_label_set_text(uic_frequencyvalue, buf);
    }

    if(uic_activepowervalue)
    {
        sprintf(buf, "%.2f kW", ac_data.active_power);
        lv_label_set_text(uic_activepowervalue, buf);
    }

    if(uic_reactivepower_value)
    {
        sprintf(buf, "%.2f kVAR", ac_data.reactive_power);
        lv_label_set_text(uic_reactivepower_value, buf);
    }

    if(uic_power_fac_value)
    {
        sprintf(buf, "%.2f", ac_data.pf);
        lv_label_set_text(uic_power_fac_value, buf);
    }

    if(uic_energyconsup_value)
    {
        sprintf(buf, "%.2f kWh", ac_data.energy);
        lv_label_set_text(uic_energyconsup_value, buf);
    }

    if(uic_ractive_energy_value)
    {
        sprintf(buf, "%.2f kVARh", ac_data.reactive_energy);
        lv_label_set_text(uic_ractive_energy_value, buf);
    }
}

/* ================= DC UPDATE ================= */
void ui_update_dc_values(void)
{
    char buf[32];

    if(uic_voltagevalue)
    {
        sprintf(buf, "%.2f V", dc_data.voltage);
        lv_label_set_text(uic_voltagevalue, buf);
    }

    if(uic_currentvalue)
    {
        sprintf(buf, "%.2f A", dc_data.current);
        lv_label_set_text(uic_currentvalue, buf);
    }

    if(uic_powervalue)
    {
        sprintf(buf, "%.2f W", dc_data.power);
        lv_label_set_text(uic_powervalue, buf);
    }

    if(uic_frequency__value)
    {
        sprintf(buf, "%.2f Hz", dc_data.frequency);
        lv_label_set_text(uic_frequency__value, buf);
    }
}


void ui_update_inverter_values(void)
{
    char buf[32];

    if(uic_voltage_value_)
    {
        sprintf(buf, "%.2f V", inverter_data.voltage);
        lv_label_set_text(uic_voltage_value_, buf);
    }

    if(uic_current_value)
    {
        sprintf(buf, "%.2f A", inverter_data.current);
        lv_label_set_text(uic_current_value, buf);
    }

    if(uic_power_value)
    {
        sprintf(buf, "%.2f W", inverter_data.power);
        lv_label_set_text(uic_power_value, buf);
    }

    if(uic_frequency_value)
    {
        sprintf(buf, "%.2f Hz", inverter_data.frequency);
        lv_label_set_text(uic_frequency_value, buf);
    }

    if(uic_SOC_value)
    {
        sprintf(buf, "%.2f %%", inverter_data.soc);
        lv_label_set_text(uic_SOC_value, buf);
    }

    if(uic_SOH_value)
    {
        sprintf(buf, "%.2f %%", inverter_data.soh);
        lv_label_set_text(uic_SOH_value, buf);
    }
}

void ui_update_battery_values(void)
{
    char buf[32];

    if(uic_voltage_value__)
    {
        sprintf(buf, "%.2f V", battery_data.voltage);
        lv_label_set_text(uic_voltage_value__, buf);
    }

    if(uic_current_value__)
    {
        sprintf(buf, "%.2f A", battery_data.current);
        lv_label_set_text(uic_current_value__, buf);
    }

    if(uic_soc_value__)
    {
        sprintf(buf, "%.2f %%", battery_data.soc);
        lv_label_set_text(uic_soc_value__, buf);
    }

    if(uic_temp_value)
    {
        sprintf(buf, "%.2f C", battery_data.temperature);
        lv_label_set_text(uic_temp_value, buf);
    }

   if(uic_fault_value_)
    {
    if(bms_online)
    {
        lv_label_set_text(
            uic_fault_value_,
            "ONLINE"
        );
    }
    else
    {
        lv_label_set_text(
            uic_fault_value_,
            "OFFLINE"
        );
    }
}
}

void ui_update_ev_values(void)
{
    char buf[20];

    // POWER
    if(ui_ev_power_value_)
    {
        sprintf(buf, "%.1f kW", ev_data.power);
        lv_label_set_text(ui_ev_power_value_, buf);
    }

    // TIME
    if(ui_ev_time_value)
    {
        sprintf(buf, "%d min", ev_data.time_min);
        lv_label_set_text(ui_ev_time_value, buf);
    }

    // ARC (SOC %)
    if(ui_ev_arc)
    {
        lv_arc_set_value(ui_ev_arc, ev_data.soc);
    }

    if(ui_ev_display_arc)
    {
        sprintf(buf, "%d%%", ev_data.soc);
        lv_label_set_text(ui_ev_display_arc, buf);
    }

    // STATUS
    if(ui_ev_dsplay_status_)
    {
        lv_label_set_text(ui_ev_dsplay_status_, ev_data.status);
    }
}