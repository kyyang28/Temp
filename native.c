
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>	// /usr/lib/jvm/java-1.8.0-openjdk-amd64/include/jni.h 

jintArray c_hello(JNIEnv *env, jobject cls, jintArray arr);

#if 0
typedef struct {
	char *name;			// function name in Java
	char *signature;	// JNI description to describe the the type and number of parameters
	void *fnPtr;		// local function pointer in C
} JNINativeMethod;
#endif

static const JNINativeMethod methods[] = {
	{ "hello", "([I)[I", (void *)c_hello },	// () means no parameter, V means return type void
};

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *jvm, void *reserved)
{
	JNIEnv *env;
	jclass cls;
	
	if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_4)) {
		return JNI_ERR; /* JNI version not supported */
	}
	
	cls = (*env)->FindClass(env, "JNIDemo");
	if (cls == NULL) {
		return JNI_ERR;
	}
	
	/* 2. Mapping java method hello() to c function c_hello() */
	if ((*env)->RegisterNatives(env, cls, methods, 1))	// 1 means the number of elements in methods array
		return JNI_ERR;
	
	return JNI_VERSION_1_4;
}

jintArray c_hello(JNIEnv *env, jobject cls, jintArray arr)
{
	jint *carr;
	jint *oarr;
	jintArray rarr;
	jint i, n = 0;
	
	carr = (*env)->GetIntArrayElements(env, arr, NULL);
	
	if (carr == NULL) {
		return 0; /* exception occurred */
	}

	/* jsize (JNICALL *GetArrayLength)(JNIEnv *env, jarray array) */
	n = (*env)->GetArrayLength(env, arr);
	oarr = malloc(sizeof(jint) * n);
	if (oarr == NULL) {
		(*env)->ReleaseIntArrayElements(env, arr, carr, 0);
		return 0;
	}

	for (i = 0; i < n; i++) {
		oarr[i] = carr[n-1-i];
	}
	
	(*env)->ReleaseIntArrayElements(env, arr, carr, 0);

	/* Create jintArray */
	/* jintArray (JNICALL *NewIntArray)(JNIEnv *env, jsize len) */
	rarr = (*env)->NewIntArray(env, n);
	if (rarr == NULL) {
		return 0;
	}

	/* Initialise jintArray rarr */
	/* 
	 *	Copies the contents of primitive arrays to a preallocated C buffer
	 *		- void (JNICALL *GetIntArrayRegion)(JNIEnv *env, jintArray array, jsize start, jsize len, jint *buf)
	 *
	 *	Copies the contents of primitive arrays from a preallocated C buffer
	 *		- void (JNICALL *SetIntArrayRegion)(JNIEnv *env, jintArray array, jsize start, jsize len, const jint *buf)
	 */
	(*env)->SetIntArrayRegion(env, rarr, 0, n, oarr);

	free(oarr);
	
	return rarr;
}

