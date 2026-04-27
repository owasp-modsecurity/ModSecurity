#ifndef SRC_JSON_JSON_ERROR_H_
#define SRC_JSON_JSON_ERROR_H_

#include <string>

namespace modsecurity {
namespace json {

struct Error {
    int code;
    std::string message;
    size_t line;
    size_t column;
};

}  // namespace json
}  // namespace modsecurity

#endif
