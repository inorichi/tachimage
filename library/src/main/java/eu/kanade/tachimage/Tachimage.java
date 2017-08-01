package eu.kanade.tachimage;

import android.graphics.Bitmap;
import android.graphics.Rect;
import android.os.Build;
import android.support.annotation.Keep;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.util.Log;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

@SuppressWarnings({"unused", "WeakerAccess", "TryWithIdenticalCatches"})
public class Tachimage {

    private final static String TAG = "Tachimage";

    private static Field bitmapWidthField;

    private static Field bitmapHeightField;

    private static Field bitmapNativePtr;

    private static Field bitmapBuffer;

    private static Method bitmapNativeReconfigure;

    private static Field configNativeInt;

    static {
        System.loadLibrary("tachimage");
        nativeInitialize();
        try {
            bitmapWidthField = Bitmap.class.getDeclaredField("mWidth");
            bitmapHeightField = Bitmap.class.getDeclaredField("mHeight");
            bitmapNativePtr = Bitmap.class.getDeclaredField(Build.VERSION.SDK_INT >= 23 ?
                    "mNativePtr" : "mNativeBitmap");
            configNativeInt = Bitmap.Config.class.getDeclaredField("nativeInt");

            bitmapWidthField.setAccessible(true);
            bitmapHeightField.setAccessible(true);
            bitmapNativePtr.setAccessible(true);
            configNativeInt.setAccessible(true);

            if (Build.VERSION.SDK_INT >= 26) {
                bitmapNativeReconfigure = Bitmap.class.getDeclaredMethod("nativeReconfigure",
                        long.class, int.class, int.class, int.class, boolean.class);
                bitmapNativeReconfigure.setAccessible(true);
            } else if (Build.VERSION.SDK_INT >= 23) {
                bitmapNativeReconfigure = Bitmap.class.getDeclaredMethod("nativeReconfigure",
                        long.class, int.class, int.class, int.class, int.class, boolean.class);
                bitmapBuffer = Bitmap.class.getDeclaredField("mBuffer");

                bitmapNativeReconfigure.setAccessible(true);
                bitmapBuffer.setAccessible(true);
            }
        } catch (NoSuchFieldException e) {
            Log.e(TAG, e.getMessage());
        } catch (NoSuchMethodException e) {
            Log.e(TAG, e.getMessage());
        }
    }

    @Nullable
    public static Rect findBorders(@NonNull Bitmap bitmap) {
        return nativeFindBorders(bitmap);
    }

    public static boolean cropBitmap(@NonNull Bitmap bitmap, int left, int top, int width, int height) {
        if (bitmap.getWidth() == width && bitmap.getHeight() == height) {
            return false;
        }
        return nativeCrop(bitmap, left, top, width, height);
    }

    public static boolean cropBitmap(@NonNull Bitmap bitmap, @NonNull Rect rect) {
        return cropBitmap(bitmap, rect.left, rect.top, rect.width(), rect.height());
    }

    public static boolean findBordersAndCrop(@NonNull Bitmap bitmap) {
        Rect rect = findBorders(bitmap);
        return rect != null && cropBitmap(bitmap, rect);
    }

    // Called from native.
    @Keep
    static void reconfigure(@NonNull Bitmap bitmap, int width, int height) {
        try {
            long bitmapPtr = bitmapNativePtr.getLong(bitmap);

            // Android O removed the reference to the pixels array.
            if (Build.VERSION.SDK_INT >= 26) {
                int nativeInt = configNativeInt.getInt(bitmap.getConfig());
                bitmapNativeReconfigure.invoke(bitmap, bitmapPtr, width, height, nativeInt, false);
            }
            // Reconfigure is available since Kit Kat, but for some reason it breaks the bitmap and
            // it can't be rendered. It seems they fixed it in M.
            else if (Build.VERSION.SDK_INT >= 23) {
                int length = ((byte[]) bitmapBuffer.get(bitmap)).length;
                int nativeInt = configNativeInt.getInt(bitmap.getConfig());
                bitmapNativeReconfigure.invoke(bitmap, bitmapPtr, width, height, nativeInt, length, false);
            }
            else if (Build.VERSION.SDK_INT >= 21) {
                nativeChangeSize21(bitmapPtr, width, height);
            } else if (Build.VERSION.SDK_INT >= 18) {
                nativeChangeSize18(bitmapPtr, width, height);
            } else {
                nativeChangeSize16(bitmapPtr, width, height);
            }
            bitmapWidthField.set(bitmap, width);
            bitmapHeightField.set(bitmap, height);
        } catch (IllegalAccessException e) {
            Log.e(TAG, e.getMessage());
        } catch (InvocationTargetException e) {
            Log.e(TAG, e.getMessage());
        }
    }

    private native static void nativeInitialize();

    @Nullable
    private native static Rect nativeFindBorders(@NonNull Bitmap bitmap);

    private native static boolean nativeCrop(@NonNull Bitmap bitmap, int left, int top, int width, int height);

    private native static void nativeChangeSize16(long bitmapPtr, int newWidth, int newHeight);

    private native static void nativeChangeSize18(long bitmapPtr, int newWidth, int newHeight);

    private native static void nativeChangeSize21(long bitmapPtr, int newWidth, int newHeight);

}
