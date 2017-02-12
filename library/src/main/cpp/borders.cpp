#include "borders.h"

// Note: Skia allows configuring the pixels position in this format, so the red color may not be in
// the least significant byte. A better approach would create a 1x1 red bitmap for finding out, but
// since tachiyomi doesn't use this format, it's not handled.
bool inline isFilled_ARGB_8888(const void *pixels, int width, int x, int y) {
    // Approximation: in order to avoid computing grayscale images, which would
    // require averaging RGB, we just use one channel.
    const uint32_t pixel = *((uint32_t *)pixels + (y * width + x));
    return (pixel & RED_MASK_ARGB_8888) == 0;
}

bool inline isFilled_RGB_565(const void *pixels, int width, int x, int y) {
    // Approximation: in order to avoid computing grayscale images, which would
    // require averaging RGB, we just use one channel.
    const uint16_t pixel = *((uint16_t *)pixels + (y * width + x));
    return pixel < RED_THRESHOLD_RGB_565;
}

bool inline isFilled_A8(const void *pixels, int width, int x, int y) {
    const uint8_t pixel = *((uint8_t *)pixels + (y * width + x));
    return pixel < RED_THRESHOLD_A8;
}

/** Return the first x position where there is a substancial amount of fill,
 * starting the search from the left. */
template<typename F> int __unused findBorderLeft(F&& func, const void *pixels, int width, int height) {
    int x, y;
    const int filledLimit = (int) round(height * filledRatioLimit);

    // Scan vertical lines in search of filled lines
    for (x = 0; x < width; x++) {
        int filledCount = 0;

        for (y = 0; y < height; y++) {
            if (func(pixels, width, x, y)) {
                filledCount++;
            }
        }

        if (filledCount > filledLimit) {
            // This line contains enough fill
            return x;
        }
    }

    // No fill found... don't crop anything
    return 0;
}

/** Return the first x position where there is a substancial amount of fill,
 * starting the search from the right. */
template<typename F> int __unused findBorderRight(F&& func, const void *pixels, int width, int height) {
    int x, y;
    const int filledLimit = (int) round(height * filledRatioLimit);

    // Scan vertical lines in search of filled lines
    for (x = width - 1; x >= 0; x--) {
        int filledCount = 0;

        for (y = 0; y < height; y++) {
            if (func(pixels, width, x, y)) {
                filledCount++;
            }
        }

        if (filledCount > filledLimit) {
            // This line contains enough fill
            return x;
        }
    }

    // No fill found... don't crop anything
    return width - 1;
}

/** Return the first y position where there is a substancial amount of fill,
 * starting the search from the top. */
template<typename F> int __unused findBorderTop(F&& func, const void *pixels, int width, int height) {
    int x, y;
    const int filledLimit = (int) round(width * filledRatioLimit);

    // Scan horizontal lines in search of filled lines
    for (y = 0; y < height; y++) {
        int filledCount = 0;

        for (x = 0; x < width; x++) {
            if (func(pixels, width, x, y)) {
                filledCount++;
            }
        }

        if (filledCount > filledLimit) {
            // This line contains enough fill
            return y;
        }
    }

    // No fill found... don't crop anything
    return 0;
}

/** Return the first y position where there is a substancial amount of fill,
 * starting the search from the bottom. */
template<typename F> int __unused findBorderBottom(F&& func, const void* pixels, int width, int height) {
    int x, y;
    const int filledLimit = (int) round(width * filledRatioLimit);

    // Scan horizontal lines in search of filled lines
    for (y = height - 1; y >= 0; y--) {
        int filledCount = 0;

        for (x = 0; x < width; x++) {
            if (func(pixels, width, x, y)) {
                filledCount++;
            }
        }

        if (filledCount > filledLimit) {
            // This line contains enough fill
            return y;
        }
    }

    // No fill found... don't crop anything
    return height - 1;
}

Borders findBorders_ARGB_8888(const void *pixels, int width, int height) {
    return {
            .left = findBorderLeft(isFilled_ARGB_8888, pixels, width, height),
            .top = findBorderTop(isFilled_ARGB_8888, pixels, width, height),
            .right = findBorderRight(isFilled_ARGB_8888, pixels, width, height),
            .bottom = findBorderBottom(isFilled_ARGB_8888, pixels, width, height)
    };
}

Borders findBorders_RGB_565(const void *pixels, int width, int height) {
    return {
            .left = findBorderLeft(isFilled_RGB_565, pixels, width, height),
            .top = findBorderTop(isFilled_RGB_565, pixels, width, height),
            .right = findBorderRight(isFilled_RGB_565, pixels, width, height),
            .bottom = findBorderBottom(isFilled_RGB_565, pixels, width, height)
    };
}

Borders findBorders_A8(const void *pixels, int width, int height) {
    return {
            .left = findBorderLeft(isFilled_A8, pixels, width, height),
            .top = findBorderTop(isFilled_A8, pixels, width, height),
            .right = findBorderRight(isFilled_A8, pixels, width, height),
            .bottom = findBorderBottom(isFilled_A8, pixels, width, height)
    };
}
