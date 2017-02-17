#include "crop.h"

template<typename T> void cropBitmap(T *pixels, uint32_t sourceWidth,
                                     uint32_t left, uint32_t top, uint32_t width, uint32_t height) {

    T *whereToGet = pixels + left + top * sourceWidth;
    T *whereToPut = pixels;

    size_t rowSize = sizeof(T) * width;

    uint32_t end = height + top;

    for (uint32_t i = top; i < end; i++) {
        memmove(whereToPut, whereToGet, rowSize);
        whereToGet += sourceWidth;
        whereToPut += width;
    }
}

void cropBitmap_ARGB_8888(void *pixels, uint32_t sourceWidth,
                          uint32_t left, uint32_t top, uint32_t width, uint32_t height) {

    cropBitmap((uint32_t *)pixels, sourceWidth, left, top, width, height);
}

void cropBitmap_RGB_565(void *pixels, uint32_t sourceWidth,
                        uint32_t left, uint32_t top, uint32_t width, uint32_t height) {

    cropBitmap((uint16_t *)pixels, sourceWidth, left, top, width, height);
}

void cropBitmap_A8(void *pixels, uint32_t sourceWidth, uint32_t left, uint32_t top,
                   uint32_t width, uint32_t height) {

    cropBitmap((uint8_t *)pixels, sourceWidth, left, top, width, height);
}
