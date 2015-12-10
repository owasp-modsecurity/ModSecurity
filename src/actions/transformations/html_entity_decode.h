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

#include <string>
#include <unordered_map>

#include "actions/action.h"
#include "actions/transformations/transformation.h"

#ifndef SRC_ACTIONS_TRANSFORMATIONS_HTML_ENTITY_DECODE_H_
#define SRC_ACTIONS_TRANSFORMATIONS_HTML_ENTITY_DECODE_H_

#ifdef __cplusplus
namespace modsecurity {
class Assay;

namespace actions {
namespace transformations {


class HtmlEntityDecodeInstantCache :
    public std::unordered_map<std::string, std::string> {
 public:
    static HtmlEntityDecodeInstantCache& getInstance() {
        static HtmlEntityDecodeInstantCache instance;
        return instance;
    }

    void cache(const std::string& value, const std::string& out) {
        emplace(value, out);
        if (size() > 100) {
            erase(begin());
        }
    }
 private:
    HtmlEntityDecodeInstantCache() {}
};


class HtmlEntityDecode : public Transformation {
 public:
    explicit HtmlEntityDecode(std::string action)
        : Transformation(action) { }

    std::string evaluate(std::string exp,
        Assay *assay) override;
};


}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity

#endif

#endif  // SRC_ACTIONS_TRANSFORMATIONS_HTML_ENTITY_DECODE_H_
