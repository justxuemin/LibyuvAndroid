/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_xuemin_libyuv_utils_YUVUtil */

#ifndef _Included_com_xuemin_libyuv_utils_YUVUtil
#define _Included_com_xuemin_libyuv_utils_YUVUtil
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_xuemin_libyuv_utils_YUVUtil
 * Method:    android420ToI420
 * Signature: ([BII[BII)V
 */
JNIEXPORT void JNICALL Java_com_xuemin_libyuv_utils_YUVUtil_android420ToI420
  (JNIEnv *, jclass, jbyteArray, jint, jint, jbyteArray, jint, jint);

/*
 * Class:     com_xuemin_libyuv_utils_YUVUtil
 * Method:    rgbaToYuv
 * Signature: ([BII[BII)V
 */
JNIEXPORT int JNICALL Java_com_xuemin_libyuv_utils_YUVUtil_bitmapToYuv
  (JNIEnv *, jclass, jobject bitmapObj, jbyteArray des);

#ifdef __cplusplus
}
#endif
#endif