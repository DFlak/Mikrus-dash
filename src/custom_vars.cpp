#include "custom_vars.h"
#include <string.h>
#include <cstdio>

// Storage for variables
static char var_odometer_buffer[16] = "0.0 km";
static char var_speed_buffer[16] = "0";
static char var_fps_buffer[16] = "0 FPS";
static char var_rpm_buffer[16] = "1200";
static char var_bearing_buffer[16] = "0°";
static char var_direction_buffer[16] = "N";
static int32_t var_arrow_rotation = 0; // ADD THIS

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

// ===== RPM =====
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
    uint32_t rpm_rounded = ((rpm + 50) / 100) * 100;
    snprintf(var_rpm_buffer, sizeof(var_rpm_buffer), "%lu", rpm_rounded);
}

// ===== BEARING =====
const char *get_var_bearing()
{
    return var_bearing_buffer;
}

void set_var_bearing(const char *value)
{
    if (value != nullptr)
    {
        strncpy(var_bearing_buffer, value, sizeof(var_bearing_buffer) - 1);
        var_bearing_buffer[sizeof(var_bearing_buffer) - 1] = '\0';
    }
}

void update_bearing_display(float bearing_degrees)
{
    snprintf(var_bearing_buffer, sizeof(var_bearing_buffer), "%.0f°", bearing_degrees);
}

// ===== DIRECTION =====
const char *get_var_direction()
{
    return var_direction_buffer;
}

void set_var_direction(const char *value)
{
    if (value != nullptr)
    {
        strncpy(var_direction_buffer, value, sizeof(var_direction_buffer) - 1);
        var_direction_buffer[sizeof(var_direction_buffer) - 1] = '\0';
    }
}

void update_direction_display(const char *direction)
{
    if (direction != nullptr)
    {
        strncpy(var_direction_buffer, direction, sizeof(var_direction_buffer) - 1);
        var_direction_buffer[sizeof(var_direction_buffer) - 1] = '\0';
    }
}

// ===== ARROW ROTATION (ADD THIS) =====
int32_t get_var_arrow_rotation()
{
    return var_arrow_rotation;
}

void set_var_arrow_rotation(int32_t value)
{
    var_arrow_rotation = value % 360;
}
