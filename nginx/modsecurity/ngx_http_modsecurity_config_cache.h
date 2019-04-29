#pragma once

#include <ngx_core.h>
#include <ngx_string.h>

/*
 * ModSecurity configuration cache is used to prevent loading configuration
 * multiple times if one and the same configuration file is applied in multiple
 * different contexts.
 *
 * The main problem with loading ModSecurity configuration independently for each 
 * context is that the allocated memory block can be quite large (10-20 Mb) so if, 
 * for example, the same configuration is used for 10-20 listeners it will cause 
 * significant memory overuse for no good reason.
 *
 * The cache helps alleviate this by storing pointers to configuration objects created
 * from ModSecurity configuration files and re-using existing objects when the same path
 * occurs several times during Nginx configuration loading.
 */

typedef struct directory_config directory_config;

typedef struct {
    ngx_rbtree_t              rbtree;
} ngx_http_modsecurity_config_cache_t;

/* Create configuration cache */
ngx_http_modsecurity_config_cache_t *ngx_http_modsecurity_config_cache_init(ngx_conf_t *cf);

/* Add configuration block for the respective file path into configuration cache */
ngx_int_t ngx_http_modsecurity_config_cache_insert(ngx_http_modsecurity_config_cache_t *cache,
    ngx_conf_t *cf, ngx_str_t *path, directory_config *config);

/* Look up configuration block for the respective file path in configuration cache */
directory_config *ngx_http_modsecurity_config_cache_lookup(ngx_http_modsecurity_config_cache_t *cache,
    ngx_str_t *path);
