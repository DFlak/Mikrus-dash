#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include "ui.h"
#include "vars.h"
#include "custom_vars.h"
#include "time_manager.h"
#include "actions.h"

#include "drivers/touch.h"
#include "touch_handler.h"
#include "time_manager.h"
#include "eeprom_manager.h"
#include "odometer.h"
#include "speed_simulator.h"
#include "compass_simulator.h"

#include <string>

TouchHandler touch_handler;
uint8_t current_screen = 0;
const uint8_t max_screens = 2;
uint32_t last_gesture_time = 0;
const uint32_t gesture_debounce_ms = 300;
int i;

// Compass rotation variables
static float last_bearing = -1.0f;
const float BEARING_THRESHOLD = 2.0f; // Reduced for smoother updates
static unsigned long last_time = 0;
static int loop_count = 0;

const long interval = 10;
unsigned long lastTime = 0;
#define GFX_BL 38

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    39, 48, 47,
    18, 17, 16, 21,
    11, 12, 13, 14, 0,
    8, 20, 3, 46, 9, 10,
    4, 5, 6, 7, 15);

Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
    bus, GFX_NOT_DEFINED, 0, true, 480, 480,
    st7701_type1_init_operations, sizeof(st7701_type1_init_operations), true,
    10, 8, 50, 10, 8, 20);

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[480 * 10];

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;
    for (uint32_t y = 0; y < h; y++)
    {
        for (uint32_t x = 0; x < w; x++)
        {
            gfx->writePixel(area->x1 + x, area->y1 + y, color_p[y * w + x].full);
        }
    }
    lv_disp_flush_ready(disp);
}

void touch_register_lvgl(void);

void update_clock_display()
{
    char time_buffer[16];
    time_manager.get_time_string(time_buffer, sizeof(time_buffer));
    lv_label_set_text(objects.clock_label, time_buffer);
}

void setup()
{
    Serial.begin(115200);
    delay(100);

    eeprom_manager.init();
    float saved_odometer = eeprom_manager.read_odometer();

    if (isnan(saved_odometer) || saved_odometer < 0)
    {
        saved_odometer = 0.0f;
        Serial.println("Invalid EEPROM value, starting from 0.0 km");
    }

    time_manager.init();
    set_var_clock_time("00:00:00");

    odometer.init(saved_odometer);
    update_odometer_display(odometer.get_odometer());

    randomSeed(millis());

    touch_init();
    touch_register_lvgl();

    gfx->begin();
    gfx->fillScreen(BLACK);
#ifdef GFX_BL
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);
#endif

    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 480 * 10);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 480;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    ui_init();

    Serial.println("Setup complete!");
}

void loop()
{
    // === CLOCK UPDATE ===
    static uint32_t last_clock_update = 0;
    if (millis() - last_clock_update > 500)
    {
        char time_buffer[16];
        time_manager.get_time_string(time_buffer, sizeof(time_buffer));
        set_var_clock_time(time_buffer);
        last_clock_update = millis();
    }

    // === TOUCH INPUT ===
    touch_update();

    // === SPEED & ODOMETER ===
    speed_simulator.update();
    update_speed_display(speed_simulator.get_speed());
    update_rpm_display(speed_simulator.get_rpm());

    odometer.set_speed(speed_simulator.get_speed());
    if (odometer.update())
    {
        update_odometer_display(odometer.get_odometer());
    }

    // === UI UPDATE @60fps ===
    static uint32_t last_ui_update = 0;
    uint32_t now_ui = millis();
    if (now_ui - last_ui_update >= 16)
    {
        lv_timer_handler();
        ui_tick();
        last_ui_update = now_ui;
    }

    // === COMPASS ROTATION ===
    compass_simulator.update();
    Serial.printf("Raw bearing: %.2f°\n", compass_simulator.get_bearing());
    float current_bearing = compass_simulator.get_bearing();
    float delta = fabsf(current_bearing - last_bearing);

    if (delta > BEARING_THRESHOLD || last_bearing < 0)
    {
        last_bearing = current_bearing;
        int32_t rotation_value = (int32_t)(current_bearing * 10); // Keep this
        set_var_arrow_rotation(rotation_value);

        lv_event_t dummy_event = {0};
        action_rotate_arrow(&dummy_event);
    }
    // === FPS COUNTER ===
    unsigned long now = millis();
    if (now - last_time > 1000)
    {
        Serial.printf("Loop FPS: %d\n", loop_count);
        loop_count = 0;
        last_time = now;
    }
    loop_count++;

    delay(1);
}
