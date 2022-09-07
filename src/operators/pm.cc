/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "src/operators/pm.h"

#include <string.h>

#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <list>
#include <memory>

#include "src/operators/operator.h"
#include "src/utils/acmp.h"
#include "src/utils/string.h"

namespace modsecurity {
namespace operators {

Pm::~Pm() {
    acmp_node_t *root = m_p->root_node;

    cleanup(root);

    free(m_p);
    m_p = NULL;
#ifdef MODSEC_MUTEX_ON_PM
    pthread_mutex_destroy(&m_lock);
#endif
}


void Pm::cleanup(acmp_node_t *n) {
    if (n == NULL) {
        return;
    }

    cleanup(n->sibling);
    cleanup(n->child);

    postOrderTraversal(n->btree);

    if (n->text && strlen(n->text) > 0) {
        free(n->text);
        n->text = NULL;
    }

    if (n->pattern && strlen(n->pattern) > 0) {
        free(n->pattern);
        n->pattern = NULL;
    }

    free(n);
}


void Pm::postOrderTraversal(acmp_btree_node_t *node) {
    if (node == NULL) {
        return;
    }

    postOrderTraversal(node->right);
    postOrderTraversal(node->left);

    free(node);
}


bool Pm::evaluate(Transaction *transaction, RuleWithActions *rule,
    const std::string &input, std::shared_ptr<RuleMessage> ruleMessage) {
    int rc;
    ACMPT pt;
    pt.parser = m_p;
    pt.ptr = NULL;
    const char *match = NULL;
#ifdef MODSEC_MUTEX_ON_PM
    pthread_mutex_lock(&m_lock);
#endif
    rc = acmp_process_quick(&pt, &match, input.c_str(), input.length());
#ifdef MODSEC_MUTEX_ON_PM
    pthread_mutex_unlock(&m_lock);
#endif

    if (rc >= 0 && transaction) {
        std::string match_(match?match:"");
        logOffset(ruleMessage, rc - match_.size() + 1, match_.size());
        transaction->m_matched.push_back(match_);

        if (rule && rule->hasCaptureAction()) {
            transaction->m_collections.m_tx_collection->storeOrUpdateFirst("0",
                match_);
            ms_dbg_a(transaction, 7, "Added pm match TX.0: " + \
                match_);
        }
    }

    return rc >= 0;
}


bool Pm::init(const std::string &file, std::string *error) {
    std::vector<std::string> vec;
    std::istringstream *iss;
    const char *err = NULL;
    char *content = NULL;
    char pm_custom_separator;
    std::string tmp;
    int start_separator,trail_separator;

#ifdef MODSEC_MUTEX_ON_PM
    pthread_mutex_init(&m_lock, NULL);
#endif

    // Checking if Pm is required based on  a custom separator
    // Example of the expected syntax: "@Pm PmCustomSeparator:| |htaccess|This is an error string| hi|"
    // Emulating PmFromFile logic, it bypasses Snort-Suricata format parsing. It permits the match
    // of the '|' symbol for piped commands matching or the usage of the pipe symbol as a separator.
    // In order to provide the separator, a non-space character is required right after "PmCustomSeparator:".
    if (m_param.compare(0, 18, "PmCustomSeparator:") == 0
        && m_param.compare(18, 1, " ") != 0 
        && m_param.size() > 19) {
        pm_custom_separator=m_param.at(18);

        start_separator=m_param.find(pm_custom_separator, 19);
        trail_separator=m_param.rfind(pm_custom_separator);
        if (start_separator == std::string::npos || trail_separator == std::string::npos) {
            error->assign("Starting or trailing custom separator not found");
            return false;
        }
        // Erase from the final separator up to the end
        m_param.erase(trail_separator,m_param.size()-trail_separator);
        // Erase up to the first separator
        m_param.erase(0,start_separator+1);

        iss = new std::istringstream(m_param);
        while (std::getline(*iss, tmp, pm_custom_separator)) {
            acmp_add_pattern(m_p, tmp.c_str(), NULL, NULL, tmp.length());
        }
    // Usual Pm behaviour with Snort-Suricata format parsing if 'PmCustomSeparator:' is not found
    } else {
        content = parse_pm_content(m_param.c_str(), m_param.length(), &err);
        if (content == NULL) {
            iss = new std::istringstream(m_param);
        } else {
            iss = new std::istringstream(content);
        }

        std::copy(std::istream_iterator<std::string>(*iss),
            std::istream_iterator<std::string>(),
            back_inserter(vec));

        for (auto &a : vec) {
            acmp_add_pattern(m_p, a.c_str(), NULL, NULL, a.length());
        }
    }

    while (m_p->is_failtree_done == 0) {
        acmp_prepare(m_p);
    }

    if (content) {
        free(content);
        content = NULL;
    }

    delete iss;

    return true;
}


}  // namespace operators
}  // namespace modsecurity
