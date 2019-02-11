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

#include <mutex>

// Implements BasicLockable requirements:
//      https://en.cppreference.com/w/cpp/named_req/BasicLockable
//
class CriticalSection
{
public:
    CriticalSection()
    {
        InitializeCriticalSection(&cs);
    }

    void lock()
    {
        EnterCriticalSection(&cs);
    }

    void unlock()
    {
        LeaveCriticalSection(&cs);
    }

    ~CriticalSection()
    {
        DeleteCriticalSection(&cs);
    }

private:
    CRITICAL_SECTION cs;
};

using CriticalSectionLock = std::unique_lock<CriticalSection>;