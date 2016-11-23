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

#include "src/operators/operator.h"

#include <cstring>

#include <string>

#include "modsecurity/transaction.h"

#include "src/operators/begins_with.h"
#include "src/operators/contains.h"
#include "src/operators/contains_word.h"
#include "src/operators/detect_sqli.h"
#include "src/operators/detect_xss.h"
#include "src/operators/ends_with.h"
#include "src/operators/eq.h"
#include "src/operators/fuzzy_hash.h"
#include "src/operators/ge.h"
#include "src/operators/geo_lookup.h"
#include "src/operators/gsblookup.h"
#include "src/operators/gt.h"
#include "src/operators/inspect_file.h"
#include "src/operators/ip_match_f.h"
#include "src/operators/ip_match_from_file.h"
#include "src/operators/ip_match.h"
#include "src/operators/le.h"
#include "src/operators/lt.h"
#include "src/operators/no_match.h"
#include "src/operators/pm_f.h"
#include "src/operators/pm_from_file.h"
#include "src/operators/pm.h"
#include "src/operators/rbl.h"
#include "src/operators/rsub.h"
#include "src/operators/rx.h"
#include "src/operators/str_eq.h"
#include "src/operators/str_match.h"
#include "src/operators/validate_byte_range.h"
#include "src/operators/validate_dtd.h"
#include "src/operators/validate_hash.h"
#include "src/operators/validate_schema.h"
#include "src/operators/validate_url_encoding.h"
#include "src/operators/validate_utf8_encoding.h"
#include "src/operators/verify_cc.h"
#include "src/operators/verify_cpf.h"
#include "src/operators/verify_ssn.h"
#include "src/operators/within.h"
#include "src/operators/unconditional_match.h"

#define IF_MATCH(a) \
    if (op_ == #a)

namespace modsecurity {
namespace operators {


bool Operator::debug(Transaction *transaction, int x, std::string a) {
#ifndef NO_LOGS
    transaction->debug(x, a);
#endif
    return true;
}


bool Operator::evaluateInternal(Transaction *transaction,
    Rule *rule, const std::string& a) {
    bool res = evaluate(transaction, rule, a);

    if (m_negation) {
        return !res;
    }

    return res;
}

bool Operator::evaluateInternal(Transaction *transaction,
    const std::string& a) {
    bool res = evaluate(transaction, a);

    if (m_negation) {
        return !res;
    }

    return res;
}


bool Operator::evaluate(Transaction *transaction, const std::string& a) {
#ifndef NO_LOGS
    if (transaction) {
        transaction->debug(2, "Operator: " + this->m_op + \
            " is not implemented or malfunctioning.");
    } else {
        std::cerr << "Operator: " + this->m_op + \
            " is not implemented or malfunctioning.";
    }
#endif

    return true;
}

Operator *Operator::instantiate(std::string op_string) {
    // Sanity check.
    if (op_string.size() <= 2) {
        return NULL;
    }

    std::string op = op_string;
    op = op.substr(1, op_string.size() - 2);

    // We assume no negation by default
    bool negation = false;
    // If there is a '!' in front substring and assign negation
    if (op.at(0) == '!') {
       op = op.substr(1, op.size() - 1);
       negation = true;
    }

    // Check to see if there is a parameter, if not param is an empty string
    std::string param = "";
    if (op.find(" ") != std::string::npos) {
        param = op;
        param.erase(0, param.find(" ") + 1);
        op.erase(op.find(" "),
            op.length() - op.find(" "));
    }

    for (std::basic_string<char>::iterator p = op.begin();
        p != op.end(); ++p) {
       *p = tolower(*p);
    }
    std::string op_ = op;
    if (op_.length() > 2) {
        op_.erase(0, 1);
        if (op_.back() == ' ') {
            op_.pop_back();
        }
    }

    IF_MATCH(beginswith) { return new BeginsWith(op, param, negation); }
    IF_MATCH(contains) { return new Contains(op, param, negation); }
    IF_MATCH(containsword) { return new ContainsWord(op, param, negation); }
    IF_MATCH(detectsqli) { return new DetectSQLi(op, param, negation); }
    IF_MATCH(detectxss) { return new DetectXSS(op, param, negation); }
    IF_MATCH(endswith) { return new EndsWith(op, param, negation); }
    IF_MATCH(eq) { return new Eq(op, param, negation); }
    IF_MATCH(fuzzyhash) { return new FuzzyHash(op, param, negation); }
    IF_MATCH(geolookup) { return new GeoLookup(op, param, negation); }
    IF_MATCH(ge) { return new Ge(op, param, negation); }
    IF_MATCH(gsblookup) { return new GsbLookup(op, param, negation); }
    IF_MATCH(gt) { return new Gt(op, param, negation); }
    IF_MATCH(inspectfile) { return new InspectFile(op, param, negation); }
    IF_MATCH(ipmatchf) { return new IpMatchF(op, param, negation); }
    IF_MATCH(ipmatchfromfile) {
        return new IpMatchFromFile(op, param, negation);
    }
    IF_MATCH(ipmatch) { return new IpMatch(op, param, negation); }
    IF_MATCH(le) { return new Le(op, param, negation); }
    IF_MATCH(lt) { return new Lt(op, param, negation); }
    IF_MATCH(nomatch) { return new NoMatch(op, param, negation); }
    IF_MATCH(pmf) { return new PmF(op, param, negation); }
    IF_MATCH(pmfromfile) { return new PmFromFile(op, param, negation); }
    IF_MATCH(pm) { return new Pm(op, param, negation); }
    IF_MATCH(rbl) { return new Rbl(op, param, negation); }
    IF_MATCH(rsub) { return new Rsub(op, param, negation); }
    IF_MATCH(rx) { return new Rx(op, param, negation); }
    IF_MATCH(streq) { return new StrEq(op, param, negation); }
    IF_MATCH(strmatch) { return new StrMatch(op, param, negation); }
    IF_MATCH(validatebyterange) {
        return new ValidateByteRange(op, param, negation);
    }
    IF_MATCH(validatedtd) { return new ValidateDTD(op, param, negation); }
    IF_MATCH(validatehash) { return new ValidateHash(op, param, negation); }
    IF_MATCH(validateschema) { return new ValidateSchema(op, param, negation); }
    IF_MATCH(validateurlencoding) {
        return new ValidateUrlEncoding(op, param, negation);
    }
    IF_MATCH(validateutf8encoding) {
        return new ValidateUtf8Encoding(op, param, negation);
    }
    IF_MATCH(verifycc) { return new VerifyCC(op, param, negation); }
    IF_MATCH(verifycpf) { return new VerifyCPF(op, param, negation); }
    IF_MATCH(verifyssn) { return new VerifySSN(op, param, negation); }
    IF_MATCH(within) { return new Within(op, param, negation); }

    IF_MATCH(unconditionalmatch) {
        return new UnconditionalMatch(op, param, negation);
    }


    return new Operator(op, param, negation);
}

}  // namespace operators
}  // namespace modsecurity
