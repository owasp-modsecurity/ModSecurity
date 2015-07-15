#include <string.h>

#include <yajl/yajl_gen.h>

#define yajl_string(g, s) yajl_gen_string(g, (const unsigned char *)s, strlen(s))

#define yajl_kv_null(g, k) yajl_string(g, k); yajl_gen_null(g)

#define yajl_kv_int(g, k, v) yajl_string(g, k); yajl_gen_integer(g, v)

#define yajl_kv_string(g, k, v) yajl_string(g, k); yajl_string(g, v)

#define yajl_kv_bool(g, k, v) yajl_string(g, k); yajl_gen_bool(g, v)
