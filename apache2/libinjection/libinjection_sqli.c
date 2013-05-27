/**
 * Copyright 2012,2013  Nick Galbreath
 * nickg@client9.com
 * BSD License -- see COPYING.txt for details
 *
 * (setq-default indent-tabs-mode nil)
 * (setq c-default-style "k&r"
 *     c-basic-offset 4)
 *  indent -kr -nut
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#if 0
#define FOLD_DEBUG printf("%d \t more=%d  pos=%d left=%d\n", __LINE__, more, (int)pos, (int)left);
#else
#define FOLD_DEBUG
#endif

#include "libinjection_sqli_data.h"

/* memchr2 finds a string of 2 characters inside another string
 * This a specialized version of "memmem" or "memchr".
 * 'memmem' doesn't exist on all platforms
 *
 * Porting notes: this is just a special version of
 *    astring.find("AB")
 *
 */
static const char *
memchr2(const char *haystack, size_t haystack_len, char c0, char c1)
{
    const char *cur = haystack;
    const char *last = haystack + haystack_len - 1;

    if (haystack_len < 2) {
        return NULL;
    }
    if (c0 == c1) {
        return NULL;
    }

    while (cur < last) {
        if (cur[0] == c0) {
            if (cur[1] == c1) {
                return cur;
            } else {
                cur += 2;
            }
        } else {
            cur += 1;
        }
    }

    return NULL;
}

/** Find largest string containing certain characters.
 *
 * C Standard library 'strspn' only works for 'c-strings' (null terminated)
 * This works on arbitrary length.
 *
 * Performance notes:
 *   not critical
 *
 * Porting notes:
 *   if accept is 'ABC', then this function would be similar to
 *   a_regexp.match(a_str, '[ABC]*'),
 */
static size_t
strlenspn(const char *s, size_t len, const char *accept)
{
    size_t i;
    for (i = 0; i < len; ++i) {
        /* likely we can do better by inlining this function
         * but this works for now
         */
        if (strchr(accept, s[i]) == NULL) {
            return i;
        }
    }
    return len;
}

static size_t
strlencspn(const char *s, size_t len, const char *accept)
{
    size_t i;
    for (i = 0; i < len; ++i) {
        /* likely we can do better by inlining this function
         * but this works for now
         */
        if (strchr(accept, s[i]) != NULL) {
            return i;
        }
    }
    return len;
}

/*
 * ASCII half-case-insenstive compare!
 *
 * DANGER: this assume arg0 is *always upper case*
 *  and arg1 is mixed case!!
 *
 * Required since libc version uses the current locale
 * and is much slower.
 */
static int cstrcasecmp(const char *a, const char *b)
{
    char ca, cb;

    do {
        ca = *a++;
        cb = *b++;
        assert(ca < 'a' || ca > 'z');
        if (cb >= 'a' && cb <= 'z')
            cb -= 0x20;
    } while (ca == cb && ca != '\0');

    return ca - cb;
}

/**
 * Case sensitive string compare.
 *  Here only to make code more readable
 */
static int streq(const char *a, const char *b)
{
    return strcmp(a, b) == 0;
}

/*
 * Case-sensitive binary search with "deferred detection of equality"
 * We assume in most cases the key will NOT be found.  This makes the
 * main loop only have one comparison branch, which should optimize
 * better in CPU.  See #Deferred_detection_of_equality in
 * http://en.wikipedia.org/wiki/Binary_search_algorithm
 *
 * This is used for fingerprint lookups, and a few other places.
 * Note in normal operation this maybe takes 1% of total run time, so
 * replacing this with another datastructure probably isn't worth
 * the effort.
 */
static int bsearch_cstr(const char *key, const char *base[], size_t nmemb)
{
    size_t pos;
    size_t left = 0;
    size_t right = nmemb - 1;

    /* assert(nmemb > 0); */

    while (left < right) {
        pos = (left + right) >> 1;
        /* assert(pos < right); */
        if (strcmp(base[pos], key) < 0) {
            left = pos + 1;
        } else {
            right = pos;
        }
    }
    if ((left == right) && strcmp(base[left], key) == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/*
 * Case-insensitive binary search
 *
 */
static int bsearch_cstrcase(const char *key, const char *base[], size_t nmemb)
{
    size_t pos;
    size_t left = 0;
    size_t right = nmemb - 1;

    while (left < right) {
        pos = (left + right) >> 1;
        /* arg0 = upper case only, arg1 = mixed case */
        if (cstrcasecmp(base[pos], key) < 0) {
            left = pos + 1;
        } else {
            right = pos;
        }
    }
    if ((left == right) && cstrcasecmp(base[left], key) == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 *
 */
#define UNUSED(x) (void)(x)

static int is_sqli_pattern(const char* key, void* callbackarg)
{
    UNUSED(callbackarg);
    return bsearch_cstr(key, sql_fingerprints, sqli_fingerprints_sz);
}

/**
 *
 *
 *
 * Porting Notes:
 *  given a mapping/hash of string to char
 *  this is just
 *    typecode = mapping[key.upper()]
 */

static char bsearch_keyword_type(const char *key, const keyword_t * keywords,
                                 size_t numb)
{
    size_t pos;
    size_t left = 0;
    size_t right = numb - 1;

    while (left < right) {
        pos = (left + right) >> 1;

        /* arg0 = upper case only, arg1 = mixed case */
        if (cstrcasecmp(keywords[pos].word, key) < 0) {
            left = pos + 1;
        } else {
            right = pos;
        }
    }
    if ((left == right) && cstrcasecmp(keywords[left].word, key) == 0) {
        return keywords[left].type;
    } else {
        return CHAR_NULL;
    }
}

static char is_keyword(const char* key)
{
    return bsearch_keyword_type(key, sql_keywords, sql_keywords_sz);
}

/* st_token methods
 *
 * The following functions manipulates the stoken_t type
 *
 *
 */

static void st_clear(stoken_t * st)
{
    st->type = CHAR_NULL;
    st->str_open = CHAR_NULL;
    st->str_close = CHAR_NULL;
    st->val[0] = CHAR_NULL;
}

static void st_assign_char(stoken_t * st, const char stype, const char value)
{
    st->type = stype;
    st->val[0] = value;
    st->val[1] = CHAR_NULL;
}

static void st_assign(stoken_t * st, const char stype, const char *value,
               size_t len)
{
    size_t last = len < ST_MAX_SIZE ? len : (ST_MAX_SIZE - 1);
    st->type = stype;
    memcpy(st->val, value, last);
    st->val[last] = CHAR_NULL;
}

static void st_copy(stoken_t * dest, const stoken_t * src)
{
    memcpy(dest, src, sizeof(stoken_t));
}

static int st_is_multiword_start(const stoken_t * st)
{
    return bsearch_cstrcase(st->val,
                            multikeywords_start,
                            multikeywords_start_sz);
}

static int st_is_unary_op(const stoken_t * st)
{
    return (st->type == 'o' && !(strcmp(st->val, "+") &&
                                 strcmp(st->val, "-") &&
                                 strcmp(st->val, "!") &&
                                 strcmp(st->val, "!!") &&
                                 /* arg0 = upper case only, arg1 = mixed case */
                                 cstrcasecmp("NOT", st->val) &&
                                 strcmp(st->val, "~")));
}

/* Parsers
 *
 *
 */

static size_t parse_white(sfilter * sf)
{
    return sf->pos + 1;
}

static size_t parse_operator1(sfilter * sf)
{
    const char *cs = sf->s;
    size_t pos = sf->pos;

    st_assign_char(sf->current, 'o', cs[pos]);
    return pos + 1;
}

static size_t parse_other(sfilter * sf)
{
    const char *cs = sf->s;
    size_t pos = sf->pos;

    st_assign_char(sf->current, '?', cs[pos]);
    return pos + 1;
}

static size_t parse_char(sfilter * sf)
{
    const char *cs = sf->s;
    size_t pos = sf->pos;

    st_assign_char(sf->current, cs[pos], cs[pos]);
    return pos + 1;
}

static size_t parse_eol_comment(sfilter * sf)
{
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;

    const char *endpos =
        (const char *) memchr((const void *) (cs + pos), '\n', slen - pos);
    if (endpos == NULL) {
        st_assign(sf->current, 'c', cs + pos, slen - pos);
        return slen;
    } else {
        st_assign(sf->current, 'c', cs + pos, endpos - cs - pos);
        return (endpos - cs) + 1;
    }
}

static size_t parse_dash(sfilter * sf)
{
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;


    size_t pos1 = pos + 1;
    if (pos1 < slen && cs[pos1] == '-') {
        return parse_eol_comment(sf);
    } else {
        st_assign_char(sf->current, 'o', '-');
        return pos1;
    }
}

static size_t is_mysql_comment(const char *cs, const size_t len, size_t pos)
{
    size_t i;

    if (pos + 2 >= len) {
        return 0;
    }
    if (cs[pos + 2] != '!') {
        return 0;
    }
    /*
     * this is a mysql comment
     *  got "/x!"
     */
    if (pos + 3 >= len) {
        return 3;
    }

    if (!isdigit(cs[pos + 3])) {
        return 3;
    }
    /*
     * handle odd case of /x!0SELECT
     */
    if (!isdigit(cs[pos + 4])) {
        return 4;
    }

    if (pos + 7 >= len) {
        return 4;
    }

    for (i = pos + 5; i <= pos + 7; ++i) {
        if (!isdigit(cs[i])) {
            return 3;
        }
    }
    return 8;
}

static size_t parse_slash(sfilter * sf)
{
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;
    const char* cur = cs + pos;
    size_t inc;

    size_t pos1 = pos + 1;
    if (pos1 == slen || cs[pos1] != '*') {
        return parse_operator1(sf);
    }

    inc = is_mysql_comment(cs, slen, pos);
    if (inc == 0) {

        /*
         * skip over initial '/x'
         */
        const char *ptr = memchr2(cur + 2, slen - (pos + 2), '*', '/');
        if (ptr == NULL) {
            /*
             * unterminated comment
             */
            st_assign(sf->current, 'c', cs + pos, slen - pos);
            return slen;
        } else {
            /*
             * postgresql allows nested comments which makes
             * this is incompatible with parsing so
             * if we find a '/x' inside the coment, then
             * make a new token.
             */
            char ctype = 'c';
            const size_t clen = (ptr + 2) - (cur);
            if (memchr2(cur + 2, ptr - (cur + 1), '/', '*') !=  NULL) {
                ctype = 'X';
            }
            st_assign(sf->current, ctype, cs + pos, clen);

            return pos + clen;
        }
    } else {
        /*
         * MySQL Comment
         */
        sf->in_comment = TRUE;
        st_clear(sf->current);
        return pos + inc;
    }
}

static size_t parse_backslash(sfilter * sf)
{
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;

    /*
     * Weird MySQL alias for NULL, "\N" (capital N only)
     */
    if (pos + 1 < slen && cs[pos + 1] == 'N') {
        st_assign(sf->current, '1', "NULL", 4);
        return pos + 2;
    } else {
        return parse_other(sf);
    }
}

/** Is input a 2-char operator?
 *
 */
static int is_operator2(const char *key)
{
    return bsearch_cstr(key, operators2, operators2_sz);
}

static size_t parse_operator2(sfilter * sf)
{
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;
    char op2[3];

    if (pos + 1 >= slen) {
        return parse_operator1(sf);
    }

    op2[0] = cs[pos];
    op2[1] = cs[pos + 1];
    op2[2] = CHAR_NULL;

    /*
     * Special Hack for MYSQL style comments
     *  instead of turning:
     * /x! FOO x/  into FOO by rewriting the string, we
     * turn it into FOO x/ and ignore the ending comment
     */
    if (sf->in_comment && op2[0] == '*' && op2[1] == '/') {
        sf->in_comment = FALSE;
        st_clear(sf->current);
        return pos + 2;
    } else if (pos + 2 < slen && op2[0] == '<' && op2[1] == '='
               && cs[pos + 2] == '>') {
        /*
         * special 3-char operator
         */
        st_assign(sf->current, 'o', "<=>", 3);
        return pos + 3;
    } else if (is_operator2(op2)) {
        if (streq(op2, "&&") || streq(op2, "||")) {
            st_assign(sf->current, '&', op2, 2);
        } else {
            /*
             * normal 2 char operator
             */
            st_assign(sf->current, 'o', op2, 2);
        }
        return pos + 2;
    } else if (op2[0] == ':') {
        /* ':' is not an operator */
        st_assign_char(sf->current, ':', ':');
        return pos + 1;
    } else {
        /*
         * must be a single char operator
         */
        return parse_operator1(sf);
    }
}

static size_t parse_string_core(const char *cs, const size_t len, size_t pos,
                                stoken_t * st, char delim, size_t offset)
{
    /*
     * offset is to skip the perhaps first quote char
     */
    const char *qpos =
        (const char *) memchr((const void *) (cs + pos + offset), delim,
                              len - pos - offset);

    /*
     * then keep string open/close info
     */
    if (offset == 1) {
        /*
         * this is real quote
         */
        st->str_open = delim;
    } else {
        /*
         * this was a simulated quote
         */
        st->str_open = CHAR_NULL;
    }

    while (TRUE) {
        if (qpos == NULL) {
            /*
             * string ended with no trailing quote
             * assign what we have
             */
            st_assign(st, 's', cs + pos + offset, len - pos - offset);
            st->str_close = CHAR_NULL;
            return len;
        } else if (*(qpos - 1) != '\\') {
            /*
             * ending quote is not escaped.. copy and end
             */
            st_assign(st, 's', cs + pos + offset,
                      qpos - (cs + pos + offset));
            st->str_close = delim;
            return qpos - cs + 1;
        } else {
            qpos =
                (const char *) memchr((const void *) (qpos + 1), delim,
                                      (cs + len) - (qpos + 1));
        }
    }
}

/**
 * Used when first char is a ' or "
 */
static size_t parse_string(sfilter * sf)
{
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;

    /*
     * assert cs[pos] == single or double quote
     */
    return parse_string_core(cs, slen, pos, sf->current, cs[pos], 1);
}

/** Parse MySQL `backtick` quoted strings
 *
 * Unforunately, the escaping rules for `-quoted strings is different
 * when finding a "`" you need to look at NEXT CHAR to see if it's "`"
 * If so, you need to jump two characters ahead
 *
 * In normal strings, you need to look at PREVIOUS CHAR... and if so
 * just jump ahead one char.
 *
 * Also we don't need to code to fake an opening "`"
 *
 * Tried to keep code as similar to parse_string_core.
 */
static size_t parse_string_tick(sfilter *sf)
{
    const size_t offset = 1;
    const char delim = '`';

    const char *cs = sf->s;
    const size_t len = sf->slen;
    size_t pos = sf->pos;
    stoken_t *st = sf->current;

    /*
     * len -pos -1 : offset is to skip the perhaps first quote char
     */
    const char *qpos =
        (const char *) memchr((const void *) (cs + pos + offset), delim,
                              len - pos - offset);

    st->str_open = delim;

    while (TRUE) {
        if (qpos == NULL) {
            /*
             * string ended with no trailing quote
             * assign what we have
             */
            st_assign(st, 's', cs + pos + offset, len - pos - offset);
            st->str_close = CHAR_NULL;
            return len;
        } else if (qpos + 1 == (cs + len) || ((qpos + 1) < (cs + len) && *(qpos + 1) != delim)) {
            /*
             * ending quote is not escaped.. copy and end
             */
            st_assign(st, 's', cs + pos + offset,
                      qpos - (cs + pos + offset));
            st->str_close = delim;
            return qpos - cs + 1;
        } else {
            /*
             * we got a `` so advance by 2 chars
             */
            qpos =
                (const char *) memchr((const void *) (qpos + 2), delim,
                                      (cs + len) - (qpos + 2));
        }
    }
}

static size_t parse_word(sfilter * sf)
{
    const char *cs = sf->s;
    size_t pos = sf->pos;
    char *dot;
    char ch;
    size_t slen =
        strlencspn(cs + pos, sf->slen - pos,
                   " .`<>:\\?=@!#~+-*/&|^%(),';\r\n\t\"\013\014");

    st_assign(sf->current, 'n', cs + pos, slen);

    dot = strchr(sf->current->val, '.');
    if (dot != NULL) {
        *dot = '\0';

        ch = is_keyword(sf->current->val);

        if (ch == 'k' || ch == 'o' || ch == 'E') {
            /*
             * we got something like "SELECT.1"
             */
            sf->current->type = ch;
            return pos + strlen(sf->current->val);
        } else {
            /*
             * something else, put back dot
             */
            *dot = '.';
        }
    }

    /*
     * do normal lookup with word including '.'
     */
    if (slen < ST_MAX_SIZE) {

        ch = is_keyword(sf->current->val);

        if (ch == CHAR_NULL) {
            ch = 'n';
        }
        sf->current->type = ch;
    }
    return pos + slen;
}

/* MySQL backticks are a cross between string and
 * and a bare word.
 *
 */
static size_t parse_tick(sfilter* sf)
{
    /* first we pretend we are looking for a string */
    size_t slen = parse_string_tick(sf);

    /* we could check to see if start and end of
     * of string are both "`", i.e. make sure we have
     * matching set.  `foo` vs. `foo
     * but I don't think it matters much
     */


    /* check value of string to see if it's a keyword,
     * function, operator, etc
     */
    char ch = is_keyword(sf->current->val);
    if (ch == 'f') {
        /* if it's a function, then convert token */
        sf->current->type = 'f';
    } else {
        /* otherwise it's a 'n' type -- mysql treats
         * everything as a bare word
         */
        sf->current->type = 'n';
    }
    return slen;
}

static size_t parse_var(sfilter * sf)
{
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos + 1;
    size_t xlen;

    /*
     * var_count is only used to reconstruct
     * the input.  It counts the number of '@'
     * seen 0 in the case of NULL, 1 or 2
     */

    /*
     * move past optional other '@'
     */
    if (pos < slen && cs[pos] == '@') {
        pos += 1;
        sf->current->var_count = 2;
    } else {
        sf->current->var_count = 1;
    }

    /*
     * MySQL allows @@`version`
     */
    if (pos < slen) {
        if (cs[pos] == '`') {
            sf->pos = pos;
            pos = parse_string_tick(sf);
            sf->current->type = 'v';
            return pos;
        } else if (cs[pos] == CHAR_SINGLE || cs[pos] == CHAR_DOUBLE) {
            sf->pos = pos;
            pos = parse_string(sf);
            sf->current->type = 'v';
            return pos;
        }
    }


    xlen = strlencspn(cs + pos, slen - pos,
                     " <>:\\?=@!#~+-*/&|^%(),';\r\n\t\"\013\014");
//                     "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.$");
    if (xlen == 0) {
        st_assign(sf->current, 'v', cs + pos, 0);
        return pos;
    } else {
        st_assign(sf->current, 'v', cs + pos, xlen);
        return pos + xlen;
    }
}

static size_t parse_money(sfilter *sf)
{
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;
    size_t xlen;

    /*
     * $1,000.00 or $1.000,00 ok!
     * This also parses $....,,,111 but that's ok
     */
    xlen = strlenspn(cs + pos + 1, slen - pos - 1, "0123456789.,");
    if (xlen == 0) {
        /*
         * just ignore '$'
         */
        return pos + 1;
    } else {
        st_assign(sf->current, '1', cs + pos, 1 + xlen);
        return pos + 1 + xlen;
    }
}

static size_t parse_number(sfilter * sf)
{
    const char *cs = sf->s;
    const size_t slen = sf->slen;
    size_t pos = sf->pos;
    size_t xlen;
    size_t start;

    if (pos + 1 < slen && cs[pos] == '0' && (cs[pos + 1] == 'X' || cs[pos + 1] == 'x')) {
        /*
         * TBD compare if isxdigit
         */
        xlen =
            strlenspn(cs + pos + 2, slen - pos - 2, "0123456789ABCDEFabcdef");
        if (xlen == 0) {
            st_assign(sf->current, 'n', "0X", 2);
            return pos + 2;
        } else {
            st_assign(sf->current, '1', cs + pos, 2 + xlen);
            return pos + 2 + xlen;
        }
    }

    start = pos;
    while (pos < slen && isdigit(cs[pos])) {
        pos += 1;
    }
    if (pos < slen && cs[pos] == '.') {
        pos += 1;
        while (pos < slen && isdigit(cs[pos])) {
            pos += 1;
        }
        if (pos - start == 1) {
            st_assign_char(sf->current, 'n', '.');
            return pos;
        }
    }

    if (pos < slen) {
        if (cs[pos] == 'E' || cs[pos] == 'e') {
            pos += 1;
            if (pos < slen && (cs[pos] == '+' || cs[pos] == '-')) {
                pos += 1;
            }
            while (pos < slen && isdigit(cs[pos])) {
                pos += 1;
            }
        } else if (isalpha(cs[pos])) {
            /*
             * oh no, we have something like '6FOO'
             * use microsoft style parsing and take just
             * the number part and leave the rest to be
             * parsed later
             */
            st_assign(sf->current, '1', cs + start, pos - start);
            return pos;
        }
    }

    st_assign(sf->current, '1', cs + start, pos - start);
    return pos;
}

int libinjection_sqli_tokenize(sfilter * sf, stoken_t *current)
{
    const char *s = sf->s;
    const size_t slen = sf->slen;
    size_t *pos = &sf->pos;
    pt2Function fnptr;

    st_clear(current);
    sf->current = current;

    /*
     * if we are at beginning of string
     *  and in single-quote or double quote mode
     *  then pretend the input starts with a quote
     */
    if (*pos == 0 && sf->delim != CHAR_NULL) {
        *pos = parse_string_core(s, slen, 0, current, sf->delim, 0);
        return TRUE;
    }

    while (*pos < slen) {
        /*
         * get current character
         */
        const unsigned ch = (unsigned int) (s[*pos]);

        /*
         * if not ascii, then continue...
         *   actually probably need to just assuming
         *   it's a string
         */
        if (ch > 127) {
            fnptr = parse_word;
        } else {

        /*
         * look up the parser, and call it
         *
         * Porting Note: this is mapping of char to function
         *   charparsers[ch]()
         */
        fnptr = char_parse_map[ch];
        }
        *pos = (*fnptr) (sf);

        /*
         *
         */
        if (current->type != CHAR_NULL) {
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Initializes parsing state
 *
 */
void libinjection_sqli_init(sfilter * sf, const char *s, size_t len, char delim)
{
    memset(sf, 0, sizeof(sfilter));
    sf->s = s;
    sf->slen = len;
    sf->delim = delim;
}

/** See if two tokens can be merged since they are compound SQL phrases.
 *
 * This takes two tokens, and, if they are the right type,
 * merges their values together.  Then checks to see if the
 * new value is special using the PHRASES mapping.
 *
 * Example: "UNION" + "ALL" ==> "UNION ALL"
 *
 * C Security Notes: this is safe to use C-strings (null-terminated)
 *  since the types involved by definition do not have embedded nulls
 *  (e.g. there is no keyword with embedded null)
 *
 * Porting Notes: since this is C, it's oddly complicated.
 *  This is just:  multikeywords[token.value + ' ' + token2.value]
 *
 */
static int syntax_merge_words(stoken_t * a, stoken_t * b)
{
    size_t sz1;
    size_t sz2;
    size_t sz3;
    char tmp[ST_MAX_SIZE];
    char ch;

    if (!
        (a->type == 'k' || a->type == 'n' || a->type == 'o'
         || a->type == 'U' || a->type == 'E')) {
        return FALSE;
    }

    if (! st_is_multiword_start(a)) {
        return FALSE;
    }

    sz1 = strlen(a->val);
    sz2 = strlen(b->val);
    sz3 = sz1 + sz2 + 1; /* +1 for space in the middle */
    if (sz3 >= ST_MAX_SIZE) { /* make sure there is room for ending null */
        return FALSE;
    }
    /*
     * oddly annoying  last.val + ' ' + current.val
     */
    memcpy(tmp, a->val, sz1);
    tmp[sz1] = ' ';
    memcpy(tmp + sz1 + 1, b->val, sz2);
    tmp[sz3] = CHAR_NULL;

    ch = bsearch_keyword_type(tmp, multikeywords, multikeywords_sz);
    if (ch != CHAR_NULL) {
        st_assign(a, ch, tmp, sz3);
        return TRUE;
    } else {
        return FALSE;
    }
}


/*
 * My apologies, this code is a mess
 */
int filter_fold(sfilter * sf)
{
    stoken_t last_comment;

    stoken_t * current;

    /* POS is the positive of where the NEXT token goes */
    size_t pos = 0;

    /* LEFT is a count of how many tokens that are already
       folded or processed (i.e. part of the fingerprint) */
    size_t left =  0;

    int more = 1;

    st_clear(&last_comment);

    /* Skip all initial comments, right-parens ( and unary operators
     *
     */
    current = &(sf->tokenvec[0]);
    while (more) {
        more = libinjection_sqli_tokenize(sf, current);
        if ( ! (current->type == 'c' || current->type == '(' || st_is_unary_op(current))) {
            break;
        }
    }

    if (! more) {
        /* If input was only comments, unary or (, then exit */
        return 0;
    } else {
        /* it's some other token */
        pos += 1;
    }

    while (1) {
        FOLD_DEBUG
        /* get up to two tokens */
        while (more && pos <= MAX_TOKENS && (pos - left) < 2) {
            current = &(sf->tokenvec[pos]);
            more = libinjection_sqli_tokenize(sf, current);
            if (more) {
                if (current->type == 'c') {
                    st_copy(&last_comment, current);
                } else {
                    last_comment.type = CHAR_NULL;
                    pos += 1;
                }
            }
        }
        FOLD_DEBUG
        /* did we get 2 tokens? if not then we are done */
        if (pos - left != 2) {
            left = pos;
            break;
        }

        /* FOLD: "ss" -> "s"
         * "foo" "bar" is valid SQL
         * just ignore second string
         */
        if (sf->tokenvec[left].type == 's' && sf->tokenvec[left+1].type == 's') {
            pos -= 1;
            continue;
        } else if (sf->tokenvec[left].type =='o' && st_is_unary_op(&sf->tokenvec[left+1])) {
            pos -= 1;
            if (left > 0) {
                left -= 1;
            }
            continue;
        } else if (sf->tokenvec[left].type =='(' && st_is_unary_op(&sf->tokenvec[left+1])) {
            pos -= 1;
            if (left > 0) {
                left -= 1;
            }
            continue;
        } else if (syntax_merge_words(&sf->tokenvec[left], &sf->tokenvec[left+1])) {
            pos -= 1;
            continue;
        } else if (sf->tokenvec[left].type == 'n' &&
                   sf->tokenvec[left+1].type == '(' && (
                       cstrcasecmp("IN", sf->tokenvec[left].val) == 0 ||
                       cstrcasecmp("DATABASE", sf->tokenvec[left].val) == 0 ||
                       cstrcasecmp("USER", sf->tokenvec[left].val) == 0 ||
                       cstrcasecmp("PASSWORD", sf->tokenvec[left].val) == 0
                       )
            ) {

            // pos is the same
            // other conversions need to go here... for instance
            // password CAN be a function, coalese CAN be a function
            sf->tokenvec[left].type = 'f';
            continue;
#if 0
        } else if (sf->tokenvec[left].type == 'o' && cstrcasecmp("LIKE", sf->tokenvec[left].val) == 0
                   && sf->tokenvec[left+1].type == '(') {
            // two use cases   "foo" LIKE "BAR" (normal operator)
            // "foo" = LIKE(1,2)
            sf->tokenvec[left].type = 'f';
            continue;
#endif
        }

        /* all cases of handing 2 tokens is done
           and nothing matched.  Get one more token
        */
        FOLD_DEBUG
        while (more && pos <= MAX_TOKENS && pos - left < 3) {
            current = &(sf->tokenvec[pos]);
            more = libinjection_sqli_tokenize(sf, current);
            if (more) {
                if (current->type == 'c') {
                    st_copy(&last_comment, current);
                } else {
                    last_comment.type = CHAR_NULL;
                    pos += 1;
                }
            }
        }

        /* do we have three tokens? If not then we are done */
        if (pos -left != 3) {
            left = pos;
            break;
        }

        /*
         * now look for three token folding
         */
        if (sf->tokenvec[left].type == '1' &&
            sf->tokenvec[left+1].type == 'o' &&
            sf->tokenvec[left+2].type == '1') {
            pos -= 2;
            continue;
        } else if (sf->tokenvec[left].type == 'o' &&
                   sf->tokenvec[left+1].type != '(' &&
                   sf->tokenvec[left+2].type == 'o') {
            if (left > 0) {
                left -= 1;
            }
            pos -= 2;
            continue;
        } else if (sf->tokenvec[left].type == '&' &&
                   sf->tokenvec[left+2].type == '&') {
            pos -= 2;
            continue;
        } else if ((sf->tokenvec[left].type == 'n' || sf->tokenvec[left].type == '1' ) &&
                   sf->tokenvec[left+1].type == 'o' &&
                   (sf->tokenvec[left+2].type == '1' || sf->tokenvec[left+2].type == 'n')) {
            pos -= 2;
            continue;
#if 0
        } else if ((sf->tokenvec[left].type == 'n' || sf->tokenvec[left].type == '1') &&
                   sf->tokenvec[left+1].type == ',' &&
                   (sf->tokenvec[left+2].type == '1' || sf->tokenvec[left+2].type == 'n')) {
            pos -= 2;
            continue;
#endif
        } else if ((sf->tokenvec[left].type == 'k' || sf->tokenvec[left].type == 'E') &&
                   st_is_unary_op(&sf->tokenvec[left+1]) &&
                   (sf->tokenvec[left+2].type == '1' || sf->tokenvec[left+2].type == 'n' || sf->tokenvec[left+2].type == 'v' || sf->tokenvec[left+2].type == 's' || sf->tokenvec[left+2].type == 'f' )) {
            // remove unary operators
            // select - 1
            st_copy(&sf->tokenvec[left+1], &sf->tokenvec[left+2]);
            pos -= 1;
        } else if (sf->tokenvec[left].type == 'n' &&
                   sf->tokenvec[left+1].type == 'n'  && sf->tokenvec[left+1].val[0] == '.' &&
                   sf->tokenvec[left+2].type == 'n') {
            /* ignore the '.n'
             * typically is this dabasename.table
             */
            pos -= 2;
            continue;
        }


        /* no folding -- assume left-most token is
           is good, now use the existing 2 tokens --
           do not get another
        */

        left += 1;

    } /* while(1) */

    /* if we have 4 or less tokens, and we had a comment token
     * at the end, add it back
     */

    if (left < MAX_TOKENS && last_comment.type == 'c') {
        st_copy(&sf->tokenvec[left], &last_comment);
        left += 1;
    }

    /* sometimes we grab a 6th token to help
       determine the type of token 5.
    */
    if (left > MAX_TOKENS) {
        left = MAX_TOKENS;
    }

    return (int)left;
}

/* secondary api: detects SQLi in a string, GIVEN a context.
 *
 * A context can be:
 *   *  CHAR_NULL (\0), process as is
 *   *  CHAR_SINGLE ('), process pretending input started with a
 *          single quote.
 *   *  CHAR_DOUBLE ("), process pretending input started with a
 *          double quote.
 *
 */
int libinjection_is_string_sqli(sfilter * sql_state,
                                const char *s, size_t slen,
                                const char delim,
                                ptr_fingerprints_fn fn, void* callbackarg)
{
    int i;
    int tlen = 0;
    char ch;
    int patmatch;

    libinjection_sqli_init(sql_state, s, slen, delim);

    tlen = filter_fold(sql_state);
    for (i = 0; i < tlen; ++i) {
        sql_state->pat[i] = sql_state->tokenvec[i].type;
    }

    /*
     * make the fingerprint pattern a c-string (null delimited)
     */
    sql_state->pat[tlen] = CHAR_NULL;

    /*
     * check for 'X' in pattern
     * this means parsing could not be done
     * accurately due to pgsql's double comments
     * or other syntax that isn't consistent
     * should be very rare false positive
     */
    if (strchr(sql_state->pat, 'X')) {
        return TRUE;
    }

    patmatch = fn(sql_state->pat, callbackarg);

    /*
     * No match.
     *
     * Set sql_state->reason to current line number
     * only for debugging purposes.
     */
    if (!patmatch) {
        sql_state->reason = __LINE__;
        return FALSE;
    }

    /*
     * We got a SQLi match
     * This next part just helps reduce false positives.
     *
     */
    switch (tlen) {
    case 2:{
        /*
         * case 2 are "very small SQLi" which make them
         * hard to tell from normal input...
         */

        /*
         * if 'comment' is '#' ignore.. too many FP
         */
        if (sql_state->tokenvec[1].val[0] == '#') {
            sql_state->reason = __LINE__;
            return FALSE;
        }

        /*
         * for fingerprint like 'nc', only comments of /x are treated
         * as SQL... ending comments of "--" and "#" are not sqli
         */
        if (sql_state->tokenvec[0].type == 'n' &&
            sql_state->tokenvec[1].type == 'c' &&
            sql_state->tokenvec[1].val[0] != '/') {
                sql_state->reason = __LINE__;
                return FALSE;
        }

        /*
         * if '1c' ends with '/x' then it's sqli
         */
        if (sql_state->tokenvec[0].type == '1' &&
            sql_state->tokenvec[1].type == 'c' &&
            sql_state->tokenvec[1].val[0] == '/') {
            return TRUE;
        }

        /*
         * if 'oc' then input must be 'CASE/x'
         * used in HPP attack
         */
        if (sql_state->tokenvec[0].type == 'o' &&
            sql_state->tokenvec[1].type == 'c' &&
            sql_state->tokenvec[1].val[0] == '/' &&
            cstrcasecmp("CASE", sql_state->tokenvec[0].val) != 0)
        {
            sql_state->reason = __LINE__;
            return FALSE;
        }

        /**
         * there are some odd base64-looking query string values
         * 1234-ABCDEFEhfhihwuefi--
         * which evaluate to "1c"... these are not SQLi
         * but 1234-- probably is.
         * Make sure the "1" in "1c" is actually a true decimal number
         *
         * Need to check -original- string since the folding step
         * may have merged tokens, e.g. "1+FOO" is folded into "1"
         *
         * Note: evasion: 1*1--
         */
        if (sql_state->tokenvec[0].type == '1'&& sql_state->tokenvec[1].type == 'c') {
            /*
             * we check that next character after the number is either whitespace,
             * or '/' or a '-' ==> sqli.
             */
            ch = sql_state->s[strlen(sql_state->tokenvec[0].val)];
            if ( ch <= 32 ) {
                /* next char was whitespace,e.g. "1234 --"
                 * this isn't exactly correct.. ideally we should skip over all whitespace
                 * but this seems to be ok for now
                 */
                return TRUE;
            }
            if (ch == '/' && sql_state->s[strlen(sql_state->tokenvec[0].val) + 1] == '*') {
                return TRUE;
            }
            if (ch == '-' && sql_state->s[strlen(sql_state->tokenvec[0].val) + 1] == '-') {
                return TRUE;
            }

            sql_state->reason = __LINE__;
            return FALSE;
        }

        /*
         * detect obvious sqli scans.. many people put '--' in plain text
         * so only detect if input ends with '--', e.g. 1-- but not 1-- foo
         */
        if ((strlen(sql_state->tokenvec[1].val) > 2)
            && sql_state->tokenvec[1].val[0] == '-') {
            sql_state->reason = __LINE__;
            return FALSE;
        }

        break;
    } /* case 2 */
    case 3:{
        /*
         * ...foo' + 'bar...
         * no opening quote, no closing quote
         * and each string has data
         */
        if (streq(sql_state->pat, "sos")
            || streq(sql_state->pat, "s&s")) {
                if ((sql_state->tokenvec[0].str_open == CHAR_NULL)
                    && (sql_state->tokenvec[2].str_close == CHAR_NULL)
                    && (sql_state->tokenvec[0].str_close == sql_state->tokenvec[2].str_open)) {
                    /*
                     * if ....foo" + "bar....
                     */
                    return TRUE;
                } else {
                    /*
                     * not sqli
                     */
                    sql_state->reason = __LINE__;
                    return FALSE;
                }
        } else if (streq(sql_state->pat, "so1")) {
            if (sql_state->tokenvec[0].str_open != CHAR_NULL) {
                /* "foo" -1 is ok, foo"-1 is not */
                sql_state->reason = __LINE__;
                return FALSE;
            }
        } else if ((sql_state->tokenvec[1].type == 'k') && cstrcasecmp("INTO OUTFILE", sql_state->tokenvec[1].val)) {
            sql_state->reason = __LINE__;
            return FALSE;
        }
        break;
    }  /* case 3 */
    case 5: {
        /* nothing right now */
        break;
    } /* case 5 */
    } /* end switch */

    return TRUE;
}

/**  Main API, detects SQLi in an input.
 *
 *
 */
int libinjection_is_sqli(sfilter * sql_state, const char *s, size_t slen,
                         ptr_fingerprints_fn fn, void* callbackarg)
{

    /*
     * no input? not sqli
     */
    if (slen == 0) {
        return FALSE;
    }

    if (fn == NULL) {
        fn = is_sqli_pattern;
    }

    /*
     * test input "as-is"
     */
    if (libinjection_is_string_sqli(sql_state, s, slen, CHAR_NULL,
                                    fn, callbackarg)) {
        return TRUE;
    }

    /*
     * if input has a single_quote, then
     * test as if input was actually '
     * example: if input if "1' = 1", then pretend it's
     *   "'1' = 1"
     * Porting Notes: example the same as doing
     *   is_string_sqli(sql_state, "'" + s, slen+1, NULL, fn, arg)
     *
     */
    if (memchr(s, CHAR_SINGLE, slen)
        && libinjection_is_string_sqli(sql_state, s, slen, CHAR_SINGLE,
                                       fn, callbackarg)) {
        return TRUE;
    }

    /*
     * same as above but with a double-quote "
     */
    if (memchr(s, CHAR_DOUBLE, slen)
        && libinjection_is_string_sqli(sql_state, s, slen, CHAR_DOUBLE,
                                       fn, callbackarg)) {
        return TRUE;
    }

    /*
     * Hurray, input is not SQLi
     */
    return FALSE;
}
