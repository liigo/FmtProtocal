/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_tianxunnet_fmt_FmtParser */

#ifndef _Included_com_tianxunnet_fmt_FmtParser
#define _Included_com_tianxunnet_fmt_FmtParser
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_tianxunnet_fmt_FmtParser
 * Method:    newFmtParser
 * Signature: (ZZ)Lcom/tianxunnet/fmt/FmtParser;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_FmtParser_newFmtParser
  (JNIEnv *, jclass, jboolean, jboolean);

/*
 * Class:     com_tianxunnet_fmt_FmtParser
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_tianxunnet_fmt_FmtParser_close
  (JNIEnv *, jobject);

/*
 * Class:     com_tianxunnet_fmt_FmtParser
 * Method:    reset
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_tianxunnet_fmt_FmtParser_reset
  (JNIEnv *, jobject);

/*
 * Class:     com_tianxunnet_fmt_FmtParser
 * Method:    pushData
 * Signature: ([BLcom/tianxunnet/fmt/OnFmtParsed;)V
 */
JNIEXPORT void JNICALL Java_com_tianxunnet_fmt_FmtParser_pushData
  (JNIEnv *, jobject, jbyteArray, jobject);

#ifdef __cplusplus
}
#endif
#endif
