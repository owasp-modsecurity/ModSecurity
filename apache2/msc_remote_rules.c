/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2013 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#include "msc_remote_rules.h"
#include "msc_status_engine.h"

#include <apr_thread_pool.h>
#include <curl/curl.h>

#include <apu.h>
#include <apr_crypto.h>
#include <apr_sha1.h>

#ifndef AP_MAX_ARGC
#define AP_MAX_ARGC 64
#endif

#ifdef WITH_REMOTE_RULES_SUPPORT

/**
 * @brief Insert a new SecRule to be processed by ModSecurity
 *
 * This is a duplicate of `invoke_cmd', which is part of the standalone module.
 * Apache versions does not include the standalone, thus, this is necessary for
 * the Apache versions. Once it is here it may not be necessary to be part of
 * the standalone module, but, for this version both function will co-exist
 * avoiding problems with 3rd parties that are extending the standalone module.
 *
 * @note Prefer this function instead of `invoke_cmd` which is part of the
 *       standalone module.
 *
 * @param cmd pointer to command_rec structure.
 * @param parms pointer to cmd_params strucutre.
 * @param mconfig pointer to main config structure.
 * @param args SecRule arguments.
 * @retval NULL if everything worked as expected otherwise an error message.
 *
 */
const char *msc_remote_invoke_cmd(const command_rec *cmd, cmd_parms *parms,
                              void *mconfig, const char *args)
{
    char *w, *w2, *w3;
    const char *errmsg = NULL;

    if ((parms->override & cmd->req_override) == 0)
        return apr_pstrcat(parms->pool, cmd->name, " not allowed here", NULL);

    parms->info = cmd->cmd_data;
    parms->cmd = cmd;

    switch (cmd->args_how) {
    case RAW_ARGS:
#ifdef RESOLVE_ENV_PER_TOKEN
        args = ap_resolve_env(parms->pool,args);
#endif
        return cmd->AP_RAW_ARGS(parms, mconfig, args);

    case TAKE_ARGV:
        {
            char *argv[AP_MAX_ARGC];
            int argc = 0;

            do {
                w = ap_getword_conf(parms->pool, &args);
                if (*w == '\0' && *args == '\0') {
                    break;
                }
                argv[argc] = w;
                argc++;
            } while (argc < AP_MAX_ARGC && *args != '\0');

            return cmd->AP_TAKE_ARGV(parms, mconfig, argc, argv);
        }

    case NO_ARGS:
        if (*args != 0)
            return apr_pstrcat(parms->pool, cmd->name, " takes no arguments",
                               NULL);

        return cmd->AP_NO_ARGS(parms, mconfig);
    case TAKE1:
        w = ap_getword_conf(parms->pool, &args);

        if (*w == '\0' || *args != 0)
            return apr_pstrcat(parms->pool, cmd->name, " takes one argument",
                               cmd->errmsg ? ", " : NULL, cmd->errmsg, NULL);

        return cmd->AP_TAKE1(parms, mconfig, w);
    case TAKE2:
        w = ap_getword_conf(parms->pool, &args);
        w2 = ap_getword_conf(parms->pool, &args);

        if (*w == '\0' || *w2 == '\0' || *args != 0)
            return apr_pstrcat(parms->pool, cmd->name, " takes two arguments",
                               cmd->errmsg ? ", " : NULL, cmd->errmsg, NULL);

        return cmd->AP_TAKE2(parms, mconfig, w, w2);
    case TAKE12:
        w = ap_getword_conf(parms->pool, &args);
        w2 = ap_getword_conf(parms->pool, &args);

        if (*w == '\0' || *args != 0)
            return apr_pstrcat(parms->pool, cmd->name, " takes 1-2 arguments",
                               cmd->errmsg ? ", " : NULL, cmd->errmsg, NULL);

        return cmd->AP_TAKE2(parms, mconfig, w, *w2 ? w2 : NULL);
    case TAKE3:
        w = ap_getword_conf(parms->pool, &args);
        w2 = ap_getword_conf(parms->pool, &args);
        w3 = ap_getword_conf(parms->pool, &args);

        if (*w == '\0' || *w2 == '\0' || *w3 == '\0' || *args != 0)
            return apr_pstrcat(parms->pool, cmd->name,
                    " takes three arguments",
                    cmd->errmsg ? ", " : NULL, cmd->errmsg, NULL);

        return cmd->AP_TAKE3(parms, mconfig, w, w2, w3);
    case TAKE23:

        w = ap_getword_conf(parms->pool, &args);
        w2 = ap_getword_conf(parms->pool, &args);
        w3 = *args ? ap_getword_conf(parms->pool, &args) : NULL;

        if (*w == '\0' || *w2 == '\0' || *args != 0)
            return apr_pstrcat(parms->pool, cmd->name,
                               " takes two or three arguments",
                               cmd->errmsg ? ", " : NULL, cmd->errmsg, NULL);

        return cmd->AP_TAKE3(parms, mconfig, w, w2, w3);
    case TAKE123:
        w = ap_getword_conf(parms->pool, &args);
        w2 = *args ? ap_getword_conf(parms->pool, &args) : NULL;
        w3 = *args ? ap_getword_conf(parms->pool, &args) : NULL;

        if (*w == '\0' || *args != 0)
            return apr_pstrcat(parms->pool, cmd->name,
                               " takes one, two or three arguments",
                               cmd->errmsg ? ", " : NULL, cmd->errmsg, NULL);

        return cmd->AP_TAKE3(parms, mconfig, w, w2, w3);
    case TAKE13:
        w = ap_getword_conf(parms->pool, &args);
        w2 = *args ? ap_getword_conf(parms->pool, &args) : NULL;
        w3 = *args ? ap_getword_conf(parms->pool, &args) : NULL;

        if (*w == '\0' || (w2 && *w2 && !w3) || *args != 0)
            return apr_pstrcat(parms->pool, cmd->name,
                               " takes one or three arguments",
                               cmd->errmsg ? ", " : NULL, cmd->errmsg, NULL);

        return cmd->AP_TAKE3(parms, mconfig, w, w2, w3);
    case ITERATE:
        while (*(w = ap_getword_conf(parms->pool, &args)) != '\0') {

            errmsg = cmd->AP_TAKE1(parms, mconfig, w);

            if (errmsg && strcmp(errmsg, DECLINE_CMD) != 0)
                return errmsg;
        }

        return errmsg;
    case ITERATE2:
        w = ap_getword_conf(parms->pool, &args);

        if (*w == '\0' || *args == 0)
            return apr_pstrcat(parms->pool, cmd->name,
                               " requires at least two arguments",
                               cmd->errmsg ? ", " : NULL, cmd->errmsg, NULL);

        while (*(w2 = ap_getword_conf(parms->pool, &args)) != '\0') {

            errmsg = cmd->AP_TAKE2(parms, mconfig, w, w2);

            if (errmsg && strcmp(errmsg, DECLINE_CMD) != 0)
                return errmsg;
        }

        return errmsg;
    case FLAG:
        w = ap_getword_conf(parms->pool, &args);

        if (*w == '\0' || (strcasecmp(w, "on") && strcasecmp(w, "off")))
            return apr_pstrcat(parms->pool, cmd->name, " must be On or Off",
                               NULL);

        return cmd->AP_FLAG(parms, mconfig, strcasecmp(w, "off") != 0);
    default:
        return apr_pstrcat(parms->pool, cmd->name,
                           " is improperly configured internally (server bug)",
                           NULL);
    }
}
/**
 * @brief Fetch an URL and fill the content into a memory buffer.
 *
 * Fill an msc_curl_memory_buffer_t structure with the content of an given
 * URL.
 *
 * @note While fetching the content, it will present the ModSecurity instance
 *       to the remote server, trough: ModSecurity Unique ID, ModSecurity
 *       status line and also, if given, key that can be used to
 *       authentication. Such data is presented in the following HTTP headers:
 *         - ModSec-status
 *         - ModSec-unique-id
 *         - ModSec-key
 *
 * @warning Cleanup the memory after use it.
 *
 * @param mp pointer to the memory pool.
 * @param uri URI to be fetched.
 * @param key KEY to be present as ModSec-key.
 * @param chunk pointer to an msc_curl_memory_buffer_t struct.
 * @param error_msg pointer an char pointer, filled is something went wrong.
 *
 * @retval n>=0 everything went fine.
 * @retval n<-1 Something wrong happened, further details on error_msg.
 *
 */
int msc_remote_grab_content(apr_pool_t *mp, const char *uri, const char *key,
    struct msc_curl_memory_buffer_t *chunk, char **error_msg)
{
    CURL *curl;
    CURLcode res;

    char id[(APR_SHA1_DIGESTSIZE*2) + 1];
    char *apr_id = NULL;
    char *beacon_str = NULL;
    char *beacon_apr = NULL;
    char *header_key = NULL;
    int beacon_str_len = 0;

    memset(id, '\0', sizeof(id));
    if (msc_status_engine_unique_id(id))
    {
        sprintf(id, "no unique id");
    }

    apr_id = apr_psprintf(mp, "ModSec-unique-id: %s", id);

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    beacon_str_len = msc_beacon_string(NULL, 0);

    beacon_str = malloc(sizeof(char) * beacon_str_len + 1);
    if (beacon_str == NULL)
    {
        beacon_str = "Failed to retrieve beacon string";
        beacon_apr = apr_psprintf(mp, "ModSec-status: %s", beacon_str);
    }
    else
    {
        msc_beacon_string(beacon_str, beacon_str_len);
        beacon_apr = apr_psprintf(mp, "ModSec-status: %s", beacon_str);
        free(beacon_str);
    }

    if (key != NULL)
    {
        header_key = apr_psprintf(mp, "ModSec-key: %s", key);
    }

    if (curl)
    {
        struct curl_slist *headers_chunk = NULL;
        curl_easy_setopt(curl, CURLOPT_URL, remote_rules_server->uri);

        headers_chunk = curl_slist_append(headers_chunk, apr_id);
        headers_chunk = curl_slist_append(headers_chunk, beacon_apr);
        if (key != NULL)
        {
            headers_chunk = curl_slist_append(headers_chunk, header_key);
        }

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, msc_curl_write_memory_cb);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);

        /* some servers don't like requests that are made without a user-agent
           field, so we provide one */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "modesecurity");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers_chunk);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            if (remote_rules_fail_action == REMOTE_RULES_WARN_ON_FAIL)
            {
                 ap_log_error(APLOG_MARK, APLOG_NOTICE, 0, NULL,
                         "Failed to fetch \"%s\" error: %s ",
                         remote_rules_server->uri, curl_easy_strerror(res));
            }
            else
            {
                *error_msg = apr_psprintf(mp, "Failed to fetch \"%s\" " \
                    "error: %s ",
                    remote_rules_server->uri, curl_easy_strerror(res));
            }

            return -1;
        }

        curl_slist_free_all(headers_chunk);
    }

    curl_easy_cleanup(curl);

    curl_global_cleanup();
    return 0;
}

/**
 * @brief Setup an apr_crypto_key_t from a given password and salt.
 *
 * apr_crypto_* demands the key to be in a format of an apr_crypto_key_t which
 * is an structure that may be defined depending on the crypto provider, thus,
 * making necessary for us to create this structure using apr internal
 * functions.
 *
 * @warning We trust that the paramenter used in apr, such as the algorithm,
 *          key size and other parameters won't change, if they do it may
 *          break the interoperability with this function with others
 *          implementations, as the key will end up with a different value
 *          than the one expected.
 *
 * @param pool pointer to the memory pool to be used.
 * @param key password to be used while creating the key.
 * @param apr_key pointer to the pointer of an apr_crypto_key_t structure.
 * @param f pointer to apr_crypto_t.
 * @param salt string to be used as salt of the key generation
 * @param error_msg pointer to char pointer, which is filled if something
 *        went wrong.
 *
 * @retval n>=0 everything went fine.
 * @retval n<-1 Something wrong happened, check error_msg for further details.
 *
 */
int msc_remote_enc_key_setup(apr_pool_t *pool,
    const char *key,
    apr_crypto_key_t **apr_key,
    apr_crypto_t *f,
    unsigned char *salt,
    char **error_msg)
{
    apr_size_t key_len = strlen(key);
    apr_size_t salt_len = 16; //FIXME: salt_len should not be hard coded.

    const int do_pad = 1;
    apr_status_t rv;

    rv = apr_crypto_passphrase(
            (apr_crypto_key_t **) apr_key,
            NULL,
            (const char *) key,
            (apr_size_t) key_len,
            (const unsigned char *) salt,
            (apr_size_t) salt_len,
            APR_KEY_AES_256,
            APR_MODE_CBC,
            (const int) do_pad,
            (const int) 4096,
            (const apr_crypto_t *) f,
            (apr_pool_t *) pool);

    if (rv == APR_ENOKEY)
    {
        *error_msg = "Internal error - apr_crypto_passphrase: Missing key";
        return -1;
    }
    else if (rv == APR_EPADDING)
    {
        *error_msg = "Internal error - apr_crypto_passphrase: APR_EPADDING";
        return -1;
    }
    else if (rv == APR_EKEYTYPE)
    {
        *error_msg = "Internal error - apr_crypto_passphrase: APR_EKEYTYPE";
        return -1;
    }
    else if (rv == APR_EKEYTYPE)
    {
        *error_msg = "Internal error - apr_crypto_passphrase: APR_EKEYTYPE";
        return -1;
    }
    else if (rv != APR_SUCCESS)
    {
        *error_msg = "Internal error - apr_crypto_passphrase: Unknown error";
        return -1;
    }

    return 0;
}

/**
 * @brief Decrypt an buffer into a memory buffer.
 *
 * Decrypt an msc_curl_memory_buffer_t structure into another
 * msc_curl_memory_buffer_t.
 *
 * Using the key provided, it creates and apr_key and uses it to decript the
 * content provided on chunk. The plain text content is saved under
 * chunk_plain.
 *
 * @warning Cleanup memory after usage.
 *
 * @param pool pointer to the memory pool to be used.
 * @param key pointer to the char array to be used as the key.
 * @param chunk msc_curl_memory_buffer_t that contains the encrypted content.
 * @param plain_text unsigned char which will hold the content of an url
 * @param plain_text_len size of the plain_text buffer
 * @param error_msg pointer to char pointer that is filled if something went
 *        wrong.
 *
 * @retval n>=0 everything went fine.
 * @retval n<-1 Something wrong happened, further details on error_msg.
 *
 */
int msc_remote_decrypt(apr_pool_t *pool,
        const char *key,
        struct msc_curl_memory_buffer_t *chunk,
        unsigned char **plain_text,
        apr_size_t *plain_text_len,
        char **error_msg)
{
    apr_crypto_key_t *apr_key = NULL;
    apr_crypto_t *f = NULL;
    const apr_crypto_driver_t *driver = NULL;
    const apu_err_t *err = NULL;
    apr_status_t rv;
    unsigned char *iv = NULL;
    unsigned char *ciphered_text = NULL;
    unsigned char *salt = NULL;

    apr_crypto_block_t *block = NULL;
    apr_size_t block_size = 0;
    apr_size_t len = 0;

    // FIXME: size should not be hardcoded.
    //        at least size of IV + Salt
    if (chunk->size < 16+16+1)
    {
        *error_msg = "Failed to download rules from a remote server: " \
            "Unexpected content.";
        return -1;
    }
    iv = chunk->memory;
    salt = chunk->memory + 16;
    ciphered_text = chunk->memory + (16 + 16);

    rv = apr_crypto_init(pool);
    if (rv != APR_SUCCESS)
    {
        *error_msg = "Internal error: failed to init crypto";
        return -1;
    }

#ifndef APU_CRYPTO_RECOMMENDED_DRIVER
    rv = apr_crypto_get_driver(&driver, "openssl", NULL, &err, pool);
#else
    rv = apr_crypto_get_driver(&driver, APU_CRYPTO_RECOMMENDED_DRIVER, NULL,
            &err, pool);
#endif
    if (rv != APR_SUCCESS || driver == NULL)
    {
        *error_msg = "Internal error - apr_crypto_get_driver: Unknown error";
        return -1;
    }

    rv = apr_crypto_make(&f, driver, NULL, pool);
    if (rv != APR_SUCCESS)
    {
        *error_msg = "Internal error - apr_crypto_make: Unknown error";
        return -1;
    }

    msc_remote_enc_key_setup(pool, key, &apr_key, f, salt, error_msg);
    if (*error_msg != NULL)
    {
        return -1;
    }

    rv = apr_crypto_block_decrypt_init(&block, &block_size, iv, apr_key, pool);
    if (rv == APR_ENOKEY)
    {
        *error_msg = "Internal error - apr_crypto_block_decrypt_init: " \
            "Missing key";
        return -1;
    }
    else if (rv == APR_ENOIV)
    {
        *error_msg = "Internal error - apr_crypto_block_decrypt_init: " \
            "Missing IV";
        return -1;
    }
    else if (rv == APR_EKEYTYPE)
    {
        *error_msg = "Internal error - apr_crypto_block_decrypt_init: " \
            "Wrong key type";
        return -1;
    }
    else if (rv == APR_EKEYLENGTH)
    {
        *error_msg = "Internal error - apr_crypto_block_decrypt_init: " \
            "Wrong key length";
        return -1;
    }
    else if (rv != APR_SUCCESS)
    {
        *error_msg = "Internal error - apr_crypto_block_decrypt_init: " \
            "Unknown error";
        return -1;
    }

    //FIXME: size should not be hardcoded like that.
    //       32 = iv + salt size.
    rv = apr_crypto_block_decrypt(plain_text, plain_text_len,
        ciphered_text, (apr_size_t) chunk->size - (16 + 16) ,
        block);

    if (rv != APR_SUCCESS)
    {
        *error_msg = "Internal error - apr_crypto_block_decrypt: Failed to " \
            "decrypt";
        return -1;
    }

    /* finalise the decryption */

    rv = apr_crypto_block_decrypt_finish(*plain_text + *plain_text_len, &len,
            block);
    if (rv != APR_SUCCESS)
    {
        *error_msg = "Internal error - apr_crypto_block_decrypt_finish: " \
            "Failed to decrypt";
        return -1;
    }

    apr_crypto_block_cleanup(block);

    return 0;
}


/**
 * @brief Add SecRules from a given URI.
 *
 * Fetch the URI and using the key provided into params decrypt and install
 * the downloaded set of rules.
 *
 * @warning Cleanup the memory may be necessary.
 *
 * @param orig_parms origin parms used at SecRemoteRule
 * @param remote_rules_server pointer to the filled msc_remote_rules_server
 *          structure.
 * @param error_msg pointer to char pointer that will be filled if something
 *          went wrong.
 *
 *
 * @retval n>=0 everything went fine.
 * @retval n<-1 Something wrong happened, further details on error_msg.
 *
 */
int msc_remote_add_rules_from_uri(cmd_parms *orig_parms,
        msc_remote_rules_server *remote_rules_server,
        char **error_msg)
{
    struct msc_curl_memory_buffer_t chunk_encrypted;
    unsigned char *plain_text = NULL;
    int len = 0;
    int start = 0;
    int end = 0;
    int added_rules = 0;
    int res = 0;
    apr_size_t plain_text_len = 0;

    apr_pool_t *mp = orig_parms->pool;

    chunk_encrypted.size = 0;
    chunk_encrypted.memory = NULL;

    res = msc_remote_grab_content(mp, remote_rules_server->uri,
            remote_rules_server->key, &chunk_encrypted, error_msg);
    if (*error_msg != NULL)
    {
        return -1;
    }

    /* error_msg is not filled when the user set SecRemoteRulesFailAction
     * to warn
     */
    if (res != 0)
    {
        return res;
    }

    if (remote_rules_server->crypto == 1)
    {
        msc_remote_decrypt(mp, remote_rules_server->key, &chunk_encrypted,
            &plain_text,
            &plain_text_len,
            error_msg);
        if (*error_msg != NULL)
        {
            return -1;
        }

        msc_remote_clean_chunk(&chunk_encrypted);
    }
    else
    {
        plain_text = chunk_encrypted.memory;
        plain_text_len = strlen(plain_text);
    }

    len = 0;
    plain_text_len = strlen(plain_text);
    while (len < plain_text_len)
    {
        if (plain_text[len]  == '\n')
        {
            const char *rule = NULL;
            int tmp = len;
            char *cmd_name;
            char *word;
            const command_rec *cmd;
            ap_directive_t *newdir;
            cmd_parms *parms = apr_pcalloc(mp, sizeof (cmd_parms));

            rule = plain_text + start;
            end = len;
            plain_text[len] = '\0';
 
            memcpy(parms, orig_parms, sizeof(cmd_parms));

            if (*rule == '#' || *rule == '\0')
            {
               goto next;
            }

            cmd_name = ap_getword_conf(mp, &rule);
            cmd = ap_find_command(cmd_name, security2_module.cmds);

            if (cmd == NULL)
            {
                *error_msg = apr_pstrcat(mp, "Unknown command in config: ",
                        cmd_name, NULL);
                return -1;
            }

            newdir = apr_pcalloc(mp, sizeof(ap_directive_t));
            newdir->filename = "remote server";
            newdir->line_num = -1;
            newdir->directive = cmd_name;
            newdir->args = apr_pstrdup(mp, rule);
            parms->directive = newdir;

#ifdef WIN32
            // some config commands fail in APR when there are file
            // permission issues or other OS-specific problems
            //
            __try
            {
#endif
                *error_msg = (char *) msc_remote_invoke_cmd(cmd, parms,
                        remote_rules_server->context, rule);
                if (*error_msg != NULL)
                {
                    return -1;
                }

                added_rules++;
#ifdef WIN32
            }
            __except(EXCEPTION_EXECUTE_HANDLER)
            {
                *error_msg = "Command failed to execute (check file/folder" \
                    "permissions, syntax, etc.).";
                return -1;
            }
#endif

next:
            start = end + 1;
        }
        len++;
    }

    remote_rules_server->amount_of_rules = added_rules;

    if (remote_rules_server->crypto == 1)
    {
        msc_remote_clean_chunk(&chunk_encrypted);
    }
}


int msc_remote_clean_chunk(struct msc_curl_memory_buffer_t *chunk)
{
    if (chunk->size <= 0)
    {
        goto end;
    }

    if (chunk->memory == NULL)
    {
        goto end;
    }

    free(chunk->memory);
    chunk->size = 0;

end:
    return 0;
}

#endif
