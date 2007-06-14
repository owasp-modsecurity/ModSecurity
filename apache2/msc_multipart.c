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
#include <ctype.h>

#include "msc_multipart.h"
#include "msc_util.h"


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
        if (!( isalnum(c)||(c == '.') )) *p = '_';
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
    while((*p == '\t')||(*p == ' ')) p++;
    if (*p == '\0') return 1; /* this is OK */
    
    if (*p != ';') return -2;
    p++;
    
    /* parse the appended parts */
    
    while(*p != '\0') {
        char *name = NULL, *value = NULL, *start = NULL;
        
        /* go over the whitespace */
        while((*p == '\t')||(*p == ' ')) p++;
        if (*p == '\0') return -3;
        
        start = p;
        while((*p != '\0')&&(*p != '=')&&(*p != '\t')&&(*p != ' ')) p++;
        if (*p == '\0') return -4;
        
        name = apr_pstrmemdup(msr->mp, start, (p - start));
        
        while((*p == '\t')||(*p == ' ')) p++;
        if (*p == '\0') return -5;

        if (*p != '=') return -13;        
        p++;
        
        while((*p == '\t')||(*p == ' ')) p++;
        if (*p == '\0') return -6;
        
        if (*p == '"') {
            /* quoted */
            
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
                    /* only " and \ can be escaped */
                    if ((*(p + 1) == '"')||(*(p + 1) == '\\')) {
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
                else
                if (*p == '"') {
                    *t = '\0';
                    break;
                }
                
                *t++ = *p++;
            }
            if (*p == '\0') return -10;
            
            p++; /* go over the quote at the end */
            
        } else {
            /* not quoted */
            
            start = p;
            while((*p != '\0')&&(is_token_char(*p))) p++;
            value = apr_pstrmemdup(msr->mp, start, (p - start));
        }
        
        /* evaluate part */
        
        if (strcmp(name, "name") == 0) {
            if (msr->mpd->mpp->name != NULL) return -14;
            msr->mpd->mpp->name = value;
            msr_log(msr, 9, "Multipart: Content-Disposition name: %s",
                log_escape_nq(msr->mp, value));
        }
        else
        if (strcmp(name, "filename") == 0) {
            if (msr->mpd->mpp->filename != NULL) return -15;
            msr->mpd->mpp->filename = value;
            msr_log(msr, 9, "Multipart: Content-Disposition filename: %s",
                log_escape_nq(msr->mp, value));
        }
        else return -11;
        
        if (*p != '\0') {
            while((*p == '\t')||(*p == ' ')) p++;
            /* the next character must be a zero or a semi-colon */ 
            if (*p == '\0') return 1; /* this is OK */
            if (*p != ';') return -12;
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
    int rc;
    
    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if ((msr->mpd->buf[0] == '\r')
        &&(msr->mpd->buf[1] == '\n')
        &&(msr->mpd->buf[2] == '\0'))
    {
        char *header_value;
        
        /* empty line */

        header_value = (char *)apr_table_get(msr->mpd->mpp->headers, "Content-Disposition");
        if (header_value == NULL) {
            *error_msg = apr_psprintf(msr->mp, "Multipart: Part is missing the Content-Disposition header");
            return -1;
        }
        
        rc = multipart_parse_content_disposition(msr, header_value);
        if (rc < 0) {
            *error_msg = apr_psprintf(msr->mp, "Multipart: Invalid Content-Disposition header (%i): %s",
                rc, log_escape_nq(msr->mp, header_value));
            return -1;
        }

        if (msr->mpd->mpp->name == NULL) {
            *error_msg = apr_psprintf(msr->mp, "Multipart: Part name missing");
            return -1;
        }        
        
        if (msr->mpd->mpp->filename != NULL) {
            msr->mpd->mpp->type = MULTIPART_FILE;
        } else {
            msr->mpd->mpp->type = MULTIPART_FORMDATA;
        }
        
        msr->mpd->mpp_state = 1;
        msr->mpd->mpp->last_header_name = NULL;
    } else {
        /* header line */
        if ((msr->mpd->buf[0] == '\t')||(msr->mpd->buf[0] == ' ')) {
            char *header_value, *new_value, *data;
            
            /* header folding, add data to the header we are building */
            
            if (msr->mpd->mpp->last_header_name == NULL) {
                /* we are not building a header at this moment */
                *error_msg = apr_psprintf(msr->mp, "Multipart: invalid part header (invalid folding)");
                return -1;
            }
            
            /* locate the beginning of the data */
            data = msr->mpd->buf;
            while((*data == '\t')||(*data == ' ')) data++;
            
            new_value = apr_pstrdup(msr->mp, data);
            remove_lf_crlf_inplace(new_value);
            
            /* update the header value in the table */
            header_value = (char *)apr_table_get(msr->mpd->mpp->headers, msr->mpd->mpp->last_header_name);
            new_value = apr_pstrcat(msr->mp, header_value, " ", new_value, NULL);
            apr_table_set(msr->mpd->mpp->headers, msr->mpd->mpp->last_header_name, new_value);
            
            msr_log(msr, 9, "Multipart: Continued folder header \"%s\" with \"%s\"",
                log_escape(msr->mp, msr->mpd->mpp->last_header_name),
                log_escape(msr->mp, data));
            
            if (strlen(new_value) > 4096) {
                *error_msg = apr_psprintf(msr->mp, "Multpart: invalid part header (too long)");
                return -1;
            }
        } else {
            char *header_name, *header_value, *data;
            
            /* new header */
            
            data = msr->mpd->buf;
            while((*data != ':')&&(*data != '\0')) data++;
            if (*data == '\0') {
                *error_msg = apr_psprintf(msr->mp, "Multipart: invalid part header (missing colon): %s",
                    log_escape_nq(msr->mp, msr->mpd->buf));
                return -1;
            }

            header_name = apr_pstrmemdup(msr->mp, msr->mpd->buf, (data - msr->mpd->buf));
            
            /* extract the value value */
            data++;
            while((*data == '\t')||(*data == ' ')) data++;
            header_value = apr_pstrdup(msr->mp, data);
            remove_lf_crlf_inplace(header_value);
                        
            /* error if the name already exists */
            if (apr_table_get(msr->mpd->mpp->headers, header_name) != NULL) {
                *error_msg = apr_psprintf(msr->mp, "Multipart: part header already exists: %s",
                    log_escape_nq(msr->mp, header_name));
                return -1;
            }
            
            apr_table_setn(msr->mpd->mpp->headers, header_name, header_value);
            msr->mpd->mpp->last_header_name = header_name;
            
            msr_log(msr, 9, "Multipart: Added part header \"%s\" \"%s\"",
                log_escape(msr->mp, header_name),
                log_escape(msr->mp, header_value));
        }
    }
    
    return 1;
}

/**
 *
 */
static int multipart_process_part_data(modsec_rec *msr, char **error_msg) {
    char *p = msr->mpd->buf + (MULTIPART_BUF_SIZE - msr->mpd->bufleft) - 2;
    char localreserve[2] = { '\0', '\0' }; /* initialized to quiet warning */
    int bytes_reserved = 0;
    
    if (error_msg == NULL) return -1;
    *error_msg = NULL;
    
    /* preserve the last two bytes for later */
    if (MULTIPART_BUF_SIZE - msr->mpd->bufleft >= 2) {
        bytes_reserved = 1;
        localreserve[0] = *p;
        localreserve[1] = *(p + 1);
        msr->mpd->bufleft += 2;
        *p = 0;
    }

    /* add data to the part we are building */
    if (msr->mpd->mpp->type == MULTIPART_FILE) {

        /* remember where we started */
        if (msr->mpd->mpp->length == 0) {
            msr->mpd->mpp->offset = msr->mpd->buf_offset;
        }

        /* only store individual files on disk if we are going
         * to keep them or if we need to have them approved later
         */
        if (msr->upload_extract_files) {
            /* first create a temporary file if we don't have it already */
            if (msr->mpd->mpp->tmp_file_fd == 0) {
                // char *filename = multipart_construct_filename(msr);

                /* construct temporary file name */
                msr->mpd->mpp->tmp_file_name = apr_psprintf(msr->mp, "%s/%s-%s-file-XXXXXX",
                    msr->txcfg->tmp_dir, current_filetime(msr->mp), msr->txid);
                msr->mpd->mpp->tmp_file_fd = msc_mkstemp(msr->mpd->mpp->tmp_file_name);

                /* do we have an opened file? */
                if (msr->mpd->mpp->tmp_file_fd < 0) {
                    *error_msg = apr_psprintf(msr->mp, "Multipart: Failed to create file: %s",
                        log_escape_nq(msr->mp, msr->mpd->mpp->tmp_file_name));
                    return -1;
                }

                msr_log(msr, 4, "Multipart: Created temporary file: %s",
                    log_escape_nq(msr->mp, msr->mpd->mpp->tmp_file_name));
            }

            /* write the reserve first */
            if (msr->mpd->reserve[0] == 1) {
                if (write(msr->mpd->mpp->tmp_file_fd, &msr->mpd->reserve[1], 2) != 2) {
                    *error_msg = apr_psprintf(msr->mp, "Multipart: writing to \"%s\" failed",
                        log_escape(msr->mp, msr->mpd->mpp->tmp_file_name));
                    return -1;
                }
                msr->mpd->mpp->tmp_file_size += 2;
                msr->mpd->mpp->length += 2;
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
            if (msr->mpd->reserve[0] == 1) {
                msr->mpd->mpp->tmp_file_size += 2;
            }
            msr->mpd->mpp->tmp_file_size += (MULTIPART_BUF_SIZE - msr->mpd->bufleft);
            msr->mpd->mpp->length += (MULTIPART_BUF_SIZE - msr->mpd->bufleft);
        }
    }
    else if (msr->mpd->mpp->type == MULTIPART_FORMDATA) {
        value_part_t *value_part = apr_pcalloc(msr->mp, sizeof(value_part_t));
    
        /* add this part to the list of parts */

        /* remember where we started */
        if (msr->mpd->mpp->length == 0) {
            msr->mpd->mpp->offset = msr->mpd->buf_offset;
        }
        
        if (msr->mpd->reserve[0] == 1) {
            value_part->data = apr_palloc(msr->mp, (MULTIPART_BUF_SIZE - msr->mpd->bufleft) + 2);
            memcpy(value_part->data, &(msr->mpd->reserve[1]), 2);
            memcpy(value_part->data + 2, msr->mpd->buf, (MULTIPART_BUF_SIZE - msr->mpd->bufleft));
            value_part->length = (MULTIPART_BUF_SIZE - msr->mpd->bufleft) + 2;
            msr->mpd->mpp->length += value_part->length;
        } else {
            value_part->length = (MULTIPART_BUF_SIZE - msr->mpd->bufleft);
            value_part->data = apr_pstrmemdup(msr->mp, msr->mpd->buf, value_part->length);
            msr->mpd->mpp->length += value_part->length;
        }
        
        *(value_part_t **)apr_array_push(msr->mpd->mpp->value_parts) = value_part;

        msr_log(msr, 9, "Multipart: Added data to variable: %s",
            log_escape_nq_ex(msr->mp, value_part->data, value_part->length));
    }
    else {
        *error_msg = apr_psprintf(msr->mp, "Multipart: unknown part type %i", msr->mpd->mpp->type);
        return -1;
    }

    /* store the reserved bytes to the multipart
     * context so that they don't get lost
     */
    if (bytes_reserved) {
        msr->mpd->reserve[0] = 1;
        msr->mpd->reserve[1] = localreserve[0];
        msr->mpd->reserve[2] = localreserve[1];
        msr->mpd->buf_offset += 2;
    }
    else {
        msr->mpd->reserve[0] = 0;
        msr->mpd->buf_offset -= 2;
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
        }

        if (msr->mpd->mpp->type != MULTIPART_FILE) {
            /* now construct a single string out of the parts */
            msr->mpd->mpp->value = multipart_combine_value_parts(msr, msr->mpd->mpp->value_parts);
            if (msr->mpd->mpp->value == NULL) return -1;
        }        

        /* add the part to the list of parts */
        *(multipart_part **)apr_array_push(msr->mpd->parts) = msr->mpd->mpp;
        if (msr->mpd->mpp->type == MULTIPART_FILE) {
            msr_log(msr, 9, "Multipart: Added file part %x to the list: name \"%s\" "
                "file name \"%s\" (offset %u, length %u)",
                msr->mpd->mpp, log_escape(msr->mp, msr->mpd->mpp->name),
                log_escape(msr->mp, msr->mpd->mpp->filename),
                msr->mpd->mpp->offset, msr->mpd->mpp->length);
        }
        else {
            msr_log(msr, 9, "Multipart: Added part %x to the list: name \"%s\" "
                "(offset %u, length %u)", msr->mpd->mpp, log_escape(msr->mp, msr->mpd->mpp->name),
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


/**
 *
 */
int multipart_init(modsec_rec *msr, char **error_msg) {
    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    msr->mpd = (multipart_data *)apr_pcalloc(msr->mp, sizeof(multipart_data));
    if (msr->mpd == NULL) return -1;

    if (msr->request_content_type == NULL) {
        *error_msg = apr_psprintf(msr->mp, "Multipart: Content-Type header not available.");
        return -1;
    }

    msr->mpd->boundary = strstr(msr->request_content_type, "boundary=");
    if ((msr->mpd->boundary != NULL)&&(*(msr->mpd->boundary + 9) != 0)) {
        msr->mpd->boundary = msr->mpd->boundary + 9;
    }
    else {
        *error_msg = apr_psprintf(msr->mp, "Multipart Boundary not found or invalid.");
        return -1;
    }

    msr->mpd->parts = apr_array_make(msr->mp, 10, sizeof(multipart_part *));
    msr->mpd->bufleft = MULTIPART_BUF_SIZE;
    msr->mpd->bufptr = msr->mpd->buf;
    msr->mpd->buf_contains_line = 1;
    msr->mpd->mpp = NULL;

    return 1;
}

/**
 *
 */
int multipart_complete(modsec_rec *msr, char **error_log) {
    if (msr->mpd == NULL) return 1;

    if ((msr->mpd->seen_data != 0)&&(msr->mpd->is_complete == 0)) {
        *error_log = apr_psprintf(msr->mp, "Multipart: final boundary missing");
        return -1;
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

    if (msr->mpd->seen_data == 0) msr->mpd->seen_data = 1;
    
    if (msr->mpd->is_complete) {
        msr_log(msr, 4, "Multipart: Ignoring data after last boundary (received %i bytes)", size);
        return 1;
    }
    
    if (msr->mpd->bufleft == 0) {
        *error_msg = apr_psprintf(msr->mp,
            "Multipart: Internal error in process_chunk: no space left in the buffer");
        return -1;
    }

    /* here we loop through the data available, byte by byte */
    while(inleft > 0) {
        char c = *inptr;
        int process_buffer = 0;

        if ((c == 0x0d)&&(msr->mpd->bufleft == 1)) {
            /* we don't want to take 0x0d as the last byte in the buffer */
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
        if ((c == 0x0a)||(msr->mpd->bufleft == 0)||(process_buffer)) {
            *(msr->mpd->bufptr) = 0;

            /* boundary preconditions: length of the line greater than
             * the length of the boundary + the first two characters
             * are dashes "-"
             */
            if ( msr->mpd->buf_contains_line
                && (strlen(msr->mpd->buf) > strlen(msr->mpd->boundary) + 2)
                && (((*(msr->mpd->buf) == '-'))&&(*(msr->mpd->buf + 1) == '-'))
                && (strncmp(msr->mpd->buf + 2, msr->mpd->boundary, strlen(msr->mpd->boundary)) == 0) ) {
                
                char *boundary_end = msr->mpd->buf + 2 + strlen(msr->mpd->boundary);
                    
                if (  (*boundary_end == '\r')
                    &&(*(boundary_end + 1) == '\n')
                    &&(*(boundary_end + 2) == '\0')
                ) {
                    /* simple boundary */
                    if (multipart_process_boundary(msr, 0, error_msg) < 0) return -1;
                }
                else
                if (  (*boundary_end == '-')
                    &&(*(boundary_end + 1) == '-')
                    &&(*(boundary_end + 2) == '\r')
                    &&(*(boundary_end + 3) == '\n')
                    &&(*(boundary_end + 4) == '\0')
                ) {
                    /* final boundary */
                    msr->mpd->is_complete = 1;
                    if (multipart_process_boundary(msr, 1, error_msg) < 0) return -1;
                }
                else {
                    /* error */
                    *error_msg = apr_psprintf(msr->mp,
                        "Multipart: Invalid boundary detected: %s",
                        log_escape_nq(msr->mp, msr->mpd->buf));
                    return -1;
                }
            }
            else {
                if (msr->mpd->mpp == NULL) {
                    msr_log(msr, 4, "Multipart: Ignoring data before first boundary.");
                } else {
                    if (msr->mpd->mpp_state == 0) {
                        if ((msr->mpd->bufleft == 0)||(process_buffer)) {
                            /* part header lines must be shorter than
                             * MULTIPART_BUF_SIZE bytes
                             */
                            *error_msg = apr_psprintf(msr->mp,
                                "Multipart: Part header line over %i bytes long",
                                MULTIPART_BUF_SIZE);
                            return -1;
                        }
                        if (multipart_process_part_header(msr, error_msg) < 0) return -1;
                    } else {
                        if (multipart_process_part_data(msr, error_msg) < 0) return -1;
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
        
        if ((msr->mpd->is_complete)&&(inleft != 0)) {
            msr_log(msr, 4, "Multipart: Ignoring data after last boundary (%i bytes left)", inleft);
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

    msr_log(msr, 4, "Multipart: Cleanup started (remove files %i).", msr->upload_remove_files);

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
                    if (unlink(parts[i]->tmp_file_name) < 0) {
                        msr_log(msr, 1, "Multipart: Failed to delete file (part) \"%s\" because %d(%s)",
                            log_escape(msr->mp, parts[i]->tmp_file_name), errno, strerror(errno));
                    } else {
                        msr_log(msr, 4, "Multipart: Deleted file (part) \"%s\"",
                            log_escape(msr->mp, parts[i]->tmp_file_name));
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
            if ((parts[i]->type == MULTIPART_FILE)&&(parts[i]->tmp_file_size == 0)) {
                /* Delete empty file. */
                if (parts[i]->tmp_file_name != NULL) {
                    if (unlink(parts[i]->tmp_file_name) < 0) {
                        msr_log(msr, 1, "Multipart: Failed to delete empty file (part) \"%s\" because %d(%s)",
                            log_escape(msr->mp, parts[i]->tmp_file_name), errno, strerror(errno));
                    } else {
                        msr_log(msr, 4, "Multipart: Deleted empty file (part) \"%s\"",
                            log_escape(msr->mp, parts[i]->tmp_file_name));
                    }
                }
            } else {
                /* Move file to the upload dir. */
                if (parts[i]->tmp_file_name != NULL) {
                    const char *new_filename = NULL;
                    const char *new_basename = NULL;

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
                        msr_log(msr, 4, "Input filter: Moved file from \"%s\" to \"%s\".",
                            log_escape(msr->mp, parts[i]->tmp_file_name),
                            log_escape(msr->mp, new_filename));
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
            arg->name_len = strlen(parts[i]->name); // TODO
            arg->value = parts[i]->value;
            arg->value_len = parts[i]->length;
            arg->value_origin_offset = parts[i]->offset;
            arg->value_origin_len = parts[i]->length;
            arg->origin = origin;

            apr_table_addn(arguments, arg->name, (void *)arg);
        }
    }

    return 1;
}

/**
 *
 */
char *multipart_reconstruct_urlencoded_body_sanitise(modsec_rec *msr) {
    multipart_part **parts;
    char *body;
    unsigned int body_len;
    int i;
    
    if (msr->mpd == NULL) return NULL;

    // TODO Cache this data somewhere in the structure

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
    if ((body == NULL)||(body_len + 1 == 0)) return NULL;
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
                if (apr_table_get(msr->arguments_to_sanitise, parts[i]->name) != NULL) {
                    memset(parts[i]->value, '*', strlen(parts[i]->value));
                }
            }
            strnurlencat(body, parts[i]->value, body_len - strlen(body));
        }
    }
                
    return body;
}
