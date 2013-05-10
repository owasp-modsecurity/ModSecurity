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

#pragma once

#define MODSECURITY_SECTION                        L"system.webServer/ModSecurity"
#define MODSECURITY_SECTION_ENABLED                L"enabled"
#define MODSECURITY_SECTION_CONFIGFILE			   L"configFile"

extern IHttpServer *                       g_pHttpServer;

extern PVOID                               g_pModuleContext;

class MODSECURITY_STORED_CONTEXT : public IHttpStoredContext
{
 public:
    MODSECURITY_STORED_CONTEXT();
    ~MODSECURITY_STORED_CONTEXT();
    
	static
    HRESULT
    GetConfig(
        IHttpContext *   pContext,
        MODSECURITY_STORED_CONTEXT ** ppModuleConfig
    );

    // virtual
    VOID
    CleanupStoredContext(
        VOID
    )
    {
        delete this;
    }

    BOOL   GetIsEnabled()
    {
        return m_bIsEnabled;
    }

    WCHAR* GetPath()
    {
        return m_pszPath;
    }

    HRESULT
    Initialize(
        IHttpContext *              pW3Context,
        IAppHostConfigException **  ppException
    );

    DWORD 
    GlobalWideCharToMultiByte(
            WCHAR*  pSource,
            DWORD   dwLengthSource,
            CHAR**  ppszDestination,
            USHORT*  pdwLengthDestination );

	void*			  m_Config;
	DWORD			  m_dwLastCheck;
	FILETIME		  m_LastChange;

private:
    HRESULT 
    GetBooleanPropertyValue( 
            IAppHostElement*            pElement,
            WCHAR*                      pszPropertyName,
            IAppHostPropertyException** pException,
            BOOL*                       pBoolValue );

    HRESULT 
    GetDWORDPropertyValue( 
            IAppHostElement*            pElement,
            WCHAR*                      pszPropertyName,
            IAppHostPropertyException** pException,
            DWORD*                      pnValue );

    HRESULT 
    GetTimeSpanPropertyValue( 
            IAppHostElement*            pElement,
            WCHAR*                      pszPropertyName,
            IAppHostPropertyException** pException,
            ULONGLONG*                 pnValue );

    HRESULT 
    GetStringPropertyValue( 
            IAppHostElement*            pElement,
            WCHAR*                      pszPropertyName,
            IAppHostPropertyException** pException,
            WCHAR**                     ppszValue );

    BOOL              m_bIsEnabled;
    WCHAR*            m_pszPath;
};
