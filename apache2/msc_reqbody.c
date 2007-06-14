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
#include "msc_parsers.h"

#define CHUNK_CAPACITY 8192

/**
 * Prepare to accept the request body (part 2).
 */
static apr_status_t modsecurity_request_body_start_init(modsec_rec *msr) {
    if(msr->msc_reqbody_storage == MSC_REQBODY_MEMORY) {
        /* Prepare to store request body in memory. */

        msr->msc_reqbody_chunks = apr_array_make(msr->msc_reqbody_mp,
            32, sizeof(msc_data_chunk *));
        if (msr->msc_reqbody_chunks == NULL) return -1;
    } else {
        /* Prepare to store request body on disk. */

        msr->msc_reqbody_filename = apr_psprintf(msr->mp, "%s/%s-%s-request_body-XXXXXX",
            msr->txcfg->tmp_dir, current_filetime(msr->mp), msr->txid);
        if (msr->msc_reqbody_filename == NULL) return -1;

        msr->msc_reqbody_fd = msc_mkstemp((char *)msr->msc_reqbody_filename);
        if (msr->msc_reqbody_fd < 0) {
            msr_log(msr, 1, "Input filter: Failed to create temporary file: %s",
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
apr_status_t modsecurity_request_body_start(modsec_rec *msr) {
    msr->msc_reqbody_length = 0;

    /* Create a separate memory pool that will be used
     * to allocate structures from (not data, which is allocated
     * via malloc).
     */
    apr_pool_create(&msr->msc_reqbody_mp, NULL);

    /* Initialise request body processors, if any. */

    if (msr->msc_reqbody_processor != NULL) {
        char *my_error_msg = NULL;

        if (strcmp(msr->msc_reqbody_processor, "MULTIPART") == 0) {
            if (multipart_init(msr, &my_error_msg) < 0) {
                msr_log(msr, 1, "Multipart parser init failed: %s", my_error_msg);
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = my_error_msg;
            }
        }
        #ifdef WITH_LIBXML2
        else
        if (strcmp(msr->msc_reqbody_processor, "XML") == 0) {
            if (xml_init(msr, &my_error_msg) < 0) {
                msr_log(msr, 1, "XML parser init failed: %s", my_error_msg);
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = my_error_msg;
            }
        }
        #endif
        else
        if (strcmp(msr->msc_reqbody_processor, "URLENCODED") == 0) {
            /* Do nothing, URLENCODED processor does not support streaming yet. */
        }
        else {
            msr_log(msr, 1, "Unknown request body processor: %s", msr->msc_reqbody_processor);
            return -1;
        }
    }

    return modsecurity_request_body_start_init(msr);
}

/**
 * Stores a chunk of request body data to disk.
 */
static apr_status_t modsecurity_request_body_store_disk(modsec_rec *msr,
    const char *data, apr_size_t length)
{
    apr_size_t i = write(msr->msc_reqbody_fd, data, length);
    if (i != length) {
        msr_log(msr, 1, "Input filter: Failed writing %lu bytes to temporary file (rc %lu).", i);
        return -1;
    }

    return 1;
}

/**
 * Stores one chunk of request body data in memory.
 */
static apr_status_t modsecurity_request_body_store_memory(modsec_rec *msr,
    const char *data, apr_size_t length)
{
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
        if (modsecurity_request_body_start_init(msr) < 0) return -1;

        /* Write the data we keep in memory */
        chunks = (msc_data_chunk **)msr->msc_reqbody_chunks->elts;
        for(i = 0; i < msr->msc_reqbody_chunks->nelts; i++) {
            disklen += chunks[i]->length;

            if (modsecurity_request_body_store_disk(msr, chunks[i]->data, chunks[i]->length) < 0) {
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

        msr_log(msr, 4, "Input filter: Wrote %lu bytes from memory to disk.", disklen);

        /* Continue with disk storage from now on */
        return modsecurity_request_body_store_disk(msr, data, length);
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
                if (msr->msc_reqbody_chunk_current == NULL) return -1;

                msr->msc_reqbody_chunk_current->data = malloc(CHUNK_CAPACITY);
                if (msr->msc_reqbody_chunk_current->data == NULL) return -1;

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
    const char *data, apr_size_t length)
{
    /* If we have a processor for this request body send
     * data to it first (but only if it did not report an
     * error on previous invocations).
     */
    if ((msr->msc_reqbody_processor != NULL)&&(msr->msc_reqbody_error == 0)) {
        char *my_error_msg = NULL;

        if (strcmp(msr->msc_reqbody_processor, "MULTIPART") == 0) {
            if (multipart_process_chunk(msr, data, length, &my_error_msg) < 0) {
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = my_error_msg;
                msr_log(msr, 4, "%s", my_error_msg);
            }
        }
        #ifdef WITH_LIBXML2
        else
        if (strcmp(msr->msc_reqbody_processor, "XML") == 0) {
            if (xml_process_chunk(msr, data, length, &my_error_msg) < 0) {
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = my_error_msg;
                msr_log(msr, 4, "%s", my_error_msg);
            }
        }
        #endif
        else
        if (strcmp(msr->msc_reqbody_processor, "URLENCODED") == 0) {
            /* Do nothing, URLENCODED processor does not support streaming. */
        }
        else {
            msr_log(msr, 1, "Unknown request body processor: %s", msr->msc_reqbody_processor);
            return -1;
        }
    }

    /* Store data. */
    if (msr->msc_reqbody_storage == MSC_REQBODY_MEMORY) {
        return modsecurity_request_body_store_memory(msr, data, length);
    }
    else
    if (msr->msc_reqbody_storage == MSC_REQBODY_DISK) {
        return modsecurity_request_body_store_disk(msr, data, length);
    }

    /* Should never happen. */
    msr_log(msr, 1, "Internal Error: Unknown value for msc_reqbody_storage: %i",
        msr->msc_reqbody_storage);
    return -1;
}

/**
 *
 */
static apr_status_t modsecurity_request_body_end_urlencoded(modsec_rec *msr) {
    msc_data_chunk **chunks, *one_chunk;
    char *d;
    int i, sofar;
    int invalid_count = 0;

    /* Allocate a buffer large enough to hold the request body. */

    if (msr->msc_reqbody_length + 1 == 0) return -1;
    msr->msc_reqbody_buffer = malloc(msr->msc_reqbody_length + 1);
    if (msr->msc_reqbody_buffer == NULL) {
        msr_log(msr, 1, "Unable to allocate memory to hold request body. Asked for %lu bytes.",
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
            msr_log(msr, 1, "Internal error, request body buffer overflow.");
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
    if (msr->msc_reqbody_chunks == NULL) return -1;
    one_chunk = (msc_data_chunk *)apr_pcalloc(msr->msc_reqbody_mp, sizeof(msc_data_chunk));
    one_chunk->data = msr->msc_reqbody_buffer;
    one_chunk->length = msr->msc_reqbody_length;
    one_chunk->is_permanent = 1;
    *(const msc_data_chunk **)apr_array_push(msr->msc_reqbody_chunks) = one_chunk;

    /* Parse URL-encoded arguments in the request body. */    

    if (parse_arguments(msr, msr->msc_reqbody_buffer, msr->msc_reqbody_length,
        msr->txcfg->argument_separator, "BODY", msr->arguments, &invalid_count) < 0)
    {
        msr_log(msr, 1, "Initialisation: Error occurred while parsing BODY arguments.");
        return -1;
    }

    return 1;
}

/**
 * Stops receiving the request body.
 */
apr_status_t modsecurity_request_body_end(modsec_rec *msr) {

    /* Close open file descriptors, if any. */
    if (msr->msc_reqbody_storage == MSC_REQBODY_DISK) {
        if (msr->msc_reqbody_fd > 0) {
            close(msr->msc_reqbody_fd);
            msr->msc_reqbody_fd = -1;
        }
    }

    msr->msc_reqbody_read = 1;

    if ((msr->msc_reqbody_processor != NULL)&&(msr->msc_reqbody_error == 0)) {
        char *my_error_msg = NULL;

        if (strcmp(msr->msc_reqbody_processor, "MULTIPART") == 0) {
            if (multipart_complete(msr, &my_error_msg) < 0) {
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = my_error_msg;
                msr_log(msr, 1, "Multipart error: %s", my_error_msg);
                return -1;
            }

            if (multipart_get_arguments(msr, "BODY", msr->arguments) < 0) {
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = "Error retrieving arguments.";
                msr_log(msr, 1, "Multipart error: %s", my_error_msg);
                return -1;
            }
        }
        else
        if (strcmp(msr->msc_reqbody_processor, "URLENCODED") == 0) {
            return modsecurity_request_body_end_urlencoded(msr);
        }
        #ifdef WITH_LIBXML2
        else
        if (strcmp(msr->msc_reqbody_processor, "XML") == 0) {
            if (xml_complete(msr, &my_error_msg) < 0) {
                msr->msc_reqbody_error = 1;
                msr->msc_reqbody_error_msg = my_error_msg;
                msr_log(msr, 4, "%s", my_error_msg);
                return -1;
            }
        }
        #endif
    }

    return 1;
}

/**
 * Prepares to forward the request body.
 */
apr_status_t modsecurity_request_body_retrieve_start(modsec_rec *msr) {
    if (msr->msc_reqbody_storage == MSC_REQBODY_MEMORY) {
        msr->msc_reqbody_chunk_position = 0;
        msr->msc_reqbody_chunk_offset = 0;
        
        msr->msc_reqbody_disk_chunk = apr_pcalloc(msr->msc_reqbody_mp, sizeof(msc_data_chunk));
        if (msr->msc_reqbody_disk_chunk == NULL) return -1;
        msr->msc_reqbody_disk_chunk->is_permanent = 1;
    }
    else
    if (msr->msc_reqbody_storage == MSC_REQBODY_DISK) {
        msr->msc_reqbody_disk_chunk = apr_pcalloc(msr->msc_reqbody_mp, sizeof(msc_data_chunk));
        if (msr->msc_reqbody_disk_chunk == NULL) return -1;

        msr->msc_reqbody_disk_chunk->is_permanent = 0;
        msr->msc_reqbody_disk_chunk->data = apr_palloc(msr->msc_reqbody_mp, CHUNK_CAPACITY);
        if (msr->msc_reqbody_disk_chunk->data == NULL) return -1;

        msr->msc_reqbody_fd = open(msr->msc_reqbody_filename, O_RDONLY | O_BINARY);
        if (msr->msc_reqbody_fd < 0) {
            msr_log(msr, 1, "Input filter: Failed to open temporary file for reading: %s",
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
    msc_data_chunk **chunk, long int nbytes)
{
    msc_data_chunk **chunks;

    if (chunk == NULL) return -1;
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
            msr_log(msr, 1, "Input filter: Error reading from temporary file: %s",
                strerror(errno));
            return -1;
        }

        *chunk = msr->msc_reqbody_disk_chunk;
        msr->msc_reqbody_disk_chunk->length = i;

        if (i == 0) return 0; /* No more data available. */

        return 1; /* More data available. */
    }

    msr_log(msr, 1, "Internal error, invalid msc_reqbody_storage value: %i",
        msr->msc_reqbody_storage);

    return -1;
}

/**
 *
 */
apr_status_t modsecurity_request_body_clear(modsec_rec *msr) {
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
                msr_log(msr, 1, "Input filter: SecUploadDir is undefined, "
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
                if (put_basename == NULL) return -1;
                put_filename = apr_psprintf(msr->msc_reqbody_mp, "%s/%s",
                    msr->txcfg->upload_dir, put_basename);                    
                if (put_filename == NULL) return -1;

                if (apr_file_rename(msr->msc_reqbody_filename, put_filename,
                    msr->msc_reqbody_mp) != APR_SUCCESS)
                {
                    msr_log(msr, 1, "Failed to rename file from \"%s\" to \"%s\".",
                        log_escape(msr->msc_reqbody_mp, msr->msc_reqbody_filename),
                        log_escape(msr->msc_reqbody_mp, put_filename));
                    return -1;
                } else {
                    msr_log(msr, 4, "Moved file from \"%s\" to \"%s\".",
                        log_escape(msr->msc_reqbody_mp, msr->msc_reqbody_filename),
                        log_escape(msr->msc_reqbody_mp, put_filename));
                }
            } else {
                /* We do not want to keep the request body. */
                if (apr_file_remove(msr->msc_reqbody_filename,
                    msr->msc_reqbody_mp) != APR_SUCCESS)
                {
                    msr_log(msr, 1, "Failed to delete temporary file: %s",
                        msr->msc_reqbody_filename);
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
