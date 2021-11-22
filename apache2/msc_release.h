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

#define MODSEC_VERSION_MAJOR       "2"
#define MODSEC_VERSION_MINOR       "9"
#define MODSEC_VERSION_MAINT       "5"
#define MODSEC_VERSION_TYPE        ""
#define MODSEC_VERSION_RELEASE     ""

#define MODSEC_VERSION_SUFFIX MODSEC_VERSION_TYPE MODSEC_VERSION_RELEASE

#define MODSEC_VERSION \
  MODSEC_VERSION_MAJOR "." MODSEC_VERSION_MINOR "." MODSEC_VERSION_MAINT \
  MODSEC_VERSION_SUFFIX

/* Apache Module Defines */
#ifdef	VERSION_IIS
#define MODSEC_MODULE_NAME "ModSecurity for IIS (STABLE)"
#else
#ifdef	VERSION_NGINX
#define MODSEC_MODULE_NAME "ModSecurity for nginx (STABLE)"
#else
#ifdef	VERSION_STANDALONE
#define MODSEC_MODULE_NAME "ModSecurity Standalone (STABLE)"
#else
#define MODSEC_MODULE_NAME "ModSecurity for Apache"
#endif
#endif
#endif
#define MODSEC_MODULE_VERSION MODSEC_VERSION
#define MODSEC_MODULE_NAME_FULL MODSEC_MODULE_NAME "/" MODSEC_MODULE_VERSION " (http://www.modsecurity.org/)"

int DSOLOCAL get_modsec_build_type(const char *name);

#endif /* _MSC_RELEASE_H_ */
