

#include <ngx_core.h>

#define NGX_POOL_CTX_SIZE  1024

typedef struct ngx_pool_context_node_s ngx_pool_context_node_t;
struct ngx_pool_context_node_s
{
    ngx_pool_context_node_t   *next;
    ngx_pool_context_node_t  **prev;
    ngx_pool_t                *pool;
    ngx_uint_t                 index;
    void                      *data;
};

static void
ngx_pool_context_cleanup(void *data);

typedef struct {
    ngx_uint_t                 size;
} ngx_pool_context_conf_t;

static void * ngx_pool_context_create_conf(ngx_cycle_t *cycle);
static char * ngx_pool_context_init_conf(ngx_cycle_t *cycle, void *conf);

static ngx_core_module_t  ngx_pool_context_module_ctx = {
    ngx_string("pool_context"),
    ngx_pool_context_create_conf,
    ngx_pool_context_init_conf,
};

static ngx_command_t  ngx_pool_context_commands[] = {

    {   ngx_string("pool_context_hash_size"),
        NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
        ngx_conf_set_num_slot,
        0,
        offsetof(ngx_pool_context_conf_t, size),
        NULL
    },
    ngx_null_command
};


ngx_module_t  ngx_pool_context_module = {
    NGX_MODULE_V1,
    &ngx_pool_context_module_ctx,          /* module context */
    ngx_pool_context_commands,             /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


#define ngx_pool_context_hash_key(r, ctx_index) ((ngx_uint_t) r + ctx_index)

#define ngx_pool_context_unlink(node) \
                                                                              \
    *(node->prev) = node->next;                                               \
                                                                              \
    if (node->next) {                                                         \
        node->next->prev = node->prev;                                        \
    }                                                                         \
                                                                              \
    node->prev = NULL;                                                        \
 

#define ngx_pool_context_link(queue, node)                                \
                                                                          \
    if (node->prev != NULL) {                                             \
        ngx_pool_context_unlink(node);                                    \
    }                                                                     \
    node->next = (ngx_pool_context_node_t *) *queue;                      \
    node->prev = (ngx_pool_context_node_t **) queue;                      \
    *queue = node;                                                        \
                                                                          \
    if (node->next) {                                                     \
        node->next->prev = &node->next;                                   \
    }


static ngx_pool_context_node_t **ngx_pool_context_hash;
static ngx_uint_t            ngx_pool_context_hash_size;

/* Nginx has removed multi-thread support, so we do not need mutex */

void *
ngx_pool_get_ctx(ngx_pool_t *pool, ngx_uint_t index)
{
    ngx_uint_t               hash;
    uint32_t                 key;
    ngx_pool_context_node_t *node;

    hash = (ngx_uint_t) pool + index;
    key = ngx_murmur_hash2((u_char *)&hash, sizeof(hash)) % ngx_pool_context_hash_size;

    node = ngx_pool_context_hash[key];

    while (node) {

        if (node->pool == pool && node->index == index) {

            return node->data;
        }
        node = node->next;
    }

    return NULL;

}


ngx_int_t
ngx_pool_set_ctx(ngx_pool_t *pool, ngx_uint_t index, void *data)
{
    ngx_uint_t              hash;
    uint32_t                key;
    ngx_pool_context_node_t *node;
    ngx_pool_cleanup_t     *cln;

    hash = (ngx_uint_t) pool + index;
    key = ngx_murmur_hash2((u_char *)&hash, sizeof(hash)) % ngx_pool_context_hash_size;

    node = ngx_pool_context_hash[key];

    while (node) {

        if (node->pool == pool
                && node->index == index) {


            node->data = data;
            return NGX_OK;
        }
        node = node->next;
    }

    cln = ngx_pool_cleanup_add(pool, sizeof(ngx_pool_context_node_t));

    if (cln == NULL) {

        return NGX_ERROR;
    }

    cln->handler = ngx_pool_context_cleanup;
    node = cln->data;

    node->prev = NULL;
    node->next = NULL;
    node->pool = pool;
    node->index = index;
    node->data = data;

    ngx_pool_context_link(&ngx_pool_context_hash[key], node);

    return NGX_OK;
}


static void
ngx_pool_context_cleanup(void *data)
{
    ngx_pool_context_node_t *node = data;

    ngx_pool_context_unlink(node);

}


static void *
ngx_pool_context_create_conf(ngx_cycle_t *cycle)
{
    ngx_pool_context_conf_t     *pcf;

    /* create config  */
    pcf = ngx_pcalloc(cycle->pool, sizeof(ngx_pool_context_conf_t));
    if (pcf == NULL) {
        return NULL;
    }

    pcf->size = NGX_CONF_UNSET_UINT;

    return pcf;
}


static char *
ngx_pool_context_init_conf(ngx_cycle_t *cycle, void *conf)
{
    ngx_pool_context_conf_t      *pcf = conf;

    ngx_conf_init_uint_value(pcf->size, NGX_POOL_CTX_SIZE);

    ngx_pool_context_hash_size = pcf->size;

    ngx_pool_context_hash = ngx_pcalloc(cycle->pool, sizeof(ngx_pool_context_node_t *) * ngx_pool_context_hash_size);

    if (ngx_pool_context_hash == NULL) {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}


