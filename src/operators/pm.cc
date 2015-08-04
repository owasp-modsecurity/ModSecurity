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

#include "operators/operator.h"

namespace ModSecurity {
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


bool Pm::evaluate(Assay *assay, const std::string &input) {
    int rc = 0;
    ACMPT pt;
    pt.parser = m_p;
    pt.ptr = NULL;
    const char *match = NULL;

    rc = acmp_process_quick(&pt, &match, input.c_str(), input.length());
    if (rc == 1) {
        // save into tx, etc...
    }

    return rc == 1;
}


bool Pm::init(const char **error) {
    std::vector<std::string> vec;

    std::istringstream iss(param);
    std::copy(std::istream_iterator<std::string>(iss),
        std::istream_iterator<std::string>(),
        back_inserter(vec));

    for (auto &a : vec) {
        acmp_add_pattern(m_p, a.c_str(), NULL, NULL, a.length());
    }


    acmp_prepare(m_p);
}


}  // namespace operators
}  // namespace ModSecurity
