#include "com_tianxunnet_fmt_FmtParser.h"
#include "protocol/buffmtparser.h"
#include <stdint.h>

// by Liigo, 2013-12.

static jclass get_parser_class(JNIEnv *env) {
// See get_fmt_class()
//	static jclass cls = 0;
//	if(cls == 0) {
//		cls = (*env)->FindClass(env, "com/tianxunnet/fmt/FmtParser");
//	}
	jclass cls = (*env)->FindClass(env, "com/tianxunnet/fmt/FmtParser");
	return cls;
}

static jfieldID get_parser_ptr_field_id(JNIEnv *env) {
//	static jfieldID fid = 0;
//	if(fid == 0) {
//		fid = (*env)->GetFieldID(env, get_parser_class(env), "parserPtr", "J");
//	}
	jfieldID fid = (*env)->GetFieldID(env, get_parser_class(env), "parserPtr", "J");
	return fid;
}

static void* get_parser_ptr(JNIEnv *env, jobject parser) {
	jlong ptr = (*env)->GetLongField(env, parser, get_parser_ptr_field_id(env));
	return (void*)(intptr_t) ptr;
}

static void set_parser_ptr(JNIEnv *env, jobject parser, jlong ptr) {
	(*env)->SetLongField(env, parser, get_parser_ptr_field_id(env), ptr);
}

/*
 * Class:     com_tianxunnet_fmt_FmtParser
 * Method:    newFmtParser
 * Signature: (ZZ)Lcom/tianxunnet/fmt/FmtParser;
 */
JNIEXPORT jobject JNICALL Java_com_tianxunnet_fmt_FmtParser_newFmtParser
  (JNIEnv *env, jclass cls, jboolean isServer, jboolean isFull) {
	// new FmtParser(parserPtr)
	jmethodID ctor = (*env)->GetMethodID(env, get_parser_class(env), "<init>", "(J)V");
	if(ctor) {
		void* parserPtr = buffered_fmt_parser_new(isServer==JNI_TRUE ? true : false);
		jobject parser = (*env)->NewObject(env, get_parser_class(env), ctor, (jlong)(intptr_t)parserPtr);
		return parser;
	}
	return 0;
}

/*
 * Class:     com_tianxunnet_fmt_FmtParser
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_tianxunnet_fmt_FmtParser_close
  (JNIEnv *env, jobject parser) {
	void* parserPtr = get_parser_ptr(env, parser);
	buffered_fmt_parser_delete(parserPtr);
	set_parser_ptr(env, parser, 0);
}

/*
 * Class:     com_tianxunnet_fmt_FmtParser
 * Method:    reset
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_tianxunnet_fmt_FmtParser_reset
  (JNIEnv *env, jobject parser) {
	buffered_fmt_parser_reset(get_parser_ptr(env, parser));
}


// defines in com_tianxunnet_fmt_Fmt.c
jobject new_fmt(JNIEnv *env, FMT* fmtPtr);

typedef struct {
	JNIEnv* env;
	jobject callback;
	void* parserPtr;
}CallbackUserdata;

static void fmtparser_callback(short int cmd, void* userdata) {
	JNIEnv *env      = ((CallbackUserdata*)userdata)->env;
	jobject callback = ((CallbackUserdata*)userdata)->callback;
	void* parserPtr  = ((CallbackUserdata*)userdata)->parserPtr;

	jclass clsOnFmtParsed = (*env)->FindClass(env, "com/tianxunnet/fmt/OnFmtParsed");
	if(callback && clsOnFmtParsed) {
		// invoke callback: OnFmtParsed.onFmtParsed()
		jmethodID onFmtParsed = (*env)->GetMethodID(env, clsOnFmtParsed, "onFmtParsed", "(Lcom/tianxunnet/fmt/Fmt;I)V");
		FMT* fmtPtr = buffered_fmt_parser_take(parserPtr);
		jobject fmt = new_fmt(env, fmtPtr);
		if(onFmtParsed) {
			(*env)->CallVoidMethod(env, callback, onFmtParsed, fmt, (jint)(unsigned short)cmd);
		}
		// Liigo:
		// buffered_fmt_parser_take() and new_fmt() call fmt_object_get() twice, so
		// here we call fmt_object_put() twice to make sure the fmtPtr be freed properly.
		// Leave fmt to java GC.
		if(fmtPtr) {
			fmt_object_put(fmtPtr);
			fmt_object_put(fmtPtr);
		}
	}
}

/*
 * Class:     com_tianxunnet_fmt_FmtParser
 * Method:    pushData
 * Signature: ([BLcom/tianxunnet/fmt/OnFmtParsed;)V
 */
JNIEXPORT void JNICALL Java_com_tianxunnet_fmt_FmtParser_pushData
  (JNIEnv *env, jobject parser, jbyteArray ary, jobject callback) {
	void* parserPtr = get_parser_ptr(env, parser);
	jint   len  = (*env)->GetArrayLength(env, ary);
	jbyte* data = (*env)->GetByteArrayElements(env, ary, NULL);
	CallbackUserdata userdata = { env, callback, parserPtr };
	buffered_fmt_parser_push(parserPtr, data, (unsigned int)len, fmtparser_callback, &userdata);
	(*env)->ReleaseByteArrayElements(env, ary, data, JNI_ABORT);
}
