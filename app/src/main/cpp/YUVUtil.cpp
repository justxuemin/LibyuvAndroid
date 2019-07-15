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

static jbyte *convertData;
static jbyte *rotateData;

void init(jint width, jint height, jint desWidth, jint desHeight) {
    convertData = (jbyte *)malloc(sizeof(jbyte) * width * height * 3 / 2);
    rotateData = (jbyte *)malloc(sizeof(jbyte) * desWidth * desHeight * 3 / 2);
}


void JNI_OnUnload(JavaVM *jvm, void *reserved) {
    free(convertData);
    free(rotateData);
}


JNIEXPORT void JNICALL Java_com_xuemin_libyuv_utils_YUVUtil_android420ToI420
        (JNIEnv * env, jclass type, jbyteArray src, jint srcWidth, jint srcHeight, jbyteArray des, jint desWidth, jint desHeight) {
    jbyte * srcArray = env->GetByteArrayElements(src, nullptr);
    jbyte * desArray = env->GetByteArrayElements(des, nullptr);

    // 源数据
    jint srcYSize = srcWidth * srcHeight;
    jint srcUSize = (srcWidth >> 1) * (srcHeight >> 1);
    jint srcVSize = srcUSize;

    jbyte * srcY = srcArray;
    jbyte * srcU = srcArray + srcYSize;
    jbyte * srcV = srcArray + srcYSize + srcUSize;

    // 目标数据
    jint desYSize = desWidth * desHeight;
    jint desUSize = (desWidth >> 1) * (desHeight >> 1);
    jint desVSize = desUSize;


    jbyte * desY = desArray;
    jbyte * desU = desArray + desYSize;
    jbyte * desV = desArray + desYSize + desUSize;


    // 临时数据
    init(srcWidth, srcHeight, desWidth, desHeight);
    jbyte * convertDesY = convertData;
    jbyte * convertDesU = convertData + srcYSize;
    jbyte * convertDesV = convertData + srcYSize + srcUSize;

    libyuv::Android420ToI420(reinterpret_cast<const uint8_t *>(srcY), srcYSize, reinterpret_cast<const uint8_t *>(srcU), srcUSize,
                             reinterpret_cast<const uint8_t *>(srcV), srcVSize,
                             1, reinterpret_cast<uint8_t *>(convertDesY), srcYSize,
                             reinterpret_cast<uint8_t *>(convertDesU), srcUSize,
                             reinterpret_cast<uint8_t *>(convertDesV), srcVSize, srcWidth, srcHeight);
    /*
     * int I420Rotate(const uint8_t* src_y,
       int src_stride_y,
       const uint8_t* src_u,
       int src_stride_u,
       const uint8_t* src_v,
       int src_stride_v,
       uint8_t* dst_y,
       int dst_stride_y,
       uint8_t* dst_u,
       int dst_stride_u,
       uint8_t* dst_v,
       int dst_stride_v,
       int width,
       int height,
       enum RotationMode mode);
     */
    jbyte * rotateDesY = rotateData;
    jbyte * rotateDesU = rotateData + srcYSize;
    jbyte * rotateDesV = rotateData + srcVSize;

    libyuv::I420Rotate(reinterpret_cast<const uint8_t *>(convertDesY), srcYSize,
                       reinterpret_cast<const uint8_t *>(convertDesU), srcUSize,
                       reinterpret_cast<const uint8_t *>(convertDesV), srcVSize,
                       reinterpret_cast<uint8_t *>(rotateDesY), srcYSize, reinterpret_cast<uint8_t *>(rotateDesU), srcUSize,
                       reinterpret_cast<uint8_t *>(rotateDesV), srcVSize, srcWidth, srcHeight,
                       libyuv::kRotate90);


    libyuv::ConvertFromI420(reinterpret_cast<const uint8_t *>(rotateDesY), srcYSize,
                            reinterpret_cast<const uint8_t *>(rotateDesU), srcUSize,
                            reinterpret_cast<const uint8_t *>(rotateDesV), srcVSize,
                            reinterpret_cast<uint8_t *>(desArray), desYSize, desWidth, desHeight, libyuv::FOURCC_YV12);

    env->ReleaseByteArrayElements(src, srcArray, 0);
    env->ReleaseByteArrayElements(des, desArray, 0);
}

JNIEXPORT jint JNICALL Java_com_xuemin_libyuv_utils_YUVUtil_bitmapToYuv
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

    libyuv::ConvertFromI420(reinterpret_cast<const uint8_t *>(convertY), bitmapInfo.width,
                            reinterpret_cast<const uint8_t *>(convertU), bitmapInfo.width / 2,
                            reinterpret_cast<const uint8_t *>(convertV), bitmapInfo.width / 2,
                            reinterpret_cast<uint8_t *>(desArray), 0, bitmapInfo.width, bitmapInfo.height, libyuv::FOURCC_ARGB);

    AndroidBitmap_unlockPixels(env, bitmapObj);
    env->ReleaseByteArrayElements(des, desArray, 0);
    free(convertData);
    return 0;
}
