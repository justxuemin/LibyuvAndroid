package com.xuemin.libyuv.utils;

import android.graphics.Bitmap;

public class YUVUtil {

    static {
        System.loadLibrary("yuvutil");
    }

    public static native void android420ToI420(byte[] src, int srcWidth, int srcHeight, byte[] des, int desWidth, int desHeight);
    public static native int bitmapToYuv(Bitmap bitmap, byte[] des);
}
