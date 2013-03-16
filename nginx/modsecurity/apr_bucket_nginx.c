
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

ngx_buf_t * apr_bucket_to_ngx_buf(apr_bucket *e, ngx_pool_t *pool) {
    ngx_buf_t         *buf, *b;
    apr_bucket_nginx  *n;
    ngx_uint_t         len;
    u_char            *data;

    if (e->type->is_metadata) {
        return NULL;
    }

    if (e->type == &apr_bucket_type_nginx) {
        n = e->data;
        b = n->buf;

        /* whole buf */
        if (e->length == (apr_size_t)ngx_buf_size(b)) {
            b->last_buf = 0;
            return b;
        }

        buf = ngx_palloc(pool, sizeof(ngx_buf_t));
        if (buf == NULL) {
            return NULL;
        }
        ngx_memcpy(buf, b, sizeof(ngx_buf_t));

        if (ngx_buf_in_memory(buf)) {
            buf->start = buf->pos = buf->pos + e->start;
            buf->end = buf->last = buf->pos + e->length;
        } else {
            buf->pos = NULL;
            buf->file_pos += e->start;
            buf->file_last = buf->file_pos + e->length;
        }

        buf->last_buf = 0;
        return buf;
    }

    if (apr_bucket_read(e, (const char **)&data,
                        &len, APR_BLOCK_READ) != APR_SUCCESS) {
        return NULL;
    }

    buf = ngx_calloc_buf(pool);
    if (buf == NULL) {
        return NULL;
    }

    if (e->type == &apr_bucket_type_pool) {
        buf->start = data;
    } else if (len != 0) {
        buf->start = ngx_palloc(pool, len);
        ngx_memcpy(buf->start, data, len);
    }

    buf->pos = buf->start;
    buf->end = buf->last = buf->start + len;
    buf->temporary = 1;
    return buf;
}

ngx_int_t
move_chain_to_brigade(ngx_chain_t *chain, apr_bucket_brigade *bb, ngx_pool_t *pool, ngx_int_t last_buf) {
    apr_bucket         *e;
    ngx_chain_t        *cl;

    while (chain) {
        e = ngx_buf_to_apr_bucket(chain->buf, bb->p, bb->bucket_alloc);
        if (e == NULL) {
            return NGX_ERROR;
        }

        APR_BRIGADE_INSERT_TAIL(bb, e);
        if (chain->buf->last_buf) {
            e = apr_bucket_eos_create(bb->bucket_alloc);
            APR_BRIGADE_INSERT_TAIL(bb, e);
            chain->buf->last_buf = 0;
            return NGX_OK;
        }
        cl = chain;
        chain = chain->next;
        ngx_free_chain(pool, cl);
    }

    if (last_buf) {
        e = apr_bucket_eos_create(bb->bucket_alloc);
        APR_BRIGADE_INSERT_TAIL(bb, e);
        return NGX_OK;
    }

    return NGX_AGAIN;
}

ngx_int_t
move_brigade_to_chain(apr_bucket_brigade *bb, ngx_chain_t **ll, ngx_pool_t *pool) {
    apr_bucket  *e;
    ngx_buf_t   *buf;
    ngx_chain_t *cl;

    cl = NULL;

    if (APR_BRIGADE_EMPTY(bb)) {
        *ll = NULL;
        return NGX_OK;
    }

    for (e = APR_BRIGADE_FIRST(bb);
            e != APR_BRIGADE_SENTINEL(bb);
            e = APR_BUCKET_NEXT(e)) {

        if (APR_BUCKET_IS_EOS(e)) {
            if (cl == NULL) {
                *ll = cl;
            } else {
                cl->buf->last_buf = 1;
            }
            apr_brigade_cleanup(bb);
            return NGX_OK;
        }

        if (APR_BUCKET_IS_METADATA(e)) {
            continue;
        }

        buf = apr_bucket_to_ngx_buf(e, pool);
        if (buf == NULL) {
            break;
        }

        cl = ngx_alloc_chain_link(pool);
        if (cl == NULL) {
            break;
        }

        cl->buf = buf;
        cl->next = NULL;
        *ll = cl;
        ll = &cl->next;
    }

    apr_brigade_cleanup(bb);
    /* no eos or error */
    return NGX_ERROR;
}

