/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2026 OWASP ModSecurity project
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact OWASP
 * directly using the email address modsecurity@owasp.org.
 *
 */


#include "modsecurity/collection/collection.h"

#include <string>
#include <vector>

#include "src/utils/regex.h"


namespace modsecurity {
namespace collection {


void Collection::resolveRegularExpression(const Utils::Regex *r,
    std::vector<const VariableValue *> *l, variables::KeyExclusions &ke) {
    // Default behaviour: fall back to the string-based resolution using the
    // pattern carried by the compiled regex. Backends that can take advantage
    // of the already-compiled regex (e.g. InMemoryPerProcess) override this.
    resolveRegularExpression(r->pattern, l, ke);
}


}  // namespace collection
}  // namespace modsecurity
