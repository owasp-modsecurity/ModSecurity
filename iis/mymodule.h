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

#define ASIO_STANDALONE
#include "asio/thread_pool.hpp"

#include "critical_section.h"
#include "event_logger.h"

//  The module implementation.
//  This class is responsible for implementing the 
//  module functionality for each of the server events
//  that it registers for.
class CMyHttpModule 
    : public CHttpModule
{
public:
    CMyHttpModule();

    REQUEST_NOTIFICATION_STATUS
    OnBeginRequest(IHttpContext*, IHttpEventProvider*) override;

    void Dispose() override;

    BOOL WriteEventViewerLog(LPCSTR szNotification, WORD category = EVENTLOG_INFORMATION_TYPE);

private:
    CriticalSection cs;
    EventLogger logger;
    asio::thread_pool threadPool{12};
    DWORD pageSize = 0;
    bool statusCallAlreadySent = false;
};

