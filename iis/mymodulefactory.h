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

#include <memory>
#include <mutex>

// Factory class for CMyHttpModule.
// This class is responsible for creating instances
// of CMyHttpModule for each request.
class CMyHttpModuleFactory : public IHttpModuleFactory
{
public:
    CMyHttpModuleFactory() /* noexcept */ = default;

    HRESULT GetHttpModule(CHttpModule** ppModule, IModuleAllocator*) override
    {
	    if (ppModule == nullptr)
        {
            return HRESULT_FROM_WIN32(ERROR_INVALID_PARAMETER);
        }

        try
        {
            // In the unlikely case if the module allocation fails,
            // the init_flag will not be set and the next time another
            // attempt will be made to initialize the module.
            std::call_once(init_flag, [this] {
                this->module = std::make_unique<CMyHttpModule>();
            });
        }
        catch (const std::bad_alloc&) 
        {
            return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
        }

        *ppModule = module.get();
        return S_OK;
    }

    void Terminate() override
    {
        if (module)
        {
            module.reset();
        }

        delete this;
    }

private:
    std::unique_ptr<CMyHttpModule> module;
    std::once_flag init_flag;
};