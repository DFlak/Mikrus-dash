#ifndef COMPASS_SIMULATOR_H
#define COMPASS_SIMULATOR_H

#include <Arduino.h>

class CompassSimulator {
private:
    float current_bearing = 0.0f;
    uint32_t last_update_time = 0;
    static constexpr uint32_t ROTATION_TIME_MS = 40000;
    bool rotating_forward = true;
    
public:
    CompassSimulator() : last_update_time(millis()) {}
    
    void update() {
        uint32_t now = millis();
        uint32_t elapsed_ms = now - last_update_time;
        last_update_time = now;  // UPDATE THIS EVERY CALL
        
        float rate = 360.0f / ROTATION_TIME_MS;  // degrees per millisecond
        
        if (rotating_forward) {
            current_bearing += (elapsed_ms * rate);
            
            if (current_bearing >= 360.0f) {
                current_bearing = 360.0f;
                rotating_forward = false;
                Serial.println("Reached 360°, starting reverse rotation");
            }
        }
        else {
            current_bearing -= (elapsed_ms * rate);
            
            if (current_bearing <= 0.0f) {
                current_bearing = 0.0f;
                rotating_forward = true;
                Serial.println("Reached 0°, starting forward rotation");
            }
        }
    }
    
    float get_bearing() const {
        return current_bearing;
    }
    
    const char* get_direction() const {
        const char* directions[] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE",
                                   "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};
        int index = ((int)((current_bearing + 11.25) / 22.5)) % 16;
        return directions[index];
    }
};

extern CompassSimulator compass_simulator;

#endif
