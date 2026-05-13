#ifndef SRC_VARIABLES_ARGS_POST_NAMES_RAW_H_
#define SRC_VARIABLES_ARGS_POST_NAMES_RAW_H_

#include "src/variables/variable.h"

namespace modsecurity {
class Transaction;
namespace variables {
DEFINE_VARIABLE_DICT(ArgsPostNamesRaw, ARGS_POST_NAMES_RAW, m_variableArgsPostNamesRaw)
}  // namespace variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_ARGS_POST_NAMES_RAW_H_
