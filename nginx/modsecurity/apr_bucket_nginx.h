#pragma once
#include <ngx_core.h>
#include "apr_buckets.h"

apr_bucket * apr_bucket_nginx_create(ngx_buf_t *buf,
                                     apr_pool_t *p,
                                     apr_bucket_alloc_t *list);

apr_bucket * apr_bucket_nginx_make(apr_bucket *e, ngx_buf_t *buf,
                                   apr_pool_t *pool);

#define ngx_buf_to_apr_bucket apr_bucket_nginx_create

ngx_buf_t * apr_bucket_to_ngx_buf(apr_bucket *e, ngx_pool_t *pool);

ngx_int_t move_chain_to_brigade(ngx_chain_t *chain, apr_bucket_brigade *bb, ngx_pool_t *pool, ngx_int_t last_buf);
ngx_int_t move_brigade_to_chain(apr_bucket_brigade *bb, ngx_chain_t **chain, ngx_pool_t *pool);

