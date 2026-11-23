#ifndef SRC_VARIABLES_ARGS_RAW_H_
#define SRC_VARIABLES_ARGS_RAW_H_

#include "src/variables/variable.h"

namespace modsecurity {
class Transaction;
namespace variables {
DEFINE_VARIABLE_DICT(ArgsRaw, ARGS_RAW, m_variableArgsRaw)
}  // namespace variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_ARGS_RAW_H_
