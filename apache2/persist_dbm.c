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

#include "persist_dbm.h"
#include "apr_sdbm.h"
#ifdef MEMORY_DATABASE_ENABLE
#include "ag_mdb_external.h"
#endif
#include "waf_lock_external.h"

#ifdef MEMORY_DATABASE_ENABLE
void* dcfg_searchAGMDBhandler(const char* col_name, struct agmdb_handle_entry *handles){
    struct agmdb_handle_entry *handle_entry = handles;
    while(handle_entry != NULL){
        if(strcmp(col_name, handle_entry->col_name) == 0)
            break;
        handle_entry = handle_entry->next;
    }
    if(handle_entry != NULL)
        return handle_entry->handle;
    else
        return NULL;
}

void dcfg_insertAGMDBhandler(directory_config *root_dcfg, struct agmdb_handle_entry *new_handle){
    new_handle->next = (struct agmdb_handle_entry *)root_dcfg->agmdb_handles;
    root_dcfg->agmdb_handles = (void*)new_handle;
}
#endif
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
static apr_table_t *collection_retrieve_ex(int db_option, void *existing_dbm, modsec_rec *msr, const char *col_name, const char *col_key, int col_key_len)
{
    apr_table_t *col = NULL;
    apr_sdbm_datum_t key;
    apr_sdbm_datum_t *value = NULL;
    const apr_array_header_t *arr;
    apr_table_entry_t *te;
    int expired = 0;
    int i;
    int rc;
    int tmp_val_len = 0;
#ifdef MEMORY_DATABASE_ENABLE
    directory_config * root_dcfg;

    //variables used for ag_dbm 
    struct agmdb_handler *ag_dbm = NULL;
    char buffer[AGMDB_MAX_ENTRY_SIZE];
#endif
    //variables used for apr_sdbm
    struct apr_sdbm_t *apr_dbm = NULL;
    char *dbm_filename = NULL;

    //=================================
    //function body
    //=================================
    
    if (msr->txcfg->data_dir == NULL) {
        msr_log(msr, 1, "collection_retrieve_ex: Unable to retrieve collection (name \"%s\", key \"%s\"). Use "
            "SecDataDir to define data directory first.", log_escape(msr->mp, col_name),
            log_escape_ex(msr->mp, col_key, col_key_len));
        goto cleanup;
    }

    //---------------------------------
    //open database
    //---------------------------------
#ifdef MEMORY_DATABASE_ENABLE
    root_dcfg = msr->dcfg1->root_config;
    if(db_option == DB_OPT_AGMDB){
        if (existing_dbm == NULL) {
            ag_dbm = dcfg_searchAGMDBhandler(col_name, (struct agmdb_handle_entry*)(root_dcfg->agmdb_handles));
            if (ag_dbm == NULL) 
                goto cleanup;            
        }
        else {
            ag_dbm = (struct agmdb_handler *)existing_dbm;
        }
    }
    else{
#endif
        if (msr->txcfg->data_dir == NULL) {
            msr_log(msr, 1, "collection_retrieve_ex_origin: Unable to retrieve collection (name \"%s\", key \"%s\"). Use "
                "SecDataDir to define data directory first.", log_escape(msr->mp, col_name),
                log_escape_ex(msr->mp, col_key, col_key_len));
            goto cleanup;
        }
        
        dbm_filename = apr_pstrcat(msr->mp, msr->txcfg->data_dir, "/", col_name, NULL);
        
        if (existing_dbm == NULL) {
#ifdef GLOBAL_COLLECTION_LOCK
	    rc = waf_get_exclusive_lock(msr->modsecurity->dbm_lock);
	    if (waf_lock_is_error(rc)) {
                msr_log(msr, 1, "collection_retrieve_ex: Failed to lock proc mutex: %s",
                        get_apr_error(msr->mp, rc));
                goto cleanup;
            }
#endif
            rc = apr_sdbm_open(&apr_dbm, dbm_filename, APR_READ | APR_SHARELOCK,
                CREATEMODE, msr->mp);
            if (rc != APR_SUCCESS) {
                apr_dbm = NULL;
#ifdef GLOBAL_COLLECTION_LOCK
	            waf_free_exclusive_lock(msr->modsecurity->dbm_lock);
#endif
                goto cleanup;
            }
        }
        else {
            apr_dbm = existing_dbm;
        }
#ifdef MEMORY_DATABASE_ENABLE
    }
#endif
    
    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "collection_retrieve_ex: collection_retrieve_ex: Retrieving collection (name \"%s\", filename \"%s\")",log_escape(msr->mp, col_name),
                log_escape(msr->mp, dbm_filename));
    }

    //---------------------------------
    //prepare the key and value container
    //---------------------------------
    key.dptr = (char *)col_key;
    key.dsize = col_key_len + 1;

    value = (apr_sdbm_datum_t *)apr_pcalloc(msr->mp, sizeof(apr_sdbm_datum_t));

    //---------------------------------
    //get the key
    //---------------------------------
#ifdef MEMORY_DATABASE_ENABLE
    if(db_option == DB_OPT_AGMDB){
        //if not called by collection_store(), need to get lock
        if(existing_dbm == NULL ){
            rc = AGMDB_getSharedLock(ag_dbm);
            if(AGMDB_isError(rc)){
                msr_log(msr, 1, "collection_retrieve_ex_agmdb: Failed to get shared lock. Error info: %s.", AGMDB_getErrorInfo(rc));
                goto cleanup;
            }
        }
        rc = AGMDB_get(ag_dbm, col_key, col_key_len, buffer, AGMDB_MAX_ENTRY_SIZE, &tmp_val_len);
        if(AGMDB_isError(rc)) {
            msr_log(msr, 1, "collection_retrieve_ex_agmdb: Failed to read from database \"%s\": %s. Error info: %s.", log_escape(msr->mp,
                col_name), col_key, AGMDB_getErrorInfo(rc));
            goto cleanup;
        }
        
        if(existing_dbm == NULL ){
            rc = AGMDB_freeSharedLock(ag_dbm);
            if(AGMDB_isError(rc)) {
                msr_log(msr, 1, "collection_retrieve_ex_agmdb: Failed to free shared lock. Error info: %s.", AGMDB_getErrorInfo(rc));
                goto cleanup;
            }
        }
        if (tmp_val_len == 0) { /* Key not found in DBM file. */
             goto cleanup;
        }
        value->dptr = buffer;
        value->dsize = tmp_val_len;
    }
    else{
#endif
        rc = apr_sdbm_fetch(apr_dbm, value, key);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "collection_retrieve_ex: Failed to read from DBM file \"%s\": %s", log_escape(msr->mp,
                dbm_filename), get_apr_error(msr->mp, rc));
            goto cleanup;
        }
        if (value->dptr == NULL) { /* Key not found in DBM file. */
            goto cleanup;
        }
        /* Close after "value" used from fetch or memory may be overwritten. */
        if (existing_dbm == NULL) {
            apr_sdbm_close(apr_dbm);
#ifdef GLOBAL_COLLECTION_LOCK
	        waf_free_exclusive_lock(msr->modsecurity->dbm_lock);
#endif
            apr_dbm = NULL;
        }
#ifdef MEMORY_DATABASE_ENABLE
    }
#endif

    //---------------------------------
    //unpack the collection
    //---------------------------------
    col = collection_unpack(msr, (const unsigned char *)value->dptr, value->dsize, 1);
    if (col == NULL) {
        goto cleanup;
    }

    //---------------------------------
    // Remove expired variables. 
    //---------------------------------
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

    //---------------------------------
    //Delete the collection if the variable "KEY" does not exist.
    //---------------------------------
    if (apr_table_get(col, "KEY") == NULL) {
        int fail_flag = 0;
#ifdef MEMORY_DATABASE_ENABLE        
        if(db_option == DB_OPT_AGMDB){
            if(existing_dbm == NULL){
                rc = AGMDB_getExclusiveLock(ag_dbm);
                if(AGMDB_isError(rc)){
                    msr_log(msr, 1, "collection_retrieve_ex: Failed to get exclusive lock. Error info: %s.", AGMDB_getErrorInfo(rc));
                    goto cleanup;
                }
            }
            rc = AGMDB_delete(ag_dbm, col_key, col_key_len);
            if(AGMDB_isError(rc))
                fail_flag = 1;

            if(existing_dbm == NULL){
                rc = AGMDB_freeExclusiveLock(ag_dbm);
                if(AGMDB_isError(rc)){
                    msr_log(msr, 1, "collection_retrieve_ex: Failed to free exclusive lock. Error info: %s.", AGMDB_getErrorInfo(rc));
                    goto cleanup;
                }
            }
        }
        else{
#endif
            if (existing_dbm == NULL) {
#ifdef GLOBAL_COLLECTION_LOCK
	        rc = waf_get_exclusive_lock(msr->modsecurity->dbm_lock);
	        if (waf_lock_is_error(rc)) {
                    msr_log(msr, 1, "collection_retrieve_ex: Failed to lock proc mutex: %s",
                            get_apr_error(msr->mp, rc));
                    goto cleanup;
                }
#endif
                rc = apr_sdbm_open(&apr_dbm, dbm_filename, APR_CREATE | APR_WRITE | APR_SHARELOCK,
                    CREATEMODE, msr->mp);
                if (rc != APR_SUCCESS) {
                    msr_log(msr, 1, "collection_retrieve_ex: Failed to access DBM file \"%s\": %s",
                        log_escape(msr->mp, dbm_filename), get_apr_error(msr->mp, rc));
                    apr_dbm = NULL;
#ifdef GLOBAL_COLLECTION_LOCK
	                waf_free_exclusive_lock(msr->modsecurity->dbm_lock);
#endif
                    goto cleanup;
                }
            }
            else {
                apr_dbm = existing_dbm;
            }

            rc = apr_sdbm_delete(apr_dbm, key);
            if (rc != APR_SUCCESS) 
                fail_flag = 1;
            if (existing_dbm == NULL) {
                apr_sdbm_close(apr_dbm);
#ifdef GLOBAL_COLLECTION_LOCK
	            waf_free_exclusive_lock(msr->modsecurity->dbm_lock);
#endif
                apr_dbm = NULL;
            }
#ifdef MEMORY_DATABASE_ENABLE             
        }
#endif        

        
        if (fail_flag == 1) {
#ifdef LOG_NO_COLL_DELET_PB
            if (msr->txcfg->debuglog_level >= 9)
#endif
                msr_log(msr, 1, "collection_retrieve_ex: Failed deleting collection (name \"%s\", "
                        "key \"%s\"): %s", log_escape(msr->mp, col_name),
                    log_escape_ex(msr->mp, col_key, col_key_len), get_apr_error(msr->mp, rc));
                msr->msc_sdbm_delete_error = 1;
                goto cleanup;
            
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

    //---------------------------------
    // Update UPDATE_RATE 
    //---------------------------------
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

    if ((existing_dbm == NULL) && (apr_dbm != NULL) && (db_option == DB_OPT_ORIGIN)) {
        /* Should not ever get here */
        msr_log(msr, 1, "collection_retrieve_ex: Internal Error: Collection remained open (name \"%s\", key \"%s\").",
            log_escape(msr->mp, col_name), log_escape_ex(msr->mp, col_key, col_key_len));

        apr_sdbm_close(apr_dbm);
#ifdef GLOBAL_COLLECTION_LOCK
	    waf_free_exclusive_lock(msr->modsecurity->dbm_lock);
#endif
    }

    return col;
cleanup:
#ifdef MEMORY_DATABASE_ENABLE
    if(db_option == DB_OPT_AGMDB){
        return NULL;
    }
    else{
#endif
        if ((existing_dbm == NULL) && apr_dbm) {
            apr_sdbm_close(apr_dbm);
#ifdef GLOBAL_COLLECTION_LOCK
	        waf_free_exclusive_lock(msr->modsecurity->dbm_lock);
#endif
        }
#ifdef MEMORY_DATABASE_ENABLE
    }
#endif
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
#ifdef MEMORY_DATABASE_ENABLE
    if(msr->dcfg1->db_option == DB_OPT_AGMDB){
        rtable = collection_retrieve_ex(DB_OPT_AGMDB , NULL, msr, col_name, col_key, col_key_len);
    }
    if(msr->dcfg1->db_option == DB_OPT_ORIGIN){
#endif
        rtable = collection_retrieve_ex(DB_OPT_ORIGIN, NULL, msr, col_name, col_key, col_key_len);
#ifdef MEMORY_DATABASE_ENABLE
    }
#endif
    msr->time_storage_read += apr_time_now() - time_before;
    
    return rtable;
}

/**
 *
 */
static int collection_store_ex(int db_option, modsec_rec *msr, apr_table_t *col){
    msc_string *var_name = NULL, *var_key = NULL;
    unsigned char *blob = NULL;
    unsigned int blob_size = 0, blob_offset = 0;
    int rc;
    apr_sdbm_datum_t key;
    apr_sdbm_datum_t value;
    const apr_array_header_t *arr;
    apr_table_entry_t *te;
    int i;
    const apr_table_t *stored_col = NULL;
    const apr_table_t *orig_col = NULL;

    directory_config *dcfg = msr->dcfg1;
#ifdef MEMORY_DATABASE_ENABLE
    directory_config *root_dcfg = dcfg->root_config;

    //variable used for AGMDB
    struct agmdb_handler *ag_dbm = NULL;
    struct agmdb_handle_entry *new_handle;
#endif
    //variable used for apr_sdbm
    char *dbm_filename = NULL;
    apr_sdbm_t *apr_dbm = NULL;

    //---------------------------------
    //prepare collection name and key
    //---------------------------------
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

    /* Delete IS_NEW on store. */
    apr_table_unset(col, "IS_NEW");

    /* Delete UPDATE_RATE on store to save space as it is calculated */
    apr_table_unset(col, "UPDATE_RATE");

    //---------------------------------
    // Update the timeout value. 
    //---------------------------------
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

    //---------------------------------
    // LAST_UPDATE_TIME 
    //---------------------------------
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

    //---------------------------------
    // UPDATE_COUNTER 
    //---------------------------------
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

    //---------------------------------
    //open database
    //---------------------------------
#ifdef MEMORY_DATABASE_ENABLE
    if(db_option == DB_OPT_AGMDB){
        ag_dbm = dcfg_searchAGMDBhandler(var_name->value, (struct agmdb_handle_entry*)(msr->dcfg1->agmdb_handles));
        if(ag_dbm == NULL) {
            //Create the DB
            root_dcfg = msr->dcfg1->root_config;
#ifdef _WIN32           
            dbm_filename = apr_pstrcat(root_dcfg->mp, "Global\\", root_dcfg->data_dir, "/", var_name->value, NULL);
#else
            dbm_filename = apr_pstrcat(root_dcfg->mp, root_dcfg->data_dir, "/", var_name->value, NULL);
#endif
            if(root_dcfg == NULL){
                msr_log(msr, 1, "collection_retrieve_ex_agmdb: Cannot find root_config in msr->dcfg1.");
                goto error;
            }
            new_handle = (struct agmdb_handle_entry *)apr_pcalloc(root_dcfg->mp, sizeof(struct agmdb_handle_entry));
            new_handle->col_name = (char*)apr_pcalloc(root_dcfg->mp, var_name->value_len);
            new_handle->handle = apr_pcalloc(root_dcfg->mp, sizeof(struct agmdb_handler));
            strcpy((char*)(new_handle->col_name), var_name->value);
            
            rc = AGMDB_openDB(new_handle->handle, dbm_filename, strlen(dbm_filename), MAXIMUM_AGMDB_ENTRY_NUM);
            if(AGMDB_isError(rc)){
                msr_log(msr, 1, "collection_retrieve_ex_agmdb: Failed to create DBM name: %s. Error info: %s", dbm_filename, AGMDB_getErrorInfo(rc));
                goto error;
            }
            ag_dbm = new_handle->handle;
            dcfg_insertAGMDBhandler(root_dcfg, new_handle);
        }
    }
    else{
#endif
        // ENH: lowercase the var name in the filename
        dbm_filename = apr_pstrcat(msr->mp, msr->txcfg->data_dir, "/", var_name->value, NULL);
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "collection_store_ex_origin: Retrieving collection (name \"%s\", filename \"%s\")",log_escape(msr->mp, var_name->value),
                    log_escape(msr->mp, dbm_filename));
        }
        
#ifdef GLOBAL_COLLECTION_LOCK
        /* Need to lock to pull in the stored data again and apply deltas. */
	rc = waf_get_exclusive_lock(msr->modsecurity->dbm_lock);
	if (waf_lock_is_error(rc)) {
            msr_log(msr, 1, "collection_store: Failed to lock proc mutex: %s",
                    get_apr_error(msr->mp, rc));
            goto error;
        }
#endif
        
        rc = apr_sdbm_open(&apr_dbm, dbm_filename, APR_CREATE | APR_WRITE | APR_SHARELOCK,
            CREATEMODE, msr->mp);
        if (rc != APR_SUCCESS) {
#ifdef GLOBAL_COLLECTION_LOCK
	        waf_free_exclusive_lock(msr->modsecurity->dbm_lock);
#endif
            msr_log(msr, 1, "collection_store_ex_origin: Failed to access DBM file \"%s\": %s", log_escape(msr->mp, dbm_filename),
                get_apr_error(msr->mp, rc));
            apr_dbm = NULL;
            goto error;
        }
#ifdef MEMORY_DATABASE_ENABLE
    }
#endif

    //---------------------------------
    //Lock and prepare to get the original collection
    //---------------------------------
#ifdef MEMORY_DATABASE_ENABLE
    if(db_option == DB_OPT_AGMDB){
        rc = AGMDB_getExclusiveLock(ag_dbm);
        if (AGMDB_isError(rc)) {
#ifdef _WIN32
            int lasterr = (int)GetLastError();
            msr_log(msr, 1, "collection_store: Failed to getExclusiveLock, lasterr = %d. Error info: %s", lasterr, AGMDB_getErrorInfo(rc));
#else
            msr_log(msr, 1, "collection_store: Failed to getExclusiveLock, errno = %d. Error info: %s", errno, AGMDB_getErrorInfo(rc));
#endif
            goto error;
        }
    }
    else{
#endif 
#ifndef GLOBAL_COLLECTION_LOCK
        /* Need to lock to pull in the stored data again and apply deltas. */
        rc = apr_sdbm_lock(apr_dbm, APR_FLOCK_EXCLUSIVE);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "collection_store_ex_origin: Failed to exclusivly lock DBM file \"%s\": %s", log_escape(msr->mp, dbm_filename),
                get_apr_error(msr->mp, rc));
            goto error;
        }
#endif
#ifdef MEMORY_DATABASE_ENABLE
    }
#endif

    //---------------------------------
    //get the origin record
    //---------------------------------
    orig_col = (const apr_table_t *)apr_table_get(msr->collections_original, var_name->value);
    if (orig_col != NULL) {
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "collection_store: Re-retrieving collection prior to store: %s",
                apr_psprintf(msr->mp, "%.*s", var_name->value_len, var_name->value));
        }
#ifdef MEMORY_DATABASE_ENABLE
        if(db_option == DB_OPT_AGMDB)
            stored_col = (const apr_table_t *)collection_retrieve_ex(db_option, ag_dbm, msr, var_name->value, var_key->value, var_key->value_len);
        else
#endif
            stored_col = (const apr_table_t *)collection_retrieve_ex(db_option, apr_dbm, msr, var_name->value, var_key->value, var_key->value_len);
    }

    //---------------------------------
    //pack the collection
    //---------------------------------
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
#ifdef MEMORY_DATABASE_ENABLE
    if(db_option == DB_OPT_AGMDB){
        blob = apr_pcalloc(msr->mp, blob_size);
        if (blob == NULL) {
            if (ag_dbm != NULL) {
                rc = AGMDB_freeExclusiveLock(ag_dbm);
                if(AGMDB_isError(rc))
                    msr_log(msr, 1, "collection_stror_ex: Fail to free exclusive lock. Error info: %s", AGMDB_getErrorInfo(rc));
            }
            msr_log(msr, 1, "collection_store_ex_agdb: fail to create blob");            
            return -1;
        }
    }
    else{
#endif
        blob = apr_pcalloc(msr->mp, blob_size);
        if (blob == NULL) {
            if (apr_dbm != NULL) {
#ifdef GLOBAL_COLLECTION_LOCK
                apr_sdbm_close(apr_dbm);
	            waf_free_exclusive_lock(msr->modsecurity->dbm_lock);
#else
                apr_sdbm_unlock(apr_dbm);
                apr_sdbm_close(apr_dbm);
#endif
            }
            return -1;
        }
#ifdef MEMORY_DATABASE_ENABLE
    }
#endif

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

    //---------------------------------
    // And, finally, store it. 
    //---------------------------------
    key.dptr = var_key->value;
    key.dsize = var_key->value_len + 1;

    value.dptr = (char *)blob;
    value.dsize = blob_size;

#ifdef MEMORY_DATABASE_ENABLE
    if(db_option == DB_OPT_AGMDB) {
        rc = AGMDB_set(ag_dbm, var_key->value, var_key->value_len, (char*)blob, blob_size);
        if(AGMDB_isError(rc)) {
            msr_log(msr, 1, "collection_store_ex_agmdb: Failed to write to database key: %s. Error info: %s.", var_key->value, AGMDB_getErrorInfo(rc));
        }

        rc = AGMDB_freeExclusiveLock(ag_dbm);
        if(AGMDB_isError(rc)){
            msr_log(msr, 1, "collection_store_ex_agmdb: Failed to free exclusive lock. Error info: %s.", AGMDB_getErrorInfo(rc));
            return -1;
        }
    }
    else {
#endif
        rc = apr_sdbm_store(apr_dbm, key, value, APR_SDBM_REPLACE);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "collection_store: Failed to write to DBM file \"%s\": %s", dbm_filename,
                    get_apr_error(msr->mp, rc));
            if (apr_dbm != NULL) {
#ifdef GLOBAL_COLLECTION_LOCK
                apr_sdbm_close(apr_dbm);
	            waf_free_exclusive_lock(msr->modsecurity->dbm_lock);
#else
                apr_sdbm_unlock(apr_dbm);
                apr_sdbm_close(apr_dbm);
#endif
            }

            return -1;
        }
#ifdef GLOBAL_COLLECTION_LOCK
        apr_sdbm_close(apr_dbm);
	    waf_free_exclusive_lock(msr->modsecurity->dbm_lock);
#else
        apr_sdbm_unlock(apr_dbm);
        apr_sdbm_close(apr_dbm);
#endif
#ifdef MEMORY_DATABASE_ENABLE
    }
#endif

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "collection_store: Persisted collection (name \"%s\", key \"%s\").",
            log_escape_ex(msr->mp, var_name->value, var_name->value_len),
            log_escape_ex(msr->mp, var_key->value, var_key->value_len));
    }

    return 0;
    
error:
    return -1;
}

/**
 *
 */
int collection_store(modsec_rec *msr, apr_table_t *col){
    int rc;
#ifdef MEMORY_DATABASE_ENABLE
    if(msr->dcfg1->db_option == DB_OPT_AGMDB){
        rc = collection_store_ex(DB_OPT_AGMDB , msr, col);
    }
    if(msr->dcfg1->db_option == DB_OPT_ORIGIN){
#endif
        rc = collection_store_ex(DB_OPT_ORIGIN, msr, col);
#ifdef MEMORY_DATABASE_ENABLE
    }
#endif
    return rc;
}

/**
 *
 */
static int collections_remove_stale_ex(int db_option, modsec_rec *msr, const char *col_name, int time_sec){
#ifdef MEMORY_DATABASE_ENABLE
    //variables for AGMDB
    struct agmdb_handler *ag_dbm = NULL;
    directory_config * root_dcfg = msr->dcfg1->root_config;
#endif
    //variables for apr_sdbm
    char *dbm_filename = NULL;
    apr_sdbm_datum_t key, value;
    apr_sdbm_t *apr_dbm = NULL;
    apr_status_t rc;
    apr_array_header_t *keys_arr;
    char **keys;
    apr_time_t now = apr_time_sec(msr->request_time);
    int i,rc2;
    
    //---------------------------------
    //AGMDB
    //---------------------------------
#ifdef MEMORY_DATABASE_ENABLE
    if(db_option == DB_OPT_AGMDB){
        if(root_dcfg == NULL)
            return -1;
        ag_dbm = dcfg_searchAGMDBhandler(col_name, (struct agmdb_handle_entry*)(root_dcfg->agmdb_handles));
        if(ag_dbm == NULL)
            return 1;
        rc2 = AGMDB_removeStale(ag_dbm);
        if(AGMDB_isError(rc2))
        msr_log(msr, 1, "collections_remove_stale_ex_agmdb: error in remove stale. Error info: %s", AGMDB_getErrorInfo(rc2));
    }
#endif
    //---------------------------------
    //apr_sdbm
    //---------------------------------
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

#ifdef GLOBAL_COLLECTION_LOCK
    rc = waf_get_exclusive_lock(msr->modsecurity->dbm_lock);
    if (waf_lock_is_error(rc)) {
        msr_log(msr, 1, "collections_remove_stale: Failed to lock proc mutex: %s",
                get_apr_error(msr->mp, rc));
        goto error;
    }
#endif
    rc = apr_sdbm_open(&apr_dbm, dbm_filename, APR_CREATE | APR_WRITE | APR_SHARELOCK,
            CREATEMODE, msr->mp);
    if (rc != APR_SUCCESS) {
#ifdef GLOBAL_COLLECTION_LOCK
        waf_free_exclusive_lock(msr->modsecurity->dbm_lock);
#endif
        msr_log(msr, 1, "collections_remove_stale: Failed to access DBM file \"%s\": %s", log_escape(msr->mp, dbm_filename),
                get_apr_error(msr->mp, rc));
        apr_dbm = NULL;
        goto error;
    }

    /* First get a list of all keys. */
    keys_arr = apr_array_make(msr->mp, 256, sizeof(char *));

#ifndef GLOBAL_COLLECTION_LOCK
    rc = apr_sdbm_lock(apr_dbm, APR_FLOCK_SHARED);
    if (rc != APR_SUCCESS) {
        msr_log(msr, 1, "collections_remove_stale: Failed to lock DBM file \"%s\": %s", log_escape(msr->mp, dbm_filename),
            get_apr_error(msr->mp, rc));
        goto error;
    }
#endif

    /* No one can write to the file while doing this so
     * do it as fast as possible.
     */
    rc = apr_sdbm_firstkey(apr_dbm, &key);
    while(rc == APR_SUCCESS) {
        if (key.dsize) {
            char *s = apr_pstrmemdup(msr->mp, key.dptr, key.dsize - 1);
            *(char **)apr_array_push(keys_arr) = s;
        }
        rc = apr_sdbm_nextkey(apr_dbm, &key);
    }
#ifndef GLOBAL_COLLECTION_LOCK
    apr_sdbm_unlock(apr_dbm);
#endif

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "collections_remove_stale: Found %d record(s) in file \"%s\".", keys_arr->nelts,
            log_escape(msr->mp, dbm_filename));
    }

    /* Now retrieve the entires one by one. */
    keys = (char **)keys_arr->elts;
    for (i = 0; i < keys_arr->nelts; i++) {
        key.dptr = keys[i];
        key.dsize = strlen(key.dptr) + 1;

        rc = apr_sdbm_fetch(apr_dbm, &value, key);
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
                    rc = apr_sdbm_delete(apr_dbm, key);
                    if (rc != APR_SUCCESS) {
#ifdef LOG_NO_COLL_DELET_PB
			if (msr->txcfg->debuglog_level >= 9)
#endif
                        msr_log(msr, 1, "collections_remove_stale: Failed deleting collection (name \"%s\", "
                            "key \"%s\"): %s", log_escape(msr->mp, col_name),
                            log_escape_ex(msr->mp, key.dptr, key.dsize - 1), get_apr_error(msr->mp, rc));
                    msr->msc_sdbm_delete_error = 1;
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

    apr_sdbm_close(apr_dbm);
#ifdef GLOBAL_COLLECTION_LOCK
    waf_free_exclusive_lock(msr->modsecurity->dbm_lock);
#endif
    return 1;

error:

    if (apr_dbm) {
        apr_sdbm_close(apr_dbm);
#ifdef GLOBAL_COLLECTION_LOCK
        waf_free_exclusive_lock(msr->modsecurity->dbm_lock);
#endif
    }
    return -1;
}

/**
 *
 */
int collections_remove_stale(modsec_rec *msr, const char *col_name) {
    int rc;
    int time_sec = (int)time(NULL);
#ifdef MEMORY_DATABASE_ENABLE
    if(msr->dcfg1->db_option == DB_OPT_AGMDB){
        return collections_remove_stale_ex(DB_OPT_AGMDB , msr, col_name, time_sec);       
    }
    if(msr->dcfg1->db_option == DB_OPT_ORIGIN){
#endif
        return collections_remove_stale_ex(DB_OPT_ORIGIN, msr, col_name, time_sec);
#ifdef MEMORY_DATABASE_ENABLE
    }
#endif
    
}
