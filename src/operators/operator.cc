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

#include "operators/operator.h"

#include <cstring>

#include <string>

#include "modsecurity/assay.h"

#include "operators/begins_with.h"
#include "operators/contains.h"
#include "operators/contains_word.h"
#include "operators/detect_sqli.h"
#include "operators/detect_xss.h"
#include "operators/ends_with.h"
#include "operators/eq.h"
#include "operators/fuzzy_hash.h"
#include "operators/ge.h"
#include "operators/geo_lookup.h"
#include "operators/gsblookup.h"
#include "operators/gt.h"
#include "operators/inspect_file.h"
#include "operators/ip_match_f.h"
#include "operators/ip_match_from_file.h"
#include "operators/ip_match.h"
#include "operators/le.h"
#include "operators/lt.h"
#include "operators/no_match.h"
#include "operators/pm_f.h"
#include "operators/pm_from_file.h"
#include "operators/pm.h"
#include "operators/rbl.h"
#include "operators/rsub.h"
#include "operators/rx.h"
#include "operators/str_eq.h"
#include "operators/str_match.h"
#include "operators/validate_byte_range.h"
#include "operators/validate_dtd.h"
#include "operators/validate_hash.h"
#include "operators/validate_schema.h"
#include "operators/validate_url_encoding.h"
#include "operators/validate_utf8_encoding.h"
#include "operators/verify_cc.h"
#include "operators/verify_cpf.h"
#include "operators/verify_ssn.h"
#include "operators/within.h"

#define IF_MATCH(a) \
    if (op_ == #a)

namespace modsecurity {
namespace operators {


bool Operator::debug(Assay *assay, int x, std::string a) {
#ifndef NO_LOGS
    assay->debug(x, a);
#endif
    return true;
}


bool Operator::evaluate(Assay *assay) {
#ifndef NO_LOGS
    if (assay) {
        assay->debug(2, "Operator: " + this->op + \
            " is not implemented or malfunctioning.");
    } else {
        std::cerr << "Operator: " + this->op + \
            " is not implemented or malfunctioning.";
    }
#endif
    return true;
}


bool Operator::evaluate(Assay *assay, const std::string& a) {
#ifndef NO_LOGS
    if (assay) {
        assay->debug(2, "Operator: " + this->op + \
            " is not implemented or malfunctioning.");
    } else {
        std::cerr << "Operator: " + this->op + \
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
        param.erase(0, op_string.find(" "));
        op.erase(op_string.find(" "),
            op_string.length() - op_string.find(" "));
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


    return new Operator(op, param, negation);
}

}  // namespace operators
}  // namespace ModSecurity

