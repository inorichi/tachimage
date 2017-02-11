#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef SKIA_STUBS_H
#define SKIA_STUBS_H

#include <stdint.h>

class SkBitmapStub16 {
public:
    mutable void*  fMipMap;
    mutable void*  fPixelRef;
    mutable size_t fPixelRefOffset;
    mutable int    fPixelLockCount;
    mutable void*  fPixels;
    mutable void*  fColorTable;
    mutable int    fRawPixelGenerationID;

    uint32_t fRowBytes;
    uint32_t fWidth;
    uint32_t fHeight;
    uint8_t  fConfig;
    uint8_t  fFlags;
    uint8_t  fBytesPerPixel;

    enum Config {
        kNo_Config,
        kA1_Config,
        kA8_Config,
        kIndex8_Config,
        kRGB_565_Config,
        kARGB_4444_Config,
        kARGB_8888_Config,
        kConfigCount
    };
};

class SkBitmapStub18 {
public:
    mutable void*  fMipMap;
    mutable void*  fPixelRef;
    mutable size_t fPixelRefOffset;
    mutable int    fPixelLockCount;
    mutable void*  fPixels;
    mutable void*  fColorTable;

    uint32_t fRowBytes;
    uint32_t fWidth;
    uint32_t fHeight;
    uint8_t  fConfig;
    uint8_t  fFlags;
    uint8_t  fBytesPerPixel;

    enum Config {
        kNo_Config,
        kA1_Config,
        kA8_Config,
        kIndex8_Config,
        kRGB_565_Config,
        kARGB_4444_Config,
        kARGB_8888_Config,
        kConfigCount
    };
};

class SkBitmapStub21 {
public:
    enum SkColorType {
        kUnknown_SkColorType,
        kAlpha_8_SkColorType,
        kRGB_565_SkColorType,
        kARGB_4444_SkColorType,
        kRGBA_8888_SkColorType,
        kBGRA_8888_SkColorType,
        kIndex_8_SkColorType,

        kLastEnum_SkColorType = kIndex_8_SkColorType,
    };
    enum SkAlphaType {};

    struct SkIPoint {
        int32_t fX, fY;
    };

    struct SkImageInfo {
        int         fWidth;
        int         fHeight;
        SkColorType fColorType;
        SkAlphaType fAlphaType;
    };

    mutable void* fPixelRef;
    mutable int   fPixelLockCount;
    mutable void* fPixels;
    mutable void* fColorTable;

    SkIPoint    fPixelRefOrigin;
    SkImageInfo fInfo;
    uint32_t    fRowBytes;
    uint8_t     fFlags;
};

#endif //SKIA_STUBS_H

#pragma clang diagnostic pop