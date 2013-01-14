
#include <ngx_core.h>
#include "apr_buckets.h"

apr_bucket * apr_bucket_nginx_create(ngx_buf_t *buf,
                                     apr_pool_t *p,
                                     apr_bucket_alloc_t *list);

apr_bucket * apr_bucket_nginx_make(apr_bucket *e, ngx_buf_t *buf,
                                   apr_pool_t *pool);

#define ngx_buf_to_apr_bucket apr_bucket_nginx_create

ngx_buf_t * apr_bucket_to_ngx_buf(apr_bucket *e, ngx_pool_t *pool);


apr_bucket_brigade *
ngx_chain_to_apr_brigade(ngx_chain_t *chain, apr_pool_t *p, apr_bucket_alloc_t *list);


/* apr_brigade_destory could be called after converted */
ngx_chain_t *
apr_brigade_to_ngx_chain(apr_bucket_brigade *b, ngx_pool_t *pool);

