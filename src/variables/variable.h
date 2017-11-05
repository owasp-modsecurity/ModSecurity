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


    virtual std::vector<const collection::Variable *>
        *evaluate(Transaction *transaction);


    virtual void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l);


    virtual void evaluateInternal(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l);

    static std::string to_s(std::vector<Variable *> *variables);

    static inline bool compareStrNoCase(const std::string &a, const std::string &b) {
        return a.size() == b.size()
             && std::equal(a.begin(), a.end(), b.begin(),
            [](char aa, char bb) {
            return toupper(aa) == bb;
        });
    };

    static void stringMatchResolveMulti(Transaction *transaction, const std::string &variable,
        std::vector<const collection::Variable *> *l) {
        size_t collection = variable.find(".");
        if (collection == std::string::npos) {
            collection = variable.find(":");
        }
        if (collection == std::string::npos) {
            if (compareStrNoCase(variable, "RESPONSE_CONTENT_TYPE")) {
                transaction->m_variableResponseContentType.evaluate(l);
            }
            else if (compareStrNoCase(variable, "ARGS_COMBINED_SIZE")) {
                transaction->m_variableARGScombinedSize.evaluate(l);
            }
            else if (compareStrNoCase(variable, "AUTH_TYPE")) {
                transaction->m_variableAuthType.evaluate(l);
            }
            else if (compareStrNoCase(variable, "FILES_COMBINED_SIZE")) {
                transaction->m_variableFilesCombinedSize.evaluate(l);
            }
            else if (compareStrNoCase(variable, "FULL_REQUEST")) {
                transaction->m_variableFullRequest.evaluate(l);
            }
            else if (compareStrNoCase(variable, "FULL_REQUEST_LENGTH")) {
                transaction->m_variableFullRequestLength.evaluate(l);
            }
            else if (compareStrNoCase(variable, "INBOUND_DATA_ERROR")) {
                transaction->m_variableInboundDataError.evaluate(l);
            }
            else if (compareStrNoCase(variable, "MATCHED_VAR")) {
                transaction->m_variableMatchedVar.evaluate(l);
            }
            else if (compareStrNoCase(variable, "MATCHED_VAR_NAME")) {
                transaction->m_variableMatchedVarName.evaluate(l);
            }
            else if (compareStrNoCase(variable, "MULTIPART_CRLF_LF_LINES")) {
                transaction->m_variableMultipartCrlfLFLines.evaluate(l);
            }
            else if (compareStrNoCase(variable, "MULTIPART_DATA_AFTER")) {
                transaction->m_variableMultipartDataAfter.evaluate(l);
            }
            else if (compareStrNoCase(variable, "MULTIPART_FILE_LIMIT_EXCEEDED")) {
                transaction->m_variableMultipartFileLimitExceeded.evaluate(l);
            }
            else if (compareStrNoCase(variable, "MULTIPART_STRICT_ERROR")) {
                transaction->m_variableMultipartStrictError.evaluate(l);
            }
            else if (compareStrNoCase(variable, "MULTIPART_HEADER_FOLDING")) {
                transaction->m_variableMultipartHeaderFolding.evaluate(l);
            }
            else if (compareStrNoCase(variable, "MULTIPART_INVALID_QUOTING")) {
                transaction->m_variableMultipartInvalidQuoting.evaluate(l);
            }
            else if (compareStrNoCase(variable, "MULTIPART_INVALID_HEADER_FOLDING")) {
                transaction->m_variableMultipartInvalidHeaderFolding.evaluate(l);
            }
            else if (compareStrNoCase(variable, "MULTIPART_UNMATCHED_BOUNDARY")) {
                transaction->m_variableMultipartUnmatchedBoundary.evaluate(l);
            }
            else if (compareStrNoCase(variable, "OUTBOUND_DATA_ERROR")) {
                transaction->m_variableOutboundDataError.evaluate(l);
            }
            else if (compareStrNoCase(variable, "PATH_INFO")) {
                transaction->m_variablePathInfo.evaluate(l);
            }
            else if (compareStrNoCase(variable, "QUERY_STRING")) {
                transaction->m_variableQueryString.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REMOTE_ADDR")) {
                transaction->m_variableRemoteAddr.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REMOTE_HOST")) {
                transaction->m_variableRemoteHost.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REMOTE_PORT")) {
                transaction->m_variableRemotePort.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REQBODY_ERROR")) {
                transaction->m_variableReqbodyError.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REQBODY_ERROR_MSG")) {
                transaction->m_variableReqbodyErrorMsg.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REQBODY_PROCESSOR_ERROR_MSG")) {
                transaction->m_variableReqbodyProcessorErrorMsg.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REQBODY_PROCESSOR_ERROR")) {
                transaction->m_variableReqbodyProcessorError.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REQBODY_PROCESSOR")) {
                transaction->m_variableReqbodyProcessor.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REQUEST_BASENAME")) {
                transaction->m_variableRequestBasename.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REQUEST_BODY")) {
                transaction->m_variableRequestBody.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REQUEST_BODY_LENGTH")) {
                transaction->m_variableRequestBodyLength.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REQUEST_FILENAME")) {
                transaction->m_variableRequestFilename.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REQUEST_LINE")) {
                transaction->m_variableRequestLine.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REQUEST_METHOD")) {
                transaction->m_variableRequestMethod.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REQUEST_PROTOCOL")) {
                transaction->m_variableRequestProtocol.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REQUEST_URI")) {
                transaction->m_variableRequestURI.evaluate(l);
            }
            else if (compareStrNoCase(variable, "REQUEST_URI_RAW")) {
                transaction->m_variableRequestURIRaw.evaluate(l);
            }
            else if (compareStrNoCase(variable, "RESOURCE")) {
                transaction->m_variableResource.evaluate(l);
            }
            else if (compareStrNoCase(variable, "RESPONSE_BODY")) {
                transaction->m_variableResponseBody.evaluate(l);
            }
            else if (compareStrNoCase(variable, "RESPONSE_CONTENT_LENGTH")) {
                transaction->m_variableResponseContentLength.evaluate(l);
            }
            else if (compareStrNoCase(variable, "RESPONSE_PROTOCOL")) {
                transaction->m_variableResponseProtocol.evaluate(l);
            }
            else if (compareStrNoCase(variable, "RESPONSE_STATUS")) {
                transaction->m_variableResponseStatus.evaluate(l);
            }
            else if (compareStrNoCase(variable, "SERVER_ADDR")) {
                transaction->m_variableServerAddr.evaluate(l);
            }
            else if (compareStrNoCase(variable, "SERVER_NAME")) {
                transaction->m_variableServerName.evaluate(l);
            }
            else if (compareStrNoCase(variable, "SERVER_PORT")) {
                transaction->m_variableServerPort.evaluate(l);
            }
            else if (compareStrNoCase(variable, "SESSIONID")) {
                transaction->m_variableSessionID.evaluate(l);
            }
            else if (compareStrNoCase(variable, "UNIQUE_ID")) {
                transaction->m_variableUniqueID.evaluate(l);
            }
            else if (compareStrNoCase(variable, "URLENCODED_ERROR")) {
                transaction->m_variableUrlEncodedError.evaluate(l);
            }
            else if (compareStrNoCase(variable, "USERID")) {
                transaction->m_variableUserID.evaluate(l);
            } else {
                transaction->m_collections.resolveMultiMatches(
                    variable, l);
            }
        } else {
            std::string col = std::string(variable, 0, collection);
            std::string var = std::string(variable, collection + 1,
                variable.length() - (collection + 1));
            if (compareStrNoCase(col, "ARGS")) {
                transaction->m_variableArgs.resolve(var, l);
            }
            else if (compareStrNoCase(variable, "ARGS_NAMES")) {
                transaction->m_variableArgsNames.resolve(var, l);
            }
            else if (compareStrNoCase(variable, "ARGS_GET_NAMES")) {
                transaction->m_variableArgsGetNames.resolve(var, l);
            }
            else if (compareStrNoCase(variable, "ARGS_POST_NAMES")) {
                transaction->m_variableArgsPostNames.resolve(var, l);
            }
            else if (compareStrNoCase(col, "RULE")) {
                transaction->m_variableRule.resolve(var, l);
            }
            else if (compareStrNoCase(col, "ARGS_GET")) {
                transaction->m_variableArgsGet.resolve(var, l);
            }
            else if (compareStrNoCase(col, "ARGS_POST")) {
                transaction->m_variableArgsPost.resolve(var, l);
            }
            else if (compareStrNoCase(col, "FILES_SIZES")) {
                transaction->m_variableFilesSizes.resolve(var, l);
            }
            else if (compareStrNoCase(col, "FILES_NAMES")) {
                transaction->m_variableFilesNames.resolve(var, l);
            }
            else if (compareStrNoCase(col, "FILES_TMP_CONTENT")) {
                transaction->m_variableFilesTmpContent.resolve(var, l);
            }
            else if (compareStrNoCase(col, "MULTIPART_FILENAME")) {
                transaction->m_variableMultiPartFileName.resolve(var, l);
            }
            else if (compareStrNoCase(col, "MULTIPART_NAME")) {
                transaction->m_variableMultiPartName.resolve(var, l);
            }
            else if (compareStrNoCase(col, "MATCHED_VARS_NAMES")) {
                transaction->m_variableMatchedVarsNames.resolve(var, l);
            }
            else if (compareStrNoCase(col, "MATCHED_VARS")) {
                transaction->m_variableMatchedVars.resolve(var, l);
            }
            else if (compareStrNoCase(col, "FILES")) {
                transaction->m_variableFiles.resolve(var, l);
            }
            else if (compareStrNoCase(col, "REQUEST_COOKIES")) {
                transaction->m_variableRequestCookies.resolve(var, l);
            }
            else if (compareStrNoCase(col, "REQUEST_HEADERS")) {
                transaction->m_variableRequestHeaders.resolve(var, l);
            }
            else if (compareStrNoCase(variable, "REQUEST_HEADERS_NAMES")) {
                transaction->m_variableRequestHeadersNames.resolve(var, l);
            }
            else if (compareStrNoCase(col, "RESPONSE_HEADERS")) {
                transaction->m_variableResponseHeaders.resolve(var, l);
            }
            else if (compareStrNoCase(variable, "RESPONSE_HEADERS_NAMES")) {
                transaction->m_variableResponseHeadersNames.resolve(var, l);
            }
            else if (compareStrNoCase(col, "GEO")) {
                transaction->m_variableGeo.resolve(var, l);
            }
            else if (compareStrNoCase(col, "REQUEST_COOKIES_NAMES")) {
                transaction->m_variableRequestCookiesNames.resolve(var, l);
            }
            else if (compareStrNoCase(col, "FILES_TMPNAMES")) {
                transaction->m_variableFilesTmpNames.resolve(var, l);
            } else {
                transaction->m_collections.resolveMultiMatches(col,
                    var, l);
            }
        }
    }

    static std::string stringMatchResolve(Transaction *transaction, const std::string &variable) {
        std::unique_ptr<std::string> variableValue = nullptr;
        size_t collection = variable.find(".");
        if (collection == std::string::npos) {
            collection = variable.find(":");
        }
        if (collection == std::string::npos) {
            if (compareStrNoCase(variable, "RESPONSE_CONTENT_TYPE")) {
                variableValue = transaction->m_variableResponseContentType.resolveFirst();
            }
            else if (compareStrNoCase(variable, "ARGS_COMBINED_SIZE")) {
                variableValue = transaction->m_variableARGScombinedSize.resolveFirst();
            }
            else if (compareStrNoCase(variable, "AUTH_TYPE")) {
                variableValue = transaction->m_variableAuthType.resolveFirst();
            }
            else if (compareStrNoCase(variable, "FILES_COMBINED_SIZE")) {
                variableValue = transaction->m_variableFilesCombinedSize.resolveFirst();
            }
            else if (compareStrNoCase(variable, "FULL_REQUEST")) {
                variableValue = transaction->m_variableFullRequest.resolveFirst();
            }
            else if (compareStrNoCase(variable, "FULL_REQUEST_LENGTH")) {
                variableValue = transaction->m_variableFullRequestLength.resolveFirst();
            }
            else if (compareStrNoCase(variable, "INBOUND_DATA_ERROR")) {
                variableValue = transaction->m_variableInboundDataError.resolveFirst();
            }
            else if (compareStrNoCase(variable, "MATCHED_VAR")) {
                variableValue = transaction->m_variableMatchedVar.resolveFirst();
            }
            else if (compareStrNoCase(variable, "MATCHED_VAR_NAME")) {
                variableValue = transaction->m_variableMatchedVarName.resolveFirst();
            }
            else if (compareStrNoCase(variable, "MULTIPART_CRLF_LF_LINES")) {
                variableValue = transaction->m_variableMultipartCrlfLFLines.resolveFirst();
            }
            else if (compareStrNoCase(variable, "MULTIPART_DATA_AFTER")) {
                variableValue = transaction->m_variableMultipartDataAfter.resolveFirst();
            }
            else if (compareStrNoCase(variable, "MULTIPART_FILE_LIMIT_EXCEEDED")) {
                variableValue = transaction->m_variableMultipartFileLimitExceeded.resolveFirst();
            }
            else if (compareStrNoCase(variable, "MULTIPART_STRICT_ERROR")) {
                variableValue = transaction->m_variableMultipartStrictError.resolveFirst();
            }
            else if (compareStrNoCase(variable, "MULTIPART_HEADER_FOLDING")) {
                variableValue = transaction->m_variableMultipartHeaderFolding.resolveFirst();
            }
            else if (compareStrNoCase(variable, "MULTIPART_INVALID_QUOTING")) {
                variableValue = transaction->m_variableMultipartInvalidQuoting.resolveFirst();
            }
            else if (compareStrNoCase(variable, "MULTIPART_INVALID_HEADER_FOLDING")) {
                variableValue = transaction->m_variableMultipartInvalidHeaderFolding.resolveFirst();
            }
            else if (compareStrNoCase(variable, "MULTIPART_UNMATCHED_BOUNDARY")) {
                variableValue = transaction->m_variableMultipartUnmatchedBoundary.resolveFirst();
            }
            else if (compareStrNoCase(variable, "OUTBOUND_DATA_ERROR")) {
                variableValue = transaction->m_variableOutboundDataError.resolveFirst();
            }
            else if (compareStrNoCase(variable, "PATH_INFO")) {
                variableValue = transaction->m_variablePathInfo.resolveFirst();
            }
            else if (compareStrNoCase(variable, "QUERY_STRING")) {
                variableValue = transaction->m_variableQueryString.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REMOTE_ADDR")) {
                variableValue = transaction->m_variableRemoteAddr.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REMOTE_HOST")) {
                variableValue = transaction->m_variableRemoteHost.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REMOTE_PORT")) {
                variableValue = transaction->m_variableRemotePort.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQBODY_ERROR")) {
                variableValue = transaction->m_variableReqbodyError.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQBODY_ERROR_MSG")) {
                variableValue = transaction->m_variableReqbodyErrorMsg.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQBODY_PROCESSOR_ERROR_MSG")) {
                variableValue = transaction->m_variableReqbodyProcessorErrorMsg.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQBODY_PROCESSOR_ERROR")) {
                variableValue = transaction->m_variableReqbodyProcessorError.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQBODY_PROCESSOR")) {
                variableValue = transaction->m_variableReqbodyProcessor.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQUEST_BASENAME")) {
                variableValue = transaction->m_variableRequestBasename.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQUEST_BODY")) {
                variableValue = transaction->m_variableRequestBody.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQUEST_BODY_LENGTH")) {
                variableValue = transaction->m_variableRequestBodyLength.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQUEST_FILENAME")) {
                variableValue = transaction->m_variableRequestFilename.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQUEST_LINE")) {
                variableValue = transaction->m_variableRequestLine.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQUEST_METHOD")) {
                variableValue = transaction->m_variableRequestMethod.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQUEST_PROTOCOL")) {
                variableValue = transaction->m_variableRequestProtocol.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQUEST_URI")) {
                variableValue = transaction->m_variableRequestURI.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQUEST_URI_RAW")) {
                variableValue = transaction->m_variableRequestURIRaw.resolveFirst();
            }
            else if (compareStrNoCase(variable, "RESOURCE")) {
                variableValue = transaction->m_variableResource.resolveFirst();
            }
            else if (compareStrNoCase(variable, "RESPONSE_BODY")) {
                variableValue = transaction->m_variableResponseBody.resolveFirst();
            }
            else if (compareStrNoCase(variable, "RESPONSE_CONTENT_LENGTH")) {
                variableValue = transaction->m_variableResponseContentLength.resolveFirst();
            }
            else if (compareStrNoCase(variable, "RESPONSE_PROTOCOL")) {
                variableValue = transaction->m_variableResponseProtocol.resolveFirst();
            }
            else if (compareStrNoCase(variable, "RESPONSE_STATUS")) {
                variableValue = transaction->m_variableResponseStatus.resolveFirst();
            }
            else if (compareStrNoCase(variable, "SERVER_ADDR")) {
                variableValue = transaction->m_variableServerAddr.resolveFirst();
            }
            else if (compareStrNoCase(variable, "SERVER_NAME")) {
                variableValue = transaction->m_variableServerName.resolveFirst();
            }
            else if (compareStrNoCase(variable, "SERVER_PORT")) {
                variableValue = transaction->m_variableServerPort.resolveFirst();
            }
            else if (compareStrNoCase(variable, "SESSIONID")) {
                variableValue = transaction->m_variableSessionID.resolveFirst();
            }
            else if (compareStrNoCase(variable, "UNIQUE_ID")) {
                variableValue = transaction->m_variableUniqueID.resolveFirst();
            }
            else if (compareStrNoCase(variable, "URLENCODED_ERROR")) {
                variableValue = transaction->m_variableUrlEncodedError.resolveFirst();
            }
            else if (compareStrNoCase(variable, "USERID")) {
                variableValue = transaction->m_variableUserID.resolveFirst();
            } else {
                variableValue = transaction->m_collections.resolveFirst(
                    variable);
            }
        } else {
            std::string col = std::string(variable, 0, collection);
            std::string var = std::string(variable, collection + 1,
                variable.length() - (collection + 1));
            if (compareStrNoCase(col, "ARGS")) {
                variableValue = transaction->m_variableArgs.resolveFirst(var);
            }
            else if (compareStrNoCase(variable, "ARGS_NAMES")) {
                variableValue = transaction->m_variableArgsNames.resolveFirst(var);
            }
            else if (compareStrNoCase(variable, "ARGS_GET_NAMES")) {
                variableValue = transaction->m_variableArgsGetNames.resolveFirst(var);
            }
            else if (compareStrNoCase(variable, "ARGS_POST_NAMES")) {
                variableValue = transaction->m_variableArgsPostNames.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "RULE")) {
                variableValue = transaction->m_variableRule.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "ARGS_GET")) {
                variableValue = transaction->m_variableArgsGet.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "ARGS_POST")) {
                variableValue = transaction->m_variableArgsPost.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "FILES_SIZES")) {
                variableValue = transaction->m_variableFilesSizes.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "FILES_NAMES")) {
                variableValue = transaction->m_variableFilesNames.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "FILES_TMP_CONTENT")) {
                variableValue = transaction->m_variableFilesTmpContent.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "MULTIPART_FILENAME")) {
                variableValue = transaction->m_variableMultiPartFileName.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "MULTIPART_NAME")) {
                variableValue = transaction->m_variableMultiPartName.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "MATCHED_VARS_NAMES")) {
                variableValue = transaction->m_variableMatchedVarsNames.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "MATCHED_VARS")) {
                variableValue = transaction->m_variableMatchedVars.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "FILES")) {
                variableValue = transaction->m_variableFiles.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "REQUEST_COOKIES")) {
                variableValue = transaction->m_variableRequestCookies.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "REQUEST_HEADERS")) {
                variableValue = transaction->m_variableRequestHeaders.resolveFirst(var);
            }
            else if (compareStrNoCase(variable, "REQUEST_HEADERS_NAMES")) {
                variableValue = transaction->m_variableRequestHeadersNames.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "RESPONSE_HEADERS")) {
                variableValue = transaction->m_variableResponseHeaders.resolveFirst(var);
            }
            else if (compareStrNoCase(variable, "RESPONSE_HEADERS_NAMES")) {
                variableValue = transaction->m_variableResponseHeadersNames.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "GEO")) {
                variableValue = transaction->m_variableGeo.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "REQUEST_COOKIES_NAMES")) {
                variableValue = transaction->m_variableRequestCookiesNames.resolveFirst(var);
            }
            else if (compareStrNoCase(col, "FILES_TMPNAMES")) {
                variableValue = transaction->m_variableFilesTmpNames.resolveFirst(var);
            } else {
                variableValue = transaction->m_collections.resolveFirst(col,
                    var);
            }
        }
        return std::string(*variableValue.get());
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

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) {
        m_var->evaluate(transaction, rule, l);
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

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) {
        std::vector<const collection::Variable *> reslIn;
        collection::Variable *val = NULL;
        int count = 0;

        m_var->evaluate(transaction, rule, &reslIn);
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
