#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_rpm;
extern const lv_img_dsc_t img_clock;
extern const lv_img_dsc_t img_speed;
extern const lv_img_dsc_t img_clt;
extern const lv_img_dsc_t img_iat;
extern const lv_img_dsc_t img_odo;
extern const lv_img_dsc_t img_fuel;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[7];


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/