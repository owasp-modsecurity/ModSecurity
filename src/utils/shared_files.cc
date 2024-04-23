/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */

#include "src/utils/shared_files.h"

#include <fcntl.h>
#ifdef WIN32
#include <windows.h>
#include <io.h>
#endif


namespace modsecurity {
namespace utils {


SharedFiles::handlers_map::iterator SharedFiles::add_new_handler(
    const std::string &fileName, std::string *error) {
    FILE *fp = fopen(fileName.c_str(), "a");
    if (fp == 0) {
        error->assign("Failed to open file: " + fileName);
        return m_handlers.end();
    }

    return m_handlers.insert({ fileName, {fp, 0} }).first;
}


bool SharedFiles::open(const std::string& fileName, std::string *error) {
    auto it = m_handlers.find(fileName);
    if (it == m_handlers.end()) {
        it = add_new_handler(fileName, error);
        if (error->size() > 0)
            return false;
    }

    if (it == m_handlers.end()) {
        error->assign("Not able to open: " + fileName);
        return false;
    }

    it->second.cnt++;

    return true;
}


void SharedFiles::close(const std::string& fileName) {
    if (fileName.empty())
        return;

    auto it = m_handlers.find(fileName);
    if (it == m_handlers.end())
        return;

    it->second.cnt--;
    if (it->second.cnt == 0)
    {
        fclose(it->second.fp);

        m_handlers.erase(it);
    }
}


bool SharedFiles::write(const std::string& fileName,
    const std::string &msg, std::string *error) {
    bool ret = true;

    auto it = m_handlers.find(fileName);
    if (it == m_handlers.end()) {
        error->assign("file is not open: " + fileName);
        return false;
    }

    //Exclusively lock whole file
#ifndef WIN32
    struct flock lock {};
    lock.l_start = lock.l_len = lock.l_whence = 0;
    lock.l_type = F_WRLCK;
    fcntl(fileno(it->second.fp), F_SETLKW, &lock);
#else
    auto handle = reinterpret_cast<HANDLE>(_get_osfhandle(fileno(it->second.fp)));
    OVERLAPPED overlapped = { 0 };
    ::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
#endif

    auto wrote = fwrite(msg.c_str(), 1, msg.size(), it->second.fp);
    if (wrote < msg.size()) {
        error->assign("failed to write: " + fileName);
        ret = false;
    }
    fflush(it->second.fp);

    //Remove exclusive lock
#ifndef WIN32
    lock.l_type = F_UNLCK;
    fcntl(fileno(it->second.fp), F_SETLKW, &lock);
#else
    overlapped = { 0 };
    ::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
#endif

    return ret;
}


}  // namespace utils
}  // namespace modsecurity
