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
#include <iostream>

#include "request_body_processor/multipart.h"

#ifndef SRC_REQUEST_BODY_PROCESSOR_MULTIPART_BLOB_H_
#define SRC_REQUEST_BODY_PROCESSOR_MULTIPART_BLOB_H_

namespace ModSecurity {
namespace RequestBodyProcessor {

class MultipartBlob {
 public:
    explicit MultipartBlob(const std::string &blob, Multipart *parent);

    bool processContent();
    bool processContentDispositionLine(const std::string &dispositionLine);
    bool processContentTypeLine(const std::string &contentTypeLine);

    void debug(int a, std::string str) {
        std::cout << "Debug: " << str << std::endl;
    }

    bool invalidQuote;
    std::string name;
    std::string filename;
    std::string contentType;
    std::string content;
 private:
    const std::string m_blob;
    Multipart *m_parent;
};

}  // namespace RequestBodyProcessor
}  // namespace ModSecurity

#endif  // SRC_REQUEST_BODY_PROCESSOR_MULTIPART_BLOB_H_
