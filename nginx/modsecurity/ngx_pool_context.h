

#ifndef _NGX_POOL_CONTEXT_H_INCLUDE_
#define _NGX_POOL_CONTEXT_H_INCLUDE_

void* ngx_pool_get_ctx(ngx_pool_t * pool, ngx_uint_t index);
ngx_int_t ngx_pool_set_ctx(ngx_pool_t * pool, ngx_uint_t index,void * data);

#define ngx_http_get_module_pool_ctx(r, module)     ngx_pool_get_ctx(r->pool, module.index)
#define ngx_http_set_pool_ctx(r, c, module)         ngx_pool_set_ctx(r->pool, module.index, c)

#endif /* _NGX_POOL_CONTEXT_H_INCLUDE_ */
