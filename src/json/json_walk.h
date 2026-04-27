#ifndef SRC_JSON_JSON_WALK_H_
#define SRC_JSON_JSON_WALK_H_

#include <string>

namespace modsecurity {
namespace json {

class EventHandler {
 public:
    virtual ~EventHandler() = default;
    virtual bool onNull() = 0;
    virtual bool onBool(bool v) = 0;
    virtual bool onNumber(const std::string &v) = 0;
    virtual bool onString(const std::string &v) = 0;
    virtual bool onStartMap() = 0;
    virtual bool onMapKey(const std::string &k) = 0;
    virtual bool onEndMap() = 0;
    virtual bool onStartArray() = 0;
    virtual bool onEndArray() = 0;
};

bool walkJsonDocument(const std::string &payload, EventHandler *handler, std::string *error);

}  // namespace json
}  // namespace modsecurity

#endif
