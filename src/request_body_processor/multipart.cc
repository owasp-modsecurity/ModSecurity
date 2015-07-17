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

namespace ModSecurity {
namespace RequestBodyProcessor {

Multipart::Multipart(std:: string header)
    : m_boundaryStartsWithWhiteSpace(false),
    m_boundaryIsQuoted(false),
    m_header(header) {
}


bool Multipart::init() {
    if (m_header.length() > 1024) {
        debug(4, "Multipart: Invalid boundary in Content-Type (length).");
        return false;
    }

    std::size_t boundary_pos = m_header.find("boundary");
    if (boundary_pos < 0) {
        if (boundary_pos > 0 && m_header.find("boundary", boundary_pos) > 0) {
            debug(4, "Multipart: Multiple boundary parameters in " \
                "Content-Type.");
            return false;
        }
    }

    std::string boundary = m_header.c_str() + boundary_pos;
    std::size_t semicolon_pos = boundary.find(";");
    if (semicolon_pos != std::string::npos
        && boundary.find(";", semicolon_pos + 1) != std::string::npos) {
        debug(4, "Multipart: Invalid boundary in Content-Type. (malformed). " \
            "Too many semicolons.");
        return false;
    }

    if (semicolon_pos < 0) {
        debug(4, "Multipart: Missing semicolon.");
    }

    if (boundary.at(8) != '=') {
        debug(4, "Multipart: Invalid boundary in Content-Type. (malformed). " \
            "Missing equals.");
        return false;
    }
#if 0
       Not checked.
       /* Check parameter name ends well. */
        if (b != (msr->mpd->boundary + 8)) {
            /* Check all characters between the end of the boundary
             * and the = character.
             */
            for (p = msr->mpd->boundary + 8; p < b; p++) {
                if (isspace(*p)) {
                    /* Flag for whitespace after parameter name. */
                    msr->mpd->flag_boundary_whitespace = 1;
                } else {
                    msr->mpd->flag_error = 1;
                    *error_msg = apr_psprintf(msr->mp, "Multipart: " \
                        "Invalid boundary in C-T (parameter name).");
                    return -1;
                }
            }
        }

        b++; /* Go over the = character. */
        len = strlen(b);

        /* Flag for whitespace before parameter value. */
        if (isspace(*b)) {
            msr->mpd->flag_boundary_whitespace = 1;
        }
#endif  // if 0
    if (boundary.at(8 + 1) == ' ') {
        m_boundaryStartsWithWhiteSpace = true;
        debug(4, "Multipart: Boundary starts with a white space");
    }

    if ((m_boundaryStartsWithWhiteSpace && boundary.at(8 + 2) == '"') ||
        (!m_boundaryStartsWithWhiteSpace && boundary.at(8 + 1) == '"')) {
        m_boundaryIsQuoted = true;
            debug(4, "Multipart: Boundary inside quotes");
    }

    if (m_boundaryIsQuoted && boundary.at(boundary.length()-1) != '"') {
        debug(4, "Multipart: Invalid boundary in Content-type (quote).");
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

    if (m_boundaryStartsWithWhiteSpace) {
        real_boundary_pos++;
    }

    if (m_boundaryIsQuoted) {
        real_boundary_pos++;
    }

    m_boundary = boundary.c_str() + real_boundary_pos;

    if (m_boundaryIsQuoted) {
        m_boundary.pop_back();
    }

    if (boundaryContainsOnlyValidCharacters() == false) {
        debug(4, "Multipart: Invalid boundary in Content-type " \
            "(invalid characters).");
        return false;
    }
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

bool Multipart::process(std::string data) {
    std::list<std::string> blobs;
    size_t start = data.find(m_boundary);
    size_t endl = 1;

    if (start != 0) {
      debug(4, "Multipart: Boundary was not the first thing.");
    }

    while (start != std::string::npos) {
        size_t end = data.find(m_boundary, start + m_boundary.length());
        if (end == std::string::npos) {
            start = end;
            continue;
        }
        std::string block = std::string(data, start + m_boundary.length() +
            + endl, end - (start + m_boundary.length() + endl) - endl);
        blobs.push_back(block);
        start = end;
    }

    double files_size = 0;
    for (std::string x : blobs) {
        MultipartBlob m(x, this);

        if (m.filename.empty() == false) {
            variables.emplace("FILES:" + m.name, m.filename);
            variables.emplace("FILES_NAMES:" + m.name, m.name);
            variables.emplace("FILES_SIZES:" + m.name,
                std::to_string(m.content.size()));
            variables.emplace("FILES_TMP_CONTENT:" + m.name, m.content);
            files_size = files_size + m.content.size();
        }
    }
    variables.emplace("FILES_COMBINED_SIZE", std::to_string(files_size));

    return true;
}


}  // namespace RequestBodyProcessor
}  // namespace ModSecurity
