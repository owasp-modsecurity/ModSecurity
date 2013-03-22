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

#include <limits.h>

#include "modsecurity.h"
#include "msc_logging.h"
#include "msc_util.h"
#include "http_log.h"
#include "apr_lib.h"
#include "acmp.h"
#include "msc_crypt.h"

#if defined(WITH_LUA)
#include "msc_lua.h"
#endif


/* -- Directory context creation and initialisation -- */

/**
 * Creates a fresh directory configuration.
 */
void *create_directory_config(apr_pool_t *mp, char *path)
{
    directory_config *dcfg = (directory_config *)apr_pcalloc(mp, sizeof(directory_config));
    if (dcfg == NULL) return NULL;

    #ifdef DEBUG_CONF
    ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, mp, "Created directory config %pp path %s", dcfg, path);
    #endif

    dcfg->mp = mp;
    dcfg->is_enabled = NOT_SET;

    dcfg->reqbody_access = NOT_SET;
    dcfg->reqintercept_oe = NOT_SET;
    dcfg->reqbody_buffering = NOT_SET;
    dcfg->reqbody_inmemory_limit = NOT_SET;
    dcfg->reqbody_limit = NOT_SET;
    dcfg->reqbody_no_files_limit = NOT_SET;
    dcfg->resbody_access = NOT_SET;

    dcfg->debuglog_name = NOT_SET_P;
    dcfg->debuglog_level = NOT_SET;
    dcfg->debuglog_fd = NOT_SET_P;

    dcfg->of_limit = NOT_SET;
    dcfg->if_limit_action = NOT_SET;
    dcfg->of_limit_action = NOT_SET;
    dcfg->of_mime_types = NOT_SET_P;
    dcfg->of_mime_types_cleared = NOT_SET;

    dcfg->cookie_format = NOT_SET;
    dcfg->argument_separator = NOT_SET;
    dcfg->cookiev0_separator = NOT_SET_P;

    dcfg->rule_inheritance = NOT_SET;
    dcfg->rule_exceptions = apr_array_make(mp, 16, sizeof(rule_exception *));
    dcfg->hash_method = apr_array_make(mp, 16, sizeof(hash_method *));

    /* audit log variables */
    dcfg->auditlog_flag = NOT_SET;
    dcfg->auditlog_type = NOT_SET;
    dcfg->max_rule_time = NOT_SET;
    dcfg->auditlog_dirperms = NOT_SET;
    dcfg->auditlog_fileperms = NOT_SET;
    dcfg->auditlog_name = NOT_SET_P;
    dcfg->auditlog2_name = NOT_SET_P;
    dcfg->auditlog_fd = NOT_SET_P;
    dcfg->auditlog2_fd = NOT_SET_P;
    dcfg->auditlog_storage_dir = NOT_SET_P;
    dcfg->auditlog_parts = NOT_SET_P;
    dcfg->auditlog_relevant_regex = NOT_SET_P;

    dcfg->ruleset = NULL;

    /* Upload */
    dcfg->tmp_dir = NOT_SET_P;
    dcfg->upload_dir = NOT_SET_P;
    dcfg->upload_keep_files = NOT_SET;
    dcfg->upload_validates_files = NOT_SET;
    dcfg->upload_filemode = NOT_SET;
    dcfg->upload_file_limit = NOT_SET;

    /* These are only used during the configuration process. */
    dcfg->tmp_chain_starter = NULL;
    dcfg->tmp_default_actionset = NULL;
    dcfg->tmp_rule_placeholders = NULL;

    /* Misc */
    dcfg->data_dir = NOT_SET_P;
    dcfg->webappid = NOT_SET_P;
    dcfg->sensor_id = NOT_SET_P;
    dcfg->httpBlkey = NOT_SET_P;

    /* Content injection. */
    dcfg->content_injection_enabled = NOT_SET;

    /* Stream inspection */
    dcfg->stream_inbody_inspection = NOT_SET;
    dcfg->stream_outbody_inspection = NOT_SET;

    /* Geo Lookups */
    dcfg->geo = NOT_SET_P;

    /* Gsb Lookups */
    dcfg->gsb = NOT_SET_P;

    /* Unicode Map */
    dcfg->u_map = NOT_SET_P;

    /* Cache */
    dcfg->cache_trans = NOT_SET;
    dcfg->cache_trans_incremental = NOT_SET;
    dcfg->cache_trans_min = NOT_SET;
    dcfg->cache_trans_max = NOT_SET;
    dcfg->cache_trans_maxitems = NOT_SET;

    /* Rule ids */
    dcfg->rule_id_htab = apr_hash_make(mp);
    dcfg->component_signatures = apr_array_make(mp, 16, sizeof(char *));

    dcfg->request_encoding = NOT_SET_P;
    dcfg->disable_backend_compression = NOT_SET;

    /* Collection timeout */
    dcfg->col_timeout = NOT_SET;

    dcfg->crypto_key = NOT_SET_P;
    dcfg->crypto_key_len = NOT_SET;
    dcfg->crypto_key_add = NOT_SET;
    dcfg->crypto_param_name = NOT_SET_P;
    dcfg->hash_is_enabled = NOT_SET;
    dcfg->hash_enforcement = NOT_SET;
    dcfg->crypto_hash_href_rx = NOT_SET;
    dcfg->crypto_hash_faction_rx = NOT_SET;
    dcfg->crypto_hash_location_rx = NOT_SET;
    dcfg->crypto_hash_iframesrc_rx = NOT_SET;
    dcfg->crypto_hash_framesrc_rx = NOT_SET;
    dcfg->crypto_hash_href_pm = NOT_SET;
    dcfg->crypto_hash_faction_pm = NOT_SET;
    dcfg->crypto_hash_location_pm = NOT_SET;
    dcfg->crypto_hash_iframesrc_pm = NOT_SET;
    dcfg->crypto_hash_framesrc_pm = NOT_SET;


    /* xml external entity */
    dcfg->xml_external_entity = NOT_SET;

    return dcfg;
}

/**
 * Copies rules between one phase of two configuration contexts,
 * taking exceptions into account.
 */
static void copy_rules_phase(apr_pool_t *mp,
                             apr_array_header_t *parent_phase_arr,
                             apr_array_header_t *child_phase_arr,
                             apr_array_header_t *exceptions_arr)
{
    rule_exception **exceptions;
    msre_rule **rules;
    int i, j;
    int mode = 0;

    rules = (msre_rule **)parent_phase_arr->elts;
    for(i = 0; i < parent_phase_arr->nelts; i++) {
        msre_rule *rule = (msre_rule *)rules[i];
        int copy = 1;

        if (mode == 0) {
            /* First rule in the chain. */
            exceptions = (rule_exception **)exceptions_arr->elts;
            for(j = 0; j < exceptions_arr->nelts; j++) {

                /* Process exceptions. */
                switch(exceptions[j]->type) {
                    case RULE_EXCEPTION_REMOVE_ID :
                        if ((rule->actionset != NULL)&&(rule->actionset->id != NULL)) {
                            int ruleid = atoi(rule->actionset->id);
                            if (rule_id_in_range(ruleid, exceptions[j]->param)) copy--;
                        }
                        break;
                    case RULE_EXCEPTION_REMOVE_MSG :
                        if ((rule->actionset != NULL)&&(rule->actionset->msg != NULL)) {
                            char *my_error_msg = NULL;

                            int rc = msc_regexec(exceptions[j]->param_data,
                                    rule->actionset->msg, strlen(rule->actionset->msg),
                                    &my_error_msg);
                            if (rc >= 0) copy--;
                        }
                        break;
                    case RULE_EXCEPTION_REMOVE_TAG :
                        if ((rule->actionset != NULL)&&(apr_is_empty_table(rule->actionset->actions) == 0)) {
                            char *my_error_msg = NULL;
                            const apr_array_header_t *tarr = NULL;
                            const apr_table_entry_t *telts = NULL;
                            int c;

                            tarr = apr_table_elts(rule->actionset->actions);
                            telts = (const apr_table_entry_t*)tarr->elts;

                            for (c = 0; c < tarr->nelts; c++) {
                                msre_action *action = (msre_action *)telts[c].val;
                                if(strcmp("tag", action->metadata->name) == 0)  {

                                    int rc = msc_regexec(exceptions[j]->param_data,
                                            action->param, strlen(action->param),
                                            &my_error_msg);
                                    if (rc >= 0) copy--;
                                }
                            }
                        }
                        break;
                }
            }

            if (copy > 0) {
#ifdef DEBUG_CONF
                ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, mp, "Copy rule %pp [id \"%s\"]", rule, rule->actionset->id);
#endif

                /* Copy the rule. */
                *(msre_rule **)apr_array_push(child_phase_arr) = rule;
                if (rule->actionset->is_chained) mode = 2;
            } else {
                if (rule->actionset->is_chained) mode = 1;
            }
        } else {
            if (mode == 2) {
#ifdef DEBUG_CONF
                ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, mp, "Copy chain %pp for rule %pp [id \"%s\"]", rule, rule->chain_starter, rule->chain_starter->actionset->id);
#endif

                /* Copy the rule (it belongs to the chain we want to include. */
                *(msre_rule **)apr_array_push(child_phase_arr) = rule;
            }

            if ((rule->actionset == NULL)||(rule->actionset->is_chained == 0)) mode = 0;
        }
    }
}

/**
 * Copies rules between two configuration contexts,
 * taking exceptions into account.
 */
static int copy_rules(apr_pool_t *mp, msre_ruleset *parent_ruleset,
                      msre_ruleset *child_ruleset,
                      apr_array_header_t *exceptions_arr)
{
    copy_rules_phase(mp, parent_ruleset->phase_request_headers,
        child_ruleset->phase_request_headers, exceptions_arr);
    copy_rules_phase(mp, parent_ruleset->phase_request_body,
        child_ruleset->phase_request_body, exceptions_arr);
    copy_rules_phase(mp, parent_ruleset->phase_response_headers,
        child_ruleset->phase_response_headers, exceptions_arr);
    copy_rules_phase(mp, parent_ruleset->phase_response_body,
        child_ruleset->phase_response_body, exceptions_arr);
    copy_rules_phase(mp, parent_ruleset->phase_logging,
        child_ruleset->phase_logging, exceptions_arr);

    return 1;
}

/**
 * Merges two directory configurations.
 */
void *merge_directory_configs(apr_pool_t *mp, void *_parent, void *_child)
{
    directory_config *parent = (directory_config *)_parent;
    directory_config *child = (directory_config *)_child;
    directory_config *merged = create_directory_config(mp, NULL);

    #ifdef DEBUG_CONF
    ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, mp, "Merge parent %pp child %pp RESULT %pp", _parent, _child, merged);
    #endif

    if (merged == NULL) return NULL;

    /* Use values from the child configuration where possible,
     * otherwise use the parent's.
     */

    merged->is_enabled = (child->is_enabled == NOT_SET
        ? parent->is_enabled : child->is_enabled);

    /* IO parameters */
    merged->reqbody_access = (child->reqbody_access == NOT_SET
        ? parent->reqbody_access : child->reqbody_access);
    merged->reqbody_buffering = (child->reqbody_buffering == NOT_SET
        ? parent->reqbody_buffering : child->reqbody_buffering);
    merged->reqbody_inmemory_limit = (child->reqbody_inmemory_limit == NOT_SET
        ? parent->reqbody_inmemory_limit : child->reqbody_inmemory_limit);
    merged->reqbody_limit = (child->reqbody_limit == NOT_SET
        ? parent->reqbody_limit : child->reqbody_limit);
    merged->reqbody_no_files_limit = (child->reqbody_no_files_limit == NOT_SET
        ? parent->reqbody_no_files_limit : child->reqbody_no_files_limit);
    merged->resbody_access = (child->resbody_access == NOT_SET
        ? parent->resbody_access : child->resbody_access);

    merged->of_limit = (child->of_limit == NOT_SET
        ? parent->of_limit : child->of_limit);
    merged->if_limit_action = (child->if_limit_action == NOT_SET
        ? parent->if_limit_action : child->if_limit_action);
    merged->of_limit_action = (child->of_limit_action == NOT_SET
        ? parent->of_limit_action : child->of_limit_action);
    merged->reqintercept_oe = (child->reqintercept_oe == NOT_SET
        ? parent->reqintercept_oe : child->reqintercept_oe);

    if (child->of_mime_types != NOT_SET_P) {
        /* Child added to the table */

        if (child->of_mime_types_cleared == 1) {
            /* The list of MIME types was cleared in the child,
             * which means the parent's MIME types went away and
             * we should not take them into consideration here.
             */
            merged->of_mime_types = child->of_mime_types;
            merged->of_mime_types_cleared = 1;
        } else {
            /* Add MIME types defined in the child to those
             * defined in the parent context.
             */
            if (parent->of_mime_types == NOT_SET_P) {
                merged->of_mime_types = child->of_mime_types;
                merged->of_mime_types_cleared = NOT_SET;
            } else {
                merged->of_mime_types = apr_table_overlay(mp, parent->of_mime_types,
                    child->of_mime_types);
                if (merged->of_mime_types == NULL) return NULL;
            }
        }
    } else {
        /* Child did not add to the table */

        if (child->of_mime_types_cleared == 1) {
            merged->of_mime_types_cleared = 1;
        } else {
            merged->of_mime_types = parent->of_mime_types;
            merged->of_mime_types_cleared = parent->of_mime_types_cleared;
        }
    }

    /* debug log */
    if (child->debuglog_fd == NOT_SET_P) {
        merged->debuglog_name = parent->debuglog_name;
        merged->debuglog_fd = parent->debuglog_fd;
    } else {
        merged->debuglog_name = child->debuglog_name;
        merged->debuglog_fd = child->debuglog_fd;
    }

    merged->debuglog_level = (child->debuglog_level == NOT_SET
        ? parent->debuglog_level : child->debuglog_level);

    merged->cookie_format = (child->cookie_format == NOT_SET
        ? parent->cookie_format : child->cookie_format);
    merged->argument_separator = (child->argument_separator == NOT_SET
        ? parent->argument_separator : child->argument_separator);
    merged->cookiev0_separator = (child->cookiev0_separator == NOT_SET_P
        ? parent->cookiev0_separator : child->cookiev0_separator);


    /* rule inheritance */
    if ((child->rule_inheritance == NOT_SET)||(child->rule_inheritance == 1)) {
        merged->rule_inheritance = parent->rule_inheritance;
        if ((child->ruleset == NULL)&&(parent->ruleset == NULL)) {
            #ifdef DEBUG_CONF
            ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, mp, "No rules in this context.");
            #endif

            /* Do nothing, there are no rules in either context. */
        } else
        if (child->ruleset == NULL) {
            #ifdef DEBUG_CONF
            ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, mp, "Using parent rules in this context.");
            #endif

            /* Copy the rules from the parent context. */
            merged->ruleset = msre_ruleset_create(parent->ruleset->engine, mp);
            copy_rules(mp, parent->ruleset, merged->ruleset, child->rule_exceptions);
        } else
        if (parent->ruleset == NULL) {
            #ifdef DEBUG_CONF
            ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, mp, "Using child rules in this context.");
            #endif

            /* Copy child rules. */
            merged->ruleset = msre_ruleset_create(child->ruleset->engine, mp);
            merged->ruleset->phase_request_headers = apr_array_copy(mp,
                child->ruleset->phase_request_headers);
            merged->ruleset->phase_request_body = apr_array_copy(mp,
                child->ruleset->phase_request_body);
            merged->ruleset->phase_response_headers = apr_array_copy(mp,
                child->ruleset->phase_response_headers);
            merged->ruleset->phase_response_body = apr_array_copy(mp,
                child->ruleset->phase_response_body);
            merged->ruleset->phase_logging = apr_array_copy(mp,
                child->ruleset->phase_logging);
        } else {
            #ifdef DEBUG_CONF
            ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, mp, "Using parent then child rules in this context.");
            #endif

            /* Copy parent rules, then add child rules to it. */
            merged->ruleset = msre_ruleset_create(parent->ruleset->engine, mp);
            copy_rules(mp, parent->ruleset, merged->ruleset, child->rule_exceptions);

            apr_array_cat(merged->ruleset->phase_request_headers,
                child->ruleset->phase_request_headers);
            apr_array_cat(merged->ruleset->phase_request_body,
                child->ruleset->phase_request_body);
            apr_array_cat(merged->ruleset->phase_response_headers,
                child->ruleset->phase_response_headers);
            apr_array_cat(merged->ruleset->phase_response_body,
                child->ruleset->phase_response_body);
            apr_array_cat(merged->ruleset->phase_logging,
                child->ruleset->phase_logging);
        }
    } else {
        merged->rule_inheritance = 0;
        if (child->ruleset != NULL) {
            /* Copy child rules. */
            merged->ruleset = msre_ruleset_create(child->ruleset->engine, mp);
            merged->ruleset->phase_request_headers = apr_array_copy(mp,
                child->ruleset->phase_request_headers);
            merged->ruleset->phase_request_body = apr_array_copy(mp,
                child->ruleset->phase_request_body);
            merged->ruleset->phase_response_headers = apr_array_copy(mp,
                child->ruleset->phase_response_headers);
            merged->ruleset->phase_response_body = apr_array_copy(mp,
                child->ruleset->phase_response_body);
            merged->ruleset->phase_logging = apr_array_copy(mp,
                child->ruleset->phase_logging);
        }
    }

    /* Merge rule exceptions. */
    merged->rule_exceptions = apr_array_append(mp, parent->rule_exceptions,
        child->rule_exceptions);

    merged->hash_method = apr_array_append(mp, parent->hash_method,
        child->hash_method);

    /* audit log variables */
    merged->auditlog_flag = (child->auditlog_flag == NOT_SET
        ? parent->auditlog_flag : child->auditlog_flag);
    merged->auditlog_type = (child->auditlog_type == NOT_SET
        ? parent->auditlog_type : child->auditlog_type);
    merged->max_rule_time = (child->max_rule_time == NOT_SET
        ? parent->max_rule_time : child->max_rule_time);
    merged->auditlog_dirperms = (child->auditlog_dirperms == NOT_SET
        ? parent->auditlog_dirperms : child->auditlog_dirperms);
    merged->auditlog_fileperms = (child->auditlog_fileperms == NOT_SET
        ? parent->auditlog_fileperms : child->auditlog_fileperms);
    if (child->auditlog_fd != NOT_SET_P) {
        merged->auditlog_fd = child->auditlog_fd;
        merged->auditlog_name = child->auditlog_name;
    } else {
        merged->auditlog_fd = parent->auditlog_fd;
        merged->auditlog_name = parent->auditlog_name;
    }
    if (child->auditlog2_fd != NOT_SET_P) {
        merged->auditlog2_fd = child->auditlog2_fd;
        merged->auditlog2_name = child->auditlog2_name;
    } else {
        merged->auditlog2_fd = parent->auditlog2_fd;
        merged->auditlog2_name = parent->auditlog2_name;
    }
    merged->auditlog_storage_dir = (child->auditlog_storage_dir == NOT_SET_P
        ? parent->auditlog_storage_dir : child->auditlog_storage_dir);
    merged->auditlog_parts = (child->auditlog_parts == NOT_SET_P
        ? parent->auditlog_parts : child->auditlog_parts);
    merged->auditlog_relevant_regex = (child->auditlog_relevant_regex == NOT_SET_P
        ? parent->auditlog_relevant_regex : child->auditlog_relevant_regex);

    /* Upload */
    merged->tmp_dir = (child->tmp_dir == NOT_SET_P
        ? parent->tmp_dir : child->tmp_dir);
    merged->upload_dir = (child->upload_dir == NOT_SET_P
        ? parent->upload_dir : child->upload_dir);
    merged->upload_keep_files = (child->upload_keep_files == NOT_SET
        ? parent->upload_keep_files : child->upload_keep_files);
    merged->upload_validates_files = (child->upload_validates_files == NOT_SET
        ? parent->upload_validates_files : child->upload_validates_files);
    merged->upload_filemode = (child->upload_filemode == NOT_SET
        ? parent->upload_filemode : child->upload_filemode);
    merged->upload_file_limit = (child->upload_file_limit == NOT_SET
        ? parent->upload_file_limit : child->upload_file_limit);

    /* Misc */
    merged->data_dir = (child->data_dir == NOT_SET_P
        ? parent->data_dir : child->data_dir);
    merged->webappid = (child->webappid == NOT_SET_P
        ? parent->webappid : child->webappid);
    merged->sensor_id = (child->sensor_id == NOT_SET_P
        ? parent->sensor_id : child->sensor_id);
    merged->httpBlkey = (child->httpBlkey == NOT_SET_P
        ? parent->httpBlkey : child->httpBlkey);

    /* Content injection. */
    merged->content_injection_enabled = (child->content_injection_enabled == NOT_SET
        ? parent->content_injection_enabled : child->content_injection_enabled);

    /* Stream inspection */
    merged->stream_inbody_inspection = (child->stream_inbody_inspection == NOT_SET
        ? parent->stream_inbody_inspection : child->stream_inbody_inspection);
    merged->stream_outbody_inspection = (child->stream_outbody_inspection == NOT_SET
        ? parent->stream_outbody_inspection : child->stream_outbody_inspection);

    /* Geo Lookup */
    merged->geo = (child->geo == NOT_SET_P
        ? parent->geo : child->geo);

    /* Gsb Lookup */
    merged->gsb = (child->gsb == NOT_SET_P
        ? parent->gsb : child->gsb);

    /* Unicode Map */
    merged->u_map = (child->u_map == NOT_SET_P
        ? parent->u_map : child->u_map);

    /* Cache */
    merged->cache_trans = (child->cache_trans == NOT_SET
        ? parent->cache_trans : child->cache_trans);
    merged->cache_trans_incremental = (child->cache_trans_incremental == NOT_SET
        ? parent->cache_trans_incremental : child->cache_trans_incremental);
    merged->cache_trans_min = (child->cache_trans_min == (apr_size_t)NOT_SET
        ? parent->cache_trans_min : child->cache_trans_min);
    merged->cache_trans_max = (child->cache_trans_max == (apr_size_t)NOT_SET
        ? parent->cache_trans_max : child->cache_trans_max);
    merged->cache_trans_maxitems = (child->cache_trans_maxitems == (apr_size_t)NOT_SET
        ? parent->cache_trans_maxitems : child->cache_trans_maxitems);

    /* Merge component signatures. */
    merged->component_signatures = apr_array_append(mp, parent->component_signatures,
        child->component_signatures);

    merged->request_encoding = (child->request_encoding == NOT_SET_P
        ? parent->request_encoding : child->request_encoding);

    merged->disable_backend_compression = (child->disable_backend_compression == NOT_SET
        ? parent->disable_backend_compression : child->disable_backend_compression);

    merged->col_timeout = (child->col_timeout == NOT_SET
        ? parent->col_timeout : child->col_timeout);

    /* Hash */
    merged->crypto_key = (child->crypto_key == NOT_SET_P
        ? parent->crypto_key : child->crypto_key);
    merged->crypto_key_len = (child->crypto_key_len == NOT_SET
        ? parent->crypto_key_len : child->crypto_key_len);
    merged->crypto_key_add = (child->crypto_key_add == NOT_SET
        ? parent->crypto_key_add : child->crypto_key_add);
    merged->crypto_param_name = (child->crypto_param_name == NOT_SET_P
        ? parent->crypto_param_name : child->crypto_param_name);
    merged->hash_is_enabled = (child->hash_is_enabled == NOT_SET
        ? parent->hash_is_enabled : child->hash_is_enabled);
    merged->hash_enforcement = (child->hash_enforcement == NOT_SET
        ? parent->hash_enforcement : child->hash_enforcement);
    merged->crypto_hash_href_rx = (child->crypto_hash_href_rx == NOT_SET
        ? parent->crypto_hash_href_rx : child->crypto_hash_href_rx);
    merged->crypto_hash_faction_rx = (child->crypto_hash_faction_rx == NOT_SET
        ? parent->crypto_hash_faction_rx : child->crypto_hash_faction_rx);
    merged->crypto_hash_location_rx = (child->crypto_hash_location_rx == NOT_SET
        ? parent->crypto_hash_location_rx : child->crypto_hash_location_rx);
    merged->crypto_hash_iframesrc_rx = (child->crypto_hash_iframesrc_rx == NOT_SET
        ? parent->crypto_hash_iframesrc_rx : child->crypto_hash_iframesrc_rx);
    merged->crypto_hash_framesrc_rx = (child->crypto_hash_framesrc_rx == NOT_SET
        ? parent->crypto_hash_framesrc_rx : child->crypto_hash_framesrc_rx);
    merged->crypto_hash_href_pm = (child->crypto_hash_href_pm == NOT_SET
        ? parent->crypto_hash_href_pm : child->crypto_hash_href_pm);
    merged->crypto_hash_faction_pm = (child->crypto_hash_faction_pm == NOT_SET
        ? parent->crypto_hash_faction_pm : child->crypto_hash_faction_pm);
    merged->crypto_hash_location_pm = (child->crypto_hash_location_pm == NOT_SET
        ? parent->crypto_hash_location_pm : child->crypto_hash_location_pm);
    merged->crypto_hash_iframesrc_pm = (child->crypto_hash_iframesrc_pm == NOT_SET
        ? parent->crypto_hash_iframesrc_pm : child->crypto_hash_iframesrc_pm);
    merged->crypto_hash_framesrc_pm = (child->crypto_hash_framesrc_pm == NOT_SET
        ? parent->crypto_hash_framesrc_pm : child->crypto_hash_framesrc_pm);

    /* xml external entity */
    merged->xml_external_entity = (child->xml_external_entity == NOT_SET
        ? parent->xml_external_entity : child->xml_external_entity);

    return merged;
}

/**
 * Initialise directory configuration. This function is *not* meant
 * to be called for directory configuration instances created during
 * the configuration phase. It can only be called on copies of those
 * (created fresh for every transaction).
 */
void init_directory_config(directory_config *dcfg)
{
    if (dcfg == NULL) return;

    if (dcfg->is_enabled == NOT_SET) dcfg->is_enabled = 0;

    if (dcfg->reqbody_access == NOT_SET) dcfg->reqbody_access = 0;
    if (dcfg->reqintercept_oe == NOT_SET) dcfg->reqintercept_oe = 0;
    if (dcfg->reqbody_buffering == NOT_SET) dcfg->reqbody_buffering = REQUEST_BODY_FORCEBUF_OFF;
    if (dcfg->reqbody_inmemory_limit == NOT_SET)
        dcfg->reqbody_inmemory_limit = REQUEST_BODY_DEFAULT_INMEMORY_LIMIT;
    if (dcfg->reqbody_limit == NOT_SET) dcfg->reqbody_limit = REQUEST_BODY_DEFAULT_LIMIT;
    if (dcfg->reqbody_no_files_limit == NOT_SET) dcfg->reqbody_no_files_limit = REQUEST_BODY_NO_FILES_DEFAULT_LIMIT;
    if (dcfg->resbody_access == NOT_SET) dcfg->resbody_access = 0;
    if (dcfg->of_limit == NOT_SET) dcfg->of_limit = RESPONSE_BODY_DEFAULT_LIMIT;
    if (dcfg->if_limit_action == NOT_SET) dcfg->if_limit_action = REQUEST_BODY_LIMIT_ACTION_REJECT;
    if (dcfg->of_limit_action == NOT_SET) dcfg->of_limit_action = RESPONSE_BODY_LIMIT_ACTION_REJECT;

    if (dcfg->of_mime_types == NOT_SET_P) {
        dcfg->of_mime_types = apr_table_make(dcfg->mp, 3);
        if (dcfg->of_mime_types_cleared != 1) {
            apr_table_setn(dcfg->of_mime_types, "text/plain", "1");
            apr_table_setn(dcfg->of_mime_types, "text/html", "1");
        }
    }

    if (dcfg->debuglog_fd == NOT_SET_P) dcfg->debuglog_fd = NULL;
    if (dcfg->debuglog_name == NOT_SET_P) dcfg->debuglog_name = NULL;
    if (dcfg->debuglog_level == NOT_SET) dcfg->debuglog_level = 0;

    if (dcfg->cookie_format == NOT_SET) dcfg->cookie_format = 0;
    if (dcfg->argument_separator == NOT_SET) dcfg->argument_separator = '&';
    if (dcfg->cookiev0_separator == NOT_SET_P) dcfg->cookiev0_separator = NULL;

    if (dcfg->rule_inheritance == NOT_SET) dcfg->rule_inheritance = 1;

    /* audit log variables */
    if (dcfg->auditlog_flag == NOT_SET) dcfg->auditlog_flag = 0;
    if (dcfg->auditlog_type == NOT_SET) dcfg->auditlog_type = AUDITLOG_SERIAL;
    if (dcfg->max_rule_time == NOT_SET) dcfg->max_rule_time = 0;
    if (dcfg->auditlog_dirperms == NOT_SET) dcfg->auditlog_dirperms = CREATEMODE_DIR;
    if (dcfg->auditlog_fileperms == NOT_SET) dcfg->auditlog_fileperms = CREATEMODE;
    if (dcfg->auditlog_fd == NOT_SET_P) dcfg->auditlog_fd = NULL;
    if (dcfg->auditlog2_fd == NOT_SET_P) dcfg->auditlog2_fd = NULL;
    if (dcfg->auditlog_name == NOT_SET_P) dcfg->auditlog_name = NULL;
    if (dcfg->auditlog2_name == NOT_SET_P) dcfg->auditlog2_name = NULL;
    if (dcfg->auditlog_storage_dir == NOT_SET_P) dcfg->auditlog_storage_dir = NULL;
    if (dcfg->auditlog_parts == NOT_SET_P) dcfg->auditlog_parts = "ABCFHZ";
    if (dcfg->auditlog_relevant_regex == NOT_SET_P) dcfg->auditlog_relevant_regex = NULL;

    /* Upload */
    if (dcfg->tmp_dir == NOT_SET_P) dcfg->tmp_dir = guess_tmp_dir(dcfg->mp);
    if (dcfg->upload_dir == NOT_SET_P) dcfg->upload_dir = NULL;
    if (dcfg->upload_keep_files == NOT_SET) dcfg->upload_keep_files = KEEP_FILES_OFF;
    if (dcfg->upload_validates_files == NOT_SET) dcfg->upload_validates_files = 0;
    if (dcfg->upload_filemode == NOT_SET) dcfg->upload_filemode = 0600;
    if (dcfg->upload_file_limit == NOT_SET) dcfg->upload_file_limit = 100;

    /* Misc */
    if (dcfg->data_dir == NOT_SET_P) dcfg->data_dir = NULL;
    if (dcfg->webappid == NOT_SET_P) dcfg->webappid = "default";
    if (dcfg->sensor_id == NOT_SET_P) dcfg->sensor_id = "default";
    if (dcfg->httpBlkey == NOT_SET_P) dcfg->httpBlkey = NULL;

    /* Content injection. */
    if (dcfg->content_injection_enabled == NOT_SET) dcfg->content_injection_enabled = 0;

    /* Stream inspection */
    if (dcfg->stream_inbody_inspection == NOT_SET) dcfg->stream_inbody_inspection = 0;
    if (dcfg->stream_outbody_inspection == NOT_SET) dcfg->stream_outbody_inspection = 0;

    /* Geo Lookup */
    if (dcfg->geo == NOT_SET_P) dcfg->geo = NULL;

    /* Gsb Lookup */
    if (dcfg->gsb == NOT_SET_P) dcfg->gsb = NULL;

    /* Unicode Map */
    if (dcfg->u_map == NOT_SET_P) dcfg->u_map = NULL;

    /* Cache */
    if (dcfg->cache_trans == NOT_SET) dcfg->cache_trans = MODSEC_CACHE_DISABLED;
    if (dcfg->cache_trans_incremental == NOT_SET) dcfg->cache_trans_incremental = 0;
    if (dcfg->cache_trans_min == (apr_size_t)NOT_SET) dcfg->cache_trans_min = 32;
    if (dcfg->cache_trans_max == (apr_size_t)NOT_SET) dcfg->cache_trans_max = 1024;
    if (dcfg->cache_trans_maxitems == (apr_size_t)NOT_SET) dcfg->cache_trans_maxitems = 512;

    if (dcfg->request_encoding == NOT_SET_P) dcfg->request_encoding = NULL;

    if (dcfg->disable_backend_compression == NOT_SET) dcfg->disable_backend_compression = 0;

    if (dcfg->col_timeout == NOT_SET) dcfg->col_timeout = 3600;

    /* Hash */
    if (dcfg->crypto_key == NOT_SET_P) dcfg->crypto_key = getkey(dcfg->mp);
    if (dcfg->crypto_key_len == NOT_SET) dcfg->crypto_key_len = strlen(dcfg->crypto_key);
    if (dcfg->crypto_key_add == NOT_SET) dcfg->crypto_key_add = HASH_KEYONLY;
    if (dcfg->crypto_param_name == NOT_SET_P) dcfg->crypto_param_name = "crypt";
    if (dcfg->hash_is_enabled == NOT_SET) dcfg->hash_is_enabled = HASH_DISABLED;
    if (dcfg->hash_enforcement == NOT_SET) dcfg->hash_enforcement = HASH_DISABLED;
    if (dcfg->crypto_hash_href_rx == NOT_SET) dcfg->crypto_hash_href_rx = 0;
    if (dcfg->crypto_hash_faction_rx == NOT_SET) dcfg->crypto_hash_faction_rx = 0;
    if (dcfg->crypto_hash_location_rx == NOT_SET) dcfg->crypto_hash_location_rx = 0;
    if (dcfg->crypto_hash_iframesrc_rx == NOT_SET) dcfg->crypto_hash_iframesrc_rx = 0;
    if (dcfg->crypto_hash_framesrc_rx == NOT_SET) dcfg->crypto_hash_framesrc_rx = 0;
    if (dcfg->crypto_hash_href_pm == NOT_SET) dcfg->crypto_hash_href_pm = 0;
    if (dcfg->crypto_hash_faction_pm == NOT_SET) dcfg->crypto_hash_faction_pm = 0;
    if (dcfg->crypto_hash_location_pm == NOT_SET) dcfg->crypto_hash_location_pm = 0;
    if (dcfg->crypto_hash_iframesrc_pm == NOT_SET) dcfg->crypto_hash_iframesrc_pm = 0;
    if (dcfg->crypto_hash_framesrc_pm == NOT_SET) dcfg->crypto_hash_framesrc_pm = 0;

    /* xml external entity */
    if (dcfg->xml_external_entity == NOT_SET) dcfg->xml_external_entity = 0;

}

/**
 *
 */
static const char *add_rule(cmd_parms *cmd, directory_config *dcfg, int type,
                            const char *p1, const char *p2, const char *p3)
{
    char *my_error_msg = NULL;
    //msre_rule *rule = NULL, *tmp_rule = NULL;
    char *rid = NULL;
    msre_rule *rule = NULL;
    extern msc_engine *modsecurity;
    int offset = 0;

    #ifdef DEBUG_CONF
    ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, cmd->pool,
        "Rule: type=%d p1='%s' p2='%s' p3='%s'", type, p1, p2, p3);
    #endif

    /* Create a ruleset if one does not exist. */
    if ((dcfg->ruleset == NULL)||(dcfg->ruleset == NOT_SET_P)) {
        dcfg->ruleset = msre_ruleset_create(modsecurity->msre, cmd->pool);
        if (dcfg->ruleset == NULL) return FATAL_ERROR;
    }

    /* Create the rule now. */
    switch(type) {
        #if defined(WITH_LUA)
        case RULE_TYPE_LUA :
            rule = msre_rule_lua_create(dcfg->ruleset, cmd->directive->filename,
                cmd->directive->line_num, p1, p2, &my_error_msg);
            break;
        #endif
        default :
            rule = msre_rule_create(dcfg->ruleset, type, cmd->directive->filename,
                cmd->directive->line_num, p1, p2, p3, &my_error_msg);
            break;
    }

    if (rule == NULL) {
        return my_error_msg;
    }

    /* Rules must have uniq ID */
    if (
#if defined(WITH_LUA)
            type != RULE_TYPE_LUA &&
#endif
            (dcfg->tmp_chain_starter == NULL))
                if(rule->actionset == NULL)
                    return "ModSecurity: Rules must have at least id action";

    if(rule->actionset != NULL && (dcfg->tmp_chain_starter == NULL))    {
        if(rule->actionset->id == NOT_SET_P
#if defined(WITH_LUA)
            && (type != RULE_TYPE_LUA)
#endif
          )
            return "ModSecurity: No action id present within the rule";
#if defined(WITH_LUA)
        if(type != RULE_TYPE_LUA)
#endif
        {
            rid = apr_hash_get(dcfg->rule_id_htab, rule->actionset->id, APR_HASH_KEY_STRING);
            if(rid != NULL) {
                return "ModSecurity: Found another rule with the same id";
            } else    {
                apr_hash_set(dcfg->rule_id_htab, apr_pstrdup(dcfg->mp, rule->actionset->id), APR_HASH_KEY_STRING, apr_pstrdup(dcfg->mp, "1"));
            }

            //tmp_rule = msre_ruleset_fetch_rule(dcfg->ruleset, rule->actionset->id, offset);
            //if(tmp_rule != NULL)
            //    return "ModSecurity: Found another rule with the same id";
        }
    }

    /* Create default actionset if one does not already exist. */
    if (dcfg->tmp_default_actionset == NULL) {
        dcfg->tmp_default_actionset = msre_actionset_create_default(modsecurity->msre);
        if (dcfg->tmp_default_actionset == NULL) return FATAL_ERROR;
    }

    /* Check some cases prior to merging so we know where it came from */

    /* Check syntax for chained rules */
    if ((rule->actionset != NULL) && (dcfg->tmp_chain_starter != NULL)) {
        /* Must NOT specify a disruptive action. */
        if (rule->actionset->intercept_action != NOT_SET) {
            return apr_psprintf(cmd->pool, "ModSecurity: Disruptive actions can only "
                "be specified by chain starter rules.");
        }

        /* Must NOT specify a skipafter action. */
        if (rule->actionset->skip_after != NOT_SET_P) {
            return apr_psprintf(cmd->pool, "ModSecurity: SkipAfter actions can only "
                "be specified by chain starter rules.");
        }

        /* Must NOT specify a phase. */
        if (rule->actionset->phase != NOT_SET) {
            return apr_psprintf(cmd->pool, "ModSecurity: Execution phases can only be "
                "specified by chain starter rules.");
        }

        /* Must NOT use metadata actions. */
        /* ENH: loop through to check for tags */
        if ((rule->actionset->id != NOT_SET_P)
            ||(rule->actionset->rev != NOT_SET_P)
            ||(rule->actionset->msg != NOT_SET_P)
            ||(rule->actionset->severity != NOT_SET)
            ||(rule->actionset->version != NOT_SET_P)
            ||(rule->actionset->accuracy != NOT_SET)
            ||(rule->actionset->maturity != NOT_SET)
            ||(rule->actionset->logdata != NOT_SET_P))
        {
            return apr_psprintf(cmd->pool, "ModSecurity: Metadata actions (id, rev, msg, tag, severity, ver, accuracy, maturity, logdata) "
                " can only be specified by chain starter rules.");
        }

        /* Must NOT use skip. */
        if (rule->actionset->skip_count != NOT_SET) {
            return apr_psprintf(cmd->pool, "ModSecurity: The skip action can only be used "
                " by chain starter rules. ");
        }
    }

    /* Merge actions with the parent.
     *
     * ENH Probably do not want this done fully for chained rules.
     */
    rule->actionset = msre_actionset_merge(modsecurity->msre, dcfg->tmp_default_actionset,
        rule->actionset, 1);

    /* Keep track of the parent action for "block" */
    rule->actionset->parent_intercept_action_rec = dcfg->tmp_default_actionset->intercept_action_rec;
    rule->actionset->parent_intercept_action = dcfg->tmp_default_actionset->intercept_action;

    /* Must NOT specify a disruptive action in logging phase. */
    if ((rule->actionset != NULL)
        && (rule->actionset->phase == PHASE_LOGGING)
        && (rule->actionset->intercept_action != ACTION_ALLOW)
        && (rule->actionset->intercept_action != ACTION_ALLOW_REQUEST)
        && (rule->actionset->intercept_action != ACTION_NONE)
    ) {
        return apr_psprintf(cmd->pool, "ModSecurity: Disruptive actions "
            "cannot be specified in the logging phase.");
    }

    if (dcfg->tmp_chain_starter != NULL) {
        rule->chain_starter = dcfg->tmp_chain_starter;
        rule->actionset->phase = rule->chain_starter->actionset->phase;
    }

    if (rule->actionset->is_chained != 1) {
        /* If this rule is part of the chain but does
         * not want more rules to follow in the chain
         * then cut it (the chain).
         */
        dcfg->tmp_chain_starter = NULL;
    } else {
        /* On the other hand, if this rule wants other
         * rules to follow it, then start a new chain
         * if there isn't one already.
         */
        if (dcfg->tmp_chain_starter == NULL) {
            dcfg->tmp_chain_starter = rule;
        }
    }

    /* Optimisation */
    if ((rule->op_name != NULL)&&(strcasecmp(rule->op_name, "inspectFile") == 0)) {
        dcfg->upload_validates_files = 1;
    }

    /* Create skip table if one does not already exist. */
    if (dcfg->tmp_rule_placeholders == NULL) {
        dcfg->tmp_rule_placeholders = apr_table_make(cmd->pool, 10);
        if (dcfg->tmp_rule_placeholders == NULL) return FATAL_ERROR;
    }

    /* Keep track of any rule IDs we need to skip after */
    if (rule->actionset->skip_after != NOT_SET_P) {
        char *tmp_id = apr_pstrdup(cmd->pool, rule->actionset->skip_after);
        apr_table_setn(dcfg->tmp_rule_placeholders, tmp_id, tmp_id);

        #ifdef DEBUG_CONF
        ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, cmd->pool,
            "Watching for skipafter target rule id=\"%s\".", tmp_id);
        #endif

    }

    #ifdef DEBUG_CONF
    ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, cmd->pool,
        "Adding rule %pp phase=%d id=\"%s\".", rule, rule->actionset->phase, (rule->actionset->id == NOT_SET_P
        ? "(none)" : rule->actionset->id));
    #endif

    /* Add rule to the recipe. */
    if (msre_ruleset_rule_add(dcfg->ruleset, rule, rule->actionset->phase) < 0) {
        return "Internal Error: Failed to add rule to the ruleset.";
    }

    /* Add an additional placeholder if this rule ID is on the list */
    if ((rule->actionset->id != NULL) && apr_table_get(dcfg->tmp_rule_placeholders, rule->actionset->id)) {
        msre_rule *phrule = apr_palloc(rule->ruleset->mp, sizeof(msre_rule));
        if (phrule == NULL) {
            return FATAL_ERROR;
        }

        #ifdef DEBUG_CONF
        ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, cmd->pool,
            "Adding placeholder %pp for rule %pp id=\"%s\".", phrule, rule, rule->actionset->id);
        #endif

        /* shallow copy of original rule with placeholder marked as target */
        memcpy(phrule, rule, sizeof(msre_rule));
        phrule->placeholder = RULE_PH_SKIPAFTER;

        /* Add placeholder. */
        if (msre_ruleset_rule_add(dcfg->ruleset, phrule, phrule->actionset->phase) < 0) {
            return "Internal Error: Failed to add placeholder to the ruleset.";
        }

        /* No longer need to search for the ID */
        apr_table_unset(dcfg->tmp_rule_placeholders, rule->actionset->id);
    }

    /* Update the unparsed rule */
    rule->unparsed = msre_rule_generate_unparsed(dcfg->ruleset->mp, rule, NULL, NULL, NULL);

    return NULL;
}

/**
 *
 */
static const char *add_marker(cmd_parms *cmd, directory_config *dcfg,
                              const char *p1, const char *p2, const char *p3)
{
    char *my_error_msg = NULL;
    msre_rule *rule = NULL;
    extern msc_engine *modsecurity;
    int p;

    #ifdef DEBUG_CONF
    ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, cmd->pool,
        "Rule: type=%d p1='%s' p2='%s' p3='%s'", RULE_TYPE_MARKER, p1, p2, p3);
    #endif

    /* Create a ruleset if one does not exist. */
    if ((dcfg->ruleset == NULL)||(dcfg->ruleset == NOT_SET_P)) {
        dcfg->ruleset = msre_ruleset_create(modsecurity->msre, cmd->pool);
        if (dcfg->ruleset == NULL) return FATAL_ERROR;
    }

    /* Create the rule now. */
    rule = msre_rule_create(dcfg->ruleset, RULE_TYPE_MARKER, cmd->directive->filename, cmd->directive->line_num, p1, p2, p3, &my_error_msg);
    if (rule == NULL) {
        return my_error_msg;
    }

    /* This is a marker */
    rule->placeholder = RULE_PH_MARKER;

    /* Add placeholder to each phase */
    for (p = PHASE_FIRST; p <= PHASE_LAST; p++) {
        #ifdef DEBUG_CONF
        ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, cmd->pool,
            "Adding marker %pp phase=%d id=\"%s\".", rule, p, (rule->actionset->id == NOT_SET_P
            ? "(none)" : rule->actionset->id));
        #endif

        if (msre_ruleset_rule_add(dcfg->ruleset, rule, p) < 0) {
            return "Internal Error: Failed to add marker to the ruleset.";
        }
    }

    /* No longer need to search for the ID */
    if (dcfg->tmp_rule_placeholders != NULL) {
        apr_table_unset(dcfg->tmp_rule_placeholders, rule->actionset->id);
    }

    return NULL;
}

/**
 *
 */
static const char *update_rule_action(cmd_parms *cmd, directory_config *dcfg,
                                      const char *p1, const char *p2, int offset)
{
    char *my_error_msg = NULL;
    msre_rule *rule = NULL;
    msre_actionset *new_actionset = NULL;
    msre_ruleset *ruleset = dcfg->ruleset;
    extern msc_engine *modsecurity;

    /* Get the ruleset if one exists */
    if ((ruleset == NULL)||(ruleset == NOT_SET_P)) {
        return NULL;
    }

    #ifdef DEBUG_CONF
    ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, cmd->pool,
        "Update rule id=\"%s\" with action \"%s\".", p1, p2);
    #endif

    /* Fetch the rule */
    rule = msre_ruleset_fetch_rule(ruleset, p1, offset);
    if (rule == NULL) {
        #ifdef DEBUG_CONF
        ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, cmd->pool,
            "Update rule id=\"%s\" with action \"%s\" failed: Rule not found.", p1, p2);
        #endif
        return NULL;
    }

    /* Check the rule actionset */
    /* ENH: Can this happen? */
    if (rule->actionset == NULL) {
        return apr_psprintf(cmd->pool, "ModSecurity: Attempt to update action for rule \"%s\" failed: Rule does not have an actionset.", p1);
    }

    /* Create a new actionset */
    new_actionset = msre_actionset_create(modsecurity->msre, p2, &my_error_msg);
    if (new_actionset == NULL) return FATAL_ERROR;
    if (my_error_msg != NULL) return my_error_msg;

    /* Must NOT change an id */
    if ((new_actionset->id != NOT_SET_P) && (rule->actionset->id != NULL) && (strcmp(rule->actionset->id, new_actionset->id) != 0)) {
        return apr_psprintf(cmd->pool, "ModSecurity: Rule IDs cannot be updated via SecRuleUpdateActionById.");
    }

    /* Must NOT alter the phase */
    if ((new_actionset->phase != NOT_SET) && (rule->actionset->phase != new_actionset->phase)) {
        return apr_psprintf(cmd->pool, "ModSecurity: Rule phases cannot be updated via SecRuleUpdateActionById.");
    }

    #ifdef DEBUG_CONF
    {
        char *actions = msre_actionset_generate_action_string(ruleset->mp, rule->actionset);
        ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, cmd->pool,
            "Update rule %pp id=\"%s\" old action: \"%s\"",
            rule,
            (rule->actionset->id == NOT_SET_P ? "(none)" : rule->actionset->id),
            actions);
    }
    #endif

    /* Merge new actions with the rule */
    /* ENH: Will this leak the old actionset? */
    rule->actionset = msre_actionset_merge(modsecurity->msre, rule->actionset,
        new_actionset, 1);
    msre_actionset_set_defaults(rule->actionset);

    /* Update the unparsed rule */
    rule->unparsed = msre_rule_generate_unparsed(ruleset->mp, rule, NULL, NULL, NULL);

    #ifdef DEBUG_CONF
    {
        char *actions = msre_actionset_generate_action_string(ruleset->mp, rule->actionset);
        ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, cmd->pool,
            "Update rule %pp id=\"%s\" new action: \"%s\"",
            rule,
            (rule->actionset->id == NOT_SET_P ? "(none)" : rule->actionset->id),
            actions);
    }
    #endif

    return NULL;
}

/* -- Configuration directives -- */

static const char *cmd_action(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    return add_rule(cmd, (directory_config *)_dcfg, RULE_TYPE_ACTION, SECACTION_TARGETS, SECACTION_ARGS, p1);
}

static const char *cmd_marker(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    const char *action = apr_pstrcat(dcfg->mp, SECMARKER_BASE_ACTIONS, p1, NULL);
    return add_marker(cmd, (directory_config *)_dcfg, SECMARKER_TARGETS, SECMARKER_ARGS, action);
}

static const char *cmd_cookiev0_separator(cmd_parms *cmd, void *_dcfg,
        const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    if (strlen(p1) != 1) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid cookie v0 separator: %s", p1);
    }

    dcfg->cookiev0_separator = p1;

    return NULL;
}

static const char *cmd_argument_separator(cmd_parms *cmd, void *_dcfg,
        const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    if (strlen(p1) != 1) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid argument separator: %s", p1);
    }

    dcfg->argument_separator = p1[0];

    return NULL;
}

static const char *cmd_audit_engine(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = _dcfg;

    if (strcasecmp(p1, "On") == 0) dcfg->auditlog_flag = AUDITLOG_ON;
    else
        if (strcasecmp(p1, "Off") == 0) dcfg->auditlog_flag = AUDITLOG_OFF;
        else
            if (strcasecmp(p1, "RelevantOnly") == 0) dcfg->auditlog_flag = AUDITLOG_RELEVANT;
            else
                return (const char *)apr_psprintf(cmd->pool,
                        "ModSecurity: Unrecognised parameter value for SecAuditEngine: %s", p1);

    return NULL;
}

static const char *cmd_audit_log(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = _dcfg;

    dcfg->auditlog_name = (char *)p1;

    if (dcfg->auditlog_name[0] == '|') {
        const char *pipe_name = dcfg->auditlog_name + 1;
        piped_log *pipe_log;

        pipe_log = ap_open_piped_log(cmd->pool, pipe_name);
        if (pipe_log == NULL) {
            return apr_psprintf(cmd->pool, "ModSecurity: Failed to open the audit log pipe: %s",
                    pipe_name);
        }
        dcfg->auditlog_fd = ap_piped_log_write_fd(pipe_log);
    }
    else {
        const char *file_name = ap_server_root_relative(cmd->pool, dcfg->auditlog_name);
        apr_status_t rc;

        rc = apr_file_open(&dcfg->auditlog_fd, file_name,
                APR_WRITE | APR_APPEND | APR_CREATE | APR_BINARY,
                CREATEMODE, cmd->pool);

        if (rc != APR_SUCCESS) {
            return apr_psprintf(cmd->pool, "ModSecurity: Failed to open the audit log file: %s",
                    file_name);
        }
    }

    return NULL;
}

static const char *cmd_audit_log2(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = _dcfg;

    if (dcfg->auditlog_name == NOT_SET_P) {
        return apr_psprintf(cmd->pool, "ModSecurity: Cannot configure a secondary audit log without a primary defined: %s", p1);
    }

    dcfg->auditlog2_name = (char *)p1;

    if (dcfg->auditlog2_name[0] == '|') {
        const char *pipe_name = ap_server_root_relative(cmd->pool, dcfg->auditlog2_name + 1);
        piped_log *pipe_log;

        pipe_log = ap_open_piped_log(cmd->pool, pipe_name);
        if (pipe_log == NULL) {
            return apr_psprintf(cmd->pool, "ModSecurity: Failed to open the secondary audit log pipe: %s",
                    pipe_name);
        }
        dcfg->auditlog2_fd = ap_piped_log_write_fd(pipe_log);
    }
    else {
        const char *file_name = ap_server_root_relative(cmd->pool, dcfg->auditlog2_name);
        apr_status_t rc;

        rc = apr_file_open(&dcfg->auditlog2_fd, file_name,
                APR_WRITE | APR_APPEND | APR_CREATE | APR_BINARY,
                CREATEMODE, cmd->pool);

        if (rc != APR_SUCCESS) {
            return apr_psprintf(cmd->pool, "ModSecurity: Failed to open the secondary audit log file: %s",
                    file_name);
        }
    }

    return NULL;
}

static const char *cmd_audit_log_parts(cmd_parms *cmd, void *_dcfg,
        const char *p1)
{
    directory_config *dcfg = _dcfg;

    if (is_valid_parts_specification((char *)p1) != 1) {
        return apr_psprintf(cmd->pool, "Invalid parts specification for SecAuditLogParts: %s", p1);
    }

    dcfg->auditlog_parts = (char *)p1;
    return NULL;
}

static const char *cmd_audit_log_relevant_status(cmd_parms *cmd, void *_dcfg,
        const char *p1)
{
    directory_config *dcfg = _dcfg;

    dcfg->auditlog_relevant_regex = msc_pregcomp(cmd->pool, p1, PCRE_DOTALL, NULL, NULL);
    if (dcfg->auditlog_relevant_regex == NULL) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid regular expression: %s", p1);
    }

    return NULL;
}

static const char *cmd_audit_log_type(cmd_parms *cmd, void *_dcfg,
        const char *p1)
{
    directory_config *dcfg = _dcfg;

    if (strcasecmp(p1, "Serial") == 0) dcfg->auditlog_type = AUDITLOG_SERIAL;
    else
        if (strcasecmp(p1, "Concurrent") == 0) dcfg->auditlog_type = AUDITLOG_CONCURRENT;
        else
            return (const char *)apr_psprintf(cmd->pool,
                    "ModSecurity: Unrecognised parameter value for SecAuditLogType: %s", p1);

    return NULL;
}

static const char *cmd_audit_log_dirmode(cmd_parms *cmd, void *_dcfg,
        const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "default") == 0) {
        dcfg->auditlog_dirperms = NOT_SET;
    }
    else {
        long int mode = strtol(p1, NULL, 8); /* expects octal mode */
        if ((mode == LONG_MAX)||(mode == LONG_MIN)||(mode <= 0)||(mode > 07777)) {
            return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecAuditLogDirMode: %s", p1);
        }

        dcfg->auditlog_dirperms = mode2fileperms(mode);
    }

    return NULL;
}

static const char *cmd_audit_log_filemode(cmd_parms *cmd, void *_dcfg,
        const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "default") == 0) {
        dcfg->auditlog_fileperms = NOT_SET;
    }
    else {
        long int mode = strtol(p1, NULL, 8); /* expects octal mode */
        if ((mode == LONG_MAX)||(mode == LONG_MIN)||(mode <= 0)||(mode > 07777)) {
            return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecAuditLogFileMode: %s", p1);
        }

        dcfg->auditlog_fileperms = mode2fileperms(mode);
    }

    return NULL;
}

static const char *cmd_audit_log_storage_dir(cmd_parms *cmd, void *_dcfg,
                                             const char *p1)
{
    directory_config *dcfg = _dcfg;

    dcfg->auditlog_storage_dir = ap_server_root_relative(cmd->pool, p1);

    return NULL;
}

static const char *cmd_cookie_format(cmd_parms *cmd, void *_dcfg,
                                     const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    if (strcmp(p1, "0") == 0) dcfg->cookie_format = COOKIES_V0;
    else
    if (strcmp(p1, "1") == 0) dcfg->cookie_format = COOKIES_V1;
    else {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid cookie format: %s", p1);
    }

    return NULL;
}

static const char *cmd_chroot_dir(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    char cwd[1025] = "";

    if (cmd->server->is_virtual) {
        return "ModSecurity: SecChrootDir not allowed in VirtualHost";
    }

    chroot_dir = (char *)p1;

    if (getcwd(cwd, 1024) == NULL) {
        return "ModSecurity: Failed to get the current working directory";
    }

    if (chdir(chroot_dir) < 0) {
        return apr_psprintf(cmd->pool, "ModSecurity: Failed to chdir to %s, errno=%d (%s)",
            chroot_dir, errno, strerror(errno));
    }

    if (chdir(cwd) < 0) {
        return apr_psprintf(cmd->pool, "ModSecurity: Failed to chdir to %s, errno=%d (%s)",
            cwd, errno, strerror(errno));
    }

    return NULL;
}

/**
 * Adds component signature to the list of signatures kept in configuration.
 */
static const char *cmd_component_signature(cmd_parms *cmd, void *_dcfg,
                                           const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    /* ENH Enforce "Name/VersionX.Y.Z (comment)" format. */
    *(char **)apr_array_push(dcfg->component_signatures) = (char *)p1;

    return NULL;
}

static const char *cmd_content_injection(cmd_parms *cmd, void *_dcfg, int flag)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;
    dcfg->content_injection_enabled = flag;
    return NULL;
}

static const char *cmd_data_dir(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    if (cmd->server->is_virtual) {
        return "ModSecurity: SecDataDir not allowed in VirtualHost.";
    }

    dcfg->data_dir = ap_server_root_relative(cmd->pool, p1);

    return NULL;
}

static const char *cmd_debug_log(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    apr_status_t rc;

    dcfg->debuglog_name = ap_server_root_relative(cmd->pool, p1);

    rc = apr_file_open(&dcfg->debuglog_fd, dcfg->debuglog_name,
                   APR_WRITE | APR_APPEND | APR_CREATE | APR_BINARY,
                   CREATEMODE, cmd->pool);

    if (rc != APR_SUCCESS) {
        return apr_psprintf(cmd->pool, "ModSecurity: Failed to open debug log file: %s",
            dcfg->debuglog_name);
    }

    return NULL;
}

/**
* \brief Add SecCollectionTimeout configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On failure
* \retval apr_psprintf On Success
*/
static const char *cmd_collection_timeout(cmd_parms *cmd, void *_dcfg,
                                       const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    dcfg->col_timeout = atoi(p1);
    /* max 30 days */
    if ((dcfg->col_timeout >= 0)&&(dcfg->col_timeout <= 2592000)) return NULL;

    return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecCollectionTimeout: %s", p1);
}

static const char *cmd_debug_log_level(cmd_parms *cmd, void *_dcfg,
                                       const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    dcfg->debuglog_level = atoi(p1);
    if ((dcfg->debuglog_level >= 0)&&(dcfg->debuglog_level <= 9)) return NULL;

    return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecDebugLogLevel: %s", p1);
}

static const char *cmd_default_action(cmd_parms *cmd, void *_dcfg,
                                      const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    extern msc_engine *modsecurity;
    char *my_error_msg = NULL;

    dcfg->tmp_default_actionset = msre_actionset_create(modsecurity->msre, p1, &my_error_msg);
    if (dcfg->tmp_default_actionset == NULL) {
        if (my_error_msg != NULL) return my_error_msg;
        else return FATAL_ERROR;
    }

    /* Must specify a disruptive action. */
    /* ENH: Remove this requirement? */
    if (dcfg->tmp_default_actionset->intercept_action == NOT_SET) {
        return apr_psprintf(cmd->pool, "ModSecurity: SecDefaultAction must specify a disruptive action.");
    }

    /* Must specify a phase. */
    /* ENH: Remove this requirement? */
    if (dcfg->tmp_default_actionset->phase == NOT_SET) {
        return apr_psprintf(cmd->pool, "ModSecurity: SecDefaultAction must specify a phase.");
    }

    /* Must not use metadata actions. */
    /* ENH: loop through to check for tags */
    if ((dcfg->tmp_default_actionset->id != NOT_SET_P)
        ||(dcfg->tmp_default_actionset->rev != NOT_SET_P)
        ||(dcfg->tmp_default_actionset->version != NOT_SET_P)
        ||(dcfg->tmp_default_actionset->maturity != NOT_SET)
        ||(dcfg->tmp_default_actionset->accuracy != NOT_SET)
        ||(dcfg->tmp_default_actionset->msg != NOT_SET_P))
    {
        return apr_psprintf(cmd->pool, "ModSecurity: SecDefaultAction must not "
            "contain any metadata actions (id, rev, msg, tag, severity, ver, accuracy, maturity, logdata).");
    }
    /* These are just a warning for now. */
    if ((dcfg->tmp_default_actionset->severity != NOT_SET)
        ||(dcfg->tmp_default_actionset->logdata != NOT_SET_P))
    {
        ap_log_perror(APLOG_MARK,
            APLOG_STARTUP|APLOG_WARNING|APLOG_NOERRNO, 0, cmd->pool,
            "ModSecurity: WARNING Using \"severity\" or \"logdata\" in "
            "SecDefaultAction is deprecated (%s:%d).",
            cmd->directive->filename, cmd->directive->line_num);
    }

    if (apr_table_get(dcfg->tmp_default_actionset->actions, "t")) {
        ap_log_perror(APLOG_MARK,
            APLOG_STARTUP|APLOG_WARNING|APLOG_NOERRNO, 0, cmd->pool,
            "ModSecurity: WARNING Using transformations in "
            "SecDefaultAction is deprecated (%s:%d).",
            cmd->directive->filename, cmd->directive->line_num);
    }

    /* Must not use chain. */
    if (dcfg->tmp_default_actionset->is_chained != NOT_SET) {
        return apr_psprintf(cmd->pool, "ModSecurity: SecDefaultAction must not "
            "contain a chain action.");
    }

    /* Must not use skip. */
    if (dcfg->tmp_default_actionset->skip_count != NOT_SET) {
        return apr_psprintf(cmd->pool, "ModSecurity: SecDefaultAction must not "
            "contain a skip action.");
    }

    /* Must not use skipAfter. */
    if (dcfg->tmp_default_actionset->skip_after != NOT_SET_P) {
        return apr_psprintf(cmd->pool, "ModSecurity: SecDefaultAction must not "
            "contain a skipAfter action.");
    }

    return NULL;
}

static const char *cmd_disable_backend_compression(cmd_parms *cmd, void *_dcfg, int flag)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;
    dcfg->disable_backend_compression = flag;
    return NULL;
}

static const char *cmd_guardian_log(cmd_parms *cmd, void *_dcfg,
                                    const char *p1, const char *p2)
{
    extern char *guardianlog_name;
    extern apr_file_t *guardianlog_fd;
    extern char *guardianlog_condition;

    if (cmd->server->is_virtual) {
        return "ModSecurity: SecGuardianLog not allowed in VirtualHost";
    }

    if (p2 != NULL) {
        if (strncmp(p2, "env=", 4) != 0) {
            return "ModSecurity: Error in condition clause";
        }
        if ( (p2[4] == '\0') || ((p2[4] == '!')&&(p2[5] == '\0')) ) {
            return "ModSecurity: Missing variable name";
        }
        guardianlog_condition = apr_pstrdup(cmd->pool, p2 + 4);
    }

    guardianlog_name = (char *)p1;

    if (guardianlog_name[0] == '|') {
        const char *pipe_name = ap_server_root_relative(cmd->pool, guardianlog_name + 1);
        piped_log *pipe_log;

        pipe_log = ap_open_piped_log(cmd->pool, pipe_name);
        if (pipe_log == NULL) {
            return apr_psprintf(cmd->pool, "ModSecurity: Failed to open the guardian log pipe: %s",
                pipe_name);
        }
        guardianlog_fd = ap_piped_log_write_fd(pipe_log);
    }
    else {
        const char *file_name = ap_server_root_relative(cmd->pool, guardianlog_name);
        apr_status_t rc;

        rc = apr_file_open(&guardianlog_fd, file_name,
            APR_WRITE | APR_APPEND | APR_CREATE | APR_BINARY,
            CREATEMODE, cmd->pool);

        if (rc != APR_SUCCESS) {
            return apr_psprintf(cmd->pool, "ModSecurity: Failed to open the guardian log file: %s",
                file_name);
        }
    }

    return NULL;
}

/**
* \brief Add SecStreamInBodyInspection configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On failure
* \retval apr_psprintf On Success
*/
static const char *cmd_stream_inbody_inspection(cmd_parms *cmd, void *_dcfg, int flag)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;
    dcfg->stream_inbody_inspection = flag;
    return NULL;
}


/**
* \brief Add SecStreamOutBodyInspection configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On failure
* \retval apr_psprintf On Success
*/
static const char *cmd_stream_outbody_inspection(cmd_parms *cmd, void *_dcfg, int flag)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;
    dcfg->stream_outbody_inspection = flag;
    return NULL;
}
/**
* \brief Add SecRulePerfTime configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On failure
* \retval apr_psprintf On Success
*/
static const char *cmd_rule_perf_time(cmd_parms *cmd, void *_dcfg,
        const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    long int limit;

    if (dcfg == NULL) return NULL;

    limit = strtol(p1, NULL, 10);
    if ((limit == LONG_MAX)||(limit == LONG_MIN)||(limit <= 0)) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecRulePerfTime: %s", p1);
    }

    dcfg->max_rule_time = limit;

    return NULL;
}

/**
* \brief Add SecReadStateLimit configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On failure
* \retval apr_psprintf On Success
*/
static const char *cmd_conn_read_state_limit(cmd_parms *cmd, void *_dcfg,
        const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    long int limit;

    if (dcfg == NULL) return NULL;

    limit = strtol(p1, NULL, 10);
    if ((limit == LONG_MAX)||(limit == LONG_MIN)||(limit <= 0)) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecReadStateLimit: %s", p1);
    }

    conn_read_state_limit = limit;

    return NULL;
}

/**
* \brief Add SecWriteStateLimit configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On failure
* \retval apr_psprintf On Success
*/
static const char *cmd_conn_write_state_limit(cmd_parms *cmd, void *_dcfg,
        const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    long int limit;

    if (dcfg == NULL) return NULL;

    limit = strtol(p1, NULL, 10);
    if ((limit == LONG_MAX)||(limit == LONG_MIN)||(limit <= 0)) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecWriteStateLimit: %s", p1);
    }

    conn_write_state_limit = limit;

    return NULL;
}


static const char *cmd_request_body_inmemory_limit(cmd_parms *cmd, void *_dcfg,
                                                   const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    long int limit;

    if (dcfg == NULL) return NULL;

    limit = strtol(p1, NULL, 10);
    if ((limit == LONG_MAX)||(limit == LONG_MIN)||(limit <= 0)) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecRequestBodyInMemoryLimit: %s", p1);
    }

    dcfg->reqbody_inmemory_limit = limit;

    return NULL;
}

static const char *cmd_request_body_limit(cmd_parms *cmd, void *_dcfg,
                                          const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    long int limit;

    if (dcfg == NULL) return NULL;

    limit = strtol(p1, NULL, 10);
    if ((limit == LONG_MAX)||(limit == LONG_MIN)||(limit <= 0)) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecRequestBodyLimit: %s", p1);
    }

    dcfg->reqbody_limit = limit;

    return NULL;
}

static const char *cmd_request_body_no_files_limit(cmd_parms *cmd, void *_dcfg,
                                                   const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    long int limit;

    if (dcfg == NULL) return NULL;

    limit = strtol(p1, NULL, 10);
    if ((limit == LONG_MAX)||(limit == LONG_MIN)||(limit <= 0)) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecRequestBodyNoFilesLimit: %s", p1);
    }

    dcfg->reqbody_no_files_limit = limit;

    return NULL;
}

static const char *cmd_request_body_access(cmd_parms *cmd, void *_dcfg,
                                           const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "on") == 0) dcfg->reqbody_access = 1;
    else
    if (strcasecmp(p1, "off") == 0) dcfg->reqbody_access = 0;
    else
    return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecRequestBodyAccess: %s", p1);

    return NULL;
}

/**
* \brief Add SecInterceptOnError configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On failure
* \retval apr_psprintf On success
*/
static const char *cmd_request_intercept_on_error(cmd_parms *cmd, void *_dcfg,
                                           const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "on") == 0) dcfg->reqintercept_oe = 1;
    else
    if (strcasecmp(p1, "off") == 0) dcfg->reqintercept_oe = 0;
    else
    return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecInterceptOnError: %s", p1);

    return NULL;
}


static const char *cmd_request_encoding(cmd_parms *cmd, void *_dcfg,
                                        const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    /* ENH Validate encoding */

    dcfg->request_encoding = p1;

    return NULL;
}

static const char *cmd_response_body_access(cmd_parms *cmd, void *_dcfg,
                                            const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "on") == 0) dcfg->resbody_access = 1;
    else
    if (strcasecmp(p1, "off") == 0) dcfg->resbody_access = 0;
    else
    return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecResponseBodyAccess: %s", p1);

    return NULL;
}

static const char *cmd_response_body_limit(cmd_parms *cmd, void *_dcfg,
                                           const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    long int limit;

    limit = strtol(p1, NULL, 10);
    if ((limit == LONG_MAX)||(limit == LONG_MIN)||(limit <= 0)) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecResponseBodyLimit: %s", p1);
    }

    if (limit > RESPONSE_BODY_HARD_LIMIT) {
        return apr_psprintf(cmd->pool, "ModSecurity: Response size limit can not exceed the hard limit: %li", RESPONSE_BODY_HARD_LIMIT);
    }

    dcfg->of_limit = limit;

    return NULL;
}

static const char *cmd_response_body_limit_action(cmd_parms *cmd, void *_dcfg,
                                                  const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    if (dcfg->is_enabled == MODSEC_DETECTION_ONLY)  {
        dcfg->of_limit_action = RESPONSE_BODY_LIMIT_ACTION_PARTIAL;
        return NULL;
    }

    if (strcasecmp(p1, "ProcessPartial") == 0) dcfg->of_limit_action = RESPONSE_BODY_LIMIT_ACTION_PARTIAL;
    else
    if (strcasecmp(p1, "Reject") == 0) dcfg->of_limit_action = RESPONSE_BODY_LIMIT_ACTION_REJECT;
    else
    return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecResponseBodyLimitAction: %s", p1);

    return NULL;
}

/**
* \brief Add SecRequestBodyLimitAction configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On failure
* \retval apr_psprintf On success
*/
static const char *cmd_resquest_body_limit_action(cmd_parms *cmd, void *_dcfg,
                                                  const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    if (dcfg->is_enabled == MODSEC_DETECTION_ONLY)  {
        dcfg->if_limit_action = REQUEST_BODY_LIMIT_ACTION_PARTIAL;
        return NULL;
    }

    if (strcasecmp(p1, "ProcessPartial") == 0) dcfg->if_limit_action = REQUEST_BODY_LIMIT_ACTION_PARTIAL;
    else
    if (strcasecmp(p1, "Reject") == 0) dcfg->if_limit_action = REQUEST_BODY_LIMIT_ACTION_REJECT;
    else
    return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecRequestBodyLimitAction: %s", p1);

    return NULL;
}

static const char *cmd_response_body_mime_type(cmd_parms *cmd, void *_dcfg,
                                               const char *_p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    char *p1 = apr_pstrdup(cmd->pool, _p1);

    /* TODO check whether the parameter is a valid MIME type of "???" */

    if ((dcfg->of_mime_types == NULL)||(dcfg->of_mime_types == NOT_SET_P)) {
        dcfg->of_mime_types = apr_table_make(cmd->pool, 10);
    }

    strtolower_inplace((unsigned char *)p1);
    apr_table_setn(dcfg->of_mime_types, p1, "1");

    return NULL;
}

static const char *cmd_response_body_mime_types_clear(cmd_parms *cmd,
                                                      void *_dcfg)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    dcfg->of_mime_types_cleared = 1;

    if ((dcfg->of_mime_types != NULL)&&(dcfg->of_mime_types != NOT_SET_P)) {
        apr_table_clear(dcfg->of_mime_types);
    }

    return NULL;
}

/**
 * \brief Add SecRuleUpdateTargetById
 *
 * \param cmd Pointer to configuration data
 * \param _dcfg Pointer to directory configuration
 * \param p1 Pointer to configuration option
 * \param p2 Pointer to configuration option
 * \param p3 Pointer to configuration option
 *
 * \retval NULL On failure|Success
 */
static const char *cmd_rule_update_target_by_id(cmd_parms *cmd, void *_dcfg,
        const char *p1, const char *p2, const char *p3)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    rule_exception *re = apr_pcalloc(cmd->pool, sizeof(rule_exception));
    if (dcfg == NULL) return NULL;

    if(p1 == NULL)  {
        return apr_psprintf(cmd->pool, "Updating target by ID with no ID");
    }

    re->type = RULE_EXCEPTION_REMOVE_ID;
    /* TODO: Validate the range here, while we can still tell the user if it's invalid */
    re->param = p1;

    if(dcfg->ruleset == NULL) {
        return apr_psprintf(cmd->pool, "Updating target by ID with no ruleset in this context");
    }

    return msre_ruleset_rule_update_target_matching_exception(NULL, dcfg->ruleset, re, p2, p3);
}

/**
 * \brief Add SecRuleUpdateTargetByTag  configuration option
 *
 * \param cmd Pointer to configuration data
 * \param _dcfg Pointer to directory configuration
 * \param p1 Pointer to configuration option RULETAG
 * \param p2 Pointer to configuration option TARGET
 * \param p3 Pointer to configuration option REPLACED_TARGET
 * \todo Finish documenting
 *
 * \retval NULL On success
 * \retval apr_psprintf On failure
 *
 * \todo Figure out error checking
 */
static const char *cmd_rule_update_target_by_tag(cmd_parms *cmd, void *_dcfg,
        const char *p1, const char *p2, const char *p3)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    rule_exception *re = apr_pcalloc(cmd->pool, sizeof(rule_exception));
    if (dcfg == NULL) return NULL;

    if(p1 == NULL)  {
        return apr_psprintf(cmd->pool, "Updating target by tag with no tag");
    }

    re->type = RULE_EXCEPTION_REMOVE_TAG;
    re->param = p1;
    re->param_data = msc_pregcomp(cmd->pool, p1, 0, NULL, NULL);
    if (re->param_data == NULL) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid regular expression: %s", p1);
    }

    return msre_ruleset_rule_update_target_matching_exception(NULL, dcfg->ruleset, re, p2, p3);
}
/**
 * \brief Add SecRuleUpdateTargetByMsg configuration option
 *
 * \param cmd Pointer to configuration data
 * \param _dcfg Pointer to directory configuration
 * \param p1 Pointer to configuration option RULEMSG
 * \param p2 Pointer to configuration option TARGET
 * \param p3 Pointer to configuration option REPLACED_TARGET
 * \todo Finish documenting
 *
 * \retval NULL On success
 * \retval apr_psprintf On failure
 *
 * \todo Figure out error checking
 */
static const char *cmd_rule_update_target_by_msg(cmd_parms *cmd, void *_dcfg,
        const char *p1, const char *p2, const char *p3)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    rule_exception *re = apr_pcalloc(cmd->pool, sizeof(rule_exception));
    if (dcfg == NULL) return NULL;

    if(p1 == NULL)  {
        return apr_psprintf(cmd->pool, "Updating target by message with no message");
    }

    re->type = RULE_EXCEPTION_REMOVE_MSG;
    re->param = p1;
    re->param_data = msc_pregcomp(cmd->pool, p1, 0, NULL, NULL);
    if (re->param_data == NULL) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid regular expression: %s", p1);
    }

    return msre_ruleset_rule_update_target_matching_exception(NULL, dcfg->ruleset, re, p2, p3);
}


static const char *cmd_rule(cmd_parms *cmd, void *_dcfg,
        const char *p1, const char *p2, const char *p3)
{
    return add_rule(cmd, (directory_config *)_dcfg, RULE_TYPE_NORMAL, p1, p2, p3);
}

static const char *cmd_rule_engine(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "on") == 0) dcfg->is_enabled = MODSEC_ENABLED;
    else
    if (strcasecmp(p1, "off") == 0) dcfg->is_enabled = MODSEC_DISABLED;
    else
    if (strcasecmp(p1, "detectiononly") == 0) {
        dcfg->is_enabled = MODSEC_DETECTION_ONLY;
        dcfg->of_limit_action = RESPONSE_BODY_LIMIT_ACTION_PARTIAL;
        dcfg->if_limit_action = REQUEST_BODY_LIMIT_ACTION_PARTIAL;
    } else
    return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecRuleEngine: %s", p1);

    return NULL;
}

static const char *cmd_rule_inheritance(cmd_parms *cmd, void *_dcfg, int flag)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;
    dcfg->rule_inheritance = flag;
    return NULL;
}

static const char *cmd_rule_script(cmd_parms *cmd, void *_dcfg,
                                   const char *p1, const char *p2)
{
    #if defined(WITH_LUA)
    const char *filename = resolve_relative_path(cmd->pool, cmd->directive->filename, p1);
    return add_rule(cmd, (directory_config *)_dcfg, RULE_TYPE_LUA, filename, p2, NULL);
    #else
    ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, cmd->pool, "Ignoring SecRuleScript \"%s\" directive (%s:%d): No Lua scripting support.", p1, cmd->directive->filename, cmd->directive->line_num);
    return NULL;
    #endif
}

static const char *cmd_rule_remove_by_id(cmd_parms *cmd, void *_dcfg,
                                         const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    rule_exception *re = apr_pcalloc(cmd->pool, sizeof(rule_exception));
    if (dcfg == NULL) return NULL;

    re->type = RULE_EXCEPTION_REMOVE_ID;
    re->param = p1;
    *(rule_exception **)apr_array_push(dcfg->rule_exceptions) = re;

    /* Remove the corresponding rules from the context straight away. */
    msre_ruleset_rule_remove_with_exception(dcfg->ruleset, re);

    return NULL;
}

/**
* \brief Add SecRuleRemoveByTag  configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On failure
* \retval apr_psprintf On success
*/
static const char *cmd_rule_remove_by_tag(cmd_parms *cmd, void *_dcfg,
                                          const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    rule_exception *re = apr_pcalloc(cmd->pool, sizeof(rule_exception));
    if (dcfg == NULL) return NULL;

    re->type = RULE_EXCEPTION_REMOVE_TAG;
    re->param = p1;
    re->param_data = msc_pregcomp(cmd->pool, p1, 0, NULL, NULL);
    if (re->param_data == NULL) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid regular expression: %s", p1);
    }
    *(rule_exception **)apr_array_push(dcfg->rule_exceptions) = re;

    /* Remove the corresponding rules from the context straight away. */
    msre_ruleset_rule_remove_with_exception(dcfg->ruleset, re);

    #ifdef DEBUG_CONF
    ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, cmd->pool, "Added exception %pp (%d %s) to dcfg %pp.", re, re->type, re->param, dcfg);
    #endif

    return NULL;
}

static const char *cmd_rule_remove_by_msg(cmd_parms *cmd, void *_dcfg,
                                          const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    rule_exception *re = apr_pcalloc(cmd->pool, sizeof(rule_exception));
    if (dcfg == NULL) return NULL;

    re->type = RULE_EXCEPTION_REMOVE_MSG;
    re->param = p1;
    re->param_data = msc_pregcomp(cmd->pool, p1, 0, NULL, NULL);
    if (re->param_data == NULL) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid regular expression: %s", p1);
    }
    *(rule_exception **)apr_array_push(dcfg->rule_exceptions) = re;

    /* Remove the corresponding rules from the context straight away. */
    msre_ruleset_rule_remove_with_exception(dcfg->ruleset, re);

    #ifdef DEBUG_CONF
    ap_log_perror(APLOG_MARK, APLOG_STARTUP|APLOG_NOERRNO, 0, cmd->pool, "Added exception %pp (%d %s) to dcfg %pp.", re, re->type, re->param, dcfg);
    #endif

    return NULL;
}

static const char *cmd_rule_update_action_by_id(cmd_parms *cmd, void *_dcfg,
        const char *p1, const char *p2)
{
    int offset = 0, rule_id = atoi(p1);
    char *opt = strchr(p1,':');
    char *savedptr = NULL;
    char *param = apr_pstrdup(cmd->pool, p1);

    if ((rule_id == LONG_MAX)||(rule_id == LONG_MIN)||(rule_id <= 0)) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for ID for update action: %s", p1);
    }

    if(opt != NULL) {
        opt++;
        offset = atoi(opt);
        opt = apr_strtok(param,":", &savedptr);
        return update_rule_action(cmd, (directory_config *)_dcfg, (const char *)opt, p2, offset);
    }

    return update_rule_action(cmd, (directory_config *)_dcfg, p1, p2, offset);
}

static const char *cmd_server_signature(cmd_parms *cmd, void *_dcfg,
                                        const char *p1)
{
    if (cmd->server->is_virtual) {
        return "ModSecurity: SecServerSignature not allowed in VirtualHost";
    }
    new_server_signature = (char *)p1;
    return NULL;
}

static const char *cmd_tmp_dir(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "none") == 0) dcfg->tmp_dir = NULL;
    else dcfg->tmp_dir = ap_server_root_relative(cmd->pool, p1);

    return NULL;
}

static const char *cmd_upload_dir(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "none") == 0) dcfg->upload_dir = NULL;
    else dcfg->upload_dir = ap_server_root_relative(cmd->pool, p1);

    return NULL;
}

static const char *cmd_upload_file_limit(cmd_parms *cmd, void *_dcfg,
                                         const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "default") == 0) {
        dcfg->upload_file_limit = NOT_SET;
    }
    else {
        dcfg->upload_file_limit = atoi(p1);
    }

    return NULL;
}

static const char *cmd_upload_filemode(cmd_parms *cmd, void *_dcfg,
                                       const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "default") == 0) {
        dcfg->upload_filemode = NOT_SET;
    }
    else {
        long int mode = strtol(p1, NULL, 8); /* expects octal mode */
        if ((mode == LONG_MAX)||(mode == LONG_MIN)||(mode <= 0)||(mode > 07777)) {
            return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecUploadFileMode: %s", p1);
        }

        dcfg->upload_filemode = (int)mode;
    }

    return NULL;
}

static const char *cmd_upload_keep_files(cmd_parms *cmd, void *_dcfg,
                                         const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "on") == 0) {
        dcfg->upload_keep_files = KEEP_FILES_ON;
    } else
    if (strcasecmp(p1, "off") == 0) {
        dcfg->upload_keep_files = KEEP_FILES_OFF;
    } else
    if (strcasecmp(p1, "relevantonly") == 0) {
        dcfg->upload_keep_files = KEEP_FILES_RELEVANT_ONLY;
    } else {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid setting for SecUploadKeepFiles: %s",
            p1);
    }
    return NULL;
}

static const char *cmd_web_app_id(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    /* ENH enforce format (letters, digits, ., _, -) */
    dcfg->webappid = p1;

    return NULL;
}

static const char *cmd_sensor_id(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    /* ENH enforce format (letters, digits, ., _, -) */
    dcfg->sensor_id = p1;

    return NULL;
}

/**
* \brief Add SecXmlExternalEntity configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On failure
* \retval apr_psprintf On Success
*/
static const char *cmd_xml_external_entity(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "on") == 0)  {
        dcfg->xml_external_entity = 1;
    }
    else if (strcasecmp(p1, "off") == 0)    {
        dcfg->xml_external_entity = 0;
    }
    else return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecXmlExternalEntity: %s", p1);

    return NULL;
}


/**
* \brief Add SecHashEngine configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On failure
* \retval apr_psprintf On Success
*/
static const char *cmd_hash_engine(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "on") == 0)  {
        dcfg->hash_is_enabled = HASH_ENABLED;
        dcfg->hash_enforcement = HASH_ENABLED;
    }
    else if (strcasecmp(p1, "off") == 0)    {
        dcfg->hash_is_enabled = HASH_DISABLED;
        dcfg->hash_enforcement = HASH_DISABLED;
    }
    else return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SexHashEngine: %s", p1);

    return NULL;
}

/**
* \brief Add SecHashPram configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On success
*/
static const char *cmd_hash_param(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    if (dcfg == NULL) return NULL;

    if (p1 == NULL) return NULL;
    dcfg->crypto_param_name = p1;

    return NULL;
}

/**
* \brief Add SecHashKey configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param _p1 Pointer to configuration option
* \param _p2 Pointer to configuration option
*
* \retval NULL On success
*/
static const char *cmd_hash_key(cmd_parms *cmd, void *_dcfg, const char *_p1, const char *_p2)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    char *p1 = NULL;

    if (dcfg == NULL) return NULL;
    if (_p1 == NULL) return NULL;

    if (strcasecmp(_p1, "Rand") == 0)    {
        p1 = apr_pstrdup(cmd->pool, getkey(cmd->pool));
        dcfg->crypto_key = p1;
        dcfg->crypto_key_len = strlen(dcfg->crypto_key);
    } else    {
        p1 = apr_pstrdup(cmd->pool, _p1);
        dcfg->crypto_key = p1;
        dcfg->crypto_key_len = strlen(p1);
    }

    if(_p2 == NULL)  {
        return NULL;
    } else    {
        if (strcasecmp(_p2, "KeyOnly") == 0)
            dcfg->crypto_key_add = HASH_KEYONLY;
        else if (strcasecmp(_p2, "SessionID") == 0)
            dcfg->crypto_key_add = HASH_SESSIONID;
        else if (strcasecmp(_p2, "RemoteIP") == 0)
            dcfg->crypto_key_add = HASH_REMOTEIP;
    }
    return NULL;
}

/**
* \brief Add SecHashMethodPm configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
* \param p2 Pointer to configuration option
*
* \retval NULL On failure
* \retval apr_psprintf On Success
*/
static const char *cmd_hash_method_pm(cmd_parms *cmd, void *_dcfg,
        const char *p1, const char *p2)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    rule_exception *re = apr_pcalloc(cmd->pool, sizeof(hash_method));
    const char *_p2 = apr_pstrdup(cmd->pool, p2);
    ACMP *p = NULL;
    const char *phrase = NULL;
    const char *next = NULL;

    if (dcfg == NULL) return NULL;

    p = acmp_create(0, cmd->pool);
    if (p == NULL) return NULL;

    if(phrase == NULL)
        phrase = apr_pstrdup(cmd->pool, _p2);

    for (;;) {
        while((apr_isspace(*phrase) != 0) && (*phrase != '\0')) phrase++;
        if (*phrase == '\0') break;
        next = phrase;
        while((apr_isspace(*next) == 0) && (*next != 0)) next++;
        acmp_add_pattern(p, phrase, NULL, NULL, next - phrase);
        phrase = next;
    }

    acmp_prepare(p);

    if (strcasecmp(p1, "HashHref") == 0) {
        re->type = HASH_URL_HREF_HASH_PM;
        re->param = _p2;
        re->param_data = (void *)p;
        if (re->param_data == NULL) {
            return apr_psprintf(cmd->pool, "ModSecurity: Invalid pattern: %s", p2);
        }
        dcfg->crypto_hash_href_pm = 1;
    }
    else if (strcasecmp(p1, "HashFormAction") == 0) {
        re->type = HASH_URL_FACTION_HASH_PM;
        re->param = _p2;
        re->param_data = (void *)p;
        if (re->param_data == NULL) {
            return apr_psprintf(cmd->pool, "ModSecurity: Invalid pattern: %s", p2);
        }
        dcfg->crypto_hash_faction_pm = 1;
    }
    else if (strcasecmp(p1, "HashLocation") == 0) {
        re->type = HASH_URL_LOCATION_HASH_PM;
        re->param = _p2;
        re->param_data = (void *)p;
        if (re->param_data == NULL) {
            return apr_psprintf(cmd->pool, "ModSecurity: Invalid pattern: %s", p2);
        }
        dcfg->crypto_hash_location_pm = 1;
    }
    else if (strcasecmp(p1, "HashIframeSrc") == 0) {
        re->type = HASH_URL_IFRAMESRC_HASH_PM;
        re->param = _p2;
        re->param_data = (void *)p;
        if (re->param_data == NULL) {
            return apr_psprintf(cmd->pool, "ModSecurity: Invalid pattern: %s", p2);
        }
        dcfg->crypto_hash_iframesrc_pm = 1;
    }
    else if (strcasecmp(p1, "HashFrameSrc") == 0) {
        re->type = HASH_URL_FRAMESRC_HASH_PM;
        re->param = _p2;
        re->param_data = (void *)p;
        if (re->param_data == NULL) {
            return apr_psprintf(cmd->pool, "ModSecurity: Invalid pattern: %s", p2);
        }
        dcfg->crypto_hash_framesrc_pm = 1;
    }

    *(hash_method **)apr_array_push(dcfg->hash_method) = re;

    return NULL;
}

/**
 * \brief Add SecHashMethodRx configuration option
 *
 * \param cmd Pointer to configuration data
 * \param _dcfg Pointer to directory configuration
 * \param p1 Pointer to configuration option
 * \param p2 Pointer to configuration option
 *
 * \retval NULL On failure
 * \retval apr_psprintf On Success
 */
static const char *cmd_hash_method_rx(cmd_parms *cmd, void *_dcfg,
        const char *p1, const char *p2)
{
    directory_config *dcfg = (directory_config *)_dcfg;
    rule_exception *re = apr_pcalloc(cmd->pool, sizeof(hash_method));
    const char *_p2 = apr_pstrdup(cmd->pool, p2);
    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "HashHref") == 0) {
        re->type = HASH_URL_HREF_HASH_RX;
        re->param = _p2;
        re->param_data = msc_pregcomp(cmd->pool, p2, 0, NULL, NULL);
        if (re->param_data == NULL) {
            return apr_psprintf(cmd->pool, "ModSecurity: Invalid regular expression: %s", p2);
        }
        dcfg->crypto_hash_href_rx = 1;
    }
    else if (strcasecmp(p1, "HashFormAction") == 0) {
        re->type = HASH_URL_FACTION_HASH_RX;
        re->param = _p2;
        re->param_data = msc_pregcomp(cmd->pool, p2, 0, NULL, NULL);
        if (re->param_data == NULL) {
            return apr_psprintf(cmd->pool, "ModSecurity: Invalid regular expression: %s", p2);
        }
        dcfg->crypto_hash_faction_rx = 1;
    }
    else if (strcasecmp(p1, "HashLocation") == 0) {
        re->type = HASH_URL_LOCATION_HASH_RX;
        re->param = _p2;
        re->param_data = msc_pregcomp(cmd->pool, p2, 0, NULL, NULL);
        if (re->param_data == NULL) {
            return apr_psprintf(cmd->pool, "ModSecurity: Invalid regular expression: %s", p2);
        }
        dcfg->crypto_hash_location_rx = 1;
    }
    else if (strcasecmp(p1, "HashIframeSrc") == 0) {
        re->type = HASH_URL_IFRAMESRC_HASH_RX;
        re->param = _p2;
        re->param_data = msc_pregcomp(cmd->pool, p2, 0, NULL, NULL);
        if (re->param_data == NULL) {
            return apr_psprintf(cmd->pool, "ModSecurity: Invalid regular expression: %s", p2);
        }
        dcfg->crypto_hash_iframesrc_rx = 1;
    }
    else if (strcasecmp(p1, "HashFrameSrc") == 0) {
        re->type = HASH_URL_FRAMESRC_HASH_RX;
        re->param = _p2;
        re->param_data = msc_pregcomp(cmd->pool, p2, 0, NULL, NULL);
        if (re->param_data == NULL) {
            return apr_psprintf(cmd->pool, "ModSecurity: Invalid regular expression: %s", p2);
        }
        dcfg->crypto_hash_framesrc_rx = 1;
    }

    *(hash_method **)apr_array_push(dcfg->hash_method) = re;

    return NULL;
}

/**
* \brief Add SecHttpBlKey configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On success
*/
static const char *cmd_httpBl_key(cmd_parms *cmd, void *_dcfg, const char *p1)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    if (dcfg == NULL) return NULL;

    if (p1 == NULL) return NULL;
    dcfg->httpBlkey = p1;

    return NULL;
}

/* PCRE Limits */

static const char *cmd_pcre_match_limit(cmd_parms *cmd,
        void *_dcfg, const char *p1)
{
    long val;

    if (cmd->server->is_virtual) {
        return "ModSecurity: SecPcreMatchLimit not allowed in VirtualHost";
    }

    val = atol(p1);
    if (val <= 0) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid setting for "
                "SecPcreMatchLimit: %s", p1);
    }
    msc_pcre_match_limit = (unsigned long int)val;

    return NULL;
}

static const char *cmd_pcre_match_limit_recursion(cmd_parms *cmd,
        void *_dcfg, const char *p1)
{
    long val;

    if (cmd->server->is_virtual) {
        return "ModSecurity: SecPcreMatchLimitRecursion not allowed in VirtualHost";
    }

    val = atol(p1);
    if (val <= 0) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid setting for "
                "SecPcreMatchLimitRecursion: %s", p1);
    }
    msc_pcre_match_limit_recursion = (unsigned long int)val;

    return NULL;
}


/* -- Geo Lookup configuration -- */

static const char *cmd_geo_lookup_db(cmd_parms *cmd, void *_dcfg,
        const char *p1)
{
    const char *filename = resolve_relative_path(cmd->pool, cmd->directive->filename, p1);
    char *error_msg;
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    if (geo_init(dcfg, filename, &error_msg) <= 0) {
        return error_msg;
    }

    return NULL;
}

/**
* \brief Add SecUnicodeCodePage configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On success
*/
static const char *cmd_unicode_codepage(cmd_parms *cmd,
                                        void *_dcfg, const char *p1)
{
    long val;

    val = atol(p1);
    if (val <= 0) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid setting for "
                                       "SecUnicodeCodePage: %s", p1);
    }

    unicode_codepage = (unsigned long int)val;

    return NULL;
}

/**
* \brief Add SecUnicodeMapFile configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On success
*/
static const char *cmd_unicode_map(cmd_parms *cmd, void *_dcfg,
                                     const char *p1)
{
    const char *filename = resolve_relative_path(cmd->pool, cmd->directive->filename, p1);
    char *error_msg;
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    if (unicode_map_init(dcfg, filename, &error_msg) <= 0) {
        return error_msg;
    }

    return NULL;
}

/**
* \brief Add SecGsbLookupDb configuration option
*
* \param cmd Pointer to configuration data
* \param _dcfg Pointer to directory configuration
* \param p1 Pointer to configuration option
*
* \retval NULL On success
*/
static const char *cmd_gsb_lookup_db(cmd_parms *cmd, void *_dcfg,
                                     const char *p1)
{
    const char *filename = resolve_relative_path(cmd->pool, cmd->directive->filename, p1);
    char *error_msg;
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    if (gsb_db_init(dcfg, filename, &error_msg) <= 0) {
        return error_msg;
    }

    return NULL;
}

/* -- Cache -- */

static const char *cmd_cache_transformations(cmd_parms *cmd, void *_dcfg,
                                             const char *p1, const char *p2)
{
    directory_config *dcfg = (directory_config *)_dcfg;

    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "on") == 0)
        dcfg->cache_trans = MODSEC_CACHE_ENABLED;
    else if (strcasecmp(p1, "off") == 0)
        dcfg->cache_trans = MODSEC_CACHE_DISABLED;
    else
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecCacheTransformations: %s", p1);

    /* Process options */
    if (p2 != NULL) {
        apr_table_t *vartable = apr_table_make(cmd->pool, 4);
        apr_status_t rc;
        char *error_msg = NULL;
        const char *charval = NULL;
        apr_int64_t intval = 0;

        if (vartable == NULL) {
            return apr_psprintf(cmd->pool, "ModSecurity: Unable to process options for SecCacheTransformations");
        }
        rc = msre_parse_generic(cmd->pool, p2, vartable, &error_msg);
        if (rc < 0) {
            return apr_psprintf(cmd->pool, "ModSecurity: Unable to parse options for SecCacheTransformations: %s", error_msg);
        }

        /* incremental */
        charval = apr_table_get(vartable, "incremental");
        if (charval != NULL) {
            if (strcasecmp(charval, "on") == 0)
                dcfg->cache_trans_incremental = 1;
            else if (strcasecmp(charval, "off") == 0)
                dcfg->cache_trans_incremental = 0;
            else
                return apr_psprintf(cmd->pool, "ModSecurity: SecCacheTransformations invalid incremental value: %s", charval);
        }

        /* minlen */
        charval = apr_table_get(vartable, "minlen");
        if (charval != NULL) {
            intval = apr_atoi64(charval);
            if (errno == ERANGE) {
                return apr_psprintf(cmd->pool, "ModSecurity: SecCacheTransformations minlen out of range: %s", charval);
            }
            if (intval < 0) {
                return apr_psprintf(cmd->pool, "ModSecurity: SecCacheTransformations minlen must be positive: %s", charval);
            }

            /* The NOT_SET indicator is -1, a signed long, and therfore
             * we cannot be >= the unsigned value of NOT_SET.
             */
            if ((unsigned long)intval >= (unsigned long)NOT_SET) {
                return apr_psprintf(cmd->pool, "ModSecurity: SecCacheTransformations minlen must be less than: %lu", (unsigned long)NOT_SET);
            }
            dcfg->cache_trans_min = (apr_size_t)intval;
        }

        /* maxlen */
        charval = apr_table_get(vartable, "maxlen");
        if (charval != NULL) {
            intval = apr_atoi64(charval);
            if (errno == ERANGE) {
                return apr_psprintf(cmd->pool, "ModSecurity: SecCacheTransformations maxlen out of range: %s", charval);
            }
            if (intval < 0) {
                return apr_psprintf(cmd->pool, "ModSecurity: SecCacheTransformations maxlen must be positive: %s", charval);
            }

            /* The NOT_SET indicator is -1, a signed long, and therfore
             * we cannot be >= the unsigned value of NOT_SET.
             */
            if ((unsigned long)intval >= (unsigned long)NOT_SET) {
                return apr_psprintf(cmd->pool, "ModSecurity: SecCacheTransformations maxlen must be less than: %lu", (unsigned long)NOT_SET);
            }
            if ((intval != 0) && ((apr_size_t)intval < dcfg->cache_trans_min)) {
                return apr_psprintf(cmd->pool, "ModSecurity: SecCacheTransformations maxlen must not be less than minlen: %lu < %" APR_SIZE_T_FMT, (unsigned long)intval, dcfg->cache_trans_min);
            }
            dcfg->cache_trans_max = (apr_size_t)intval;

        }

        /* maxitems */
        charval = apr_table_get(vartable, "maxitems");
        if (charval != NULL) {
            intval = apr_atoi64(charval);
            if (errno == ERANGE) {
                return apr_psprintf(cmd->pool, "ModSecurity: SecCacheTransformations maxitems out of range: %s", charval);
            }
            if (intval < 0) {
                return apr_psprintf(cmd->pool, "ModSecurity: SecCacheTransformations maxitems must be positive: %s", charval);
            }
            dcfg->cache_trans_maxitems = (apr_size_t)intval;
        }
    }

    return NULL;
}


/* -- Configuration directives definitions -- */

#define CMD_SCOPE_MAIN  (RSRC_CONF)
#define CMD_SCOPE_ANY   (RSRC_CONF | ACCESS_CONF)

#if defined(HTACCESS_CONFIG)
#define CMD_SCOPE_HTACCESS  (OR_OPTIONS)
#endif

const command_rec module_directives[] = {

#ifdef HTACCESS_CONFIG
    AP_INIT_TAKE1 (
        "SecAction",
        cmd_action,
        NULL,
        CMD_SCOPE_HTACCESS,
        "an action list"
    ),
#else
    AP_INIT_TAKE1 (
        "SecAction",
        cmd_action,
        NULL,
        CMD_SCOPE_ANY,
        "an action list"
    ),
#endif

    AP_INIT_TAKE1 (
        "SecArgumentSeparator",
        cmd_argument_separator,
        NULL,
        CMD_SCOPE_ANY,
        "character that will be used as separator when parsing application/x-www-form-urlencoded content."
    ),

    AP_INIT_TAKE1 (
        "SecCookiev0Separator",
        cmd_cookiev0_separator,
        NULL,
        CMD_SCOPE_ANY,
        "character that will be used as separator when parsing cookie v0 content."
    ),

    AP_INIT_TAKE1 (
        "SecAuditEngine",
        cmd_audit_engine,
        NULL,
        CMD_SCOPE_ANY,
        "On, Off or RelevantOnly to determine the level of audit logging"
    ),

    AP_INIT_TAKE1 (
        "SecAuditLog",
        cmd_audit_log,
        NULL,
        CMD_SCOPE_ANY,
        "filename of the primary audit log file"
    ),

    AP_INIT_TAKE1 (
        "SecAuditLog2",
        cmd_audit_log2,
        NULL,
        CMD_SCOPE_ANY,
        "filename of the secondary audit log file"
    ),

    AP_INIT_TAKE1 (
        "SecAuditLogParts",
        cmd_audit_log_parts,
        NULL,
        CMD_SCOPE_ANY,
        "list of audit log parts that go into the log."
    ),

    AP_INIT_TAKE1 (
        "SecAuditLogRelevantStatus",
        cmd_audit_log_relevant_status,
        NULL,
        CMD_SCOPE_ANY,
        "regular expression that will be used to determine if the response status is relevant for audit logging"
    ),

    AP_INIT_TAKE1 (
        "SecAuditLogType",
        cmd_audit_log_type,
        NULL,
        CMD_SCOPE_ANY,
        "whether to use the old audit log format (Serial) or new (Concurrent)"
    ),

    AP_INIT_TAKE1 (
        "SecAuditLogStorageDir",
        cmd_audit_log_storage_dir,
        NULL,
        CMD_SCOPE_ANY,
        "path to the audit log storage area; absolute, or relative to the root of the server"
    ),

    AP_INIT_TAKE1 (
        "SecAuditLogDirMode",
        cmd_audit_log_dirmode,
        NULL,
        CMD_SCOPE_ANY,
        "octal permissions mode for concurrent audit log directories"
    ),

    AP_INIT_TAKE1 (
        "SecAuditLogFileMode",
        cmd_audit_log_filemode,
        NULL,
        CMD_SCOPE_ANY,
        "octal permissions mode for concurrent audit log files"
    ),

    AP_INIT_TAKE12 (
        "SecCacheTransformations",
        cmd_cache_transformations,
        NULL,
        CMD_SCOPE_ANY,
        "whether or not to cache transformations. Defaults to true."
    ),

    AP_INIT_TAKE1 (
        "SecChrootDir",
        cmd_chroot_dir,
        NULL,
        CMD_SCOPE_MAIN,
        "path of the directory to which server will be chrooted"
    ),

    AP_INIT_TAKE1 (
        "SecComponentSignature",
        cmd_component_signature,
        NULL,
        CMD_SCOPE_MAIN,
        "component signature to add to ModSecurity signature."
    ),

    AP_INIT_FLAG (
        "SecContentInjection",
        cmd_content_injection,
        NULL,
        CMD_SCOPE_ANY,
        "On or Off"
    ),

    AP_INIT_FLAG (
        "SecStreamOutBodyInspection",
        cmd_stream_outbody_inspection,
        NULL,
        CMD_SCOPE_ANY,
        "On or Off"
    ),

    AP_INIT_FLAG (
        "SecStreamInBodyInspection",
        cmd_stream_inbody_inspection,
        NULL,
        CMD_SCOPE_ANY,
        "On or Off"
    ),

    AP_INIT_TAKE1 (
        "SecCookieFormat",
        cmd_cookie_format,
        NULL,
        CMD_SCOPE_ANY,
        "version of the Cookie specification to use for parsing. Possible values are 0 and 1."
    ),

    AP_INIT_TAKE1 (
        "SecDataDir",
        cmd_data_dir,
        NULL,
        CMD_SCOPE_MAIN,
        "path to the persistent data storage area" // TODO
    ),

    AP_INIT_TAKE1 (
        "SecDebugLog",
        cmd_debug_log,
        NULL,
        CMD_SCOPE_ANY,
        "path to the debug log file"
    ),

    AP_INIT_TAKE1 (
        "SecDebugLogLevel",
        cmd_debug_log_level,
        NULL,
        CMD_SCOPE_ANY,
        "debug log level, which controls the verbosity of logging."
        " Use values from 0 (no logging) to 9 (a *lot* of logging)."
    ),

    AP_INIT_TAKE1 (
        "SecCollectionTimeout",
        cmd_collection_timeout,
        NULL,
        CMD_SCOPE_ANY,
        "set default collections timeout. default it 3600"
    ),

    AP_INIT_TAKE1 (
        "SecDefaultAction",
        cmd_default_action,
        NULL,
        CMD_SCOPE_ANY,
        "default action list"
    ),

    AP_INIT_FLAG (
        "SecDisableBackendCompression",
        cmd_disable_backend_compression,
        NULL,
        CMD_SCOPE_ANY,
        "When set to On, removes the compression headers from the backend requests."
    ),

    AP_INIT_TAKE1 (
        "SecGsbLookupDB",
        cmd_gsb_lookup_db,
        NULL,
        RSRC_CONF,
        "database google safe browsing"
    ),

    AP_INIT_TAKE1 (
        "SecUnicodeCodePage",
        cmd_unicode_codepage,
        NULL,
        CMD_SCOPE_MAIN,
        "Unicode CodePage"
    ),

    AP_INIT_TAKE1 (
        "SecUnicodeMapFile",
        cmd_unicode_map,
        NULL,
        CMD_SCOPE_MAIN,
        "Unicode Map file"
    ),

    AP_INIT_TAKE1 (
        "SecGeoLookupDB",
        cmd_geo_lookup_db,
        NULL,
        RSRC_CONF,
        "database for geographical lookups module."
    ),

    AP_INIT_TAKE12 (
        "SecGuardianLog",
        cmd_guardian_log,
        NULL,
        CMD_SCOPE_MAIN,
        "The filename of the filter debugging log file"
    ),

    AP_INIT_TAKE1 (
        "SecMarker",
        cmd_marker,
        NULL,
        CMD_SCOPE_ANY,
        "marker for a skipAfter target"
    ),

    AP_INIT_TAKE1 (
        "SecPcreMatchLimit",
        cmd_pcre_match_limit,
        NULL,
        CMD_SCOPE_MAIN,
        "PCRE match limit"
    ),

    AP_INIT_TAKE1 (
        "SecPcreMatchLimitRecursion",
        cmd_pcre_match_limit_recursion,
        NULL,
        CMD_SCOPE_MAIN,
        "PCRE match limit recursion"
    ),

    AP_INIT_TAKE1 (
        "SecRequestBodyAccess",
        cmd_request_body_access,
        NULL,
        CMD_SCOPE_ANY,
        "On or Off"
    ),

    AP_INIT_TAKE1 (
        "SecInterceptOnError",
        cmd_request_intercept_on_error,
        NULL,
        CMD_SCOPE_ANY,
        "On or Off"
    ),

    AP_INIT_TAKE1 (
        "SecRulePerfTime",
        cmd_rule_perf_time,
        NULL,
        CMD_SCOPE_ANY,
        "Threshold to log slow rules in usecs."
    ),

    AP_INIT_TAKE1 (
        "SecReadStateLimit",
        cmd_conn_read_state_limit,
        NULL,
        CMD_SCOPE_ANY,
        "maximum number of threads in READ_BUSY state per ip address"
    ),

    AP_INIT_TAKE1 (
        "SecWriteStateLimit",
        cmd_conn_write_state_limit,
        NULL,
        CMD_SCOPE_ANY,
        "maximum number of threads in WRITE_BUSY state per ip address"
    ),

    AP_INIT_TAKE1 (
        "SecRequestBodyInMemoryLimit",
        cmd_request_body_inmemory_limit,
        NULL,
        CMD_SCOPE_ANY,
        "maximum request body size that will be placed in memory (except for POST urlencoded requests)."
    ),

    AP_INIT_TAKE1 (
        "SecRequestBodyLimit",
        cmd_request_body_limit,
        NULL,
        CMD_SCOPE_ANY,
        "maximum request body size ModSecurity will accept."
    ),

    AP_INIT_TAKE1 (
        "SecRequestBodyNoFilesLimit",
        cmd_request_body_no_files_limit,
        NULL,
        CMD_SCOPE_ANY,
        "maximum request body size ModSecurity will accept, but excluding the size of uploaded files."
    ),

    AP_INIT_TAKE1 (
        "SecRequestEncoding",
        cmd_request_encoding,
        NULL,
        CMD_SCOPE_ANY,
        "character encoding used in request."
    ),

    AP_INIT_TAKE1 (
        "SecResponseBodyAccess",
        cmd_response_body_access,
        NULL,
        CMD_SCOPE_ANY,
        "On or Off"
    ),

    AP_INIT_TAKE1 (
        "SecResponseBodyLimit",
        cmd_response_body_limit,
        NULL,
        CMD_SCOPE_ANY,
        "byte limit for response body"
    ),

    AP_INIT_TAKE1 (
        "SecResponseBodyLimitAction",
        cmd_response_body_limit_action,
        NULL,
        CMD_SCOPE_ANY,
        "what happens when the response body limit is reached"
    ),

    AP_INIT_TAKE1 (
        "SecRequestBodyLimitAction",
        cmd_resquest_body_limit_action,
        NULL,
        CMD_SCOPE_ANY,
        "what happens when the request body limit is reached"
    ),

    AP_INIT_ITERATE (
        "SecResponseBodyMimeType",
        cmd_response_body_mime_type,
        NULL,
        CMD_SCOPE_ANY,
        "adds given MIME types to the list of types that will be buffered on output"
    ),

    AP_INIT_NO_ARGS (
        "SecResponseBodyMimeTypesClear",
        cmd_response_body_mime_types_clear,
        NULL,
        CMD_SCOPE_ANY,
        "clears the list of MIME types that will be buffered on output"
    ),

#ifdef HTACCESS_CONFIG
    AP_INIT_TAKE23 (
        "SecRule",
        cmd_rule,
        NULL,
        CMD_SCOPE_HTACCESS,
        "rule target, operator and optional action list"
    ),
#else
    AP_INIT_TAKE23 (
        "SecRule",
        cmd_rule,
        NULL,
        CMD_SCOPE_ANY,
        "rule target, operator and optional action list"
    ),
#endif

    AP_INIT_TAKE1 (
        "SecRuleEngine",
        cmd_rule_engine,
        NULL,
        CMD_SCOPE_ANY,
        "On or Off"
    ),

    AP_INIT_TAKE1 (
        "SecXmlExternalEntity",
        cmd_xml_external_entity,
        NULL,
        CMD_SCOPE_ANY,
        "On or Off"
    ),

    AP_INIT_FLAG (
        "SecRuleInheritance",
        cmd_rule_inheritance,
        NULL,
        CMD_SCOPE_ANY,
        "On or Off"
    ),

    AP_INIT_TAKE12 (
        "SecRuleScript",
        cmd_rule_script,
        NULL,
        CMD_SCOPE_ANY,
        "rule script and optional actionlist"
    ),

#ifdef HTACCESS_CONFIG
    AP_INIT_ITERATE (
        "SecRuleRemoveById",
        cmd_rule_remove_by_id,
        NULL,
        CMD_SCOPE_HTACCESS,
        "rule ID for removal"
    ),

    AP_INIT_ITERATE (
        "SecRuleRemoveByTag",
        cmd_rule_remove_by_tag,
        NULL,
        CMD_SCOPE_HTACCESS,
        "rule tag for removal"
    ),

    AP_INIT_ITERATE (
        "SecRuleRemoveByMsg",
        cmd_rule_remove_by_msg,
        NULL,
        CMD_SCOPE_HTACCESS,
        "rule message for removal"
    ),
#else
    AP_INIT_ITERATE (
        "SecRuleRemoveById",
        cmd_rule_remove_by_id,
        NULL,
        CMD_SCOPE_ANY,
        "rule ID for removal"
    ),

    AP_INIT_ITERATE (
        "SecRuleRemoveByTag",
        cmd_rule_remove_by_tag,
        NULL,
        CMD_SCOPE_ANY,
        "rule tag for removal"
    ),

    AP_INIT_ITERATE (
        "SecRuleRemoveByMsg",
        cmd_rule_remove_by_msg,
        NULL,
        CMD_SCOPE_ANY,
        "rule message for removal"
    ),
#endif

    AP_INIT_TAKE2 (
        "SecHashMethodPm",
        cmd_hash_method_pm,
        NULL,
        CMD_SCOPE_ANY,
        "Hash method and pattern"
    ),

    AP_INIT_TAKE2 (
        "SecHashMethodRx",
        cmd_hash_method_rx,
        NULL,
        CMD_SCOPE_ANY,
        "Hash method and regex"
    ),

#ifdef HTACCESS_CONFIG
    AP_INIT_TAKE2 (
        "SecRuleUpdateActionById",
        cmd_rule_update_action_by_id,
        NULL,
        CMD_SCOPE_HTACCESS,
        "updated action list"
    ),

    AP_INIT_TAKE23 (
        "SecRuleUpdateTargetById",
        cmd_rule_update_target_by_id,
        NULL,
        CMD_SCOPE_HTACCESS,
        "updated target list"
    ),

    AP_INIT_TAKE23 (
        "SecRuleUpdateTargetByTag",
        cmd_rule_update_target_by_tag,
        NULL,
        CMD_SCOPE_HTACCESS,
        "rule tag pattern and updated target list"
    ),

    AP_INIT_TAKE23 (
        "SecRuleUpdateTargetByMsg",
        cmd_rule_update_target_by_msg,
        NULL,
        CMD_SCOPE_HTACCESS,
        "rule message pattern and updated target list"
    ),
#else
    AP_INIT_TAKE2 (
        "SecRuleUpdateActionById",
        cmd_rule_update_action_by_id,
        NULL,
        CMD_SCOPE_ANY,
        "updated action list"
    ),

    AP_INIT_TAKE23 (
        "SecRuleUpdateTargetById",
        cmd_rule_update_target_by_id,
        NULL,
        CMD_SCOPE_ANY,
        "updated target list"
    ),

    AP_INIT_TAKE23 (
        "SecRuleUpdateTargetByTag",
        cmd_rule_update_target_by_tag,
        NULL,
        CMD_SCOPE_ANY,
        "rule tag pattern and updated target list"
    ),

    AP_INIT_TAKE23 (
        "SecRuleUpdateTargetByMsg",
        cmd_rule_update_target_by_msg,
        NULL,
        CMD_SCOPE_ANY,
        "rule message pattern and updated target list"
    ),
#endif

    AP_INIT_TAKE1 (
        "SecServerSignature",
        cmd_server_signature,
        NULL,
        CMD_SCOPE_MAIN,
        "the new signature of the server"
    ),

    AP_INIT_TAKE1 (
        "SecTmpDir",
        cmd_tmp_dir,
        NULL,
        CMD_SCOPE_ANY,
        "path to the temporary storage area"
    ),

    AP_INIT_TAKE1 (
        "SecUploadDir",
        cmd_upload_dir,
        NULL,
        CMD_SCOPE_ANY,
        "path to the file upload area"
    ),

    AP_INIT_TAKE1 (
        "SecUploadFileLimit",
        cmd_upload_file_limit,
        NULL,
        CMD_SCOPE_ANY,
        "limit the number of uploaded files processed"
    ),

    AP_INIT_TAKE1 (
        "SecUploadFileMode",
        cmd_upload_filemode,
        NULL,
        CMD_SCOPE_ANY,
        "octal permissions mode for uploaded files"
    ),

    AP_INIT_TAKE1 (
        "SecUploadKeepFiles",
        cmd_upload_keep_files,
        NULL,
        CMD_SCOPE_ANY,
        "On or Off"
    ),

    AP_INIT_TAKE1 (
        "SecWebAppId",
        cmd_web_app_id,
        NULL,
        CMD_SCOPE_ANY,
        "id"
    ),

    AP_INIT_TAKE1 (
        "SecSensorId",
        cmd_sensor_id,
        NULL,
        CMD_SCOPE_MAIN,
        "sensor id"
    ),

    AP_INIT_TAKE1 (
        "SecHttpBlKey",
        cmd_httpBl_key,
        NULL,
        CMD_SCOPE_ANY,
        "httpBl access key"
    ),

    AP_INIT_TAKE1 (
        "SecHashEngine",
        cmd_hash_engine,
        NULL,
        CMD_SCOPE_ANY,
        "On or Off"
    ),

    AP_INIT_TAKE2 (
        "SecHashKey",
        cmd_hash_key,
        NULL,
        CMD_SCOPE_ANY,
        "Set Hash key"
    ),

    AP_INIT_TAKE1 (
        "SecHashParam",
        cmd_hash_param,
        NULL,
        CMD_SCOPE_ANY,
        "Set Hash parameter"
    ),

    { NULL }
};
