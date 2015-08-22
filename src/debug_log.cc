/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "modsecurity/debug_log.h"

#include <stddef.h>

#include <fstream>


namespace ModSecurity {


/**
 * @name    new_instance
 * @brief   Create a new instance of the DebugLog.
 *
 * @return Debug log pointer
 * @retval >0   Debug log structure was initialized correctly
 * @retval NULL Debug log could not be initialized.
 *
 */
DebugLog *DebugLog::new_instance() {
    return new DebugLog();
}

/**
 * @name    setOutputFile
 * @brief   Set an output file where the log will be saved
 *
 * @param file_path Path to the log file.
 *
 * @return If the operation successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
bool DebugLog::setOutputFile(const std::string& file_path) {
    if (is_open()) {
        close();
    }

    open(file_path, std::fstream::out | std::fstream::app);

    if (!is_open()) {
        return false;
    }

    return true;
}


/**
 * @name    write_log
 * @brief   Write a message into the debug log.
 *
 * @param debug_level Debug level of the given message.
 * @param text Message to be written.
 *
 * @return If the operation successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
bool DebugLog::write_log(int debug_level, const std::string &text) {
    std::cout << "?" << std::to_string(is_open()) << ":" << std::to_string(m_debug_level) <<" [" << debug_level << "] " << text << std::endl;
    if (!is_open()) {
        return false;
    }

    if (debug_level <= m_debug_level) {
        *this << "[" << debug_level << "] " << text << std::endl;
    }

    return true;
}


/**
 * @name    setDebugLevel
 * @brief   Changes the default debug level.
 *
 * @param level Debug level.
 *
 * @return If the operation successful or not.
 * @retval true Operation was successful.
 * @retval false Operation failed.
 *
 */
bool DebugLog::setDebugLevel(int level) {
    if (level < 0 || level > 9) {
        return false;
    }

    m_debug_level = level;

    return true;
}


/**
 * @name    isConfigured
 * @brief   Returns if debug log is configured or not.
 *
 * @return If the debug log is configured or not
 * @retval true It is configured.
 * @retval false It is not configured.
 *
 */
bool DebugLog::isConfigured() {
    return m_is_configured;
}


void DebugLog::refCountDecreaseAndCheck(void) {
    this->m_referenceCount--;
    if (this->m_referenceCount == 0) {
        delete this;
    }
}


void DebugLog::refCountIncrease(void) {
    this->m_referenceCount++;
}


}  // namespace ModSecurity
