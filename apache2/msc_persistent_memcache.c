/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2013 Trustwave Holdings, Inc. (http://www.trustwave.com/)
*
* You may not use this file except in compliance with
* the License.  You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* If any of the files related to licensing are missing or if you have any
* other questions related to licensing please contact Trustwave Holdings, Inc.
* directly using the email address security@modsecurity.org.
*/

#include "msc_persistent_memcache.h"

#ifdef WITH_LIBMEMCACHED
#include <memcached.h>
#endif

#define MEMCACHE_INDEX_ELEMENT "__index"


static char *msc_collection_default_keys[]= {
    "__expire_KEY",
    "__key",
    "__name",
    MEMCACHE_INDEX_ELEMENT,
    "CREATE_TIME",
    "LAST_UPDATE_TIME",
    "TIMEOUT",
    "UPDATE_COUNTER",
    "UPDATE_RATE",
    "KEY",
    NULL
};


int memcache_is_collection_default_key(const char *key)
{
#ifdef WITH_LIBMEMCACHED
    char **keys = msc_collection_default_keys;
    char *k = NULL;

    while (k = *keys++) {
        if (strcmp(key, k) == 0) {
            return 1;
        }
    }
#endif
    return 0;
}


static int memcache_store_key(modsec_rec *msr, const msc_string *var_name,
        const msc_string *var_key, const char *key, msc_string* var,
        const time_t timeout)
{
#ifdef WITH_LIBMEMCACHED
    char memc_key[MEMCACHED_MAX_KEY];
    memcached_return rc;

    snprintf(memc_key, MEMCACHED_MAX_KEY, "%s,%s,%s", var_name->value,
            var_key->value, key);

    rc = memcached_set(memcache, memc_key, strlen(memc_key), var->value,
             var->value_len, timeout, (uint32_t)0);

    if (rc != MEMCACHED_SUCCESS) {
        msr_log(msr, 9, "memcache_store_key: Problems storing the key: %s " \
               "(%s)  with value: %s", key, memc_key, var->value);
        return -1;
    }

    msr_log(msr, 9, "memcache_store_key: key: %s, value: %s stored.", memc_key,
            var->value);
#endif
    return 0;
}


static msc_string *memcache_fetch_key(modsec_rec *msr, const char *col_name,
        const char *col_key, char *key)
{
    msc_string *var = NULL;
#ifdef WITH_LIBMEMCACHED
    char namespace_key[MEMCACHED_MAX_KEY];
    char *value = NULL;
    size_t value_length = 0;

    snprintf(namespace_key, MEMCACHED_MAX_KEY-1, "%s,%s,%s", col_name, col_key,
            key);
    value = memcached_get(memcache, namespace_key, strlen(namespace_key),
            &value_length, 0, 0);

    if (value) {
        var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
        var->name = key;
        var->name_len = strlen(key);
        var->value = apr_pstrdup(msr->mp, value);
        var->value_len = value_length;

        free(value);

        msr_log(msr, 9, "memcache_fetch_key: key: %s, value: %s retrieved",
                 key, var->value);
    }
    else {
        msr_log(msr, 9, "memcache_fetch_key: Failed to fetch key: %s (%s)",
                key, namespace_key);
    }
#endif
    return var;
}


static int memcache_apr_table_to_memcache (modsec_rec *msr, const apr_table_t *col,
        const msc_string *var_name, const msc_string *var_key)
{
    int ret = 0;
#ifdef WITH_LIBMEMCACHED
    const apr_array_header_t *arr = NULL;
    apr_table_entry_t *te = NULL;
    msc_string *var = NULL;
    msc_string *index = NULL;
    msc_string *timeout_string = NULL;
    time_t timeout = NULL;
    int i = 0;

    timeout_string = (msc_string *)apr_table_get(col, "TIMEOUT");
    if (timeout_string == NULL) {
        msr_log(msr, 9, "memcache_apr_table_to_memcache: Failed to convert an "\
                "apr table to memcache, it is missing the attribute TIMEOUT. " \
                "Aborting.");
        ret = -1;
        goto missing_timeout;
    }

    //FIXME: OMG. Too many cast.
    timeout = (int) atoi((char *)timeout_string->value);

    index = apr_pcalloc(msr->mp, sizeof(msc_string));

    arr = apr_table_elts(col);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_string *var = (msc_string *)te[i].val;
        
        /* Index will be stored after all other alements, so that we can
           compute its value.
         */
        if (strcmp(te[i].key, MEMCACHE_INDEX_ELEMENT) == 0) 
            continue;

        if (!memcache_is_collection_default_key(te[i].key)) {
            msr_log(msr, 9, "memcache_apr_table_to_memcache: key: \"%s\" is " \
                    "a custom attribute, index will be necessary.", te[i].key);

            if (index->value_len > 0) {
                index->value = apr_psprintf(msr->mp, "%s %s", index->value,
                        te[i].key);
                index->value_len = strlen(index->value);
            }
            else {
                index->value = apr_psprintf(msr->mp, "%s", te[i].key);
                index->value_len = strlen(index->value);
            }
        }

        ret = memcache_store_key(msr, var_name, var_key, te[i].key, var,
                timeout);
        if (ret) {
           msr_log(msr, 9, "memcache_apr_table_to_memcache: Failed to store " \
                   "element into memcache server. Aborting.");
           goto failed;
        }

    }

    ret = memcache_store_key(msr, var_name, var_key, MEMCACHE_INDEX_ELEMENT,
            index, timeout);

    if (ret) {
        msr_log(msr, 9, "memcache_apr_table_to_memcache: Failed to store " \
                "memcache into memcache server. Aborting.");
        goto failed;
    }

    msr_log(msr, 4, "memcache_apr_table_to_memcache: Sucessfully saved " \
            "object, with the following custom elements: %s", index->value);

missing_timeout:
failed:
#endif
    return ret;
}


static apr_table_t *memcache_to_apr_table (modsec_rec *msr,
        const char *col_name, const char *col_key, const int col_key_len)
{
#ifdef WITH_LIBMEMCACHED
    char **keys = msc_collection_default_keys;
    char **custom_keys = NULL;
    char *k = NULL;
    apr_table_t *ret = NULL;
    msc_string *value = NULL;
    msc_string *index;
    int fetched_any = 0;

    msr_log(msr, 9, "memcache_to_apr_table: Retrieving %s form memcache " \
            "server", col_name);

    ret = apr_table_make(msr->mp, 24);
    if (ret == NULL) {
        msr_log(msr, 9, "memcache_to_apr_table: failed to create the apr " \
                "table");
        goto failed_make_table;
    }

   
    while (k = *keys++) {
        value = memcache_fetch_key(msr, col_name, col_key, k);
        if (value) {
            /* msr_log(msr, 9, "memcache_to_apr_table: Successfully retrieved: " \
                    "%s with value: %s", value->name, value->value); */
            apr_table_setn(ret, value->name, (void *)value);
            fetched_any = 1;
        }
    }

    /* If the table does not exist at all. */
    if (fetched_any == 0) {
        goto not_exist;
    }

    index = (msc_string *)apr_table_get(ret, MEMCACHE_INDEX_ELEMENT);
    if (index) {
            msr_log(msr, 9, "memcache_to_apr_table: This collection contains "\
                    "custom elements: %s", index->value);

        apr_tokenize_to_argv(index->value, &custom_keys, msr->mp);
        while (k = *custom_keys++) {
            value = memcache_fetch_key(msr, col_name, col_key, k);
            if (value) {
                /*msr_log(msr, 9, "memcache_to_apr_table: Successfully " \
                        "retrieved custom attribute: %s with value: %s",
                        value->name, value->value);*/

                apr_table_setn(ret, value->name, (void *)value);
            }
        }
    }

    /* Sanity check by looking at TIMEOUT key */
    if (apr_table_get(ret, MEMCACHE_INDEX_ELEMENT) == NULL) {
        msr_log(msr, 9, "memcache_to_apr_table: Something odd happens, this " \
                        "collection is missing basic elements such as: %s. " \
                        "Acting like it does not exist.",
                        MEMCACHE_INDEX_ELEMENT);
        goto not_exist;
    }

failed_make_table:
    return ret;
not_exist:
#endif
    return NULL;
}


static void memcache_remove_expired_variables(modsec_rec *msr,
        apr_table_t *rtable)
{
#ifdef WITH_LIBMEMCACHED
    const apr_array_header_t *arr = NULL;
    apr_table_entry_t *te = NULL;
    int expired = 0;
    int i = 0;

    /* Remove expired variables. */
    do {
        arr = apr_table_elts(rtable);
        te = (apr_table_entry_t *)arr->elts;

        for (i = 0; i < arr->nelts; i++) {
            if (strncmp(te[i].key, "__expire_", 9) == 0) {
                msc_string *var = (msc_string *)te[i].val;
                int expiry_time = atoi(var->value);

                if (expiry_time <= apr_time_sec(msr->request_time)) {
                    char *key_to_expire = te[i].key;

                    /* Done early if the col expired */
                    if (strcmp(key_to_expire, "__expire_KEY") == 0) {
                        expired = 1;
                    }
                    
                    if (msr->txcfg->debuglog_level >= 9) {
                        msr_log(msr, 9, "memcache_remove_expired_variables: " \
                                "Removing key \"%s\" from collection.",
                                key_to_expire + 9);
                        msr_log(msr, 9, "memcache_remove_expired_variables: " \
                                "Removing key \"%s\" from collection.",
                                key_to_expire);
                    }
                    
                    apr_table_unset(rtable, key_to_expire + 9);
                    apr_table_unset(rtable, key_to_expire);
                    
                    if (msr->txcfg->debuglog_level >= 4) {
                        msr_log(msr, 4, "memcache_remove_expired_variables: " \
                                "Removed expired variable \"%s\".",
                                key_to_expire + 9);
                    }
                    
                    break;
                }
            }
        }
    } while((!expired && (i != arr->nelts)));
#endif
}


static void memcache_collection_update_rate(modsec_rec *msr,
        apr_table_t *rtable)
{
#ifdef WITH_LIBMEMCACHED
    msc_string *var;
    int create_time, counter;

    var = (msc_string *)apr_table_get(rtable, "CREATE_TIME");
    if (var == NULL) {
        /* Error. */
    } else {
        create_time = atoi(var->value);
        var = (msc_string *)apr_table_get(rtable, "UPDATE_COUNTER");
        if (var == NULL) {
            /* Error. */
        } else {
            apr_time_t td;
            counter = atoi(var->value);

            /* UPDATE_RATE is removed on store, so add it back here */
            var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
            var->name = "UPDATE_RATE";
            var->name_len = strlen(var->name);
            apr_table_setn(rtable, var->name, (void *)var);

            /* NOTE: No rate if there has been no time elapsed */
            td = (apr_time_sec(apr_time_now()) - create_time);
            if (td == 0) {
                var->value = apr_psprintf(msr->mp, "%d", 0);
            }
            else {
                var->value = apr_psprintf(msr->mp, "%" APR_TIME_T_FMT,
                    (apr_time_t)((60 * counter)/td));
            }
            var->value_len = strlen(var->value);
        }
    }
#endif
}


apr_table_t *msc_memcache_collection_retrieve(modsec_rec *msr,
        const char *col_name, const char *col_key, const int col_key_len)
{
    apr_table_t *rtable = NULL;
#ifdef WITH_LIBMEMCACHED
    int i = 0;
    apr_time_t time_before = apr_time_now();

    rtable = memcache_to_apr_table(msr, col_name, col_key, col_key_len);

    if (rtable == NULL) {
        msr_log(msr, 4, "msc_memcache_collection_retrieve: Collection: %s " \
                "was not found on memcache server or there was a problem " \
                "fetching it.", col_name);
        goto not_know;
    }

    memcache_remove_expired_variables(msr, rtable);
    memcache_collection_update_rate(msr, rtable);

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "msc_memcache_collection_retrieve: Retrieved " \
                "collection (name \"%s\", key \"%s\").",
                log_escape(msr->mp, col_name),
                log_escape_ex(msr->mp, col_key, col_key_len));
    }

not_know:
    msr->time_storage_read += apr_time_now() - time_before;
#endif
    return rtable;
}


int msc_memcache_collection_store(modsec_rec *msr, apr_table_t *col) {
#ifdef WITH_LIBMEMCACHED
    msc_string *var_name = NULL, *var_key = NULL;
    apr_status_t rc;
    const apr_array_header_t *arr;
    apr_table_entry_t *te;
    int i;
    const apr_table_t *stored_col = NULL;
    const apr_table_t *orig_col = NULL;
    unsigned int blob_size, blob_offset;

    var_name = (msc_string *)apr_table_get(col, "__name");
    var_key = (msc_string *)apr_table_get(col, "__key");

    if (var_key == NULL || var_name == NULL) {
        msr_log(msr, 4, "msc_memcache_collection_store: Not able to store " \
                "collection, missing basic elements.");

        goto failed;
    }

    /* Delete IS_NEW on store. */
    apr_table_unset(col, "IS_NEW");

    /* Delete UPDATE_RATE on store to save space as it is calculated */
    apr_table_unset(col, "UPDATE_RATE");

    /* Update the timeout value. */
    {
        msc_string *var = (msc_string *)apr_table_get(col, "TIMEOUT");
        if (var != NULL) {
            int timeout = atoi(var->value);
            var = (msc_string *)apr_table_get(col, "__expire_KEY");
            if (var != NULL) {
                var->value = apr_psprintf(msr->mp, "%" APR_TIME_T_FMT, \
                        (apr_time_t)(apr_time_sec(apr_time_now()) + timeout));
                var->value_len = strlen(var->value);
            }
        }
    }

    /* LAST_UPDATE_TIME */
    {
        msc_string *var = (msc_string *)apr_table_get(col, "LAST_UPDATE_TIME");
        if (var == NULL) {
            var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
            var->name = "LAST_UPDATE_TIME";
            var->name_len = strlen(var->name);
            apr_table_setn(col, var->name, (void *)var);
        }
        var->value = apr_psprintf(msr->mp, "%" APR_TIME_T_FMT, \
                (apr_time_t)(apr_time_sec(apr_time_now())));
        var->value_len = strlen(var->value);
    }

    /* UPDATE_COUNTER */
    {
        msc_string *var = (msc_string *)apr_table_get(col, "UPDATE_COUNTER");
        int counter = 0;
        if (var == NULL) {
            var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
            var->name = "UPDATE_COUNTER";
            var->name_len = strlen(var->name);
            apr_table_setn(col, var->name, (void *)var);
        } else {
            counter = atoi(var->value);
        }
        var->value = apr_psprintf(msr->mp, "%d", counter + 1);
        var->value_len = strlen(var->value);
    }

    orig_col = (const apr_table_t *)apr_table_get(msr->collections_original, \
            var_name->value);

    if (orig_col != NULL) {
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "msc_memcache_collection_store: Re-retrieving " \
                    "collection prior to store: %s", apr_psprintf(msr->mp,
                "%.*s", var_name->value_len, var_name->value));
        }

        stored_col = msc_memcache_collection_retrieve(msr, var_name->value,
                var_key->value, var_key->value_len);
    }

    memcache_apr_table_to_memcache(msr, col, var_name, var_key);

    msr_log(msr, 4, "msc_memcache_collection_store: Successfully stored " \
                    "collection.");
    return 0;
failed:
#endif
    return -1;
}

