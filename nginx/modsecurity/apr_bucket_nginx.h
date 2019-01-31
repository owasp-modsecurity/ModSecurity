#pragma once
#include <ngx_core.h>
#include "apr_buckets.h"

apr_bucket * apr_bucket_nginx_create(ngx_buf_t *buf, apr_bucket_alloc_t *list);

apr_bucket * apr_bucket_nginx_make(apr_bucket *e, ngx_buf_t *buf);

#define ngx_buf_to_apr_bucket apr_bucket_nginx_create

ngx_int_t copy_chain_to_brigade(ngx_chain_t *chain, apr_bucket_brigade *bb, ngx_int_t last_buf);
