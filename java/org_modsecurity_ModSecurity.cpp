#undef inline
#define inline __inline

#include "org_modsecurity_ModSecurity.h"
#include "api.h"
#include <math.h>

#define MODSECURITY_JAVACLASS				"org/modsecurity/ModSecurity"
#define SERVLETINPUTSTREAM_JAVACLASS		"javax/servlet/ServletInputStream"

#define TOSTRING_MET						"toString"
#define STRINGRETURN_SIG					"()Ljava/lang/String;"

#define INPUTSTREAM_READ_MET				"read"
#define INPUTSTREAM_READ_SIG				"([BII)I"

#define MODSECURITY_LOG_MET					"log"
#define MODSECURITY_LOG_SIG					"(ILjava/lang/String;)V"

#define MODSECURITY__HTTPHEADERS_MET		"getHttpHeaders"
#define MODSECURITY__HTTPHEADERS_SIG		"(Ljavax/servlet/http/HttpServletRequest;)[[Ljava/lang/String;"
//#define MODSECURITY__REQUESTBODY_MET		"getRequestBody"
//#define MODSECURITY__REQUESTBODY_SIG		"(Ljavax/servlet/http/HttpServletRequest;)Ljava/lang/String;"
#define MODSECURITY__ISPV6_MET				"isIPv6"
#define MODSECURITY__ISPV6_SIG				"(Ljava/lang/String;)Z"

#define SERVLETREQUEST_SERVERNAME_MET		"getServerName"
#define SERVLETREQUEST_CHARENCODING_MET		"getCharacterEncoding"
#define SERVLETREQUEST_CONTENTTYPE_MET		"getContentType"
#define SERVLETREQUEST_SERVERPORT_MET		"getServerPort"
#define SERVLETREQUEST_SERVERPORT_SIG		"()I"
#define SERVLETREQUEST_REMOTEADDR_MET		"getRemoteAddr"

#define SERVLETREQUEST_INPUTSTREAM_MET		"getInputStream"
#define SERVLETREQUEST_INPUTSTREAM_SIG		"()Ljavax/servlet/ServletInputStream;"

#define HTTPSERVLETREQUEST_PATHINFO_MET		"getPathInfo"
#define HTTPSERVLETREQUEST_QUERYSTRING_MET	"getQueryString"
#define HTTPSERVLETREQUEST_METHOD_MET		"getMethod"
#define HTTPSERVLETREQUEST_PROTOCOL_MET		"getProtocol"


#define HTTPSERVLETREQUEST_REQUESTURL_MET	"getRequestURL"
#define HTTPSERVLETREQUEST_REQUESTURL_SIG	"()Ljava/lang/StringBuffer;"



//typedef struct {
JavaVM *jvm;
jobject modSecurityInstance;
directory_config *config;
//} JavaModSecurityContext;
jmethodID logMethod;

#define JAVASERVLET_CONTEXT "JavaServletContext"

void storeJavaServletContext(request_rec *r, jobject obj)
{
	apr_table_setn(r->notes, JAVASERVLET_CONTEXT, (const char *)obj);
}

jobject getJavaServletContext(request_rec *r)
{
	jobject obj = NULL;
	request_rec *rx = NULL;

	/* Look in the current request first. */
	obj = (jobject)apr_table_get(r->notes, JAVASERVLET_CONTEXT);
	if (obj != NULL)
		return obj;

	/* If this is a subrequest then look in the main request. */
	if (r->main != NULL) 
	{
		obj = (jobject)apr_table_get(r->main->notes, JAVASERVLET_CONTEXT);
		if (obj != NULL) 
		{
			return obj;
		}
	}

	/* If the request was redirected then look in the previous requests. */
	rx = r->prev;
	while(rx != NULL) 
	{
		obj = (jobject)apr_table_get(rx->notes, JAVASERVLET_CONTEXT);
		if (obj != NULL) 
		{
			return obj;
		}
		rx = rx->prev;
	}

	return NULL;
}

apr_status_t memCleanup(void *mem)
{
	free(mem);
	return APR_SUCCESS;
}

apr_sockaddr_t *CopySockAddr(jclass msClass, JNIEnv *env, apr_pool_t *pool, char *addrstr, jstring addrStrJstr)
{
	jmethodID isIPv6Met = (env)->GetStaticMethodID(msClass, MODSECURITY__ISPV6_MET, MODSECURITY__ISPV6_SIG);
	jboolean isIPv6 = (env)->CallStaticBooleanMethod(msClass, isIPv6Met, addrStrJstr);

	apr_sockaddr_t *addr = (apr_sockaddr_t *)apr_palloc(pool, sizeof(apr_sockaddr_t));
	int adrlen = 16, iplen = 4;

	if(isIPv6)
	{
		adrlen = 46;
		iplen = 16;
		addr->family = AF_INET6;
	}
	else
		addr->family = AF_INET;

	addr->addr_str_len = adrlen;


	addr->hostname = "unknown";
#ifdef WIN32
	addr->ipaddr_len = sizeof(IN_ADDR);
#else
	addr->ipaddr_len = sizeof(struct in_addr);
#endif
	addr->ipaddr_ptr = &addr->sa.sin.sin_addr;
	addr->pool = pool;
	addr->port = 80;
#ifdef WIN32
	memcpy(&addr->sa.sin.sin_addr.S_un.S_addr, addrstr, iplen);
#else
	memcpy(&addr->sa.sin.sin_addr.s_addr, pAddr->sa_data, iplen);
#endif
	addr->sa.sin.sin_family = addr->family;
	addr->sa.sin.sin_port = 80;
	addr->salen = sizeof(addr->sa);
	addr->servname = addr->hostname;

	return addr;
}


char* fromJStringMethod(JNIEnv *env, jmethodID method, jobject obj, request_rec *r)
{
	char *str;
	jstring jStr = (jstring) (env)->CallObjectMethod(obj, method);
	if (jStr != NULL)
	{
		str = (char*) (env)->GetStringUTFChars(jStr, 0);
		(env)->ReleaseStringUTFChars(jStr, str);
	}
	else
		str = "";

	apr_pool_cleanup_register(r->pool, str, memCleanup, apr_pool_cleanup_null);

	return str;
}



void logSec(void *obj, int level, char *str)
{
	JNIEnv *env;
	jstring jStr;

	if (!(jvm)->AttachCurrentThread((void **)&env, NULL))
	{
		jStr = (env)->NewStringUTF(str);

		(env)->CallVoidMethod(modSecurityInstance, logMethod, level, jStr);

		(jvm)->DetachCurrentThread();
		//in the context of a JVM thread, any leaked local references are automatically cleaned up.
		//(env)->ReleaseStringUTFChars(jStr, str);
	}
}


apr_status_t ReadBodyCallback(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos)
{
	jobject inputStream = getJavaServletContext(r);
	JNIEnv *env;

	*readcnt = 0;

	if(inputStream == NULL)
	{
		*is_eos = 1;
		return APR_SUCCESS;
	}

	if (!(jvm)->AttachCurrentThread((void **)&env, NULL))
	{
		jclass inputStreamClass = env->FindClass(SERVLETINPUTSTREAM_JAVACLASS);
		jmethodID read = (env)->GetMethodID(inputStreamClass, INPUTSTREAM_READ_MET, INPUTSTREAM_READ_SIG);

		jbyteArray byteArrayBuf = (env)->NewByteArray(length);

		jint count = (env)->CallIntMethod(inputStream, read, byteArrayBuf, 0, length);

		if (count == -1 || count > length || env->ExceptionCheck() == JNI_TRUE) //end of stream
		{
			*is_eos = 1;
		}
		else
		{
			*readcnt = count;

			jbyte* bufferPtr = (env)->GetByteArrayElements(byteArrayBuf, NULL);
			
			memcpy(buf, bufferPtr, *readcnt);

			(env)->ReleaseByteArrayElements(byteArrayBuf, bufferPtr, NULL);
			(env)->DeleteLocalRef(byteArrayBuf);
		}

		(jvm)->DetachCurrentThread();
	}

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
	(env)->GetJavaVM(&jvm);
	modSecurityInstance = (env)->NewGlobalRef(obj);
	//modSecurityClass = env->GetObjectClass(obj);
	logMethod = (env)->GetMethodID(env->GetObjectClass(obj), MODSECURITY_LOG_MET, MODSECURITY_LOG_SIG);

	modsecSetLogHook(NULL, logSec);

	modsecSetReadBody(ReadBodyCallback);
	modsecSetReadResponse(ReadResponseCallback);
	modsecSetWriteBody(WriteBodyCallback);
	modsecSetWriteResponse(WriteResponseCallback);

	modsecInit();
	//char *compname = (char *)malloc(128);
	//s->server_hostname = compname;
	modsecStartConfig();
	config = modsecGetDefaultConfig();
	modsecFinalizeConfig();
	modsecInitProcess();
	config = NULL;

	return APR_SUCCESS;
}

JNIEXPORT jint JNICALL Java_org_modsecurity_ModSecurity_destroy(JNIEnv *env, jobject obj)
{
	(env)->DeleteGlobalRef(modSecurityInstance);
	//(env)->DeleteGlobalRef(modSecurityClass);

	return APR_SUCCESS;
}

//int getPort(request_rec *r)
//{
//	int port = 0;
//	char *port_str = NULL;
//
//	if(r->hostname != NULL)
//	{
//		int k = 0;
//		char *ptr = (char *)r->hostname;
//
//		while(*ptr != 0 && *ptr != ':')
//			ptr++;
//
//		if(*ptr == ':')
//		{
//			*ptr = 0;
//			port_str = ptr + 1;
//			port = atoi(port_str);
//		}
//	}
//	return port;
//}



JNIEXPORT jint JNICALL Java_org_modsecurity_ModSecurity_onRequest(JNIEnv *env, jobject obj, jstring configPath, jobject servletRequest, jobject httpServletRequest, jstring requestID, jboolean reloadConfig)
{
	conn_rec *c;
	request_rec *r;

	const char *path = (env)->GetStringUTFChars(configPath, NULL);
	const char *reqID = (env)->GetStringUTFChars(requestID, NULL);

	if (config == NULL || reloadConfig)
	{
		config = modsecGetDefaultConfig();
		const char *err = modsecProcessConfig(config, path, NULL);

		if(err != NULL)
		{
			logSec(NULL, 0, (char*)err);
			return DONE;
		}
	}

	c = modsecNewConnection();
	modsecProcessConnection(c);
	r = modsecNewRequest(c, config);


	jclass httpServletRequestClass = env->GetObjectClass(httpServletRequest);
	jclass servletRequestClass = env->GetObjectClass(servletRequest);
	jclass modSecurityClass = env->GetObjectClass(obj);


	jmethodID getInputStream = (env)->GetMethodID(servletRequestClass, SERVLETREQUEST_INPUTSTREAM_MET, SERVLETREQUEST_INPUTSTREAM_SIG);
	jobject inputStream = (env)->CallObjectMethod(servletRequest, getInputStream);
	//jobject gref = env->NewGlobalRef(inputStream);
	//apr_pool_cleanup_register(r->pool, gref, jDeleteGlobalRef, apr_pool_cleanup_null);

	storeJavaServletContext(r, inputStream);

	jmethodID getServerName = (env)->GetMethodID(servletRequestClass, SERVLETREQUEST_SERVERNAME_MET, STRINGRETURN_SIG);
	r->hostname = fromJStringMethod(env, getServerName, servletRequest, r);

	jmethodID getServerPort = (env)->GetMethodID(servletRequestClass, SERVLETREQUEST_SERVERPORT_MET, SERVLETREQUEST_SERVERPORT_SIG);
	int port = (env)->CallIntMethod(servletRequest, getServerPort);
	size_t len = (size_t) ceil(log10((float) abs(port)));
	char *port_str = (char*) apr_palloc(r->pool, len);
	itoa(port, port_str, 10);


	jmethodID getPathInfo = (env)->GetMethodID(httpServletRequestClass, HTTPSERVLETREQUEST_PATHINFO_MET, STRINGRETURN_SIG);
	r->path_info = fromJStringMethod(env, getPathInfo, httpServletRequest, r);


	jmethodID getQueryString = (env)->GetMethodID(httpServletRequestClass, HTTPSERVLETREQUEST_QUERYSTRING_MET, STRINGRETURN_SIG);
	r->args = fromJStringMethod(env, getQueryString, httpServletRequest, r);


	jmethodID getHttpHeaders = (env)->GetStaticMethodID(modSecurityClass, MODSECURITY__HTTPHEADERS_MET, MODSECURITY__HTTPHEADERS_SIG);
	jobjectArray headersTable = (jobjectArray) (env)->CallStaticObjectMethod(modSecurityClass, getHttpHeaders, httpServletRequest);
	jsize size = (env)->GetArrayLength(headersTable);

	for (int i = 0; i < size; i++) 
	{
		const char *headerName;
		const char *headerValue;
		jobjectArray row = (jobjectArray) (env)->GetObjectArrayElement(headersTable, i);
		jstring headerNameJStr = (jstring) (env)->GetObjectArrayElement(row, 0);
		jstring headerValueJStr = (jstring) (env)->GetObjectArrayElement(row, 1);

		if (headerNameJStr != NULL && headerValueJStr != NULL)
		{
			headerName = (env)->GetStringUTFChars(headerNameJStr, 0);
			apr_pool_cleanup_register(r->pool, headerName, memCleanup, apr_pool_cleanup_null);

			headerValue = (env)->GetStringUTFChars(headerValueJStr, 0);
			apr_pool_cleanup_register(r->pool, headerValue, memCleanup, apr_pool_cleanup_null);

			apr_table_setn(r->headers_in, headerName, headerValue);

			(env)->ReleaseStringUTFChars(headerNameJStr, headerName);
			(env)->ReleaseStringUTFChars(headerValueJStr, headerValue);
		}
	}


	jmethodID getCharacterEncoding = (env)->GetMethodID(servletRequestClass, SERVLETREQUEST_CHARENCODING_MET, STRINGRETURN_SIG);
	r->content_encoding = fromJStringMethod(env, getCharacterEncoding, servletRequest, r);


	jmethodID getContentType = (env)->GetMethodID(servletRequestClass, SERVLETREQUEST_CONTENTTYPE_MET, STRINGRETURN_SIG);
	r->content_type = fromJStringMethod(env, getContentType, servletRequest, r);


	const char *lng = apr_table_get(r->headers_in, "Content-Languages");
	if(lng != NULL)
	{
		r->content_languages = apr_array_make(r->pool, 1, sizeof(const char *));
		*(const char **)apr_array_push(r->content_languages) = lng;
	}

	jmethodID getMethod = (env)->GetMethodID(httpServletRequestClass, HTTPSERVLETREQUEST_METHOD_MET, STRINGRETURN_SIG);
	const char* method = fromJStringMethod(env, getMethod, httpServletRequest, r);

	//#define	SETMETHOD(m) if(strcmp(method,#m) == 0){ r->method = method; r->method_number = M_##m; }

	r->method = "INVALID";
	r->method_number = M_INVALID;

	//might be faster with elseif
	if (strcmp(method, "OPTIONS") == 0) { r->method = method; r->method_number = M_OPTIONS; }
	else if (strcmp(method, "GET") == 0) { r->method = method; r->method_number = M_GET; }
	else if (strcmp(method, "POST") == 0) { r->method = method; r->method_number = M_POST; }
	else if (strcmp(method, "PUT") == 0) { r->method = method; r->method_number = M_PUT; }
	else if (strcmp(method, "DELETE") == 0) { r->method = method; r->method_number = M_DELETE; }
	else if (strcmp(method, "TRACE") == 0) { r->method = method; r->method_number = M_TRACE; }
	else if (strcmp(method, "CONNECT") == 0) { r->method = method; r->method_number = M_CONNECT; }
	else if (strcmp(method, "MOVE") == 0) { r->method = method; r->method_number = M_MOVE; }
	else if (strcmp(method, "COPY") == 0) { r->method = method; r->method_number = M_COPY; }
	else if (strcmp(method, "PROPFIND") == 0) { r->method = method; r->method_number = M_PROPFIND; }
	else if (strcmp(method, "PROPPATCH") == 0) { r->method = method; r->method_number = M_PROPPATCH; }
	else if (strcmp(method, "MKCOL") == 0) { r->method = method; r->method_number = M_MKCOL; }
	else if (strcmp(method, "LOCK") == 0) { r->method = method; r->method_number = M_LOCK; }
	else if (strcmp(method, "UNLOCK") == 0) { r->method = method; r->method_number = M_UNLOCK; }

	jmethodID getProtocol = (env)->GetMethodID(httpServletRequestClass, HTTPSERVLETREQUEST_PROTOCOL_MET, STRINGRETURN_SIG);
	r->protocol = fromJStringMethod(env, getProtocol, httpServletRequest, r);

	r->request_time = apr_time_now();

	r->parsed_uri.scheme = "http";
	r->parsed_uri.path = r->path_info;
	r->parsed_uri.hostname = (char *)r->hostname;
	r->parsed_uri.is_initialized = 1;
	r->parsed_uri.port = port;
	r->parsed_uri.port_str = port_str;
	r->parsed_uri.query = r->args;
	r->parsed_uri.dns_looked_up = 0;
	r->parsed_uri.dns_resolved = 0;
	r->parsed_uri.password = NULL;
	r->parsed_uri.user = NULL;
	r->parsed_uri.fragment = NULL;

	jmethodID getRequestURL = (env)->GetMethodID(httpServletRequestClass, HTTPSERVLETREQUEST_REQUESTURL_MET, HTTPSERVLETREQUEST_REQUESTURL_SIG);
	jobject stringBuffer = (env)->CallObjectMethod(httpServletRequest, getRequestURL);
	if (stringBuffer != NULL)
	{
		jmethodID toStringBuff = (env)->GetMethodID((env)->GetObjectClass(stringBuffer), TOSTRING_MET, STRINGRETURN_SIG);
		r->unparsed_uri = fromJStringMethod(env, toStringBuff, stringBuffer, r);
		r->uri = r->unparsed_uri;
	}

	r->the_request = (char *)apr_palloc(r->pool, strlen(r->method) + 1 + strlen(r->uri) + 1 + strlen(r->protocol) + 1);

	strcpy(r->the_request, r->method);
	strcat(r->the_request, " ");
	strcat(r->the_request, r->uri);
	strcat(r->the_request, " ");
	strcat(r->the_request, r->protocol);

	apr_table_setn(r->subprocess_env, "UNIQUE_ID", reqID);


	jmethodID getRemoteAddr = (env)->GetMethodID(servletRequestClass, SERVLETREQUEST_REMOTEADDR_MET, STRINGRETURN_SIG);
	jstring remoteAddrJStr = (jstring) (env)->CallObjectMethod(servletRequest, getRemoteAddr);
	char *remoteAddr = (char*) (env)->GetStringUTFChars(remoteAddrJStr, 0);
	apr_pool_cleanup_register(r->pool, remoteAddr, memCleanup, apr_pool_cleanup_null);

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
	c->remote_addr = CopySockAddr(modSecurityClass, env, r->pool, remoteAddr, remoteAddrJStr);
	c->remote_ip = remoteAddr;
#else
	c->client_addr = CopySockAddr(modSecurityClass, env, r->pool, remoteAddr, remoteAddrJStr);
	c->client_ip = remoteAddr;
#endif
	c->remote_host = NULL;

	
	int status = modsecProcessRequest(r);

	(env)->ReleaseStringUTFChars(remoteAddrJStr, remoteAddr);
	(env)->ReleaseStringUTFChars(configPath, path);
	(env)->ReleaseStringUTFChars(requestID, reqID);
	(env)->DeleteLocalRef(inputStream);

	return status;
}