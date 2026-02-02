#ifndef TOUCH_HANDLER_H
#define TOUCH_HANDLER_H

#include <lvgl.h>
#include <Arduino.h>
#include <math.h>

typedef enum {
    GESTURE_NONE,
    GESTURE_LEFT,
    GESTURE_RIGHT,
    GESTURE_UP,
    GESTURE_DOWN
} gesture_t;

struct TouchTracker {
    int16_t start_x = 0;
    int16_t start_y = 0;
    int16_t last_x = 0;
    int16_t last_y = 0;
    bool is_touching = false;
    uint32_t touch_start_time = 0;
};

struct GestureConfig {
    uint16_t min_distance;
    uint8_t min_velocity;
    uint16_t velocity_check_interval;
    uint8_t horizontal_weight;
    uint8_t vertical_weight;
};

class TouchHandler {
private:
    TouchTracker tracker;
    GestureConfig config;
    gesture_t last_detected_gesture = GESTURE_NONE;

public:
    TouchHandler() {}
    
    void init(const GestureConfig& cfg) {
        config = cfg;
        Serial.println("TouchHandler initialized with min_distance = " + String(cfg.min_distance));
    }
    
    void update_touch(int16_t x, int16_t y, bool pressed) {
        if (pressed && !tracker.is_touching) {
            tracker.start_x = x;
            tracker.start_y = y;
            tracker.last_x = x;
            tracker.last_y = y;
            tracker.is_touching = true;
            tracker.touch_start_time = millis();
            last_detected_gesture = GESTURE_NONE;
        } 
        else if (pressed && tracker.is_touching) {
            tracker.last_x = x;
            tracker.last_y = y;
        }
        else if (!pressed && tracker.is_touching) {
            tracker.is_touching = false;
            classify_gesture();
        }
    }
    
    void classify_gesture() {
    int16_t delta_x = tracker.last_x - tracker.start_x;
    int16_t delta_y = tracker.last_y - tracker.start_y;
    
    float distance_float = sqrt((delta_x * delta_x) + (delta_y * delta_y));
    uint16_t distance = (uint16_t)distance_float;
    
    Serial.printf("Swipe: dx=%d dy=%d dist=%u pixels\n", delta_x, delta_y, distance);
    
    if (distance < config.min_distance) {
        Serial.printf("Filtered: distance %u < minimum %u\n", distance, config.min_distance);
        last_detected_gesture = GESTURE_NONE;
        return;
    }
    
    uint16_t abs_x = abs(delta_x);
    uint16_t abs_y = abs(delta_y);
    uint16_t total = abs_x + abs_y;
    
    uint16_t dominant_threshold = (total * 60) / 100;
    
    if (abs_x > abs_y && abs_x > dominant_threshold) {
        // Horizontal swipe: negative = left, positive = right
        last_detected_gesture = (delta_x < 0) ? GESTURE_LEFT : GESTURE_RIGHT;
    } 
    else if (abs_y > abs_x && abs_y > dominant_threshold) {
        // Vertical swipe: negative = up, positive = down
        last_detected_gesture = (delta_y < 0) ? GESTURE_UP : GESTURE_DOWN;
    } 
    else {
        last_detected_gesture = GESTURE_NONE;
        Serial.println("Filtered: swipe is too diagonal");
        return;
    }
    
    const char* gesture_names[] = {"NONE", "LEFT", "RIGHT", "UP", "DOWN"};
    Serial.printf("Detected: %s\n", gesture_names[last_detected_gesture]);
}

    
    gesture_t get_gesture() const {
        return last_detected_gesture;
    }
    
    void clear_gesture() {
        last_detected_gesture = GESTURE_NONE;
    }
};

#endif
