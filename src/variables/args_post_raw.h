#ifndef SRC_VARIABLES_ARGS_POST_RAW_H_
#define SRC_VARIABLES_ARGS_POST_RAW_H_

#include "src/variables/variable.h"

namespace modsecurity {
class Transaction;
namespace variables {
DEFINE_VARIABLE_DICT(ArgsPostRaw, ARGS_POST_RAW, m_variableArgsPostRaw)
}  // namespace variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_ARGS_POST_RAW_H_
