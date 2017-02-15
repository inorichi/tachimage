#include "borders.h"

bool inline isFilled_ARGB_8888(const void *pixels, int width, int x, int y) {
    const uint32_t *pixelPtr = (uint32_t *)pixels + (y * width + x);

    // In this format, Skia stores the red color in the first byte, regardless of the endianness.
    return *((uint8_t *)pixelPtr) < redTreshold_RGB_A8888;
}

bool inline isFilled_RGB_565(const void *pixels, int width, int x, int y) {
    const uint16_t pixel = *((uint16_t *)pixels + (y * width + x));
    return pixel < redThreshold_RGB_565;
}

bool inline isFilled_A8(const void *pixels, int width, int x, int y) {
    const uint8_t pixel = *((uint8_t *)pixels + (y * width + x));
    return pixel < redTreshold_A8;
}

/** Return the first x position where there is a substancial amount of fill,
 * starting the search from the left. */
template<typename F>
int findBorderLeft(F&& func, const void *pixels, int width, int height) {
    int x, y;
    const int filledLimit = (int) round(height * filledRatioLimit / 2);

    // Scan vertical lines in search of filled lines
    for (x = 0; x < width; x++) {
        int filledCount = 0;

        for (y = 0; y < height; y+=2) {
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
template<typename F>
int findBorderRight(F&& func, const void *pixels, int width, int height) {
    int x, y;
    const int filledLimit = (int) round(height * filledRatioLimit / 2);

    // Scan vertical lines in search of filled lines
    for (x = width - 1; x >= 0; x--) {
        int filledCount = 0;

        for (y = 0; y < height; y+=2) {
            if (func(pixels, width, x, y)) {
                filledCount++;
            }
        }

        if (filledCount > filledLimit) {
            // This line contains enough fill
            return x + 1;
        }
    }

    // No fill found... don't crop anything
    return width;
}

/** Return the first y position where there is a substancial amount of fill,
 * starting the search from the top. */
template<typename F>
int findBorderTop(F&& func, const void *pixels, int width, int height, int left, int right) {
    int x, y;
    const int filledLimit = (int) round(width * filledRatioLimit / 2);

    // Scan horizontal lines in search of filled lines
    for (y = 0; y < height; y++) {
        int filledCount = 0;

        for (x = left; x < right; x+=2) {
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
template<typename F>
int findBorderBottom(F&& func, const void *pixels, int width, int height, int left, int right) {
    int x, y;
    const int filledLimit = (int) round(width * filledRatioLimit / 2);

    // Scan horizontal lines in search of filled lines
    for (y = height - 1; y >= 0; y--) {
        int filledCount = 0;

        for (x = left; x < right; x+=2) {
            if (func(pixels, width, x, y)) {
                filledCount++;
            }
        }

        if (filledCount > filledLimit) {
            // This line contains enough fill
            return y + 1;
        }
    }

    // No fill found... don't crop anything
    return height;
}

Borders findBorders_ARGB_8888(const void *pixels, int width, int height) {
    int left = findBorderLeft(isFilled_ARGB_8888, pixels, width, height);
    int right = findBorderRight(isFilled_ARGB_8888, pixels, width, height);

    return {
            .left = left,
            .top = findBorderTop(isFilled_ARGB_8888, pixels, width, height, left, right),
            .right = right,
            .bottom = findBorderBottom(isFilled_ARGB_8888, pixels, width, height, left, right)
    };
}

Borders findBorders_RGB_565(const void *pixels, int width, int height) {
    int left = findBorderLeft(isFilled_RGB_565, pixels, width, height);
    int right = findBorderRight(isFilled_RGB_565, pixels, width, height);

    return {
            .left = left,
            .top = findBorderTop(isFilled_RGB_565, pixels, width, height, left, right),
            .right = right,
            .bottom = findBorderBottom(isFilled_RGB_565, pixels, width, height, left, right)
    };
}

Borders findBorders_A8(const void *pixels, int width, int height) {
    int left = findBorderLeft(isFilled_A8, pixels, width, height);
    int right = findBorderRight(isFilled_A8, pixels, width, height);

    return {
            .left = left,
            .top = findBorderTop(isFilled_A8, pixels, width, height, left, right),
            .right = right,
            .bottom = findBorderBottom(isFilled_A8, pixels, width, height, left, right)
    };
}
