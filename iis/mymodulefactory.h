#ifndef __MODULE_FACTORY_H__
#define __MODULE_FACTORY_H__

// Factory class for CMyHttpModule.
// This class is responsible for creating instances
// of CMyHttpModule for each request.
class CMyHttpModuleFactory : public IHttpModuleFactory
{
        CMyHttpModule *            m_pModule;

public:
	CMyHttpModuleFactory()
	{
		m_pModule = NULL;
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

		if(m_pModule == NULL)
		{
			m_pModule = new CMyHttpModule();

			if ( m_pModule == NULL )
			{
				hr = HRESULT_FROM_WIN32( ERROR_NOT_ENOUGH_MEMORY );
				goto Finished;
			}
		}

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
