/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2011 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "persist_dbm.h"
#include "apr_sdbm.h"

/**
 *
 */
static apr_table_t *collection_unpack(modsec_rec *msr, const unsigned char *blob, unsigned int blob_size,
    int log_vars)
{
    apr_table_t *col = NULL;
    unsigned int blob_offset;

    col = apr_table_make(msr->mp, 32);
    if (col == NULL) return NULL;

    /* ENH verify the first 3 bytes (header) */

    blob_offset = 3;
    while (blob_offset + 1 < blob_size) {
        msc_string *var = apr_pcalloc(msr->mp, sizeof(msc_string));

        var->name_len = (blob[blob_offset] << 8) + blob[blob_offset + 1];
        if (var->name_len == 0) {
            /* Is the length a name length, or just the end of the blob? */
            if (blob_offset < blob_size - 2) {
                /* This should never happen as the name length
                 * includes the terminating NUL and should be 1 for ""
                 */
                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "collection_unpack: BLOB[%d]: %s", blob_offset, log_escape_hex(msr->mp, blob + blob_offset, blob_size - blob_offset));
                }
                msr_log(msr, 4, "collection_unpack: Possibly corrupted database: var name length = 0 at blob offset %u-%u.", blob_offset, blob_offset + 1);
            }
            break;
        }
        else if (var->name_len > 65536) {
            /* This should never happen as the length is restricted on store
             * to 65536.
             */
            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "collection_unpack: BLOB[%d]: %s", blob_offset, log_escape_hex(msr->mp, blob + blob_offset, blob_size - blob_offset));
            }
            msr_log(msr, 4, "collection_unpack: Possibly corrupted database: var name length > 65536 (0x%04x) at blob offset %u-%u.", var->name_len, blob_offset, blob_offset + 1);
            break;
        }

        blob_offset += 2;
        if (blob_offset + var->name_len > blob_size) return NULL;
        var->name = apr_pstrmemdup(msr->mp, (const char *)blob + blob_offset, var->name_len - 1);
        blob_offset += var->name_len;
        var->name_len--;

        var->value_len = (blob[blob_offset] << 8) + blob[blob_offset + 1];
        blob_offset += 2;

        if (blob_offset + var->value_len > blob_size) return NULL;
        var->value = apr_pstrmemdup(msr->mp, (const char *)blob + blob_offset, var->value_len - 1);
        blob_offset += var->value_len;
        var->value_len--;

        if (log_vars && (msr->txcfg->debuglog_level >= 9)) {
            msr_log(msr, 9, "collection_unpack: Read variable: name \"%s\", value \"%s\".",
                log_escape_ex(msr->mp, var->name, var->name_len),
                log_escape_ex(msr->mp, var->value, var->value_len));
        }

        apr_table_addn(col, var->name, (void *)var);
    }

    return col;
}

/**
 *
 */
static apr_table_t *collection_retrieve_ex(apr_sdbm_t *existing_dbm, modsec_rec *msr, const char *col_name,
    const char *col_key, int col_key_len)
{
    char *dbm_filename = NULL;
    apr_status_t rc;
    apr_sdbm_datum_t key;
    apr_sdbm_datum_t *value = NULL;
    apr_sdbm_t *dbm = NULL;
    apr_table_t *col = NULL;
    const apr_array_header_t *arr;
    apr_table_entry_t *te;
    int expired = 0;
    int i;

    if (msr->txcfg->data_dir == NULL) {
        msr_log(msr, 1, "collection_retrieve_ex: Unable to retrieve collection (name \"%s\", key \"%s\"). Use "
            "SecDataDir to define data directory first.", log_escape(msr->mp, col_name),
            log_escape_ex(msr->mp, col_key, col_key_len));
        goto cleanup;
    }

    dbm_filename = apr_pstrcat(msr->mp, msr->txcfg->data_dir, "/", col_name, NULL);

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "collection_retrieve_ex: collection_retrieve_ex: Retrieving collection (name \"%s\", filename \"%s\")",log_escape(msr->mp, col_name),
                log_escape(msr->mp, dbm_filename));
    }

    key.dptr = (char *)col_key;
    key.dsize = col_key_len + 1;

    if (existing_dbm == NULL) {
        rc = apr_sdbm_open(&dbm, dbm_filename, APR_READ | APR_SHARELOCK,
            CREATEMODE, msr->mp);
        if (rc != APR_SUCCESS) {
            dbm = NULL;
            goto cleanup;
        }
    }
    else {
        dbm = existing_dbm;
    }

    value = (apr_sdbm_datum_t *)apr_pcalloc(msr->mp, sizeof(apr_sdbm_datum_t));
    rc = apr_sdbm_fetch(dbm, value, key);
    if (rc != APR_SUCCESS) {
        msr_log(msr, 1, "collection_retrieve_ex: Failed to read from DBM file \"%s\": %s", log_escape(msr->mp,
            dbm_filename), get_apr_error(msr->mp, rc));
        goto cleanup;
    }

    if (value->dptr == NULL) { /* Key not found in DBM file. */
        goto cleanup;
    }

    /* ENH Need expiration (and perhaps other metadata) accessible in blob
     * form to determine if converting to a table is needed.  This will
     * save some cycles.
     */

    /* Transform raw data into a table. */
    col = collection_unpack(msr, (const unsigned char *)value->dptr, value->dsize, 1);
    if (col == NULL) {
        goto cleanup;
    }

    /* Close after "value" used from fetch or memory may be overwritten. */
    if (existing_dbm == NULL) {
        apr_sdbm_close(dbm);
        dbm = NULL;
    }

    /* Remove expired variables. */
    do {
        arr = apr_table_elts(col);
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
                        msr_log(msr, 9, "collection_retrieve_ex: Removing key \"%s\" from collection.", key_to_expire + 9);
                        msr_log(msr, 9, "collection_retrieve_ex: Removing key \"%s\" from collection.", key_to_expire);
                    }
                    
                    apr_table_unset(col, key_to_expire + 9);
                    apr_table_unset(col, key_to_expire);
                    
                    if (msr->txcfg->debuglog_level >= 4) {
                        msr_log(msr, 4, "collection_retrieve_ex: Removed expired variable \"%s\".", key_to_expire + 9);
                    }
                    
                    break;
                }
            }
        }
    } while(!expired && (i != arr->nelts));

    /* Delete the collection if the variable "KEY" does not exist.
     *
     * ENH It would probably be more efficient to hold the DBM
     * open until determined if it needs deleted than to open a second
     * time.
     */
    if (apr_table_get(col, "KEY") == NULL) {
        if (existing_dbm == NULL) {
            rc = apr_sdbm_open(&dbm, dbm_filename, APR_CREATE | APR_WRITE | APR_SHARELOCK,
                CREATEMODE, msr->mp);
            if (rc != APR_SUCCESS) {
                msr_log(msr, 1, "collection_retrieve_ex: Failed to access DBM file \"%s\": %s",
                    log_escape(msr->mp, dbm_filename), get_apr_error(msr->mp, rc));
                dbm = NULL;
                goto cleanup;
            }
        }
        else {
            dbm = existing_dbm;
        }

        rc = apr_sdbm_delete(dbm, key);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "collection_retrieve_ex: Failed deleting collection (name \"%s\", "
                "key \"%s\"): %s", log_escape(msr->mp, col_name),
                log_escape_ex(msr->mp, col_key, col_key_len), get_apr_error(msr->mp, rc));
            goto cleanup;
        }


        if (existing_dbm == NULL) {
            apr_sdbm_close(dbm);
            dbm = NULL;
        }

        if (expired && (msr->txcfg->debuglog_level >= 9)) {
            msr_log(msr, 9, "collection_retrieve_ex: Collection expired (name \"%s\", key \"%s\").", col_name,
                log_escape_ex(msr->mp, col_key, col_key_len));
        }
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "collection_retrieve_ex: Deleted collection (name \"%s\", key \"%s\").",
                log_escape(msr->mp, col_name), log_escape_ex(msr->mp, col_key, col_key_len));
        }
        goto cleanup;
    }

    /* Update UPDATE_RATE */
    {
        msc_string *var;
        int create_time, counter;

        var = (msc_string *)apr_table_get(col, "CREATE_TIME");
        if (var == NULL) {
            /* Error. */
        } else {
            create_time = atoi(var->value);
            var = (msc_string *)apr_table_get(col, "UPDATE_COUNTER");
            if (var == NULL) {
                /* Error. */
            } else {
                apr_time_t td;
                counter = atoi(var->value);

                /* UPDATE_RATE is removed on store, so add it back here */
                var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
                var->name = "UPDATE_RATE";
                var->name_len = strlen(var->name);
                apr_table_setn(col, var->name, (void *)var);

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
    }

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "collection_retrieve_ex: Retrieved collection (name \"%s\", key \"%s\").",
            log_escape(msr->mp, col_name), log_escape_ex(msr->mp, col_key, col_key_len));
    }

    if ((existing_dbm == NULL) && dbm) {
        /* Should not ever get here */
        msr_log(msr, 1, "collection_retrieve_ex: Internal Error: Collection remained open (name \"%s\", key \"%s\").",
            log_escape(msr->mp, col_name), log_escape_ex(msr->mp, col_key, col_key_len));

        apr_sdbm_close(dbm);
    }

    return col;

cleanup:

    if ((existing_dbm == NULL) && dbm) {
        apr_sdbm_close(dbm);
    }

    return NULL;
}

/**
 *
 */
apr_table_t *collection_retrieve(modsec_rec *msr, const char *col_name,
    const char *col_key, int col_key_len)
{
    apr_time_t time_before = apr_time_now();
    apr_table_t *rtable = NULL;
    
    rtable = collection_retrieve_ex(NULL, msr, col_name, col_key, col_key_len);
    
    msr->time_storage_read += apr_time_now() - time_before;
    
    return rtable;
}

/**
 *
 */
int collection_store(modsec_rec *msr, apr_table_t *col) {
    char *dbm_filename = NULL;
    msc_string *var_name = NULL, *var_key = NULL;
    unsigned char *blob = NULL;
    unsigned int blob_size, blob_offset;
    apr_status_t rc;
    apr_sdbm_datum_t key;
    apr_sdbm_datum_t value;
    apr_sdbm_t *dbm = NULL;
    const apr_array_header_t *arr;
    apr_table_entry_t *te;
    int i;
    const apr_table_t *stored_col = NULL;
    const apr_table_t *orig_col = NULL;

    var_name = (msc_string *)apr_table_get(col, "__name");
    if (var_name == NULL) {
        goto error;
    }

    var_key = (msc_string *)apr_table_get(col, "__key");
    if (var_key == NULL) {
        goto error;
    }

    if (msr->txcfg->data_dir == NULL) {
        msr_log(msr, 1, "collection_store: Unable to store collection (name \"%s\", key \"%s\"). Use "
            "SecDataDir to define data directory first.", log_escape_ex(msr->mp, var_name->value, var_name->value_len),
            log_escape_ex(msr->mp, var_key->value, var_key->value_len));
        goto error;
    }

    // ENH: lowercase the var name in the filename
    dbm_filename = apr_pstrcat(msr->mp, msr->txcfg->data_dir, "/", var_name->value, NULL);

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "collection_store: Retrieving collection (name \"%s\", filename \"%s\")",log_escape(msr->mp, var_name->value),
                log_escape(msr->mp, dbm_filename));
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
                var->value = apr_psprintf(msr->mp, "%" APR_TIME_T_FMT, (apr_time_t)(apr_time_sec(apr_time_now()) + timeout));
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
        var->value = apr_psprintf(msr->mp, "%" APR_TIME_T_FMT, (apr_time_t)(apr_time_sec(apr_time_now())));
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

    /* ENH Make the expiration timestamp accessible in blob form so that
     * it is easier/faster to determine expiration without having to
     * convert back to table form
     */

    rc = apr_sdbm_open(&dbm, dbm_filename, APR_CREATE | APR_WRITE | APR_SHARELOCK,
        CREATEMODE, msr->mp);
    if (rc != APR_SUCCESS) {
        msr_log(msr, 1, "collection_store: Failed to access DBM file \"%s\": %s", log_escape(msr->mp, dbm_filename),
            get_apr_error(msr->mp, rc));
        dbm = NULL;
        goto error;
    }

    /* Need to lock to pull in the stored data again and apply deltas. */
    rc = apr_sdbm_lock(dbm, APR_FLOCK_EXCLUSIVE);
    if (rc != APR_SUCCESS) {
        msr_log(msr, 1, "collection_store: Failed to exclusivly lock DBM file \"%s\": %s", log_escape(msr->mp, dbm_filename),
            get_apr_error(msr->mp, rc));
        goto error;
    }

    /* If there is an original value, then create a delta and
     * apply the delta to the current value */
    orig_col = (const apr_table_t *)apr_table_get(msr->collections_original, var_name->value);
    if (orig_col != NULL) {
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "collection_store: Re-retrieving collection prior to store: %s",
                apr_psprintf(msr->mp, "%.*s", var_name->value_len, var_name->value));
        }

        stored_col = (const apr_table_t *)collection_retrieve_ex(dbm, msr, var_name->value, var_key->value, var_key->value_len);
    }

    /* Merge deltas and calculate the size first. */
    blob_size = 3 + 2;
    arr = apr_table_elts(col);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_string *var = (msc_string *)te[i].val;
        int len;

        /* If there is an original value, then apply the delta
         * to the latest stored value */
        if (stored_col != NULL) {
            const msc_string *orig_var = (const msc_string *)apr_table_get(orig_col, var->name);
            if (orig_var != NULL) {
                const msc_string *stored_var = (const msc_string *)apr_table_get(stored_col, var->name);
                if (stored_var != NULL) {
                    int origval = atoi(orig_var->value);
                    int ourval = atoi(var->value);
                    int storedval = atoi(stored_var->value);
                    int delta = ourval - origval;
                    int newval = storedval + delta;

                    if (newval < 0) newval = 0; /* Counters never go below zero. */

                    var->value = apr_psprintf(msr->mp, "%d", newval);
                    var->value_len = strlen(var->value);
                    
                    if (msr->txcfg->debuglog_level >= 9) {
                        msr_log(msr, 9, "collection_store: Delta applied for %s.%s %d->%d (%d): %d + (%d) = %d [%s,%d]",
                        log_escape_ex(msr->mp, var_name->value, var_name->value_len),
                        log_escape_ex(msr->mp, var->name, var->name_len),
                        origval, ourval, delta, storedval, delta, newval, var->value, var->value_len);
                    }
                }
            }
        }

        len = var->name_len + 1;
        if (len >= 65536) len = 65536;
        blob_size += len + 2;

        len = var->value_len + 1;
        if (len >= 65536) len = 65536;
        blob_size += len + 2;
    }

    /* Now generate the binary object. */
    blob = apr_pcalloc(msr->mp, blob_size);
    if (blob == NULL) {
        goto error;
    }

    blob[0] = 0x49;
    blob[1] = 0x52;
    blob[2] = 0x01;

    blob_offset = 3;
    arr = apr_table_elts(col);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_string *var = (msc_string *)te[i].val;
        int len;

        len = var->name_len + 1;
        if (len >= 65536) len = 65536;

        blob[blob_offset + 0] = (len & 0xff00) >> 8;
        blob[blob_offset + 1] = len & 0x00ff;
        memcpy(blob + blob_offset + 2, var->name, len - 1);
        blob[blob_offset + 2 + len - 1] = '\0';
        blob_offset += 2 + len;

        len = var->value_len + 1;
        if (len >= 65536) len = 65536;

        blob[blob_offset + 0] = (len & 0xff00) >> 8;
        blob[blob_offset + 1] = len & 0x00ff;
        memcpy(blob + blob_offset + 2, var->value, len - 1);
        blob[blob_offset + 2 + len - 1] = '\0';
        blob_offset += 2 + len;

        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "collection_store: Wrote variable: name \"%s\", value \"%s\".",
                log_escape_ex(msr->mp, var->name, var->name_len),
                log_escape_ex(msr->mp, var->value, var->value_len));
        }
    }

    blob[blob_offset] = 0;
    blob[blob_offset + 1] = 0;

    /* And, finally, store it. */
    key.dptr = var_key->value;
    key.dsize = var_key->value_len + 1;

    value.dptr = (char *)blob;
    value.dsize = blob_size;

    rc = apr_sdbm_store(dbm, key, value, APR_SDBM_REPLACE);
    if (rc != APR_SUCCESS) {
        msr_log(msr, 1, "collection_store: Failed to write to DBM file \"%s\": %s", dbm_filename,
            get_apr_error(msr->mp, rc));
        goto error;
    }

    apr_sdbm_close(dbm);

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "collection_store: Persisted collection (name \"%s\", key \"%s\").",
            log_escape_ex(msr->mp, var_name->value, var_name->value_len),
            log_escape_ex(msr->mp, var_key->value, var_key->value_len));
    }

    return 0;

error:

    if (dbm) {
        apr_sdbm_close(dbm);
    }

    return -1;
}

/**
 *
 */
int collections_remove_stale(modsec_rec *msr, const char *col_name) {
    char *dbm_filename = NULL;
    apr_sdbm_datum_t key, value;
    apr_sdbm_t *dbm = NULL;
    apr_status_t rc;
    apr_array_header_t *keys_arr;
    char **keys;
    apr_time_t now = apr_time_sec(msr->request_time);
    int i;

    if (msr->txcfg->data_dir == NULL) {
        /* The user has been warned about this problem enough times already by now.
         * msr_log(msr, 1, "Unable to access collection file (name \"%s\"). Use SecDataDir to "
         *     "define data directory first.", log_escape(msr->mp, col_name));
         */
        goto error;
    }

    if(strstr(col_name,"USER") || strstr(col_name,"SESSION") || strstr(col_name, "RESOURCE"))
        dbm_filename = apr_pstrcat(msr->mp, msr->txcfg->data_dir, "/", msr->txcfg->webappid, "_", col_name, NULL);
    else
        dbm_filename = apr_pstrcat(msr->mp, msr->txcfg->data_dir, "/", col_name, NULL);

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "collections_remove_stale: Retrieving collection (name \"%s\", filename \"%s\")",log_escape(msr->mp, col_name),
                log_escape(msr->mp, dbm_filename));
    }

    rc = apr_sdbm_open(&dbm, dbm_filename, APR_CREATE | APR_WRITE | APR_SHARELOCK,
            CREATEMODE, msr->mp);
    if (rc != APR_SUCCESS) {
        msr_log(msr, 1, "collections_remove_stale: Failed to access DBM file \"%s\": %s", log_escape(msr->mp, dbm_filename),
                get_apr_error(msr->mp, rc));
        dbm = NULL;
        goto error;
    }

    /* First get a list of all keys. */
    keys_arr = apr_array_make(msr->mp, 256, sizeof(char *));
    rc = apr_sdbm_lock(dbm, APR_FLOCK_SHARED);
    if (rc != APR_SUCCESS) {
        msr_log(msr, 1, "collections_remove_stale: Failed to lock DBM file \"%s\": %s", log_escape(msr->mp, dbm_filename),
            get_apr_error(msr->mp, rc));
        goto error;
    }

    /* No one can write to the file while doing this so
     * do it as fast as possible.
     */
    rc = apr_sdbm_firstkey(dbm, &key);
    while(rc == APR_SUCCESS) {
        char *s = apr_pstrmemdup(msr->mp, key.dptr, key.dsize - 1);
        *(char **)apr_array_push(keys_arr) = s;
        rc = apr_sdbm_nextkey(dbm, &key);
    }
    apr_sdbm_unlock(dbm);

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "collections_remove_stale: Found %d record(s) in file \"%s\".", keys_arr->nelts,
            log_escape(msr->mp, dbm_filename));
    }

    /* Now retrieve the entires one by one. */
    keys = (char **)keys_arr->elts;
    for (i = 0; i < keys_arr->nelts; i++) {
        key.dptr = keys[i];
        key.dsize = strlen(key.dptr) + 1;

        rc = apr_sdbm_fetch(dbm, &value, key);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "collections_remove_stale: Failed reading DBM file \"%s\": %s",
                log_escape(msr->mp, dbm_filename), get_apr_error(msr->mp, rc));
            goto error;
        }

        if (value.dptr != NULL) {
            apr_table_t *col = NULL;
            msc_string *var = NULL;

            col = collection_unpack(msr, (const unsigned char *)value.dptr, value.dsize, 0);
            if (col == NULL) {
                goto error;
            }

            var = (msc_string *)apr_table_get(col, "__expire_KEY");
            if (var == NULL) {
                msr_log(msr, 1, "collections_remove_stale: Collection cleanup discovered entry with no "
                    "__expire_KEY (name \"%s\", key \"%s\").",
                    log_escape(msr->mp, col_name), log_escape_ex(msr->mp, key.dptr, key.dsize - 1));
            } else {
                unsigned int expiry_time = atoi(var->value);

                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "collections_remove_stale: Record (name \"%s\", key \"%s\") set to expire in %" APR_TIME_T_FMT " seconds.",
                        log_escape(msr->mp, col_name), log_escape_ex(msr->mp, key.dptr, key.dsize - 1),
                        expiry_time - now);
                }

                if (expiry_time <= now) {
                    rc = apr_sdbm_delete(dbm, key);
                    if (rc != APR_SUCCESS) {
                        msr_log(msr, 1, "collections_remove_stale: Failed deleting collection (name \"%s\", "
                            "key \"%s\"): %s", log_escape(msr->mp, col_name),
                            log_escape_ex(msr->mp, key.dptr, key.dsize - 1), get_apr_error(msr->mp, rc));
                        goto error;
                    }
                    
                    if (msr->txcfg->debuglog_level >= 4) {
                        msr_log(msr, 4, "collections_remove_stale: Removed stale collection (name \"%s\", "
                            "key \"%s\").", log_escape(msr->mp, col_name),
                            log_escape_ex(msr->mp, key.dptr, key.dsize - 1));
                    }
                }
            }
        } else {
            /* Ignore entry not found - it may have been removed in the meantime. */
        }
    }

    apr_sdbm_close(dbm);

    return 1;

error:

    if (dbm) {
        apr_sdbm_close(dbm);
    }

    return -1;
}
