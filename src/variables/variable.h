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

#include <vector>
#include <string>
#include <list>
#include <utility>
#include <memory>

#include "modsecurity/transaction.h"
#include "modsecurity/rule.h"
#include "src/utils/string.h"
#include "src/utils/regex.h"

#ifndef SRC_VARIABLES_VARIABLE_H_
#define SRC_VARIABLES_VARIABLE_H_

namespace modsecurity {

class Transaction;
namespace Variables {

class Variable {
 public:
    /**
     *
     */
    enum VariableType {
      /**
       *
       */
      SingleMatch,
      /**
       *
       */
      MultipleMatches,
      /**
       *
       */
      RegularExpression
    };

    /**
     *
     */
    enum VariableKind {
      /**
       *
       */
      DirectVariable,
      /**
       *
       */
      CollectionVarible,
    };

    explicit Variable(std::string _name);
    Variable(std::string name, VariableKind kind);
    virtual ~Variable() { }

    virtual void evaluate(Transaction *t,
        Rule *rule,
        std::vector<const collection::Variable *> *l) = 0;

    static std::string to_s(std::vector<Variable *> *variables);

    static inline bool comp(const std::string &a, const std::string &b) {
        return a.size() == b.size()
             && std::equal(a.begin(), a.end(), b.begin(),
            [](char aa, char bb) {
            return toupper(aa) == bb;
        });
    }

    static void stringMatchResolveMulti(Transaction *t,
        const std::string &variable,
        std::vector<const collection::Variable *> *l) {
        size_t collection = variable.find(".");
        if (collection == std::string::npos) {
            collection = variable.find(":");
        }
        if (collection == std::string::npos) {
            if (comp(variable, "RESPONSE_CONTENT_TYPE")) {
                t->m_variableResponseContentType.evaluate(l);
            } else if (comp(variable, "ARGS_COMBINED_SIZE")) {
                t->m_variableARGScombinedSize.evaluate(l);
            } else if (comp(variable, "AUTH_TYPE")) {
                t->m_variableAuthType.evaluate(l);
            } else if (comp(variable, "FILES_COMBINED_SIZE")) {
                t->m_variableFilesCombinedSize.evaluate(l);
            } else if (comp(variable, "FULL_REQUEST")) {
                t->m_variableFullRequest.evaluate(l);
            } else if (comp(variable, "FULL_REQUEST_LENGTH")) {
                t->m_variableFullRequestLength.evaluate(l);
            } else if (comp(variable, "INBOUND_DATA_ERROR")) {
                t->m_variableInboundDataError.evaluate(l);
            } else if (comp(variable, "MATCHED_VAR")) {
                t->m_variableMatchedVar.evaluate(l);
            } else if (comp(variable, "MATCHED_VAR_NAME")) {
                t->m_variableMatchedVarName.evaluate(l);
            } else if (comp(variable, "MULTIPART_CRLF_LF_LINES")) {
                t->m_variableMultipartCrlfLFLines.evaluate(l);
            } else if (comp(variable, "MULTIPART_DATA_AFTER")) {
                t->m_variableMultipartDataAfter.evaluate(l);
            } else if (comp(variable, "MULTIPART_FILE_LIMIT_EXCEEDED")) {
                t->m_variableMultipartFileLimitExceeded.evaluate(l);
            } else if (comp(variable, "MULTIPART_STRICT_ERROR")) {
                t->m_variableMultipartStrictError.evaluate(l);
            } else if (comp(variable, "MULTIPART_HEADER_FOLDING")) {
                t->m_variableMultipartHeaderFolding.evaluate(l);
            } else if (comp(variable, "MULTIPART_INVALID_QUOTING")) {
                t->m_variableMultipartInvalidQuoting.evaluate(l);
            } else if (comp(variable, "MULTIPART_INVALID_HEADER_FOLDING")) {
                t->m_variableMultipartInvalidHeaderFolding.evaluate(l);
            } else if (comp(variable, "MULTIPART_UNMATCHED_BOUNDARY")) {
                t->m_variableMultipartUnmatchedBoundary.evaluate(l);
            } else if (comp(variable, "OUTBOUND_DATA_ERROR")) {
                t->m_variableOutboundDataError.evaluate(l);
            } else if (comp(variable, "PATH_INFO")) {
                t->m_variablePathInfo.evaluate(l);
            } else if (comp(variable, "QUERY_STRING")) {
                t->m_variableQueryString.evaluate(l);
            } else if (comp(variable, "REMOTE_ADDR")) {
                t->m_variableRemoteAddr.evaluate(l);
            } else if (comp(variable, "REMOTE_HOST")) {
                t->m_variableRemoteHost.evaluate(l);
            } else if (comp(variable, "REMOTE_PORT")) {
                t->m_variableRemotePort.evaluate(l);
            } else if (comp(variable, "REQBODY_ERROR")) {
                t->m_variableReqbodyError.evaluate(l);
            } else if (comp(variable, "REQBODY_ERROR_MSG")) {
                t->m_variableReqbodyErrorMsg.evaluate(l);
            } else if (comp(variable, "REQBODY_PROCESSOR_ERROR_MSG")) {
                t->m_variableReqbodyProcessorErrorMsg.evaluate(l);
            } else if (comp(variable, "REQBODY_PROCESSOR_ERROR")) {
                t->m_variableReqbodyProcessorError.evaluate(l);
            } else if (comp(variable, "REQBODY_PROCESSOR")) {
                t->m_variableReqbodyProcessor.evaluate(l);
            } else if (comp(variable, "REQUEST_BASENAME")) {
                t->m_variableRequestBasename.evaluate(l);
            } else if (comp(variable, "REQUEST_BODY")) {
                t->m_variableRequestBody.evaluate(l);
            } else if (comp(variable, "REQUEST_BODY_LENGTH")) {
                t->m_variableRequestBodyLength.evaluate(l);
            } else if (comp(variable, "REQUEST_FILENAME")) {
                t->m_variableRequestFilename.evaluate(l);
            } else if (comp(variable, "REQUEST_LINE")) {
                t->m_variableRequestLine.evaluate(l);
            } else if (comp(variable, "REQUEST_METHOD")) {
                t->m_variableRequestMethod.evaluate(l);
            } else if (comp(variable, "REQUEST_PROTOCOL")) {
                t->m_variableRequestProtocol.evaluate(l);
            } else if (comp(variable, "REQUEST_URI")) {
                t->m_variableRequestURI.evaluate(l);
            } else if (comp(variable, "REQUEST_URI_RAW")) {
                t->m_variableRequestURIRaw.evaluate(l);
            } else if (comp(variable, "RESOURCE")) {
                t->m_variableResource.evaluate(l);
            } else if (comp(variable, "RESPONSE_BODY")) {
                t->m_variableResponseBody.evaluate(l);
            } else if (comp(variable, "RESPONSE_CONTENT_LENGTH")) {
                t->m_variableResponseContentLength.evaluate(l);
            } else if (comp(variable, "RESPONSE_PROTOCOL")) {
                t->m_variableResponseProtocol.evaluate(l);
            } else if (comp(variable, "RESPONSE_STATUS")) {
                t->m_variableResponseStatus.evaluate(l);
            } else if (comp(variable, "SERVER_ADDR")) {
                t->m_variableServerAddr.evaluate(l);
            } else if (comp(variable, "SERVER_NAME")) {
                t->m_variableServerName.evaluate(l);
            } else if (comp(variable, "SERVER_PORT")) {
                t->m_variableServerPort.evaluate(l);
            } else if (comp(variable, "SESSIONID")) {
                t->m_variableSessionID.evaluate(l);
            } else if (comp(variable, "UNIQUE_ID")) {
                t->m_variableUniqueID.evaluate(l);
            } else if (comp(variable, "URLENCODED_ERROR")) {
                t->m_variableUrlEncodedError.evaluate(l);
            } else if (comp(variable, "USERID")) {
                t->m_variableUserID.evaluate(l);
            } else {
                t->m_collections.resolveMultiMatches(variable, l);
            }
        } else {
            std::string col = std::string(variable, 0, collection);
            std::string var = std::string(variable, collection + 1,
                variable.length() - (collection + 1));
            if (comp(col, "ARGS")) {
                t->m_variableArgs.resolve(var, l);
            } else if (comp(variable, "ARGS_NAMES")) {
                t->m_variableArgsNames.resolve(var, l);
            } else if (comp(variable, "ARGS_GET_NAMES")) {
                t->m_variableArgsGetNames.resolve(var, l);
            } else if (comp(variable, "ARGS_POST_NAMES")) {
                t->m_variableArgsPostNames.resolve(var, l);
            } else if (comp(col, "RULE")) {
                t->m_variableRule.resolve(var, l);
            } else if (comp(col, "ARGS_GET")) {
                t->m_variableArgsGet.resolve(var, l);
            } else if (comp(col, "ARGS_POST")) {
                t->m_variableArgsPost.resolve(var, l);
            } else if (comp(col, "FILES_SIZES")) {
                t->m_variableFilesSizes.resolve(var, l);
            } else if (comp(col, "FILES_NAMES")) {
                t->m_variableFilesNames.resolve(var, l);
            } else if (comp(col, "FILES_TMP_CONTENT")) {
                t->m_variableFilesTmpContent.resolve(var, l);
            } else if (comp(col, "MULTIPART_FILENAME")) {
                t->m_variableMultipartFileName.resolve(var, l);
            } else if (comp(col, "MULTIPART_NAME")) {
                t->m_variableMultipartName.resolve(var, l);
            } else if (comp(col, "MATCHED_VARS_NAMES")) {
                t->m_variableMatchedVarsNames.resolve(var, l);
            } else if (comp(col, "MATCHED_VARS")) {
                t->m_variableMatchedVars.resolve(var, l);
            } else if (comp(col, "FILES")) {
                t->m_variableFiles.resolve(var, l);
            } else if (comp(col, "REQUEST_COOKIES")) {
                t->m_variableRequestCookies.resolve(var, l);
            } else if (comp(col, "REQUEST_HEADERS")) {
                t->m_variableRequestHeaders.resolve(var, l);
            } else if (comp(variable, "REQUEST_HEADERS_NAMES")) {
                t->m_variableRequestHeadersNames.resolve(var, l);
            } else if (comp(col, "RESPONSE_HEADERS")) {
                t->m_variableResponseHeaders.resolve(var, l);
            } else if (comp(variable, "RESPONSE_HEADERS_NAMES")) {
                t->m_variableResponseHeadersNames.resolve(var, l);
            } else if (comp(col, "GEO")) {
                t->m_variableGeo.resolve(var, l);
            } else if (comp(col, "REQUEST_COOKIES_NAMES")) {
                t->m_variableRequestCookiesNames.resolve(var, l);
            } else if (comp(col, "FILES_TMPNAMES")) {
                t->m_variableFilesTmpNames.resolve(var, l);
            } else {
                t->m_collections.resolveMultiMatches(col, var, l);
            }
        }
    }

    static std::string stringMatchResolve(Transaction *t,
        const std::string &variable) {
        std::unique_ptr<std::string> vv = nullptr;
        size_t collection = variable.find(".");
        if (collection == std::string::npos) {
            collection = variable.find(":");
        }
        if (collection == std::string::npos) {
            if (comp(variable, "RESPONSE_CONTENT_TYPE")) {
                vv = t->m_variableResponseContentType.resolveFirst();
            } else if (comp(variable, "ARGS_COMBINED_SIZE")) {
                vv = t->m_variableARGScombinedSize.resolveFirst();
            } else if (comp(variable, "AUTH_TYPE")) {
                vv = t->m_variableAuthType.resolveFirst();
            } else if (comp(variable, "FILES_COMBINED_SIZE")) {
                vv = t->m_variableFilesCombinedSize.resolveFirst();
            } else if (comp(variable, "FULL_REQUEST")) {
                vv = t->m_variableFullRequest.resolveFirst();
            } else if (comp(variable, "FULL_REQUEST_LENGTH")) {
                vv = t->m_variableFullRequestLength.resolveFirst();
            } else if (comp(variable, "INBOUND_DATA_ERROR")) {
                vv = t->m_variableInboundDataError.resolveFirst();
            } else if (comp(variable, "MATCHED_VAR")) {
                vv = t->m_variableMatchedVar.resolveFirst();
            } else if (comp(variable, "MATCHED_VAR_NAME")) {
                vv = t->m_variableMatchedVarName.resolveFirst();
            } else if (comp(variable, "MULTIPART_CRLF_LF_LINES")) {
                vv = t->m_variableMultipartCrlfLFLines.resolveFirst();
            } else if (comp(variable, "MULTIPART_DATA_AFTER")) {
                vv = t->m_variableMultipartDataAfter.resolveFirst();
            } else if (comp(variable, "MULTIPART_FILE_LIMIT_EXCEEDED")) {
                vv = t->m_variableMultipartFileLimitExceeded.resolveFirst();
            } else if (comp(variable, "MULTIPART_STRICT_ERROR")) {
                vv = t->m_variableMultipartStrictError.resolveFirst();
            } else if (comp(variable, "MULTIPART_HEADER_FOLDING")) {
                vv = t->m_variableMultipartHeaderFolding.resolveFirst();
            } else if (comp(variable, "MULTIPART_INVALID_QUOTING")) {
                vv = t->m_variableMultipartInvalidQuoting.resolveFirst();
            } else if (comp(variable, "MULTIPART_INVALID_HEADER_FOLDING")) {
                vv = t->m_variableMultipartInvalidHeaderFolding.resolveFirst();
            } else if (comp(variable, "MULTIPART_UNMATCHED_BOUNDARY")) {
                vv = t->m_variableMultipartUnmatchedBoundary.resolveFirst();
            } else if (comp(variable, "OUTBOUND_DATA_ERROR")) {
                vv = t->m_variableOutboundDataError.resolveFirst();
            } else if (comp(variable, "PATH_INFO")) {
                vv = t->m_variablePathInfo.resolveFirst();
            } else if (comp(variable, "QUERY_STRING")) {
                vv = t->m_variableQueryString.resolveFirst();
            } else if (comp(variable, "REMOTE_ADDR")) {
                vv = t->m_variableRemoteAddr.resolveFirst();
            } else if (comp(variable, "REMOTE_HOST")) {
                vv = t->m_variableRemoteHost.resolveFirst();
            } else if (comp(variable, "REMOTE_PORT")) {
                vv = t->m_variableRemotePort.resolveFirst();
            } else if (comp(variable, "REQBODY_ERROR")) {
                vv = t->m_variableReqbodyError.resolveFirst();
            } else if (comp(variable, "REQBODY_ERROR_MSG")) {
                vv = t->m_variableReqbodyErrorMsg.resolveFirst();
            } else if (comp(variable, "REQBODY_PROCESSOR_ERROR_MSG")) {
                vv = t->m_variableReqbodyProcessorErrorMsg.resolveFirst();
            } else if (comp(variable, "REQBODY_PROCESSOR_ERROR")) {
                vv = t->m_variableReqbodyProcessorError.resolveFirst();
            } else if (comp(variable, "REQBODY_PROCESSOR")) {
                vv = t->m_variableReqbodyProcessor.resolveFirst();
            } else if (comp(variable, "REQUEST_BASENAME")) {
                vv = t->m_variableRequestBasename.resolveFirst();
            } else if (comp(variable, "REQUEST_BODY")) {
                vv = t->m_variableRequestBody.resolveFirst();
            } else if (comp(variable, "REQUEST_BODY_LENGTH")) {
                vv = t->m_variableRequestBodyLength.resolveFirst();
            } else if (comp(variable, "REQUEST_FILENAME")) {
                vv = t->m_variableRequestFilename.resolveFirst();
            } else if (comp(variable, "REQUEST_LINE")) {
                vv = t->m_variableRequestLine.resolveFirst();
            } else if (comp(variable, "REQUEST_METHOD")) {
                vv = t->m_variableRequestMethod.resolveFirst();
            } else if (comp(variable, "REQUEST_PROTOCOL")) {
                vv = t->m_variableRequestProtocol.resolveFirst();
            } else if (comp(variable, "REQUEST_URI")) {
                vv = t->m_variableRequestURI.resolveFirst();
            } else if (comp(variable, "REQUEST_URI_RAW")) {
                vv = t->m_variableRequestURIRaw.resolveFirst();
            } else if (comp(variable, "RESOURCE")) {
                vv = t->m_variableResource.resolveFirst();
            } else if (comp(variable, "RESPONSE_BODY")) {
                vv = t->m_variableResponseBody.resolveFirst();
            } else if (comp(variable, "RESPONSE_CONTENT_LENGTH")) {
                vv = t->m_variableResponseContentLength.resolveFirst();
            } else if (comp(variable, "RESPONSE_PROTOCOL")) {
                vv = t->m_variableResponseProtocol.resolveFirst();
            } else if (comp(variable, "RESPONSE_STATUS")) {
                vv = t->m_variableResponseStatus.resolveFirst();
            } else if (comp(variable, "SERVER_ADDR")) {
                vv = t->m_variableServerAddr.resolveFirst();
            } else if (comp(variable, "SERVER_NAME")) {
                vv = t->m_variableServerName.resolveFirst();
            } else if (comp(variable, "SERVER_PORT")) {
                vv = t->m_variableServerPort.resolveFirst();
            } else if (comp(variable, "SESSIONID")) {
                vv = t->m_variableSessionID.resolveFirst();
            } else if (comp(variable, "UNIQUE_ID")) {
                vv = t->m_variableUniqueID.resolveFirst();
            } else if (comp(variable, "URLENCODED_ERROR")) {
                vv = t->m_variableUrlEncodedError.resolveFirst();
            } else if (comp(variable, "USERID")) {
                vv = t->m_variableUserID.resolveFirst();
            } else {
                vv = t->m_collections.resolveFirst(variable);
            }
        } else {
            std::string col = std::string(variable, 0, collection);
            std::string var = std::string(variable, collection + 1,
                variable.length() - (collection + 1));
            if (comp(col, "ARGS")) {
                vv = t->m_variableArgs.resolveFirst(var);
            } else if (comp(variable, "ARGS_NAMES")) {
                vv = t->m_variableArgsNames.resolveFirst(var);
            } else if (comp(variable, "ARGS_GET_NAMES")) {
                vv = t->m_variableArgsGetNames.resolveFirst(var);
            } else if (comp(variable, "ARGS_POST_NAMES")) {
                vv = t->m_variableArgsPostNames.resolveFirst(var);
            } else if (comp(col, "RULE")) {
                vv = t->m_variableRule.resolveFirst(var);
            } else if (comp(col, "ARGS_GET")) {
                vv = t->m_variableArgsGet.resolveFirst(var);
            } else if (comp(col, "ARGS_POST")) {
                vv = t->m_variableArgsPost.resolveFirst(var);
            } else if (comp(col, "FILES_SIZES")) {
                vv = t->m_variableFilesSizes.resolveFirst(var);
            } else if (comp(col, "FILES_NAMES")) {
                vv = t->m_variableFilesNames.resolveFirst(var);
            } else if (comp(col, "FILES_TMP_CONTENT")) {
                vv = t->m_variableFilesTmpContent.resolveFirst(var);
            } else if (comp(col, "MULTIPART_FILENAME")) {
                vv = t->m_variableMultipartFileName.resolveFirst(var);
            } else if (comp(col, "MULTIPART_NAME")) {
                vv = t->m_variableMultipartName.resolveFirst(var);
            } else if (comp(col, "MATCHED_VARS_NAMES")) {
                vv = t->m_variableMatchedVarsNames.resolveFirst(var);
            } else if (comp(col, "MATCHED_VARS")) {
                vv = t->m_variableMatchedVars.resolveFirst(var);
            } else if (comp(col, "FILES")) {
                vv = t->m_variableFiles.resolveFirst(var);
            } else if (comp(col, "REQUEST_COOKIES")) {
                vv = t->m_variableRequestCookies.resolveFirst(var);
            } else if (comp(col, "REQUEST_HEADERS")) {
                vv = t->m_variableRequestHeaders.resolveFirst(var);
            } else if (comp(variable, "REQUEST_HEADERS_NAMES")) {
                vv = t->m_variableRequestHeadersNames.resolveFirst(var);
            } else if (comp(col, "RESPONSE_HEADERS")) {
                vv = t->m_variableResponseHeaders.resolveFirst(var);
            } else if (comp(variable, "RESPONSE_HEADERS_NAMES")) {
                vv = t->m_variableResponseHeadersNames.resolveFirst(var);
            } else if (comp(col, "GEO")) {
                vv = t->m_variableGeo.resolveFirst(var);
            } else if (comp(col, "REQUEST_COOKIES_NAMES")) {
                vv = t->m_variableRequestCookiesNames.resolveFirst(var);
            } else if (comp(col, "FILES_TMPNAMES")) {
                vv = t->m_variableFilesTmpNames.resolveFirst(var);
            } else {
                vv = t->m_collections.resolveFirst(col, var);
            }
        }
        return std::string(*vv.get());
    }

    std::string m_name;
    std::string m_collectionName;

    VariableType m_type;
    VariableKind m_kind;
    bool m_isExclusion;
    bool m_isCount;
};


class VariableModificatorExclusion : public Variable {
 public:
    explicit VariableModificatorExclusion(std::unique_ptr<Variable> var)
        : Variable(var->m_name),
        m_var(std::move(var)) {
            m_isExclusion = true;
        }

    void evaluate(Transaction *t,
        Rule *rule,
        std::vector<const collection::Variable *> *l) {
        m_var->evaluate(t, rule, l);
    }

    std::unique_ptr<Variable> m_var;
};


class VariableModificatorCount : public Variable {
 public:
    explicit VariableModificatorCount(std::unique_ptr<Variable> var)
        : Variable(var->m_name),
        m_var(std::move(var)) {
            m_isCount = true;
        }

    void evaluate(Transaction *t,
        Rule *rule,
        std::vector<const collection::Variable *> *l) {
        std::vector<const collection::Variable *> reslIn;
        collection::Variable *val = NULL;
        int count = 0;

        m_var->evaluate(t, rule, &reslIn);
        for (const collection::Variable *a : reslIn) {
            count++;
	    delete a;
            a = NULL;
        }
        reslIn.clear();

        std::string *res = new std::string(std::to_string(count));
        std::string *name = new std::string(m_name);
        val = new collection::Variable(name, res);
        delete name;
        delete res;

        l->push_back(val);
        return;
    }

    std::unique_ptr<Variable> m_var;
};

}  // namespace Variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_VARIABLE_H_
