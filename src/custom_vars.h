#ifndef CUSTOM_VARS_H
#define CUSTOM_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Custom variable functions
extern const char *get_var_odometer();
extern void set_var_odometer(const char *value);
extern const char *get_var_speed();
extern void set_var_speed(const char *value);
extern const char *get_var_fps();
extern void set_var_fps(const char *value);
extern const char *get_var_rpm();
extern void set_var_rpm(const char *value);
extern const char *get_var_bearing();
extern void set_var_bearing(const char *value);
extern const char *get_var_direction();
extern void set_var_direction(const char *value);

// Arrow rotation (from EEZ-GUI)
extern int32_t get_var_arrow_rotation();
extern void set_var_arrow_rotation(int32_t value);

#ifdef __cplusplus
}

// C++ only
void update_odometer_display(float odometer_km);
void update_speed_display(float speed_kmh);
void update_fps_display(uint32_t fps);
void update_rpm_display(uint32_t rpm);
void update_bearing_display(float bearing_degrees);
void update_direction_display(const char* direction);

#endif

#endif
