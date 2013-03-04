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

#include "modsecurity.h"
#include <ctype.h>
#include <sys/stat.h>

#include "msc_multipart.h"
#include "msc_util.h"
#include "msc_parsers.h"

void validate_quotes(modsec_rec *msr, char *data)  {
    int i, len;

    if(msr == NULL)
        return;

    if(msr->mpd == NULL)
        return;

    if(data == NULL)
        return;

    len = strlen(data);

    for(i = 0; i < len; i++)   {

        if(data[i] == '\'') {
            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Multipart: Invalid quoting detected: %s length %d bytes",
                        log_escape_nq(msr->mp, data), len);
            }
            msr->mpd->flag_invalid_quoting = 1;
        }
    }
}

#if 0
static char *multipart_construct_filename(modsec_rec *msr) {
    char c, *p, *q = msr->mpd->mpp->filename;
    char *filename;

    /* find the last backward slash and consider the
     * filename to be only what's right from it
     */
    p = strrchr(q, '\\');
    if (p != NULL) q = p + 1;

    /* do the same for the forward slash */
    p = strrchr(q, '/');
    if (p != NULL) q = p + 1;

    /* allow letters, digits and dots, replace
     * everything else with underscores
     */
    p = filename = apr_pstrdup(msr->mp, q);
    while((c = *p) != 0) {
        if (!( isalnum(c) || (c == '.') )) *p = '_';
        p++;
    }

    return filename;
}
#endif

/**
 *
 */
static int multipart_parse_content_disposition(modsec_rec *msr, char *c_d_value) {
    char *p = NULL, *t = NULL;

    /* accept only what we understand */
    if (strncmp(c_d_value, "form-data", 9) != 0) {
        return -1;
    }

    /* see if there are any other parts to parse */

    p = c_d_value + 9;
    while((*p == '\t') || (*p == ' ')) p++;
    if (*p == '\0') return 1; /* this is OK */

    if (*p != ';') return -2;
    p++;

    /* parse the appended parts */

    while(*p != '\0') {
        char *name = NULL, *value = NULL, *start = NULL;

        /* go over the whitespace */
        while((*p == '\t') || (*p == ' ')) p++;
        if (*p == '\0') return -3;

        start = p;
        while((*p != '\0') && (*p != '=') && (*p != '\t') && (*p != ' ')) p++;
        if (*p == '\0') return -4;

        name = apr_pstrmemdup(msr->mp, start, (p - start));

        while((*p == '\t') || (*p == ' ')) p++;
        if (*p == '\0') return -5;

        if (*p != '=') return -13;
        p++;

        while((*p == '\t') || (*p == ' ')) p++;
        if (*p == '\0') return -6;

        /* Accept both quotes as some backends will accept them, but
         * technically "'" is invalid and so flag_invalid_quoting is
         * set so the user can deal with it in the rules if they so wish.
         */

        if ((*p == '"') || (*p == '\'')) {
            /* quoted */
            char quote = *p;

            if (quote == '\'') {
                msr->mpd->flag_invalid_quoting = 1;
            }

            p++;
            if (*p == '\0') return -7;

            start = p;
            value = apr_pstrdup(msr->mp, p);
            t = value;

            while(*p != '\0') {
                if (*p == '\\') {
                    if (*(p + 1) == '\0') {
                        /* improper escaping */
                        return -8;
                    }
                    /* only quote and \ can be escaped */
                    if ((*(p + 1) == quote) || (*(p + 1) == '\\')) {
                        p++;
                    }
                    else {
                        /* improper escaping */

                        /* We allow for now because IE sends
                         * improperly escaped content and there's
                         * nothing we can do about it.
                         *
                         * return -9;
                         */
                    }
                }
                else if (*p == quote) {
                    *t = '\0';
                    break;
                }

                *(t++) = *(p++);
            }
            if (*p == '\0') return -10;

            p++; /* go over the quote at the end */

        } else {
            /* not quoted */

            start = p;
            while((*p != '\0') && (is_token_char(*p))) p++;
            value = apr_pstrmemdup(msr->mp, start, (p - start));
        }

        /* evaluate part */

        if (strcmp(name, "name") == 0) {

            validate_quotes(msr, value);

            msr->multipart_name = apr_pstrdup(msr->mp, value);

            if (msr->mpd->mpp->name != NULL) {
                msr_log(msr, 4, "Multipart: Warning: Duplicate Content-Disposition name: %s",
                    log_escape_nq(msr->mp, value));
                return -14;
            }
            msr->mpd->mpp->name = value;

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Multipart: Content-Disposition name: %s",
                    log_escape_nq(msr->mp, value));
            }
        }
        else
        if (strcmp(name, "filename") == 0) {

            validate_quotes(msr, value);

            msr->multipart_filename = apr_pstrdup(msr->mp, value);

            if (msr->mpd->mpp->filename != NULL) {
                msr_log(msr, 4, "Multipart: Warning: Duplicate Content-Disposition filename: %s",
                    log_escape_nq(msr->mp, value));
                return -15;
            }
            msr->mpd->mpp->filename = value;

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Multipart: Content-Disposition filename: %s",
                    log_escape_nq(msr->mp, value));
            }
        }
        else return -11;

        if (*p != '\0') {
            while((*p == '\t') || (*p == ' ')) p++;
            /* the next character must be a zero or a semi-colon */
            if (*p == '\0') return 1; /* this is OK */
            if (*p != ';') {
                p--;
                if(*p == '\'' || *p == '\"') {
                    if (msr->txcfg->debuglog_level >= 9) {
                        msr_log(msr, 9, "Multipart: Invalid quoting detected: %s length %d bytes",
                                log_escape_nq(msr->mp, p), strlen(p));
                    }
                    msr->mpd->flag_invalid_quoting = 1;
                }
                p++;
                return -12;
            }
            p++; /* move over the semi-colon */
        }

        /* loop will stop when (*p == '\0') */
    }

    return 1;
}

/**
 *
 */
static int multipart_process_part_header(modsec_rec *msr, char **error_msg) {
    int i, len, rc;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    /* Check for nul bytes. */
    len = MULTIPART_BUF_SIZE - msr->mpd->bufleft;
    for(i = 0; i < len; i++) {
        if (msr->mpd->buf[i] == '\0') {
            *error_msg = apr_psprintf(msr->mp, "Multipart: Nul byte in part headers.");
            return -1;
        }
    }

    /* The buffer is data so increase the data length counter. */
    msr->msc_reqbody_no_files_length += (MULTIPART_BUF_SIZE - msr->mpd->bufleft);

    if (len > 1) {
        if (msr->mpd->buf[len - 2] == '\r') {
            msr->mpd->flag_crlf_line = 1;
        } else {
            msr->mpd->flag_lf_line = 1;
        }
    } else {
        msr->mpd->flag_lf_line = 1;
    }

    /* Is this an empty line? */
    if (   ((msr->mpd->buf[0] == '\r')
                &&(msr->mpd->buf[1] == '\n')
                &&(msr->mpd->buf[2] == '\0') )
            || ((msr->mpd->buf[0] == '\n')
                &&(msr->mpd->buf[1] == '\0') ) )
    { /* Empty line. */
        char *header_value = NULL;

        header_value = (char *)apr_table_get(msr->mpd->mpp->headers, "Content-Disposition");
        if (header_value == NULL) {
            *error_msg = apr_psprintf(msr->mp, "Multipart: Part missing Content-Disposition header.");
            return -1;
        }

        rc = multipart_parse_content_disposition(msr, header_value);
        if (rc < 0) {
            *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid Content-Disposition header (%d): %s.",
                    rc, log_escape_nq(msr->mp, header_value));
            return -1;
        }

        if (msr->mpd->mpp->name == NULL) {
            *error_msg = apr_psprintf(msr->mp, "Multipart: Content-Disposition header missing name field.");
            return -1;
        }

        if (msr->mpd->mpp->filename != NULL) {
            /* Some parsers use crude methods to extract the name and filename
             * values from the C-D header. We need to check for the case where they
             * didn't understand C-D but we did.
             */
            if (strstr(header_value, "filename=") == NULL) {
                *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid Content-Disposition header (filename).");
                return -1;
            }

            msr->mpd->mpp->type = MULTIPART_FILE;
        } else {
            msr->mpd->mpp->type = MULTIPART_FORMDATA;
        }

        msr->mpd->mpp_state = 1;
        msr->mpd->mpp->last_header_name = NULL;
    } else {
        /* Header line. */

        if (isspace(msr->mpd->buf[0])) {
            char *header_value, *new_value, *data;

            /* header folding, add data to the header we are building */
            msr->mpd->flag_header_folding = 1;

            /* RFC-2557 states header folding is SP / HTAB, but PHP and
             * perhaps others will take any whitespace.  So, we accept,
             * but with a flag set.
             */
            if ((msr->mpd->buf[0] != '\t') && (msr->mpd->buf[0] != ' ')) {
                msr->mpd->flag_invalid_header_folding = 1;
            }

            if (msr->mpd->mpp->last_header_name == NULL) {
                /* we are not building a header at this moment */
                *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid part header (folding error).");
                return -1;
            }

            /* locate the beginning of data */
            data = msr->mpd->buf;
            while(isspace(*data)) {
                /* Flag invalid header folding if an invalid RFC-2557 character is used anywhere
                 * in the folding prefix.
                 */
                if ((*data != '\t') && (*data != ' ')) {
                    msr->mpd->flag_invalid_header_folding = 1;
                }
                data++;
            }

            new_value = apr_pstrdup(msr->mp, data);
            remove_lf_crlf_inplace(new_value);

            /* update the header value in the table */
            header_value = (char *)apr_table_get(msr->mpd->mpp->headers, msr->mpd->mpp->last_header_name);
            new_value = apr_pstrcat(msr->mp, header_value, " ", new_value, NULL);
            apr_table_set(msr->mpd->mpp->headers, msr->mpd->mpp->last_header_name, new_value);

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Multipart: Continued folder header \"%s\" with \"%s\"",
                    log_escape(msr->mp, msr->mpd->mpp->last_header_name),
                    log_escape(msr->mp, data));
            }

            if (strlen(new_value) > MULTIPART_BUF_SIZE) {
                *error_msg = apr_psprintf(msr->mp, "Multipart: Part header too long.");
                return -1;
            }
        } else {
            char *header_name, *header_value, *data;

            /* new header */

            data = msr->mpd->buf;
            while((*data != ':') && (*data != '\0')) data++;
            if (*data == '\0') {
                *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid part header (colon missing): %s.",
                    log_escape_nq(msr->mp, msr->mpd->buf));
                return -1;
            }

            /* extract header name */
            header_name = apr_pstrmemdup(msr->mp, msr->mpd->buf, (data - msr->mpd->buf));
            if (data == msr->mpd->buf) {
                *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid part header (header name missing).");

                 return -1;
            }

            /* extract the value value */
            data++;
            while((*data == '\t') || (*data == ' ')) data++;
            header_value = apr_pstrdup(msr->mp, data);
            remove_lf_crlf_inplace(header_value);

            /* error if the name already exists */
            if (apr_table_get(msr->mpd->mpp->headers, header_name) != NULL) {
                *error_msg = apr_psprintf(msr->mp, "Multipart: Duplicate part header: %s.",
                    log_escape_nq(msr->mp, header_name));
                return -1;
            }

            apr_table_setn(msr->mpd->mpp->headers, header_name, header_value);
            msr->mpd->mpp->last_header_name = header_name;

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Multipart: Added part header \"%s\" \"%s\"",
                    log_escape(msr->mp, header_name),
                    log_escape(msr->mp, header_value));
            }
        }
    }

    return 1;
}

/**
 *
 */
static int multipart_process_part_data(modsec_rec *msr, char **error_msg) {
    char *p = msr->mpd->buf + (MULTIPART_BUF_SIZE - msr->mpd->bufleft);
    char localreserve[2] = { '\0', '\0' }; /* initialized to quiet warning */
    int bytes_reserved = 0;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    /* Preserve some bytes for later. */
    if (   ((MULTIPART_BUF_SIZE - msr->mpd->bufleft) >= 1)
        && (*(p - 1) == '\n') )
    {
        if (   ((MULTIPART_BUF_SIZE - msr->mpd->bufleft) >= 2)
            && (*(p - 2) == '\r') )
        {
            /* Two bytes. */
            bytes_reserved = 2;
            localreserve[0] = *(p - 2);
            localreserve[1] = *(p - 1);
            msr->mpd->bufleft += 2;
            *(p - 2) = 0;
        } else {
            /* Only one byte. */
            bytes_reserved = 1;
            localreserve[0] = *(p - 1);
            localreserve[1] = 0;
            msr->mpd->bufleft += 1;
            *(p - 1) = 0;
        }
    }

    /* add data to the part we are building */
    if (msr->mpd->mpp->type == MULTIPART_FILE) {
        int extract = msr->upload_extract_files;

        /* remember where we started */
        if (msr->mpd->mpp->length == 0) {
            msr->mpd->mpp->offset = msr->mpd->buf_offset;
        }

        /* check if the file limit has been reached */
        if (extract && (msr->mpd->nfiles >= msr->txcfg->upload_file_limit)) {
            if (msr->mpd->flag_file_limit_exceeded == 0) {
                *error_msg = apr_psprintf(msr->mp,
                            "Multipart: Upload file limit exceeded "
                            "SecUploadFileLimit %d.",
                            msr->txcfg->upload_file_limit);
                msr_log(msr, 3, "%s", *error_msg);

                msr->mpd->flag_file_limit_exceeded = 1;
            }

            extract = 0;
        }

        /* only store individual files on disk if we are going
         * to keep them or if we need to have them approved later
         */
        if (extract) {
            /* first create a temporary file if we don't have it already */
            if (msr->mpd->mpp->tmp_file_fd == 0) {
                /* construct temporary file name */
                msr->mpd->mpp->tmp_file_name = apr_psprintf(msr->mp, "%s/%s-%s-file-XXXXXX",
                    msr->txcfg->tmp_dir, current_filetime(msr->mp), msr->txid);
                msr->mpd->mpp->tmp_file_fd = msc_mkstemp_ex(msr->mpd->mpp->tmp_file_name, msr->txcfg->upload_filemode);

                /* do we have an opened file? */
                if (msr->mpd->mpp->tmp_file_fd < 0) {
                    *error_msg = apr_psprintf(msr->mp, "Multipart: Failed to create file: %s",
                        log_escape_nq(msr->mp, msr->mpd->mpp->tmp_file_name));
                    return -1;
                }

                /* keep track of the files count */
                msr->mpd->nfiles++;

                if (msr->txcfg->debuglog_level >= 4) {
                    msr_log(msr, 4,
                        "Multipart: Created temporary file %d (mode %04o): %s",
                        msr->mpd->nfiles,
                        (unsigned int)msr->txcfg->upload_filemode,
                        log_escape_nq(msr->mp, msr->mpd->mpp->tmp_file_name));
                }
            }

            /* write the reserve first */
            if (msr->mpd->reserve[0] != 0) {
                if (write(msr->mpd->mpp->tmp_file_fd, &msr->mpd->reserve[1], msr->mpd->reserve[0]) != msr->mpd->reserve[0]) {
                    *error_msg = apr_psprintf(msr->mp, "Multipart: writing to \"%s\" failed",
                        log_escape(msr->mp, msr->mpd->mpp->tmp_file_name));
                    return -1;
                }

                msr->mpd->mpp->tmp_file_size += msr->mpd->reserve[0];
                msr->mpd->mpp->length += msr->mpd->reserve[0];
            }

            /* write data to the file */
            if (write(msr->mpd->mpp->tmp_file_fd, msr->mpd->buf, MULTIPART_BUF_SIZE - msr->mpd->bufleft)
                != (MULTIPART_BUF_SIZE - msr->mpd->bufleft))
            {
                *error_msg = apr_psprintf(msr->mp, "Multipart: writing to \"%s\" failed",
                    log_escape(msr->mp, msr->mpd->mpp->tmp_file_name));
                return -1;
            }

            msr->mpd->mpp->tmp_file_size += (MULTIPART_BUF_SIZE - msr->mpd->bufleft);
            msr->mpd->mpp->length += (MULTIPART_BUF_SIZE - msr->mpd->bufleft);
        } else {
            /* just keep track of the file size */
            msr->mpd->mpp->tmp_file_size += (MULTIPART_BUF_SIZE - msr->mpd->bufleft) + msr->mpd->reserve[0];
            msr->mpd->mpp->length += (MULTIPART_BUF_SIZE - msr->mpd->bufleft)  + msr->mpd->reserve[0];
        }
    }
    else if (msr->mpd->mpp->type == MULTIPART_FORMDATA) {
        value_part_t *value_part = apr_pcalloc(msr->mp, sizeof(value_part_t));

        /* The buffer contains data so increase the data length counter. */
        msr->msc_reqbody_no_files_length += (MULTIPART_BUF_SIZE - msr->mpd->bufleft) + msr->mpd->reserve[0];

        /* add this part to the list of parts */

        /* remember where we started */
        if (msr->mpd->mpp->length == 0) {
            msr->mpd->mpp->offset = msr->mpd->buf_offset;
        }

        if (msr->mpd->reserve[0] != 0) {
            value_part->data = apr_palloc(msr->mp, (MULTIPART_BUF_SIZE - msr->mpd->bufleft) + msr->mpd->reserve[0]);
            memcpy(value_part->data, &(msr->mpd->reserve[1]), msr->mpd->reserve[0]);
            memcpy(value_part->data + msr->mpd->reserve[0], msr->mpd->buf, (MULTIPART_BUF_SIZE - msr->mpd->bufleft));

            value_part->length = (MULTIPART_BUF_SIZE - msr->mpd->bufleft) + msr->mpd->reserve[0];
            msr->mpd->mpp->length += value_part->length;
        } else {
            value_part->length = (MULTIPART_BUF_SIZE - msr->mpd->bufleft);
            value_part->data = apr_pstrmemdup(msr->mp, msr->mpd->buf, value_part->length);
            msr->mpd->mpp->length += value_part->length;
        }

        *(value_part_t **)apr_array_push(msr->mpd->mpp->value_parts) = value_part;

        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "Multipart: Added data to variable: %s",
                log_escape_nq_ex(msr->mp, value_part->data, value_part->length));
        }
    }
    else {
        *error_msg = apr_psprintf(msr->mp, "Multipart: unknown part type %d", msr->mpd->mpp->type);
        return -1;
    }

    /* store the reserved bytes to the multipart
     * context so that they don't get lost
     */
    if (bytes_reserved) {
        msr->mpd->reserve[0] = bytes_reserved;
        msr->mpd->reserve[1] = localreserve[0];
        msr->mpd->reserve[2] = localreserve[1];
        msr->mpd->buf_offset += bytes_reserved;
    }
    else {
        msr->mpd->buf_offset -= msr->mpd->reserve[0];
        msr->mpd->reserve[0] = 0;
    }

    return 1;
}

/**
 *
 */
static char *multipart_combine_value_parts(modsec_rec *msr, apr_array_header_t *value_parts) {
    value_part_t **parts = NULL;
    char *rval = apr_palloc(msr->mp, msr->mpd->mpp->length + 1);
    unsigned long int offset;
    int i;

    if (rval == NULL) return NULL;

    offset = 0;
    parts = (value_part_t **)value_parts->elts;
    for(i = 0; i < value_parts->nelts; i++) {
        if (offset + parts[i]->length <= msr->mpd->mpp->length) {
            memcpy(rval + offset, parts[i]->data, parts[i]->length);
            offset += parts[i]->length;
        }
    }
    rval[offset] = '\0';

    return rval;
}

/**
 *
 */
static int multipart_process_boundary(modsec_rec *msr, int last_part, char **error_log) {
    /* if there was a part being built finish it */
    if (msr->mpd->mpp != NULL) {
        /* close the temp file */
        if ((msr->mpd->mpp->type == MULTIPART_FILE)
            &&(msr->mpd->mpp->tmp_file_name != NULL)
            &&(msr->mpd->mpp->tmp_file_fd != 0))
        {
            close(msr->mpd->mpp->tmp_file_fd);
            msr->mpd->mpp->tmp_file_fd = -1;
        }

        if (msr->mpd->mpp->type != MULTIPART_FILE) {
            /* now construct a single string out of the parts */
            msr->mpd->mpp->value = multipart_combine_value_parts(msr, msr->mpd->mpp->value_parts);
            if (msr->mpd->mpp->value == NULL) return -1;
        }

        if (msr->mpd->mpp->name) {
            /* add the part to the list of parts */
            *(multipart_part **)apr_array_push(msr->mpd->parts) = msr->mpd->mpp;
            if (msr->mpd->mpp->type == MULTIPART_FILE) {
                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "Multipart: Added file part %pp to the list: name \"%s\" "
                        "file name \"%s\" (offset %u, length %u)",
                        msr->mpd->mpp, log_escape(msr->mp, msr->mpd->mpp->name),
                        log_escape(msr->mp, msr->mpd->mpp->filename),
                        msr->mpd->mpp->offset, msr->mpd->mpp->length);
                }
            }
            else {
                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "Multipart: Added part %pp to the list: name \"%s\" "
                        "(offset %u, length %u)", msr->mpd->mpp, log_escape(msr->mp, msr->mpd->mpp->name),
                        msr->mpd->mpp->offset, msr->mpd->mpp->length);
                }
            }
        }
        else {
            msr->mpd->flag_invalid_part = 1;
            msr_log(msr, 3, "Multipart: Skipping invalid part %pp (part name missing): "
                "(offset %u, length %u)", msr->mpd->mpp,
                msr->mpd->mpp->offset, msr->mpd->mpp->length);
        }

        msr->mpd->mpp = NULL;
    }

    if (last_part == 0) {
        /* start building a new part */
        msr->mpd->mpp = (multipart_part *)apr_pcalloc(msr->mp, sizeof(multipart_part));
        if (msr->mpd->mpp == NULL) return -1;
        msr->mpd->mpp->type = MULTIPART_FORMDATA;
        msr->mpd->mpp_state = 0;

        msr->mpd->mpp->headers = apr_table_make(msr->mp, 10);
        if (msr->mpd->mpp->headers == NULL) return -1;
        msr->mpd->mpp->last_header_name = NULL;

        msr->mpd->reserve[0] = 0;
        msr->mpd->reserve[1] = 0;
        msr->mpd->reserve[2] = 0;
        msr->mpd->reserve[3] = 0;

        msr->mpd->mpp->value_parts = apr_array_make(msr->mp, 10, sizeof(value_part_t *));
    }

    return 1;
}

static int multipart_boundary_characters_valid(char *boundary) {
    unsigned char *p = (unsigned char *)boundary;
    unsigned char c;

    if (p == NULL) return -1;

    while((c = *p) != '\0') {
        /* Control characters and space not allowed. */
        if (c < 32) {
            return 0;
        }

        /* Non-ASCII characters not allowed. */
        if (c > 126) {
            return 0;
        }

        switch(c) {
            /* Special characters not allowed. */
            case '(' :
            case ')' :
            case '<' :
            case '>' :
            case '@' :
            case ',' :
            case ';' :
            case ':' :
            case '\\' :
            case '"' :
            case '/' :
            case '[' :
            case ']' :
            case '?' :
            case '=' :
                return 0;
                break;

            default :
                /* Do nothing. */
                break;
        }

        p++;
    }

    return 1;
}

static int multipart_count_boundary_params(apr_pool_t *mp, const char *header_value) {
    char *duplicate = NULL;
    char *s = NULL;
    int count = 0;

    if (header_value == NULL) return -1;
    duplicate = apr_pstrdup(mp, header_value);
    if (duplicate == NULL) return -1;

    /* Performing a case-insensitive search. */
    strtolower_inplace((unsigned char *)duplicate);

    s = duplicate;
    while((s = strstr(s, "boundary")) != NULL) {
        s += 8;

        if (strchr(s, '=') != NULL) {
            count++;
        }
    }

    return count;
}

/**
 *
 */
int multipart_init(modsec_rec *msr, char **error_msg) {
    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    msr->mpd = (multipart_data *)apr_pcalloc(msr->mp, sizeof(multipart_data));
    if (msr->mpd == NULL) return -1;

    msr->mpd->parts = apr_array_make(msr->mp, 10, sizeof(multipart_part *));
    msr->mpd->bufleft = MULTIPART_BUF_SIZE;
    msr->mpd->bufptr = msr->mpd->buf;
    msr->mpd->buf_contains_line = 1;
    msr->mpd->mpp = NULL;

    if (msr->request_content_type == NULL) {
        msr->mpd->flag_error = 1;
        *error_msg = apr_psprintf(msr->mp, "Multipart: Content-Type header not available.");
        return -1;
    }

    if (strlen(msr->request_content_type) > 1024) {
        msr->mpd->flag_error = 1;
        *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid boundary in C-T (length).");
        return -1;
    }

    if (strncasecmp(msr->request_content_type, "multipart/form-data", 19) != 0) {
        msr->mpd->flag_error = 1;
        *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid MIME type.");
        return -1;
    }

    /* Count how many times the word "boundary" appears in the C-T header. */
    if (multipart_count_boundary_params(msr->mp, msr->request_content_type) > 1) {
        msr->mpd->flag_error = 1;
        *error_msg = apr_psprintf(msr->mp, "Multipart: Multiple boundary parameters in C-T.");
        return -1;
    }

    msr->mpd->boundary = strstr(msr->request_content_type, "boundary");
    if (msr->mpd->boundary != NULL) {
        char *p = NULL;
        char *b = NULL;
        int seen_semicolon = 0;
        int len = 0;

        /* Check for extra characters before the boundary. */
        for (p = (char *)(msr->request_content_type + 19); p < msr->mpd->boundary; p++) {
            if (!isspace(*p)) {
                if ((seen_semicolon == 0) && (*p == ';')) {
                    seen_semicolon = 1; /* It is OK to have one semicolon. */
                } else {
                    msr->mpd->flag_error = 1;
                    *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid boundary in C-T (malformed).");
                    return -1;
                }
            }
        }

        /* Have we seen the semicolon in the header? */
        if (seen_semicolon == 0) {
            msr->mpd->flag_missing_semicolon = 1;
        }

        b = strchr(msr->mpd->boundary + 8, '=');
        if (b == NULL) {
            msr->mpd->flag_error = 1;
            *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid boundary in C-T (malformed).");
            return -1;
        }

        /* Check parameter name ends well. */
        if (b != (msr->mpd->boundary + 8)) {
            /* Check all characters between the end of the boundary
             * and the = character.
             */
            for (p = msr->mpd->boundary + 8; p < b; p++) {
                if (isspace(*p)) {
                    /* Flag for whitespace after parameter name. */
                    msr->mpd->flag_boundary_whitespace = 1;
                } else {
                    msr->mpd->flag_error = 1;
                    *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid boundary in C-T (parameter name).");
                    return -1;
                }
            }
        }

        b++; /* Go over the = character. */
        len = strlen(b);

        /* Flag for whitespace before parameter value. */
        if (isspace(*b)) {
            msr->mpd->flag_boundary_whitespace = 1;
        }

        /* Is the boundary quoted? */
        if ((len >= 2) && (*b == '"') && (*(b + len - 1) == '"')) {
            /* Quoted. */
            msr->mpd->boundary = apr_pstrndup(msr->mp, b + 1, len - 2);
            if (msr->mpd->boundary == NULL) return -1;
            msr->mpd->flag_boundary_quoted = 1;
        } else {
            /* Not quoted. */

            /* Test for partial quoting. */
            if (   (*b == '"')
                || ((len >= 2) && (*(b + len - 1) == '"')) )
            {
                msr->mpd->flag_error = 1;
                *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid boundary in C-T (quote).");
                return -1;
            }

            msr->mpd->boundary = apr_pstrdup(msr->mp, b);
            if (msr->mpd->boundary == NULL) return -1;
            msr->mpd->flag_boundary_quoted = 0;
        }

        /* Case-insensitive test for the string "boundary" in the boundary. */
        if (multipart_count_boundary_params(msr->mp, msr->mpd->boundary) != 0) {
            msr->mpd->flag_error = 1;
            *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid boundary in C-T (content).");
            return -1;
        }

        /* Validate the characters used in the boundary. */
        if (multipart_boundary_characters_valid(msr->mpd->boundary) != 1) {
            msr->mpd->flag_error = 1;
            *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid boundary in C-T (characters).");
            return -1;
        }

        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "Multipart: Boundary%s: %s",
                (msr->mpd->flag_boundary_quoted ? " (quoted)" : ""),
                log_escape_nq(msr->mp, msr->mpd->boundary));
        }

        if (strlen(msr->mpd->boundary) == 0) {
            msr->mpd->flag_error = 1;
            *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid boundary in C-T (empty).");
            return -1;
        }
    }
    else { /* Could not find boundary in the C-T header. */
        msr->mpd->flag_error = 1;

        /* Test for case-insensitive boundary. Allowed by the RFC but highly unusual. */
        if (multipart_count_boundary_params(msr->mp, msr->request_content_type) > 0) {
            *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid boundary in C-T (case sensitivity).");
            return -1;
        }

        *error_msg = apr_psprintf(msr->mp, "Multipart: Boundary not found in C-T.");
        return -1;
    }

    return 1;
}

/**
 * Finalise multipart processing. This method is invoked at the end, when it
 * is clear that there is no more data to be processed.
 */
int multipart_complete(modsec_rec *msr, char **error_msg) {
    if (msr->mpd == NULL) return 1;

    if (msr->txcfg->debuglog_level >= 4) {
        if (msr->mpd->flag_data_before) {
            msr_log(msr, 4, "Multipart: Warning: seen data before first boundary.");
        }

        if (msr->mpd->flag_data_after) {
            msr_log(msr, 4, "Multipart: Warning: seen data after last boundary.");
        }

        if (msr->mpd->flag_boundary_quoted) {
            msr_log(msr, 4, "Multipart: Warning: boundary was quoted.");
        }

        if (msr->mpd->flag_boundary_whitespace) {
            msr_log(msr, 4, "Multipart: Warning: boundary whitespace in C-T header.");
        }

        if (msr->mpd->flag_header_folding) {
            msr_log(msr, 4, "Multipart: Warning: header folding used.");
        }

        if (msr->mpd->flag_crlf_line && msr->mpd->flag_lf_line) {
            msr_log(msr, 4, "Multipart: Warning: mixed line endings used (CRLF/LF).");
        }
        else if (msr->mpd->flag_lf_line) {
            msr_log(msr, 4, "Multipart: Warning: incorrect line endings used (LF).");
        }

        if (msr->mpd->flag_missing_semicolon) {
            msr_log(msr, 4, "Multipart: Warning: missing semicolon in C-T header.");
        }

        if (msr->mpd->flag_invalid_quoting) {
            msr_log(msr, 4, "Multipart: Warning: invalid quoting used.");
        }

        if (msr->mpd->flag_invalid_part) {
            msr_log(msr, 4, "Multipart: Warning: invalid part parsing.");
        }

        if (msr->mpd->flag_invalid_header_folding) {
            msr_log(msr, 4, "Multipart: Warning: invalid header folding used.");
        }
    }

    if ((msr->mpd->seen_data != 0) && (msr->mpd->is_complete == 0)) {
        if (msr->mpd->boundary_count > 0) {
            /* Check if we have the final boundary (that we haven't
             * processed yet) in the buffer.
             */
            if (msr->mpd->buf_contains_line) {
                if ( ((unsigned int)(MULTIPART_BUF_SIZE - msr->mpd->bufleft) == (4 + strlen(msr->mpd->boundary)))
                    && (*(msr->mpd->buf) == '-')
                    && (*(msr->mpd->buf + 1) == '-')
                    && (strncmp(msr->mpd->buf + 2, msr->mpd->boundary, strlen(msr->mpd->boundary)) == 0)
                    && (*(msr->mpd->buf + 2 + strlen(msr->mpd->boundary)) == '-')
                    && (*(msr->mpd->buf + 2 + strlen(msr->mpd->boundary) + 1) == '-') )
                {
                    /* Looks like the final boundary - process it. */
                    if (multipart_process_boundary(msr, 1 /* final */, error_msg) < 0) {
                        msr->mpd->flag_error = 1;
                        return -1;
                    }

                    /* The payload is complete after all. */
                    msr->mpd->is_complete = 1;
                }
            }

            if (msr->mpd->is_complete == 0) {
                *error_msg = apr_psprintf(msr->mp, "Multipart: Final boundary missing.");
                return -1;
            }
        } else {
            *error_msg = apr_psprintf(msr->mp, "Multipart: No boundaries found in payload.");
            return -1;
        }
    }

    return 1;
}

/**
 *
 */
int multipart_process_chunk(modsec_rec *msr, const char *buf,
    unsigned int size, char **error_msg)
{
    char *inptr = (char *)buf;
    unsigned int inleft = size;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (size == 0) return 1;

    msr->mpd->seen_data = 1;

    if (msr->mpd->is_complete) {
        msr->mpd->flag_data_before = 1;

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Multipart: Ignoring data after last boundary (received %u bytes)", size);
        }

        return 1;
    }

    if (msr->mpd->bufleft == 0) {
        msr->mpd->flag_error = 1;
        *error_msg = apr_psprintf(msr->mp,
            "Multipart: Internal error in process_chunk: no space left in the buffer");
        return -1;
    }

    /* here we loop through the available data, one byte at a time */
    while(inleft > 0) {
        char c = *inptr;
        int process_buffer = 0;

        if ((c == '\r') && (msr->mpd->bufleft == 1)) {
            /* we don't want to take \r as the last byte in the buffer */
            process_buffer = 1;
        } else {
            inptr++;
            inleft = inleft - 1;

            *(msr->mpd->bufptr) = c;
            msr->mpd->bufptr++;
            msr->mpd->bufleft--;
        }

        /* until we either reach the end of the line
         * or the end of our internal buffer
         */
        if ((c == '\n') || (msr->mpd->bufleft == 0) || (process_buffer)) {
            int processed_as_boundary = 0;

            *(msr->mpd->bufptr) = 0;

            /* Do we have something that looks like a boundary? */
            if (   msr->mpd->buf_contains_line
                && (strlen(msr->mpd->buf) > 3)
                && (*(msr->mpd->buf) == '-')
                && (*(msr->mpd->buf + 1) == '-') )
            {
                /* Does it match our boundary? */
                if (   (strlen(msr->mpd->buf) >= strlen(msr->mpd->boundary) + 2)
                    && (strncmp(msr->mpd->buf + 2, msr->mpd->boundary, strlen(msr->mpd->boundary)) == 0) )
                {
                    char *boundary_end = msr->mpd->buf + 2 + strlen(msr->mpd->boundary);
                    int is_final = 0;

                    /* Is this the final boundary? */
                    if ((*boundary_end == '-') && (*(boundary_end + 1)== '-')) {
                        is_final = 1;
                        boundary_end += 2;

                        if (msr->mpd->is_complete != 0) {
                            msr->mpd->flag_error = 1;
                            *error_msg = apr_psprintf(msr->mp,
                                "Multipart: Invalid boundary (final duplicate).");
                            return -1;
                        }
                    }

                    /* Allow for CRLF and LF line endings. */
                    if (   ( (*boundary_end == '\r')
                              && (*(boundary_end + 1) == '\n')
                              && (*(boundary_end + 2) == '\0') )
                        || ( (*boundary_end == '\n')
                              && (*(boundary_end + 1) == '\0') ) )
                    {
                        if (*boundary_end == '\n') {
                            msr->mpd->flag_lf_line = 1;
                        } else {
                            msr->mpd->flag_crlf_line = 1;
                        }

                        if (multipart_process_boundary(msr, (is_final ? 1 : 0), error_msg) < 0) {
                            msr->mpd->flag_error = 1;
                            return -1;
                        }

                        if (is_final) {
                            msr->mpd->is_complete = 1;
                        }

                        processed_as_boundary = 1;
                        msr->mpd->boundary_count++;
                    }
                    else {
                        /* error */
                        msr->mpd->flag_error = 1;
                        *error_msg = apr_psprintf(msr->mp,
                            "Multipart: Invalid boundary: %s",
                            log_escape_nq(msr->mp, msr->mpd->buf));
                        return -1;
                    }
                } else { /* It looks like a boundary but we couldn't match it. */
                    char *p = NULL;

                    /* Check if an attempt to use quotes around the boundary was made. */
                    if (   (msr->mpd->flag_boundary_quoted)
                        && (strlen(msr->mpd->buf) >= strlen(msr->mpd->boundary) + 3)
                        && (*(msr->mpd->buf + 2) == '"')
                        && (strncmp(msr->mpd->buf + 3, msr->mpd->boundary, strlen(msr->mpd->boundary)) == 0)
                    ) {
                        msr->mpd->flag_error = 1;
                        *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid boundary (quotes).");
                        return -1;
                    }

                    /* Check the beginning of the boundary for whitespace. */
                    p = msr->mpd->buf + 2;
                    while(isspace(*p)) {
                        p++;
                    }

                    if (   (p != msr->mpd->buf + 2)
                        && (strncmp(p, msr->mpd->boundary, strlen(msr->mpd->boundary)) == 0)
                    ) {
                        /* Found whitespace in front of a boundary. */
                        msr->mpd->flag_error = 1;
                        *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid boundary (whitespace).");
                        return -1;
                    }

                    msr->mpd->flag_unmatched_boundary = 1;
                }
            } else { /* We do not think the buffer contains a boundary. */
                /* Look into the buffer to see if there's anything
                 * there that resembles a boundary.
                 */
                if (msr->mpd->buf_contains_line) {
                    int i, len = (MULTIPART_BUF_SIZE - msr->mpd->bufleft);
                    char *p = msr->mpd->buf;

                    for(i = 0; i < len; i++) {
                        if ((p[i] == '-') && (i + 1 < len) && (p[i + 1] == '-'))
                        {
                            if (strncmp(p + i + 2, msr->mpd->boundary, strlen(msr->mpd->boundary)) == 0) {
                                msr->mpd->flag_unmatched_boundary = 1;
                                break;
                            }
                        }
                    }
                }
            }

            /* Process as data if it was not a boundary. */
            if (processed_as_boundary == 0) {
                if (msr->mpd->mpp == NULL) {
                    msr->mpd->flag_data_before = 1;

                    if (msr->txcfg->debuglog_level >= 4) {
                        msr_log(msr, 4, "Multipart: Ignoring data before first boundary.");
                    }
                } else {
                    if (msr->mpd->mpp_state == 0) {
                        if ((msr->mpd->bufleft == 0) || (process_buffer)) {
                            /* part header lines must be shorter than
                             * MULTIPART_BUF_SIZE bytes
                             */
                            msr->mpd->flag_error = 1;
                            *error_msg = apr_psprintf(msr->mp,
                                "Multipart: Part header line over %d bytes long",
                                MULTIPART_BUF_SIZE);
                            return -1;
                        }

                        if (multipart_process_part_header(msr, error_msg) < 0) {
                            msr->mpd->flag_error = 1;
                            return -1;
                        }
                    } else {
                        if (multipart_process_part_data(msr, error_msg) < 0) {
                            msr->mpd->flag_error = 1;
                            return -1;
                        }
                    }
                }
            }

            /* Update the offset of the data we are about
             * to process. This is to allow us to know the
             * offsets of individual files and variables.
             */
            msr->mpd->buf_offset += (MULTIPART_BUF_SIZE - msr->mpd->bufleft);

            /* reset the pointer to the beginning of the buffer
             * and continue to accept input data
             */
            msr->mpd->bufptr = msr->mpd->buf;
            msr->mpd->bufleft = MULTIPART_BUF_SIZE;
            msr->mpd->buf_contains_line = (c == 0x0a) ? 1 : 0;
        }

        if ((msr->mpd->is_complete) && (inleft != 0)) {
            msr->mpd->flag_data_after = 1;

            if (msr->txcfg->debuglog_level >= 4) {
                msr_log(msr, 4, "Multipart: Ignoring data after last boundary (%u bytes left)", inleft);
            }

            return 1;
        }
    }

    return 1;
}

/**
 *
 */
apr_status_t multipart_cleanup(modsec_rec *msr) {
    int keep_files = 0;

    if (msr->mpd == NULL) return -1;

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Multipart: Cleanup started (remove files %d).", msr->upload_remove_files);
    }

    if (msr->upload_remove_files == 0) {
        if (msr->txcfg->upload_dir == NULL) {
            msr_log(msr, 1, "Input filter: SecUploadDir is undefined, unable to store "
                    "multipart files.");
        } else {
            keep_files = 1;
        }
    }

    /* Loop through the list of parts
     * and delete the temporary files, but only if
     * file storage was not requested, or if storage
     * of relevant files was requested and this isn't
     * such a request.
     */
    if (keep_files == 0) {
        multipart_part **parts;
        int i;

        parts = (multipart_part **)msr->mpd->parts->elts;
        for(i = 0; i < msr->mpd->parts->nelts; i++) {
            if (parts[i]->type == MULTIPART_FILE) {
                if (parts[i]->tmp_file_name != NULL) {
                    /* make sure it is closed first */
                    if (parts[i]->tmp_file_fd > 0) {
                        close(parts[i]->tmp_file_fd);
                        parts[i]->tmp_file_fd = -1;
                    }

                    if (unlink(parts[i]->tmp_file_name) < 0) {
                        msr_log(msr, 1, "Multipart: Failed to delete file (part) \"%s\" because %d(%s)",
                            log_escape(msr->mp, parts[i]->tmp_file_name), errno, strerror(errno));
                    } else {
                        if (msr->txcfg->debuglog_level >= 4) {
                            msr_log(msr, 4, "Multipart: Deleted file (part) \"%s\"",
                                log_escape(msr->mp, parts[i]->tmp_file_name));
                        }
                    }
                }
            }
        }
    } else {
        /* delete empty files, move the others to the upload dir */
        multipart_part **parts;
        int i;

        parts = (multipart_part **)msr->mpd->parts->elts;
        for(i = 0; i < msr->mpd->parts->nelts; i++) {
            if (    (parts[i]->type == MULTIPART_FILE)
                 && (parts[i]->tmp_file_size == 0))
            {
                /* Delete empty file. */
                if (parts[i]->tmp_file_name != NULL) {
                    /* make sure it is closed first */
                    if (parts[i]->tmp_file_fd > 0) {
                        close(parts[i]->tmp_file_fd);
                        parts[i]->tmp_file_fd = -1;
                    }

                    if (unlink(parts[i]->tmp_file_name) < 0) {
                        msr_log(msr, 1, "Multipart: Failed to delete empty file (part) \"%s\" because %d(%s)",
                            log_escape(msr->mp, parts[i]->tmp_file_name), errno, strerror(errno));
                    } else {
                        if (msr->txcfg->debuglog_level >= 4) {
                            msr_log(msr, 4, "Multipart: Deleted empty file (part) \"%s\"",
                                log_escape(msr->mp, parts[i]->tmp_file_name));
                        }
                    }
                }
            } else {
                /* Move file to the upload dir. */
                if (parts[i]->tmp_file_name != NULL) {
                    const char *new_filename = NULL;
                    const char *new_basename = NULL;

                    /* make sure it is closed first */
                    if (parts[i]->tmp_file_fd > 0) {
                        close(parts[i]->tmp_file_fd);
                        parts[i]->tmp_file_fd = -1;
                    }

                    new_basename = file_basename(msr->mp, parts[i]->tmp_file_name);
                    if (new_basename == NULL) return -1;
                    new_filename = apr_psprintf(msr->mp, "%s/%s", msr->txcfg->upload_dir,
                        new_basename);
                    if (new_filename == NULL) return -1;

                    if (apr_file_rename(parts[i]->tmp_file_name, new_filename,
                        msr->msc_reqbody_mp) != APR_SUCCESS)
                    {
                        msr_log(msr, 1, "Input filter: Failed to rename file from \"%s\" to \"%s\".",
                            log_escape(msr->mp, parts[i]->tmp_file_name),
                            log_escape(msr->mp, new_filename));
                        return -1;
                    } else {
                        if (msr->txcfg->debuglog_level >= 4) {
                            msr_log(msr, 4, "Input filter: Moved file from \"%s\" to \"%s\".",
                                log_escape(msr->mp, parts[i]->tmp_file_name),
                                log_escape(msr->mp, new_filename));
                        }
                    }
                }
            }
        }
    }

    return 1;
}

/**
 *
 */
int multipart_get_arguments(modsec_rec *msr, char *origin, apr_table_t *arguments) {
    multipart_part **parts;
    int i;

    parts = (multipart_part **)msr->mpd->parts->elts;
    for(i = 0; i < msr->mpd->parts->nelts; i++) {
        if (parts[i]->type == MULTIPART_FORMDATA) {
            msc_arg *arg = (msc_arg *)apr_pcalloc(msr->mp, sizeof(msc_arg));
            if (arg == NULL) return -1;

            arg->name = parts[i]->name;
            arg->name_len = strlen(parts[i]->name);
            arg->value = parts[i]->value;
            arg->value_len = parts[i]->length;
            arg->value_origin_offset = parts[i]->offset;
            arg->value_origin_len = parts[i]->length;
            arg->origin = origin;

            add_argument(msr, arguments, arg);
        }
    }

    return 1;
}

/**
 *
 */
char *multipart_reconstruct_urlencoded_body_sanitize(modsec_rec *msr) {
    multipart_part **parts;
    char *body;
    unsigned int body_len;
    int i;

    if (msr->mpd == NULL) return NULL;

    /* calculate the size of the buffer */
    body_len = 1;
    parts = (multipart_part **)msr->mpd->parts->elts;
    for(i = 0; i < msr->mpd->parts->nelts; i++) {
        if (parts[i]->type == MULTIPART_FORMDATA) {
            body_len += 4;
            body_len += strlen(parts[i]->name) * 3;
            body_len += strlen(parts[i]->value) * 3;
        }
    }

    /* allocate the buffer */
    body = apr_palloc(msr->mp, body_len + 1);
    if ((body == NULL) || (body_len + 1 == 0)) return NULL;
    *body = 0;

    parts = (multipart_part **)msr->mpd->parts->elts;
    for(i = 0; i < msr->mpd->parts->nelts; i++) {
        if (parts[i]->type == MULTIPART_FORMDATA) {
            if (*body != 0) {
                strncat(body, "&", body_len - strlen(body));
            }
            strnurlencat(body, parts[i]->name, body_len - strlen(body));
            strncat(body, "=", body_len - strlen(body));

            /* Sanitise the variable. Since we are only doing this for
             * the logging we will actually write over the data we keep
             * in the memory.
             */
            if (msr->phase >= PHASE_LOGGING) {
                if (apr_table_get(msr->arguments_to_sanitize, parts[i]->name) != NULL) {
                    memset(parts[i]->value, '*', strlen(parts[i]->value));
                }
            }
            strnurlencat(body, parts[i]->value, body_len - strlen(body));
        }
    }

    return body;
}
