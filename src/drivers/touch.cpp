// ===== USE TAMC_GT911 LIBRARY WITH LVGL INPUT DEVICE =====
// Library automatically installed via platformio.ini
// Correct API: touch.read(), touch.touches, touch.points[]
// This code bridges GT911 hardware -> LVGL input device for button clicks
// ===================================================================

#define TOUCH_DEBUG true // Set to false to disable debug output

#include <TAMC_GT911.h>
#include <lvgl.h>
#include "ui.h"
#include "screens.h"

// GT911: SDA=19, SCL=45, INT=-1, RST=-1, Width=480, Height=480
// (INT and RST not used, set to -1)
TAMC_GT911 touch = TAMC_GT911(19, 45, -1, -1, 480, 480);

// LVGL input device - MUST BE STATIC (not on stack!)
static lv_indev_t *indev_touchpad = NULL;
static lv_indev_drv_t indev_drv; // ← FIX: Static, not local variable

// Shared touch state (updated by touch_update, read by touchpad_read)
static int16_t current_touch_x = 0;
static int16_t current_touch_y = 0;
static bool current_touch_pressed = false;

// Swipe navigation state
static uint8_t current_screen = 0; // 0 = MAIN, 1 = MAIN2
static int16_t touch_start_x = 0;
static int16_t touch_start_y = 0;
static int16_t touch_last_x = 0;
static int16_t touch_last_y = 0;
static bool touch_active = false;
const int16_t SWIPE_THRESHOLD = 50;
const int16_t VERTICAL_THRESHOLD = 30;

// Touch state
static uint32_t last_touch_time = 0;
const uint32_t TOUCH_POLL_INTERVAL = 20;

// Forward declare screen objects - defined in screens.c
extern objects_t objects;

// ===== LVGL INPUT DEVICE CALLBACK =====
// Called by LVGL to read touch state - uses shared state from touch_update()
static void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    // Simply report the current touch state (updated by touch_update)
    if (current_touch_pressed)
    {
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }

    // Report coordinates to LVGL (needed for button detection)
    data->point.x = current_touch_x;
    data->point.y = current_touch_y;
}

// ===== SWIPE DETECTION & TOUCH STATE UPDATE =====
void touch_update(void)
{
    uint32_t now = millis();
    if (now - last_touch_time < TOUCH_POLL_INTERVAL)
    {
        return;
    }
    last_touch_time = now;

    // Read hardware only once per poll interval
    touch.read();

    if (touch.touches > 0)
    {
        // Touch detected - update shared state for LVGL callback
        current_touch_x = touch.points[0].x;
        current_touch_y = touch.points[0].y;
        current_touch_pressed = true;

        int16_t touch_x = touch.points[0].x;
        int16_t touch_y = touch.points[0].y;

        if (!touch_active)
        {
            // First touch - store start position for swipe detection
            touch_active = true;
            touch_start_x = touch_x;
            touch_start_y = touch_y;
            touch_last_x = touch_x;
            touch_last_y = touch_y;

            if (TOUCH_DEBUG)
            {
                Serial.print("  [Touch START] x=");
                Serial.print(touch_start_x);
                Serial.print(" y=");
                Serial.println(touch_start_y);
            }
        }
        else
        {
            // Continuous motion - update last position
            touch_last_x = touch_x;
            touch_last_y = touch_y;

            if (TOUCH_DEBUG)
            {
                int16_t dx = touch_x - touch_start_x;
                int16_t dy = touch_y - touch_start_y;
                Serial.print("  [Touch MOVE] x=");
                Serial.print(touch_x);
                Serial.print(" y=");
                Serial.print(touch_y);
                Serial.print(" (Δdx=");
                Serial.print(dx);
                Serial.print(" Δdy=");
                Serial.println(dy);
            }
        }
    }
    else
    {
        // No touch detected
        current_touch_pressed = false;

        if (touch_active)
        {
            // Touch was released - check for swipe
            int16_t dx = touch_last_x - touch_start_x;
            int16_t dy = touch_last_y - touch_start_y;

            if (TOUCH_DEBUG)
            {
                Serial.print("  [Touch RELEASE] dx=");
                Serial.print(dx);
                Serial.print(" dy=");
                Serial.print(dy);
                Serial.print(" (thresholds: horiz≥");
                Serial.print(SWIPE_THRESHOLD);
                Serial.print(", vert≤");
                Serial.print(VERTICAL_THRESHOLD);
                Serial.println(")");
            }

            // Swipe must be primarily horizontal
            if (abs(dx) >= SWIPE_THRESHOLD && abs(dy) <= VERTICAL_THRESHOLD)
            {
                if (dx < 0)
                { // Swipe LEFT → Screen 2 (MAIN2)
                    if (current_screen == 0)
                    {
                        current_screen = 1;
                        loadScreen(SCREEN_ID_MAIN2);
                        Serial.println("✓ Swiped LEFT → Screen 2");
                    }
                }
                else
                { // Swipe RIGHT → Screen 1 (MAIN)
                    if (current_screen == 1)
                    {
                        current_screen = 0;
                        loadScreen(SCREEN_ID_MAIN);
                        Serial.println("✓ Swiped RIGHT → Screen 1");
                    }
                }
            }
            else if (TOUCH_DEBUG)
            {
                if (abs(dx) < SWIPE_THRESHOLD)
                {
                    Serial.print("  → Horizontal too small (");
                    Serial.print(abs(dx));
                    Serial.println("px)");
                }
                if (abs(dy) > VERTICAL_THRESHOLD)
                {
                    Serial.print("  → Vertical too large (");
                    Serial.print(abs(dy));
                    Serial.println("px)");
                }
            }
            touch_active = false;
        }
    }
}

void touch_init(void)
{
    Serial.println("Initializing TAMC_GT911 touch...");

    // TAMC_GT911 begins on I2C bus
    touch.begin();

    Serial.println("✓ GT911 ready");
    Serial.println("✓ Swipe left/right to navigate screens");

    if (TOUCH_DEBUG)
    {
        Serial.println("[DEBUG MODE ENABLED]");
        Serial.print("  Swipe thresholds: horizontal≥");
        Serial.print(SWIPE_THRESHOLD);
        Serial.print("px, vertical≤");
        Serial.print(VERTICAL_THRESHOLD);
        Serial.println("px");
    }
}

void touch_register_lvgl(void)
{
    Serial.println("Registering LVGL input device...");

    // Initialize LVGL input device driver (using static variable)
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;

    // Register the input device
    indev_touchpad = lv_indev_drv_register(&indev_drv);

    Serial.println("✓ LVGL input device registered - buttons are now clickable!");
}

uint8_t get_current_screen(void)
{
    return current_screen;
}