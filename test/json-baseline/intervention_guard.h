// Copyright 2026 ModSecurity contributors.
#ifndef TEST_JSON_BASELINE_INTERVENTION_GUARD_H_
#define TEST_JSON_BASELINE_INTERVENTION_GUARD_H_

#include <cstdlib>

#include "modsecurity/intervention.h"

namespace json_test {
// The engine allocates url/log with its C API. Always use its matching cleanup,
// including when output serialization throws before the normal end of scope.
class InterventionGuard {
 public:
    InterventionGuard() {
        value.status = 200;
    }
    ~InterventionGuard() {
        modsecurity::intervention::free(&value);
    }
    InterventionGuard(const InterventionGuard &) = delete;
    InterventionGuard &operator=(const InterventionGuard &) = delete;

    modsecurity::ModSecurityIntervention value{};
};
}  // namespace json_test

#endif  // TEST_JSON_BASELINE_INTERVENTION_GUARD_H_
