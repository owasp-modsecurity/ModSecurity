/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2020 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#ifdef __cplusplus
#include <stack>
#include <vector>
#include <string>
#include <list>
#include <memory>
#include <utility>
#endif

#ifndef HEADERS_MODSECURITY_RULE_H_
#define HEADERS_MODSECURITY_RULE_H_

#include "modsecurity/transaction.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/variable_value.h"

#ifdef __cplusplus

namespace modsecurity {
namespace variables {
class Variable;
class Variables;
}
namespace operators {
class Operator;
}


class Rule {
 public:
    Rule(std::unique_ptr<std::string> fileName, int lineNumber)
        : m_fileName(std::move(fileName)),
        m_lineNumber(lineNumber),
        m_phase(modsecurity::Phases::RequestHeadersPhase) {
        }

    Rule(const Rule &r)
        : m_fileName(r.m_fileName),
        m_lineNumber(r.m_lineNumber),
        m_phase(r.m_phase) {

    }

    virtual bool evaluate(Transaction *transaction) = 0;

    std::shared_ptr<std::string> getFileName() const {
        return m_fileName;
    }

    int getLineNumber() const {
        return m_lineNumber;
    }

    int getPhase() const { return m_phase; }
    void setPhase(int phase) { m_phase = phase; }

    virtual std::string getReference() {
        return *m_fileName + ":" + std::to_string(m_lineNumber);
    }

 private:
    std::shared_ptr<std::string> m_fileName;
    int m_lineNumber;
    // FIXME: phase may not be neede to SecMarker.
    int m_phase;
};


}  // namespace modsecurity
#endif


#endif  // HEADERS_MODSECURITY_RULE_H_
