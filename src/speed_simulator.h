#ifndef SPEED_SIMULATOR_H
#define SPEED_SIMULATOR_H

#include <Arduino.h>

class SpeedSimulator {
private:
    float current_speed = 0.0f;
    float target_speed = 0.0f;
    uint32_t last_change_time = 0;
    static constexpr uint32_t ACCELERATION_TIME_MS = 8000;  // 8 seconds to reach 100 km/h
    static constexpr float MAX_SPEED = 140.0f;
    static constexpr float MIN_SPEED = 0.0f;
    bool accelerating = true;  // Start by accelerating
    
public:
    SpeedSimulator() {}
    
    // Update speed with smooth acceleration/deceleration
    void update() {
        uint32_t now = millis();
        uint32_t elapsed_ms = now - last_change_time;
        
        // Acceleration phase: 0 -> 100 km/h in 8 seconds
        if (accelerating && current_speed < 140.0f) {
            float rate = 100.0f / ACCELERATION_TIME_MS;  // km/h per ms
            current_speed = (elapsed_ms * rate);
            
            if (current_speed >= 140.0f) {
                current_speed = 140.0f;
                // Start deceleration phase
                accelerating = false;
                last_change_time = now;
                Serial.println("Reached 140 km/h, starting deceleration");
            }
        }
        // Deceleration phase: 140 -> 0 km/h
        else if (!accelerating && current_speed > 0.0f) {
            float rate = 100.0f / ACCELERATION_TIME_MS;  // Same rate for symmetry
            current_speed = 140.0f - (elapsed_ms * rate);
            
            if (current_speed <= 0.0f) {
                current_speed = 0.0f;
                // Start acceleration phase again
                accelerating = true;
                last_change_time = now;
                Serial.println("Reached 0 km/h, restarting acceleration");
            }
        }
    }
    
    float get_speed() const {
        return current_speed;
    }
};

extern SpeedSimulator speed_simulator;

#endif
