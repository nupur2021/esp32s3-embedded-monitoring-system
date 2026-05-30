#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <lvgl.h>
#include <ESP32-TWAI-CAN.hpp>
#include "gfx_conf.h"
#include "ui.h"
#include "ui_update.h"

/* =====================================================
   DISPLAY
===================================================== */

#define SCREEN_WIDTH   800
#define SCREEN_HEIGHT  480

static lv_disp_draw_buf_t draw_buf;

/* SMALLER BUFFER = FASTER + STABLE */
static lv_color_t buf1[SCREEN_WIDTH * 10];
static lv_color_t buf2[SCREEN_WIDTH * 10];

static lv_disp_drv_t disp_drv;

/* =====================================================
   RS485
===================================================== */

#define RS485_RX     17
#define RS485_TX     18
#define RS485_EN     38

HardwareSerial RS485Serial(2);

/* =====================================================
   CAN
===================================================== */

#define CAN_RX       9
#define CAN_TX       10

/* =====================================================
   TIMERS
===================================================== */

unsigned long canTimer  = 0;
unsigned long commTimer = 0;

/* =====================================================
   DISPLAY FLUSH
===================================================== */

void my_disp_flush(lv_disp_drv_t *disp,const lv_area_t *area,lv_color_t *color_p)
{
    uint32_t w =area->x2 - area->x1 + 1;
    uint32_t h =area->y2 - area->y1 + 1;
    tft.pushImageDMA(area->x1,area->y1,w,h,(lgfx::rgb565_t*)&color_p->full);
    lv_disp_flush_ready(disp);
}

/* =====================================================
   TOUCH
===================================================== */

void my_touchpad_read(lv_indev_drv_t * indev_driver,lv_indev_data_t * data)
{
    uint16_t x, y;
    bool touched = tft.getTouch(&x, &y);
    if(!touched)
    {
        data->state =
            LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = x;
        data->point.y = y;
    }
}

/* =====================================================
   DEBUG PRINT
===================================================== */

void debug_print(const char *msg)
{
    Serial.println(msg);
}

/* =====================================================
   RS485 INIT
===================================================== */

void rs485_init()
{
    pinMode(RS485_EN, OUTPUT);
    digitalWrite(RS485_EN, LOW);
    RS485Serial.begin(9600,SERIAL_8N1,RS485_RX,RS485_TX);
    Serial.println("RS485 READY");
}

/* =====================================================
   RS485 SEND
===================================================== */

void rs485_send(uint8_t *data,int len)
{
    digitalWrite(RS485_EN, HIGH);
    delayMicroseconds(100);
    RS485Serial.write(data, len);
    RS485Serial.flush();
    delayMicroseconds(100);
    digitalWrite(RS485_EN, LOW);
}

/* =====================================================
   RS485 AVAILABLE
===================================================== */

int rs485_available()
{
    return RS485Serial.available();
}

/* =====================================================
   RS485 READ
===================================================== */

uint8_t rs485_read()
{
    return RS485Serial.read();
}

/* =====================================================
   CAN UPDATE
===================================================== */

bool canbus_update_data(void)
{
    CanFrame rxFrame;

    if(ESP32Can.readFrame(rxFrame,0))
    {
        /*
           CAN DATA PROCESS HERE
        */

        return true;
    }

    return false;
}

/* =====================================================
   CAN INIT
===================================================== */

void can_init()
{
    ESP32Can.setPins( CAN_TX, CAN_RX);
    ESP32Can.setSpeed(TWAI_SPEED_500KBPS);
    if(ESP32Can.begin())
    {
        Serial.println("CAN READY");
    }
    else
    {
        Serial.println("CAN FAILED");
    }
}

/* =====================================================
   CAN SEND
===================================================== */

void can_send_message()
{
    CanFrame txFrame = {0};
    txFrame.identifier = 0x100;
    txFrame.extd = 0;
    txFrame.data_length_code = 8;
    txFrame.data[0] = 0x11;
    txFrame.data[1] = 0x22;
    txFrame.data[2] = 0x33;
    txFrame.data[3] = 0x44;
    txFrame.data[4] = 0x55;
    txFrame.data[5] = 0x66;
    txFrame.data[6] = 0x77;
    txFrame.data[7] = 0x88;

    ESP32Can.writeFrame(txFrame);
}

/* =====================================================
   COMMUNICATION UPDATE
===================================================== */

void communication_update()
{
    canbus_update_data();
}

/* =====================================================
   LVGL TIMER
===================================================== */

void lvgl_update_timer(lv_timer_t *t)
{
    ui_update_common_values();
    ui_update_dc_values();
    ui_update_inverter_values();
    ui_update_battery_values();
    ui_update_ev_values();
}

/* =====================================================
   SETUP
===================================================== */

void setup()
{
    Serial.begin(115200);

    /* DISPLAY */

    tft.begin();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);

    /* LVGL */

    lv_init();
    lv_disp_draw_buf_init( &draw_buf, buf1,buf2, SCREEN_WIDTH * 10);
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res =SCREEN_WIDTH;
    disp_drv.ver_res =SCREEN_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /* TOUCH */

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type =LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb =my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    /* UI */

    ui_init();
    lv_scr_load(ui_SMPS_SCREEN);

    /* RS485 */

    rs485_init();

    /* CAN */

    can_init();

    /* LVGL TIMER */

    lv_timer_create(lvgl_update_timer,300,NULL);
    Serial.println("SYSTEM READY");
}

/* =====================================================
   LOOP
===================================================== */

void loop()
{
    lv_timer_handler();

    /* COMMUNICATION TASK */

    if(millis() - commTimer > 50)
    {
        commTimer = millis();
        communication_update();
    }

    /* CAN SEND EVERY 1 SECOND */

    if(millis() - canTimer > 1000)
    {
        canTimer = millis();
        can_send_message();
    }

    delay(1);
}