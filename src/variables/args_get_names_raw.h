#ifndef SRC_VARIABLES_ARGS_GET_NAMES_RAW_H_
#define SRC_VARIABLES_ARGS_GET_NAMES_RAW_H_

#include "src/variables/variable.h"

namespace modsecurity {
class Transaction;
namespace variables {
DEFINE_VARIABLE_DICT(ArgsGetNamesRaw, ARGS_GET_NAMES_RAW, m_variableArgsGetNamesRaw)
}  // namespace variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_ARGS_GET_NAMES_RAW_H_
