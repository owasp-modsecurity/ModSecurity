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

#include "actions/ctl_request_body_processor_xml.h"

#include <iostream>
#include <string>

#include "modsecurity/transaction.h"

namespace modsecurity {
namespace actions {


bool CtlRequestBodyProcessorXML::evaluate(Rule *rule,
    Transaction *transaction) {
    transaction->m_requestBodyProcessor = Transaction::XMLRequestBody;
    return true;
}


}  // namespace actions
}  // namespace modsecurity
