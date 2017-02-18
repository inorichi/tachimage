#include <jni.h>
#include <android/bitmap.h>
#include "skia-stubs.h"
#include "borders.h"
#include "crop.h"

static jclass rectClass;
static jmethodID rectCtor;

static jclass tachimageClass;
static jmethodID reconfigureMethod;

extern "C"
JNIEXPORT void JNICALL
Java_eu_kanade_tachimage_Tachimage_nativeInitialize(JNIEnv *env, jclass type) {
    jclass tmpClass;

    tmpClass = env->FindClass("android/graphics/Rect");
    rectClass = (jclass) env->NewGlobalRef(tmpClass);
    env->DeleteLocalRef(tmpClass);
    rectCtor = env->GetMethodID(rectClass, "<init>", "(IIII)V");

    tmpClass = env->FindClass("eu/kanade/tachimage/Tachimage");
    tachimageClass = (jclass) env->NewGlobalRef(tmpClass);
    env->DeleteLocalRef(tmpClass);
    reconfigureMethod = env->GetStaticMethodID(tachimageClass, "reconfigure",
                                               "(Landroid/graphics/Bitmap;II)V");
}

Borders findBorders(void *pixels, int width, int height, int format) {
    if (format == ANDROID_BITMAP_FORMAT_RGB_565) {
        return findBorders_RGB_565(pixels, width, height);
    } else if (format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        return findBorders_ARGB_8888(pixels, width, height);
    } else if (format == ANDROID_BITMAP_FORMAT_A_8) {
        return findBorders_A8(pixels, width, height);
    } else {
        return { .left = 0, .top = 0, .right = width, .bottom = height };
    }
}

extern "C" JNIEXPORT
jobject JNICALL
Java_eu_kanade_tachimage_Tachimage_nativeFindBorders(JNIEnv *env, jclass type, jobject bitmap) {
    AndroidBitmapInfo info;
    void *pixels;

    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) {
        return nullptr;
    }

    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) {
        return nullptr;
    }

    Borders b = findBorders(pixels, info.width, info.height, info.format);

    AndroidBitmap_unlockPixels(env, bitmap);

    return env->NewObject(rectClass, rectCtor, b.left, b.top, b.right, b.bottom);
}

extern "C" JNIEXPORT
bool JNICALL
Java_eu_kanade_tachimage_Tachimage_nativeCrop(JNIEnv *env, jclass type, jobject bitmap,
                                              uint32_t left, uint32_t top, uint32_t width,
                                              uint32_t height) {

    AndroidBitmapInfo info;
    void* pixels;

    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) {
        return false;
    }

    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) {
        return false;
    }

    bool result = true;
    if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        env->CallStaticVoidMethod(tachimageClass, reconfigureMethod, bitmap, width, height);
        cropBitmap_RGB_565(pixels, info.width, left, top, width, height);
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        env->CallStaticVoidMethod(tachimageClass, reconfigureMethod, bitmap, width, height);
        cropBitmap_ARGB_8888(pixels, info.width, left, top, width, height);
    } else if (info.format == ANDROID_BITMAP_FORMAT_A_8) {
        env->CallStaticVoidMethod(tachimageClass, reconfigureMethod, bitmap, width, height);
        cropBitmap_A8(pixels, info.width, left, top, width, height);
    } else {
        result = false;
    }

    AndroidBitmap_unlockPixels(env, bitmap);

    return result;
}

template<class T> void changeImageSize(T *bitmapPtr, uint32_t newWidth, uint32_t newHeight) {
    bitmapPtr->fRowBytes = bitmapPtr->fRowBytes / bitmapPtr->fWidth * newWidth;
    bitmapPtr->fWidth = newWidth;
    bitmapPtr->fHeight = newHeight;
}

extern "C" JNIEXPORT
void JNICALL
Java_eu_kanade_tachimage_Tachimage_nativeChangeSize16(JNIEnv *env, jclass type, jlong bitmapPtr,
                                                      uint32_t newWidth, uint32_t newHeight) {

    changeImageSize((SkBitmapStub16 *) bitmapPtr, newWidth, newHeight);
}

extern "C" JNIEXPORT
void JNICALL
Java_eu_kanade_tachimage_Tachimage_nativeChangeSize18(JNIEnv *env, jclass type, jlong bitmapPtr,
                                                      uint32_t newWidth, uint32_t newHeight) {

    changeImageSize((SkBitmapStub18 *) bitmapPtr, newWidth, newHeight);
}


extern "C" JNIEXPORT
void JNICALL
Java_eu_kanade_tachimage_Tachimage_nativeChangeSize21(JNIEnv *env, jclass type, jlong bitmapPtr,
                                                      uint32_t newWidth, uint32_t newHeight) {

    SkBitmapStub21 * stubPtr = (SkBitmapStub21 *) bitmapPtr;
    stubPtr->fRowBytes = stubPtr->fRowBytes / stubPtr->fInfo.fWidth * newWidth;
    stubPtr->fInfo.fWidth = newWidth;
    stubPtr->fInfo.fHeight = newHeight;
}
