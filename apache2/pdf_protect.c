/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2007 Breach Security, Inc. (http://www.breach.com/)
 *
 * You should have received a copy of the licence along with this
 * program (stored in the file "LICENSE"). If the file is missing,
 * or if you have any other questions related to the licence, please
 * write to Breach Security, Inc. at support@breach.com.
 *
 */
#include "modsecurity.h"
#include "apache2.h"
#include "pdf_protect.h"

#include <ctype.h>
#include "apr_sha1.h"

#define DEFAULT_TIMEOUT         10
#define DEFAULT_TOKEN_NAME      "PDFPTOKEN"
#define ATTACHMENT_MIME_TYPE    "application/octet-stream"
#define NOTE_TWEAK_HEADERS      "pdfp-note-tweakheaders"
#define NOTE_DONE               "pdfp-note-done"
#define REDIRECT_STATUS         HTTP_TEMPORARY_REDIRECT
#define DISPOSITION_VALUE       "attachment;"

// TODO We need ID and REV values for the PDF XSS alert.

// TODO It would be nice if the user could choose the ID/REV/SEVERITY/MESSAGE, etc.

static char *encode_sha1_base64(apr_pool_t *mp, const char *data) {
    unsigned char digest[APR_SHA1_DIGESTSIZE];
    apr_sha1_ctx_t context;

    /* Calculate the hash first. */
    apr_sha1_init(&context);
    apr_sha1_update(&context, data, strlen(data));
    apr_sha1_final(digest, &context);

    /* Now transform with transport-friendly hex encoding. */
    return bytes2hex(mp, digest, APR_SHA1_DIGESTSIZE);
}

static char *create_hash(modsec_rec *msr,
    const char *time_string)
{
    const char *content = NULL;

    if (msr->txcfg->pdfp_secret == NULL) {
        msr_log(msr, 1, "PdfProtect: Unable to generate hash. Please configure SecPdfProtectSecret.");
        return NULL;
    }
    
    /* Our protection token is made out of the client's IP
     * address, the secret key, and the token expiry time.
     */
    content = apr_pstrcat(msr->mp, msr->remote_addr, msr->txcfg->pdfp_secret,
        time_string, NULL);
    if (content == NULL) return NULL;
    
    return encode_sha1_base64(msr->mp, content);
}

/**
 *
 */
static char *create_token(modsec_rec *msr) {
    unsigned int current_time;
    const char *time_string = NULL;
    const char *hash = NULL;
    int timeout = DEFAULT_TIMEOUT;

    if (msr->txcfg->pdfp_timeout != -1) {
        timeout = msr->txcfg->pdfp_timeout;
    }
    
    current_time = apr_time_sec(apr_time_now());
    time_string = apr_psprintf(msr->mp, "%d", current_time + timeout);
    if (time_string == NULL) return NULL;
    
    hash = create_hash(msr, time_string);
    if (hash == NULL) return NULL;
    
    return apr_pstrcat(msr->mp, hash, "|", time_string, NULL);
}

/**
 *
 */
static char *construct_new_uri(modsec_rec *msr) {
    const char *token_parameter = NULL;
    const char *new_uri = NULL;
    const char *token = NULL;
    const char *token_name = DEFAULT_TOKEN_NAME;

    token = create_token(msr);
    if (token == NULL) return NULL;

    if (msr->txcfg->pdfp_token_name != NULL) {
        token_name = msr->txcfg->pdfp_token_name;
    }
    
    token_parameter = apr_pstrcat(msr->mp, token_name, "=", token, NULL);
    if (token_parameter == NULL) return NULL;
    
    if (msr->r->args == NULL) { /* No other parameters. */
        new_uri = apr_pstrcat(msr->mp, msr->r->uri, "?", token_parameter, "#PDFP", NULL);
    } else { /* Preserve existing paramters. */
        new_uri = apr_pstrcat(msr->mp, msr->r->uri, "?", msr->r->args, "&",
            token_parameter, "#PDFP", NULL);
    }
    
    return (char *)new_uri;
}

/**
 *
 */
static char *extract_token(modsec_rec *msr) {
    char *search_string = NULL;
    char *p = NULL, *t = NULL;
    const char *token_name = DEFAULT_TOKEN_NAME;
    
    if ((msr->r == NULL)||(msr->r->args == NULL)) {
        return NULL;
    }

    if (msr->txcfg->pdfp_token_name != NULL) {
        token_name = msr->txcfg->pdfp_token_name;
    }
    
    search_string = apr_pstrcat(msr->mp, msr->txcfg->pdfp_token_name, "=", NULL);
    if (search_string == NULL) return NULL;
    
    p = strstr(msr->r->args, search_string);
    if (p == NULL) return NULL;

    t = p = p + strlen(search_string);
    while ((*t != '\0')&&(*t != '&')) t++;
    
    return apr_pstrmemdup(msr->mp, p, t - p);
} 

/**
 *
 */
static int validate_time_string(const char *time_string) {
    char *p = (char *)time_string;

    while(*p != '\0') {
        if (!isdigit(*p)) return 0;
        p++;
    }

    return 1;
}

/**
 *
 */
static int verify_token(modsec_rec *msr, const char *token, char **error_msg) {
    unsigned int current_time, expiry_time;
    const char *time_string = NULL;
    const char *given_hash = NULL;
    const char *hash = NULL;
    const char *p = NULL;

    if (error_msg == NULL) return 0;
    *error_msg = NULL;
    
    /* Split token into its parts - hash and expiry time. */
    p = strstr(token, "|");
    if (p == NULL) return 0;
    
    given_hash = apr_pstrmemdup(msr->mp, token, p - token);
    time_string = p + 1;
    if (!validate_time_string(time_string)) {
        *error_msg = apr_psprintf(msr->mp, "PdfProtect: Invalid time string: %s",
            log_escape_nq(msr->mp, time_string));
        return 0;
    }
    expiry_time = atoi(time_string);

    /* Check the hash. */
    hash = create_hash(msr, time_string);
    if (strcmp(given_hash, hash) != 0) {
        *error_msg = apr_psprintf(msr->mp, "PdfProtect: Invalid hash: %s (expected %s)",
            log_escape_nq(msr->mp, given_hash), log_escape_nq(msr->mp, hash));
        return 0;
    }

    /* Check time. */
    current_time = apr_time_sec(apr_time_now());
    if (current_time > expiry_time) {
        *error_msg = apr_psprintf(msr->mp, "PdfProtect: Token has expired.");
        return 0;
    }
    
    return 1;
}

/**
 *
 */
apr_status_t pdfp_output_filter(ap_filter_t *f, apr_bucket_brigade *bb_in) {
    modsec_rec *msr = (modsec_rec *)f->ctx;

    if (msr == NULL) {
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, f->r->server,
            "ModSecurity: Internal Error: Unable to retrieve context in PDF output filter.");

        ap_remove_output_filter(f);

        return send_error_bucket(msr, f, HTTP_INTERNAL_SERVER_ERROR);
    }

    if (msr->txcfg->pdfp_enabled == 1) {
        // TODO Should we look at err_headers_out too?
        const char *h_content_type = apr_table_get(f->r->headers_out, "Content-Type");

        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "PdfProtect: r->content_type=%s, header C-T=%s",
                log_escape_nq(msr->mp, f->r->content_type),
                log_escape_nq(msr->mp, h_content_type));
        }

        /* Have we been asked to tweak the headers? */
        if (apr_table_get(f->r->notes, NOTE_TWEAK_HEADERS) != NULL) {
            /* Yes! */
            apr_table_set(f->r->headers_out, "Content-Disposition", DISPOSITION_VALUE);
            f->r->content_type = ATTACHMENT_MIME_TYPE;
        }

        /* Check if we've already done the necessary work in
         * the first phase.
         */
        if (apr_table_get(f->r->notes, NOTE_DONE) != NULL) {
            /* We have, so return straight away. */
            ap_remove_output_filter(f);
            return ap_pass_brigade(f->next, bb_in);
        }

        /* Proceed to detect dynamically-generated PDF files. */

        // TODO application/x-pdf, application/vnd.fdf, application/vnd.adobe.xfdf,
        // application/vnd.adobe.xdp+xml, application/vnd.adobe.xfd+xml, application/vnd.pdf
        // application/acrobat, text/pdf, text/x-pdf ???
        if (((f->r->content_type != NULL)&&(strcasecmp(f->r->content_type, "application/pdf") == 0))
            || ((h_content_type != NULL)&&(strcasecmp(h_content_type, "application/pdf") == 0)))
        {
            request_rec *r = f->r;
            const char *token = NULL;

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "PdfProtect: Detected a dynamically-generated PDF in %s",
                    log_escape_nq(msr->mp, r->uri));
            }

            /* If we are configured with ForcedDownload protection method then we
             * can do our thing here and finish early.
             */
            if (msr->txcfg->pdfp_method == PDF_PROTECT_METHOD_FORCED_DOWNLOAD) {
                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "PdfProtect: Forcing download of a dynamically "
                        "generated PDF file.");
                }

                apr_table_set(f->r->headers_out, "Content-Disposition", DISPOSITION_VALUE);
                f->r->content_type = ATTACHMENT_MIME_TYPE;

                ap_remove_output_filter(f);

                return ap_pass_brigade(f->next, bb_in);
            }

            /* If we are here that means TokenRedirection is the desired protection method. */

            /* Is this a non-GET request? */
            if ((f->r->method_number != M_GET)&&
                ((msr->txcfg->pdfp_only_get == 1)||(msr->txcfg->pdfp_only_get == -1))
            ) {
                /* This is a non-GET request and we have been configured
                 * not to intercept it. So we are going to tweak the headers
                 * to force download.
                 */
                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "PdfProtect: Forcing download of a dynamically "
                        "generated PDF file and non-GET method.");
                }

                apr_table_set(f->r->headers_out, "Content-Disposition", DISPOSITION_VALUE);
                f->r->content_type = ATTACHMENT_MIME_TYPE;

                ap_remove_output_filter(f);

                return ap_pass_brigade(f->next, bb_in);
            }

            /* Locate the protection token. */
            token = extract_token(msr);
    
            if (token == NULL) { /* No token. */
                char *new_uri = NULL;

                /* Create a new URI with the protection token inside. */
                new_uri = construct_new_uri(msr);
                if (new_uri != NULL) {
                    /* Redirect user to the new URI. */
                    if (msr->txcfg->debuglog_level >= 9) {
                        msr_log(msr, 9, "PdfProtect: PDF request without a token - "
                            "redirecting to %s.", log_escape_nq(msr->mp, new_uri));
                    }

                    apr_table_set(r->headers_out, "Location", new_uri);

                    return send_error_bucket(msr, f, REDIRECT_STATUS);
                }
            } else { /* Token found. */
                char *my_error_msg = NULL;

                /* Verify the token is valid. */
                
                if (verify_token(msr, token, &my_error_msg)) { /* Valid. */
                    /* Do nothing - serve the PDF file. */
                    if (msr->txcfg->debuglog_level >= 9) {
                        msr_log(msr, 9, "PdfProtect: PDF request with a valid token "
                            "- serving PDF file normally.");
                    }

                    /* Fall through. */
                } else { /* Not valid. */
                    /* The token is not valid. We will tweak the response
                     * to prevent the PDF file from opening in the browser.
                     */
                    if (msr->txcfg->debuglog_level >= 4) {
                        msr_log(msr, 9, "PdfProtect: PDF request with an invalid token "
                            "- serving file as attachment.");
                    }

                    apr_table_set(r->headers_out, "Content-Disposition", DISPOSITION_VALUE);
                    r->content_type = ATTACHMENT_MIME_TYPE;
            
                    /* Fall through. */
                }
            }
        }
    }    

    ap_remove_output_filter(f);

    return ap_pass_brigade(f->next, bb_in);
}

/**
 *
 */
int pdfp_check(modsec_rec *msr) {
    const char *token = NULL;
    directory_config *cfg = NULL;
    char *uri = NULL;
    char *p = NULL;

    if (msr->txcfg->pdfp_enabled != 1) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "PdfProtect: Not enabled here.");
        }

        return 0;
    }

    if (msr->txcfg->pdfp_method != PDF_PROTECT_METHOD_TOKEN_REDIRECTION) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "PdfProtect: Configured with ForcedDownload as protection method, "
                "skipping detection on the inbound.");
        }

        return 0;
    }

    /* Then determine whether we need to act at
     * all. If the request is not for a PDF file
     * return straight away.
     */

    if (msr->r->uri == NULL) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "PdfProtect: Unable to inspect URI because it is NULL.");
        }

        return -1; /* Error. */
    }

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "PdfProtect: URI=%s, filename=%s, QUERY_STRING=%s.",
            log_escape_nq(msr->mp, msr->r->uri), log_escape_nq(msr->mp, msr->r->filename),
            log_escape_nq(msr->mp, msr->r->args));
    }

    uri = apr_pstrdup(msr->mp, msr->r->uri);
    if (uri == NULL) return -1; /* Error. */
    ap_str_tolower(uri);

    /* Attempt to figure out if this is a request for a PDF file. We are
     * going to be liberal and look for the extension anywhere in the URI,
     * not just at the end.
     */
    p = strstr(uri, ".pdf");
    if (p == NULL) {
        /* We do not think this is a PDF file. */
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4,  "PdfProtect: No indication in the URI this is a "
                "request for a PDF file.");
        }

        return 0;
    }

    /* Ignore request methods other than GET and HEAD if
     * configured to do so.
     */
    if ((msr->r->method_number != M_GET)&&(cfg->pdfp_only_get != 0)) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "PdfProtect: Not intercepting a GET/HEAD request "
            "(method=%s/%d).", log_escape_nq(msr->mp, msr->r->method), msr->r->method_number);
        }

        return 0;
    }

    /* We make a note for ourselves that we've already handled
     * the request.
     */
    apr_table_set(msr->r->notes, NOTE_DONE, "1");

    /* Locate the protection token. */
    token = extract_token(msr);
    
    if (token == NULL) { /* No token. */
        char *new_uri = NULL;

        /* Create a new URI with the protection token inside. */
        new_uri = construct_new_uri(msr);
        if (new_uri == NULL) return DECLINED;
        
        /* Redirect user to the new URI. */
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "PdfProtect: PDF request without a token - redirecting to %s.",
                log_escape_nq(msr->mp, new_uri));
        }

        apr_table_set(msr->r->headers_out, "Location", new_uri);

        return REDIRECT_STATUS;
    } else { /* Token found. */
        char *my_error_msg = NULL;

        /* Verify the token is valid. */
        if (verify_token(msr, token, &my_error_msg)) { /* Valid. */
            /* Do nothing - serve the PDF file. */
            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "PdfProtect: PDF request with a valid token - "
                    "serving PDF file normally.");
            }

            return 0;
        } else { /* Not valid. */
            /* The token is not valid. We will tweak the response
             * to prevent the PDF file from opening in the browser.
             */

            // TODO Log alert

            /* Changing response headers before response generation phase takes
             * place is not really reliable. Although we do this we also make
             * a note for ourselves (in the output filter) to check the headers
             * again just before they are sent back to the end user.
             */
            apr_table_set(msr->r->headers_out, "Content-Disposition", DISPOSITION_VALUE);
            msr->r->content_type = ATTACHMENT_MIME_TYPE;
            apr_table_set(msr->r->notes, NOTE_TWEAK_HEADERS, "1");
            
            /* Proceed with response (PDF) generation. */
            return 0;
        }
    }

    return 0;
}
