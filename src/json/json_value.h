#ifndef SRC_JSON_JSON_VALUE_H_
#define SRC_JSON_JSON_VALUE_H_

#include <string>

namespace modsecurity {
namespace json {

class ValueView {
 public:
    enum Type {
        kNull,
        kBool,
        kNumber,
        kString,
        kArray,
        kObject,
        kUnknown
    };

    ValueView() : m_type(kUnknown) {}
    explicit ValueView(Type t) : m_type(t) {}
    Type type() const { return m_type; }

 private:
    Type m_type;
};

}  // namespace json
}  // namespace modsecurity

#endif
