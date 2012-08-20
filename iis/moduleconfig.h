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
