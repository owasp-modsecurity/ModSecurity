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

#include "operators/pm.h"

#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <list>

#include "operators/operator.h"
#include "utils/acmp.h"

namespace modsecurity {
namespace operators {

Pm::~Pm() {
    postOrderTraversal(m_p->root_node->btree);

    free(m_p->root_node);
    m_p->root_node = NULL;
    if (m_p) {
        free(m_p);
        m_p = NULL;
    }
}


void Pm::postOrderTraversal(acmp_btree_node_t *node) {
    if (node == NULL) {
        return;
    }

    postOrderTraversal(node->left);
    postOrderTraversal(node->right);

    if (node->node->text) {
        free(node->node->text);
        node->node->text = NULL;
    }

    free(node->node);
    node->node = NULL;
    free(node);
    node = NULL;
}

void Pm::replaceAll(std::string str, const std::string& from,
    const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        size_t end_pos = start_pos + from.length();
        str.replace(start_pos, end_pos, to);
        start_pos += to.length();
    }
}

bool Pm::evaluate(Transaction *transaction, const std::string &input) {
    int rc = 0;
    ACMPT pt;
    pt.parser = m_p;
    pt.ptr = NULL;
    const char *match = NULL;

    rc = acmp_process_quick(&pt, &match, input.c_str(), input.length());
    if (rc == 1) {
        transaction->m_matched.push_back(std::string(match));
    }

    return rc == 1;
}


bool Pm::init(const std::string &file, const char **error) {
    std::vector<std::string> vec;
    std::istringstream *iss;

    replaceAll(param, "\\", "\\\\");

    char *content = parse_pm_content(param.c_str(), param.length(), error);
    if (content == NULL) {
        iss = new std::istringstream(param);
    } else {
        iss = new std::istringstream(content);
    }

    std::copy(std::istream_iterator<std::string>(*iss),
        std::istream_iterator<std::string>(),
        back_inserter(vec));

    for (auto &a : vec) {
        acmp_add_pattern(m_p, a.c_str(), NULL, NULL, a.length());
    }

    acmp_prepare(m_p);

    if (content) {
        free(content);
        content = NULL;
    }

    delete iss;

    return true;
}


}  // namespace operators
}  // namespace modsecurity
