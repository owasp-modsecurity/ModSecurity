#include "src/json/json_walk.h"

#include <sstream>
#include <string>

#ifdef MSC_JSON_C_BACKEND_JSONC
#include <json-c/json.h>
#endif

#ifdef MSC_JSON_CXX_BACKEND_NLOHMANNJSON
#include <nlohmann/json.hpp>
#endif

namespace modsecurity {
namespace json {

#ifdef MSC_JSON_C_BACKEND_JSONC
static bool walkJsonC(struct json_object *obj, EventHandler *handler) {
    if (obj == nullptr) {
        return handler->onNull();
    }

    const json_type t = json_object_get_type(obj);
    switch (t) {
        case json_type_null:
            return handler->onNull();
        case json_type_boolean:
            return handler->onBool(json_object_get_boolean(obj) != 0);
        case json_type_double:
        case json_type_int: {
            const char *serialized = json_object_to_json_string_ext(obj, JSON_C_TO_STRING_PLAIN);
            return serialized != nullptr ? handler->onNumber(serialized) : false;
        }
        case json_type_string:
            return handler->onString(json_object_get_string(obj));
        case json_type_object: {
            if (!handler->onStartMap()) {
                return false;
            }
            json_object_object_foreach(obj, key, val) {
                if (!handler->onMapKey(key) || !walkJsonC(val, handler)) {
                    return false;
                }
            }
            return handler->onEndMap();
        }
        case json_type_array: {
            if (!handler->onStartArray()) {
                return false;
            }
            const int len = json_object_array_length(obj);
            for (int i = 0; i < len; ++i) {
                if (!walkJsonC(json_object_array_get_idx(obj, i), handler)) {
                    return false;
                }
            }
            return handler->onEndArray();
        }
    }
    return false;
}
#endif

#ifdef MSC_JSON_CXX_BACKEND_NLOHMANNJSON
static bool walkNlohmann(const nlohmann::json &j, EventHandler *handler) {
    if (j.is_null()) {
        return handler->onNull();
    }
    if (j.is_boolean()) {
        return handler->onBool(j.get<bool>());
    }
    if (j.is_number()) {
        return handler->onNumber(j.dump());
    }
    if (j.is_string()) {
        return handler->onString(j.get<std::string>());
    }
    if (j.is_object()) {
        if (!handler->onStartMap()) {
            return false;
        }
        for (auto it = j.begin(); it != j.end(); ++it) {
            if (!handler->onMapKey(it.key()) || !walkNlohmann(it.value(), handler)) {
                return false;
            }
        }
        return handler->onEndMap();
    }
    if (j.is_array()) {
        if (!handler->onStartArray()) {
            return false;
        }
        for (const auto &entry : j) {
            if (!walkNlohmann(entry, handler)) {
                return false;
            }
        }
        return handler->onEndArray();
    }
    return false;
}
#endif

bool walkJsonDocument(const std::string &payload, EventHandler *handler, std::string *error) {
    if (handler == nullptr) {
        if (error != nullptr) {
            *error = "JSON event handler cannot be null.";
        }
        return false;
    }

#ifdef MSC_JSON_C_BACKEND_JSONC
    json_tokener *tok = json_tokener_new();
    if (tok == nullptr) {
        if (error != nullptr) {
            *error = "json-c tokenizer allocation failed.";
        }
        return false;
    }
    json_object *root = json_tokener_parse_ex(tok, payload.c_str(), payload.size());
    if (json_tokener_get_error(tok) != json_tokener_success) {
        if (error != nullptr) {
            *error = json_tokener_error_desc(json_tokener_get_error(tok));
        }
        json_tokener_free(tok);
        return false;
    }
    json_tokener_free(tok);
    const bool ok = walkJsonC(root, handler);
    json_object_put(root);
    return ok;
#elif defined(MSC_JSON_CXX_BACKEND_NLOHMANNJSON)
    try {
        const nlohmann::json parsed = nlohmann::json::parse(payload);
        return walkNlohmann(parsed, handler);
    } catch (const std::exception &e) {
        if (error != nullptr) {
            *error = e.what();
        }
        return false;
    }
#else
    if (error != nullptr) {
        *error = "No functional DOM backend selected for JSON walking.";
    }
    return false;
#endif
}

}  // namespace json
}  // namespace modsecurity
