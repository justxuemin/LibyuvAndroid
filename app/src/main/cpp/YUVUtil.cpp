#include <jni.h>
#include <string>
#include "libyuv.h"
#include <android/log.h>
#include <android/bitmap.h>
#include "com_xuemin_libyuv_utils_YUVUtil.h"

#define TAG "libyuv" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型


JNIEXPORT void JNICALL Java_com_xuemin_libyuv_utils_YUVUtil_yuvToARGB
        (JNIEnv * env, jclass type, jbyteArray src, jint srcWidth, jint srcHeight, jbyteArray des) {
    jbyte * srcArray = env->GetByteArrayElements(src, nullptr);
    jbyte * desArray = env->GetByteArrayElements(des, nullptr);

    jbyte * srcY = srcArray;
    jbyte * srcU = srcArray + (srcWidth * srcHeight) * sizeof(jbyte);
    jbyte * srcV = srcArray + (srcWidth * srcHeight) * sizeof(jbyte) + (srcWidth * srcHeight) / 4 * sizeof(jbyte);

    jbyte * convertData = static_cast<jbyte *>(malloc(sizeof(jbyte) * srcWidth * srcHeight * 3 / 2));
    jbyte * convertY = convertData;
    jbyte * convertU = convertData + (srcWidth * srcHeight) * sizeof(jbyte);
    jbyte * convertV = convertData + (srcWidth * srcHeight) * sizeof(jbyte) + (srcWidth * srcHeight) / 4 * sizeof(jbyte);
    libyuv::Android420ToI420(reinterpret_cast<const uint8_t *>(srcY), srcWidth, reinterpret_cast<const uint8_t *>(srcU), srcWidth / 2,
                             reinterpret_cast<const uint8_t *>(srcV), srcWidth / 2, 1,
                             reinterpret_cast<uint8_t *>(convertY), srcWidth, reinterpret_cast<uint8_t *>(convertU), srcWidth / 2,
                             reinterpret_cast<uint8_t *>(convertV), srcWidth / 2, srcWidth, srcHeight);

    jbyte * rotateData = static_cast<jbyte *>(malloc((sizeof(jbyte)) * srcWidth * srcHeight * 3 / 2));
    jbyte * rotateY = rotateData;
    jbyte * rotateU = rotateData + (srcWidth * srcHeight) * sizeof(jbyte);
    jbyte * rotateV = rotateData + (srcWidth * srcHeight) * sizeof(jbyte) + (srcWidth * srcHeight) / 4 * sizeof(jbyte);

    libyuv::I420Rotate(reinterpret_cast<const uint8_t *>(convertY), srcWidth,
                       reinterpret_cast<const uint8_t *>(convertU), srcWidth / 2,
                       reinterpret_cast<const uint8_t *>(convertV), srcWidth / 2,
                       reinterpret_cast<uint8_t *>(rotateY), srcHeight, reinterpret_cast<uint8_t *>(rotateU), srcHeight / 2,
                       reinterpret_cast<uint8_t *>(rotateV), srcHeight/ 2, srcWidth, srcHeight, libyuv::kRotate90);

    libyuv::ConvertFromI420(reinterpret_cast<const uint8_t *>(rotateY), srcHeight,
                            reinterpret_cast<const uint8_t *>(rotateU), srcHeight / 2,
                            reinterpret_cast<const uint8_t *>(rotateV), srcHeight / 2,
                            reinterpret_cast<uint8_t *>(desArray), 0, srcHeight, srcWidth, libyuv::FOURCC_ARGB);

    free(convertData);
    free(rotateData);
    env->ReleaseByteArrayElements(src, srcArray, 0);
    env->ReleaseByteArrayElements(des, desArray, 0);
}

JNIEXPORT jint JNICALL Java_com_xuemin_libyuv_utils_YUVUtil_bitmapToYuvToARGB
        (JNIEnv *env, jclass type, jobject bitmapObj, jbyteArray des) {

    jbyte * desArray = env->GetByteArrayElements(des, nullptr);
    jint length = env->GetArrayLength(des);
    LOGE("des length %d", length);

    AndroidBitmapInfo bitmapInfo;
    if (AndroidBitmap_getInfo(env, bitmapObj, &bitmapInfo) < 0) {
        return -1;
    }

    int* bitmapData = nullptr;
    if (AndroidBitmap_lockPixels(env, bitmapObj, reinterpret_cast<void **>(&bitmapData))) {
        return -1;
    }
    /*
     * int RGBAToI420(const uint8_t* src_rgba,
               int src_stride_rgba,
               uint8_t* dst_y,
               int dst_stride_y,
               uint8_t* dst_u,
               int dst_stride_u,
               uint8_t* dst_v,
               int dst_stride_v,
               int width,
               int height)
     */
    LOGE("Bitmap format : format-%d, stride-%d, width-%d, height-%d", bitmapInfo.format, bitmapInfo.stride, bitmapInfo.width, bitmapInfo.height);
    jbyte * convertData = static_cast<jbyte *>(malloc(sizeof(jbyte) * bitmapInfo.width * bitmapInfo.height * 3 / 2));
    jbyte * convertY = convertData;
    jbyte * convertU = convertData + (bitmapInfo.width * bitmapInfo.height) * sizeof(jbyte);
    jbyte * convertV = convertData + (bitmapInfo.width * bitmapInfo.height) * sizeof(jbyte) + (bitmapInfo.width * bitmapInfo.height) / 4 * sizeof(jbyte);

    // YV12 与 I420都是(4:2:0)
    // YV12 : Y + V + U
    // I420 : Y + U + V
    libyuv::ARGBToI420(reinterpret_cast<const uint8_t *>(bitmapData), bitmapInfo.stride,
                       reinterpret_cast<uint8_t *>(convertY), bitmapInfo.width, reinterpret_cast<uint8_t *>(convertU), bitmapInfo.width / 2,
                       reinterpret_cast<uint8_t *>(convertV), bitmapInfo.width / 2, bitmapInfo.width, bitmapInfo.height);

    jbyte * rotateData = static_cast<jbyte *>(malloc((sizeof(jbyte)) * bitmapInfo.width * bitmapInfo.height * 3 / 2));
    jbyte * rotateY = rotateData;
    jbyte * rotateU = rotateData + (bitmapInfo.width * bitmapInfo.height) * sizeof(jbyte);
    jbyte * rotateV = rotateData + (bitmapInfo.width * bitmapInfo.height) * sizeof(jbyte) + (bitmapInfo.width * bitmapInfo.height) / 4 * sizeof(jbyte);

    libyuv::I420Rotate(reinterpret_cast<const uint8_t *>(convertY), bitmapInfo.width,
                       reinterpret_cast<const uint8_t *>(convertU), bitmapInfo.width / 2,
                       reinterpret_cast<const uint8_t *>(convertV), bitmapInfo.width / 2,
                       reinterpret_cast<uint8_t *>(rotateY), bitmapInfo.height, reinterpret_cast<uint8_t *>(rotateU), bitmapInfo.height / 2,
                       reinterpret_cast<uint8_t *>(rotateV), bitmapInfo.height / 2, bitmapInfo.width, bitmapInfo.height, libyuv::kRotate90);

    libyuv::ConvertFromI420(reinterpret_cast<const uint8_t *>(rotateY), bitmapInfo.height,
                            reinterpret_cast<const uint8_t *>(rotateU), bitmapInfo.height / 2,
                            reinterpret_cast<const uint8_t *>(rotateV), bitmapInfo.height / 2,
                            reinterpret_cast<uint8_t *>(desArray), 0, bitmapInfo.height, bitmapInfo.width, libyuv::FOURCC_ARGB);

    AndroidBitmap_unlockPixels(env, bitmapObj);
    env->ReleaseByteArrayElements(des, desArray, 0);
    free(convertData);
    free(rotateData);
    return 0;
}
