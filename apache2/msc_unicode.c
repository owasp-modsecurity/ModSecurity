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

#include "msc_unicode.h"

#define CODEPAGE_SEPARATORS  " \t\n\r"

/** \brief Load Unicode file
 *
 * \param dcfg Pointer to directory configuration
 * \param error_msg Error message
 *
 * \retval 1 On Success
 * \retval 0 On Fail
 */
static int unicode_map_create(directory_config *dcfg, char **error_msg)
{
    char errstr[1024];
    apr_pool_t *mp = dcfg->mp;
    unicode_map *u_map = dcfg->u_map;
    apr_int32_t wanted = APR_FINFO_SIZE;
    apr_finfo_t finfo;
    apr_status_t rc;
    apr_size_t nbytes;
    unsigned int codepage = 0;
    char *buf = NULL, *p = NULL, *savedptr = NULL;
    char *ucode = NULL, *hmap = NULL;
    int found = 0, processing = 0;
    int Code = 0, Map = 0;

    if(unicode_map_table != NULL)   {
        free(unicode_map_table);
        unicode_map_table = NULL;
    }

    if ((rc = apr_file_open(&u_map->map, u_map->mapfn, APR_READ, APR_OS_DEFAULT, mp)) != APR_SUCCESS) {
        *error_msg = apr_psprintf(mp, "Could not open unicode map file \"%s\": %s", u_map->mapfn, apr_strerror(rc, errstr, 1024));
        return 0;
    }

    if ((rc = apr_file_info_get(&finfo, wanted, u_map->map)) != APR_SUCCESS)  {
        *error_msg = apr_psprintf(mp, "Could not cannot get unicode map file information \"%s\": %s", u_map->mapfn, apr_strerror(rc, errstr, 1024));
        apr_file_close(u_map->map);
        return 0;
    }

    buf = (char *)malloc(finfo.size+1);

    if (buf == NULL)   {
        *error_msg = apr_psprintf(mp, "Could not alloc memory for unicode map");
        apr_file_close(u_map->map);
        return 0;
    }

    rc = apr_file_read_full(u_map->map, buf, finfo.size, &nbytes);

    if (unicode_map_table != NULL)  {
        memset(unicode_map_table, -1, (sizeof(int)*65536));
    } else {
        unicode_map_table = (int *)malloc(sizeof(int) * 65536);

        if(unicode_map_table == NULL) {
            *error_msg = apr_psprintf(mp, "Could not alloc memory for unicode map");
            free(buf);
            buf = NULL;
            apr_file_close(u_map->map);
            return 0;
        }

        memset(unicode_map_table, -1, (sizeof(int)*65536));
    }

    /* Setting some unicode values - http://tools.ietf.org/html/rfc3490#section-3.1 */

    /* Set 0x3002 -> 0x2e */
    unicode_map_table[0x3002] = 0x2e;
    /* Set 0xFF61 -> 0x2e */
    unicode_map_table[0xff61] = 0x2e;
    /* Set 0xFF0E -> 0x2e */
    unicode_map_table[0xff0e] = 0x2e;
    /* Set 0x002E -> 0x2e */
    unicode_map_table[0x002e] = 0x2e;

    p = apr_strtok(buf,CODEPAGE_SEPARATORS,&savedptr);

    while (p != NULL)   {

        codepage = atol(p);

        if (codepage == unicode_codepage)   {
            found = 1;
        }

        if (found == 1 && (strchr(p,':') != NULL))   {
            char *mapping = strdup(p);
            processing = 1;

            if(mapping != NULL) {
                ucode = apr_strtok(mapping,":", &hmap);
                sscanf(ucode,"%x",&Code);
                sscanf(hmap,"%x",&Map);
                if(Code >= 0 || Code <= 65535)    {
                    unicode_map_table[Code] = Map;
                }

                free(mapping);
                mapping = NULL;
            }
        }

        if (processing == 1 && (strchr(p,':') == NULL)) {
            free(buf);
            buf = NULL;
            break;
        }

        p = apr_strtok(NULL,CODEPAGE_SEPARATORS,&savedptr);
    }

    apr_file_close(u_map->map);

    free(buf);
    buf = NULL;

    return 1;
}


/** \brief Init unicode map
 *
 * \param dcfg Pointer to directory configuration
 * \param mapfn Unicode map filename
 * \param error_msg Error message
 *
 * \retval unicode_map_create On Success
 * \retval -1 On Fail
 */
int unicode_map_init(directory_config *dcfg, const char *mapfn, char **error_msg)
{

    *error_msg = NULL;

    if ((dcfg->u_map == NULL) || (dcfg->u_map == NOT_SET_P)) {
        dcfg->u_map = apr_pcalloc(dcfg->mp, sizeof(unicode_map));
        if (dcfg->u_map == NULL)  {
            return -1;
        }
    }

    dcfg->u_map->map = NULL;
    dcfg->u_map->mapfn = apr_pstrdup(dcfg->mp, mapfn);

    return unicode_map_create(dcfg, error_msg);
}

