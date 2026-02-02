#ifndef ODOMETER_H
#define ODOMETER_H

#include <Arduino.h>

class Odometer {
private:
    float current_odometer_km = 0.0f;
    float current_speed_kmh = 0.0f;
    uint32_t last_update_time = 0;
    uint32_t last_save_time = 0;
    
    static const uint32_t UPDATE_INTERVAL_MS = 10000;    // Update every 10 seconds
    static const uint32_t SAVE_INTERVAL_MS = 300000;     // Save every 5 minutes
    
public:
    Odometer() {}
    
    // Initialize with value from EEPROM
    void init(float initial_value) {
        current_odometer_km = initial_value;
        last_update_time = millis();
        last_save_time = millis();
        Serial.printf("Odometer initialized: %.1f km\n", current_odometer_km);
    }
    
    // Set current speed (from GPS, or simulated)
    void set_speed(float speed_kmh) {
        current_speed_kmh = speed_kmh;
    }
    
    // Update odometer - call frequently (every loop)
    // Returns true if odometer was updated
    bool update() {
        uint32_t now = millis();
        
        // Update odometer every 10 seconds
        if (now - last_update_time >= UPDATE_INTERVAL_MS) {
            float elapsed_seconds = (now - last_update_time) / 1000.0f;
            float distance_km = (current_speed_kmh / 3600.0f) * elapsed_seconds;
            
            current_odometer_km += distance_km;
            last_update_time = now;
            
            Serial.printf("Odometer updated: speed=%.1f km/h, distance=+%.3f km, total=%.1f km\n", 
                          current_speed_kmh, distance_km, current_odometer_km);
            
            // Save to EEPROM every 5 minutes
            if (now - last_save_time >= SAVE_INTERVAL_MS) {
                save_to_eeprom();
                last_save_time = now;
            }
            
            return true;
        }
        
        return false;
    }
    
    // Get current odometer value
    float get_odometer() const {
        return current_odometer_km;
    }
    
    // Save to EEPROM manually
    void save_to_eeprom();
    
    // Reset odometer
    void reset() {
        current_odometer_km = 0.0f;
        Serial.println("Odometer reset to 0.0 km");
    }
};

extern Odometer odometer;

#endif
