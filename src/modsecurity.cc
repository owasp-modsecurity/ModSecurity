/**
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

#include <ctime>
#include <iostream>

#include "modsecurity/modsecurity.h"
#include "src/unique_id.h"


namespace ModSecurity {

/**
 * @name    ModSecurity
 * @brief   Initilizes ModSecurity CPP API
 *
 * ModSecurity initializer. Although it is not really doing anything
 * in this version, further versions of this API may demands this to
 * be executed.
 *
 * Example Usage:
 * @code
 *
 * using ModSecurity::ModSecurity;
 *
 * ModSecurity *msc = new ModSecurity();
 *
 * @endcode
 */
ModSecurity::ModSecurity() {
    UniqueId::uniqueId();
}


/**
 * @name    msc_init
 * @brief   Initilizes ModSecurity C API
 *
 * ModSecurity initializer. Although it is not really doing anything
 * in this version, further versions of this API may demands this to
 * be executed.
 *
 * Example Usage:
 * @code
 *
 * ModSecurity msc = msc_init();
 *
 * @endcode
 */
extern "C" ModSecurity *msc_init() {
    ModSecurity *modsec = new ModSecurity();

    return modsec;
}



}  // namespace ModSecurity
