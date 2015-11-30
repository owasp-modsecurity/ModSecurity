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

#include "request_body_processor/multipart.h"

#include <list>
#include <iostream>
#include <string>

#include "request_body_processor/multipart_blob.h"

namespace modsecurity {
namespace RequestBodyProcessor {

Multipart::Multipart(std:: string header, Assay *assay)
    : crlf(false),
    containsDataAfter(false),
    containsDataBefore(false),
    lf(false),
    missingSemicolon(false),
    invalidQuote(false),
    boundaryStartsWithWhiteSpace(false),
    boundaryIsQuoted(false),
    m_assay(assay),
    m_header(header) {
}


bool Multipart::init() {
    if (m_header.length() > 1024) {
#ifndef NO_LOGS
        debug(4, "Multipart: Invalid boundary in Content-Type (length).");
#endif
        return false;
    }

    std::size_t boundary_pos = m_header.find("boundary");
    if (boundary_pos != std::string::npos &&
        m_header.find("boundary", boundary_pos + 1) != std::string::npos) {
#ifndef NO_LOGS
        debug(4, "Multipart: Multiple boundary parameters in " \
            "Content-Type.");
#endif
        return false;
    }

    std::string boundary = m_header.c_str() + boundary_pos;
    std::size_t semicolon_pos = boundary.find(";");
    if (semicolon_pos != std::string::npos
        && boundary.find(";", semicolon_pos + 1) != std::string::npos) {
#ifndef NO_LOGS
        debug(4, "Multipart: Invalid boundary in Content-Type. (malformed). " \
            "Too many semicolons.");
#endif
        return false;
    }

    if (semicolon_pos == std::string::npos) {
#ifndef NO_LOGS
        debug(4, "Multipart: Missing semicolon.");
#endif
        this->missingSemicolon = true;
    }

    if (boundary.at(8) != '=') {
#ifndef NO_LOGS
        debug(4, "Multipart: Invalid boundary in Content-Type. (malformed). " \
            "Missing equals.");
#endif
        return false;
    }

    if (boundary.at(8 + 1) == ' ') {
        boundaryStartsWithWhiteSpace = true;
#ifndef NO_LOGS
        debug(4, "Multipart: Boundary starts with a white space");
#endif
    }

    if ((boundaryStartsWithWhiteSpace && boundary.at(8 + 2) == '"') ||
        (!boundaryStartsWithWhiteSpace && boundary.at(8 + 1) == '"')) {
        boundaryIsQuoted = true;
#ifndef NO_LOGS
            debug(4, "Multipart: Boundary inside quotes");
#endif
    }

    if (boundaryIsQuoted && boundary.at(boundary.length()-1) != '"') {
#ifndef NO_LOGS
        debug(4, "Multipart: Invalid boundary in Content-type (quote).");
#endif
        return false;
    }

#if 0
        Not checking
        /* Case-insensitive test for the string "boundary" in the boundary. */
        if (count_boundary_params(msr->mp, msr->mpd->boundary) != 0) {
            msr->mpd->flag_error = 1;
            *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid boundary " \
                "in C-T (content).");
            return -1;
        }
#endif  //  if 0

    int real_boundary_pos = 9;

    if (boundaryStartsWithWhiteSpace) {
        real_boundary_pos++;
    }

    if (boundaryIsQuoted) {
        real_boundary_pos++;
    }

    m_boundary = boundary.c_str() + real_boundary_pos;

    if (boundaryIsQuoted) {
        m_boundary.pop_back();
    }

    if (boundaryContainsOnlyValidCharacters() == false) {
#ifndef NO_LOGS
        debug(4, "Multipart: Invalid boundary in Content-type " \
            "(invalid characters).");
#endif
        return false;
    }

    return true;
}


bool Multipart::boundaryContainsOnlyValidCharacters() {
    if (m_boundary.empty()) {
        return false;
    }

    for (int i = 0; i < m_boundary.length(); i++) {
        int c = m_boundary.at(i);

        /* Control characters and space not allowed. */
        /* Non-ASCII characters not allowed. */
        if (c < 32 || c > 126) {
            return false;
        }

        switch (c) {
            /* Special characters not allowed. */
            case '(' :
            case ')' :
            case '<' :
            case '>' :
            case '@' :
            case ',' :
            case ';' :
            case ':' :
            case '\\' :
            case '"' :
            case '/' :
            case '[' :
            case ']' :
            case '?' :
            case '=' :
                return false;
                break;
            default :
                break;
        }
    }

    return true;
}

void Multipart::checkForCrlfLf(const std::string &data) {
    size_t start = 0;
    size_t pos = 0;

    pos = data.find("\n", start);
    while (pos != std::string::npos) {
        if (pos > 1 && data.at(pos-1) == '\r') {
            this->crlf = true;
        } else {
            this->lf = true;
        }
        pos = data.find("\n", pos + 1);
    }
}

bool Multipart::process(std::string data) {
    std::list<std::string> blobs;
    size_t start = data.find(m_boundary);
    size_t endl = 1;
    size_t lastValidBoundary = 0;
    size_t firstValidBoundary = start;
    double files_size = 0;

    if (start != 0) {
#ifndef NO_LOGS
        debug(4, "Multipart: Boundary was not the first thing.");
#endif
        this->containsDataBefore = true;
    }
    while (start != std::string::npos) {
        size_t end = data.find(m_boundary, start + m_boundary.length());
        if (end == std::string::npos) {
            start = end;
            continue;
        }
        std::string block = std::string(data, start + m_boundary.length() +
            + endl, end - (start + m_boundary.length() + endl) - endl);

        checkForCrlfLf(block);

        //  if (this->crlf) {
        //      block.erase(0, 1);
        //  }

        blobs.push_back(block);
        lastValidBoundary = end;
        start = end;
    }

    size_t lastPiece = m_boundary.length() + lastValidBoundary \
        + firstValidBoundary + 2;
    if (this->crlf) {
        lastPiece = lastPiece + 2;
    } else {
        lastPiece = lastPiece + 1;
    }

    if (data.length() > lastPiece) {
        this->containsDataAfter = true;
    }

    std::string filename("");
    std::string name("");
    int i = 0;
    for (std::string x : blobs) {
        i++;
#ifndef NO_LOGS
        debug(5, "Multipart: Inspecting blob: " + std::to_string(i));
#endif
        MultipartBlob m(x, this);

        if (m.name.empty() == false) {
            name = m.name;
        } else {
            name = "no-name-" + std::to_string(i);
        }

        if (m.filename.empty() == false) {
            filename = m.filename;
        } else {
            filename = "no-file-name-" + std::to_string(i);
        }

        variables.emplace("FILES:" + name, filename);
        variables.emplace("FILES_NAMES:" + name, name);
        variables.emplace("FILES_SIZES:" + name,
            std::to_string(m.content.size()));
#ifndef NO_LOGS
        debug(5, "Multipart: Saving FILES_TMP_CONTENT:" + name + " variable.");
#endif
        variables.emplace("FILES_TMP_CONTENT:" + name, m.content);
        files_size = files_size + m.content.size();
        if (m.invalidQuote) {
#ifndef NO_LOGS
            debug(4, "Multipart: Found invalid quoting.");
#endif
            this->invalidQuote = true;
        }
    }
    if (filename.empty() == false) {
        variables.emplace("MULTIPART_FILENAME", filename);
    }
    if (name.empty() == false) {
        variables.emplace("MULTIPART_NAME", name);
    }
    variables.emplace("FILES_COMBINED_SIZE", std::to_string(files_size));

    return true;
}


}  // namespace RequestBodyProcessor
}  // namespace ModSecurity
