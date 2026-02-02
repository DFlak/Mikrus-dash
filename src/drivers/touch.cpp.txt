#define TOUCH_DEBUG true

#include <Arduino.h>
#include <lvgl.h>
#include <TAMC_GT911.h>
#include "../touch_handler.h"
#include "ui.h"
#include "screens.h"

TAMC_GT911 touch = TAMC_GT911(19, 45, -1, -1, 480, 480);

static lv_indev_t *indev_touchpad = NULL;
static lv_indev_drv_t indev_drv;

static int16_t current_touch_x = 0;
static int16_t current_touch_y = 0;
static bool current_touch_pressed = false;

static TouchHandler touch_handler;

static uint8_t current_screen = 0;
static uint32_t last_gesture_time = 0;
static const uint32_t GESTURE_DEBOUNCE_MS = 300;
static const uint8_t MAX_SCREENS = 2;

static uint32_t last_touch_time = 0;
static const uint32_t TOUCH_POLL_INTERVAL = 20;

extern objects_t objects;

static void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    if (current_touch_pressed)
    {
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }

    data->point.x = current_touch_x;
    data->point.y = current_touch_y;
}

void navigate_screen(int8_t direction)
{
    uint32_t now = millis();

    if (now - last_gesture_time < GESTURE_DEBOUNCE_MS)
    {
        if (TOUCH_DEBUG)
            Serial.println("[DEBOUNCED] Swipe too soon");
        return;
    }

    last_gesture_time = now;

    int16_t next_screen = (int16_t)current_screen + direction;

    if (next_screen < 0)
    {
        if (TOUCH_DEBUG)
            Serial.println("Already at first screen");
        return;
    }

    if (next_screen >= MAX_SCREENS)
    {
        if (TOUCH_DEBUG)
            Serial.println("Already at last screen");
        return;
    }

    current_screen = (uint8_t)next_screen;

    if (TOUCH_DEBUG)
        Serial.printf("Navigating to screen %d\n", current_screen);

    switch (current_screen)
    {
    case 0:
        loadScreen(SCREEN_ID_MAIN);
        if (TOUCH_DEBUG)
            Serial.println("Loaded MAIN screen");
        break;

    case 1:
        loadScreen(SCREEN_ID_MAIN2);
        if (TOUCH_DEBUG)
            Serial.println("Loaded MAIN2 screen");
        break;
    }
}

void touch_update(void)
{
    uint32_t now = millis();

    if (now - last_touch_time < TOUCH_POLL_INTERVAL)
        return;

    last_touch_time = now;

    touch.read();

    if (touch.touches > 0)
    {
        int16_t touch_x = touch.points[0].x;
        int16_t touch_y = touch.points[0].y;

        current_touch_x = touch_x;
        current_touch_y = touch_y;
        current_touch_pressed = true;

        touch_handler.update_touch(touch_x, touch_y, true);
    }
    else
    {
        if (current_touch_pressed)
        {
            current_touch_pressed = false;
            touch_handler.update_touch(0, 0, false);

            gesture_t detected = touch_handler.get_gesture();

            if (detected != GESTURE_NONE)
            {
                switch (detected)
                {
                case GESTURE_LEFT:
                    if (TOUCH_DEBUG)
                        Serial.println("LEFT swipe - next screen");
                    navigate_screen(1);
                    break;

                case GESTURE_RIGHT:
                    if (TOUCH_DEBUG)
                        Serial.println("RIGHT swipe - previous screen");
                    navigate_screen(-1);
                    break;

                case GESTURE_UP:
                    if (TOUCH_DEBUG)
                        Serial.println("UP swipe");
                    break;

                case GESTURE_DOWN:
                    if (TOUCH_DEBUG)
                        Serial.println("DOWN swipe");
                    break;

                case GESTURE_NONE:
                default:
                    break;
                }
            }

            touch_handler.clear_gesture();
        }
    }
}

void touch_init(void)
{
    Serial.println("\n=== TOUCH INIT ===");
    Serial.println("Initializing GT911...");

    touch.begin();
    Serial.println("GT911 ready");

    GestureConfig gesture_config;
    gesture_config.min_distance = 100;
    gesture_config.min_velocity = 5;
    gesture_config.velocity_check_interval = 50;
    gesture_config.horizontal_weight = 1;
    gesture_config.vertical_weight = 1;

    touch_handler.init(gesture_config);
    Serial.println("TouchHandler initialized (100px threshold)");

    if (TOUCH_DEBUG)
    {
        Serial.println("[DEBUG MODE]");
        Serial.println("Left swipe (100px) -> next screen");
        Serial.println("Right swipe (100px) -> prev screen");
        Serial.println("Debounce: 300ms");
    }

    Serial.println("===================\n");
}

void touch_register_lvgl(void)
{
    Serial.println("Registering LVGL input...");

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;

    indev_touchpad = lv_indev_drv_register(&indev_drv);

    Serial.println("LVGL input registered\n");
}

uint8_t get_current_screen(void)
{
    return current_screen;
}
