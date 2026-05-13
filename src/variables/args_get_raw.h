#ifndef SRC_VARIABLES_ARGS_GET_RAW_H_
#define SRC_VARIABLES_ARGS_GET_RAW_H_

#include "src/variables/variable.h"

namespace modsecurity {
class Transaction;
namespace variables {
DEFINE_VARIABLE_DICT(ArgsGetRaw, ARGS_GET_RAW, m_variableArgsGetRaw)
}  // namespace variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_ARGS_GET_RAW_H_
