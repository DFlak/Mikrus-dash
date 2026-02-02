#ifndef SPEED_SIMULATOR_H
#define SPEED_SIMULATOR_H

#include <Arduino.h>

class SpeedSimulator {
private:
    float current_speed = 0.0f;
    float target_speed = 0.0f;
    uint32_t last_change_time = 0;
    static constexpr uint32_t ACCELERATION_TIME_MS = 11200;  // 11.2 seconds for 0-140 km/h
    static constexpr float MAX_SPEED = 140.0f;
    static constexpr float MIN_SPEED = 0.0f;
    static constexpr uint32_t MIN_RPM = 1200;
    static constexpr uint32_t MAX_RPM = 8500;
    bool accelerating = true;
    
public:
    SpeedSimulator() {}
    
    void update() {
        uint32_t now = millis();
        uint32_t elapsed_ms = now - last_change_time;
        
        // Acceleration phase: 0 -> 140 km/h in 11.2 seconds
        if (accelerating && current_speed < 140.0f) {
            float rate = 140.0f / ACCELERATION_TIME_MS;  // km/h per ms
            current_speed = (elapsed_ms * rate);
            
            if (current_speed >= 140.0f) {
                current_speed = 140.0f;
                accelerating = false;
                last_change_time = now;
                Serial.println("Reached 140 km/h, starting deceleration");
            }
        }
        // Deceleration phase: 140 -> 0 km/h in 11.2 seconds
        else if (!accelerating && current_speed > 0.0f) {
            float rate = 140.0f / ACCELERATION_TIME_MS;
            current_speed = 140.0f - (elapsed_ms * rate);
            
            if (current_speed <= 0.0f) {
                current_speed = 0.0f;
                accelerating = true;
                last_change_time = now;
                Serial.println("Reached 0 km/h, restarting acceleration");
            }
        }
    }
    
    float get_speed() const {
        return current_speed;
    }
    
    uint32_t get_rpm() const {
        // Linear: 0 km/h = 1200 RPM, 140 km/h = 8500 RPM
        float rpm_float = MIN_RPM + (current_speed / 140.0f) * (MAX_RPM - MIN_RPM);
        return (uint32_t)rpm_float;
    }
};

extern SpeedSimulator speed_simulator;

#endif
