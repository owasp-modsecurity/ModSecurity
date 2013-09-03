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
 * Version info.
 * See python's normalized version
 * http://www.python.org/dev/peps/pep-0386/#normalizedversion
 */
#define LIBINJECTION_VERSION "3.5.3"

/**
 * Libinjection's sqli module makes a "normalized"
 * value of the token.  This is the maximum size
 * Token with values larger than this will be truncated
 */
#ifndef LIBINJECTION_SQLI_TOKEN_SIZE
#define LIBINJECTION_SQLI_TOKEN_SIZE 32
#endif

/**
 * Number of tokens used to create a fingerprint
 */
#ifndef LIBINJECTION_SQLI_MAX_TOKENS
#define LIBINJECTION_SQLI_MAX_TOKENS 5
#endif

#if LIBINJECTION_SQLI_MAX_TOKENS >= 8
#define LIBINJECTION_SQLI_BUFFER_SZ (LIBINJECTION_SQLI_MAX_TOKENS + 1)
#else
#define LIBINJECTION_SQLI_BUFFER_SZ 8
#endif


enum lookup_type {
    FLAG_NONE          = 0,
    FLAG_QUOTE_NONE    = 1 << 1,
    FLAG_QUOTE_SINGLE  = 1 << 2,
    FLAG_QUOTE_DOUBLE  = 1 << 3,

    FLAG_SQL_ANSI      = 1 << 4,
    FLAG_SQL_MYSQL     = 1 << 5,

    LOOKUP_WORD,
    LOOKUP_TYPE,
    LOOKUP_OPERATOR,
    LOOKUP_FINGERPRINT
};

typedef struct {
#ifdef SWIG
%immutable;
#endif
    char type;
    char str_open;
    char str_close;

    /*
     * position and length of token
     * in original string
     */
    size_t pos;
    size_t len;

    /*  count:
     *  in type 'v', used for number of opening '@'
     *  but maybe unsed in other contexts
     */
    int  count;

    char val[LIBINJECTION_SQLI_TOKEN_SIZE];
}  stoken_t;


/**
 * Pointer to function, takes cstr input,
 *  returns '\0' for no match, else a char
 */
struct libinjection_sqli_state;
typedef char (*ptr_lookup_fn)(struct libinjection_sqli_state*, int lookuptype, const char* word, size_t len);

typedef struct libinjection_sqli_state {
#ifdef SWIG
%immutable;
#endif

    /*
     * input, does not need to be null terminated.
     * it is also not modified.
     */
    const char *s;

    /*
     * input length
     */
    size_t slen;

    /*
     * How to lookup a word or fingerprint
     */
    ptr_lookup_fn lookup;
    void*         userdata;

    /*
     *
     */
    int flags;

    /*
     * pos is index in string we are at when tokenizing
     */
    size_t pos;

    /* MAX TOKENS + 1 since we use one extra token
     * to determine the type of the previous token
     */
    stoken_t tokenvec[LIBINJECTION_SQLI_BUFFER_SZ];

    /*
     * Pointer to token position in tokenvec, above
     */
    stoken_t *current;

    /*
     * fingerprint pattern c-string
     * +1 for ending null
     * Mimimum of 8 bytes to add gcc's -fstack-protector to work
     */
    char fingerprint[LIBINJECTION_SQLI_BUFFER_SZ];

    /*
     * Line number of code that said decided if the input was SQLi or
     * not.  Most of the time it's line that said "it's not a matching
     * fingerprint" but there is other logic that sometimes approves
     * an input. This is only useful for debugging.
     *
     */
    int reason;

    /* Number of ddw (dash-dash-white) comments
     * These comments are in the form of
     *   '--[whitespace]' or '--[EOF]'
     *
     * All databases treat this as a comment.
     */
     int stats_comment_ddw;

    /* Number of ddx (dash-dash-[notwhite]) comments
     *
     * ANSI SQL treats these are comments, MySQL treats this as
     * two unary operators '-' '-'
     *
     * If you are parsing result returns FALSE and
     * stats_comment_dd > 0, you should reparse with
     * COMMENT_MYSQL
     *
     */
    int stats_comment_ddx;

    /*
     * c-style comments found  /x .. x/
     */
    int stats_comment_c;

    /* '#' operators or mysql EOL comments found
     *
     */
    int stats_comment_hash;

    /*
     * number of tokens folded away
     */
    int stats_folds;

    /*
     * total tokens processed
     */
    int stats_tokens;

} sfilter;

/**
 *
 */
void libinjection_sqli_init(sfilter* sql_state,
                            const char* s, size_t slen,
                            int flags);

/**
 * Main API: tests for SQLi in three possible contexts, no quotes,
 * single quote and double quote
 *
 * \param sql_state
 * \param s
 * \param slen
 * \param fn a pointer to a function that determines if a fingerprint
 *        is a match or not.  If NULL, then a hardwired list is
 *        used. Useful for loading fingerprints data from custom
 *        sources.
 *
 * \return 1 (true) if SQLi, 0 (false) if benign
 */
int libinjection_is_sqli(sfilter * sql_state);

/*  FOR H@CKERS ONLY
 *
 */
void libinjection_sqli_callback(sfilter* sql_state, ptr_lookup_fn fn, void* userdata);


/*
 * Resets state, but keeps initial string and callbacks
 */
void libinjection_sqli_reset(sfilter* sql_state, int flags);

/**
 *
 */

/**
 * This detects SQLi in a single context, mostly useful for custom
 * logic and debugging.
 *
 * \param sql_state
 *
 * \returns a pointer to sfilter.fingerprint as convenience
 *          do not free!
 *
 */
const char* libinjection_sqli_fingerprint(sfilter * sql_state, int flags);

/**
 * The default "word" to token-type or fingerprint function.  This
 * uses a ASCII case-insensitive binary tree.
 */
char libinjection_sqli_lookup_word(sfilter *sql_state, int lookup_type,
                                   const char* s, size_t slen);

/* Streaming tokenization interface.
 *
 * sql_state->current is updated with the current token.
 *
 * \returns 1, has a token, keep going, or 0 no tokens
 *
 */
int  libinjection_sqli_tokenize(sfilter * sql_state);

/**
 * parses and folds input, up to 5 tokens
 *
 */
int libinjection_sqli_fold(sfilter * sql_state);

/** The built-in default function to match fingerprints
 *  and do false negative/positive analysis.  This calls the following
 *  two functions.  With this, you over-ride one part or the other.
 *
 *     return libinjection_sqli_blacklist(sql_state) &&
 *        libinject_sqli_not_whitelist(sql_state);
 *
 * \param sql_state should be filled out after libinjection_sqli_fingerprint is called
 */
int libinjection_sqli_check_fingerprint(sfilter *sql_state);

/* Given a pattern determine if it's a SQLi pattern.
 *
 * \return TRUE if sqli, false otherwise
 */
int libinjection_sqli_blacklist(sfilter* sql_state);

/* Given a positive match for a pattern (i.e. pattern is SQLi), this function
 * does additional analysis to reduce false positives.
 *
 * \return TRUE if sqli, false otherwise
 */
int libinjection_sqli_not_whitelist(sfilter* sql_state);

#ifdef __cplusplus
}
#endif

#endif /* _LIBINJECTION_H */
