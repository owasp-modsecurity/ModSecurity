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
    IAppHostAdminManager       *pAdminManager           = NULL;
    IAppHostElement            *pSessionTrackingElement = NULL;
    IAppHostPropertyException  *pPropertyException      = NULL;

    PCWSTR pszConfigPath = pW3Context->GetMetadata()->GetMetaPath();
    BSTR bstrUrlPath     = SysAllocString( pszConfigPath );

    pAdminManager = g_pHttpServer->GetAdminManager();

    if ( ( FAILED( hr ) ) || ( pAdminManager == NULL ) )
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

    if ( pSessionTrackingElement == NULL )
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
    if ( pPropertyException != NULL )
    {

        ppException = ( IAppHostConfigException** ) &pPropertyException;
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
    if ( pPropertyException != NULL )
    {

        ppException = ( IAppHostConfigException** ) &pPropertyException;
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
    IAppHostProperty        *pProperty      = NULL;    
    VARIANT                 vPropertyValue;

    if ( 
           ( pElement        == NULL ) || 
           ( pszPropertyName == NULL ) ||
           ( pException      == NULL ) ||
           ( pBoolValue      == NULL )
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

    if ( pProperty == NULL )
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
    *pException = NULL;

    hr = pProperty->get_Exception( pException );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    // No need to continue if we got an exception...
    if ( ( *pException ) != NULL )
    {
        goto Failure;
    }

    // Finally, get the value:
    *pBoolValue = ( vPropertyValue.boolVal == VARIANT_TRUE ) ? TRUE : FALSE;
    

Failure:
    VariantClear( &vPropertyValue );

    if ( pProperty != NULL )
    {
        pProperty->Release();
        pProperty = NULL;
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
    IAppHostProperty        *pProperty      = NULL;    
    VARIANT                 vPropertyValue;

    if ( 
           ( pElement        == NULL ) || 
           ( pszPropertyName == NULL ) ||
           ( pException      == NULL ) ||
           ( pnValue         == NULL )
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

    if ( pProperty == NULL )
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
    *pException = NULL;

    hr = pProperty->get_Exception( pException );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    // No need to continue if we got an exception...
    if ( ( *pException ) != NULL )
    {
        goto Failure;
    }

    // Finally, get the value:
    *pnValue =  vPropertyValue.ulVal;  

Failure:
    VariantClear( &vPropertyValue );

    if ( pProperty != NULL )
    {
        pProperty->Release();
        pProperty = NULL;
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
    IAppHostProperty        *pProperty      = NULL;    
    VARIANT                 vPropertyValue;

    if ( 
           ( pElement        == NULL ) || 
           ( pszPropertyName == NULL ) ||
           ( pException      == NULL ) ||
           ( pnValue         == NULL )
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

    if ( pProperty == NULL )
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
    *pException = NULL;

    hr = pProperty->get_Exception( pException );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    // No need to continue if we got an exception...
    if ( ( *pException ) != NULL )
    {
        goto Failure;
    }

    // Finally, get the value:
    *pnValue =  vPropertyValue.ullVal;  

Failure:
    VariantClear( &vPropertyValue );

    if ( pProperty != NULL )
    {
        pProperty->Release();
        pProperty = NULL;
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
    IAppHostProperty        *pProperty      = NULL;    
    DWORD                   dwLength;
    VARIANT                 vPropertyValue;

    if ( 
           ( pElement        == NULL ) || 
           ( pszPropertyName == NULL ) ||
           ( pException      == NULL ) ||
           ( ppszValue       == NULL )
       )
    {
        hr = E_INVALIDARG;
        goto Failure;
    }

    *ppszValue = NULL;

    // Get the property object for the string attribute:
    hr = pElement->GetPropertyByName( 
                        pszPropertyName,
                        &pProperty );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    if ( pProperty == NULL )
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
    *pException = NULL;

    hr = pProperty->get_Exception( pException );

    if ( FAILED( hr ) )
    {
        goto Failure;
    }

    // No need to continue if we got an exception...
    if ( ( *pException ) != NULL )
    {
        goto Failure;
    }

    // Finally, get the value:
    dwLength = SysStringLen( vPropertyValue.bstrVal );
    *ppszValue = new WCHAR[ dwLength + 1 ];

    if ( (*ppszValue) == NULL )
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

    if ( pProperty != NULL )
    {
        pProperty->Release();
        pProperty = NULL;
    }

    return hr;
}

MODSECURITY_STORED_CONTEXT::~MODSECURITY_STORED_CONTEXT()
{
    if ( m_pszPath != NULL )
    {
        delete [] m_pszPath;
        m_pszPath = NULL;
    }
}

MODSECURITY_STORED_CONTEXT::MODSECURITY_STORED_CONTEXT():
    m_bIsEnabled ( FALSE ),
    m_pszPath( NULL ),
	m_Config( NULL ),
	m_dwLastCheck( 0 )
{
	m_LastChange.dwLowDateTime = 0;
	m_LastChange.dwHighDateTime = 0;
}

DWORD 
MODSECURITY_STORED_CONTEXT::GlobalWideCharToMultiByte(
        WCHAR*  pSource,
        DWORD   dwLengthSource,
        CHAR**  ppszDestination,
        USHORT*  pdwLengthDestination )
{
    DWORD       dwResult    = NULL;
    DWORD       dwCount     = 0;

    if (  
          ( pSource == NULL ) ||
          ( ppszDestination == NULL ) ||
          ( pdwLengthDestination == NULL ) 
       )
    {
        dwResult = ERROR_INVALID_PARAMETER;
        goto Exit;
    }

    // Initialize result length
    *pdwLengthDestination = 0;
    *ppszDestination     = NULL;

    dwCount =   WideCharToMultiByte( 
                    CP_ACP, 
                    0, 
                    pSource, 
                    dwLengthSource + 1, 
                    *ppszDestination, 
                    0,
                    NULL,
                    NULL );

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

    if ( NULL == ( *ppszDestination ) )
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
                NULL,
                NULL )
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
        if ( pdwLengthDestination != NULL )
        {
            *pdwLengthDestination = 0;
        }

        if ( ppszDestination != NULL )
        {
            if ( ( *ppszDestination ) != NULL )
            {
                delete [] ( *ppszDestination );
                ( *ppszDestination ) = NULL;
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
    MODSECURITY_STORED_CONTEXT * pModuleConfig      = NULL;
    IHttpModuleContextContainer *    pMetadataContainer = NULL;
	IAppHostConfigException *        pException         = NULL;

    pMetadataContainer = pContext->GetMetadata()->GetModuleContextContainer();

	if ( pMetadataContainer == NULL )
	{
        hr = E_UNEXPECTED;
        return hr;
	}

    pModuleConfig = (MODSECURITY_STORED_CONTEXT *)pMetadataContainer->GetModuleContext( g_pModuleContext );	
    if ( pModuleConfig != NULL )
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
    if ( pModuleConfig == NULL )
    {
        return E_OUTOFMEMORY;
    }

    //
    // Read module configuration data and store in MODSECURITY_STORED_CONTEXT
    //
    hr = pModuleConfig->Initialize( pContext, &pException );
    if ( FAILED( hr )  || pException != NULL )
    {
        pModuleConfig->CleanupStoredContext();

        pModuleConfig = NULL;
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
        pModuleConfig = NULL;

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
