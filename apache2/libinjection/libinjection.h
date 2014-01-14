/**
 * Copyright 2012, 2013 Nick Galbreath
 * nickg@client9.com
 * BSD License -- see COPYING.txt for details
 *
 * https://libinjection.client9.com/
 *
 */

#ifndef _LIBINJECTION_H
#define _LIBINJECTION_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Pull in size_t
 */
#include <string.h>

/*
 * Version info.
 *
 * This is moved into a function to allow SWIG and other auto-generated
 * binding to not be modified during minor release changes.  We change
 * change the version number in the c source file, and not regenerated
 * the binding
 *
 * See python's normalized version
 * http://www.python.org/dev/peps/pep-0386/#normalizedversion
 */
const char* libinjection_version(void);

/**
 * Simple API for SQLi detection - returns a SQLi fingerprint or NULL
 * is benign input
 *
 * \param[in] s  input string, may contain nulls, does not need to be null-terminated
 * \param[in] slen input string length
 * \param[out] fingerprint buffer of 8+ characters.  c-string,
 * \return 1 if SQLi, 0 if benign.  fingerprint will be set or set to empty string.
 */
int libinjection_sqli(const char* s, size_t slen, char fingerprint[]);

#ifdef __cplusplus
}
#endif

#endif /* _LIBINJECTION_H */
