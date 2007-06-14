/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2007 Breach Security, Inc. (http://www.breach.com/)
 *
 * You should have received a copy of the licence along with this
 * program (stored in the file "LICENSE"). If the file is missing,
 * or if you have any other questions related to the licence, please
 * write to Breach Security, Inc. at support@breach.com.
 *
 */
#include "persist_dbm.h"
#include "apr_sdbm.h"

/**
 *
 */
static apr_table_t *collection_unpack(modsec_rec *msr, char *blob, unsigned int blob_size,
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
        if (var->name_len == 0) break;

        blob_offset += 2;
        if (blob_offset + var->name_len > blob_size) return NULL;
        var->name = apr_pstrmemdup(msr->mp, blob + blob_offset, var->name_len - 1);
        blob_offset += var->name_len;
        var->name_len--;

        var->value_len = (blob[blob_offset] << 8) + blob[blob_offset + 1];
        blob_offset += 2;

        if (blob_offset + var->value_len > blob_size) return NULL;
        var->value = apr_pstrmemdup(msr->mp, blob + blob_offset, var->value_len - 1);
        blob_offset += var->value_len;
        var->value_len--;

        if (log_vars) {
            msr_log(msr, 9, "Read variable: name \"%s\", value \"%s\".",
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
apr_table_t *collection_retrieve(modsec_rec *msr, const char *col_name,
    const char *col_key, int col_key_len)
{
    char *dbm_filename = NULL;
    apr_status_t rc;
    apr_sdbm_datum_t key;
    apr_sdbm_datum_t *value = NULL;
    apr_sdbm_t *dbm;
    apr_table_t *col = NULL;
    const apr_array_header_t *arr;
    apr_table_entry_t *te;
    int i;

    if (msr->txcfg->data_dir == NULL) {
        msr_log(msr, 1, "Unable to retrieve collection (name \"%s\", key \"%s\"). Use "
            "SecDataDir to define data directory first.", log_escape(msr->mp, col_name),
            log_escape(msr->mp, col_key));
        return NULL;
    }

    dbm_filename = apr_pstrcat(msr->mp, msr->txcfg->data_dir, "/", col_name, NULL);

    rc = apr_sdbm_open(&dbm, dbm_filename, APR_READ | APR_SHARELOCK,
        CREATEMODE, msr->mp);
    if (rc != APR_SUCCESS) {
        return NULL;
    }

    key.dptr = (char *)col_key;
    key.dsize = col_key_len + 1;

    value = (apr_sdbm_datum_t *)apr_pcalloc(msr->mp, sizeof(apr_sdbm_datum_t));
    rc = apr_sdbm_fetch(dbm, value, key);
    if (rc != APR_SUCCESS) {
        apr_sdbm_close(dbm);
        msr_log(msr, 1, "Failed to read from DBM file \"%s\": %s", log_escape(msr->mp,
            dbm_filename), get_apr_error(msr->mp, rc));
        return NULL;
    }
    
    if (value->dptr == NULL) { /* Key not found in DBM file. */
        apr_sdbm_close(dbm);
        return NULL;
    }

    /* Transform raw data into a table. */
    col = collection_unpack(msr, value->dptr, value->dsize, 1);
    if (col == NULL) return NULL;

    /* Remove expired variables. */
    do {
        arr = apr_table_elts(col);
        te = (apr_table_entry_t *)arr->elts;
        for (i = 0; i < arr->nelts; i++) {
            if (strncmp(te[i].key, "__expire_", 9) == 0) {
                msc_string *var = (msc_string *)te[i].val;
                int expiry_time = atoi(var->value);

                /* Do not remove the record itself. */
                if (strcmp(te[i].key, "__expire_KEY") == 0) continue;                

                if (expiry_time <= apr_time_sec(msr->request_time)) {
                    char *key_to_expire = apr_pstrdup(msr->mp, te[i].key);
                    msr_log(msr, 9, "Removing key \"%s\" from collection.", key_to_expire + 9);
                    apr_table_unset(col, key_to_expire + 9);
                    msr_log(msr, 9, "Removing key \"%s\" from collection.", key_to_expire);
                    apr_table_unset(col, key_to_expire);
                    msr_log(msr, 4, "Removed expired variable \"%s\".", key_to_expire + 9);
                    break;
                }
            }
        }
    } while(i != arr->nelts);
    

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
                int td;
                counter = atoi(var->value);
                var = (msc_string *)apr_table_get(col, "UPDATE_RATE");
                if (var == NULL) {
                    var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
                    var->name = "UPDATE_RATE";
                    var->name_len = strlen(var->name);
                    apr_table_setn(col, var->name, (void *)var);
                }

                /* NOTE: No rate if there has been no time elapsed */
                td = (apr_time_sec(apr_time_now()) - create_time);
                if (td == 0) {
                    var->value = apr_psprintf(msr->mp, "%i", 0);
                }
                else {
                    var->value = apr_psprintf(msr->mp, "%i",
                        (int)((60 * counter)/td));
                }
                var->value_len = strlen(var->value);
            }
        }
    }

    apr_sdbm_close(dbm);

    msr_log(msr, 4, "Retrieved collection (name \"%s\", key \"%s\").",
            log_escape(msr->mp, col_name), log_escape(msr->mp, col_key));

    return col;
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
    apr_sdbm_t *dbm;
    const apr_array_header_t *arr;
    apr_table_entry_t *te;
    int i;

    var_name = (msc_string *)apr_table_get(col, "__name");
    if (var_name == NULL) {
        return -1;
    }

    var_key = (msc_string *)apr_table_get(col, "__key");
    if (var_key == NULL) {
        return -1;
    }

    if (msr->txcfg->data_dir == NULL) {
        msr_log(msr, 1, "Unable to store collection (name \"%s\", key \"%s\"). Use "
            "SecDataDir to define data directory first.",
            log_escape(msr->mp, var_name->value), log_escape(msr->mp, var_key->value));
        return -1;
    }

    dbm_filename = apr_pstrcat(msr->mp, msr->txcfg->data_dir, "/", var_name->value, NULL);    

    /* Remove expired variables. */
    do {
        arr = apr_table_elts(col);
        te = (apr_table_entry_t *)arr->elts;
        for (i = 0; i < arr->nelts; i++) {
            if (strncmp(te[i].key, "__expire_", 9) == 0) {
                msc_string *var = (msc_string *)te[i].val;
                int expiry_time = atoi(var->value);

                /* Do not remove the record itself. */
                if (strcmp(te[i].key, "__expire_KEY") == 0) continue;                

                if (expiry_time <= apr_time_sec(msr->request_time)) {
                    char *key_to_expire = apr_pstrdup(msr->mp, te[i].key);
                    msr_log(msr, 9, "Removing key \"%s\" from collection.", key_to_expire + 9);
                    apr_table_unset(col, key_to_expire + 9);
                    msr_log(msr, 9, "Removing key \"%s\" from collection.", key_to_expire);
                    apr_table_unset(col, key_to_expire);
                    msr_log(msr, 4, "Removed expired variable \"%s\".", key_to_expire + 9);
                    break;
                }
            }
        }
    } while(i != arr->nelts);

    /* Delete the collection if the variable "KEY" does not exist. */
    if (apr_table_get(col, "KEY") == NULL) {

        rc = apr_sdbm_open(&dbm, dbm_filename, APR_CREATE | APR_WRITE | APR_SHARELOCK,
            CREATEMODE, msr->mp);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "Failed to access DBM file \"%s\": %s",
                log_escape(msr->mp, dbm_filename), get_apr_error(msr->mp, rc));
            return -1;
        }

        key.dptr = var_key->value;
        key.dsize = var_key->value_len + 1;

        rc = apr_sdbm_delete(dbm, key);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "Failed deleting collection (name \"%s\", "
                "key \"%s\"): %s", log_escape(msr->mp, var_name->value),
                log_escape(msr->mp, var_key->value), get_apr_error(msr->mp, rc));
            apr_sdbm_close(dbm);
            return -1;
        }

        msr_log(msr, 4, "Deleted collection (name \"%s\", key \"%s\").", 
            log_escape(msr->mp, var_name->value), log_escape(msr->mp, var_key->value));
        apr_sdbm_close(dbm);

        return 1;
    }

    /* Update the timeout value. */
    {
        msc_string *var = (msc_string *)apr_table_get(col, "TIMEOUT");
        if (var != NULL) {
            int timeout = atoi(var->value);
            var = (msc_string *)apr_table_get(col, "__expire_KEY");
            if (var != NULL) {
                var->value = apr_psprintf(msr->mp, "%i", (int)(apr_time_sec(apr_time_now()) + timeout));
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
        var->value = apr_psprintf(msr->mp, "%i", (int)(apr_time_sec(apr_time_now())));
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
        var->value = apr_psprintf(msr->mp, "%i", counter + 1);
        var->value_len = strlen(var->value);
    }

    /* Calculate the size first. */
    blob_size = 3 + 2;
    arr = apr_table_elts(col);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_string *var = (msc_string *)te[i].val;
        int len;

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
        return -1;
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

        msr_log(msr, 9, "Wrote variable: name \"%s\", value \"%s\".",
            log_escape_ex(msr->mp, var->name, var->name_len),
            log_escape_ex(msr->mp, var->value, var->value_len));
    }

    blob[blob_offset] = 0;
    blob[blob_offset + 1] = 0;

    /* And, finally, store it. */
    dbm_filename = apr_pstrcat(msr->mp, msr->txcfg->data_dir, "/", var_name->value, NULL);    

    rc = apr_sdbm_open(&dbm, dbm_filename, APR_CREATE | APR_WRITE | APR_SHARELOCK,
        CREATEMODE, msr->mp);
    if (rc != APR_SUCCESS) {
        msr_log(msr, 1, "Failed to access DBM file \"%s\": %s", log_escape(msr->mp, dbm_filename),
            get_apr_error(msr->mp, rc));
        return -1;
    }

    key.dptr = var_key->value;
    key.dsize = var_key->value_len + 1;

    value.dptr = (char *)blob;
    value.dsize = blob_size;

    rc = apr_sdbm_store(dbm, key, value, APR_SDBM_REPLACE);
    if (rc != APR_SUCCESS) {
        msr_log(msr, 1, "Failed to write to DBM file \"%s\": %s", dbm_filename,
            get_apr_error(msr->mp, rc));
        apr_sdbm_close(dbm);
        return -1;
    }    

    msr_log(msr, 4, "Persisted collection (name \"%s\", key \"%s\").",
        log_escape(msr->mp, var_name->value), log_escape(msr->mp, var_key->value));

    apr_sdbm_close(dbm);

    return 0;    
}

/**
 *
 */
int collections_remove_stale(modsec_rec *msr, const char *col_name) {
    char *dbm_filename = NULL;
    apr_sdbm_datum_t key, value;
    apr_sdbm_t *dbm;
    apr_status_t rc;
    apr_array_header_t *keys_arr;
    char **keys;
    int i;
    unsigned int now = (unsigned int)apr_time_sec(msr->request_time);

    if (msr->txcfg->data_dir == NULL) {
        /* The user has been warned about this problem enough times already by now. 
         * msr_log(msr, 1, "Unable to access collection file (name \"%s\"). Use SecDataDir to "
         *     "define data directory first.", log_escape(msr->mp, col_name));
         */
        return -1;
    }

    dbm_filename = apr_pstrcat(msr->mp, msr->txcfg->data_dir, "/", col_name, NULL);

    rc = apr_sdbm_open(&dbm, dbm_filename, APR_CREATE | APR_WRITE | APR_SHARELOCK,
        CREATEMODE, msr->mp);
    if (rc != APR_SUCCESS) {
        msr_log(msr, 1, "Failed to access DBM file \"%s\": %s", log_escape(msr->mp, dbm_filename),
            get_apr_error(msr->mp, rc));
        return -1;
    }

    /* First get a list of all keys. */
    keys_arr = apr_array_make(msr->mp, 256, sizeof(char *));
    rc = apr_sdbm_lock(dbm, APR_FLOCK_SHARED);
    if (rc != APR_SUCCESS) {
        msr_log(msr, 1, "Failed to lock DBM file \"%s\": %s", log_escape(msr->mp, dbm_filename),
            get_apr_error(msr->mp, rc));
        apr_sdbm_close(dbm);
        return -1;
    }

    /* No one can write to the file while we're
     * doing this so let's do it as fast as we can.
     */
    rc = apr_sdbm_firstkey(dbm, &key);
    while(rc == APR_SUCCESS) {
        char *s = apr_pstrmemdup(msr->mp, key.dptr, key.dsize);
        *(char **)apr_array_push(keys_arr) = s;
        rc = apr_sdbm_nextkey(dbm, &key);
    }
    apr_sdbm_unlock(dbm);

    msr_log(msr, 9, "Found %i record(s) in file \"%s\".", keys_arr->nelts,
        log_escape(msr->mp, dbm_filename));

    /* Now retrieve the entires one by one. */
    keys = (char **)keys_arr->elts;
    for (i = 0; i < keys_arr->nelts; i++) {
        key.dptr = keys[i];
        key.dsize = strlen(key.dptr) + 1;

        rc = apr_sdbm_fetch(dbm, &value, key);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "Failed reading DBM file \"%s\": %s",
                log_escape(msr->mp, dbm_filename), get_apr_error(msr->mp, rc));
            apr_sdbm_close(dbm);
            return -1;
        }

        if (value.dptr != NULL) {
            apr_table_t *col = NULL;
            msc_string *var = NULL;

            col = collection_unpack(msr, value.dptr, value.dsize, 0);
            if (col == NULL) {
                return -1;
            }

            var = (msc_string *)apr_table_get(col, "__expire_KEY");
            if (var == NULL) {
                msr_log(msr, 1, "Collection cleanup discovered entry with no "
                    "__expire_KEY (name \"%s\", key \"%s\").",
                    log_escape(msr->mp, col_name), log_escape(msr->mp, key.dptr));
            } else {
                unsigned int expiry_time = atoi(var->value);

                msr_log(msr, 9, "Record (name \"%s\", key \"%s\") set to expire in %i seconds.",
                    log_escape(msr->mp, col_name), log_escape(msr->mp, key.dptr),
                    expiry_time - now);

                if (expiry_time <= now) {
                    rc = apr_sdbm_delete(dbm, key);
                    if (rc != APR_SUCCESS) {
                        msr_log(msr, 1, "Failed deleting collection (name \"%s\", "
                            "key \"%s\"): %s", log_escape(msr->mp, col_name),
                            log_escape(msr->mp, key.dptr), get_apr_error(msr->mp, rc));
                        return -1;
                    }
                    msr_log(msr, 4, "Removed stale collection (name \"%s\", "
                            "key \"%s\").", log_escape(msr->mp, col_name),
                            log_escape(msr->mp, key.dptr));
                }
            }
        } else {
            /* Ignore entry not found - it may have been removed in the meantime. */
        }
    }

    apr_sdbm_close(dbm);

    return 1;
}
