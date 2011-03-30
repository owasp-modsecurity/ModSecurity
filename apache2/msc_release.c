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
