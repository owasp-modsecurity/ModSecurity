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

#include "request_body_processor/multipart_blob.h"

#include <list>
#include <iostream>
#include <string>


namespace modsecurity {
namespace RequestBodyProcessor {

MultipartBlob::MultipartBlob(const std::string &blob, Multipart *parent)
    : m_blob(blob),
    invalidQuote(false),
    m_parent(parent) {
    processContent();
}


bool MultipartBlob::processContent() {
    size_t end = 0;
    size_t offset = 0;

    end = m_blob.find("\n", offset);
    if (end == std::string::npos) {
#ifndef NO_LOGS
        debug(4, "Missing end of line");
#endif
        return false;
    }
    std::string firstLine = std::string(m_blob, offset, end);
    offset = end + 1;
    end = m_blob.find("\n", offset);
    if (end == std::string::npos) {
#ifndef NO_LOGS
        debug(4, "Missing end of line");
#endif
        return false;
    }
    std::string secondLine = std::string(m_blob, offset, end - offset);

    bool dispositionLine = processContentDispositionLine(firstLine);
    if (dispositionLine == false) {
        return false;
    }

    bool contentTypeLine = processContentTypeLine(secondLine);
    if (contentTypeLine == false) {
        return false;
    }

    offset = end + 1;
    if (contentType.empty() == false) {
        end = m_blob.find_first_of("\n", offset);
        if (end == std::string::npos) {
#ifndef NO_LOGS
            debug(4, "Missing end of line");
#endif
            return false;
        }
        offset = end + 1;
    }
    content = std::string(m_blob, offset, m_blob.length() - offset + 1);

    return true;
}


bool MultipartBlob::processContentTypeLine(
    const std::string &contentTypeLine) {
    size_t contentTypeKeyLength = 14;

    if (contentTypeLine.length() <= contentTypeKeyLength) {
        return true;
    }

    contentType = std::string(contentTypeLine, contentTypeKeyLength,
        contentTypeLine.length() - contentTypeKeyLength);
    return true;
}


bool MultipartBlob::processContentDispositionLine(
    const std::string &dispositionLine) {
    size_t offset;

    if (dispositionLine.size() < 30 ||
        dispositionLine.compare(21, 9, "form-data") != 0) {
#ifndef NO_LOGS
        debug(4, "Multipart: Content-Disposition is unknown");
#endif
        return false;
    }

    // Find name=
    offset = dispositionLine.find("name=");
    if (offset != std::string::npos) {
        size_t invalidQuote = dispositionLine.find("\'", offset);
        if (invalidQuote != std::string::npos) {
            this->invalidQuote = true;
        }
        offset = offset + 5 /* name= */ + 1 /* " */;
        size_t end = dispositionLine.find("\"", offset);
        if (end != std::string::npos) {
            name = std::string(dispositionLine, offset, end - offset);
        }
    }

    // Find filename=
    offset = dispositionLine.find("filename=");
    if (offset != std::string::npos) {
        size_t invalidQuote = dispositionLine.find("\'", offset);
        if (invalidQuote != std::string::npos) {
            this->invalidQuote = true;
        }
        offset = offset + 9 /* filename= */ + 1 /* " */;
        size_t end = dispositionLine.find("\"", offset);
        if (end != std::string::npos) {
            filename = std::string(dispositionLine, offset, end - offset);
        }
    }

    return true;
}


}  // namespace RequestBodyProcessor
}  // namespace modsecurity
