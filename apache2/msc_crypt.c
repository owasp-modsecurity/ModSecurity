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

#include "msc_crypt.h"
#include "msc_util.h"
#include "apr_sha1.h"
#include "apr_uri.h"
#include "apr_base64.h"
#include "acmp.h"
#include "libxml/HTMLtree.h"
#include "libxml/uri.h"
#include <string.h>

/**
 * \brief Normalize path in URI
 *
 * \param msr ModSecurity transaction resource
 * \param input The URI to be normalized
 *
 * \retval input normalized input
 * \retval NULL on fail
 */
char *normalize_path(modsec_rec *msr, char *input) {
    xmlURI *uri = NULL;
    char *parsed_content = NULL;
    char *content = NULL;

    if(msr == NULL) return NULL;

    if(input == NULL) return NULL;

    uri = xmlParseURI(input);

    if(uri != NULL && uri->path)    {
        if(uri->scheme) {
            content = apr_psprintf(msr->mp, "%s://", uri->scheme);
            parsed_content = apr_pstrcat(msr->mp, content, NULL);
        }

        if(uri->server) {
            content = apr_psprintf(msr->mp, "%s", uri->server);
            if(parsed_content)
                parsed_content = apr_pstrcat(msr->mp, parsed_content, content, NULL);
            else
                parsed_content = apr_pstrcat(msr->mp, content, NULL);
        }

        if(uri->port)   {
            content = apr_psprintf(msr->mp, ":%d", uri->port);
            if(parsed_content)
                parsed_content = apr_pstrcat(msr->mp, parsed_content, content, NULL);
            else
                parsed_content = apr_pstrcat(msr->mp, content, NULL);
        }

        if(uri->path)   {
            char *Uri = NULL;
            int bytes = 0;
            int i;

            xmlNormalizeURIPath(uri->path);
            Uri = apr_pstrdup(msr->mp, uri->path);

            for(i = 0; i < (int)strlen(Uri); i++)    {
                if(Uri[i] != '.' && Uri[i] != '/')  {
                    if (i - 1 < 0)
                        i = 0;
                    else
                        i--;
                    if(Uri[i] == '/')
                        --bytes;
                    break;
                }   else    {
                    bytes++;
                }
            }

            if(bytes >= (int)strlen(uri->path))
                return NULL;

            content = apr_psprintf(msr->mp, "%s", uri->path+bytes);
            if(parsed_content)
                parsed_content = apr_pstrcat(msr->mp, parsed_content, content, NULL);
            else
                parsed_content = apr_pstrcat(msr->mp, content, NULL);
        }

        if(uri->query_raw)  {
            content = apr_psprintf(msr->mp, "?%s", uri->query_raw);
            if(parsed_content)
                parsed_content = apr_pstrcat(msr->mp, parsed_content, content, NULL);
            else
                parsed_content = apr_pstrcat(msr->mp, content, NULL);
        }

        if(uri->fragment)   {
            content = apr_psprintf(msr->mp, "#%s", uri->fragment);
            if(parsed_content)
                parsed_content = apr_pstrcat(msr->mp, parsed_content, content, NULL);
            else
                parsed_content = apr_pstrcat(msr->mp, content, NULL);
        }
        xmlFreeURI(uri);
        return apr_pstrdup(msr->mp, parsed_content);
    }

    if(uri != NULL) xmlFreeURI(uri);
    return apr_pstrdup(msr->mp, input);
}

/**
 * \brief Create a random password
 *
 * \param mp ModSecurity transaction memory pool
 *
 * \retval key random key
 */
char *getkey(apr_pool_t *mp) {
    unsigned char digest[APR_SHA1_DIGESTSIZE];
    char *sig, *key, *value;
    apr_sha1_ctx_t ctx;
    char salt[64];

    apr_generate_random_bytes(salt, sizeof(salt));
    key = apr_psprintf(mp,"%s",salt);

    apr_sha1_init (&ctx);
    apr_sha1_update (&ctx, (const char*)key, strlen(key));
    apr_sha1_update (&ctx, "\0", 1);

    apr_generate_random_bytes(salt, sizeof(salt));
    value = apr_psprintf(mp,"%s",salt);

    apr_sha1_update (&ctx, value, strlen (value));
    apr_sha1_final (digest, &ctx);

    sig = apr_pcalloc (mp, apr_base64_encode_len (sizeof (digest)));
    apr_base64_encode (sig, (const char*)digest, sizeof (digest));

    return sig;
}


/**
 * \brief Generate the MAC for a given message
 *
 * \param msr ModSecurity transaction resource
 * \param key The key used within HMAC
 * \param key_len Key length
 * \param msg The message to generate the MAC
 * \param msglen The message length
 *
 * \retval hex_digest The MAC
 */
char *hmac(modsec_rec *msr, const char *key, int key_len,
        unsigned char *msg, int msglen) {
    apr_sha1_ctx_t ctx;
    unsigned char digest[APR_SHA1_DIGESTSIZE];
    unsigned char hmac_ipad[HMAC_PAD_SIZE], hmac_opad[HMAC_PAD_SIZE];
    unsigned char nkey[APR_SHA1_DIGESTSIZE];
    unsigned char *hmac_key = (unsigned char *) key;
    char hex_digest[APR_SHA1_DIGESTSIZE * 2], *hmac_digest;
    const char hex[] = "0123456789abcdef";
    int i;

    if (key_len > HMAC_PAD_SIZE-1) {
        hmac_key = nkey;
        key_len = APR_SHA1_DIGESTSIZE;
    }

    memset ((void *) hmac_ipad, 0, sizeof (hmac_ipad));
    memset ((void *) hmac_opad, 0, sizeof (hmac_opad));
    memmove (hmac_ipad, hmac_key, key_len);
    memmove (hmac_opad, hmac_key, key_len);

    for (i = 0; i < HMAC_PAD_SIZE-1; i++)  {
        hmac_ipad[i] ^= 0x36;
        hmac_opad[i] ^= 0x5c;
    }

    apr_sha1_init (&ctx);
    apr_sha1_update_binary (&ctx, hmac_ipad, HMAC_PAD_SIZE-1);
    apr_sha1_update_binary (&ctx, (const unsigned char *) msg, msglen);
    apr_sha1_final (digest, &ctx);

    apr_sha1_init (&ctx);
    apr_sha1_update_binary (&ctx, hmac_opad, HMAC_PAD_SIZE-1);
    apr_sha1_update_binary (&ctx, digest, sizeof (digest));
    apr_sha1_final (digest, &ctx);

    hmac_digest = hex_digest;
    for (i = 0; i < sizeof (digest); i++) {
        *hmac_digest++ = hex[digest[i] >> 4];
        *hmac_digest++ = hex[digest[i] & 0xF];
    }

    *hmac_digest = '\0';

    return apr_pstrdup (msr->mp, hex_digest);
}


/**
 * \brief Init html response body parser
 *
 * \param msr ModSecurity transaction resource
 *
 * \retval 1 on success
 * \retval -1 on fail
 */
int init_response_body_html_parser(modsec_rec *msr)   {
    char *charset = NULL;
    char *final_charset = NULL;
    char sep;

    if(msr == NULL) return -1;

    if(msr->r == NULL) return -1;

    if(msr->r->content_type == NULL) return -1;

    if(msr->crypto_html_tree != NULL){
        xmlFreeDoc(msr->crypto_html_tree);
        msr->crypto_html_tree = NULL;
    }

    if((strncmp("text/html",msr->r->content_type,  9) != 0)){
        if (msr->txcfg->debuglog_level >= 4)
            msr_log(msr, 4,
                    "init_response_body_html_parser: skipping html_tree generation for Content[%s].", msr->r->content_type);
        if(msr->crypto_html_tree != NULL){
            xmlFreeDoc(msr->crypto_html_tree);
            msr->crypto_html_tree = NULL;
        }
        return -1;
    }

    if (msr->resbody_length == 0) {
        if (msr->txcfg->debuglog_level >= 4)
            msr_log(msr, 4, "init_response_body_html_parser: skipping html_tree generation for zero length respomse body.");
        msr->crypto_html_tree = NULL;
        return 0;
    }

    if((msr->r->content_encoding == NULL)||(apr_strnatcasecmp(msr->r->content_encoding,"(null)")==0)){
        charset=m_strcasestr(msr->r->content_type,"charset=");
        if(charset == NULL){
            if (msr->txcfg->debuglog_level >= 4)
                msr_log(msr, 4, "init_response_body_html_parser: assuming ISO-8859-1.");
            msr->crypto_html_tree = htmlReadMemory(msr->resbody_data, msr->resbody_length, NULL,
                    "ISO-8859-1", HTML_PARSE_RECOVER | HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
            htmlSetMetaEncoding ((htmlDocPtr) msr->crypto_html_tree, (const xmlChar *) "ISO-8859-1");
        }
        else{
            charset+=8;
            final_charset=strchr(charset,' ');
            if(final_charset == NULL) final_charset = strchr(charset,';');
            if(final_charset != NULL) {
                sep = *final_charset;
                *final_charset = '\0';
            }

            if (msr->txcfg->debuglog_level >= 4)
                msr_log(msr, 4,
                        "init_response_body_html_parser: Charset[%s]",charset);
            msr->crypto_html_tree = htmlReadMemory(msr->resbody_data, msr->resbody_length, NULL,
                    charset, HTML_PARSE_RECOVER| HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
            htmlSetMetaEncoding ((htmlDocPtr) msr->crypto_html_tree, (const xmlChar *)charset);
            if(final_charset != NULL) *final_charset=sep;
        }

    }
    else{
        if (msr->txcfg->debuglog_level >= 4)
            msr_log(msr, 4,"init_response_body_html_parser: Enconding[%s].",msr->r->content_encoding);
        msr->crypto_html_tree = htmlReadMemory(msr->resbody_data, msr->resbody_length, NULL,
                msr->r->content_encoding, HTML_PARSE_RECOVER | HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
        htmlSetMetaEncoding ((htmlDocPtr) msr->crypto_html_tree, (const xmlChar *)msr->r->content_encoding);
    }
    if(msr->crypto_html_tree == NULL){
        if (msr->txcfg->debuglog_level >= 4)
            msr_log(msr, 4,
                    "init_response_body_html_parser: Failed to parse response body.");
        return -1;
    }
    else {
        if (msr->txcfg->debuglog_level >= 4)
            msr_log(msr, 4,
                    "init_response_body_html_parser: Successfully html parser generated.");
        return 1;
    }

    return 1;
}

/**
 * \brief Execute all hash methods
 *
 * \param msr ModSecurity transaction resource
 * \param link The html attr value to be checked
 * \param type The hash method type
 *
 * \retval 1 Match
 * \retval 0 No Match
 * \retval -1 on fail
 */
int do_hash_method(modsec_rec *msr, char *link, int type)   {
    hash_method **em = NULL;
    int i = 0;
    char *error_msg = NULL;
    char *my_error_msg = NULL;
    int ovector[33];
    int rc;

    if(msr == NULL) return -1;

    em = (hash_method **)msr->txcfg->hash_method->elts;

    if(msr->txcfg->hash_method->nelts == 0)
        return 1;

    for (i = 0; i < msr->txcfg->hash_method->nelts; i++) {

        if(em[i] != NULL && em[i]->param_data != NULL){

            switch(type)    {
                case HASH_URL_HREF_HASH_PM:
                    if(em[i]->type == HASH_URL_HREF_HASH_PM)   {
                        const char *match = NULL;
                        apr_status_t rc = 0;
                        ACMPT pt;

                        pt.parser = (ACMP *)em[i]->param_data;
                        pt.ptr = NULL;

                        rc = acmp_process_quick(&pt, &match, link, strlen(link));

                        if (rc) {
                            return 1;
                        } else  {
                            return 0;
                        }
                    }
                    break;
                case HASH_URL_HREF_HASH_RX:
                    if(em[i]->type == HASH_URL_HREF_HASH_RX)   {
                        rc = msc_regexec_capture(em[i]->param_data, link, strlen(link), ovector, 30, &my_error_msg);
                        if ((rc == PCRE_ERROR_MATCHLIMIT) || (rc == PCRE_ERROR_RECURSIONLIMIT)) {
                            msc_string *s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));

                            if (s == NULL) return -1;
                            s->name = apr_pstrdup(msr->mp, "MSC_PCRE_LIMITS_EXCEEDED");
                            if (s->name == NULL) return -1;
                            s->name_len = strlen(s->name);
                            s->value = apr_pstrdup(msr->mp, "1");
                            if (s->value == NULL) return -1;
                            s->value_len = 1;
                            apr_table_setn(msr->tx_vars, s->name, (void *)s);

                            error_msg = apr_psprintf(msr->mp,
                                    "Execution error - "
                                    "PCRE limits exceeded for Hash regex [%s] (%d): %s",
                                    em[i]->param,rc, my_error_msg);

                            if (msr->txcfg->debuglog_level >= 4)
                                msr_log(msr, 4, "%s.", error_msg);

                            return 0; /* No match. */
                        }
                        else if (rc < -1) {
                            error_msg = apr_psprintf(msr->mp, "Regex execution failed (%d): %s",
                                    rc, my_error_msg);
                            if (msr->txcfg->debuglog_level >= 4)
                                msr_log(msr, 4, "%s.", error_msg);
                            return -1;
                        }
                        if (rc != PCRE_ERROR_NOMATCH) { /* Match. */
                            return 1;
                        }
                    }
                    break;
                case HASH_URL_FACTION_HASH_PM:
                    if(em[i]->type == HASH_URL_FACTION_HASH_PM)   {
                       const char *match = NULL;
                        apr_status_t rc = 0;
                        ACMPT pt;

                        pt.parser = (ACMP *)em[i]->param_data;
                        pt.ptr = NULL;

                        rc = acmp_process_quick(&pt, &match, link, strlen(link));

                        if (rc) {
                            return 1;
                        } else  {
                            return 0;
                        }
                    }
                    break;
                case HASH_URL_FACTION_HASH_RX:
                    if(em[i]->type == HASH_URL_FACTION_HASH_RX)   {
                        rc = msc_regexec_capture(em[i]->param_data, link, strlen(link), ovector, 30, &my_error_msg);
                        if ((rc == PCRE_ERROR_MATCHLIMIT) || (rc == PCRE_ERROR_RECURSIONLIMIT)) {
                            msc_string *s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));

                            if (s == NULL) return -1;
                            s->name = apr_pstrdup(msr->mp, "MSC_PCRE_LIMITS_EXCEEDED");
                            if (s->name == NULL) return -1;
                            s->name_len = strlen(s->name);
                            s->value = apr_pstrdup(msr->mp, "1");
                            if (s->value == NULL) return -1;
                            s->value_len = 1;
                            apr_table_setn(msr->tx_vars, s->name, (void *)s);

                            error_msg = apr_psprintf(msr->mp,
                                    "Execution error - "
                                    "PCRE limits exceeded for Hash regex [%s] (%d): %s",
                                    em[i]->param,rc, my_error_msg);

                            if (msr->txcfg->debuglog_level >= 4)
                                msr_log(msr, 4, "%s.", error_msg);

                            return 0; /* No match. */
                        }
                        else if (rc < -1) {
                            error_msg = apr_psprintf(msr->mp, "Regex execution failed (%d): %s",
                                    rc, my_error_msg);
                            if (msr->txcfg->debuglog_level >= 4)
                                msr_log(msr, 4, "%s.", error_msg);
                            return -1;
                        }
                        if (rc != PCRE_ERROR_NOMATCH) { /* Match. */
                            return 1;
                        }
                    }
                    break;
                case HASH_URL_LOCATION_HASH_PM:
                    if(em[i]->type == HASH_URL_LOCATION_HASH_PM)   {
                       const char *match = NULL;
                        apr_status_t rc = 0;
                        ACMPT pt;

                        pt.parser = (ACMP *)em[i]->param_data;
                        pt.ptr = NULL;

                        rc = acmp_process_quick(&pt, &match, link, strlen(link));

                        if (rc) {
                            return 1;
                        } else  {
                            return 0;
                        }
                    }
                    break;
                case HASH_URL_LOCATION_HASH_RX:
                    if(em[i]->type == HASH_URL_LOCATION_HASH_RX)   {
                        rc = msc_regexec_capture(em[i]->param_data, link, strlen(link), ovector, 30, &my_error_msg);
                        if ((rc == PCRE_ERROR_MATCHLIMIT) || (rc == PCRE_ERROR_RECURSIONLIMIT)) {
                            msc_string *s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));

                            if (s == NULL) return -1;
                            s->name = apr_pstrdup(msr->mp, "MSC_PCRE_LIMITS_EXCEEDED");
                            if (s->name == NULL) return -1;
                            s->name_len = strlen(s->name);
                            s->value = apr_pstrdup(msr->mp, "1");
                            if (s->value == NULL) return -1;
                            s->value_len = 1;
                            apr_table_setn(msr->tx_vars, s->name, (void *)s);

                            error_msg = apr_psprintf(msr->mp,
                                    "Execution error - "
                                    "PCRE limits exceeded for Hash regex [%s] (%d): %s",
                                    em[i]->param,rc, my_error_msg);

                            if (msr->txcfg->debuglog_level >= 4)
                                msr_log(msr, 4, "%s.", error_msg);

                            return 0; /* No match. */
                        }
                        else if (rc < -1) {
                            error_msg = apr_psprintf(msr->mp, "Regex execution failed (%d): %s",
                                    rc, my_error_msg);
                            if (msr->txcfg->debuglog_level >= 4)
                                msr_log(msr, 4, "%s.", error_msg);
                            return -1;
                        }
                        if (rc != PCRE_ERROR_NOMATCH) { /* Match. */
                            return 1;
                        }
                    }
                    break;
                case HASH_URL_IFRAMESRC_HASH_PM:
                    if(em[i]->type == HASH_URL_IFRAMESRC_HASH_PM)   {
                       const char *match = NULL;
                        apr_status_t rc = 0;
                        ACMPT pt;

                        pt.parser = (ACMP *)em[i]->param_data;
                        pt.ptr = NULL;

                        rc = acmp_process_quick(&pt, &match, link, strlen(link));

                        if (rc) {
                            return 1;
                        } else  {
                            return 0;
                        }
                    }
                    break;
                case HASH_URL_IFRAMESRC_HASH_RX:
                    if(em[i]->type == HASH_URL_IFRAMESRC_HASH_RX)   {
                        rc = msc_regexec_capture(em[i]->param_data, link, strlen(link), ovector, 30, &my_error_msg);
                        if ((rc == PCRE_ERROR_MATCHLIMIT) || (rc == PCRE_ERROR_RECURSIONLIMIT)) {
                            msc_string *s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));

                            if (s == NULL) return -1;
                            s->name = apr_pstrdup(msr->mp, "MSC_PCRE_LIMITS_EXCEEDED");
                            if (s->name == NULL) return -1;
                            s->name_len = strlen(s->name);
                            s->value = apr_pstrdup(msr->mp, "1");
                            if (s->value == NULL) return -1;
                            s->value_len = 1;
                            apr_table_setn(msr->tx_vars, s->name, (void *)s);

                            error_msg = apr_psprintf(msr->mp,
                                    "Execution error - "
                                    "PCRE limits exceeded for Hash regex [%s] (%d): %s",
                                    em[i]->param,rc, my_error_msg);

                            if (msr->txcfg->debuglog_level >= 4)
                                msr_log(msr, 4, "%s.", error_msg);

                            return 0; /* No match. */
                        }
                        else if (rc < -1) {
                            error_msg = apr_psprintf(msr->mp, "Regex execution failed (%d): %s",
                                    rc, my_error_msg);
                            if (msr->txcfg->debuglog_level >= 4)
                                msr_log(msr, 4, "%s.", error_msg);
                            return -1;
                        }
                        if (rc != PCRE_ERROR_NOMATCH) { /* Match. */
                            return 1;
                        }
                    }
                    break;
                case HASH_URL_FRAMESRC_HASH_PM:
                    if(em[i]->type == HASH_URL_FRAMESRC_HASH_PM)   {
                       const char *match = NULL;
                        apr_status_t rc = 0;
                        ACMPT pt;

                        pt.parser = (ACMP *)em[i]->param_data;
                        pt.ptr = NULL;

                        rc = acmp_process_quick(&pt, &match, link, strlen(link));

                        if (rc) {
                            return 1;
                        } else  {
                            return 0;
                        }
                    }
                    break;
                case HASH_URL_FRAMESRC_HASH_RX:
                    if(em[i]->type == HASH_URL_FRAMESRC_HASH_RX)   {
                        rc = msc_regexec_capture(em[i]->param_data, link, strlen(link), ovector, 30, &my_error_msg);
                        if ((rc == PCRE_ERROR_MATCHLIMIT) || (rc == PCRE_ERROR_RECURSIONLIMIT)) {
                            msc_string *s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));

                            if (s == NULL) return -1;
                            s->name = apr_pstrdup(msr->mp, "MSC_PCRE_LIMITS_EXCEEDED");
                            if (s->name == NULL) return -1;
                            s->name_len = strlen(s->name);
                            s->value = apr_pstrdup(msr->mp, "1");
                            if (s->value == NULL) return -1;
                            s->value_len = 1;
                            apr_table_setn(msr->tx_vars, s->name, (void *)s);

                            error_msg = apr_psprintf(msr->mp,
                                    "Execution error - "
                                    "PCRE limits exceeded for Hash regex [%s] (%d): %s",
                                    em[i]->param,rc, my_error_msg);

                            if (msr->txcfg->debuglog_level >= 4)
                                msr_log(msr, 4, "%s.", error_msg);

                            return 0; /* No match. */
                        }
                        else if (rc < -1) {
                            error_msg = apr_psprintf(msr->mp, "Regex execution failed (%d): %s",
                                    rc, my_error_msg);
                            if (msr->txcfg->debuglog_level >= 4)
                                msr_log(msr, 4, "%s.", error_msg);
                            return -1;
                        }
                        if (rc != PCRE_ERROR_NOMATCH) { /* Match. */
                            return 1;
                        }
                    }
                    break;
            }
        }

    }
    return 0;
}

/**
 * \brief Hash the html elements
 *
 * \param msr ModSecurity transaction resource
 *
 * \retval 1 On success
 * \retval 0 No element was changed
 * \retval -1 On fail
 */
int hash_response_body_links(modsec_rec *msr)   {
    int lsize = 0, fsize = 0, lcount = 0, fcount = 0, i;
    int isize = 0, icount = 0, frsize = 0, frcount = 0;
    xmlXPathContextPtr  xpathCtx = NULL;
    xmlXPathObjectPtr   xpathObj = NULL;
    xmlChar *content_option = NULL;
    char *mac_link = NULL;
    int rc, elts = 0;

    if(msr == NULL)
        return -1;

    if (msr->crypto_html_tree == NULL) {
        if (msr->txcfg->debuglog_level >= 4)
            msr_log(msr, 4, "hash_response_body_links: Cannot parse NULL html tree");
        return -1;
    }

    if(msr->txcfg->crypto_hash_href_rx == 0 && msr->txcfg->crypto_hash_href_pm == 0
            && msr->txcfg->crypto_hash_faction_rx == 0 && msr->txcfg->crypto_hash_faction_pm == 0
            && msr->txcfg->crypto_hash_iframesrc_rx == 0 && msr->txcfg->crypto_hash_iframesrc_pm == 0
            && msr->txcfg->crypto_hash_framesrc_rx == 0 && msr->txcfg->crypto_hash_framesrc_pm == 0)
        return -1;

    xpathCtx = xmlXPathNewContext(msr->crypto_html_tree);
    if(xpathCtx == NULL) {
        if (msr->txcfg->debuglog_level >= 4)
            msr_log(msr, 4, "hash_response_body_links: Unable to create Xpath context.");
        goto ctx_error;
    }

    lcount=fcount=0;

    if(msr->txcfg->crypto_hash_href_rx == 1 || msr->txcfg->crypto_hash_href_pm == 1)    {

        xpathObj = xmlXPathEvalExpression((xmlChar*)"//*[@href]", xpathCtx);
        if(xpathObj == NULL) {
            if (msr->txcfg->debuglog_level >= 4)
                msr_log(msr, 4,
                        "hash_response_body_links: Unable to evaluate xpath expression.");
            goto obj_error;
        }

        lsize = (xpathObj->nodesetval) ? xpathObj->nodesetval->nodeNr : 0;
        for(i = lsize - 1; i >=0; i--) {
            register xmlNodePtr cur;

            cur = xpathObj->nodesetval->nodeTab[i];
            if(cur != NULL){
                xmlChar *href = xmlGetProp(cur, (const xmlChar *) "href");
                char *content_href = normalize_path(msr, (char *)href);

                if(content_href != NULL && strstr(content_href,msr->txcfg->crypto_param_name) == NULL) {
                    if(msr->txcfg->crypto_hash_href_rx == 1)    {
                        rc = do_hash_method(msr, (char *)content_href, HASH_URL_HREF_HASH_RX);
                        if(rc > 0)  {
                            mac_link = NULL;
                            mac_link = do_hash_link(msr, (char *)content_href, FULL_LINK);
                            if(mac_link != NULL) {
                                xmlSetProp(cur, (const xmlChar *) "href", (const xmlChar *) mac_link);
                                lcount++;
                                msr->of_stream_changed = 1;
                            }
                            mac_link = NULL;
                            if(href != NULL)
                                xmlFree(href);
                            continue;
                        }
                    }
                    if(msr->txcfg->crypto_hash_href_pm == 1)    {
                        rc = do_hash_method(msr, (char *)content_href, HASH_URL_HREF_HASH_PM);
                        if(rc > 0)  {
                            mac_link = NULL;
                            mac_link = do_hash_link(msr, (char *)content_href, FULL_LINK);
                            if(mac_link != NULL) {
                                xmlSetProp(cur, (const xmlChar *) "href", (const xmlChar *) mac_link);
                                lcount++;
                                msr->of_stream_changed = 1;
                            }
                            mac_link = NULL;
                            if(href != NULL)
                                xmlFree(href);
                            continue;
                        }
                    }
                }

                if(href != NULL)    {
                    xmlFree(href);
                    href = NULL;
                }
            }
        }

        if(xpathObj != NULL)
            xmlXPathFreeObject(xpathObj);
    }

    if(msr->txcfg->crypto_hash_faction_rx == 1 || msr->txcfg->crypto_hash_faction_pm == 1) {
        xpathObj = xmlXPathEvalExpression((xmlChar*)"//form", xpathCtx);
        if(xpathObj == NULL) {
            if (msr->txcfg->debuglog_level >= 4)
                msr_log(msr, 4,
                        "hash_response_body_links: Unable to evaluate xpath expression.");
            goto obj_error;
        }

        fsize = (xpathObj->nodesetval) ? xpathObj->nodesetval->nodeNr : 0;
        for(i = fsize - 1; i >=0; i--) {
            register xmlNodePtr cur;

            cur = xpathObj->nodesetval->nodeTab[i];
            if((cur != NULL)){
                xmlChar *action = NULL;
                char *content_action = NULL;

                if(content_option)
                    xmlFree(content_option);

                action = xmlGetProp(cur, (const xmlChar *) "action");
                content_action = normalize_path(msr, (char *)action);
                content_option = xmlGetProp(cur, (const xmlChar *) "option");

                if(content_action != NULL && content_option == NULL && strstr(content_action,msr->txcfg->crypto_param_name) == NULL) {
                    if(msr->txcfg->crypto_hash_faction_rx == 1) {
                        rc = do_hash_method(msr, (char *)content_action, HASH_URL_FACTION_HASH_RX);
                        if(rc > 0)  {
                            mac_link = NULL;
                            mac_link = do_hash_link(msr, (char *)content_action, FULL_LINK);
                            if(mac_link != NULL) {
                                xmlSetProp(cur, (const xmlChar *) "action", (const xmlChar *) mac_link);
                                fcount++;
                                msr->of_stream_changed = 1;
                            }
                            mac_link = NULL;
                            if(action != NULL)
                                xmlFree(action);
                            continue;
                        }
                    }
                    if(msr->txcfg->crypto_hash_faction_pm == 1) {
                        rc = do_hash_method(msr, (char *)content_action, HASH_URL_FACTION_HASH_PM);
                        if(rc > 0)  {
                            mac_link = NULL;
                            mac_link = do_hash_link(msr, (char *)content_action, FULL_LINK);
                            if(mac_link != NULL) {
                                xmlSetProp(cur, (const xmlChar *) "action", (const xmlChar *) mac_link);
                                fcount++;
                                msr->of_stream_changed = 1;
                            }
                            mac_link = NULL;
                            if(action != NULL)
                                xmlFree(action);
                            continue;
                        }
                    }
                }

                if(action != NULL)  {
                    xmlFree(action);
                    action = NULL;
                }

                if(content_option)  {
                    xmlFree(content_option);
                    content_option = NULL;
                }
            }
        }

        if(xpathObj != NULL)
            xmlXPathFreeObject(xpathObj);
    }

    if(msr->txcfg->crypto_hash_iframesrc_rx == 1 || msr->txcfg->crypto_hash_iframesrc_pm == 1) {
        xpathObj = xmlXPathEvalExpression((xmlChar*)"//iframe", xpathCtx);
        if(xpathObj == NULL) {
            if (msr->txcfg->debuglog_level >= 4)
                msr_log(msr, 4,
                        "hash_response_body_links: Unable to evaluate xpath expression.");
            goto obj_error;
        }

        isize = (xpathObj->nodesetval) ? xpathObj->nodesetval->nodeNr : 0;
        for(i = isize - 1; i >=0; i--) {
            register xmlNodePtr cur;

            cur = xpathObj->nodesetval->nodeTab[i];
            if((cur != NULL)){

                xmlChar *src = xmlGetProp(cur, (const xmlChar *) "src");
                char *content_src = normalize_path(msr, (char *)src);

                if(content_src != NULL && strstr(content_src,msr->txcfg->crypto_param_name) == NULL) {
                    if(msr->txcfg->crypto_hash_iframesrc_rx == 1) {
                        rc = do_hash_method(msr, (char *)content_src, HASH_URL_IFRAMESRC_HASH_RX);
                        if(rc > 0)  {
                            mac_link = NULL;
                            mac_link = do_hash_link(msr, (char *)content_src, FULL_LINK);
                            if(mac_link != NULL) {
                                xmlSetProp(cur, (const xmlChar *) "src", (const xmlChar *) mac_link);
                                icount++;
                                msr->of_stream_changed = 1;
                            }
                            mac_link = NULL;
                            if(src != NULL)
                                xmlFree(src);
                            continue;
                        }
                    }
                    if(msr->txcfg->crypto_hash_iframesrc_pm == 1) {
                        rc = do_hash_method(msr, (char *)content_src, HASH_URL_IFRAMESRC_HASH_PM);
                        if(rc > 0)  {
                            mac_link = NULL;
                            mac_link = do_hash_link(msr, (char *)content_src, FULL_LINK);
                            if(mac_link != NULL) {
                                xmlSetProp(cur, (const xmlChar *) "src", (const xmlChar *) mac_link);
                                icount++;
                                msr->of_stream_changed = 1;
                            }
                            mac_link = NULL;
                            if(src != NULL)
                                xmlFree(src);
                            continue;
                        }
                    }
                }

                if(src != NULL) {
                    xmlFree(src);
                    src = NULL;
                }
            }
        }

        if(xpathObj != NULL)
            xmlXPathFreeObject(xpathObj);
    }

    if(msr->txcfg->crypto_hash_framesrc_rx == 1 || msr->txcfg->crypto_hash_framesrc_pm == 1) {
        xpathObj = xmlXPathEvalExpression((xmlChar*)"//frame", xpathCtx);
        if(xpathObj == NULL) {
            if (msr->txcfg->debuglog_level >= 4)
                msr_log(msr, 4,
                        "hash_response_body_links: Unable to evaluate xpath expression.");
            goto obj_error;
        }

        frsize = (xpathObj->nodesetval) ? xpathObj->nodesetval->nodeNr : 0;
        for(i = frsize - 1; i >=0; i--) {
            register xmlNodePtr cur;

            cur = xpathObj->nodesetval->nodeTab[i];
            if((cur != NULL)){

                xmlChar *src = xmlGetProp(cur, (const xmlChar *) "src");
                char *content_src = normalize_path(msr, (char *)src);

                if(content_src != NULL && strstr(content_src,msr->txcfg->crypto_param_name) == NULL) {
                    if(msr->txcfg->crypto_hash_framesrc_rx == 1) {
                        rc = do_hash_method(msr, (char *)content_src, HASH_URL_FRAMESRC_HASH_RX);
                        if(rc > 0)  {
                            mac_link = NULL;
                            mac_link = do_hash_link(msr, (char *)content_src, FULL_LINK);
                            if(mac_link != NULL) {
                                xmlSetProp(cur, (const xmlChar *) "src", (const xmlChar *) mac_link);
                                frcount++;
                                msr->of_stream_changed = 1;
                            }
                            mac_link = NULL;
                            if(src != NULL)
                                xmlFree(src);
                            continue;
                        }
                    }
                    if(msr->txcfg->crypto_hash_framesrc_pm == 1) {
                        rc = do_hash_method(msr, (char *)content_src, HASH_URL_FRAMESRC_HASH_PM);
                        if(rc > 0)  {
                            mac_link = NULL;
                            mac_link = do_hash_link(msr, (char *)content_src, FULL_LINK);
                            if(mac_link != NULL) {
                                xmlSetProp(cur, (const xmlChar *) "src", (const xmlChar *) mac_link);
                                frcount++;
                                msr->of_stream_changed = 1;
                            }
                            mac_link = NULL;
                            if(src != NULL)
                                xmlFree(src);
                            continue;
                        }
                    }
                }

                if(src != NULL) {
                    xmlFree(src);
                    src = NULL;
                }
            }
        }

        if(xpathObj != NULL)
            xmlXPathFreeObject(xpathObj);
    }

    if(xpathCtx != NULL)
        xmlXPathFreeContext(xpathCtx);

    if (msr->txcfg->debuglog_level >= 4)    {
        msr_log(msr, 4, "hash_response_body_links: Processed [%d] iframe src, [%d] hashed.",isize, icount);
        msr_log(msr, 4, "hash_response_body_links: Processed [%d] frame src, [%d] hashed.",frsize, frcount);
        msr_log(msr, 4, "hash_response_body_links: Processed [%d] form actions, [%d] hashed.",fsize, fcount);
        msr_log(msr, 4, "hash_response_body_links: Processed [%d] links, [%d] hashed.",lsize, lcount);
    }

    if(msr->of_stream_changed == 0) {
        if(msr->crypto_html_tree != NULL)   {
            xmlFreeDoc(msr->crypto_html_tree);
            msr->crypto_html_tree = NULL;
        }
        return 0;
    }

    elts = (icount+frcount+fcount+lcount);

    if((elts >= INT32_MAX) || (elts < 0))
        return 0;

    return elts;

obj_error:
    if(xpathCtx != NULL)
    xmlXPathFreeContext(xpathCtx);
ctx_error:
    return -1;
}

/**
 * \brief Inject the new response body
 *
 * \param msr ModSecurity transaction resource
 * \param elts Number of hashed elements
 *
 * \retval 1 On success
 * \retval -1 On fail
 */
int inject_hashed_response_body(modsec_rec *msr, int elts) {
    xmlOutputBufferPtr output_buf = NULL;
    xmlCharEncodingHandlerPtr  handler = NULL;
    char *p = NULL;
    const char *ctype = NULL;
    const char *encoding = NULL;
    char *new_ct = NULL, *content_value = NULL;

    if(msr == NULL) return -1;

    if(msr->r == NULL) return -1;

    if (msr->crypto_html_tree == NULL) {
        if (msr->txcfg->debuglog_level >= 4)
            msr_log(msr, 4, "inject_hashed_response_body: Cannot parse NULL html tree");
        return -1;
    }

    if(msr->r->content_type != NULL)
        ctype = msr->r->content_type;

    encoding = (const char *) htmlGetMetaEncoding(msr->crypto_html_tree);

    if (ctype && encoding == NULL) {
        if (ctype && (p = m_strcasestr(ctype, "charset=") , p != NULL)) {
            p += 8 ;
            if (encoding = apr_pstrndup(msr->mp, p, strcspn(p, " ;") ), encoding) {
                xmlCharEncoding enc;
                enc = xmlParseCharEncoding(encoding);
                handler = xmlFindCharEncodingHandler(encoding);
            }
        }
    } else  {
        if(encoding != NULL)    {
            xmlCharEncoding enc;
            enc = xmlParseCharEncoding(encoding);
            handler = xmlFindCharEncodingHandler(encoding);
        }
    }

    if (msr->txcfg->debuglog_level >= 4)
        msr_log(msr, 4, "inject_hashed_response_body: Detected encoding type [%s].", encoding);

    if (handler == NULL)
        handler = xmlFindCharEncodingHandler("UTF-8");
    if (handler == NULL)
        handler = xmlFindCharEncodingHandler("ISO-8859-1");
    if (handler == NULL)
        handler = xmlFindCharEncodingHandler("HTML");
    if (handler == NULL)
        handler = xmlFindCharEncodingHandler("ascii");

    if(handler == NULL) {
        xmlFreeDoc(msr->crypto_html_tree);
        return -1;
    }

    apr_table_unset(msr->r->headers_out,"Content-Type");
    new_ct = (char*)apr_psprintf(msr->mp, "text/html;%s",handler->name);
    apr_table_set(msr->r->err_headers_out,"Content-Type",new_ct);

    if (msr->txcfg->debuglog_level >= 4)
        msr_log(msr, 4, "inject_hashed_response_body: Using content-type [%s].", handler->name);

    output_buf = xmlAllocOutputBuffer(handler);
    if (output_buf == NULL) {
        if (msr->txcfg->debuglog_level >= 4)
            msr_log(msr, 4, "inject_hashed_response_body: Unable to allocate memory buffer.");
        xmlFreeDoc(msr->crypto_html_tree);
        return -1;
    }

    htmlDocContentDumpFormatOutput(output_buf, msr->crypto_html_tree, NULL, 0);

#ifdef  LIBXML2_NEW_BUFFER

    if (output_buf->conv == NULL || (output_buf->conv && xmlOutputBufferGetSize(output_buf) == 0)) {

        if(output_buf->buffer == NULL || xmlOutputBufferGetSize(output_buf) == 0)  {
            xmlOutputBufferClose(output_buf);
            xmlFreeDoc(msr->crypto_html_tree);
            msr->of_stream_changed = 0;
            return -1;
        }

        if(msr->stream_output_data != NULL) {
            free(msr->stream_output_data);
            msr->stream_output_data =  NULL;
        }

        msr->stream_output_length = xmlOutputBufferGetSize(output_buf);
        msr->stream_output_data = (char *)malloc(msr->stream_output_length+1);

        if (msr->stream_output_data == NULL) {
            xmlOutputBufferClose(output_buf);
            xmlFreeDoc(msr->crypto_html_tree);
            return -1;
        }

        memset(msr->stream_output_data, 0x0, msr->stream_output_length+1);
        memcpy(msr->stream_output_data, xmlOutputBufferGetContent(output_buf), msr->stream_output_length);

        if (msr->txcfg->debuglog_level >= 4)
            msr_log(msr, 4, "inject_hashed_response_body: Copying XML tree from CONTENT to stream buffer [%d] bytes.", xmlOutputBufferGetSize(output_buf));

    } else {

        if(output_buf->conv == NULL || xmlOutputBufferGetSize(output_buf) == 0)  {
            xmlOutputBufferClose(output_buf);
            xmlFreeDoc(msr->crypto_html_tree);
            msr->of_stream_changed = 0;
            return -1;
        }

        if(msr->stream_output_data != NULL) {
            free(msr->stream_output_data);
            msr->stream_output_data =  NULL;
        }

        msr->stream_output_length = xmlOutputBufferGetSize(output_buf);
        msr->stream_output_data = (char *)malloc(msr->stream_output_length+1);

        if (msr->stream_output_data == NULL) {
            xmlOutputBufferClose(output_buf);
            xmlFreeDoc(msr->crypto_html_tree);
            return -1;
        }

        memset(msr->stream_output_data, 0x0, msr->stream_output_length+1);
        memcpy(msr->stream_output_data, xmlOutputBufferGetContent(output_buf), msr->stream_output_length);

        if (msr->txcfg->debuglog_level >= 4)
            msr_log(msr, 4, "inject_hashed_response_body: Copying XML tree from CONV to stream buffer [%d] bytes.", xmlOutputBufferGetSize(output_buf));

    }

#else

    if (output_buf->conv == NULL || (output_buf->conv && output_buf->conv->use == 0)) {

        if(output_buf->buffer == NULL || output_buf->buffer->use == 0)  {
            xmlOutputBufferClose(output_buf);
            xmlFreeDoc(msr->crypto_html_tree);
            msr->of_stream_changed = 0;
            return -1;
        }

        if(msr->stream_output_data != NULL) {
            free(msr->stream_output_data);
            msr->stream_output_data =  NULL;
        }

        msr->stream_output_length = output_buf->buffer->use;
        msr->stream_output_data = (char *)malloc(msr->stream_output_length+1);

        if (msr->stream_output_data == NULL) {
            xmlOutputBufferClose(output_buf);
            xmlFreeDoc(msr->crypto_html_tree);
            return -1;
        }

        memset(msr->stream_output_data, 0x0, msr->stream_output_length+1);
        memcpy(msr->stream_output_data, output_buf->buffer->content, msr->stream_output_length);

        if (msr->txcfg->debuglog_level >= 4)
            msr_log(msr, 4, "inject_hashed_response_body: Copying XML tree from CONTENT to stream buffer [%d] bytes.", output_buf->buffer->use);

    } else {

        if(output_buf->conv == NULL || output_buf->conv->use == 0)  {
            xmlOutputBufferClose(output_buf);
            xmlFreeDoc(msr->crypto_html_tree);
            msr->of_stream_changed = 0;
            return -1;
        }

        if(msr->stream_output_data != NULL) {
            free(msr->stream_output_data);
            msr->stream_output_data =  NULL;
        }

        msr->stream_output_length = output_buf->conv->use;
        msr->stream_output_data = (char *)malloc(msr->stream_output_length+1);

        if (msr->stream_output_data == NULL) {
            xmlOutputBufferClose(output_buf);
            xmlFreeDoc(msr->crypto_html_tree);
            return -1;
        }

        memset(msr->stream_output_data, 0x0, msr->stream_output_length+1);
        memcpy(msr->stream_output_data, output_buf->conv->content, msr->stream_output_length);

        if (msr->txcfg->debuglog_level >= 4)
            msr_log(msr, 4, "inject_hashed_response_body: Copying XML tree from CONV to stream buffer [%d] bytes.", output_buf->conv->use);

    }

#endif

    xmlOutputBufferClose(output_buf);

    content_value = (char*)apr_psprintf(msr->mp, "%"APR_SIZE_T_FMT, msr->stream_output_length);
    apr_table_unset(msr->r->headers_out,"Content-Length");

    if (msr->txcfg->debuglog_level >= 4)
        msr_log(msr, 4, "inject_hashed_response_body: Setting new content value %s", content_value);
    apr_table_set(msr->r->headers_out, "Content-Length", content_value);

    xmlFreeDoc(msr->crypto_html_tree);

    if (msr->txcfg->debuglog_level >= 4)
        msr_log(msr, 4, "inject_hashed_response_body: Stream buffer [%"APR_SIZE_T_FMT"]. Done",msr->stream_output_length);

    return 1;
}

/**
 * \brief Parse and MAC html elements
 *
 * \param msr ModSecurity transaction resource
 * \param link The html attr value to be checked
 * \param type The hash method type
 *
 * \retval mac_link MACed link
 * \retval NULL on fail
 */
char *do_hash_link(modsec_rec *msr, char *link, int type)  {
    char  *mac_link = NULL;
    char *path_chunk = NULL;
    char *hash_value = NULL;
    char *qm = NULL;

    if(msr == NULL) return NULL;

    if(strlen(link) > 7 && strncmp("http:",(char*)link,5)==0){
        path_chunk = strchr(link+7,'/');
        if(path_chunk != NULL)  {
            if (msr->txcfg->debuglog_level >= 4)
                msr_log(msr, 4, "Signing data [%s]", path_chunk+1);

            if(msr->txcfg->crypto_key_add == HASH_KEYONLY)
                hash_value =  hmac(msr, msr->txcfg->crypto_key, msr->txcfg->crypto_key_len, (unsigned char *) path_chunk+1, strlen((char*)path_chunk)-1);

            if(msr->txcfg->crypto_key_add == HASH_SESSIONID)  {
                if(strlen(msr->sessionid) == 0)   {
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
                    const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->client_ip);
#else
                    const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->remote_ip);
#endif
                    if (msr->txcfg->debuglog_level >= 4)
                        msr_log(msr, 4, "Session id is empty. Using REMOTE_IP");
                    msr->txcfg->crypto_key_len = strlen(new_pwd);
                    hash_value = hmac(msr, new_pwd, msr->txcfg->crypto_key_len, (unsigned char *) path_chunk+1, strlen((char*)path_chunk)-1);
                } else {
                    const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->sessionid);
                    if (msr->txcfg->debuglog_level >= 4)
                        msr_log(msr, 4, "Using session id [%s]", msr->sessionid);
                    msr->txcfg->crypto_key_len = strlen(new_pwd);
                    hash_value = hmac(msr, new_pwd, msr->txcfg->crypto_key_len, (unsigned char *) path_chunk+1, strlen((char*)path_chunk)-1);
                }
            }

            if(msr->txcfg->crypto_key_add == HASH_REMOTEIP)   {
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
                const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->client_ip);
#else
                const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->remote_ip);
#endif
                msr->txcfg->crypto_key_len = strlen(new_pwd);
                hash_value = hmac(msr, new_pwd, msr->txcfg->crypto_key_len, (unsigned char *) path_chunk+1, strlen((char*)path_chunk)-1);
            }
        } else  {
            return NULL;
        }
    } else
        if(strlen(link) > 8 && strncmp("https",(char*)link,5)==0){
            path_chunk = strchr(link+8,'/');
            if(path_chunk != NULL)  {
                if (msr->txcfg->debuglog_level >= 4)
                    msr_log(msr, 4, "Signing data [%s]", path_chunk+1);

                if(msr->txcfg->crypto_key_add == HASH_KEYONLY)
                    hash_value =  hmac(msr, msr->txcfg->crypto_key, msr->txcfg->crypto_key_len, (unsigned char *) path_chunk+1, strlen((char*)path_chunk)-1);

                if(msr->txcfg->crypto_key_add == HASH_SESSIONID)  {
                    if(strlen(msr->sessionid) == 0)   {
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
                        const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->client_ip);
#else
                        const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->remote_ip);
#endif
                        if (msr->txcfg->debuglog_level >= 4)
                            msr_log(msr, 4, "Session id is empty. Using REMOTE_IP");
                        msr->txcfg->crypto_key_len = strlen(new_pwd);
                        hash_value = hmac(msr, new_pwd, msr->txcfg->crypto_key_len, (unsigned char *) path_chunk+1, strlen((char*)path_chunk)-1);
                    } else {
                        const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->sessionid);
                        if (msr->txcfg->debuglog_level >= 4)
                            msr_log(msr, 4, "Using session id [%s]", msr->sessionid);
                        msr->txcfg->crypto_key_len = strlen(new_pwd);
                        hash_value = hmac(msr, new_pwd, msr->txcfg->crypto_key_len, (unsigned char *) path_chunk+1, strlen((char*)path_chunk)-1);
                    }
                }

                if(msr->txcfg->crypto_key_add == HASH_REMOTEIP)   {
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
                    const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->client_ip);
#else
                    const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->remote_ip);
#endif
                    msr->txcfg->crypto_key_len = strlen(new_pwd);
                    hash_value = hmac(msr, new_pwd, msr->txcfg->crypto_key_len, (unsigned char *) path_chunk+1, strlen((char*)path_chunk)-1);
                }
            } else  {
                return NULL;
            }
        }
        else if(*link=='/'){
            if (msr->txcfg->debuglog_level >= 4)
                msr_log(msr, 4, "Signing data [%s]", link+1);

            if(msr->txcfg->crypto_key_add == HASH_KEYONLY)
                hash_value = hmac(msr, msr->txcfg->crypto_key, msr->txcfg->crypto_key_len, (unsigned char *) link+1, strlen((char*)link)-1);

            if(msr->txcfg->crypto_key_add == HASH_SESSIONID)  {
                if(strlen(msr->sessionid) == 0)   {
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
                    const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->client_ip);
#else
                    const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->remote_ip);
#endif
                    if (msr->txcfg->debuglog_level >= 4)
                        msr_log(msr, 4, "Session id is empty. Using REMOTE_IP");
                    msr->txcfg->crypto_key_len = strlen(new_pwd);
                    hash_value = hmac(msr, new_pwd, msr->txcfg->crypto_key_len, (unsigned char *) link+1, strlen((char*)link)-1);
                } else  {
                    const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->sessionid);
                    if (msr->txcfg->debuglog_level >= 4)
                        msr_log(msr, 4, "Using session id [%s]", msr->sessionid);
                    msr->txcfg->crypto_key_len = strlen(new_pwd);
                    hash_value = hmac(msr, new_pwd, msr->txcfg->crypto_key_len, (unsigned char *) link+1, strlen((char*)link)-1);
                }
            }

            if(msr->txcfg->crypto_key_add == HASH_REMOTEIP)   {
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
                const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->client_ip);
#else
                const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->remote_ip);
#endif
                msr->txcfg->crypto_key_len = strlen(new_pwd);
                hash_value = hmac(msr, new_pwd, msr->txcfg->crypto_key_len, (unsigned char *) link+1, strlen((char*)link)-1);
            }

        }
        else {
            char *relative_link = NULL;
            char *filename = NULL;
            char *relative_path = NULL;
            char *relative_uri = NULL;

            filename = file_basename(msr->mp, msr->r->parsed_uri.path);

            if(filename == NULL || (strlen(msr->r->parsed_uri.path) - strlen(filename) < 0))
                return NULL;

            relative_path = apr_pstrndup(msr->mp, msr->r->parsed_uri.path, strlen(msr->r->parsed_uri.path) - strlen(filename));
            relative_uri = apr_pstrcat(msr->mp, relative_path, link, NULL);

            relative_link = relative_uri+1;

            if (msr->txcfg->debuglog_level >= 4)
                msr_log(msr, 4, "Signing data [%s] size %d", relative_link, strlen(relative_link));

            if(msr->txcfg->crypto_key_add == HASH_KEYONLY)
                hash_value = hmac(msr, msr->txcfg->crypto_key, msr->txcfg->crypto_key_len, (unsigned char *) relative_link, strlen((char*)relative_link));

            if(msr->txcfg->crypto_key_add == HASH_SESSIONID)  {
                if(strlen(msr->sessionid) == 0)   {
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
                    const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->client_ip);
#else
                    const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->remote_ip);
#endif
                    if (msr->txcfg->debuglog_level >= 4)
                        msr_log(msr, 4, "Session id is empty. Using REMOTE_IP");
                    msr->txcfg->crypto_key_len = strlen(new_pwd);
                    hash_value = hmac(msr, new_pwd, msr->txcfg->crypto_key_len, (unsigned char *) relative_link, strlen((char*)relative_link));
                } else {
                    const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->sessionid);
                    if (msr->txcfg->debuglog_level >= 4)
                        msr_log(msr, 4, "Using session id [%s]", msr->sessionid);
                    msr->txcfg->crypto_key_len = strlen(new_pwd);
                    hash_value = hmac(msr, new_pwd, msr->txcfg->crypto_key_len, (unsigned char *) relative_link, strlen((char*)relative_link));
                }
            }

            if(msr->txcfg->crypto_key_add == HASH_REMOTEIP)   {
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
                const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->client_ip);
#else
                const char *new_pwd = apr_psprintf(msr->mp,"%s%s", msr->txcfg->crypto_key, msr->r->connection->remote_ip);
#endif
                msr->txcfg->crypto_key_len = strlen(new_pwd);
                hash_value = hmac(msr, new_pwd, msr->txcfg->crypto_key_len, (unsigned char *) relative_link, strlen((char*)relative_link));
            }
        }

    if(hash_value == NULL) return NULL;

    if(type == HASH_ONLY)
        return hash_value;

    qm = strchr((char*)link,'?');
    if(qm == NULL){
        mac_link= (char*)apr_psprintf(msr->mp, "%s?%s=%s", link, msr->txcfg->crypto_param_name, (char *)hash_value);
    }
    else{
        mac_link= (char*)apr_psprintf(msr->mp, "%s&%s=%s", link, msr->txcfg->crypto_param_name, (char*)hash_value);
    }

    return mac_link;
}

/**
 * \brief Modify Location in case of status 302 and 301
 *
 * \param msr ModSecurity transaction resource
 *
 * \retval 1 On Success
 * \retval 0 on fail
 */
int modify_response_header(modsec_rec *msr) {
    char *mac_link = NULL;
    const char *location = NULL;
    int rc = 0;

    if(msr == NULL) return 0;

    if (msr->txcfg->debuglog_level >= 9)
        msr_log(msr, 4, "HTTP status (%d)", msr->response_status);

    if(msr->response_status != HTTP_MOVED_TEMPORARILY &&
            msr->response_status != HTTP_MOVED_PERMANENTLY)    {
        if (msr->txcfg->debuglog_level >= 9)
            msr_log(msr, 4, "Skipping status other than 302 an 301");
        return 0;
    }

    location = apr_table_get(msr->r->headers_out, "Location");

    if(location == NULL || strlen(location) == 0)
        return 0;

    if (msr->txcfg->debuglog_level >= 9)
        msr_log(msr, 4, "Processing reponse header location [%s]", location);

    if(msr->txcfg->crypto_hash_location_rx == 1) {
        rc = do_hash_method(msr, (char *)location, HASH_URL_LOCATION_HASH_RX);

        if(rc > 0)  {
            mac_link = NULL;
            mac_link = do_hash_link(msr, (char *)location, FULL_LINK);
        }   else    {
            return 0;
        }

    } else if(msr->txcfg->crypto_hash_location_pm == 1) {
        rc = do_hash_method(msr, (char *)location, HASH_URL_LOCATION_HASH_PM);

        if(rc > 0)  {
            mac_link = NULL;
            mac_link = do_hash_link(msr, (char *)location, FULL_LINK);
        }   else    {
            return 0;
        }

    }

    if(mac_link == NULL)
        return 0;

    if (msr->txcfg->debuglog_level >= 9)
        msr_log(msr, 4, "Setting new reponse header location [%s]", mac_link);

    if(rc > 0)  {
        apr_table_unset(msr->r->headers_out,"Location");
        apr_table_set(msr->r->headers_out, "Location",(char*)apr_psprintf(msr->mp,"%s", mac_link));
    }

    return 1;
}
