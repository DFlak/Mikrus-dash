#include "custom_vars.h"
#include <string.h>
#include <cstdio>

// Storage for variables
static char var_odometer_buffer[16] = "0.0 km";
static char var_speed_buffer[16] = "0 km/h"; // ADD THIS

// ===== ODOMETER =====
const char *get_var_odometer()
{
    return var_odometer_buffer;
}

void set_var_odometer(const char *value)
{
    if (value != nullptr)
    {
        strncpy(var_odometer_buffer, value, sizeof(var_odometer_buffer) - 1);
        var_odometer_buffer[sizeof(var_odometer_buffer) - 1] = '\0';
    }
}

void update_odometer_display(float odometer_km)
{
    snprintf(var_odometer_buffer, sizeof(var_odometer_buffer), "%.1f km", odometer_km);
}

// ===== SPEED (ADD THIS) =====
const char *get_var_speed()
{
    return var_speed_buffer;
}

void set_var_speed(const char *value)
{
    if (value != nullptr)
    {
        strncpy(var_speed_buffer, value, sizeof(var_speed_buffer) - 1);
        var_speed_buffer[sizeof(var_speed_buffer) - 1] = '\0';
    }
}

void update_speed_display(float speed_kmh)
{
    snprintf(var_speed_buffer, sizeof(var_speed_buffer), "%.0f", speed_kmh);
}
