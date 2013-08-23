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

#define MODSECURITY__HTTPREQHEADERS_MET		"getHttpRequestHeaders"
#define MODSECURITY__HTTPREQHEADERS_SIG		"(Ljavax/servlet/http/HttpServletRequest;)[[Ljava/lang/String;"
#define MODSECURITY__HTTPRESHEADERS_MET		"getHttpResponseHeaders"
#define MODSECURITY__HTTPRESHEADERS_SIG		"(Ljavax/servlet/http/HttpServletResponse;)[[Ljava/lang/String;"
#define MODSECURITY__ISPV6_MET				"isIPv6"
#define MODSECURITY__ISPV6_SIG				"(Ljava/lang/String;)Z"

#define HTTPTRANSACTION_HTTPREQUEST_MET		"getHttpRequest"
#define HTTPTRANSACTION_HTTPREQUEST_SIG		"()Ljavax/servlet/http/HttpServletRequest;"
#define HTTPTRANSACTION_MSHTTPREQUEST_MET	"getMsHttpRequest"
#define HTTPTRANSACTION_MSHTTPREQUEST_SIG	"()Lorg/modsecurity/MsHttpServletRequest;"
#define HTTPTRANSACTION_MSHTTPRESPONSE_MET	"getMsHttpResponse"
#define HTTPTRANSACTION_MSHTTPRESPONSE_SIG	"()Lorg/modsecurity/MsHttpServletResponse;"

#define HTTPTRANSACTION_TRANSACTIONID_MET	"getTransactionID"

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

#define MSHTTPSERVLETREQUEST_READBODY_MET	"readBody"
#define MSHTTPSERVLETREQUEST_READBODY_SIG	"(I)V"
#define MSHTTPSERVLETREQUEST_SETBODY_MET	"setBodyBytes"
#define MSHTTPSERVLETREQUEST_SETBODY_SIG	"([B)V"

#define SERVLETRESPONSE_CONTENTTYPE_MET		"getContentType"
#define SERVLETRESPONSE_CHARENCODING_MET	"getCharacterEncoding"

#define MSSERVLETRESPONSE_OUTPUTSTREAM_MET	"getByteArrayStream"
#define MSSERVLETRESPONSE_OUTPUTSTREAM_SIG	"()Ljava/io/ByteArrayInputStream;"


//typedef struct {
JavaVM *jvm;
jobject modSecurityInstance;
directory_config *config;
//} JavaModSecurityContext;
jmethodID logMethod;


apr_table_t *requests;
apr_pool_t *requestsPool;
char *serverHostname;

#define JAVASERVLET_INSTREAM	"MSReqBStr"
#define JAVASERVLET_OUTSTREAM	"MSResBStr"
#define JAVASERVLET_TRANSACTION	"MSTran"

void storeJavaServletContext(request_rec *r, const char *key, jobject obj)
{
	apr_table_setn(r->notes, key, (const char *)obj);
}

void removeJavaServletContext(request_rec *r, const char *key)
{
	apr_table_unset(r->notes, key);
}

jobject getJavaServletContext(request_rec *r, const char *key)
{
	jobject obj = NULL;
	request_rec *rx = NULL;

	/* Look in the current request first. */
	obj = (jobject)apr_table_get(r->notes, key);
	if (obj != NULL)
		return obj;

	/* If this is a subrequest then look in the main request. */
	if (r->main != NULL) 
	{
		obj = (jobject)apr_table_get(r->main->notes, key);
		if (obj != NULL) 
		{
			return obj;
		}
	}

	/* If the request was redirected then look in the previous requests. */
	rx = r->prev;
	while(rx != NULL) 
	{
		obj = (jobject)apr_table_get(rx->notes, key);
		if (obj != NULL) 
		{
			return obj;
		}
		rx = rx->prev;
	}

	return NULL;
}


apr_sockaddr_t *CopySockAddr(jclass msClass, JNIEnv *env, apr_pool_t *pool, char *addrstr, jstring addrStrJstr)
{
	jmethodID isIPv6Met = (*env)->GetStaticMethodID(env, msClass, MODSECURITY__ISPV6_MET, MODSECURITY__ISPV6_SIG);
	jboolean isIPv6 = (*env)->CallStaticBooleanMethod(env, msClass, isIPv6Met, addrStrJstr);

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


    addr->hostname = (char*) "unknown";
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
    memcpy(&addr->sa.sin.sin_addr.s_addr, addrstr, iplen);
#endif
	addr->sa.sin.sin_family = addr->family;
	addr->sa.sin.sin_port = 80;
	addr->salen = sizeof(addr->sa);
	addr->servname = addr->hostname;

	return addr;
}

inline char* fromJString(JNIEnv *env, jstring jStr, apr_pool_t *pool)
{
	char *str;
	if (jStr != NULL)
	{
		const char *jCStr = (*env)->GetStringUTFChars(env, jStr, NULL);
		int len = strlen(jCStr);
		str = (char*) apr_palloc(pool, len + 1);
		memcpy(str, jCStr, len);
		str[len] = '\0'; //null terminate
		(*env)->ReleaseStringUTFChars(env, jStr, jCStr); //release java memory
	}
	else
        str = (char*) "";

	return str;
}

inline char* fromJStringMethod(JNIEnv *env, jmethodID method, jobject obj, apr_pool_t *pool)
{
	jstring jStr = (jstring) (*env)->CallObjectMethod(env, obj, method);

	return fromJString(env, jStr, pool);
}


void logSec(void *obj, int level, char *str)
{
	JNIEnv *env;
	jstring jStr;

	if (!(*jvm)->AttachCurrentThread(jvm, (void **)&env, NULL)) //get the Enviroment from the JavaVM
	{
		jStr = (*env)->NewStringUTF(env, str);

		(*env)->CallVoidMethod(env, modSecurityInstance, logMethod, level, jStr);

		(*jvm)->DetachCurrentThread(jvm);
		//in the context of a JVM thread, any leaked local references are automatically cleaned up
		//(*env)->ReleaseStringUTFChars(jStr, str);
	}
}


apr_status_t ReadBodyCallback(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos)
{
	JNIEnv *env;
	jclass inputStreamClass;
	jmethodID read;
	jbyteArray byteArrayBuf;
	jint count;
	jbyte *bufferPtr;
	jobject inputStream = getJavaServletContext(r, JAVASERVLET_INSTREAM); //servlet request input stream
	

	*readcnt = 0;

	if(inputStream == NULL)
	{
		*is_eos = 1;
		return APR_SUCCESS;
	}

	if (!(*jvm)->AttachCurrentThread(jvm, (void **)&env, NULL))
	{
		//read request body from the servlet input stream using 'read' method
		inputStreamClass = (*env)->FindClass(env, SERVLETINPUTSTREAM_JAVACLASS); 
		read = (*env)->GetMethodID(env, inputStreamClass, INPUTSTREAM_READ_MET, INPUTSTREAM_READ_SIG);

		byteArrayBuf = (*env)->NewByteArray(env, length);

		count = (*env)->CallIntMethod(env, inputStream, read, byteArrayBuf, 0, length);
		bufferPtr = (*env)->GetByteArrayElements(env, byteArrayBuf, NULL);

		if (count == -1 || count > length || (*env)->ExceptionCheck(env) == JNI_TRUE) //end of stream
		{
			*is_eos = 1;
		}
		else
		{
			*readcnt = count;

			memcpy(buf, bufferPtr, *readcnt);
		}
        (*env)->ReleaseByteArrayElements(env, byteArrayBuf, bufferPtr, 0);
		(*env)->DeleteLocalRef(env, byteArrayBuf);

		(*jvm)->DetachCurrentThread(jvm);
	}

	return APR_SUCCESS;
}

apr_status_t WriteBodyCallback(request_rec *r, char *buf, unsigned int length)
{
	jobject httpTransaction;
	JNIEnv *env;
	jclass httpTransactionClass;
	jmethodID getHttpRequest;
	jobject httpServletRequest;
	jclass httpServletRequestClass;
	jmethodID setBodyBytes;
	jbyte *jbuf;
	int i;
	jbyteArray byteArrayBuf;


	httpTransaction = getJavaServletContext(r, JAVASERVLET_TRANSACTION);

	if(httpTransaction == NULL)
	{
		return APR_SUCCESS;
	}

	if (!(*jvm)->AttachCurrentThread(jvm, (void **)&env, NULL))
	{
		httpTransactionClass = (*env)->GetObjectClass(env, httpTransaction);

		getHttpRequest = (*env)->GetMethodID(env, httpTransactionClass, HTTPTRANSACTION_MSHTTPREQUEST_MET, HTTPTRANSACTION_MSHTTPREQUEST_SIG);
		httpServletRequest = (*env)->CallObjectMethod(env, httpTransaction, getHttpRequest);

		httpServletRequestClass = (*env)->GetObjectClass(env, httpServletRequest);
		setBodyBytes = (*env)->GetMethodID(env, httpServletRequestClass, MSHTTPSERVLETREQUEST_SETBODY_MET, MSHTTPSERVLETREQUEST_SETBODY_SIG);

		jbuf = (jbyte*) apr_palloc(requestsPool, sizeof(jbyte) * length);
		for (i = 0; i < length; i++)
			jbuf[i] = buf[i];

		byteArrayBuf = (*env)->NewByteArray(env, length);
		(*env)->SetByteArrayRegion(env, byteArrayBuf, 0, length, jbuf);

		//on setBodyBytes we copy buf bytes
		(*env)->CallVoidMethod(env, httpServletRequest, setBodyBytes, byteArrayBuf);

		//(*env)->ReleaseByteArrayElements(byteArrayBuf, jbuf, NULL);

		(*jvm)->DetachCurrentThread(jvm);
	}
	return APR_SUCCESS;
}

apr_status_t ReadResponseCallback(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos)
{
	jobject inputStream;
	JNIEnv *env;
	jclass inputStreamClass;
	jmethodID read;
	jbyteArray byteArrayBuf;
	jint count;
	jbyte* bufferPtr;


	inputStream = getJavaServletContext(r, JAVASERVLET_OUTSTREAM);
	*readcnt = 0;

	if(inputStream == NULL)
	{
		*is_eos = 1;
		return APR_SUCCESS;
	}

	if (!(*jvm)->AttachCurrentThread(jvm, (void **)&env, NULL))
	{
		inputStreamClass = (*env)->GetObjectClass(env, inputStream);
		read = (*env)->GetMethodID(env, inputStreamClass, INPUTSTREAM_READ_MET, INPUTSTREAM_READ_SIG);

		byteArrayBuf = (*env)->NewByteArray(env, length);

		count = (*env)->CallIntMethod(env, inputStream, read, byteArrayBuf, 0, length);
		bufferPtr = (*env)->GetByteArrayElements(env, byteArrayBuf, NULL);

		if (count == -1 || count > length || (*env)->ExceptionCheck(env) == JNI_TRUE) //end of stream
		{
			*is_eos = 1;
		}
		else
		{
			*readcnt = count;

			memcpy(buf, bufferPtr, *readcnt);
		}
        (*env)->ReleaseByteArrayElements(env, byteArrayBuf, bufferPtr, 0);
		(*env)->DeleteLocalRef(env, byteArrayBuf);

		(*jvm)->DetachCurrentThread(jvm);
	}

	return APR_SUCCESS;
}

apr_status_t WriteResponseCallback(request_rec *r, char *buf, unsigned int length)
{
	JNIEnv *env;
	jbyteArray byteArrayBuf;
	jclass httpTransactionClass;
	jmethodID getHttpResponse;
	jobject httpServletResponse;
	jbyte *jbuf;
	int i;
	jmethodID setBodyBytes;
	jclass httpServletResponseClass;
	jobject httpTransaction = getJavaServletContext(r, JAVASERVLET_TRANSACTION);

	if(httpTransaction == NULL)
	{
		(*jvm)->DetachCurrentThread(jvm);
		return APR_SUCCESS;
	}

	if (!(*jvm)->AttachCurrentThread(jvm, (void **)&env, NULL))
	{
		httpTransactionClass = (*env)->GetObjectClass(env, httpTransaction);

		getHttpResponse = (*env)->GetMethodID(env, httpTransactionClass, HTTPTRANSACTION_MSHTTPRESPONSE_MET, HTTPTRANSACTION_MSHTTPRESPONSE_SIG);
		httpServletResponse = (*env)->CallObjectMethod(env, httpTransaction, getHttpResponse);

		
		httpServletResponseClass = (*env)->GetObjectClass(env, httpServletResponse);
		setBodyBytes = (*env)->GetMethodID(env, httpServletResponseClass, MSHTTPSERVLETREQUEST_SETBODY_MET, MSHTTPSERVLETREQUEST_SETBODY_SIG);

		jbuf = (jbyte*) apr_palloc(requestsPool, sizeof(jbyte) * length);
		for (i = 0; i < length; i++)
			jbuf[i] = buf[i];

		byteArrayBuf = (*env)->NewByteArray(env, length);
		(*env)->SetByteArrayRegion(env, byteArrayBuf, 0, length, jbuf);

		//on setBodyBytes we copy buf bytes
		(*env)->CallVoidMethod(env, httpServletResponse, setBodyBytes, byteArrayBuf);

		//(*env)->ReleaseByteArrayElements(byteArrayBuf, jbuf, NULL);

		(*jvm)->DetachCurrentThread(jvm);
	}
	return APR_SUCCESS;
}

JNIEXPORT jint JNICALL Java_org_modsecurity_ModSecurity_initialize(JNIEnv *env, jobject obj, jstring serverName)
{
	(*env)->GetJavaVM(env, &jvm);
	modSecurityInstance = (*env)->NewGlobalRef(env, obj); //Save the ModSecurity object for further use

	logMethod = (*env)->GetMethodID(env, (*env)->GetObjectClass(env, obj), MODSECURITY_LOG_MET, MODSECURITY_LOG_SIG); //log method ID

	modsecSetLogHook(NULL, logSec);

	modsecSetReadBody(ReadBodyCallback);
	modsecSetReadResponse(ReadResponseCallback);
	modsecSetWriteBody(WriteBodyCallback);
	modsecSetWriteResponse(WriteResponseCallback);

	modsecInit();

	modsecStartConfig();
	config = modsecGetDefaultConfig();
	modsecFinalizeConfig();
	modsecInitProcess();
	config = NULL;

	//table for requests
	apr_pool_create(&requestsPool, NULL);
	requests = apr_table_make(requestsPool, 10);

	serverHostname = fromJString(env, serverName, requestsPool);

	return APR_SUCCESS;
}

JNIEXPORT jint JNICALL Java_org_modsecurity_ModSecurity_destroy(JNIEnv *env, jobject obj)
{
	(*env)->DeleteGlobalRef(env, modSecurityInstance);

	apr_pool_destroy(requestsPool);

	modsecTerminate();

	return APR_SUCCESS;
}


inline void setHeaders(JNIEnv *env, jclass httpServletRequestClass, jobject httpServletR, apr_table_t *reqHeaders, apr_pool_t *pool, const char *headersMet, const char *headersSig)
{
	jmethodID getHttpHeaders;
	jobjectArray headersTable;
	jsize size;
	int i;

	//All headers are returned in a table by a static method from ModSecurity class
	getHttpHeaders = (*env)->GetStaticMethodID(env, httpServletRequestClass, headersMet, headersSig);
	headersTable = (jobjectArray) (*env)->CallStaticObjectMethod(env, httpServletRequestClass, getHttpHeaders, httpServletR);
	size = (*env)->GetArrayLength(env, headersTable);

	for (i = 0; i < size; i++) 
	{
		const char *headerName;
		const char *headerValue;
		jobjectArray row = (jobjectArray) (*env)->GetObjectArrayElement(env, headersTable, i);
		jstring headerNameJStr = (jstring) (*env)->GetObjectArrayElement(env, row, 0);
		jstring headerValueJStr = (jstring) (*env)->GetObjectArrayElement(env, row, 1);

		if (headerNameJStr != NULL && headerValueJStr != NULL)
		{
			headerName = fromJString(env, headerNameJStr, pool);
			//apr_pool_cleanup_register(r->pool, headerName, memCleanup, apr_pool_cleanup_null);

			headerValue = fromJString(env, headerValueJStr, pool);
			//apr_pool_cleanup_register(r->pool, headerValue, memCleanup, apr_pool_cleanup_null);

			apr_table_setn(reqHeaders, headerName, headerValue);

			(*env)->DeleteLocalRef(env, headerNameJStr);
			(*env)->DeleteLocalRef(env, headerValueJStr);
		}
	}
}

JNIEXPORT jint JNICALL Java_org_modsecurity_ModSecurity_onRequest(JNIEnv *env, jobject obj, jstring configPath, jobject httpTransaction, jboolean reloadConfig)
{
	conn_rec *c;
	request_rec *r;
	const char *err;
	jmethodID getHttpRequest;
	jclass httpTransactionClass;
	jobject httpServletRequest;
	jobject servletRequest;
	jclass httpServletRequestClass;
	jclass servletRequestClass;
	jclass modSecurityClass;
	jmethodID readBody;
	jmethodID getTransactionID;
	jmethodID getInputStream;
	const char *reqID;
	jobject inputStream;
	jmethodID getServerName;
	jmethodID getServerPort;
	int port;
	char *port_str;
	jmethodID getPathInfo;
	jmethodID getQueryString;
	jmethodID getCharacterEncoding;
	jmethodID getContentType;
	const char *lng;
	jmethodID getMethod;
	const char* method;
	jmethodID getProtocol;
	jmethodID getRequestURL;
	jobject stringBuffer;
	jmethodID toStringBuff;
	char *url;
	jmethodID getRemoteAddr;
	jstring remoteAddrJStr;
	char *remoteAddr;
	int status;
	int len;

	if (config == NULL || reloadConfig)
	{
		const char *path;
		config = modsecGetDefaultConfig();
		path = fromJString(env, configPath, config->mp); //path to modsecurity.conf

		err = modsecProcessConfig(config, path, NULL);

		if(err != NULL)
		{
			logSec(NULL, 0, (char*)err);

			return DONE;
		}
	}

	c = modsecNewConnection();
	modsecProcessConnection(c);
	r = modsecNewRequest(c, config);

	r->server->server_hostname = serverHostname;
	httpTransactionClass = (*env)->GetObjectClass(env, httpTransaction);
	getHttpRequest = (*env)->GetMethodID(env, httpTransactionClass, HTTPTRANSACTION_MSHTTPREQUEST_MET, HTTPTRANSACTION_MSHTTPREQUEST_SIG);

	httpServletRequest = (*env)->CallObjectMethod(env, httpTransaction, getHttpRequest);
	servletRequest = httpServletRequest; //superclass of HttpServletRequest

	httpServletRequestClass = (*env)->GetObjectClass(env, httpServletRequest); //MsHttpServletRequest interface
	servletRequestClass = (*env)->GetObjectClass(env, servletRequest); //ServletRequest interface
	modSecurityClass = (*env)->GetObjectClass(env, obj); //ModSecurity class

	//readBody method reads all bytes from the inputStream or a maximum of 'config->reqbody_limit' bytes
	readBody = (*env)->GetMethodID(env, httpServletRequestClass, MSHTTPSERVLETREQUEST_READBODY_MET, MSHTTPSERVLETREQUEST_READBODY_SIG);
	(*env)->CallVoidMethod(env, httpServletRequest, readBody, config->reqbody_limit);

	if ((*env)->ExceptionCheck(env) == JNI_TRUE) //read body raised an Exception, return to JVM
	{
		modsecFinishRequest(r);
		return DONE;
	}


	getTransactionID = (*env)->GetMethodID(env, httpTransactionClass, HTTPTRANSACTION_TRANSACTIONID_MET, STRINGRETURN_SIG);
	reqID = fromJStringMethod(env, getTransactionID, httpTransaction, r->pool); //fromJString(env, requestID, r->pool); //unique ID of this request
	apr_table_setn(requests, reqID, (const char*) r); //store this request for response processing


	getInputStream = (*env)->GetMethodID(env, httpServletRequestClass, SERVLETREQUEST_INPUTSTREAM_MET, SERVLETREQUEST_INPUTSTREAM_SIG);
	inputStream = (*env)->CallObjectMethod(env, httpServletRequest, getInputStream); //Request body input stream used in the read body callback

	storeJavaServletContext(r, JAVASERVLET_INSTREAM, inputStream);
	storeJavaServletContext(r, JAVASERVLET_TRANSACTION, httpTransaction);


	getServerName = (*env)->GetMethodID(env, servletRequestClass, SERVLETREQUEST_SERVERNAME_MET, STRINGRETURN_SIG);
	r->hostname = fromJStringMethod(env, getServerName, servletRequest, r->pool);

	getServerPort = (*env)->GetMethodID(env, servletRequestClass, SERVLETREQUEST_SERVERPORT_MET, SERVLETREQUEST_SERVERPORT_SIG);
	port = (*env)->CallIntMethod(env, servletRequest, getServerPort); //server port
	port_str = apr_itoa(r->pool, port);


	getPathInfo = (*env)->GetMethodID(env, httpServletRequestClass, HTTPSERVLETREQUEST_PATHINFO_MET, STRINGRETURN_SIG);
	r->path_info = fromJStringMethod(env, getPathInfo, httpServletRequest, r->pool);


	getQueryString = (*env)->GetMethodID(env, httpServletRequestClass, HTTPSERVLETREQUEST_QUERYSTRING_MET, STRINGRETURN_SIG);
	r->args = fromJStringMethod(env, getQueryString, httpServletRequest, r->pool);


	setHeaders(env, httpServletRequestClass, httpServletRequest, r->headers_in, r->pool, MODSECURITY__HTTPREQHEADERS_MET, MODSECURITY__HTTPREQHEADERS_SIG);


	getCharacterEncoding = (*env)->GetMethodID(env, servletRequestClass, SERVLETREQUEST_CHARENCODING_MET, STRINGRETURN_SIG);
	r->content_encoding = fromJStringMethod(env, getCharacterEncoding, servletRequest, r->pool);


	getContentType = (*env)->GetMethodID(env, servletRequestClass, SERVLETREQUEST_CONTENTTYPE_MET, STRINGRETURN_SIG);
	r->content_type = fromJStringMethod(env, getContentType, servletRequest, r->pool);


	lng = apr_table_get(r->headers_in, "Content-Languages");
	if(lng != NULL)
	{
		r->content_languages = apr_array_make(r->pool, 1, sizeof(const char *));
		*(const char **)apr_array_push(r->content_languages) = lng;
	}

	getMethod = (*env)->GetMethodID(env, httpServletRequestClass, HTTPSERVLETREQUEST_METHOD_MET, STRINGRETURN_SIG);
	method = fromJStringMethod(env, getMethod, httpServletRequest, r->pool);

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

	getProtocol = (*env)->GetMethodID(env, httpServletRequestClass, HTTPSERVLETREQUEST_PROTOCOL_MET, STRINGRETURN_SIG);
	r->protocol = fromJStringMethod(env, getProtocol, httpServletRequest, r->pool);

	r->request_time = apr_time_now();

    r->parsed_uri.scheme = (char*) "http";
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


	//the request Url is in a StringBuffer object
	getRequestURL = (*env)->GetMethodID(env, httpServletRequestClass, HTTPSERVLETREQUEST_REQUESTURL_MET, HTTPSERVLETREQUEST_REQUESTURL_SIG);
	stringBuffer = (*env)->CallObjectMethod(env, httpServletRequest, getRequestURL);
	if (stringBuffer != NULL)
	{
		toStringBuff = (*env)->GetMethodID(env, (*env)->GetObjectClass(env, stringBuffer), TOSTRING_MET, STRINGRETURN_SIG);
		url = fromJStringMethod(env, toStringBuff, stringBuffer, r->pool);

		if (strcmp(r->args, "") != 0)
		{
			len = strlen(url) + 1 + strlen(r->args) + 1;
			r->unparsed_uri = (char*)apr_palloc(r->pool, len); //unparsed uri with full query
			strcpy(r->unparsed_uri, url);
			strcat(r->unparsed_uri, "?");
			strcat(r->unparsed_uri, r->args);
			r->unparsed_uri[len] = 0;
		}
		else
		{
			r->unparsed_uri = url;
		}
		r->uri = r->unparsed_uri;
	}

	len = strlen(r->method) + 1 + strlen(r->uri) + 1 + strlen(r->protocol) + 1;
	r->the_request = (char *)apr_palloc(r->pool, len);

	strcpy(r->the_request, r->method);
	strcat(r->the_request, " ");
	strcat(r->the_request, r->uri);
	strcat(r->the_request, " ");
	strcat(r->the_request, r->protocol);
	r->the_request[len] = 0;

	apr_table_setn(r->subprocess_env, "UNIQUE_ID", reqID);

	getRemoteAddr = (*env)->GetMethodID(env, servletRequestClass, SERVLETREQUEST_REMOTEADDR_MET, STRINGRETURN_SIG);
	remoteAddrJStr = (jstring) (*env)->CallObjectMethod(env, servletRequest, getRemoteAddr);
	remoteAddr = fromJString(env, remoteAddrJStr, r->pool);
	//apr_pool_cleanup_register(r->pool, remoteAddr, memCleanup, apr_pool_cleanup_null);

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
	c->remote_addr = CopySockAddr(modSecurityClass, env, r->pool, remoteAddr, remoteAddrJStr);
	c->remote_ip = remoteAddr;
#else
	c->client_addr = CopySockAddr(modSecurityClass, env, r->pool, remoteAddr, remoteAddrJStr);
	c->client_ip = remoteAddr;
#endif
	c->remote_host = NULL;

	status = modsecProcessRequest(r);

	removeJavaServletContext(r, JAVASERVLET_INSTREAM);
	removeJavaServletContext(r, JAVASERVLET_TRANSACTION);

	if (status != DECLINED) //Java modsecurityFilter blocks the request, onResponse will not be called, it's safe to finish the request
	{
		apr_table_unset(requests, reqID);
		modsecFinishRequest(r);
	}

	return status;
}



JNIEXPORT jint JNICALL Java_org_modsecurity_ModSecurity_onResponse(JNIEnv *env, jobject obj, jobject httpTransaction)
{
	jclass httpTransactionClass;
	jmethodID getTransactionID;
	jstring reqIDjStr;
	const char *reqID;
	request_rec *r;
	jmethodID getHttpResponse;
	jobject httpServletResponse;
	jclass httpServletResponseClass;
	jmethodID getOutputStream;
	jobject responseStream;
	jmethodID getContentType;
	jmethodID getCharEncoding;
	char *ct;
	const char *lng;
	int status;


	httpTransactionClass = (*env)->GetObjectClass(env, httpTransaction);

	getTransactionID = (*env)->GetMethodID(env, httpTransactionClass, HTTPTRANSACTION_TRANSACTIONID_MET, STRINGRETURN_SIG);
	reqIDjStr = (jstring) (*env)->CallObjectMethod(env, httpTransaction, getTransactionID);
	reqID = (*env)->GetStringUTFChars(env, reqIDjStr, NULL);

	r = (request_rec*) apr_table_get(requests, reqID);
	apr_table_unset(requests, reqID); //remove this request from the requests table
	(*env)->ReleaseStringUTFChars(env, reqIDjStr, reqID);

	if (r == NULL)
	{
		return DONE;
	}

	getHttpResponse = (*env)->GetMethodID(env, httpTransactionClass, HTTPTRANSACTION_MSHTTPRESPONSE_MET, HTTPTRANSACTION_MSHTTPRESPONSE_SIG);
	httpServletResponse = (*env)->CallObjectMethod(env, httpTransaction, getHttpResponse);

	httpServletResponseClass = (*env)->GetObjectClass(env, httpServletResponse); //MsHttpServletResponse class
	//jclass modSecurityClass = (*env)->GetObjectClass(obj); //ModSecurity class

	getOutputStream = (*env)->GetMethodID(env, httpServletResponseClass, MSSERVLETRESPONSE_OUTPUTSTREAM_MET, MSSERVLETRESPONSE_OUTPUTSTREAM_SIG);
	responseStream = (*env)->CallObjectMethod(env, httpServletResponse, getOutputStream); //Response output stream used in the read response callback

	if ((*env)->ExceptionCheck(env) == JNI_TRUE)
	{
		modsecFinishRequest(r);
		return DONE;
	}

	storeJavaServletContext(r, JAVASERVLET_TRANSACTION, httpTransaction);
	storeJavaServletContext(r, JAVASERVLET_OUTSTREAM, responseStream);

	getContentType = (*env)->GetMethodID(env, httpServletResponseClass, SERVLETRESPONSE_CONTENTTYPE_MET, STRINGRETURN_SIG);
	ct = fromJStringMethod(env, getContentType, httpServletResponse, r->pool);
	if(strcmp(ct, "") == 0)
        ct = (char*) "text/html";
	r->content_type = ct;


	getCharEncoding = (*env)->GetMethodID(env, httpServletResponseClass, SERVLETRESPONSE_CHARENCODING_MET, STRINGRETURN_SIG);
	r->content_encoding = fromJStringMethod(env, getCharEncoding, httpServletResponse, r->pool);


	setHeaders(env, httpServletResponseClass, httpServletResponse, r->headers_out, r->pool, MODSECURITY__HTTPRESHEADERS_MET, MODSECURITY__HTTPRESHEADERS_SIG);

	lng = apr_table_get(r->headers_out, "Content-Languages");
	if(lng != NULL)
	{
		r->content_languages = apr_array_make(r->pool, 1, sizeof(const char *));
		*(const char **)apr_array_push(r->content_languages) = lng;
	}


	status = modsecProcessResponse(r);

	removeJavaServletContext(r, JAVASERVLET_OUTSTREAM);
	removeJavaServletContext(r, JAVASERVLET_TRANSACTION);
	modsecFinishRequest(r);

	return status;
}
