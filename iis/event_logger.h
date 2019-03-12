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

#include <Windows.h>

// A simple class to use to write messages to common Windows event log.
// The class is Moveable but non-Copyable.
class EventLogger
{
public:
    EventLogger() /* noexcept */
        : handle{RegisterEventSource(nullptr, "ModSecurity")}
    {
    }

    BOOL Log(const char* message, DWORD category)
    {
        if (handle != nullptr)
        {
            return ReportEvent(
                handle,
                category, 0, 0x1,
                NULL, 1, 0, &message, NULL);
        }
        return FALSE;
    }

    EventLogger(const EventLogger&) = delete;
    EventLogger& operator=(const EventLogger&) = delete;

    EventLogger(EventLogger&& rhs) /* noexcept */
        : handle(rhs.handle)
    {
        rhs.handle = nullptr;
    }

    EventLogger& operator=(EventLogger&& rhs) /* noexcept */
    {
        handle = rhs.handle;
        rhs.handle = nullptr;
        return *this;
    }

    ~EventLogger()
    {
        if (handle) {
            DeregisterEventSource(handle);
        }
    }

private:
    HANDLE handle;
};