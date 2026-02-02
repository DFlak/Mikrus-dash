#include "images.h"

const ext_img_desc_t images[8] = {
    { "rpm", &img_rpm },
    { "clock", &img_clock },
    { "speed", &img_speed },
    { "clt", &img_clt },
    { "iat", &img_iat },
    { "odo", &img_odo },
    { "fuel", &img_fuel },
    { "strzalka", &img_strzalka },
};
