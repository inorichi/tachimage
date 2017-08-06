#ifndef CROP_H
#define CROP_H

#include <cstring>

void cropBitmap_ARGB_8888(void *pixels, uint32_t sourceWidth,
                          uint32_t left, uint32_t top, uint32_t width, uint32_t height);

void cropBitmap_RGB_565(void *pixels, uint32_t sourceWidth,
                        uint32_t left, uint32_t top, uint32_t width, uint32_t height);

void cropBitmap_A8(void *pixels, uint32_t sourceWidth,
                   uint32_t left, uint32_t top, uint32_t width, uint32_t height);

#endif //CROP_H
