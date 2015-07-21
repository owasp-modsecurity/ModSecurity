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


#ifndef SRC_REQUEST_BODY_PROCESSOR_MULTIPART_H_
#define SRC_REQUEST_BODY_PROCESSOR_MULTIPART_H_

#include "modsecurity/assay.h"

namespace ModSecurity {
namespace RequestBodyProcessor {

class Multipart {
 public:
    explicit Multipart(std::string header);
    bool init();

    bool boundaryContainsOnlyValidCharacters();
    bool conuntBoundaryParameters();
    bool process(std::string data);
    void checkForCrlfLf(const std::string &blob);

    ModSecurityStringVariables variables;
    bool crlf;
    bool lf;
 private:
    void debug(int a, std::string str) {
        std::cout << "Debug: " << str << std::endl;
    }
    std::string m_boundary;
    std::string m_header;

    bool m_boundaryStartsWithWhiteSpace = false;
    bool m_boundaryIsQuoted = false;
};

}  // namespace RequestBodyProcessor
}  // namespace ModSecurity

#endif  // SRC_REQUEST_BODY_PROCESSOR_MULTIPART_H_
