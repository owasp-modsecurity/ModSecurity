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

#include "src/operators/validate_byte_range.h"

#include <cctype>
#include <cstring>
#include <string>
#include <memory>

#include "src/operators/operator.h"

namespace modsecurity {
namespace operators {

namespace {

std::string trimCopy(const std::string &value) {
    std::string::size_type start = 0;
    std::string::size_type end = value.size();

    while (start < end
        && std::isspace(static_cast<unsigned char>(value[start]))) {
        start++;
    }
    while (end > start
        && std::isspace(static_cast<unsigned char>(value[end - 1]))) {
        end--;
    }

    return value.substr(start, end - start);
}

bool parseStrictInt(const std::string &value, int *result, std::string *error) {
    const std::string trimmed = trimCopy(value);

    if (trimmed.empty()) {
        error->assign("Not able to convert '" + value + "' into a number");
        return false;
    }

    size_t pos = 0;

    try {
        *result = std::stoi(trimmed, &pos);
    } catch (...) {
        error->assign("Not able to convert '" + trimmed + "' into a number");
        return false;
    }

    if (pos != trimmed.size()) {
        error->assign("Not able to convert '" + trimmed + "' into a number");
        return false;
    }

    return true;
}

inline void allowByte(std::array<unsigned char, 32> *table, int value) {
    (*table)[value >> 3] = ((*table)[value >> 3]
        | (1U << static_cast<unsigned char>(value & 0x7)));
}

}  // namespace


bool ValidateByteRange::getRange(const std::string &rangeRepresentation,
    std::string *error) {
    return getRange(rangeRepresentation, &table, error);
}


bool ValidateByteRange::getRange(const std::string &rangeRepresentation,
    std::array<unsigned char, kTableSize> *targetTable,
    std::string *error) const {
    const std::string range = trimCopy(rangeRepresentation);
    const size_t pos = range.find_first_of("-");
    int start = 0;
    int end = 0;

    if (pos == std::string::npos) {
        if (parseStrictInt(range, &start, error) == false) {
            return false;
        }
        if ((start < 0) || (start > 255)) {
            error->assign("Invalid byte value: " +
                std::to_string(start));
            return false;
        }
        allowByte(targetTable, start);
        return true;
    }

    if (parseStrictInt(std::string(range, 0, pos), &start, error) == false) {
        return false;
    }

    if (parseStrictInt(std::string(range, pos + 1,
            range.length() - (pos + 1)), &end, error) == false) {
        return false;
    }

    if ((start < 0) || (start > 255)) {
        error->assign("Invalid range start value: " +
            std::to_string(start));
        return false;
    }
    if ((end < 0) || (end > 255)) {
       error->assign("Invalid range end value: " + std::to_string(end));
       return false;
    }
    if (start > end) {
       error->assign("Invalid range: " + std::to_string(start) + "-" +
           std::to_string(end));
       return false;
    }

    while (start <= end) {
        allowByte(targetTable, start);
        start++;
    }

    return true;
}


bool ValidateByteRange::init(const std::string &file,
    std::string *error) {
    std::array<unsigned char, kTableSize> parsedTable{};
    std::string::size_type pos = 0;

    table.fill('\0');

    while (true) {
        const std::string::size_type nextPos = m_param.find(',', pos);
        if (const std::string token = nextPos == std::string::npos
                ? m_param.substr(pos)
                : m_param.substr(pos, nextPos - pos);
            getRange(token, &parsedTable, error) == false) {
            /*
             * Keep byte 0 allowed on invalid parameters so callers that
             * continue after init() failure keep legacy behaviour.
             */
            table[0] = table[0] | 1U;
            return false;
        }

        if (nextPos == std::string::npos) {
            break;
        }

        pos = nextPos + 1;
    }

    table = parsedTable;
    return true;
}


bool ValidateByteRange::evaluate(Transaction *transaction, RuleWithActions *rule,
    const std::string &input, RuleMessage &ruleMessage) {
    bool ret = true;

    size_t count = 0;
    for (std::string::size_type i = 0; i < input.length(); i++) {
        int x = (unsigned char) input[i];
        if (!(table[x >> 3] & (1 << (x & 0x7)))) {
            // debug(9, "Value " + std::to_string(x) + " in " +
            //     input + " ouside range: " + param);
            logOffset(ruleMessage, i, 1);
            count++;
        }
    }

    ret = (count != 0);

    // debug(9, "Found %d byte(s) in %s outside range: %s.",
    //     count, var->name, rule->op_param);

    return ret;
}


}  // namespace operators
}  // namespace modsecurity
