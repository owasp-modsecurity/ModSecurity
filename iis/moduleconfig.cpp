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
#define inline inline

//  IIS7 Server API header file
#include "httpserv.h"

//  Project header files
#include "mymodule.h"
#include "mymodulefactory.h"
#include "moduleconfig.h"

HRESULT
MODSECURITY_STORED_CONTEXT::Initialize(
    IHttpContext *              pW3Context,
    IAppHostConfigException **  ppException
)
{
    HRESULT                    hr                       = S_OK;
    IAppHostAdminManager       *pAdminManager           = nullptr;
    IAppHostElement            *pSessionTrackingElement = nullptr;
    IAppHostPropertyException  *pPropertyException      = nullptr;

    PCWSTR pszConfigPath = pW3Context->GetMetadata()->GetMetaPath();
    BSTR bstrUrlPath     = SysAllocString( pszConfigPath );

    pAdminManager = g_pHttpServer->GetAdminManager();

    if ( ( FAILED( hr ) ) || ( pAdminManager == nullptr ) )
    {
        hr = E_UNEXPECTED;
        goto Failure;   
    }

    // Get a handle to the section:
    hr = pAdminManager->GetAdminSection(
                                MODSECURITY_SECTION,
                                bstrUrlPath,
                                &pSessionTrackingElement );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    if ( pSessionTrackingElement == nullptr )
    {
        hr = E_UNEXPECTED;
        goto Failure;
    }

    // Get the property object for the 'enabled' attribute:
    hr = GetBooleanPropertyValue( 
                pSessionTrackingElement,
                MODSECURITY_SECTION_ENABLED,
                &pPropertyException,
                &m_bIsEnabled);

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    // If there is a config failure, we cannot continue execution:
    if ( pPropertyException != nullptr )
    {
        
        // If there is a config failure, we cannot continue execution
        // To avoid using reinterpret_cast, we will use static_cast to 
        // convert the IAppHostPropertyException to IAppHostConfigException
        if( ppException != nullptr )
        {
            *ppException = static_cast<IAppHostConfigException*>(pPropertyException);
            (*ppException) -> AddRef();
        }
        goto Failure;
    }

    if ( m_bIsEnabled == FALSE )
    {
        // There is no point in reading any more of the config associated with the session
        // tracking section, since this feature is not enabled for the current URL 
        goto Failure;
    }

    // Get the property object for the 'configfile' attribute:
    hr = GetStringPropertyValue( 
                pSessionTrackingElement,
                MODSECURITY_SECTION_CONFIGFILE,
                &pPropertyException,
                &m_pszPath);

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    // If there is a config failure, we cannot continue execution:
    if ( pPropertyException != nullptr )
    {
        if ( ppException != nullptr ){
            // See prevoius comment regarding static_cast and ppException assignment
            *ppException = static_cast<IAppHostConfigException*>(pPropertyException);
            (*ppException) -> AddRef();
        }
        goto Failure;
    }

Failure:
    SysFreeString( bstrUrlPath );
    return hr;
}

HRESULT 
MODSECURITY_STORED_CONTEXT::GetBooleanPropertyValue( 
        IAppHostElement*            pElement,
        WCHAR*                      pszPropertyName,
        IAppHostPropertyException** pException,
        BOOL*                       pBoolValue )
{
    HRESULT                 hr              = S_OK;
    IAppHostProperty        *pProperty      = nullptr;    
    VARIANT                 vPropertyValue;

    if ( 
           ( pElement        == nullptr ) || 
           ( pszPropertyName == nullptr ) ||
           ( pException      == nullptr ) ||
           ( pBoolValue      == nullptr )
       )
    {
        hr = E_INVALIDARG;
        goto Failure;
    }

    // Get the property object for the BOOLEAN attribute:
    hr = pElement->GetPropertyByName( 
                        pszPropertyName,
                        &pProperty );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    if ( pProperty == nullptr )
    {
        hr = E_UNEXPECTED;
        goto Failure;
    }

    // Get the attribute value:
    VariantInit( &vPropertyValue );

    hr = pProperty->get_Value( &vPropertyValue );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    // See it there is an exception that might be due to the actual value in the 
    // config not meeting validation criteria
    *pException = nullptr;

    hr = pProperty->get_Exception( pException );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    // No need to continue if we got an exception...
    if ( ( *pException ) != nullptr )
    {
        goto Failure;
    }

    // Finally, get the value:
    *pBoolValue = ( vPropertyValue.boolVal == VARIANT_TRUE ) ? TRUE : FALSE;
    

Failure:
    VariantClear( &vPropertyValue );

    if ( pProperty != nullptr )
    {
        pProperty->Release();
        pProperty = nullptr;
    }

    return hr;
}

HRESULT 
MODSECURITY_STORED_CONTEXT::GetDWORDPropertyValue( 
        IAppHostElement*            pElement,
        WCHAR*                      pszPropertyName,
        IAppHostPropertyException** pException,
        DWORD*                      pnValue )
{
    HRESULT                 hr              = S_OK;
    IAppHostProperty        *pProperty      = nullptr;    
    VARIANT                 vPropertyValue;

    if ( 
           ( pElement        == nullptr ) || 
           ( pszPropertyName == nullptr ) ||
           ( pException      == nullptr ) ||
           ( pnValue         == nullptr )
       )
    {
        hr = E_INVALIDARG;
        goto Failure;
    }

    // Get the property object for the INT attribute:
    hr = pElement->GetPropertyByName( 
                        pszPropertyName,
                        &pProperty );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    if ( pProperty == nullptr )
    {
        hr = E_UNEXPECTED;
        goto Failure;
    }

    // Get the attribute value:
    VariantInit( &vPropertyValue );

    hr = pProperty->get_Value( &vPropertyValue );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    // See it there is an exception that might be due to the actual value in the 
    // config not meeting validation criteria
    *pException = nullptr;

    hr = pProperty->get_Exception( pException );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    // No need to continue if we got an exception...
    if ( ( *pException ) != nullptr )
    {
        goto Failure;
    }

    // Finally, get the value:
    *pnValue =  vPropertyValue.ulVal;  

Failure:
    VariantClear( &vPropertyValue );

    if ( pProperty != nullptr )
    {
        pProperty->Release();
        pProperty = nullptr;
    }

    return hr;
}

HRESULT 
MODSECURITY_STORED_CONTEXT::GetTimeSpanPropertyValue( 
        IAppHostElement*            pElement,
        WCHAR*                      pszPropertyName,
        IAppHostPropertyException** pException,
        ULONGLONG*                 pnValue )
{
    HRESULT                 hr              = S_OK;
    IAppHostProperty        *pProperty      = nullptr;    
    VARIANT                 vPropertyValue;

    if ( 
           ( pElement        == nullptr ) || 
           ( pszPropertyName == nullptr ) ||
           ( pException      == nullptr ) ||
           ( pnValue         == nullptr )
       )
    {
        hr = E_INVALIDARG;
        goto Failure;
    }

    // Get the property object for the INT attribute:
    hr = pElement->GetPropertyByName( 
                        pszPropertyName,
                        &pProperty );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    if ( pProperty == nullptr )
    {
        hr = E_UNEXPECTED;
        goto Failure;
    }

    // Get the attribute value:
    VariantInit( &vPropertyValue );

    hr = pProperty->get_Value( &vPropertyValue );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    // See it there is an exception that might be due to the actual value in the 
    // config not meeting validation criteria
    *pException = nullptr;

    hr = pProperty->get_Exception( pException );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    // No need to continue if we got an exception...
    if ( ( *pException ) != nullptr )
    {
        goto Failure;
    }

    // Finally, get the value:
    *pnValue =  vPropertyValue.ullVal;  

Failure:
    VariantClear( &vPropertyValue );

    if ( pProperty != nullptr )
    {
        pProperty->Release();
        pProperty = nullptr;
    }

    return hr;
}

HRESULT 
MODSECURITY_STORED_CONTEXT::GetStringPropertyValue( 
        IAppHostElement*            pElement,
        WCHAR*                      pszPropertyName,
        IAppHostPropertyException** pException,
        WCHAR**                     ppszValue )
{
    HRESULT                 hr              = S_OK;
    IAppHostProperty        *pProperty      = nullptr;    
    DWORD                   dwLength;
    VARIANT                 vPropertyValue;

    if ( 
           ( pElement        == nullptr ) || 
           ( pszPropertyName == nullptr ) ||
           ( pException      == nullptr ) ||
           ( ppszValue       == nullptr )
       )
    {
        hr = E_INVALIDARG;
        goto Failure;
    }

    *ppszValue = nullptr;

    // Get the property object for the string attribute:
    hr = pElement->GetPropertyByName( 
                        pszPropertyName,
                        &pProperty );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    if ( pProperty == nullptr )
    {
        hr = E_UNEXPECTED;
        goto Failure;
    }

    // Get the attribute value:
    VariantInit( &vPropertyValue );

    hr = pProperty->get_Value( &vPropertyValue );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    // See it there is an exception that might be due to the actual value in the 
    // config not meeting validation criteria
    *pException = nullptr;

    hr = pProperty->get_Exception( pException );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    // No need to continue if we got an exception...
    if ( ( *pException ) != nullptr )
    {
        goto Failure;
    }

    // Finally, get the value:
    dwLength = SysStringLen( vPropertyValue.bstrVal );
    *ppszValue = new WCHAR[ dwLength + 1 ];

    if ( (*ppszValue) == nullptr )
    {
        hr = E_OUTOFMEMORY;
        goto Failure;
    }

    wcsncpy(
        *ppszValue,
        vPropertyValue.bstrVal,
        dwLength );

    (*ppszValue)[ dwLength ] = L'\0';
    
Failure:
    VariantClear( &vPropertyValue );

    if ( pProperty != nullptr )
    {
        pProperty->Release();
        pProperty = nullptr;
    }

    return hr;
}

MODSECURITY_STORED_CONTEXT::~MODSECURITY_STORED_CONTEXT()
{
    if ( m_pszPath != nullptr )
    {
        delete [] m_pszPath;
        m_pszPath = nullptr;
    }
}

MODSECURITY_STORED_CONTEXT::MODSECURITY_STORED_CONTEXT():
    m_bIsEnabled ( FALSE ),
    m_pszPath( nullptr ),
	m_Config( nullptr )
{
}

DWORD 
MODSECURITY_STORED_CONTEXT::GlobalWideCharToMultiByte(
        WCHAR*  pSource,
        DWORD   dwLengthSource,
        CHAR**  ppszDestination,
        USHORT*  pdwLengthDestination )
{
    DWORD       dwResult    = 0;
    DWORD       dwCount     = 0;

    if (  
          ( pSource == nullptr ) ||
          ( ppszDestination == nullptr ) ||
          ( pdwLengthDestination == nullptr ) 
       )
    {
        dwResult = ERROR_INVALID_PARAMETER;
        goto Exit;
    }

    // Initialize result length
    *pdwLengthDestination = 0;
    *ppszDestination     = nullptr;

    dwCount =   WideCharToMultiByte( 
                    CP_ACP, 
                    0, 
                    pSource, 
                    dwLengthSource + 1, 
                    *ppszDestination, 
                    0,
                    nullptr,
                    nullptr );

    if ( 0 == dwCount )
    {
        dwResult = GetLastError ();

        if ( dwResult == 0 )
        {
            dwResult = ERROR_INVALID_DATA;
        }

        goto Exit;
    }

    *ppszDestination = new CHAR[ dwCount + 1 ];

    if ( nullptr == ( *ppszDestination ) )
    {
        dwResult = ERROR_OUTOFMEMORY;
        goto Exit;
    }

    // Make sure the memory is 'clean':
    SecureZeroMemory(
        ( *ppszDestination ),
        ( dwCount + 1 ) * sizeof ( CHAR ) );

    if ( 
        0 == WideCharToMultiByte( 
                CP_ACP, 
                0, 
                pSource, 
                dwLengthSource + 1, 
                *ppszDestination, 
                dwCount,
                nullptr,
                nullptr )
       )
    {
        dwResult = GetLastError();

        goto Exit;
    }

    *pdwLengthDestination = ( USHORT )dwCount;

Exit:
    if ( dwResult != 0 )
    {
        // Make sure we do the proper cleanup in the error case:
        if ( pdwLengthDestination != nullptr )
        {
            *pdwLengthDestination = 0;
        }

        if ( ppszDestination != nullptr )
        {
            if ( ( *ppszDestination ) != nullptr )
            {
                delete [] ( *ppszDestination );
                ( *ppszDestination ) = nullptr;
            }
        }
    }

    return dwResult;
}

HRESULT
MODSECURITY_STORED_CONTEXT::GetConfig(
    IHttpContext *   pContext,
    MODSECURITY_STORED_CONTEXT ** ppModuleConfig
)
{
    HRESULT                          hr                 = S_OK;
    MODSECURITY_STORED_CONTEXT * pModuleConfig      = nullptr;
    IHttpModuleContextContainer *    pMetadataContainer = nullptr;
	IAppHostConfigException *        pException         = nullptr;

    pMetadataContainer = pContext->GetMetadata()->GetModuleContextContainer();

	if ( pMetadataContainer == nullptr )
	{
        hr = E_UNEXPECTED;
        return hr;
	}

    pModuleConfig = (MODSECURITY_STORED_CONTEXT *)pMetadataContainer->GetModuleContext( g_pModuleContext );	
    if ( pModuleConfig != nullptr )
    {
        //
        // We found stored data for this module for the metadata
        // object which is different for unique configuration path
        //
        *ppModuleConfig = pModuleConfig;
        return S_OK;
    }

    //
    // If we reach here, that means this is first request or first
    // request after a configuration change IIS core will throw stored context away
    // if a change notification arrives for this metadata path
    //
    pModuleConfig = new MODSECURITY_STORED_CONTEXT();
    if ( pModuleConfig == nullptr )
    {
        return E_OUTOFMEMORY;
    }

    //
    // Read module configuration data and store in MODSECURITY_STORED_CONTEXT
    //
    hr = pModuleConfig->Initialize( pContext, &pException );
    if ( FAILED( hr )  || pException != nullptr )
    {
        pModuleConfig->CleanupStoredContext();

        pModuleConfig = nullptr;
        hr = E_UNEXPECTED;

        return hr;
    }

    //
    // Store MODSECURITY_STORED_CONTEXT data as metadata stored context
    //
    hr = pMetadataContainer->SetModuleContext( pModuleConfig,
                                               g_pModuleContext );
    if ( FAILED( hr ) )
    {
        pModuleConfig->CleanupStoredContext();
        pModuleConfig = nullptr;

        //
        // It is possible that some other thread stored context before this thread
        // could do. Check returned hr and return context stored by other thread
        //
        if ( hr == HRESULT_FROM_WIN32( ERROR_ALREADY_ASSIGNED ) )
        {
            *ppModuleConfig = (MODSECURITY_STORED_CONTEXT *)pMetadataContainer->GetModuleContext( g_pModuleContext );
            return S_OK;
        }
    }

    *ppModuleConfig = pModuleConfig;
    return hr;
}
