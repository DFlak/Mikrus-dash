#ifndef TOUCH_H
#define TOUCH_H

void touch_init(void);
void touch_register_lvgl(void);
void touch_update(void);
uint8_t get_current_screen(void);

#endif