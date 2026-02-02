#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <Arduino.h>
#include <time.h>

class TimeManager {
private:
    uint32_t start_millis;
    uint32_t start_time_seconds;
    
public:
    TimeManager() : start_millis(0), start_time_seconds(0) {}
    
    void init() {
        start_millis = millis();
        start_time_seconds = 0;
        Serial.println("TimeManager initialized: 00:00:00");
    }
    
    void sync_gps(uint32_t unix_timestamp) {
        start_millis = millis();
        start_time_seconds = unix_timestamp;
        Serial.printf("TimeManager synced to GPS: %lu\n", unix_timestamp);
    }
    
    struct tm get_time() {
        uint32_t elapsed_ms = millis() - start_millis;
        uint32_t total_seconds = start_time_seconds + (elapsed_ms / 1000);
        
        time_t time_val = (time_t)total_seconds;
        struct tm* timeinfo = gmtime(&time_val);
        return *timeinfo;
    }
    
    void get_time_string(char* buffer, size_t size) {
        struct tm timeinfo = get_time();
        snprintf(buffer, size, "%02d:%02d:%02d", 
                 timeinfo.tm_hour, 
                 timeinfo.tm_min, 
                 timeinfo.tm_sec);
    }
    
    uint32_t get_seconds() {
        uint32_t elapsed_ms = millis() - start_millis;
        return start_time_seconds + (elapsed_ms / 1000);
    }
};

extern TimeManager time_manager;

#endif
