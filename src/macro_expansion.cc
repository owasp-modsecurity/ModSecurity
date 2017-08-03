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


#include <memory>

#include "src/macro_expansion.h"
#include "modsecurity/transaction.h"
#include "modsecurity/collection/variable.h"
#include "src/variables/rule.h"
#include "src/variables/tx.h"
#include "src/variables/highest_severity.h"
#include "src/utils/string.h"


namespace modsecurity {

MacroExpansion::MacroExpansion() { }


std::string MacroExpansion::expandKeepOriginal(const std::string& input,
    Transaction *transaction) {
    std::string a = MacroExpansion::expand(input, transaction);

    if (a != input) {
        return "\"" + a + "\" (Was: " + input + ")";
    }

    return input;
}


std::string MacroExpansion::expand(const std::string& input,
    Transaction *transaction) {
    return expand(input, NULL, transaction);
}


std::string MacroExpansion::expand(const std::string& input,
     modsecurity::Rule *rule, Transaction *transaction) {
    std::string res;
    size_t pos = input.find("%{");

    if (pos != std::string::npos) {
        res = input;
    } else {
        return input;
    }

    while (pos != std::string::npos) {
        size_t start = pos;
        size_t end = res.find("}");
        size_t new_pos = start;
        if (end == std::string::npos) {
            return res;
        }
        std::string variable(res, start + 2, end - (start + 2));
        std::unique_ptr<std::string> variableValue = nullptr;
        size_t collection = variable.find(".");
        if (collection == std::string::npos) {
            collection = variable.find(":");
        }
        if (collection == std::string::npos) {
            if (compareStrNoCase(variable, "ARGS_GET_NAMES")) {
                variableValue = transaction->m_variableArgGetNames.resolveFirst();
            }
            else if (compareStrNoCase(variable, "ARGS_POST_NAMES")) {
                variableValue = transaction->m_variableArgPostNames.resolveFirst();
            }
            else if (compareStrNoCase(variable, "REQUEST_HEADERS_NAMES")) {
                variableValue = transaction->m_variableRequestHeadersNames.resolveFirst();
            }
            else if (compareStrNoCase(variable, "RESPONSE_CONTENT_TYPE")) {
                variableValue = transaction->m_variableResponseContentType.resolveFirst();
            }
            else if (compareStrNoCase(variable, "RESPONSE_HEADERS_NAMES")) {
                variableValue = transaction->m_variableResponseHeadersNames.resolveFirst();
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
            if (compareStrNoCase(variable, "ARGS_NAMES")) {
                variableValue = transaction->m_variableArgsNames.resolveFirst(var);
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
            else if (compareStrNoCase(col, "RESPONSE_HEADERS")) {
                variableValue = transaction->m_variableResponseHeaders.resolveFirst(var);
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

#ifndef NO_LOGS
        if (variableValue) {
            transaction->debug(6, "Resolving: " + variable + " to: " +
                *variableValue);
        } else {
            transaction->debug(6, "Resolving: " + variable + " to: NULL");
        }
#endif
        res.erase(start, end - start + 1);
        if (res[start] == '%') {
            res.erase(start, 1);
        }

        if (variableValue != NULL) {
            res.insert(start, *variableValue);
            new_pos = new_pos + (*variableValue).length();
        }
        if (new_pos + 3 >= res.length()) {
            break;
        }

        pos = res.find("%{", new_pos);
    }

    return res;
}

}  // namespace modsecurity
