#undef inline
#define inline __inline

//#include <stdio.h>
#include "org_modsecurity_ModSecurity.h"

#include "api.h"


#define MODSECURITY_JAVACLASS	"org/modsecurity/ModSecurity"

#define MODSECURITY_LOGMETHOD	"log"
#define MODSECURITY_LOGSIG		"(ILjava/lang/String;)V"

JavaVM *jvm;
jobject modSecurityInstance;
jclass modSecurityClass;
//
void logSec(void *obj, int level, char *str)
{
	JNIEnv *env;
	jstring jStr;
	jmethodID logMethod;
    (*jvm)->AttachCurrentThread(jvm, (void **)&env, NULL);

	jStr = (*env)->NewStringUTF(env, str);
	
	logMethod = (*env)->GetMethodID(env, modSecurityClass, MODSECURITY_LOGMETHOD, MODSECURITY_LOGSIG);
	(*env)->CallVoidMethod(env, modSecurityInstance, logMethod, level, jStr);
	
	(*env)->ReleaseStringUTFChars(env, jStr, str);

	(*jvm)->DetachCurrentThread(jvm);
}


apr_status_t ReadBodyCallback(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos)
{
	return APR_SUCCESS;
}

apr_status_t WriteBodyCallback(request_rec *r, char *buf, unsigned int length)
{
	return APR_SUCCESS;
}

apr_status_t ReadResponseCallback(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos)
{
	return APR_SUCCESS;
}

apr_status_t WriteResponseCallback(request_rec *r, char *buf, unsigned int length)
{
	return APR_SUCCESS;
}

JNIEXPORT jint JNICALL Java_org_modsecurity_ModSecurity_initialize(JNIEnv *env, jobject obj)
{
	jclass localCls;
	(*env)->GetJavaVM(env, &jvm);
	modSecurityInstance = (*env)->NewGlobalRef(env, obj);
	localCls = (*env)->FindClass(env, MODSECURITY_JAVACLASS);
	modSecurityClass = (jclass) (*env)->NewGlobalRef(env, localCls);
	(*env)->DeleteLocalRef(env, localCls);

	modsecSetLogHook(NULL, logSec);
	
	modsecSetReadBody(ReadBodyCallback);
	modsecSetReadResponse(ReadResponseCallback);
	modsecSetWriteBody(WriteBodyCallback);
	modsecSetWriteResponse(WriteResponseCallback);

	return APR_SUCCESS;
}

JNIEXPORT jint JNICALL Java_org_modsecurity_ModSecurity_destroy(JNIEnv *env, jobject obj)
{
	(*env)->DeleteGlobalRef(env, modSecurityInstance);
	(*env)->DeleteGlobalRef(env, modSecurityClass);
	return APR_SUCCESS;
}
