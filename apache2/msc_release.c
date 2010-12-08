/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2010 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * This product is released under the terms of the General Public Licence,
 * version 2 (GPLv2). Please refer to the file LICENSE (included with this
 * distribution) which contains the complete text of the licence.
 *
 * There are special exceptions to the terms and conditions of the GPL
 * as it is applied to this software. View the full text of the exception in
 * file MODSECURITY_LICENSING_EXCEPTION in the directory of this software
 * distribution.
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address support@trustwave.com.
 *
 */

#include "msc_release.h"

modsec_build_type_rec modsec_build_type[] = {
    { "-dev", 1 },     /* Development build */
    { "-rc", 3 },      /* Release Candidate build */
    { "", 9 },         /* Production build */
    { "-tw", 9 },      /* Truswave Holdings build */
    { "-trunk", 9 },   /* Trunk build */
    { NULL, -1 }       /* terminator */
};

int get_modsec_build_type(const char *name)
{
    int i;

    for (i = 0; modsec_build_type[i].name != NULL; i++) {
        if (strcmp(((name == NULL) ? MODSEC_VERSION_TYPE : name), modsec_build_type[i].name) == 0) {
            return modsec_build_type[i].val;
        }
    }

    return 9; /* so no warning */
}
