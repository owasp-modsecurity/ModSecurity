#ifndef SRC_JSON_JSON_BACKEND_H_
#define SRC_JSON_JSON_BACKEND_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct msc_json_gen_ctx *yajl_gen;

typedef enum {
    yajl_gen_status_ok = 0,
    yajl_gen_status_error = 1
} yajl_gen_status;

enum {
    yajl_gen_beautify = 1
};

typedef struct {
    unsigned int version;
    void *(*malloc)(void *ctx, size_t sz);
    void *(*realloc)(void *ctx, void *ptr, size_t sz);
    void (*free)(void *ctx, void *ptr);
    void *ctx;
} yajl_alloc_funcs;

typedef enum {
    MSC_JSON_CAP_DOM = 1 << 0,
    MSC_JSON_CAP_EVENTS = 1 << 1,
    MSC_JSON_CAP_INCREMENTAL = 1 << 2,
    MSC_JSON_CAP_WRITER = 1 << 3,
    MSC_JSON_CAP_ON_DEMAND = 1 << 4,
    MSC_JSON_CAP_TYPED_BINDING = 1 << 5
} msc_json_capability;

const char *msc_json_c_backend_name(void);
const char *msc_json_c_backend_origin(void);
const char *msc_json_cxx_backend_name(void);
const char *msc_json_cxx_backend_origin(void);
uint32_t msc_json_c_capabilities(void);
uint32_t msc_json_cxx_capabilities(void);

yajl_gen yajl_gen_alloc(const yajl_alloc_funcs *allocFuncs);
void yajl_gen_free(yajl_gen g);
yajl_gen_status yajl_gen_config(yajl_gen g, int opt, ...);
yajl_gen_status yajl_gen_map_open(yajl_gen g);
yajl_gen_status yajl_gen_map_close(yajl_gen g);
yajl_gen_status yajl_gen_array_open(yajl_gen g);
yajl_gen_status yajl_gen_array_close(yajl_gen g);
yajl_gen_status yajl_gen_string(yajl_gen g, const unsigned char *str, size_t len);
yajl_gen_status yajl_gen_number(yajl_gen g, const char *str, size_t len);
yajl_gen_status yajl_gen_integer(yajl_gen g, long long num);
yajl_gen_status yajl_gen_get_buf(yajl_gen g, const unsigned char **buf, size_t *len);

#ifdef __cplusplus
}
#endif

#endif
