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

#ifndef HEADERS_MODSECURITY_VARIABLE_NAMES_H_
#define HEADERS_MODSECURITY_VARIABLE_NAMES_H_

#ifdef __cplusplus
#include <string>

// These constants back AnchoredVariable::m_name / AnchoredSetVariable::m_name,
// which were changed from owning std::string members to `const std::string &`
// references (see transaction.h). Every TransactionAnchoredVariables that
// used to copy-construct ~77 std::string objects per Transaction now just
// binds a reference to the single instance of each constant here, built
// once at static-init time. VariableValue (see variable_value.h) always
// copies from these immediately on construction, so nothing needs to
// outlive any individual call - these only need to outlive the process.

namespace modsecurity {
namespace variable_names {

inline const std::string REQUEST_HEADERS_NAMES              = "REQUEST_HEADERS_NAMES";
inline const std::string RESPONSE_CONTENT_TYPE               = "RESPONSE_CONTENT_TYPE";
inline const std::string RESPONSE_HEADERS_NAMES               = "RESPONSE_HEADERS_NAMES";
inline const std::string ARGS_COMBINED_SIZE                   = "ARGS_COMBINED_SIZE";
inline const std::string AUTH_TYPE                            = "AUTH_TYPE";
inline const std::string FILES_COMBINED_SIZE                  = "FILES_COMBINED_SIZE";
inline const std::string FULL_REQUEST                         = "FULL_REQUEST";
inline const std::string FULL_REQUEST_LENGTH                  = "FULL_REQUEST_LENGTH";
inline const std::string INBOUND_DATA_ERROR                   = "INBOUND_DATA_ERROR";
inline const std::string MATCHED_VAR                          = "MATCHED_VAR";
inline const std::string MATCHED_VAR_NAME                     = "MATCHED_VAR_NAME";
inline const std::string MSC_PCRE_ERROR                       = "MSC_PCRE_ERROR";
inline const std::string MSC_PCRE_LIMITS_EXCEEDED              = "MSC_PCRE_LIMITS_EXCEEDED";
inline const std::string MULTIPART_BOUNDARY_QUOTED             = "MULTIPART_BOUNDARY_QUOTED";
inline const std::string MULTIPART_BOUNDARY_WHITESPACE         = "MULTIPART_BOUNDARY_WHITESPACE";
inline const std::string MULTIPART_CRLF_LF_LINES                = "MULTIPART_CRLF_LF_LINES";
inline const std::string MULTIPART_DATA_AFTER                  = "MULTIPART_DATA_AFTER";
inline const std::string MULTIPART_DATA_BEFORE                 = "MULTIPART_DATA_BEFORE";
inline const std::string MULTIPART_FILE_LIMIT_EXCEEDED          = "MULTIPART_FILE_LIMIT_EXCEEDED";
inline const std::string MULTIPART_HEADER_FOLDING               = "MULTIPART_HEADER_FOLDING";
inline const std::string MULTIPART_INVALID_HEADER_FOLDING       = "MULTIPART_INVALID_HEADER_FOLDING";
inline const std::string MULTIPART_INVALID_PART                 = "MULTIPART_INVALID_PART";
inline const std::string MULTIPART_INVALID_QUOTING              = "MULTIPART_INVALID_QUOTING";
inline const std::string MULTIPART_LF_LINE                      = "MULTIPART_LF_LINE";
inline const std::string MULTIPART_MISSING_SEMICOLON            = "MULTIPART_MISSING_SEMICOLON";
inline const std::string MULTIPART_STRICT_ERROR                 = "MULTIPART_STRICT_ERROR";
inline const std::string MULTIPART_UNMATCHED_BOUNDARY           = "MULTIPART_UNMATCHED_BOUNDARY";
inline const std::string OUTBOUND_DATA_ERROR                    = "OUTBOUND_DATA_ERROR";
inline const std::string PATH_INFO                              = "PATH_INFO";
inline const std::string QUERY_STRING                           = "QUERY_STRING";
inline const std::string REMOTE_ADDR                            = "REMOTE_ADDR";
inline const std::string REMOTE_HOST                            = "REMOTE_HOST";
inline const std::string REMOTE_PORT                            = "REMOTE_PORT";
inline const std::string REQBODY_ERROR                          = "REQBODY_ERROR";
inline const std::string REQBODY_ERROR_MSG                      = "REQBODY_ERROR_MSG";
inline const std::string REQBODY_PROCESSOR_ERROR                = "REQBODY_PROCESSOR_ERROR";
inline const std::string REQBODY_PROCESSOR_ERROR_MSG            = "REQBODY_PROCESSOR_ERROR_MSG";
inline const std::string REQBODY_PROCESSOR                      = "REQBODY_PROCESSOR";
inline const std::string REQUEST_BASENAME                       = "REQUEST_BASENAME";
inline const std::string REQUEST_BODY                           = "REQUEST_BODY";
inline const std::string REQUEST_BODY_LENGTH                    = "REQUEST_BODY_LENGTH";
inline const std::string REQUEST_FILENAME                       = "REQUEST_FILENAME";
inline const std::string REQUEST_LINE                           = "REQUEST_LINE";
inline const std::string REQUEST_METHOD                         = "REQUEST_METHOD";
inline const std::string REQUEST_PROTOCOL                       = "REQUEST_PROTOCOL";
inline const std::string REQUEST_URI                            = "REQUEST_URI";
inline const std::string REQUEST_URI_RAW                        = "REQUEST_URI_RAW";
inline const std::string RESOURCE                               = "RESOURCE";
inline const std::string RESPONSE_BODY                          = "RESPONSE_BODY";
inline const std::string RESPONSE_CONTENT_LENGTH                = "RESPONSE_CONTENT_LENGTH";
inline const std::string RESPONSE_PROTOCOL                      = "RESPONSE_PROTOCOL";
inline const std::string RESPONSE_STATUS                        = "RESPONSE_STATUS";
inline const std::string SERVER_ADDR                            = "SERVER_ADDR";
inline const std::string SERVER_NAME                            = "SERVER_NAME";
inline const std::string SERVER_PORT                            = "SERVER_PORT";
inline const std::string SESSIONID                              = "SESSIONID";
inline const std::string UNIQUE_ID                              = "UNIQUE_ID";
inline const std::string URLENCODED_ERROR                       = "URLENCODED_ERROR";
inline const std::string USERID                                 = "USERID";
inline const std::string ARGS                                   = "ARGS";
inline const std::string ARGS_GET                               = "ARGS_GET";
inline const std::string ARGS_POST                              = "ARGS_POST";
inline const std::string FILES_SIZES                            = "FILES_SIZES";
inline const std::string FILES_NAMES                            = "FILES_NAMES";
inline const std::string FILES_TMP_CONTENT                      = "FILES_TMP_CONTENT";
inline const std::string MULTIPART_FILENAME                     = "MULTIPART_FILENAME";
inline const std::string MULTIPART_NAME                         = "MULTIPART_NAME";
inline const std::string MATCHED_VARS_NAMES                     = "MATCHED_VARS_NAMES";
inline const std::string MATCHED_VARS                           = "MATCHED_VARS";
inline const std::string FILES                                  = "FILES";
inline const std::string REQUEST_COOKIES                        = "REQUEST_COOKIES";
inline const std::string REQUEST_HEADERS                        = "REQUEST_HEADERS";
inline const std::string RESPONSE_HEADERS                       = "RESPONSE_HEADERS";
inline const std::string GEO                                    = "GEO";
inline const std::string REQUEST_COOKIES_NAMES                  = "REQUEST_COOKIES_NAMES";
inline const std::string FILES_TMPNAMES                         = "FILES_TMPNAMES";
inline const std::string MULTIPART_PART_HEADERS                 = "MULTIPART_PART_HEADERS";

}  // namespace variable_names
}  // namespace modsecurity

#endif  // __cplusplus

#endif  // HEADERS_MODSECURITY_VARIABLE_NAMES_H_
