#include "src/json/json_backend.h"

#include <stdarg.h>

#include <cstdio>
#include <string>
#include <vector>

struct Frame {
    enum Kind { Map, Array } kind;
    bool first;
    bool expectingKey;
};

struct msc_json_gen_ctx {
    bool beautify;
    std::string out;
    const unsigned char *cached;
    std::vector<Frame> stack;
};

static void appendEscaped(std::string *dst, const unsigned char *str, size_t len) {
    static const char hex[] = "0123456789abcdef";
    auto appendByteEscape = [&](unsigned char c) {
        dst->append("\\u00");
        dst->push_back(hex[(c >> 4) & 0x0f]);
        dst->push_back(hex[c & 0x0f]);
    };

    dst->push_back('"');
    for (size_t i = 0; i < len; ) {
        const unsigned char c = str[i];
        switch (c) {
            case '"': *dst += "\\\""; break;
            case '\\': *dst += "\\\\"; break;
            case '\b': *dst += "\\b"; break;
            case '\f': *dst += "\\f"; break;
            case '\n': *dst += "\\n"; break;
            case '\r': *dst += "\\r"; break;
            case '\t': *dst += "\\t"; break;
            default:
                if (c < 0x20) {
                    appendByteEscape(c);
                    ++i;
                } else if (c < 0x80) {
                    dst->push_back(static_cast<char>(c));
                    ++i;
                } else {
                    size_t need = 0;
                    bool validLead = true;
                    if ((c & 0xE0) == 0xC0) {
                        need = 2;
                        if (c < 0xC2) validLead = false;
                    } else if ((c & 0xF0) == 0xE0) {
                        need = 3;
                    } else if ((c & 0xF8) == 0xF0) {
                        need = 4;
                        if (c > 0xF4) validLead = false;
                    } else {
                        validLead = false;
                    }

                    if (!validLead || i + need > len) {
                        appendByteEscape(c);
                        ++i;
                        break;
                    }

                    bool valid = true;
                    for (size_t j = 1; j < need; ++j) {
                        if ((str[i + j] & 0xC0) != 0x80) {
                            valid = false;
                            break;
                        }
                    }
                    if (valid && need == 3) {
                        const unsigned char b1 = str[i + 1];
                        if ((c == 0xE0 && b1 < 0xA0) || (c == 0xED && b1 >= 0xA0)) {
                            valid = false;
                        }
                    }
                    if (valid && need == 4) {
                        const unsigned char b1 = str[i + 1];
                        if ((c == 0xF0 && b1 < 0x90) || (c == 0xF4 && b1 >= 0x90)) {
                            valid = false;
                        }
                    }

                    if (!valid) {
                        appendByteEscape(c);
                        ++i;
                        break;
                    }

                    dst->append(reinterpret_cast<const char *>(str + i), need);
                    i += need;
                }
                break;
        }
        if (c == '"' || c == '\\' || c == '\b' || c == '\f' || c == '\n' || c == '\r' || c == '\t') {
            ++i;
        }
    }
    dst->push_back('"');
}

static yajl_gen_status beginToken(msc_json_gen_ctx *g) {
    if (g->stack.empty()) {
        return g->out.empty() ? yajl_gen_status_ok : yajl_gen_status_error;
    }

    Frame &f = g->stack.back();
    if (f.kind == Frame::Map) {
        if (f.expectingKey) {
            if (!f.first) {
                g->out.push_back(',');
            }
            f.first = false;
        } else {
            g->out.push_back(':');
        }
        return yajl_gen_status_ok;
    }

    if (!f.first) {
        g->out.push_back(',');
    }
    f.first = false;
    return yajl_gen_status_ok;
}

static void endToken(msc_json_gen_ctx *g) {
    if (!g->stack.empty() && g->stack.back().kind == Frame::Map) {
        g->stack.back().expectingKey = !g->stack.back().expectingKey;
    }
}

extern "C" {

const char *msc_json_c_backend_name(void) {
#ifdef MSC_JSON_C_BACKEND_YYJSON
    return "yyjson";
#elif defined(MSC_JSON_C_BACKEND_JSONC)
    return "json-c";
#elif defined(MSC_JSON_C_BACKEND_JANSSON)
    return "jansson";
#elif defined(MSC_JSON_C_BACKEND_CJSON)
    return "cjson";
#elif defined(MSC_JSON_C_BACKEND_NONE)
    return "none";
#else
    return "legacy";
#endif
}

const char *msc_json_c_backend_origin(void) {
#ifdef MSC_JSON_C_BACKEND_SYSTEM
    return "system";
#elif defined(MSC_JSON_C_BACKEND_VENDORED)
    return "vendored";
#else
    return "builtin";
#endif
}

const char *msc_json_cxx_backend_name(void) {
#ifdef MSC_JSON_CXX_BACKEND_NLOHMANNJSON
    return "nlohmannjson";
#elif defined(MSC_JSON_CXX_BACKEND_JSONCPP)
    return "jsoncpp";
#elif defined(MSC_JSON_CXX_BACKEND_JSONCONS)
    return "jsoncons";
#elif defined(MSC_JSON_CXX_BACKEND_SIMDJSON)
    return "simdjson";
#elif defined(MSC_JSON_CXX_BACKEND_GLAZE)
    return "glaze";
#else
    return "none";
#endif
}

const char *msc_json_cxx_backend_origin(void) {
#ifdef MSC_JSON_CXX_BACKEND_SYSTEM
    return "system";
#elif defined(MSC_JSON_CXX_BACKEND_VENDORED)
    return "vendored";
#else
    return "n/a";
#endif
}

uint32_t msc_json_c_capabilities(void) {
#ifdef MSC_JSON_C_BACKEND_NONE
    return 0;
#else
    return MSC_JSON_CAP_WRITER | MSC_JSON_CAP_DOM | MSC_JSON_CAP_EVENTS;
#endif
}

uint32_t msc_json_cxx_capabilities(void) {
#ifdef MSC_JSON_CXX_BACKEND_NONE
    return 0;
#else
    return MSC_JSON_CAP_DOM | MSC_JSON_CAP_EVENTS | MSC_JSON_CAP_TYPED_BINDING;
#endif
}

yajl_gen yajl_gen_alloc(const void *allocFuncs) {
    (void)allocFuncs;
    return new msc_json_gen_ctx{false, "", nullptr, {}};
}

void yajl_gen_free(yajl_gen g) {
    delete g;
}

yajl_gen_status yajl_gen_config(yajl_gen g, int opt, ...) {
    if (g == nullptr) {
        return yajl_gen_status_error;
    }
    if (opt == yajl_gen_beautify) {
        va_list ap;
        va_start(ap, opt);
        g->beautify = va_arg(ap, int) != 0;
        va_end(ap);
    }
    return yajl_gen_status_ok;
}

yajl_gen_status yajl_gen_map_open(yajl_gen g) {
    if (g == nullptr || beginToken(g) != yajl_gen_status_ok) {
        return yajl_gen_status_error;
    }
    g->out.push_back('{');
    g->stack.push_back({Frame::Map, true, true});
    return yajl_gen_status_ok;
}

yajl_gen_status yajl_gen_map_close(yajl_gen g) {
    if (g == nullptr || g->stack.empty() || g->stack.back().kind != Frame::Map || !g->stack.back().expectingKey) {
        return yajl_gen_status_error;
    }
    g->stack.pop_back();
    g->out.push_back('}');
    endToken(g);
    return yajl_gen_status_ok;
}

yajl_gen_status yajl_gen_array_open(yajl_gen g) {
    if (g == nullptr || beginToken(g) != yajl_gen_status_ok) {
        return yajl_gen_status_error;
    }
    g->out.push_back('[');
    g->stack.push_back({Frame::Array, true, false});
    return yajl_gen_status_ok;
}

yajl_gen_status yajl_gen_array_close(yajl_gen g) {
    if (g == nullptr || g->stack.empty() || g->stack.back().kind != Frame::Array) {
        return yajl_gen_status_error;
    }
    g->stack.pop_back();
    g->out.push_back(']');
    endToken(g);
    return yajl_gen_status_ok;
}

yajl_gen_status yajl_gen_string(yajl_gen g, const unsigned char *str, size_t len) {
    if (g == nullptr || str == nullptr || beginToken(g) != yajl_gen_status_ok) {
        return yajl_gen_status_error;
    }
    appendEscaped(&g->out, str, len);
    endToken(g);
    return yajl_gen_status_ok;
}

yajl_gen_status yajl_gen_number(yajl_gen g, const char *str, size_t len) {
    if (g == nullptr || str == nullptr || beginToken(g) != yajl_gen_status_ok) {
        return yajl_gen_status_error;
    }
    g->out.append(str, len);
    endToken(g);
    return yajl_gen_status_ok;
}

yajl_gen_status yajl_gen_integer(yajl_gen g, long long num) {
    char buffer[64];
    const int written = std::snprintf(buffer, sizeof(buffer), "%lld", num);
    if (written <= 0) {
        return yajl_gen_status_error;
    }
    return yajl_gen_number(g, buffer, static_cast<size_t>(written));
}

yajl_gen_status yajl_gen_get_buf(yajl_gen g, const unsigned char **buf, size_t *len) {
    if (g == nullptr || buf == nullptr || len == nullptr || !g->stack.empty()) {
        return yajl_gen_status_error;
    }
    g->cached = reinterpret_cast<const unsigned char *>(g->out.c_str());
    *buf = g->cached;
    *len = g->out.size();
    return yajl_gen_status_ok;
}

}  // extern "C"
