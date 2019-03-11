/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2018 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/operators/yara.h"

#include <string>

#include "src/operators/operator.h"
#include "src/utils/system.h"

namespace modsecurity {
namespace operators {

bool Yara::init(const std::string &param2, std::string *error) {
#ifdef WITH_YARA

    std::string digit;
    std::string file;
    std::istream *iss;
    struct yara_chunk *chunk, *t;
    std::string err;

    auto pos = m_param.find_last_of(' ');
    if (pos == std::string::npos) {
        error->assign("Please use @yara with filename and value");
        return false;
    }
    digit.append(std::string(m_param, pos+1));
    file.append(std::string(m_param, 0, pos));
    try {
        m_threshold = std::stoi(digit);
    } catch (...) {
        error->assign("Expecting a digit, got: " + digit);
        return false;
    }

    std::string resource = utils::find_resource(file, param2, &err);
    iss = new std::ifstream(resource, std::ios::in);

    if (((std::ifstream *)iss)->is_open() == false) {
        error->assign("Failed to open file: " + m_param + ". " + err);
        delete iss;
        return false;
    }

// YARA 
    int result = yr_initialize();
    if (result != ERROR_SUCCESS) {
        error->assign("yr_initialize() FAILED!");
        return false;
    }

    for (std::string line; std::getline(*iss, line); ) {
       chunk = (struct yara_chunk *)calloc(1,
            sizeof(struct yara_chunk));

        chunk->data = strdup(line.c_str());
        chunk->next = NULL;

        if (m_head == NULL) {
            m_head = chunk;
        } else {
            t = m_head;

            while (t->next) {
                t = t->next;
            }

            t->next = chunk;
        }
    }

    delete iss;
    return true;
#else
    error->assign("@yara: YARA support was not enabled " \
        "during the compilation.");
    return false;
#endif
}

Yara::~Yara() {
    struct yara_chunk *c = m_head;
    while (c) {
        struct yara_chunk *t = c;
        free(c->data);
        c->data = NULL;
        c = c->next;
        free(t);
    }
    m_head = NULL;
}


bool Yara::evaluate(Transaction *t, const std::string &str) {
#ifdef WITH_YARA
    struct yara_chunk *chunk = m_head;
/*
    while (chunk != NULL) {
        int i = 0;
        if (i >= m_threshold) {
            return true;
        }
        chunk = chunk->next;
    }
 YARA 
    yr_finalize();
    return true;
*/

/**
* @todo Finalize Implementing the operator Yara.
*       Reference: https://yara.readthedocs.io/en/v3.9.0/capi.html
*/
    return true;

#endif
    /* No match. */
    return true;
}

}  // namespace operators
}  // namespace modsecurity
