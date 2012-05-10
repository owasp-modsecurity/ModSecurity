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

#include "msc_gsb.h"

/** \brief Load GSB database
 *
 * \param dcfg Pointer to directory configuration
 * \param error_msg Error message
 *
 * \retval 1 On Success
 * \retval 0 On Fail
 */
static int gsb_db_create(directory_config *dcfg, char **error_msg)
{
    char errstr[1024];
    apr_pool_t *mp = dcfg->mp;
    gsb_db *gsb = dcfg->gsb;
    apr_int32_t wanted = APR_FINFO_SIZE;
    apr_finfo_t finfo;
    apr_status_t rc;
    apr_size_t nbytes;
    char *buf = NULL, *p = NULL, *savedptr = NULL;
    char *op = NULL;

    if ((rc = apr_file_open(&gsb->db, gsb->dbfn, APR_READ, APR_OS_DEFAULT, mp)) != APR_SUCCESS) {
        *error_msg = apr_psprintf(mp, "Could not open gsb database \"%s\": %s", gsb->dbfn, apr_strerror(rc, errstr, 1024));
        return 0;
    }

    if ((rc = apr_file_info_get(&finfo, wanted, gsb->db)) != APR_SUCCESS)  {
        *error_msg = apr_psprintf(mp, "Could not cannot get gsb malware file information \"%s\": %s", gsb->dbfn, apr_strerror(rc, errstr, 1024));
        apr_file_close(gsb->db);
        return 0;
    }

    buf = (char *)malloc(finfo.size+1);

    if (buf == NULL)   {
        *error_msg = apr_psprintf(mp, "Could not alloc memory for gsb data");
        apr_file_close(gsb->db);
        return 0;
    }

    rc = apr_file_read_full(gsb->db, buf, finfo.size, &nbytes);

    gsb->gsb_table = apr_hash_make(dcfg->mp);

    if (gsb->gsb_table == NULL)   {
        *error_msg = apr_psprintf(mp, "Could not alloc memory for gsb table");
        free(buf);
        buf = NULL;
        apr_file_close(gsb->db);
        return 0;
    }

    p = apr_strtok(buf,"\t",&savedptr);

    while (p != NULL)   {

        op = strchr(p,'+');

        if(op != NULL)   {
            char *hash = ++op;
            if(strlen(hash) == 32)
            apr_hash_set(gsb->gsb_table, hash, APR_HASH_KEY_STRING, "malware");
        }

        op = strchr(p,'-');

        if(op != NULL)   {
            char *hash = ++op;
            if(strlen(hash) == 32)
            apr_hash_set(gsb->gsb_table, hash, APR_HASH_KEY_STRING, NULL);
        }

        p = apr_strtok(NULL,"\t",&savedptr);
    }

    apr_file_close(gsb->db);

    free(buf);
    buf = NULL;

    return 1;
}


/** \brief Init GSB database
 *
 * \param dcfg Pointer to directory configuration
 * \param dbfn Database filename
 * \param error_msg Error message
 *
 * \retval gsb_db_create On Success
 * \retval -1 On Fail
 */
int gsb_db_init(directory_config *dcfg, const char *dbfn, char **error_msg)
{

    *error_msg = NULL;

    if ((dcfg->gsb == NULL) || (dcfg->gsb == NOT_SET_P)) {
        dcfg->gsb = apr_pcalloc(dcfg->mp, sizeof(gsb_db));
        if (dcfg->gsb == NULL)  {
            return -1;
        }
    }

    dcfg->gsb->db = NULL;
    dcfg->gsb->dbfn = apr_pstrdup(dcfg->mp, dbfn);

    return gsb_db_create(dcfg, error_msg);
}

