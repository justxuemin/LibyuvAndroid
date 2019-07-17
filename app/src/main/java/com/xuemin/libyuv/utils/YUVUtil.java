package com.xuemin.libyuv.utils;

import android.graphics.Bitmap;

public class YUVUtil {

    static {
        System.loadLibrary("yuvutil");
    }

    public static native void yuvToARGB(byte[] src, int srcWidth, int srcHeight, byte[] des);
    public static native int bitmapToYuvToARGB(Bitmap bitmap, byte[] des);
}
