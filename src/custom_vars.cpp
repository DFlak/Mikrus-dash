#include "custom_vars.h"
#include <string.h>
#include <cstdio>

// Storage for variables
static char var_odometer_buffer[16] = "0.0 km";
static char var_speed_buffer[16] = "0";
static char var_fps_buffer[16] = "0 FPS";
static char var_rpm_buffer[16] = "1200 RPM"; // ADD THIS

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

// ===== SPEED =====
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
    // Round to even speeds only (2, 4, 6, 8, etc)
    uint32_t speed_even = ((uint32_t)speed_kmh / 2) * 2;
    snprintf(var_speed_buffer, sizeof(var_speed_buffer), "%lu", speed_even);
}

// ===== FPS =====
const char *get_var_fps()
{
    return var_fps_buffer;
}

void set_var_fps(const char *value)
{
    if (value != nullptr)
    {
        strncpy(var_fps_buffer, value, sizeof(var_fps_buffer) - 1);
        var_fps_buffer[sizeof(var_fps_buffer) - 1] = '\0';
    }
}

void update_fps_display(uint32_t fps)
{
    snprintf(var_fps_buffer, sizeof(var_fps_buffer), "%lu FPS", fps);
}

// ===== RPM (ADD THIS) =====
const char *get_var_rpm()
{
    return var_rpm_buffer;
}

void set_var_rpm(const char *value)
{
    if (value != nullptr)
    {
        strncpy(var_rpm_buffer, value, sizeof(var_rpm_buffer) - 1);
        var_rpm_buffer[sizeof(var_rpm_buffer) - 1] = '\0';
    }
}

void update_rpm_display(uint32_t rpm)
{
    // Round RPM to nearest 100
    uint32_t rpm_rounded = ((rpm + 50) / 100) * 100;
    snprintf(var_rpm_buffer, sizeof(var_rpm_buffer), "%lu", rpm_rounded);
}
