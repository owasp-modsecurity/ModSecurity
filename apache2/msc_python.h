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

#ifdef WITH_PYTHON

#ifndef _MSC_PYTHON_H_
#define _MSC_PYTHON_H_

typedef struct msc_python_script msc_python_script;

#include <Python.h>

#include "apr_general.h"
#include "apr_tables.h"
#include "modsecurity.h"

struct msc_python_script {
    const char *name;
    PyObject *pName;
    PyObject *pModule;
    PyObject *extInstance;
};

char DSOLOCAL *python_load(msc_python_script **script, const char *filename, apr_pool_t *pool);

#endif /* _MSC_PYTHON_H_ */
#endif /* WITH_PYTHON */
