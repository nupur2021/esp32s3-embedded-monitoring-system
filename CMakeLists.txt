SET(SOURCES ui_SMPS_SCREEN.c
    ui_HOME_SCREEN.c
    ui_INVERTERSCREEN.c
    ui_CCU_SCREEN.c
    ui_AC_CONTROLER_SCREEN_.c
    ui_EV_CHARGER_SCREEN_.c
    ui_DC_CONTROLER_SCREEN.c
    ui_LIFO_BATTERY_SCREEN.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    ui_img_smpslogo800x400_png.c
    ui_img_carbackground_800x480_png.c)

add_library(ui ${SOURCES})
