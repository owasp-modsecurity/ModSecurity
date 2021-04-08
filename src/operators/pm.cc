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
#ifndef WITH_HS
#include "src/utils/acmp.h"
#endif
#include "src/utils/string.h"

namespace modsecurity {
namespace operators {

Pm::~Pm() {
#ifdef WITH_HS
    m_hs = NULL;
#else
    acmp_node_t *root = m_p->root_node;

    cleanup(root);

    free(m_p);
    m_p = NULL;
#endif
#ifdef MODSEC_MUTEX_ON_PM
    pthread_mutex_destroy(&m_lock);
#endif
}

#ifndef WITH_HS
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
#endif

#ifndef WITH_HS
void Pm::postOrderTraversal(acmp_btree_node_t *node) {
    if (node == NULL) {
        return;
    }

    postOrderTraversal(node->right);
    postOrderTraversal(node->left);

    free(node);
}
#endif


bool Pm::evaluate(Transaction *transaction, RuleWithActions *rule,
    const std::string &input, std::shared_ptr<RuleMessage> ruleMessage) {
    int rc = 0;
    const char *match = NULL;
#ifdef WITH_HS
#ifdef MODSEC_MUTEX_ON_PM
    pthread_mutex_lock(&m_lock);
#endif
    rc = m_hs->search(input.c_str(), input.length(), &match);
#ifdef MODSEC_MUTEX_ON_PM
    pthread_mutex_unlock(&m_lock);
#endif
#else
    ACMPT pt;
    pt.parser = m_p;
    pt.ptr = NULL;
#ifdef MODSEC_MUTEX_ON_PM
    pthread_mutex_lock(&m_lock);
#endif
    rc = acmp_process_quick(&pt, &match, input.c_str(), input.length());
#ifdef MODSEC_MUTEX_ON_PM
    pthread_mutex_unlock(&m_lock);
#endif
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

static
char *parse_pm_content(const char *op_parm, unsigned short int op_len, const char **error_msg)  {
    char *parm = NULL;
    char *content;
    unsigned short int offset = 0;
//    char converted = 0;
    int i, x;
    unsigned char bin = 0, esc = 0, bin_offset = 0;
    unsigned char c;
    unsigned char bin_parm[3] = { 0 };
    char *processed = NULL;

    content = strdup(op_parm);

    if (content == NULL) {
        *error_msg = std::string("Error allocating memory for pattern matching content.").c_str();
        return NULL;
    }

    while (offset < op_len && (content[offset] == ' ' || content[offset] == '\t')) {
        offset++;
    };

    op_len = strlen(content);

    if (content[offset] == '\"' && content[op_len-1] == '\"') {
        parm = strdup(content + offset + 1);
        if (parm  == NULL) {
            *error_msg = std::string("Error allocating memory for pattern matching content.").c_str();
            free(content);
            content = NULL;
            return NULL;
        }
        parm[op_len - offset - 2] = '\0';
    } else {
        parm = strdup(content + offset);
        if (parm == NULL) {
            free(content);
            content = NULL;
            *error_msg = std::string("Error allocating memory for pattern matching content.").c_str();
            return NULL;
        }
    }

    free(content);
    content = NULL;

    op_len = strlen(parm);

    if (op_len == 0)   {
        *error_msg = "Content length is 0.";
        free(parm);
        return NULL;
    }

    for (i = 0, x = 0; i < op_len; i++) {
        if (parm[i] == '|') {
            if (bin) {
                bin = 0;
            } else {
                bin = 1;
            }
        } else if(!esc && parm[i] == '\\') {
            esc = 1;
        } else {
            if (bin) {
                if (parm[i] == 0 || parm[i] == 1 || parm[i] == 2 ||
                    parm[i] == 3 || parm[i] == 4 || parm[i] == 5 ||
                    parm[i] == 6 || parm[i] == 7 || parm[i] == 8 ||
                    parm[i] == 9 ||
                    parm[i] == 'A' || parm[i] == 'a' ||
                    parm[i] == 'B' || parm[i] == 'b' ||
                    parm[i] == 'C' || parm[i] == 'c' ||
                    parm[i] == 'D' || parm[i] == 'd' ||
                    parm[i] == 'E' || parm[i] == 'e' ||
                    parm[i] == 'F' || parm[i] == 'f')
                {
                    bin_parm[bin_offset] = (char)parm[i];
                    bin_offset++;
                    if (bin_offset == 2) {
                        c = strtol((char *)bin_parm, (char **) NULL, 16) & 0xFF;
                        bin_offset = 0;
                        parm[x] = c;
                        x++;
                        //converted = 1;
                    }
                } else if (parm[i] == ' ') {
                }
            } else if (esc) {
                if (parm[i] == ':' ||
                        parm[i] == ';' ||
                        parm[i] == '\\' ||
                        parm[i] == '\"')
                {
                    parm[x] = parm[i];
                    x++;
                } else {
                    *error_msg = std::string("Unsupported escape sequence.").c_str();
                    free(parm);
                    return NULL;
                }
                esc = 0;
                //converted = 1;
            } else {
                parm[x] = parm[i];
                x++;
            }
        }
    }

#if 0
    if (converted) {
        op_len = x;
    }
#endif

    //processed = memcpy(processed, parm, op_len);
    processed = strdup(parm);
    free(parm);
    parm = NULL;

    if (processed == NULL) {
        *error_msg = std::string("Error allocating memory for pattern matching content.").c_str();
        return NULL;
    }

    return processed;
}

bool Pm::init(const std::string &file, std::string *error) {
    std::vector<std::string> vec;
    std::istringstream *iss;
    const char *err = NULL;

#ifdef MODSEC_MUTEX_ON_PM
    pthread_mutex_init(&m_lock, NULL);
#endif
    char *content = parse_pm_content(m_param.c_str(), m_param.length(), &err);
    if (content == NULL) {
        iss = new std::istringstream(m_param);
    } else {
        iss = new std::istringstream(content);
    }

    std::copy(std::istream_iterator<std::string>(*iss),
        std::istream_iterator<std::string>(),
        back_inserter(vec));

    for (auto &a : vec) {
#ifdef WITH_HS
        m_hs->addPattern(a.c_str(), a.length());
    }
    if (m_hs->compile(error) == false) {
        if (content) {
            free(content);
            content = NULL;
        }
        delete iss;
        return false;
    }
#else
        acmp_add_pattern(m_p, a.c_str(), NULL, NULL, a.length());
    }

    while (m_p->is_failtree_done == 0) {
        acmp_prepare(m_p);
    }
#endif

    if (content) {
        free(content);
        content = NULL;
    }

    delete iss;
    return true;
}


}  // namespace operators
}  // namespace modsecurity
