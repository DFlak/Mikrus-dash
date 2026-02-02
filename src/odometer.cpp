#include "odometer.h"
#include "eeprom_manager.h"

Odometer odometer;

void Odometer::save_to_eeprom()
{
    eeprom_manager.write_odometer(current_odometer_km);
}
