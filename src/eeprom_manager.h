#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>

class EEPROMManager {
private:
    static const uint32_t EEPROM_SIZE = 512;
    static const uint32_t ODOMETER_ADDR = 0;  // First 4 bytes for odometer
    
public:
    EEPROMManager() {}
    
    // Initialize EEPROM (call once in setup)
    void init() {
        EEPROM.begin(EEPROM_SIZE);
        Serial.println("EEPROM initialized");
    }
    
    // Read odometer from EEPROM (in km, stored as float)
    float read_odometer() {
        float value = 0.0f;
        EEPROM.readBytes(ODOMETER_ADDR, (uint8_t *)&value, sizeof(float));
        Serial.printf("EEPROM read odometer: %.1f km\n", value);
        return value;
    }
    
    // Write odometer to EEPROM
    void write_odometer(float value) {
        EEPROM.writeBytes(ODOMETER_ADDR, (uint8_t *)&value, sizeof(float));
        EEPROM.commit();  // Commit changes to flash
        Serial.printf("EEPROM wrote odometer: %.1f km\n", value);
    }
};

extern EEPROMManager eeprom_manager;

#endif
