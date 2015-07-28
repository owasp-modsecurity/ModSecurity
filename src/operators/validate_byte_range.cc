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

#include "operators/validate_byte_range.h"

#include <string>

#include "operators/operator.h"

namespace ModSecurity {
namespace operators {

bool ValidateByteRange::getRange(const std::string &rangeRepresentation,
    const char **error) {
    size_t pos = param.find_first_of("-");
    int start;
    int end;

    if (pos == std::string::npos) {
        try {
            start = std::stoi(rangeRepresentation);
        } catch(...) {
            *error = ("Not able to convert '" + rangeRepresentation + "' into a number").c_str();
            return false;
        }
        table[start >> 3] = (table[start >> 3] | (1 << (start & 0x7)));
        return true;
    }

    try {
        start = std::stoi(std::string(rangeRepresentation, 0, pos));
    } catch (...) {
        *error = ("Not able to convert '" + std::string(rangeRepresentation, 0, pos) + "' into a number").c_str();
        return false;
    }

    try {
        end = std::stoi(std::string(rangeRepresentation, pos + 1,
            rangeRepresentation.length() - (pos + 1)));
    } catch (...) {
        *error = ("Not able to convert '" + std::string(rangeRepresentation, pos + 1,
            rangeRepresentation.length() - (pos + 1)) + "' into a number").c_str();
        return false;
    }

    if ((start < 0) || (start > 255)) {
        *error = ("Invalid range start value: " + std::to_string(start)).c_str();
        return false;
    }
    if ((end < 0) || (end > 255)) {
       *error = ("Invalid range end value: " + std::to_string(end)).c_str();
       return false;
    }
    if (start > end) {
       *error = ("Invalid range: " + std::to_string(start) + "-" +
           std::to_string(end)).c_str();
       return false;
    }

    while(start <= end) {
        table[start >> 3] = (table[start >> 3] | (1 << (start & 0x7)));
        start++;
    }

    return true;
}


bool ValidateByteRange::init(const char **error) {
    size_t pos = param.find_first_of(",");

    if (pos == std::string::npos) {
        getRange(param, error);
    }

    while (pos != std::string::npos) {
        size_t next_pos = param.find_first_of(",", pos + 1);
        if (next_pos == std::string::npos) {
            getRange(std::string(param, pos + 1, param.length() - (pos + 1)), error);
        } else {
            getRange(std::string(param, pos + 1, next_pos), error);
        }
        pos = next_pos;
    }
}



bool ValidateByteRange::evaluate(Assay *assay, const std::string &input) {
    bool ret = true;

    size_t count = 0;
    for(int i = 0; i < input.length(); i++) {
        int x = input.at(i);
        if (!(table[x >> 3] & (1 << (x & 0x7)))) {
            //debug(9, "Value " + std::to_string(x) + " in " + input + " ouside range: " + param);
            count++;
        }
    }

    ret = (count != 0);
    //if (count == 0) return 0;

    //debug(9, "Found %d byte(s) in %s outside range: %s.",
        //count, var->name, rule->op_param);

    if (negation) {
        return !ret;
    }

    return ret;
}


}  // namespace operators
}  // namespace ModSecurity
