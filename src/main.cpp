#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include "ui.h"
#include "vars.h"
#include "drivers/touch.h"
#include "touch_handler.h"
#include <string>

TouchHandler touch_handler;
uint8_t current_screen = 0;               // Which screen are we on? (0 or 1)
const uint8_t max_screens = 2;            // Total number of screens (update this when you add more)
uint32_t last_gesture_time = 0;           // When was the last gesture?
const uint32_t gesture_debounce_ms = 300; // Wait 300ms before allowing next gesture
int i;
std::string predkosc_string;

extern "C" const char *get_var_predkosc_string()
{
    return predkosc_string.c_str();
}

extern "C" void set_var_predkosc_string(const char *value)
{
    predkosc_string = value;
}
// Define the variable to be incremented
// Set the time interval in milliseconds (e.g., 100ms)
const long interval = 300;
// Variable to store the last time the action was performed
unsigned long lastTime = 0;
int32_t predkosc;
int32_t get_var_predkosc()
{
    return predkosc;
}

void set_var_predkosc(int32_t value)
{
    predkosc = value;
}

#define GFX_BL 38
// Your existing display setup
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

// LVGL buffers and driver
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[480 * 10];

// Flush callback (pixel-by-pixel)
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

void setup()
{
    Serial.begin(115200);
    touch_init();

    gfx->begin();
    gfx->fillScreen(BLACK);
#ifdef GFX_BL
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);
#endif

    // LVGL init
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
    touch_register_lvgl();
    Serial.println("Setup complete!");
}

void loop()
{
    touch_update();
    unsigned long currentTime = millis();

    // Check if the desired interval has passed
    if (currentTime - lastTime >= interval)
    {
        // Update the last time to the current time
        lastTime = currentTime;
        // Increment the variable
        predkosc++;
        set_var_predkosc(predkosc);
        if (predkosc > 200)
        {
            predkosc = 0;
        }

        char speed_buffer[6];
        snprintf(speed_buffer, sizeof(speed_buffer), "%d", predkosc);
        set_var_predkosc_string(speed_buffer);
    }
    lv_timer_handler();
    ui_tick();
    delay(1);
}
