#include <com_tianxunnet_fmt_Fmt.h>
#include "protocol/protocol.h"

// by Liigo, 2013-12.

static jclass get_fmt_class(JNIEnv *env) {
// in Android VM:
// JNI ERROR (app bug): accessed stale local reference ...
// JNI WARNING: jclass is an invalid local reference ...
//
// http://android-developers.blogspot.com.ar/2011/11/jni-local-reference-changes-in-ics.html
//
//	static jclass cls = 0;
//	if(cls == 0) {
//		cls = (*env)->FindClass(env, "com/tianxunnet/fmt/Fmt");
//	}
	jclass cls = (*env)->FindClass(env, "com/tianxunnet/fmt/Fmt");
	//TODO: use NewGlobalRef() to catch cls
	return cls;
}

static jfieldID get_fmt_ptr_field_id(JNIEnv *env) {
//	static jfieldID fid = 0;
//	if(fid == 0) {
//		fid = (*env)->GetFieldID(env, get_fmt_class(env), "fmtPtr", "J");
//	}
	jfieldID fid = (*env)->GetFieldID(env, get_fmt_class(env), "fmtPtr", "J");
	//TODO: use NewGlobalRef() to catch fid
	return fid;
}

static FMT* get_fmt_ptr(JNIEnv *env, jobject fmt) {
	jlong ptr = (*env)->GetLongField(env, fmt, get_fmt_ptr_field_id(env));
	return (FMT*) ptr;
}

static void set_fmt_ptr(JNIEnv *env, jobject fmt, FMT* ptr) {
	(*env)->SetLongField(env, fmt, get_fmt_ptr_field_id(env), (jlong)ptr);
}

jobject new_fmt(JNIEnv *env, FMT* fmtPtr) {
	if(fmtPtr == 0)
		return 0;
	fmt_object_get(fmtPtr);
	// new Fmt(fmtPtr)
	jmethodID ctor = (*env)->GetMethodID(env, get_fmt_class(env), "<init>", "(J)V");
	if(ctor) {
		jobject fmt = (*env)->NewObject(env, get_fmt_class(env), ctor, (jlong)fmtPtr);
		return fmt;
	}
	fmt_object_put(fmtPtr);
	return 0;
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    newObjectFmt
 * Signature: ()Lcom/tianxunnet/fmt/Fmt;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_Fmt_newObjectFmt
  (JNIEnv *env, jclass fmt) {
	return new_fmt(env, fmt_new_object());
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    newArrayFmt
 * Signature: ()Lcom/tianxunnet/fmt/Fmt;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_Fmt_newArrayFmt
  (JNIEnv *env, jclass fmt) {
	return new_fmt(env, fmt_new_array());
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    newByteFmt
 * Signature: (B)Lcom/tianxunnet/fmt/Fmt;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_Fmt_newByteFmt
  (JNIEnv *env, jclass fmt, jbyte v) {
	return new_fmt(env, fmt_new_byte((unsigned char)v));
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    newShortFmt
 * Signature: (S)Lcom/tianxunnet/fmt/Fmt;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_Fmt_newShortFmt
  (JNIEnv *env, jclass fmt, jshort v) {
	return new_fmt(env, fmt_new_short((short)v));
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    newIntFmt
 * Signature: (I)Lcom/tianxunnet/fmt/Fmt;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_Fmt_newIntFmt
  (JNIEnv *env, jclass fmt, jint v) {
	return new_fmt(env, fmt_new_integer((int)v));
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    newLongFmt
 * Signature: (J)Lcom/tianxunnet/fmt/Fmt;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_Fmt_newLongFmt
  (JNIEnv *env, jclass fmt, jlong v) {
	return new_fmt(env, fmt_new_long((__int64)v));
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    newFloatFmt
 * Signature: (F)Lcom/tianxunnet/fmt/Fmt;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_Fmt_newFloatFmt
  (JNIEnv *env, jclass fmt, jfloat v) {
	return new_fmt(env, fmt_new_double((double)v)); // no double version currently
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    newDoubleFmt
 * Signature: (D)Lcom/tianxunnet/fmt/Fmt;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_Fmt_newDoubleFmt
  (JNIEnv *env, jclass fmt, jdouble v) {
	return new_fmt(env, fmt_new_double((double)v));
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    newDatetime
 * Signature: (J)Lcom/tianxunnet/fmt/Fmt;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_Fmt_newDatetime
  (JNIEnv *env, jclass fmt, jlong v) {
	return new_fmt(env, fmt_new_datetime((__int64)v / 1000)); // milliseconds to seconds
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    newBinaryFmt
 * Signature: ([B)Lcom/tianxunnet/fmt/Fmt;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_Fmt_newBinaryFmt
  (JNIEnv *env, jclass fmt, jbyteArray ary) {
	jint   len  = (*env)->GetArrayLength(env, ary);
	jbyte* data = (*env)->GetByteArrayElements(env, ary, NULL);
	FMT* fmtPtr = fmt_new_string((const unsigned char*)data, (unsigned int)len);
	(*env)->ReleaseByteArrayElements(env, ary, data, JNI_ABORT);
	return new_fmt(env, fmtPtr);
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    addField
 * Signature: (Ljava/lang/String;Lcom/tianxunnet/fmt/Fmt;)V
 */
JNIEXPORT void JNICALL Java_com_tianxunnet_fmt_Fmt_addField
  (JNIEnv *env, jobject fmt, jstring name, jobject fieldFmt) {
	FMT* fmtPtr = get_fmt_ptr(env, fmt);
	FMT* fieldFmtPtr = get_fmt_ptr(env, fieldFmt);
	const jbyte* utf8 = (*env)->GetStringUTFChars(env, name, NULL);
	fmt_object_add(fmtPtr, (const char*)utf8, fieldFmtPtr);
	(*env)->ReleaseStringUTFChars(env, name, utf8);
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    delField
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_tianxunnet_fmt_Fmt_delField
  (JNIEnv *env, jobject fmt, jstring name) {
	FMT* fmtPtr = get_fmt_ptr(env, fmt);
	const jbyte* utf8 = (*env)->GetStringUTFChars(env, name, NULL);
	fmt_object_remove(fmtPtr, (const char*)utf8);
	(*env)->ReleaseStringUTFChars(env, name, utf8);
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    getField
 * Signature: (Ljava/lang/String;)Lcom/tianxunnet/fmt/Fmt;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_Fmt_getField
  (JNIEnv *env, jobject fmt, jstring name) {
	FMT* fmtPtr = get_fmt_ptr(env, fmt);
	const jbyte* utf8 = (*env)->GetStringUTFChars(env, name, NULL);
	FMT* newFmtPtr = fmt_object_lookup(fmtPtr, (const char*)utf8);
	(*env)->ReleaseStringUTFChars(env, name, utf8);
	return new_fmt(env, newFmtPtr);
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    getFieldCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_tianxunnet_fmt_Fmt_getFieldCount
  (JNIEnv *env, jobject fmt) {
	return (jint) fmt_object_total(get_fmt_ptr(env, fmt));
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    arrayLength
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_tianxunnet_fmt_Fmt_arrayLength
  (JNIEnv *env, jobject fmt) {
	return (jint) fmt_array_length(get_fmt_ptr(env, fmt));
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    arrayAppend
 * Signature: (Lcom/tianxunnet/fmt/Fmt;)V
 */
JNIEXPORT void JNICALL Java_com_tianxunnet_fmt_Fmt_arrayAppend
  (JNIEnv *env, jobject fmt, jobject elementFmt) {
	FMT* fmtPtr = get_fmt_ptr(env, fmt);
	FMT* elementFmtPtr = get_fmt_ptr(env, elementFmt);
	fmt_array_append(fmtPtr, elementFmtPtr);
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    arrayRemove
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_tianxunnet_fmt_Fmt_arrayRemove
  (JNIEnv *env, jobject fmt, jint index) {
	fmt_array_remove(get_fmt_ptr(env, fmt), (unsigned int)index);
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    arrayAt
 * Signature: (I)Lcom/tianxunnet/fmt/Fmt;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_Fmt_arrayAt
  (JNIEnv *env, jobject fmt, jint index) {
	FMT* elementPtr = fmt_array_get_idx(get_fmt_ptr(env, fmt), (unsigned int)index);
	return new_fmt(env, elementPtr);
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    incRef
 * Signature: ()Lcom/tianxunnet/fmt/Fmt;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_Fmt_incRef
  (JNIEnv *env, jobject fmt) {
	fmt_object_get(get_fmt_ptr(env, fmt));
	return fmt;
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    decRef
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_tianxunnet_fmt_Fmt_decRef
  (JNIEnv *env, jobject fmt) {
	fmt_object_put(get_fmt_ptr(env, fmt));
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    getType
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_tianxunnet_fmt_Fmt_getType
  (JNIEnv *env, jobject fmt) {
	FMT* fmtPtr = get_fmt_ptr(env, fmt);
	return (jint) (fmtPtr ? fmtPtr->t : 0);
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    getByte
 * Signature: ()B
 */
JNIEXPORT jbyte JNICALL Java_com_tianxunnet_fmt_Fmt_getByte
  (JNIEnv *env, jobject fmt) {
	FMT* fmtPtr = get_fmt_ptr(env, fmt);
	return (jbyte) fmtPtr->m_byte;
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    getShort
 * Signature: ()S
 */
JNIEXPORT jshort JNICALL Java_com_tianxunnet_fmt_Fmt_getShort
  (JNIEnv *env, jobject fmt) {
	FMT* fmtPtr = get_fmt_ptr(env, fmt);
	return (jshort) fmtPtr->m_short;
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    getInt
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_tianxunnet_fmt_Fmt_getInt
  (JNIEnv *env, jobject fmt) {
	FMT* fmtPtr = get_fmt_ptr(env, fmt);
	return (jint) fmtPtr->m_int;
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    getLong
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_tianxunnet_fmt_Fmt_getLong
  (JNIEnv *env, jobject fmt) {
	FMT* fmtPtr = get_fmt_ptr(env, fmt);
	return (jlong) fmtPtr->m_long;
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    getFloat
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL Java_com_tianxunnet_fmt_Fmt_getFloat
  (JNIEnv *env, jobject fmt) {
	FMT* fmtPtr = get_fmt_ptr(env, fmt);
	return (jfloat) fmtPtr->m_double; // no float value currently
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    getDouble
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_com_tianxunnet_fmt_Fmt_getDouble
  (JNIEnv *env, jobject fmt) {
	FMT* fmtPtr = get_fmt_ptr(env, fmt);
	return (jdouble) fmtPtr->m_double;
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    getDatetime
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_tianxunnet_fmt_Fmt_getDatetime
  (JNIEnv *env, jobject fmt) {
	FMT* fmtPtr = get_fmt_ptr(env, fmt);
	return (jlong) fmtPtr->m_date; // seconds after UTC 1970/1/1 00:00:00
}

static jbyteArray create_jbyte_array(JNIEnv *env, const void* data, unsigned int len) {
	jbyteArray ary = (*env)->NewByteArray(env, (jint)len);
	(*env)->SetByteArrayRegion(env, ary, 0, (jint)len, (jbyte*)data);
	return ary;
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    getBinary
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_tianxunnet_fmt_Fmt_getBinary
  (JNIEnv *env, jobject fmt) {
	FMT* fmtPtr = get_fmt_ptr(env, fmt);
	create_jbyte_array(env, fmtPtr->m_str, fmtPtr->m_slen);
}

/**
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    getBinaryLength
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_tianxunnet_fmt_Fmt_getBinaryLength
 (JNIEnv *env, jobject fmt) {
	FMT* fmtPtr = get_fmt_ptr(env, fmt);
	return (jint) fmtPtr->m_slen;
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    packet
 * Signature: (II)[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_tianxunnet_fmt_Fmt_packet
  (JNIEnv *env, jobject fmt, jint cmd, jint userid) {
	automem_t mem;
	automem_init(&mem, 128);
	fmt_packet_build(&mem, get_fmt_ptr(env, fmt), (short)cmd, (unsigned int)userid);
	jbyteArray ary = create_jbyte_array(env, mem.pdata, mem.size);
	automem_uninit(&mem);
	return ary;
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    serverPacket
 * Signature: (I)[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_tianxunnet_fmt_Fmt_serverPacket
  (JNIEnv *env, jobject fmt, jint cmd) {
	automem_t mem;
	automem_init(&mem, 128);
	fmt_packet_build_server(&mem, get_fmt_ptr(env, fmt), (short)cmd);
	jbyteArray ary = create_jbyte_array(env, mem.pdata, mem.size);
	automem_uninit(&mem);
	return ary;
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    quickSerialize
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_tianxunnet_fmt_Fmt_quickSerialize
  (JNIEnv *env, jobject fmt) {
	automem_t mem;
	automem_init(&mem, 128);
	fmt_quick_serial(&mem, get_fmt_ptr(env, fmt));
	jbyteArray ary = create_jbyte_array(env, mem.pdata, mem.size);
	automem_uninit(&mem);
	return ary;
}

/*
 * Class:     com_tianxunnet_fmt_Fmt
 * Method:    quickParse
 * Signature: ([B)Lcom/tianxunnet/fmt/Fmt;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_Fmt_quickParse
  (JNIEnv *env, jobject fmt, jbyteArray ary) {
	jint   len  = (*env)->GetArrayLength(env, ary);
	jbyte* data = (*env)->GetByteArrayElements(env, ary, NULL);
	FMT* fmtPtr = fmt_quick_parse((char*)data, (int)len);
	(*env)->ReleaseByteArrayElements(env, ary, data, JNI_ABORT);
	return new_fmt(env, fmtPtr);
}
