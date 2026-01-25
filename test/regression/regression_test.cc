/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */

#include "test/regression/regression_test.h"

#include <string.h>

#include <sstream>
#include <unordered_map>
#include <string>

#ifdef WITH_YAJL
#include <yajl/yajl_gen.h>
#endif

namespace modsecurity_test {

std::string RegressionTest::print() {
    std::stringstream i;

#if 0
    i << KRED << "Test failed." << RESET << " From: " \
    i << this->filename << std::endl;
    i << "{" << std::endl;
    i << "  \"ret\": \"" << this->ret << "\"" << std::endl;
    i << "  \"type\": \"" << this->type << "\"" << std::endl;
    i << "  \"name\": \"" << this->name << "\"" << std::endl;
    i << "  \"input\": \"" << this->input << "\"" << std::endl;
    i << "  \"param\": \"" << this->param << "\"" << std::endl;
    i << "}" << std::endl;
    i << "Expecting: " << this->ret << " - operator returned: " << \
        this->obtained << std::endl;
#endif
    return i.str();
}


inline std::string RegressionTest::yajl_array_to_str(const yajl_val &node) {
    std::stringstream i;
    for (int z = 0; z < node->u.array.len; z++) {
        yajl_val val3 = node->u.array.values[z];
        const char *key = YAJL_GET_STRING(val3);
        i << key;
    }
    return i.str();
}


inline std::vector<std::string> RegressionTest::yajl_array_to_vec_str(
    const yajl_val &node) {
    std::vector<std::string> vec;
    for (int z = 0; z < node->u.array.len; z++) {
        yajl_val val3 = node->u.array.values[z];
        const char *key = YAJL_GET_STRING(val3);
        vec.push_back(key);
    }
    return vec;
}


inline std::vector<std::pair<std::string, std::string>>
    RegressionTest::yajl_array_to_map(const yajl_val &node) {
    std::vector<std::pair<std::string, std::string>> vec;
    for (int z = 0; z < node->u.object.len; z++) {
        const char *key = node->u.object.keys[z];
        yajl_val val3 = node->u.object.values[z];
        const char *value = YAJL_GET_STRING(val3);
        std::pair<std::string, std::string> a(key, value);
        vec.push_back(a);
    }
    return vec;
}


RegressionTest *RegressionTest::from_yajl_node(const yajl_val &node) {
    size_t nelem = node->u.object.len;
    RegressionTest *u = new RegressionTest();
    u->http_code = 200;

    for (int i = 0; i < nelem; i++) {
        const char *key = node->u.object.keys[ i ];
        yajl_val val = node->u.object.values[ i ];

        if (strcmp(key, "enabled") == 0) {
           u->enabled = YAJL_GET_INTEGER(val);
        }
        if (strcmp(key, "version_min") == 0) {
            u->version_min = YAJL_GET_INTEGER(val);
        }
        if (strcmp(key, "version_max") == 0) {
            u->version_max = YAJL_GET_INTEGER(val);
        }
        if (strcmp(key, "title") == 0) {
           u->title = YAJL_GET_STRING(val);
        }
        if (strcmp(key, "url") == 0) {
           u->url = YAJL_GET_STRING(val);
        }
        if (strcmp(key, "resource") == 0) {
           u->resource = YAJL_GET_STRING(val);
        }
        if (strcmp(key, "github_issue") == 0) {
            u->github_issue = YAJL_GET_INTEGER(val);
        }
        if (strcmp(key, "client") == 0) {
            for (int j = 0; j < val->u.object.len; j++) {
                const char *key2 = val->u.object.keys[j];
                yajl_val val2 = val->u.object.values[j];

                if (strcmp(key2, "ip") == 0) {
                    u->clientIp = YAJL_GET_STRING(val2);
                }
                if (strcmp(key2, "port") == 0) {
                    u->clientPort = YAJL_GET_INTEGER(val2);
                }
            }
        }
        if (strcmp(key, "server") == 0) {
            for (int j = 0; j < val->u.object.len; j++) {
                const char *key2 = val->u.object.keys[j];
                yajl_val val2 = val->u.object.values[j];

                if (strcmp(key2, "ip") == 0) {
                    u->serverIp = YAJL_GET_STRING(val2);
                }
                if (strcmp(key2, "port") == 0) {
                    u->serverPort = YAJL_GET_INTEGER(val2);
                }
                if (strcmp(key2, "hostname") == 0) {
                    u->hostname = YAJL_GET_STRING(val2);
                }
            }
        }
        if (strcmp(key, "request") == 0) {
            for (int j = 0; j < val->u.object.len; j++) {
                const char *key2 = val->u.object.keys[j];
                yajl_val val2 = val->u.object.values[j];

                if (strcmp(key2, "uri") == 0) {
                   u->uri = YAJL_GET_STRING(val2);
                }
                if (strcmp(key2, "method") == 0) {
                   u->method = YAJL_GET_STRING(val2);
                }
                if (strcmp(key2, "http_version") == 0) {
                   u->httpVersion = YAJL_GET_NUMBER(val2);
                }
                if (strcmp(key2, "headers") == 0) {
                    u->request_headers = yajl_array_to_map(val2);
                }
                if (strcmp(key2, "body") == 0) {
                    u->request_body = yajl_array_to_str(val2);
                }
            }
        }
        if (strcmp(key, "response") == 0) {
            for (int j = 0; j < val->u.object.len; j++) {
                const char *key2 = val->u.object.keys[j];
                yajl_val val2 = val->u.object.values[j];

                if (strcmp(key2, "headers") == 0) {
                    u->response_headers = yajl_array_to_map(val2);
                }
                if (strcmp(key2, "body") == 0) {
                    u->response_body = yajl_array_to_str(val2);
                }
                if (strcmp(key2, "protocol") == 0) {
                    u->response_protocol = YAJL_GET_STRING(val2);
                }
            }
        }
        if (strcmp(key, "expected") == 0) {
            for (int j = 0; j < val->u.object.len; j++) {
                const char *key2 = val->u.object.keys[j];
                yajl_val val2 = val->u.object.values[j];

                if (strcmp(key2, "audit_log") == 0) {
                    u->audit_log = YAJL_GET_STRING(val2);
                }
                if (strcmp(key2, "debug_log") == 0) {
                    u->debug_log = YAJL_GET_STRING(val2);
                }
                if (strcmp(key2, "error_log") == 0) {
                    u->error_log = YAJL_GET_STRING(val2);
                }
                if (strcmp(key2, "http_code") == 0) {
                    u->http_code = YAJL_GET_INTEGER(val2);
                }
                if (strcmp(key2, "redirect_url") == 0) {
                    u->redirect_url = YAJL_GET_STRING(val2);
                }
                if (strcmp(key2, "parser_error") == 0) {
                    u->parser_error = YAJL_GET_STRING(val2);
                }
            }
        }
        if (strcmp(key, "rules") == 0) {
            std::stringstream si;
            for (int j = 0; j < val->u.array.len; j++) {
                yajl_val val2 = val->u.array.values[ j ];
                const char *keyj = YAJL_GET_STRING(val2);
                si << keyj << "\n";
            }
            u->rules = si.str();
        }
    }

    u->name = u->title;

    return u;
}

RegressionTests *RegressionTests::from_yajl_node(const yajl_val &node) {
    RegressionTests *u = new RegressionTests(node);
    size_t num_tests = node->u.array.len;
    for ( int i = 0; i < num_tests; i++ ) {
        yajl_val obj = node->u.array.values[i];
        u->tests.emplace_back(*RegressionTest::from_yajl_node(obj));
    }
    return u;
}

RegressionTests::~RegressionTests() {
#ifdef WITH_YAJL
    yajl_tree_free(node);
#endif
}

#ifdef WITH_YAJL

static yajl_gen_status jayl_gen_string_view(yajl_gen g, std::string_view s) {
    return yajl_gen_string(g, reinterpret_cast<const unsigned char *>(s.data()), s.length());
}

static yajl_gen_status jayl_gen_key_val(yajl_gen g, std::string_view key, std::string_view val) {
    auto s = jayl_gen_string_view(g, key);
    if (s != yajl_gen_status_ok) {
        return s;
    }
    return jayl_gen_string_view(g, val);
}

static yajl_gen_status copy_number(yajl_gen g, std::string_view key, yajl_val val) {
    if (!YAJL_IS_NUMBER(val)) {
        std::cerr << "error: " << key << " must be number.\n";
        exit(1);
    }
    auto s = jayl_gen_string_view(g, key);
    if (s != yajl_gen_status_ok) {
        return s;
    }
    return yajl_gen_number(g,
        reinterpret_cast<const char *>(val->u.number.r),
        strlen(val->u.number.r));
}

static yajl_gen_status copy_string(yajl_gen g, std::string_view key, yajl_val val) {
    if (!YAJL_IS_STRING(val)) {
        std::cerr << "error: " << key << " must be string.\n";
        exit(1);
    }
    return jayl_gen_key_val(g, key, val->u.string);
}

static void ensure_obj(std::string_view key, yajl_val obj) {
    if (!YAJL_IS_OBJECT(obj)) {
        std::cerr << "error: " << key << " must be object.\n";
        exit(1);
    }
}

static void copy_str_map(yajl_gen g, std::string_view key, yajl_val val) {
    if (!YAJL_IS_OBJECT(val)) {
        std::cerr << "error: " << key << " must be object.\n";
        exit(1);
    }
    jayl_gen_string_view(g, key);
    yajl_gen_map_open(g);
    for (size_t i = 0; i < val->u.object.len; ++i) {
        const char *key2 = val->u.object.keys[i];
        yajl_val val2 = val->u.object.values[i];
        copy_string(g, key2, val2);
    }
    yajl_gen_map_close(g);
}

static void copy_str_array(yajl_gen g, std::string_view key, yajl_val val) {
    if (!YAJL_IS_ARRAY(val)) {
        std::cerr << "error: " << key << " must be array.\n";
        exit(1);
    }
    jayl_gen_string_view(g, key);
    yajl_gen_array_open(g);
    for (size_t i = 0; i < val->u.array.len; ++i) {
        yajl_val val2 = val->u.array.values[i];
        if (!YAJL_IS_STRING(val2)) {
            std::cerr << "error: array element of " << key << " must be string.\n";
            exit(1);
        }
        jayl_gen_string_view(g, val2->u.string);
    }
    yajl_gen_array_close(g);
}

static void copy_body(yajl_gen g, std::string_view key, yajl_val val) {
    if (YAJL_IS_STRING(val)) {
        jayl_gen_key_val(g, key, val->u.string);
    } else {
        copy_str_array(g, key, val);
    }
}

std::string RegressionTests::toJSON() {
    const unsigned char *buf;
    size_t len;
    yajl_gen g;

    g = yajl_gen_alloc(NULL);
    if (g == NULL) {
        return "";
    }
    yajl_gen_config(g, yajl_gen_beautify, 1);

    if (!YAJL_IS_ARRAY(node)) {
        std::cerr << "error: toplevel must be array.\n";
        exit(1);
    }

    yajl_gen_array_open(g);
    for (size_t i = 0; i < node->u.array.len; ++i) {
        yajl_val test_obj = node->u.array.values[i];
        ensure_obj("test", test_obj);
        yajl_gen_map_open(g);
        for (size_t j = 0; j < test_obj->u.object.len; ++j) {
            const char *key = test_obj->u.object.keys[j];
            yajl_val val = test_obj->u.object.values[j];
            if (strcmp(key, "enabled") == 0
                || strcmp(key, "version_min") == 0
                || strcmp(key, "version_max") == 0
                || strcmp(key, "github_issue") == 0) {
                copy_number(g, key, val);
            } else if (strcmp(key, "title") == 0
                || strcmp(key, "url") == 0
                || strcmp(key, "resource") == 0) {
                copy_string(g, key, val);
            } else if (strcmp(key, "client") == 0) {
                ensure_obj("client", val);
                jayl_gen_string_view(g, "client");
                yajl_gen_map_open(g);
                for (size_t k = 0; k < val->u.object.len; ++k) {
                    const char *key2 = val->u.object.keys[k];
                    yajl_val val2 = val->u.object.values[k];
                    if (strcmp(key2, "ip") == 0) {
                        copy_string(g, key2, val2);
                    } else if (strcmp(key2, "port") == 0) {
                        copy_number(g, key2, val2);
                    }
                }
                yajl_gen_map_close(g);
            } else if (strcmp(key, "server") == 0) {
                ensure_obj("server", val);
                jayl_gen_string_view(g, "server");
                yajl_gen_map_open(g);
                for (size_t k = 0; k < val->u.object.len; ++k) {
                    const char *key2 = val->u.object.keys[k];
                    yajl_val val2 = val->u.object.values[k];
                    if (strcmp(key2, "ip") == 0
                        || strcmp(key2, "hostname") == 0) {
                        copy_string(g, key2, val2);
                    } else if (strcmp(key2, "port") == 0) {
                        copy_number(g, key2, val2);
                    }
                }
                yajl_gen_map_close(g);
            } else if (strcmp(key, "request") == 0) {
                ensure_obj("request", val);
                jayl_gen_string_view(g, "request");
                yajl_gen_map_open(g);
                for (size_t k = 0; k < val->u.object.len; ++k) {
                    const char *key2 = val->u.object.keys[k];
                    yajl_val val2 = val->u.object.values[k];
                    if (strcmp(key2, "url") == 0
                        || strcmp(key2, "method") == 0) {
                        copy_string(g, key2, val2);
                    } else if (strcmp(key2, "http_version") == 0) {
                        copy_number(g, key2, val2);
                    } else if (strcmp(key2, "headers") == 0) {
                        copy_str_map(g, key2, val2);
                    } else if (strcmp(key2, "body") == 0) {
                        copy_body(g, key2, val2);
                    }
                }
                yajl_gen_map_close(g);
            } else if (strcmp(key, "response") == 0) {
                ensure_obj("response", val);
                jayl_gen_string_view(g, "response");
                yajl_gen_map_open(g);
                for (size_t k = 0; k < val->u.object.len; ++k) {
                    const char *key2 = val->u.object.keys[k];
                    yajl_val val2 = val->u.object.values[k];
                    if (strcmp(key2, "protocol") == 0) {
                        copy_string(g, key2, val2);
                    } else if (strcmp(key2, "headers") == 0) {
                        copy_str_map(g, key2, val2);
                    } else if (strcmp(key2, "body") == 0) {
                        copy_body(g, key2, val2);
                    }
                }
                yajl_gen_map_close(g);
            } else if (strcmp(key, "expected") == 0) {
                ensure_obj("expected", val);
                jayl_gen_string_view(g, "expected");
                yajl_gen_map_open(g);
                for (size_t k = 0; k < val->u.object.len; ++k) {
                    const char *key2 = val->u.object.keys[k];
                    yajl_val val2 = val->u.object.values[k];
                    if (strcmp(key2, "audit_log") == 0
                        || strcmp(key2, "debug_log") == 0
                        || strcmp(key2, "error_log") == 0
                        || strcmp(key2, "redirect_url") == 0
                        || strcmp(key2, "parser_error") == 0) {
                        copy_string(g, key2, val2);
                    } else if (strcmp(key2, "http_code") == 0) {
                        copy_number(g, key2, val2);
                    }
                }
                yajl_gen_map_close(g);
            } else if (strcmp(key, "rules") == 0) {
                copy_str_array(g, key, val);
            }
        }
        yajl_gen_map_close(g);
    }
    yajl_gen_array_close(g);

    yajl_gen_get_buf(g, &buf, &len);
    std::string s{reinterpret_cast<const char*>(buf), len};
    yajl_gen_free(g);
    return s;
}

#endif // WITH_YAJL

}  // namespace modsecurity_test
