#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations



// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_CLOCK_TIME = 0,
    FLOW_GLOBAL_VARIABLE_ODOMETER = 1,
    FLOW_GLOBAL_VARIABLE_SPEED = 2
};

// Native global variables

extern int32_t get_var_predkosc();
extern void set_var_predkosc(int32_t value);
extern const char *get_var_predkosc_string();
extern void set_var_predkosc_string(const char *value);
extern const char *get_var_clock_time();
extern void set_var_clock_time(const char *value);
extern const char *get_var_odometer();
extern void set_var_odometer(const char *value);
extern const char *get_var_speed();
extern void set_var_speed(const char *value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/