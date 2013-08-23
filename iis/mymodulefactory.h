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

#ifndef __MODULE_FACTORY_H__
#define __MODULE_FACTORY_H__

// Factory class for CMyHttpModule.
// This class is responsible for creating instances
// of CMyHttpModule for each request.
class CMyHttpModuleFactory : public IHttpModuleFactory
{
        CMyHttpModule *				m_pModule;
		CRITICAL_SECTION			m_csLock;

public:
	CMyHttpModuleFactory()
	{
		m_pModule = NULL;

		InitializeCriticalSection(&m_csLock);
	}

	virtual
    HRESULT
    GetHttpModule(
        OUT CHttpModule            **ppModule, 
        IN IModuleAllocator        *
    )
    {
        HRESULT                    hr = S_OK;

	    if ( ppModule == NULL )
        {
            hr = HRESULT_FROM_WIN32( ERROR_INVALID_PARAMETER );
            goto Finished;
        }

		EnterCriticalSection(&m_csLock);

		if(m_pModule == NULL)
		{
			m_pModule = new CMyHttpModule();

			if ( m_pModule == NULL )
			{
				hr = HRESULT_FROM_WIN32( ERROR_NOT_ENOUGH_MEMORY );
				goto Finished;
			}
		}

		LeaveCriticalSection(&m_csLock);

        *ppModule = m_pModule;

	Finished:

        return hr;
    }

    virtual 
    void
    Terminate()
    {
        if ( m_pModule != NULL )
        {
			//m_pModule->WriteEventViewerLog("Module terminated.");
            delete m_pModule;
            m_pModule = NULL;
        }

        delete this;
    }
};

#endif
