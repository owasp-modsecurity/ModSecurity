/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2013 Trustwave Holdings, Inc. (http://www.trustwave.com/)
*
* You may not use this file except in compliance with
* the License.  You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* If any of the files related to licensing are missing or if you have any
* other questions related to licensing please contact Trustwave Holdings, Inc.
* directly using the email address security@modsecurity.org.
*/

#define WIN32_LEAN_AND_MEAN

#undef inline

#include <string>
#include <cctype>
#include <memory>
#include <future>

//  IIS7 Server API header file
#include <Windows.h>
#include <sal.h>
#include <strsafe.h>
#include "httpserv.h"

//  Project header files
#include "mymodule.h"
#include "mymodulefactory.h"

#include "api.h"
#include "moduleconfig.h"

#include "winsock2.h"

#include "asio/packaged_task.hpp"
#include "asio/post.hpp"

// These helpers make sure that the underlying structures
// are correctly released on any exit path due to RAII.
using ConnRecPtr = std::shared_ptr<conn_rec>;
using RequestRecPtr = std::shared_ptr<request_rec>;

ConnRecPtr MakeConnReq()
{
    ConnRecPtr c{modsecNewConnection(), modsecFinishConnection};
    modsecProcessConnection(c.get());
    return c;
}

RequestRecPtr MakeRequestRec(conn_rec* c, directory_config* config)
{
    return {modsecNewRequest(c, config), modsecFinishRequest};
}

struct RequestStoredContext
    : IHttpStoredContext
{
    ConnRecPtr connRec;
    RequestRecPtr requestRec;
    IHttpContext* httpContext;
    IHttpEventProvider* provider;
    char* responseBuffer = nullptr;
    ULONGLONG responseLength = 0;
    ULONGLONG responsePosition = 0;
    bool responseProcessingEnabled = false;
    std::future<int> detectionProcessing;

    void CleanupStoredContext() override
    {
        delete this;
    }

    void FinishRequest()
    {
        requestRec.reset();
        connRec.reset();
    }
};

char *GetIpAddr(apr_pool_t *pool, PSOCKADDR pAddr)
{
    const char *format = "%15[0-9.]:%5[0-9]";
    char ip[16] = { 0 };  // ip4 addresses have max len 15
    char port[6] = { 0 }; // port numbers are 16bit, ie 5 digits max

    DWORD len = 50;
    char *buf = (char *)apr_palloc(pool, len);

    if(buf == NULL)
        return "";

    buf[0] = 0;

    WSAAddressToString(pAddr, sizeof(SOCKADDR), NULL, buf, &len);

    // test for IPV4 with port on the end
    if (sscanf(buf, format, ip, port) == 2) {
        // IPV4 but with port - remove the port
        char* input = ":";
        char* ipv4 = strtok(buf, input);
        return ipv4;
    }

    return buf;
}

apr_sockaddr_t *CopySockAddr(apr_pool_t *pool, PSOCKADDR pAddr)
{
    apr_sockaddr_t *addr = (apr_sockaddr_t *)apr_palloc(pool, sizeof(apr_sockaddr_t));
    int adrlen = 16, iplen = 4;

    if(pAddr->sa_family == AF_INET6)
    {
        adrlen = 46;
        iplen = 16;
    }

    addr->addr_str_len = adrlen;
    addr->family = pAddr->sa_family;

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
    memcpy(&addr->sa.sin.sin_addr.S_un.S_addr, pAddr->sa_data, iplen);
#else
    memcpy(&addr->sa.sin.sin_addr.s_addr, pAddr->sa_data, iplen);
#endif
    addr->sa.sin.sin_family = pAddr->sa_family;
    addr->sa.sin.sin_port = 80;
    addr->salen = sizeof(addr->sa);
    addr->servname = addr->hostname;

    return addr;
}

//----------------------------------------------------------------------------

char *ZeroTerminate(const char *str, size_t len, apr_pool_t *pool)
{
    char *_n = (char *)apr_palloc(pool, len + 1);

    memcpy(_n, str, len);

    _n[len] = 0;

    return _n;
}

//----------------------------------------------------------------------------
// FUNCTION: ConvertUTF16ToUTF8
// DESC: Converts Unicode UTF-16 (Windows default) text to Unicode UTF-8.
//----------------------------------------------------------------------------

char *ConvertUTF16ToUTF8( __in const WCHAR * pszTextUTF16, size_t cchUTF16, apr_pool_t *pool )
{
    //
    // Special case of NULL or empty input string
    //
    if ( (pszTextUTF16 == NULL) || (*pszTextUTF16 == L'\0') || cchUTF16 == 0 )
    {
        // Return empty string
        return "";
    }

    //
    // Get size of destination UTF-8 buffer, in CHAR's (= bytes)
    //
    int cbUTF8 = ::WideCharToMultiByte(
        CP_UTF8,                // convert to UTF-8
        0,      // specify conversion behavior
        pszTextUTF16,           // source UTF-16 string
        static_cast<int>( cchUTF16 ),   // total source string length, in WCHAR's, 
        NULL,                   // unused - no conversion required in this step
        0,                      // request buffer size
        NULL, NULL              // unused
        );

    if ( cbUTF8 == 0 )
    {
        return "";
    }

    //
    // Allocate destination buffer for UTF-8 string
    //

    int cchUTF8 = cbUTF8; // sizeof(CHAR) = 1 byte

    char *pszUTF8 = (char *)apr_palloc(pool, cchUTF8 + 1 );

    //
    // Do the conversion from UTF-16 to UTF-8
    //
    int result = ::WideCharToMultiByte(
        CP_UTF8,                // convert to UTF-8
        0,      // specify conversion behavior
        pszTextUTF16,           // source UTF-16 string
        static_cast<int>( cchUTF16 ),   // total source string length, in WCHAR's, 
                                        // including end-of-string \0
        pszUTF8,                // destination buffer
        cbUTF8,                 // destination buffer size, in bytes
        NULL, NULL              // unused
        );  

    if ( result == 0 )
    {
        return "";
    }

    pszUTF8[cchUTF8] =  0;

    return pszUTF8;
}
 
static void Log(void *obj, int level, char *str)
{
    CMyHttpModule *mod = (CMyHttpModule *)obj;
    WORD logcat = EVENTLOG_INFORMATION_TYPE;

    level &= APLOG_LEVELMASK;

    if(level <= APLOG_ERR)
        logcat = EVENTLOG_ERROR_TYPE;

    if(level == APLOG_WARNING || strstr(str, "Warning.") != NULL)
        logcat = EVENTLOG_WARNING_TYPE;

    mod->WriteEventViewerLog(str, logcat);
}

#define NOTE_IIS "iis-tx-context"

void StoreIISContext(request_rec *r, RequestStoredContext *rsc)
{
    apr_table_setn(r->notes, NOTE_IIS, (const char *)rsc);
}

RequestStoredContext *RetrieveIISContext(request_rec *r)
{
    RequestStoredContext *msr = NULL;
    request_rec *rx = NULL;

    /* Look in the current request first. */
    msr = (RequestStoredContext *)apr_table_get(r->notes, NOTE_IIS);
    if (msr != NULL) {
        //msr->r = r;
        return msr;
    }

    /* If this is a subrequest then look in the main request. */
    if (r->main != NULL) {
        msr = (RequestStoredContext *)apr_table_get(r->main->notes, NOTE_IIS);
        if (msr != NULL) {
            //msr->r = r;
            return msr;
        }
    }

    /* If the request was redirected then look in the previous requests. */
    rx = r->prev;
    while(rx != NULL) {
        msr = (RequestStoredContext *)apr_table_get(rx->notes, NOTE_IIS);
        if (msr != NULL) {
            //msr->r = r;
            return msr;
        }
        rx = rx->prev;
    }

    return NULL;
}

/*
 * Read request body from IIS structures, copy it over into
 * Apache buckets and then assign back the entire flatten body
 * to IHttpRequest.
 * If the request body is chunked, it will be transformed into fixed-length.
*/
static HRESULT SaveRequestBodyToRequestRec(RequestStoredContext* rsc)
{
    request_rec* aprRequest = rsc->requestRec.get();
    conn_rec* conn = rsc->connRec.get();
    IHttpRequest* iisRequest = rsc->httpContext->GetRequest();
    apr_bucket_brigade* brigade = apr_brigade_create(conn->pool, conn->bucket_alloc);
    if (brigade == nullptr)
    {
        return E_OUTOFMEMORY;
    }

    while (iisRequest->GetRemainingEntityBytes() > 0)
    {
        DWORD bytesRead = 0;
        char* buf = static_cast<char*>(apr_palloc(aprRequest->pool, HUGE_STRING_LEN));
        HRESULT hr = iisRequest->ReadEntityBody(buf, HUGE_STRING_LEN, false, &bytesRead, nullptr);
        if (FAILED(hr))
        {
            // End of data is okay.
            if (ERROR_HANDLE_EOF != (hr & 0x0000FFFF))
            {
                return hr;
            }

            break;
        }

        apr_bucket* bucket = apr_bucket_pool_create(buf, bytesRead, aprRequest->pool, conn->bucket_alloc);
        if (bucket == nullptr)
        {
            return E_OUTOFMEMORY;
        }
        APR_BRIGADE_INSERT_TAIL(brigade, bucket);
    }

    if (!APR_BRIGADE_EMPTY(brigade)) {
        apr_bucket* e = apr_bucket_eos_create(conn->bucket_alloc);
        if (e == nullptr)
        {
            return E_OUTOFMEMORY;
        }
        APR_BRIGADE_INSERT_TAIL(brigade, e);

        modsecSetBodyBrigade(aprRequest, brigade);

        apr_off_t contentLength = 0;
        apr_status_t status = apr_brigade_length(brigade, FALSE, &contentLength);
        if (status != APR_SUCCESS)
        {
            return E_FAIL;
        }

        // Remove/Modify Transfer-Encoding header if "chunked" Encoding is set in the request. 
        // This is to avoid sending both Content-Length and Chunked Transfer-Encoding in the request header.
        static const std::string CHUNKED = "chunked";
        USHORT encodingLength = 0;
        const char* transferEncoding = iisRequest->GetHeader(HttpHeaderTransferEncoding, &encodingLength);
        if (transferEncoding)
        {
            if (encodingLength == CHUNKED.size() &&
                std::equal(CHUNKED.cbegin(), CHUNKED.cend(), transferEncoding,
                [](char lhs, char rhs) { return std::tolower(lhs) == std::tolower(rhs); }))
            {
                iisRequest->DeleteHeader(HttpHeaderTransferEncoding);
            }
        }

        auto contentLengthStr = std::to_string(contentLength);
        HRESULT hr = iisRequest->SetHeader(
            HttpHeaderContentLength,
            contentLengthStr.c_str(),
            contentLengthStr.size(),
            TRUE);

        if (FAILED(hr))
        {
            return hr;
        }

        // since we clean the APR pool at the end of OnSendRequest, we must get IIS-managed memory chunk
        //
        char* requestBuffer = static_cast<char*>(rsc->httpContext->AllocateRequestMemory(contentLength));
        status = apr_brigade_flatten(brigade, requestBuffer, reinterpret_cast<apr_size_t*>(&contentLength));
        if (status != APR_SUCCESS)
        {
            return E_FAIL;
        }
        return iisRequest->InsertEntityBody(requestBuffer, contentLength);
    }

    return S_OK;
}

REQUEST_NOTIFICATION_STATUS
CMyHttpModule::OnBeginRequest(IHttpContext* httpContext, IHttpEventProvider* provider)
{
    if (httpContext == nullptr || httpContext->GetRequest() == nullptr)
    {
        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    CriticalSectionLock lock{cs};
    MODSECURITY_STORED_CONTEXT* config = nullptr;
    HRESULT hr = MODSECURITY_STORED_CONTEXT::GetConfig(httpContext, &config);
    if (FAILED(hr))
    {
        httpContext->GetResponse()->SetStatus(500, "WAF internal error. Unable to get config.");
        httpContext->SetRequestHandled();
        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    // If module is disabled, don't go any further
    //
    if (!config->GetIsEnabled())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto reportConfigurationError = [config, httpContext] {
        config->configLoadingFailed = true;
        httpContext->GetResponse()->SetStatus(500, "WAF internal error. Invalid configuration.");
        httpContext->SetRequestHandled();
        return RQ_NOTIFICATION_FINISH_REQUEST;
    };

    // If we previously failed to load the config, don't spam the event log by trying and failing again
    if (config->configLoadingFailed)
    {
        return reportConfigurationError();
    }

    if (config->config == nullptr)
    {
        char *path;
        USHORT pathlen;

        hr = config->GlobalWideCharToMultiByte(config->GetPath(), wcslen(config->GetPath()), &path, &pathlen);
        if (FAILED(hr))
        {
            return reportConfigurationError();
        }

        config->config = modsecGetDefaultConfig();

        PCWSTR servpath = httpContext->GetApplication()->GetApplicationPhysicalPath();
        char *apppath;
        USHORT apppathlen;

        hr = config->GlobalWideCharToMultiByte((WCHAR *)servpath, wcslen(servpath), &apppath, &apppathlen);
        if (FAILED(hr))
        {
            delete path;
            return reportConfigurationError();
        }

        if (path[0] != 0)
        {
            const char * err = modsecProcessConfig(config->config, path, apppath);

            if (err != NULL)
            {
                WriteEventViewerLog(err, EVENTLOG_ERROR_TYPE);
                delete apppath;
                delete path;
                return reportConfigurationError();
            }

            modsecReportRemoteLoadedRules();
            if (!statusCallAlreadySent)
            {
                statusCallAlreadySent = true;
                modsecStatusEngineCall();
            }
        }

        delete apppath;
        delete path;
    }

    ConnRecPtr connRec = MakeConnReq();
    conn_rec* c = connRec.get();
    RequestRecPtr requestRec = MakeRequestRec(c, config->config);
    request_rec* r = requestRec.get();

    auto rsc = std::make_unique<RequestStoredContext>();
    rsc->connRec = connRec;
    rsc->requestRec = requestRec;
    rsc->httpContext = httpContext;
    rsc->provider = provider;
    rsc->responseProcessingEnabled = (config->config->resbody_access == 1);
    RequestStoredContext* context = rsc.get();

    StoreIISContext(r, rsc.get());

    httpContext->GetModuleContextContainer()->SetModuleContext(rsc.release(), g_pModuleContext);

    HTTP_REQUEST *req = httpContext->GetRequest()->GetRawHttpRequest();

    r->hostname = ConvertUTF16ToUTF8(req->CookedUrl.pHost, req->CookedUrl.HostLength / sizeof(WCHAR), r->pool);
    r->path_info = ConvertUTF16ToUTF8(req->CookedUrl.pAbsPath, req->CookedUrl.AbsPathLength / sizeof(WCHAR), r->pool);

    if(r->hostname == NULL)
    {
        if(req->Headers.KnownHeaders[HttpHeaderHost].pRawValue != NULL)
            r->hostname = ZeroTerminate(req->Headers.KnownHeaders[HttpHeaderHost].pRawValue,
                                        req->Headers.KnownHeaders[HttpHeaderHost].RawValueLength, r->pool);
    }

    int port = 0;
    char *port_str = NULL;

    if(r->hostname != NULL)
    {
        int k = 0;
        char *ptr = (char *)r->hostname;

        while(*ptr != 0 && *ptr != ':')
            ptr++;

        if(*ptr == ':')
        {
            *ptr = 0;
            port_str = ptr + 1;
            port = atoi(port_str);
        }
    }

    if(req->CookedUrl.pQueryString != NULL && req->CookedUrl.QueryStringLength > 0)
        r->args = ConvertUTF16ToUTF8(req->CookedUrl.pQueryString + 1, (req->CookedUrl.QueryStringLength / sizeof(WCHAR)) - 1, r->pool);

#define _TRANSHEADER(id,str) if(req->Headers.KnownHeaders[id].pRawValue != NULL) \
    {\
        apr_table_setn(r->headers_in, str, \
            ZeroTerminate(req->Headers.KnownHeaders[id].pRawValue, req->Headers.KnownHeaders[id].RawValueLength, r->pool)); \
    }

    _TRANSHEADER(HttpHeaderCacheControl, "Cache-Control");
    _TRANSHEADER(HttpHeaderConnection, "Connection");
    _TRANSHEADER(HttpHeaderDate, "Date");
    _TRANSHEADER(HttpHeaderKeepAlive, "Keep-Alive");             
    _TRANSHEADER(HttpHeaderPragma, "Pragma");                
    _TRANSHEADER(HttpHeaderTrailer, "Trailer");               
    _TRANSHEADER(HttpHeaderTransferEncoding, "Transfer-Encoding");      
    _TRANSHEADER(HttpHeaderUpgrade, "Upgrade");               
    _TRANSHEADER(HttpHeaderVia, "Via");                   
    _TRANSHEADER(HttpHeaderWarning, "Warning");               
    _TRANSHEADER(HttpHeaderAllow, "Allow");                 
    _TRANSHEADER(HttpHeaderContentLength, "Content-Length");         
    _TRANSHEADER(HttpHeaderContentType, "Content-Type");           
    _TRANSHEADER(HttpHeaderContentEncoding, "Content-Encoding");       
    _TRANSHEADER(HttpHeaderContentLanguage, "Content-Language");       
    _TRANSHEADER(HttpHeaderContentLocation, "Content-Location");       
    _TRANSHEADER(HttpHeaderContentMd5, "Content-Md5");            
    _TRANSHEADER(HttpHeaderContentRange, "Content-Range");          
    _TRANSHEADER(HttpHeaderExpires, "Expires");               
    _TRANSHEADER(HttpHeaderLastModified, "Last-Modified");          
    _TRANSHEADER(HttpHeaderAccept, "Accept");                
    _TRANSHEADER(HttpHeaderAcceptCharset, "Accept-Charset");        
    _TRANSHEADER(HttpHeaderAcceptEncoding, "Accept-Encoding");        
    _TRANSHEADER(HttpHeaderAcceptLanguage, "Accept-Language");        
    _TRANSHEADER(HttpHeaderAuthorization, "Authorization");         
    _TRANSHEADER(HttpHeaderCookie, "Cookie");                
    _TRANSHEADER(HttpHeaderExpect, "Expect");                
    _TRANSHEADER(HttpHeaderFrom, "From");                  
    _TRANSHEADER(HttpHeaderHost, "Host");                  
    _TRANSHEADER(HttpHeaderIfMatch, "If-Match");               
    _TRANSHEADER(HttpHeaderIfModifiedSince, "If-Modified-Since");       
    _TRANSHEADER(HttpHeaderIfNoneMatch, "If-None-Match");           
    _TRANSHEADER(HttpHeaderIfRange, "If-Range");               
    _TRANSHEADER(HttpHeaderIfUnmodifiedSince, "If-Unmodified-Since");     
    _TRANSHEADER(HttpHeaderMaxForwards, "Max-Forwards");           
    _TRANSHEADER(HttpHeaderProxyAuthorization, "Proxy-Authorization");    
    _TRANSHEADER(HttpHeaderReferer, "Referer");               
    _TRANSHEADER(HttpHeaderRange, "Range");                 
    _TRANSHEADER(HttpHeaderTe, "TE");                    
    _TRANSHEADER(HttpHeaderTranslate, "Translate");             
    _TRANSHEADER(HttpHeaderUserAgent, "User-Agent");

#undef _TRANSHEADER

    for(int i = 0; i < req->Headers.UnknownHeaderCount; i++)
    {
        apr_table_setn(r->headers_in, 
            ZeroTerminate(req->Headers.pUnknownHeaders[i].pName, req->Headers.pUnknownHeaders[i].NameLength, r->pool), 
            ZeroTerminate(req->Headers.pUnknownHeaders[i].pRawValue, req->Headers.pUnknownHeaders[i].RawValueLength, r->pool));
    }

    r->content_encoding = apr_table_get(r->headers_in, "Content-Encoding");
    r->content_type = apr_table_get(r->headers_in, "Content-Type");

    const char *lng = apr_table_get(r->headers_in, "Content-Languages");

    if(lng != NULL)
    {
        r->content_languages = apr_array_make(r->pool, 1, sizeof(const char *));

        *(const char **)apr_array_push(r->content_languages) = lng;
    }

    switch(req->Verb)
    {
    case HttpVerbUnparsed:
    case HttpVerbUnknown:
    case HttpVerbInvalid:
    case HttpVerbTRACK:  // used by Microsoft Cluster Server for a non-logged trace
    case HttpVerbSEARCH:
    default:
        r->method = "INVALID";
        r->method_number = M_INVALID;
        break;
    case HttpVerbOPTIONS:
        r->method = "OPTIONS";
        r->method_number = M_OPTIONS;
        break;
    case HttpVerbGET:
    case HttpVerbHEAD:
        r->method = "GET";
        r->method_number = M_GET;
        break;
    case HttpVerbPOST:
        r->method = "POST";
        r->method_number = M_POST;
        break;
    case HttpVerbPUT:
        r->method = "PUT";
        r->method_number = M_PUT;
        break;
    case HttpVerbDELETE:
        r->method = "DELETE";
        r->method_number = M_DELETE;
        break;
    case HttpVerbTRACE:
        r->method = "TRACE";
        r->method_number = M_TRACE;
        break;
    case HttpVerbCONNECT:
        r->method = "CONNECT";
        r->method_number = M_CONNECT;
        break;
    case HttpVerbMOVE:
        r->method = "MOVE";
        r->method_number = M_MOVE;
        break;
    case HttpVerbCOPY:
        r->method = "COPY";
        r->method_number = M_COPY;
        break;
    case HttpVerbPROPFIND:
        r->method = "PROPFIND";
        r->method_number = M_PROPFIND;
        break;
    case HttpVerbPROPPATCH:
        r->method = "PROPPATCH";
        r->method_number = M_PROPPATCH;
        break;
    case HttpVerbMKCOL:
        r->method = "MKCOL";
        r->method_number = M_MKCOL;
        break;
    case HttpVerbLOCK:
        r->method = "LOCK";
        r->method_number = M_LOCK;
        break;
    case HttpVerbUNLOCK:
        r->method = "UNLOCK";
        r->method_number = M_UNLOCK;
        break;
    }

    if(HTTP_EQUAL_VERSION(req->Version, 0, 9))
        r->protocol = "HTTP/0.9";
    else if(HTTP_EQUAL_VERSION(req->Version, 1, 0))
        r->protocol = "HTTP/1.0";
    else
        r->protocol = "HTTP/1.1";

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

    r->unparsed_uri = ZeroTerminate(req->pRawUrl, req->RawUrlLength, r->pool);
    r->uri = r->unparsed_uri;

    r->the_request = (char *)apr_palloc(r->pool, strlen(r->method) + 1 + req->RawUrlLength + 1 + strlen(r->protocol) + 1);

    strcpy(r->the_request, r->method);
    strcat(r->the_request, " ");
    strcat(r->the_request, r->uri);
    strcat(r->the_request, " ");
    strcat(r->the_request, r->protocol);

    HTTP_REQUEST_ID httpRequestID;
    char *pszValue = (char *)apr_palloc(r->pool, 24);

    httpRequestID = httpContext->GetRequest()->GetRawHttpRequest()->RequestId;

    _ui64toa(httpRequestID, pszValue, 10);

    apr_table_setn(r->subprocess_env, "UNIQUE_ID", pszValue);

    PSOCKADDR pAddr = httpContext->GetRequest()->GetRemoteAddress();

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
    c->remote_addr = CopySockAddr(r->pool, pAddr);
    c->remote_ip = GetIpAddr(r->pool, pAddr);
#else
    c->client_addr = CopySockAddr(r->pool, pAddr);
    c->client_ip = GetIpAddr(r->pool, pAddr);
#endif
    c->remote_host = NULL;

    if (config->config->reqbody_access) {
        hr = SaveRequestBodyToRequestRec(context);
        if (FAILED(hr)) {
            context->provider->SetErrorStatus(hr);
            return RQ_NOTIFICATION_FINISH_REQUEST;
        }
    }

    if (config->config->is_enabled == MODSEC_DETECTION_ONLY)
    {
        // We post the processing task to the thread pool to happen in the background.
        // We store the future to track and wait for this processing in case if we also
        // need to process the response because we need request processing to finish
        // before we can start with the response.
        context->detectionProcessing = asio::post(threadPool,
            std::packaged_task<int()> {
            [connRec, requestRec] {
                return modsecProcessRequest(requestRec.get());
            }
        });
    }
    else
    {
        lock.unlock();
        int status = modsecProcessRequest(r);
        lock.lock();

        if (status != DECLINED)
        {
            httpContext->GetResponse()->SetStatus(status, "ModSecurity Action");
            httpContext->SetRequestHandled();

            return RQ_NOTIFICATION_FINISH_REQUEST;
        }
    }

    return RQ_NOTIFICATION_CONTINUE;
}

CMyHttpModule::CMyHttpModule()
{
    modsecSetLogHook(this, Log);

    server_rec* s = modsecInit();
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
    char* compname = static_cast<char *>(malloc(size));
    if (compname == nullptr)
    {
        throw std::bad_alloc();
    }
    GetComputerName(compname, &size);
    s->server_hostname = compname;

    modsecStartConfig();
    modsecFinalizeConfig();
    modsecInitProcess();
}

void CMyHttpModule::Dispose()
{
}

BOOL CMyHttpModule::WriteEventViewerLog(LPCSTR szNotification, WORD category)
{
    return logger.Log(szNotification, category);
}
