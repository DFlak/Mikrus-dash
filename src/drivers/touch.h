#ifndef TOUCH_H
#define TOUCH_H

#include <cstdint>

void touch_init(void);
void touch_register_lvgl(void);
void touch_update(void);
uint8_t get_current_screen(void);
void navigate_screen(int8_t direction);

#endif
