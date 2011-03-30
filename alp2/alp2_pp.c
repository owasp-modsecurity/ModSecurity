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

#include <ctype.h>
#include <stdio.h>
#include <sys/param.h>

#include "alp2_pp.h"

/**
 * Take the line in the buffer and replace the new line
 * at the end with a NUL byte.
 */
char *alp2_pp_line_chomp(alp2_pp_t *pp) {
    if (pp->line_pos == 0) {
        pp->line_buf[0] = '\0';
    }
    else {
        pp->line_buf[pp->line_pos - 1] = '\0';
    }

    return &(pp->line_buf[0]);
}

/**
 * Look into the line buffer to determine if it
 * contains a boundary line.
 */
static int alp2_pp_is_boundary_line(alp2_pp_t *alp_pp) {
    char *new_boundary = NULL;
    unsigned int id;
    size_t i;

    /* A boundary line cannot be less than 14 characters long. */
    if (alp_pp->line_pos < 15) {
        return 0;
    }

    /* The first two characters must both be dashes. */
    if ((alp_pp->line_buf[0] != '-')||(alp_pp->line_buf[1] != '-')) {
        return 0;
    }

    /* Extract the boundary. */
    i = 2; /* Start after the second dash. */
    while((isxdigit(alp_pp->line_buf[i]))&&(i < alp_pp->line_pos)) {
        i++;
    }    

    /* The boundary cannot be shorter than 8 characters. */
    if (i - 2 < 8) {
        return 0;
    }

    // TODO Memory leak; use a single parser buffer to avoid per-entry
    //      allocation from the parser pool.
    new_boundary = apr_pstrndup(alp_pp->mp, &(alp_pp->line_buf[2]), i - 2);

    /* Check if the rest of the line is valid. */
    if (  (i + 5 < alp_pp->line_pos) /* Need at lest 5 more bytes. */
        ||(alp_pp->line_buf[i + 0] != '-')
        ||(alp_pp->line_buf[i + 1] < 'A')
        ||(alp_pp->line_buf[i + 1] > 'Z')
        ||(alp_pp->line_buf[i + 2] != '-')
        ||(alp_pp->line_buf[i + 3] != '-')
        ||(alp_pp->line_buf[i + 4] != '\n') )
    {
        return 0;
    }

    id = alp_pp->line_buf[i + 1];

    /* Are we in a middle of an entry right now? */
    if (alp_pp->current_entry == NULL) {
        /* We will accept a new boundary. */
        alp_pp->boundary = new_boundary;

        return id;
    }
    else {
        /* The boundary must match the boundary of
         * the entry we are currently working on.
         */
        if (strcmp(alp_pp->current_entry->boundary, new_boundary) != 0) {
            return 0;
        }
        else {
            return id;
        }
    }

    return 0;
}

/**
 * Process data belonging to a single part.
 */
static void alp2_pp_process_part_data(alp2_pp_t *alp_pp) {
    if (alp_pp->current_part == NULL) {
        return;
    }
    
    /* Invoke part processor. */
    if (alp_pp->callback != NULL) {
        if (alp_pp->callback(alp_pp, ALP2_EVENT_PART_DATA) == 0) {
            alp_pp->done = 1;
        }
    }
                
    /* Keep track of part size. */
    alp_pp->current_part->size += alp_pp->line_pos;

    /* Update the MD5 hash calculation. */
    if ((alp_pp->current_entry != NULL)&&(alp_pp->line_pos > 0)) {
        apr_md5_update(alp_pp->current_entry->md5_context, &alp_pp->line_buf[0], alp_pp->line_pos - 1);
    }
}

/**
 * Initialise parser.
 */
int alp2_pp_init(alp2_pp_t *alp_pp, void *user_data,
    int (*callback)(alp2_pp_t *alp_pp, int event_type), apr_pool_t *mp)
{
    memset(alp_pp, 0, sizeof(alp2_pp_t));

    alp_pp->user_data = user_data;
    alp_pp->callback = callback;
    alp_pp->mp = mp; /* Use the parent pool directly. */

    /* Set-up the line buffer. */
    alp_pp->line_buf = apr_pcalloc(mp, ALP2_MAX_LINE_SIZE);
    alp_pp->line_size = ALP2_MAX_LINE_SIZE;
    alp_pp->line_has_start = 1;
    alp_pp->line_offset = 0;
    
    return 1;
}

/**
 * Process data the parser has stored in the input buffer.
 */
static apr_status_t alp2_pp_process_internal(alp2_pp_t *alp_pp) {
    /* Do not proceed if we've previously 
     * encountered a fatal error.
     */
    if (alp_pp->errored != 0) {
        return ALP2_ERROR_FATAL;
    }

    if (alp_pp->done) {
        return ALP2_DONE;
    }

    /* Go back straight away if we don't have anything to work with. */
    if (alp_pp->input_len == 0) {
        return ALP2_NEED_DATA;
    }

    while (alp_pp->input_pos < alp_pp->input_len) {
        int c;

        if (alp_pp->done) {
            return ALP2_DONE;
        }

        if (alp_pp->line_pos >= alp_pp->line_size) {
            /* Our line buffer is full with the
             * line incomplete.
             */
            alp2_pp_process_part_data(alp_pp);

            /* Reset line buffer . */
            alp_pp->line_pos = 0;
            alp_pp->line_has_start = 0;
            alp_pp->line_offset = alp_pp->current_offset;
        }

        /* Consume one byte. */
        c = alp_pp->input_buf[alp_pp->input_pos];
        alp_pp->input_pos++;
        alp_pp->current_offset++;
        
        /* Copy the byte to the line buffer. */
        alp_pp->line_buf[alp_pp->line_pos] = c;
        alp_pp->line_pos++;
       
        /* Are we at the end of a line? */       
        if (c == '\n') {
            if (alp_pp->line_has_start) {
                /* We have one complete line. */
                
                int id = alp2_pp_is_boundary_line(alp_pp);

                if (id != 0) {
                    /* The line is a boundary. */
                    
                    /* Finish with the previous part, if any. */
                    if (alp_pp->current_part != NULL) {
                        /* Update the MD5 context. */
                        apr_md5_update(alp_pp->current_entry->md5_context,
                            &alp_pp->line_buf[0], alp_pp->line_pos - 1);
                        
                        /* Event PART_END. */
                        if (alp_pp->callback != NULL) {
                            if (alp_pp->callback(alp_pp, ALP2_EVENT_PART_END) == 0) {
                                alp_pp->done = 1;
                            }
                        }
                        
                        /* Add part to the current entry. */
                        *(alp2_pp_part_t **)apr_array_push(alp_pp->current_entry->parts)
                            = alp_pp->current_part;

                        /* Delete part. */
                        alp_pp->current_part = NULL;
                        
                        /* If the new part is part Z, then finish
                         * with the current entry. */
                        if (id == 'Z') {
                            alp_pp->current_entry->size = alp_pp->current_offset - alp_pp->current_entry->offset;

                            /* Create the MD5 digest. */
                            apr_md5_final(alp_pp->current_entry->md5_digest,
                                alp_pp->current_entry->md5_context);

                            /* Event ENTRY_END. */
                            if (alp_pp->callback != NULL) {
                                if (alp_pp->callback(alp_pp, ALP2_EVENT_ENTRY_END) == 0) {
                                    alp_pp->done = 1;
                                }
                            }

                            /* We are about to destroy our only reference to the per-entry
                             * memory pool, but that is all right since we've passed all
                             * responsibility for the entry to the higher-level handler.
                             */
                            alp_pp->current_entry = NULL;
                        }
                    }
                    
                    if (id != 'Z') {
                        /* Create new entry if necessary. */
                        if (alp_pp->current_entry == NULL) {
                            apr_pool_t *new_pool = NULL;

                            /* Create a per-entry pool directly from the main memory pool. */
                            apr_pool_create(&new_pool, apr_pool_parent_get(alp_pp->mp));

                            alp_pp->current_entry = apr_pcalloc(new_pool, sizeof(alp2_pp_entry_t));
                            alp_pp->current_entry->mp = new_pool;
                            alp_pp->current_entry->offset = alp_pp->line_offset;
                            alp_pp->current_entry->boundary = apr_pstrdup(new_pool, alp_pp->boundary);
                            alp_pp->boundary = NULL;

                            alp_pp->current_entry->parts = apr_array_make(alp_pp->current_entry->mp,
                                16, sizeof(alp2_pp_part_t *));

                            /* Initialise the MD5 context. */
                            alp_pp->current_entry->md5_context = apr_pcalloc(alp_pp->current_entry->mp,
                                sizeof(apr_md5_ctx_t));
                            apr_md5_init(alp_pp->current_entry->md5_context);

                            /* Start calculating the has with the first line. */
                            apr_md5_update(alp_pp->current_entry->md5_context, &alp_pp->line_buf[0], alp_pp->line_pos - 1);

                            /* Event ENTRY_START. */
                            if (alp_pp->callback != NULL) {
                                if (alp_pp->callback(alp_pp, ALP2_EVENT_ENTRY_START) == 0) {
                                    alp_pp->done = 1;
                                }
                            }
                        }
                    
                        /* Create new part, but only if we are not
                         * dealing with an entry terminator.
                         */
                        alp_pp->current_part = apr_pcalloc(alp_pp->current_entry->mp, sizeof(alp2_pp_part_t));
                        alp_pp->current_part->id = id;
                        alp_pp->current_part->offset = alp_pp->current_offset;

                        /* Event PART_START. */
                        if (alp_pp->callback != NULL) {
                            if (alp_pp->callback(alp_pp, ALP2_EVENT_PART_START) == 0) {
                                alp_pp->done = 1;
                            }
                        }
                    }
                }
                else {
                    /* The line does not contain a boundary,
                     * so process it as part data.
                     */
                    alp2_pp_process_part_data(alp_pp);
                }
            }
            else {
                /* We have a chunk of data that is not a line, which
                 * probably means that our buffer was not big enough, either
                 * because the line (is a line and it) was too big, or because
                 * we are processing binary data. Ideally the latter.
                 */
                alp2_pp_process_part_data(alp_pp);
            }
            
            /* Reset the line buffer. */
            alp_pp->line_pos = 0;
            alp_pp->line_has_start = 1;
            alp_pp->line_offset = alp_pp->current_offset;
        }
    }

    if (alp_pp->done) {
        return ALP2_DONE;
    }
    else {
        return ALP2_NEED_DATA;
    }
}

/**
 * Process the provided data.
 */
int alp2_pp_process(alp2_pp_t *alp_pp, const char *data, size_t len) {
    /* Do not proceed if we've previously 
     * encountered a fatal error.
     */
    if (alp_pp->errored != 0) {
        return ALP2_ERROR_FATAL;
    }
    
    /* Check that we've used up the existing buffer. */
    if (alp_pp->input_pos < alp_pp->input_len) {
        return ALP2_ERROR_INCORRECT_STATE;
    }
    
    alp_pp->input_buf = data;
    alp_pp->input_len = len;
    alp_pp->input_pos = 0;
    
    return alp2_pp_process_internal(alp_pp);
}

/**
 * Clean-up the parser structures.
 */
void alp2_pp_terminate(alp2_pp_t *alp_pp) {
    /* Nothing to do, but we may need
     * to do something in the future.
     */
}
