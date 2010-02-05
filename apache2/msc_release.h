/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2010 Breach Security, Inc. (http://www.breach.com/)
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
 * other questions related to licensing please contact Breach Security, Inc.
 * directly using the email address support@breach.com.
 *
 */
#ifndef _MSC_RELEASE_H_
#define _MSC_RELEASE_H_

#include <stdlib.h>
#include <string.h>

/* ENH: Clean this mess up by detecting this is possible */
#if !(defined(_AIX) || defined(WIN32) || defined(CYGWIN) || defined(NETWARE) || defined(SOLARIS2) || defined(OSF1))
#define DSOLOCAL __attribute__((visibility("hidden")))
#else
#define DSOLOCAL
#endif

#if defined(DEBUG_MEM)
/* Nothing Yet */
#endif

/* For GNU C, tell the compiler to check printf like formatters */
#if (defined(__GNUC__) && !defined(SOLARIS2))
#define PRINTF_ATTRIBUTE(a,b) __attribute__((format (printf, a, b)))
#else
#define PRINTF_ATTRIBUTE(a,b)
#endif

typedef struct modsec_build_type_rec {
    const char * name;
    int          val;
} modsec_build_type_rec;
extern DSOLOCAL modsec_build_type_rec modsec_build_type[];

#define MODSEC_VERSION_MAJOR       "2"
#define MODSEC_VERSION_MINOR       "6"
#define MODSEC_VERSION_MAINT       "0"
#define MODSEC_VERSION_TYPE        "-trunk"
#define MODSEC_VERSION_RELEASE     ""

#define MODSEC_VERSION_SUFFIX MODSEC_VERSION_TYPE MODSEC_VERSION_RELEASE

#define MODSEC_VERSION \
  MODSEC_VERSION_MAJOR "." MODSEC_VERSION_MINOR "." MODSEC_VERSION_MAINT \
  MODSEC_VERSION_SUFFIX

/* Apache Module Defines */
#define MODSEC_MODULE_NAME "ModSecurity for Apache"
#define MODSEC_MODULE_VERSION MODSEC_VERSION
#define MODSEC_MODULE_NAME_FULL MODSEC_MODULE_NAME "/" MODSEC_MODULE_VERSION " (http://www.modsecurity.org/)"

int DSOLOCAL get_modsec_build_type(const char *name);

#endif /* _MSC_RELEASE_H_ */
