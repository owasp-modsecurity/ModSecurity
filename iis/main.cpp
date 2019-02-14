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

#include <memory>

//  IIS7 Server API header file
#include "httpserv.h"

//  Project header files
#include "mymodule.h"
#include "mymodulefactory.h"

//  Global server instance
IHttpServer* g_pHttpServer = nullptr;

//  Global module context id
void* g_pModuleContext = nullptr;

//  The RegisterModule entrypoint implementation.
//  This method is called by the server when the module DLL is 
//  loaded in order to create the module factory,
//  and register for server events.
HRESULT __stdcall
RegisterModule(DWORD, IHttpModuleRegistrationInfo* moduleInfo, IHttpServer * httpServer)
{
    if (moduleInfo == nullptr || httpServer == nullptr)
    {
        return HRESULT_FROM_WIN32(ERROR_INVALID_PARAMETER);
    }

    // step 1: save the IHttpServer and the module context id for future use
	//
    g_pModuleContext = moduleInfo->GetId();
    g_pHttpServer = httpServer;

    try
    {
        // step 2: create the module factory
        //
        auto factory = std::make_unique<CMyHttpModuleFactory>();

        // step 3: register for server events
        //
        auto result = moduleInfo->SetRequestNotifications(
            factory.release(), /* module factory */
            RQ_BEGIN_REQUEST | RQ_SEND_RESPONSE /* server event mask */,
            RQ_END_REQUEST /* server post event mask */);
        if (FAILED(result))
        {
            return result;
        }

        result = moduleInfo->SetPriorityForRequestNotification(RQ_BEGIN_REQUEST, PRIORITY_ALIAS_FIRST);
        if (FAILED(result))
        {
            return result;
        }

        return moduleInfo->SetPriorityForRequestNotification(RQ_SEND_RESPONSE, PRIORITY_ALIAS_LAST);
    }
    catch (const std::bad_alloc&)
    {
        return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
    }
}
