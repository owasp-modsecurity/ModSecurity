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
#include "re.h"
#include "msc_parsers.h"

#define CHUNK_CAPACITY 8192


/**
 *
 */
void msre_engine_reqbody_processor_register(msre_engine *engine,
    const char *name, void *fn_init, void *fn_process, void *fn_complete)
{
    msre_reqbody_processor_metadata *metadata = 
        (msre_reqbody_processor_metadata *)apr_pcalloc(engine->mp,
            sizeof(msre_reqbody_processor_metadata));
    if (metadata == NULL) return;

    metadata->name = name;
    metadata->init = fn_init;
    metadata->process = fn_process;
    metadata->complete = fn_complete;
    apr_table_setn(engine->reqbody_processors, name, (void *)metadata);
}

/**
 * Prepare to accept the request body (part 2).
 */
static apr_status_t modsecurity_request_body_start_init(modsec_rec *msr, char **error_msg) {
    *error_msg = NULL;

    if(msr->msc_reqbody_storage == MSC_REQBODY_MEMORY) {
        /* Prepare to store request body in memory. */

        msr->msc_reqbody_chunks = apr_array_make(msr->msc_reqbody_mp,
            32, sizeof(msc_data_chunk *));
        if (msr->msc_reqbody_chunks == NULL) {
            *error_msg = apr_pstrdup(msr->mp, "Input filter: Failed to prepare in-memory storage.");
            return -1;
        }
    } else {
        /* Prepare to store request body on disk. */

        msr->msc_reqbody_filename = apr_psprintf(msr->mp, "%s/%s-%s-request_body-XXXXXX",
            msr->txcfg->tmp_dir, current_filetime(msr->mp), msr->txid);
        if (msr->msc_reqbody_filename == NULL) {
            *error_msg = apr_pstrdup(msr->mp, "Input filter: Failed to generate an on-disk filename.");
            return -1;
        }

        msr->msc_reqbody_fd = msc_mkstemp((char *)msr->msc_reqbody_filename);
        if (msr->msc_reqbody_fd < 0) {
            *error_msg = apr_psprintf(msr->mp, "Input filter: Failed to create temporary file: %s",
                msr->msc_reqbody_filename);
            return -1;
        }

        msr_log(msr, 4, "Input filter: Created temporary file to store request body: %s",
            msr->msc_reqbody_filename);
    }

    return 1;
}

/**
 * Prepare to accept the request body (part 1).
 */
apr_status_t modsecurity_request_body_start(modsec_rec *msr, char **error_msg) {
    *error_msg = NULL;
    msr->msc_reqbody_length = 0;
    msr->stream_input_length = 0;

    /* Create a separate memory pool that will be used
     * to allocate structures from (not data, which is allocated
     * via malloc).
     */
    apr_pool_create(&msr->msc_reqbody_mp, NULL);

    /* Initialise request body processors, if any. */

    if (msr->msc_reqbody_processor != NULL) {
        char *my_error_msg = NULL;
        msre_reqbody_processor_metadata *metadata =
            (msre_reqbody_processor_metadata *)apr_table_get(msr->modsecurity->msre->reqbody_processors, msr->msc_reqbody_processor);

        if (metadata != NULL) {
            if (   (metadata->init != NULL)
                && (metadata->init(msr, &my_error_msg) < 0))
            {
                *error_msg = apr_psprintf(msr->mp,
                                          "%s parsing error (init): %s",
                                          msr->msc_reqbody_processor,
                                          my_error_msg);
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = my_error_msg;
                msr_log(msr, 2, "%s", *error_msg);
            }
        }
        // TODO: All these below need to be registered in the same way as above
        else if (strcmp(msr->msc_reqbody_processor, "MULTIPART") == 0) {
            if (multipart_init(msr, &my_error_msg) < 0) {
                *error_msg = apr_psprintf(msr->mp, "Multipart parsing error (init): %s", my_error_msg);
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = my_error_msg;
                msr_log(msr, 2, "%s", *error_msg);
            }
        }
        else if (strcmp(msr->msc_reqbody_processor, "XML") == 0) {
            if (xml_init(msr, &my_error_msg) < 0) {
                *error_msg = apr_psprintf(msr->mp, "XML parsing error (init): %s", my_error_msg);
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = my_error_msg;
                msr_log(msr, 2, "%s", *error_msg);
            }
        }
        else if (strcmp(msr->msc_reqbody_processor, "URLENCODED") == 0) {
            /* Do nothing, URLENCODED processor does not support streaming yet. */
        }
        else {
            *error_msg = apr_psprintf(msr->mp, "Unknown request body processor: %s",
                msr->msc_reqbody_processor);
            return -1;
        }
    }

    return modsecurity_request_body_start_init(msr, error_msg);
}

/**
 * Stores a chunk of request body data to disk.
 */
static apr_status_t modsecurity_request_body_store_disk(modsec_rec *msr,
    const char *data, apr_size_t length, char **error_msg)
{
    apr_size_t i;

    *error_msg = NULL;

    i = write(msr->msc_reqbody_fd, data, length);
    if (i != length) {
        *error_msg = apr_psprintf(msr->mp, "Input filter: Failed writing %" APR_SIZE_T_FMT
            " bytes to temporary file (rc %" APR_SIZE_T_FMT ").", length, i);
        return -1;
    }

    return 1;
}

/**
 * Stores one chunk of request body data in memory.
 */
static apr_status_t modsecurity_request_body_store_memory(modsec_rec *msr,
    const char *data, apr_size_t length, char **error_msg)
{
    *error_msg = NULL;

    /* Would storing this chunk mean going over the limit? */
    if ((msr->msc_reqbody_spilltodisk)
        && (msr->msc_reqbody_length + length > (apr_size_t)msr->txcfg->reqbody_inmemory_limit))
    {
        msc_data_chunk **chunks;
        unsigned int disklen = 0;
        int i;

        msr_log(msr, 4, "Input filter: Request too large to store in memory, switching to disk.");

        /* NOTE Must use modsecurity_request_body_store_disk() here
         *      to prevent data to be sent to the streaming
         *      processors again.
         */

        /* Initialise disk storage */
        msr->msc_reqbody_storage = MSC_REQBODY_DISK;
        if (modsecurity_request_body_start_init(msr, error_msg) < 0) return -1;

        /* Write the data we keep in memory */
        chunks = (msc_data_chunk **)msr->msc_reqbody_chunks->elts;
        for(i = 0; i < msr->msc_reqbody_chunks->nelts; i++) {
            disklen += chunks[i]->length;

            if (modsecurity_request_body_store_disk(msr, chunks[i]->data, chunks[i]->length, error_msg) < 0) {
                return -1;
            }

            free(chunks[i]->data);
            chunks[i]->data = NULL;
        }

        /* Clear the memory pool as we no longer need the bits. */

        /* IMP1 But since we only used apr_pool_clear memory might
         * not be released back to the OS straight away?
         */
        msr->msc_reqbody_chunks = NULL;
        apr_pool_clear(msr->msc_reqbody_mp);

        msr_log(msr, 4, "Input filter: Wrote %u bytes from memory to disk.", disklen);

        /* Continue with disk storage from now on */
        return modsecurity_request_body_store_disk(msr, data, length, error_msg);
    }

    /* If we're here that means we are not over the
     * request body in-memory limit yet.
     */
    {
        unsigned long int bucket_offset, bucket_left;

        bucket_offset = 0;
        bucket_left = length;

        /* Although we store the request body in chunks we don't
         * want to use the same chunk sizes as the incoming memory
         * buffers. They are often of very small sizes and that
         * would make us waste a lot of memory. That's why we
         * use our own chunks of CHUNK_CAPACITY sizes.
         */

        /* Loop until we empty this bucket into our chunks. */
        while(bucket_left > 0) {
            /* Allocate a new chunk if we have to. */
            if (msr->msc_reqbody_chunk_current == NULL) {
                msr->msc_reqbody_chunk_current = (msc_data_chunk *)
                    apr_pcalloc(msr->msc_reqbody_mp, sizeof(msc_data_chunk));
                if (msr->msc_reqbody_chunk_current == NULL) {
                    *error_msg = apr_psprintf(msr->mp, "Input filter: Failed to allocate %lu bytes "
                        "for request body chunk.", (unsigned long)sizeof(msc_data_chunk));
                    return -1;
                }

                msr->msc_reqbody_chunk_current->data = malloc(CHUNK_CAPACITY);
                if (msr->msc_reqbody_chunk_current->data == NULL) {
                    *error_msg = apr_psprintf(msr->mp, "Input filter: Failed to allocate %d bytes "
                        "for request body chunk data.", CHUNK_CAPACITY);
                    return -1;
                }

                msr->msc_reqbody_chunk_current->length = 0;
                msr->msc_reqbody_chunk_current->is_permanent = 1;

                *(const msc_data_chunk **)apr_array_push(msr->msc_reqbody_chunks)
                    = msr->msc_reqbody_chunk_current;
            }

            if (bucket_left < (CHUNK_CAPACITY - msr->msc_reqbody_chunk_current->length)) {
                /* There's enough space in the current chunk. */
                memcpy(msr->msc_reqbody_chunk_current->data +
                    msr->msc_reqbody_chunk_current->length, data + bucket_offset, bucket_left);
                msr->msc_reqbody_chunk_current->length += bucket_left;
                bucket_left = 0;
            } else {
                /* Fill the existing chunk. */
                unsigned long int copy_length = CHUNK_CAPACITY -
                    msr->msc_reqbody_chunk_current->length;

                memcpy(msr->msc_reqbody_chunk_current->data +
                    msr->msc_reqbody_chunk_current->length, data + bucket_offset, copy_length);
                bucket_offset += copy_length;
                bucket_left -= copy_length;
                msr->msc_reqbody_chunk_current->length += copy_length;

                /* We're done with this chunk. Setting the pointer
                 * to NULL is going to force a new chunk to be allocated
                 * on the next go.
                 */
                msr->msc_reqbody_chunk_current = NULL;
            }
        }

        msr->msc_reqbody_length += length;
    }

    return 1;
}

/**
 * Stores one chunk of request body data. Returns -1 on error.
 */
apr_status_t modsecurity_request_body_store(modsec_rec *msr,
    const char *data, apr_size_t length, char **error_msg)
{
    *error_msg = NULL;

    /* If we have a processor for this request body send
     * data to it first (but only if it did not report an
     * error on previous invocations).
     */
    if ((msr->msc_reqbody_processor != NULL) && (msr->msc_reqbody_error == 0)) {
        char *my_error_msg = NULL;
        msre_reqbody_processor_metadata *metadata =
            (msre_reqbody_processor_metadata *)apr_table_get(msr->modsecurity->msre->reqbody_processors, msr->msc_reqbody_processor);

        if (metadata != NULL) {
            if (   (metadata->process != NULL)
                && (metadata->process(msr, data, length, &my_error_msg) < 0))
            {
                *error_msg = apr_psprintf(msr->mp,
                                          "%s parsing error: %s",
                                          msr->msc_reqbody_processor,
                                          my_error_msg);
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = my_error_msg;
                msr_log(msr, 2, "%s", *error_msg);
            }
        }
        // TODO: All these below need to be registered in the same way as above
        else if (strcmp(msr->msc_reqbody_processor, "MULTIPART") == 0) {
            /* The per-request data length counter will
             * be updated by the multipart parser.
             */

            /* Process data as multipart/form-data. */
            if (multipart_process_chunk(msr, data, length, &my_error_msg) < 0) {
                *error_msg = apr_psprintf(msr->mp, "Multipart parsing error: %s", my_error_msg);
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = *error_msg;
                msr_log(msr, 2, "%s", *error_msg);
            }
        }
        else if (strcmp(msr->msc_reqbody_processor, "XML") == 0) {
            /* Increase per-request data length counter. */
            msr->msc_reqbody_no_files_length += length;

            /* Process data as XML. */
            if (xml_process_chunk(msr, data, length, &my_error_msg) < 0) {
                *error_msg = apr_psprintf(msr->mp, "XML parsing error: %s", my_error_msg);
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = *error_msg;
                msr_log(msr, 2, "%s", *error_msg);
            }
        }
        else if (strcmp(msr->msc_reqbody_processor, "URLENCODED") == 0) {
            /* Increase per-request data length counter. */
            msr->msc_reqbody_no_files_length += length;

            /* Do nothing else, URLENCODED processor does not support streaming. */
        }
        else {
            *error_msg = apr_psprintf(msr->mp, "Unknown request body processor: %s",
                msr->msc_reqbody_processor);
            return -1;
        }
    } else if (msr->txcfg->reqbody_buffering != REQUEST_BODY_FORCEBUF_OFF) {
        /* Increase per-request data length counter if forcing buffering. */
        msr->msc_reqbody_no_files_length += length;
    }

    /* Check that we are not over the request body no files limit. */
    if (msr->msc_reqbody_no_files_length >= (unsigned long) msr->txcfg->reqbody_no_files_limit) {

        *error_msg = apr_psprintf(msr->mp, "Request body no files data length is larger than the "
                "configured limit (%ld).", msr->txcfg->reqbody_no_files_limit);
        if (msr->txcfg->debuglog_level >= 1) {
            msr_log(msr, 1, "%s", *error_msg);
        }

        if ((msr->txcfg->is_enabled == MODSEC_ENABLED) && (msr->txcfg->if_limit_action == REQUEST_BODY_LIMIT_ACTION_REJECT))   {
            return -5;
        } else if (msr->txcfg->if_limit_action == REQUEST_BODY_LIMIT_ACTION_PARTIAL)  {
            if(msr->txcfg->is_enabled == MODSEC_ENABLED)
                return -5;
        }
    }


    /* Store data. */
    if (msr->msc_reqbody_storage == MSC_REQBODY_MEMORY) {
        return modsecurity_request_body_store_memory(msr, data, length, error_msg);
    }
    else
        if (msr->msc_reqbody_storage == MSC_REQBODY_DISK) {
        return modsecurity_request_body_store_disk(msr, data, length, error_msg);
    }

    /* Should never happen. */
    *error_msg = apr_psprintf(msr->mp, "Internal error, unknown value for msc_reqbody_storage: %u",
        msr->msc_reqbody_storage);
    return -1;
}

apr_status_t modsecurity_request_body_to_stream(modsec_rec *msr, const char *buffer, int buflen, char **error_msg) {
    char *stream_input_body = NULL;
    char *data = NULL;
    int first_pkt = 0;

    if(msr->stream_input_data == NULL)  {
        msr->stream_input_data = (char *)calloc(sizeof(char), msr->stream_input_length + 1);
        first_pkt = 1;
    }
    else    {

        data = (char *)malloc(msr->stream_input_length + 1 - buflen);

        if(data == NULL)
            return -1;

        memset(data, 0, msr->stream_input_length + 1 - buflen);
        memcpy(data, msr->stream_input_data, msr->stream_input_length - buflen);

        stream_input_body = (char *)realloc(msr->stream_input_data, msr->stream_input_length + 1);

        msr->stream_input_data = (char *)stream_input_body;
    }

    if (msr->stream_input_data == NULL) {
        if(data)    {
            free(data);
            data = NULL;
        }
        *error_msg = apr_psprintf(msr->mp, "Unable to allocate memory to hold request body on stream. Asked for %" APR_SIZE_T_FMT " bytes.",
                msr->stream_input_length + 1);
        return -1;
    }

    memset(msr->stream_input_data, 0, msr->stream_input_length+1);

    if(first_pkt)   {
        memcpy(msr->stream_input_data, buffer, msr->stream_input_length);
    } else {
        memcpy(msr->stream_input_data, data, msr->stream_input_length - buflen);
        memcpy(msr->stream_input_data+(msr->stream_input_length - buflen), buffer, buflen);
    }

    if(data)    {
        free(data);
        data = NULL;
    }

    return 1;
}
/**
 * Replace a bunch of chunks holding a request body with a single large chunk.
 */
static apr_status_t modsecurity_request_body_end_raw(modsec_rec *msr, char **error_msg) {
    msc_data_chunk **chunks, *one_chunk;
    char *d;
    int i, sofar;

    *error_msg = NULL;

    /* Allocate a buffer large enough to hold the request body. */

    if (msr->msc_reqbody_length + 1 == 0) {
        *error_msg = apr_psprintf(msr->mp, "Internal error, request body length will overflow: %u",
            msr->msc_reqbody_length);
        return -1;
    }

    msr->msc_reqbody_buffer = malloc(msr->msc_reqbody_length + 1);
    if (msr->msc_reqbody_buffer == NULL) {
        *error_msg = apr_psprintf(msr->mp, "Unable to allocate memory to hold request body. Asked for %u bytes.",
            msr->msc_reqbody_length + 1);
        return -1;
    }

    msr->msc_reqbody_buffer[msr->msc_reqbody_length] = '\0';

    /* Copy the data we keep in chunks into the new buffer. */

    sofar = 0;
    d = msr->msc_reqbody_buffer;
    chunks = (msc_data_chunk **)msr->msc_reqbody_chunks->elts;
    for(i = 0; i < msr->msc_reqbody_chunks->nelts; i++) {
        if (sofar + chunks[i]->length <= msr->msc_reqbody_length) {
            memcpy(d, chunks[i]->data, chunks[i]->length);
            d += chunks[i]->length;
            sofar += chunks[i]->length;
        } else {
            *error_msg = apr_psprintf(msr->mp, "Internal error, request body buffer overflow.");
            return -1;
        }
    }


    /* Now free the memory used by the chunks. */

    chunks = (msc_data_chunk **)msr->msc_reqbody_chunks->elts;
    for(i = 0; i < msr->msc_reqbody_chunks->nelts; i++) {
        free(chunks[i]->data);
        chunks[i]->data = NULL;
    }

    /* Create a new array with only one chunk in it. */

    msr->msc_reqbody_chunks = apr_array_make(msr->msc_reqbody_mp, 2, sizeof(msc_data_chunk *));
    if (msr->msc_reqbody_chunks == NULL) {
        *error_msg = apr_pstrdup(msr->mp, "Failed to create structure to hold request body.");
        return -1;
    }

    one_chunk = (msc_data_chunk *)apr_pcalloc(msr->msc_reqbody_mp, sizeof(msc_data_chunk));
    one_chunk->data = msr->msc_reqbody_buffer;
    one_chunk->length = msr->msc_reqbody_length;
    one_chunk->is_permanent = 1;
    *(const msc_data_chunk **)apr_array_push(msr->msc_reqbody_chunks) = one_chunk;

    if(msr->txcfg->reqbody_limit > 0 && msr->txcfg->reqbody_limit < msr->msc_reqbody_length)    {
        msr->msc_reqbody_length = msr->txcfg->reqbody_limit;
    }

    return 1;
}

/**
 *
 */
static apr_status_t modsecurity_request_body_end_urlencoded(modsec_rec *msr, char **error_msg) {
    int invalid_count = 0;

    *error_msg = NULL;

    /* Create the raw buffer */
    if (modsecurity_request_body_end_raw(msr, error_msg) != 1) {
        return -1;
    }

    /* Parse URL-encoded arguments in the request body. */

    if (parse_arguments(msr, msr->msc_reqbody_buffer, msr->msc_reqbody_length,
        msr->txcfg->argument_separator, "BODY", msr->arguments, &invalid_count) < 0)
    {
        *error_msg = apr_pstrdup(msr->mp, "Initialisation: Error occurred while parsing BODY arguments.");
        return -1;
    }

    if (invalid_count) {
        msr->urlencoded_error = 1;
    }

    return 1;
}

/**
 * Stops receiving the request body.
 */
apr_status_t modsecurity_request_body_end(modsec_rec *msr, char **error_msg) {
    *error_msg = NULL;

    /* Close open file descriptors, if any. */
    if (msr->msc_reqbody_storage == MSC_REQBODY_DISK) {
        if (msr->msc_reqbody_fd > 0) {
            close(msr->msc_reqbody_fd);
            msr->msc_reqbody_fd = -1;
        }
    }

    /* Note that we've read the body. */
    msr->msc_reqbody_read = 1;

    /* Finalise body processing. */
    if ((msr->msc_reqbody_processor != NULL) && (msr->msc_reqbody_error == 0)) {
        char *my_error_msg = NULL;
        msre_reqbody_processor_metadata *metadata =
            (msre_reqbody_processor_metadata *)apr_table_get(msr->modsecurity->msre->reqbody_processors, msr->msc_reqbody_processor);

        if (metadata != NULL) {
            if (   (metadata->complete != NULL)
                && (metadata->complete(msr, &my_error_msg) < 0))
            {
                *error_msg = apr_psprintf(msr->mp,
                                          "%s parsing error (complete): %s",
                                          msr->msc_reqbody_processor,
                                          my_error_msg);
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = my_error_msg;
                msr_log(msr, 2, "%s", *error_msg);
            }
        }
        // TODO: All these below need to be registered in the same way as above
        else if (strcmp(msr->msc_reqbody_processor, "MULTIPART") == 0) {
            if (multipart_complete(msr, &my_error_msg) < 0) {
                *error_msg = apr_psprintf(msr->mp, "Multipart parsing error: %s", my_error_msg);
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = *error_msg;
                if (msr->txcfg->debuglog_level >= 4) {
                    msr_log(msr, 4, "%s", *error_msg);
                }
                return -1;
            }

            if (multipart_get_arguments(msr, "BODY", msr->arguments) < 0) {
                *error_msg = "Multipart parsing error: Failed to retrieve arguments.";
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = *error_msg;
                msr_log(msr, 2, "%s", *error_msg);
                return -1;
            }
        }
        else if (strcmp(msr->msc_reqbody_processor, "URLENCODED") == 0) {
            return modsecurity_request_body_end_urlencoded(msr, error_msg);
        }
        else if (strcmp(msr->msc_reqbody_processor, "XML") == 0) {
            if (xml_complete(msr, &my_error_msg) < 0) {
                *error_msg = apr_psprintf(msr->mp, "XML parser error: %s", my_error_msg);
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = *error_msg;
                msr_log(msr, 2, "%s", *error_msg);
                return -1;
            }
        }
    } else if (msr->txcfg->reqbody_buffering != REQUEST_BODY_FORCEBUF_OFF) {
        /* Convert to a single continous buffer, but don't do anything else. */
        return modsecurity_request_body_end_raw(msr, error_msg);
    }

    /* Note the request body no files length. */
    msr_log(msr, 4, "Request body no files length: %" APR_SIZE_T_FMT, msr->msc_reqbody_no_files_length);

    return 1;
}

/**
 * Prepares to forward the request body.
 */
apr_status_t modsecurity_request_body_retrieve_start(modsec_rec *msr, char **error_msg) {
    *error_msg = NULL;

    if (msr->msc_reqbody_storage == MSC_REQBODY_MEMORY) {
        msr->msc_reqbody_chunk_position = 0;
        msr->msc_reqbody_chunk_offset = 0;

        msr->msc_reqbody_disk_chunk = apr_pcalloc(msr->msc_reqbody_mp, sizeof(msc_data_chunk));
        if (msr->msc_reqbody_disk_chunk == NULL) {
            *error_msg = apr_psprintf(msr->mp, "Failed to allocate %lu bytes for request body disk chunk.",
                (unsigned long)sizeof(msc_data_chunk));
            return -1;
        }
        msr->msc_reqbody_disk_chunk->is_permanent = 1;
    }
    else
    if (msr->msc_reqbody_storage == MSC_REQBODY_DISK) {
        msr->msc_reqbody_disk_chunk = apr_pcalloc(msr->msc_reqbody_mp, sizeof(msc_data_chunk));
        if (msr->msc_reqbody_disk_chunk == NULL) {
            *error_msg = apr_psprintf(msr->mp, "Failed to allocate %lu bytes for request body disk chunk.",
                (unsigned long)sizeof(msc_data_chunk));
            return -1;
        }

        msr->msc_reqbody_disk_chunk->is_permanent = 0;
        msr->msc_reqbody_disk_chunk->data = apr_palloc(msr->msc_reqbody_mp, CHUNK_CAPACITY);
        if (msr->msc_reqbody_disk_chunk->data == NULL) {
            *error_msg = apr_psprintf(msr->mp, "Failed to allocate %d bytes for request body disk chunk data.",
                CHUNK_CAPACITY);
            return -1;
        }

        msr->msc_reqbody_fd = open(msr->msc_reqbody_filename, O_RDONLY | O_BINARY);
        if (msr->msc_reqbody_fd < 0) {
            *error_msg = apr_psprintf(msr->mp, "Failed to open temporary file for reading: %s",
                msr->msc_reqbody_filename);
            return -1;
        }
    }

    return 1;
}

/**
 *
 */
apr_status_t modsecurity_request_body_retrieve_end(modsec_rec *msr) {
    if (msr->msc_reqbody_storage == MSC_REQBODY_DISK) {
        if (msr->msc_reqbody_fd > 0) {
            close(msr->msc_reqbody_fd);
            msr->msc_reqbody_fd = -1;
        }
    }

    return 1;
}

/**
 * Returns one chunk of request body data. It stores a NULL
 * in the chunk pointer when there is no data to return. The
 * return code is 1 if more calls can be made to retrieve more
 * data, 0 if there is no more data to retrieve, or -1 on error.
 *
 * The caller can limit the amount of data returned by providing
 * a non-negative value in nbytes.
 */
apr_status_t modsecurity_request_body_retrieve(modsec_rec *msr,
    msc_data_chunk **chunk, long int nbytes, char **error_msg)
{
    msc_data_chunk **chunks;

    *error_msg = NULL;

    if (chunk == NULL) {
        *error_msg = apr_pstrdup(msr->mp, "Internal error, retrieving request body chunk.");
        return -1;
    }
    *chunk = NULL;

    if (msr->msc_reqbody_storage == MSC_REQBODY_MEMORY) {
        /* Are there any chunks left? */
        if (msr->msc_reqbody_chunk_position >= msr->msc_reqbody_chunks->nelts) {
            /* No more chunks. */
            return 0;
        }

        /* We always respond with the same chunk, just different information in it. */
        *chunk = msr->msc_reqbody_disk_chunk;

        /* Advance to the current chunk and position on the
         * next byte we need to send.
         */
        chunks = (msc_data_chunk **)msr->msc_reqbody_chunks->elts;
        msr->msc_reqbody_disk_chunk->data = chunks[msr->msc_reqbody_chunk_position]->data
            + msr->msc_reqbody_chunk_offset;

        if (nbytes < 0) {
            /* Send what's left in this chunk as there is no limit on the size. */
            msr->msc_reqbody_disk_chunk->length = chunks[msr->msc_reqbody_chunk_position]->length;
            msr->msc_reqbody_chunk_position++;
            msr->msc_reqbody_chunk_offset = 0;
        } else {
            /* We have a limit we must obey. */

            if (chunks[msr->msc_reqbody_chunk_position]->length -
                msr->msc_reqbody_chunk_offset <= (unsigned int)nbytes)
            {
                /* If what's left in our chunk is less than the limit
                 * then send it all back.
                 */
                msr->msc_reqbody_disk_chunk->length =
                    chunks[msr->msc_reqbody_chunk_position]->length -
                    msr->msc_reqbody_chunk_offset;
                msr->msc_reqbody_chunk_position++;
                msr->msc_reqbody_chunk_offset = 0;
            } else {
                /* If we have more data in our chunk, send the
                 * maximum bytes we can (nbytes).
                 */
                msr->msc_reqbody_disk_chunk->length = nbytes;
                msr->msc_reqbody_chunk_offset += nbytes;
            }
        }

        /* If we've advanced beyond our last chunk then
         * we have no more data to send.
         */
        if (msr->msc_reqbody_chunk_position >= msr->msc_reqbody_chunks->nelts) {
            return 0; /* No more chunks. */
        }

        /* More data available. */
        return 1;
    }

    if (msr->msc_reqbody_storage == MSC_REQBODY_DISK) {
        long int my_nbytes = CHUNK_CAPACITY;
        int i;

        /* Send CHUNK_CAPACITY bytes at a time unless a lower limit was requested. */
        if ((nbytes != -1)&&(my_nbytes > nbytes)) {
            my_nbytes = nbytes;
        }

        i = read(msr->msc_reqbody_fd, msr->msc_reqbody_disk_chunk->data, my_nbytes);
        if (i < 0) {
            *error_msg = apr_psprintf(msr->mp, "Input filter: Error reading from temporary file: %s",
                strerror(errno));
            return -1;
        }

        *chunk = msr->msc_reqbody_disk_chunk;
        msr->msc_reqbody_disk_chunk->length = i;

        if (i == 0) return 0; /* No more data available. */

        return 1; /* More data available. */
    }

    /* Should never happen. */
    *error_msg = apr_psprintf(msr->mp, "Internal error, invalid msc_reqbody_storage value: %u",
        msr->msc_reqbody_storage);

    return -1;
}

/**
 *
 */
apr_status_t modsecurity_request_body_clear(modsec_rec *msr, char **error_msg) {
    *error_msg = NULL;

    /* Release memory we used to store request body data. */
    if (msr->msc_reqbody_chunks != NULL) {
        msc_data_chunk **chunks = (msc_data_chunk **)msr->msc_reqbody_chunks->elts;
        int i;

        for(i = 0; i < msr->msc_reqbody_chunks->nelts; i++) {
            if (chunks[i]->data != NULL) {
                free(chunks[i]->data);
                chunks[i]->data = NULL;
            }
        }
    }

    if (msr->msc_reqbody_storage == MSC_REQBODY_DISK) {
        int keep_body = 0;

        /* Should we keep the body? This normally
         * happens when a PUT method was used, which
         * means the body is actually a file.
         */
        if ((msr->upload_remove_files == 0)&&(strcasecmp(msr->request_method, "PUT") == 0)) {
            if (msr->txcfg->upload_dir != NULL) {
                keep_body = 1;
            } else {
                *error_msg = apr_psprintf(msr->mp, "Input filter: SecUploadDir is undefined, "
                    "unable to store PUT file.");
            }
        }

        /* Deal with a request body stored in a file. */

        if (msr->msc_reqbody_filename != NULL) {
            if (keep_body) {
                /* Move request body (which is a file) to the storage area. */
                const char *put_filename = NULL;
                const char *put_basename = NULL;

                /* Construct the new filename. */
                put_basename = file_basename(msr->msc_reqbody_mp, msr->msc_reqbody_filename);
                if (put_basename == NULL) {
                    *error_msg = apr_psprintf(msr->mp, "Input filter: Failed to generate basename to PUT file \"%s\"", log_escape(msr->msc_reqbody_mp, msr->msc_reqbody_filename));
                    return -1;
                }
                put_filename = apr_psprintf(msr->msc_reqbody_mp, "%s/%s",
                    msr->txcfg->upload_dir, put_basename);
                if (put_filename == NULL) {
                    *error_msg = apr_psprintf(msr->mp, "Input filter: Failed to generate filename to PUT file \"%s\"", log_escape(msr->msc_reqbody_mp, msr->msc_reqbody_filename));
                    return -1;
                }

                if (apr_file_rename(msr->msc_reqbody_filename, put_filename,
                    msr->msc_reqbody_mp) != APR_SUCCESS)
                {
                    *error_msg = apr_psprintf(msr->mp, "Input filter: Failed to rename file from \"%s\" to \"%s\".",
                        log_escape(msr->msc_reqbody_mp, msr->msc_reqbody_filename),
                        log_escape(msr->msc_reqbody_mp, put_filename));
                    return -1;
                } else {
                    msr_log(msr, 4, "Input filter: Moved file from \"%s\" to \"%s\".",
                        log_escape(msr->msc_reqbody_mp, msr->msc_reqbody_filename),
                        log_escape(msr->msc_reqbody_mp, put_filename));
                }
            } else {
                /* make sure it is closed first */
                if (msr->msc_reqbody_fd > 0) {
                    close(msr->msc_reqbody_fd);
                    msr->msc_reqbody_fd = -1;
                }

                /* We do not want to keep the request body. */
                if (apr_file_remove(msr->msc_reqbody_filename,
                    msr->msc_reqbody_mp) != APR_SUCCESS)
                {
                    *error_msg = apr_psprintf(msr->mp, "Input filter: Failed to delete temporary file: %s",
                        log_escape(msr->mp, msr->msc_reqbody_filename));
                    return -1;
                }

                msr_log(msr, 4, "Input filter: Removed temporary file: %s",
                    msr->msc_reqbody_filename);
            }

            msr->msc_reqbody_filename = NULL;
        }
    }

    if (msr->msc_reqbody_mp != NULL) {
        apr_pool_destroy(msr->msc_reqbody_mp);
        msr->msc_reqbody_mp = NULL;
    }

    return 1;
}
