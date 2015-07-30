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

#include <iostream>
#include <fstream>
#include <string>
#include <functional>

#include <GeoIPCity.h>

#ifndef SRC_UTILS_IP_TREE_H_
#define SRC_UTILS_IP_TREE_H_

#include "modsecurity/assay.h"
#include "utils/msc_tree.h"

namespace ModSecurity {
namespace Utils {


class IpTree {
 public:
    IpTree()
        : m_tree(NULL) { }
    ~IpTree();

    bool contains(const std::string &ip);
    bool addFromBuffer(const std::string& buffer, std::string *error);
    void postOrderTraversal(TreeNode *node);
 private:
    TreeRoot *m_tree;
};


}  // namespace Utils
}  // namespace ModSecurity

#endif  // SRC_UTILS_IP_TREE_H_
