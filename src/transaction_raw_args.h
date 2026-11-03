/*
 * ModSecurity, http://www.modsecurity.org/
 */

#ifndef SRC_TRANSACTION_RAW_ARGS_H_
#define SRC_TRANSACTION_RAW_ARGS_H_

#include <string>
#include <string_view>

#include "modsecurity/transaction.h"

namespace modsecurity {

inline void addRawArgument(Transaction *t, std::string_view orig,
    std::string_view key, std::string_view value, size_t offset) {
    offset = offset + key.size() + 1;
    const std::string keyStr(key);
    const std::string valueStr(value);
    t->m_variableArgsRaw.set(keyStr, valueStr, offset);

    if (orig == "GET") {
        t->m_variableArgsGetRaw.set(keyStr, valueStr, offset);
    } else if (orig == "POST") {
        t->m_variableArgsPostRaw.set(keyStr, valueStr, offset);
    }
}

}  // namespace modsecurity

#endif  // SRC_TRANSACTION_RAW_ARGS_H_
