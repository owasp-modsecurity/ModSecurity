/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2024 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#ifndef SRC_REQUEST_BODY_PROCESSOR_JSON_ADAPTER_H_
#define SRC_REQUEST_BODY_PROCESSOR_JSON_ADAPTER_H_

#include <string>

#include "src/request_body_processor/json_backend.h"

namespace modsecurity {
namespace RequestBodyProcessor {

class JSONAdapter {
 public:
    JsonParseResult parse(const std::string &input, JsonEventSink *sink,
        const JsonBackendParseOptions &options = JsonBackendParseOptions()) const;
};

}  // namespace RequestBodyProcessor
}  // namespace modsecurity

#endif  // SRC_REQUEST_BODY_PROCESSOR_JSON_ADAPTER_H_
