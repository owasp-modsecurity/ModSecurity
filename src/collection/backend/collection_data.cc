/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2023 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/collection/backend/collection_data.h"


namespace modsecurity {
namespace collection {
namespace backend {


bool CollectionData::isExpired() const {
    if (m_hasExpiryTime == false) {
        return false;
    }
    auto now = std::chrono::system_clock::now();
    return (now >= m_expiryTime);
}


void CollectionData::setExpiry(int32_t seconds_until_expiry) {
    m_expiryTime = std::chrono::system_clock::now() + std::chrono::seconds(seconds_until_expiry);
    m_hasExpiryTime = true;
}


}  // namespace backend
}  // namespace collection
}  // namespace modsecurity
