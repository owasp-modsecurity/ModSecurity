/**
 * Copyright 2012, 2013 Nick Galbreath
 * nickg@client9.com
 * BSD License -- see COPYING.txt for details
 *
 *
 * HOW TO USE:
 *
 *   // Normalize query or postvar value
 *   // If it comes in urlencoded, then it's up to you
 *   // to urldecode it.  If it's in correct form already
 *   // then nothing to do!
 *
 *   sfilter s;
 *   int sqli = is_sqli(&s, user_string, new_len);
 *
 *   // 0 = not sqli
 *   // 1 = is sqli
 *
 *   // That's it!  sfilter s has some data on how it matched or not
 *   // details to come!
 *
 */

#ifndef _SQLPARSE_H
#define _SQLPARSE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Version info.
 * See python's normalized version
 * http://www.python.org/dev/peps/pep-0386/#normalizedversion
 */
#define LIBINJECTION_VERSION "1.1.0"

#define ST_MAX_SIZE 32
#define MAX_TOKENS 5

#define CHAR_NULL '\0'
#define CHAR_SINGLE '\''
#define CHAR_DOUBLE '"'

typedef struct {
    char type;
    char str_open;
    char str_close;
    char val[ST_MAX_SIZE];
} stoken_t;

typedef struct {
    /* input */
    const char *s;
    size_t slen;

    /* current tokenize state */
    size_t pos;
    int    in_comment;

    /* syntax fixups state */
    stoken_t syntax_current;
    stoken_t syntax_last;
    stoken_t syntax_comment;

    /* constant folding state */
    stoken_t fold_current;
    stoken_t fold_last;
    int fold_state;

    /* final sqli data */
    stoken_t tokenvec[MAX_TOKENS];

    /*  +1 for ending null */
    char pat[MAX_TOKENS + 1];
    char delim;
    int reason;
} sfilter;

/**
 * Pointer to function, takes cstr input, return true/false
 */
typedef int (*ptr_fingerprints_fn)(const char*);

/**
 * Main API: tests for SQLi in three possible contexts, no quotes,
 * single quote and double quote
 *
 * \return 1 (true) if SQLi, 0 (false) if benign
 */
int is_sqli(sfilter * sql_state, const char *s, size_t slen,
            ptr_fingerprints_fn fn);

/**
 * This detects SQLi in a single context, mostly useful for custom
 * logic and debugging.
 *
 * \param delim must be "NULL" (no context), single quote or double quote.
 *        Other values will likely be ignored.
 *
 * \return 1 (true) if SQLi, 0 (false) if not SQLi **in this context**
 *
 */
int is_string_sqli(sfilter * sql_state, const char *s, size_t slen,
                   const char delim,
                   ptr_fingerprints_fn fn);

#ifdef __cplusplus
}
#endif

#endif /* _SQLPARSE_H */
