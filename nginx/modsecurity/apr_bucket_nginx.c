
#include <apr_bucket_nginx.h>

static apr_status_t nginx_bucket_read(apr_bucket *b, const char **str,
                                      apr_size_t *len, apr_read_type_e block);
static void nginx_bucket_destroy(void *data);

static const apr_bucket_type_t apr_bucket_type_nginx = {
    "NGINX", 5, APR_BUCKET_DATA,
    nginx_bucket_destroy,
    nginx_bucket_read,
    apr_bucket_setaside_noop,
    apr_bucket_shared_split,
    apr_bucket_shared_copy
};


typedef struct apr_bucket_nginx {
    apr_bucket_refcount  refcount;
    ngx_buf_t           *buf;
} apr_bucket_nginx;

/* ngx_buf_t to apr_bucket */
apr_bucket * apr_bucket_nginx_create(ngx_buf_t *buf,
                                     apr_pool_t *p,
                                     apr_bucket_alloc_t *list)
{

    apr_bucket *b = apr_bucket_alloc(sizeof(*b), list);

    APR_BUCKET_INIT(b); /* link */
    b->free = apr_bucket_free;
    b->list = list;
    return apr_bucket_nginx_make(b, buf, p);
}

apr_bucket * apr_bucket_nginx_make(apr_bucket *b, ngx_buf_t *buf,
                                   apr_pool_t *pool)
{
    apr_bucket_nginx *n;

    n = apr_bucket_alloc(sizeof(*n), b->list);

    n->buf = buf;

    b = apr_bucket_shared_make(b, n, 0, ngx_buf_size(buf));
    b->type = &apr_bucket_type_nginx;
    return b;
}

static apr_status_t nginx_bucket_read(apr_bucket *b, const char **str,
                                      apr_size_t *len, apr_read_type_e block)
{
    apr_bucket_nginx *n = b->data;
    ngx_buf_t        *buf = n->buf;
    u_char           *data;
    ssize_t           size;

    if (buf->pos == NULL && ngx_buf_size(buf) != 0) {
        data = apr_bucket_alloc(ngx_buf_size(buf), b->list);
        if (data == NULL) {
            return APR_EGENERAL;
        }

        size = ngx_read_file(buf->file, data, ngx_buf_size(buf), buf->file_pos);
        if (size != ngx_buf_size(buf)) {
            apr_bucket_free(data);
            return APR_EGENERAL;
        }
        buf->pos = data;
    }

    *str = (char *)buf->pos + b->start;
    *len = b->length;

    return APR_SUCCESS;
}


static void nginx_bucket_destroy(void *data)
{
    apr_bucket_nginx *n = data;
    ngx_buf_t *buf = n->buf;

    if (apr_bucket_shared_destroy(n)) {
        if (!ngx_buf_in_memory(buf) && buf->pos != NULL) {
            apr_bucket_free(buf->pos);
            buf->pos = NULL;
        }
        apr_bucket_free(n);
    }
}


ngx_int_t
copy_chain_to_brigade(ngx_chain_t *chain, apr_bucket_brigade *bb, ngx_pool_t *pool, ngx_int_t last_buf) {
    apr_bucket         *e;

    while (chain) {
        e = ngx_buf_to_apr_bucket(chain->buf, bb->p, bb->bucket_alloc);
        if (e == NULL) {
            return NGX_ERROR;
        }

        APR_BRIGADE_INSERT_TAIL(bb, e);
        if (chain->buf->last_buf) {
            e = apr_bucket_eos_create(bb->bucket_alloc);
            APR_BRIGADE_INSERT_TAIL(bb, e);
            return NGX_OK;
        }

        chain = chain->next;
    }

    if (last_buf) {
        e = apr_bucket_eos_create(bb->bucket_alloc);
        APR_BRIGADE_INSERT_TAIL(bb, e);
        return NGX_OK;
    }

    return NGX_AGAIN;
}
