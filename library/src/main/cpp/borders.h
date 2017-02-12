#ifndef BORDERS_H
#define BORDERS_H

#include <cstdlib>
#include <cmath>

/** A line will be considered as having content if 0.25% of it is filled. */
const float filledRatioLimit = 0.0025;

#define THRESHOLD 0.5

#define RED_THRESHOLD_RGB_565 (((uint16_t)(31.0 * THRESHOLD)) << (5 + 6))
#define RED_THRESHOLD_A8 ((uint8_t)(255.0 * THRESHOLD))
#define RED_MASK_ARGB_8888 ((uint32_t)(255.0 * THRESHOLD))

struct Borders{
    int left, top, right, bottom;
};

Borders findBorders_ARGB_8888(const void *pixels, int width, int height);

Borders findBorders_RGB_565(const void *pixels, int width, int height);

Borders findBorders_A8(const void *pixels, int width, int height);

#endif //BORDERS_H