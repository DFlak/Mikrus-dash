#include "vars.h"
#include "time_manager.h"
#include <string.h>

// Storage for variables
static int32_t var_predkosc = 0;
static char var_clock_time_buffer[16] = "00:00:00";

// ===== PREDKOSC (Speed) =====
int32_t get_var_predkosc()
{
    return var_predkosc;
}

void set_var_predkosc(int32_t value)
{
    var_predkosc = value;
}

const char *get_var_predkosc_string()
{
    static char buffer[16];
    snprintf(buffer, sizeof(buffer), "%ld", var_predkosc);
    return buffer;
}

void set_var_predkosc_string(const char *value)
{
    if (value != nullptr)
    {
        var_predkosc = atol(value);
    }
}

// ===== CLOCK_TIME =====
const char *get_var_clock_time()
{
    return var_clock_time_buffer;
}

void set_var_clock_time(const char *value)
{
    if (value != nullptr)
    {
        strncpy(var_clock_time_buffer, value, sizeof(var_clock_time_buffer) - 1);
        var_clock_time_buffer[sizeof(var_clock_time_buffer) - 1] = '\0';
    }
}
