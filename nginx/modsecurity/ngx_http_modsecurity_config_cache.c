#include "ngx_http_modsecurity_config_cache.h"

typedef struct {
    // Must be the first member of the structure because of pointer casts 
    // used by insert/lookup routines.
    ngx_rbtree_node_t         rbnode;

    ngx_str_t                 path;
    directory_config          *config;
} ngx_http_modsecurity_config_cache_node_t;

/*
 * Implemented similarly to ngx_string.c#ngx_str_rbtree_insert_value()
 */
static void
ngx_http_modsecurity_config_cache_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t  **p = NULL;
    for ( ;; ) {

        ngx_http_modsecurity_config_cache_node_t *n = (ngx_http_modsecurity_config_cache_node_t*) node;
        ngx_http_modsecurity_config_cache_node_t *t = (ngx_http_modsecurity_config_cache_node_t*) temp;

        if (node->key != temp->key) {

            p = (node->key < temp->key) ? &temp->left : &temp->right;

        } else if (n->path.len != t->path.len) {

            p = (n->path.len < t->path.len) ? &temp->left : &temp->right;

        } else {
            p = (ngx_strncasecmp(n->path.data, t->path.data, n->path.len) < 0)
                 ? &temp->left : &temp->right;
        }

        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}

ngx_http_modsecurity_config_cache_t *ngx_http_modsecurity_config_cache_init(ngx_conf_t *cf)
{
    ngx_rbtree_node_t *sentinel = ngx_pcalloc(cf->pool, sizeof(ngx_rbtree_node_t));
    if (sentinel == NULL) {
        return NULL;
    }

    ngx_http_modsecurity_config_cache_t *cache = ngx_pcalloc(cf->pool, sizeof(ngx_http_modsecurity_config_cache_t));
    if (cache == NULL) {
        return NULL;
    }

    ngx_rbtree_init(&cache->rbtree, sentinel, ngx_http_modsecurity_config_cache_rbtree_insert_value);
    return cache;
}

ngx_int_t ngx_http_modsecurity_config_cache_insert(
    ngx_http_modsecurity_config_cache_t* cache, ngx_conf_t *cf,
    ngx_str_t *path, directory_config *config)
{
    ngx_http_modsecurity_config_cache_node_t *node = ngx_pcalloc(cf->pool, sizeof(ngx_http_modsecurity_config_cache_node_t));
    if (node == NULL) {
        return NGX_ERROR;
    }

    node->path.data = ngx_pstrdup(cf->pool, path);
    node->path.len = path->len;
    node->rbnode.key = ngx_crc32_long(path->data, path->len);
    node->config = config;

    ngx_rbtree_insert(&cache->rbtree, &node->rbnode);
    return NGX_OK;
}

/*
 * Implemented similarly to ngx_string.c#ngx_str_rbtree_lookup()
 */
directory_config *ngx_http_modsecurity_config_cache_lookup(
    ngx_http_modsecurity_config_cache_t *cache, ngx_str_t *path)
{
    ngx_uint_t hash = ngx_crc32_long(path->data, path->len);
    ngx_rbtree_node_t *node = cache->rbtree.root;
    ngx_rbtree_node_t *sentinel = cache->rbtree.sentinel;

    while (node != sentinel) {
        ngx_http_modsecurity_config_cache_node_t *n = (ngx_http_modsecurity_config_cache_node_t *) node;

        if (hash != node->key) {
            node = (hash < node->key) ? node->left : node->right;
            continue;
        }

        if (path->len != n->path.len) {
            node = (path->len < n->path.len) ? node->left : node->right;
            continue;
        }

        ngx_int_t rc = ngx_memcmp(path->data, n->path.data, path->len);

        if (rc < 0) {
            node = node->left;
            continue;
        }

        if (rc > 0) {
            node = node->right;
            continue;
        }

        return n->config;
    }

    return NULL;

}
