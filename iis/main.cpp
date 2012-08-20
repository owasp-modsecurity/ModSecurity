#define WIN32_LEAN_AND_MEAN

#undef inline
#define inline inline

//  IIS7 Server API header file
#include "httpserv.h"

//  Project header files
#include "mymodule.h"
#include "mymodulefactory.h"

//  Global server instance
IHttpServer *                       g_pHttpServer = NULL;

//  Global module context id
PVOID                               g_pModuleContext = NULL;

//  The RegisterModule entrypoint implementation.
//  This method is called by the server when the module DLL is 
//  loaded in order to create the module factory,
//  and register for server events.
HRESULT
__stdcall
RegisterModule(
    DWORD                           dwServerVersion,
    IHttpModuleRegistrationInfo *   pModuleInfo,
    IHttpServer *                   pHttpServer
)
{
    HRESULT                             hr = S_OK;
    CMyHttpModuleFactory  *             pFactory = NULL;
    //IHttpModuleRegistrationInfo2 *		pModuleInfo2;

    if ( pModuleInfo == NULL || pHttpServer == NULL )
    {
        hr = HRESULT_FROM_WIN32( ERROR_INVALID_PARAMETER );
        goto Finished;
    }

    /*hr = HttpGetExtendedInterface( g_pGlobalInfo, 
                                   pModuleInfo, 
                                   &pModuleInfo2 );
    if ( FAILED ( hr ) )
    {
        goto Finished;
    }*/

    // step 1: save the IHttpServer and the module context id for future use
	//
    g_pModuleContext = pModuleInfo->GetId();
    g_pHttpServer = pHttpServer;

    // step 2: create the module factory
	//
    pFactory = new CMyHttpModuleFactory();
    if ( pFactory == NULL )
    {
        hr = HRESULT_FROM_WIN32( ERROR_NOT_ENOUGH_MEMORY );
        goto Finished;
    }

    // step 3: register for server events
	//
    hr = pModuleInfo->SetRequestNotifications( pFactory, /* module factory */
                                               RQ_BEGIN_REQUEST | RQ_SEND_RESPONSE /* server event mask */,
                                               RQ_END_REQUEST /* server post event mask */);
    if ( FAILED( hr ) )
    {
        goto Finished;
    }

	hr = pModuleInfo->SetPriorityForRequestNotification(RQ_BEGIN_REQUEST, PRIORITY_ALIAS_FIRST);
	hr = pModuleInfo->SetPriorityForRequestNotification(RQ_SEND_RESPONSE, PRIORITY_ALIAS_LAST);	// reverted!
	//hr = pModuleInfo2->SetPriorityForPostRequestNotification(RQ_END_REQUEST, PRIORITY_ALIAS_LAST);

    pFactory = NULL;

Finished:
    
/*    if ( pFactory != NULL )
    {
        delete pFactory;
        pFactory = NULL;
    }   */

    return hr;
}
