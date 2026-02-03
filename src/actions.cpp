#include "actions.h"
#include "screens.h"
#include <Arduino.h>

// Your existing actions...

extern "C" void action_rotate_arrow(lv_event_t *e)
{
    extern objects_t objects;
    extern int32_t get_var_arrow_rotation();

    if (objects.strzalka != NULL)
    {
        int32_t rotation_tenths = get_var_arrow_rotation();
        Serial.printf("Setting LVGL angle to: %ld (%.1f°)\n", rotation_tenths, rotation_tenths / 10.0f);
        lv_img_set_angle(objects.strzalka, rotation_tenths);
    }
}