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

#include <string>
#include <memory>
#include <charconv>

#include "src/operators/operator.h"

namespace modsecurity {
namespace operators {

bool ValidateByteRange::getRange(const std::string &rangeRepresentation,
    std::string *error) {
    size_t pos = rangeRepresentation.find_first_of("-");
    int start;
    int end;

    if (pos == std::string::npos) {  
        const auto conv_res = std::from_chars(rangeRepresentation.data(), rangeRepresentation.data() + rangeRepresentation.size(), start);
        if (conv_res.ec == std::errc::invalid_argument) {
            error->assign("Not able to convert '" + rangeRepresentation +
                "' into a number");
            return false;
        }

        table[start >> 3] = (table[start >> 3] | (1 << (start & 0x7)));
        return true;
    }

    std::string to_be_converted(rangeRepresentation, 0, pos);
    const auto conv_res2 = std::from_chars(to_be_converted.data(), to_be_converted.data() + to_be_converted.size(), start);
    if (conv_res2.ec == std::errc::invalid_argument) {
        error->assign("Not able to convert '" + to_be_converted + "' into a number");
        return false;
    }

    std::string to_be_converted2(rangeRepresentation, pos + 1, rangeRepresentation.length() - (pos + 1));
    const auto conv_res3 = std::from_chars(to_be_converted2.data(), to_be_converted2.data() + to_be_converted2.size(), end);
    if (conv_res3.ec == std::errc::invalid_argument) {
        error->assign("Not able to convert '" + to_be_converted2 + "' into a number");
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
        table[start >> 3] = (table[start >> 3] | (1 << (start & 0x7)));
        start++;
    }

    return true;
}


bool ValidateByteRange::init(const std::string &file,
    std::string *error) {
    size_t pos = m_param.find_first_of(",");

    if (pos == std::string::npos) {
        getRange(m_param, error);
    } else {
        getRange(std::string(m_param, 0, pos), error);
    }

    while (pos != std::string::npos) {
        size_t next_pos = m_param.find_first_of(",", pos + 1);

        if (next_pos == std::string::npos) {
            getRange(std::string(m_param, pos + 1, m_param.length() -
                (pos + 1)), error);
        } else {
            getRange(std::string(m_param, pos + 1, next_pos - (pos + 1)), error);
        }
        pos = next_pos;
    }

    return true;
}


bool ValidateByteRange::evaluate(Transaction *transaction, RuleWithActions *rule,
    const std::string &input, std::shared_ptr<RuleMessage> ruleMessage) {
    bool ret = true;

    size_t count = 0;
    for (int i = 0; i < input.length(); i++) {
        int x = (unsigned char) input.at(i);
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
