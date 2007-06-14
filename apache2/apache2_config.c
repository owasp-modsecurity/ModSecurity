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
#include <limits.h>

#include "modsecurity.h"
#include "msc_logging.h"
#include "http_log.h"

/* #define DEBUG_CONF 1 */

/* -- Directory context creation and initialisation -- */

/**
 * Creates a fresh directory configuration.
 */
void *create_directory_config(apr_pool_t *mp, char *path) {
    directory_config *dcfg = (directory_config *)apr_pcalloc(mp, sizeof(directory_config));
    if (dcfg == NULL) return NULL;

    #ifdef DEBUG_CONF
    fprintf(stderr, "Created directory config %x path %s\n", dcfg, path);
    #endif

    dcfg->mp = mp;
    dcfg->is_enabled = NOT_SET;

    dcfg->reqbody_access = NOT_SET;
    dcfg->reqbody_inmemory_limit = NOT_SET;
    dcfg->reqbody_limit = NOT_SET;
    dcfg->resbody_access = NOT_SET;

    dcfg->debuglog_name = NOT_SET_P;
    dcfg->debuglog_level = NOT_SET;    
    dcfg->debuglog_fd = NOT_SET_P;

    dcfg->of_limit = NOT_SET;
    dcfg->of_mime_types = NOT_SET_P;    
    dcfg->of_mime_types_cleared = NOT_SET;

    dcfg->cookie_format = NOT_SET;
    dcfg->argument_separator = NOT_SET;

    dcfg->rule_inheritance = NOT_SET;
    dcfg->rule_exceptions = apr_array_make(mp, 16, sizeof(rule_exception *));

    /* audit log variables */
    dcfg->auditlog_flag = NOT_SET;
    dcfg->auditlog_type = NOT_SET;
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

    /* These are only used during the configuration process. */
    dcfg->tmp_chain_starter = NULL;
    dcfg->tmp_default_actionset = NULL;

    /* Misc */
    dcfg->data_dir = NOT_SET_P;
    dcfg->webappid = NOT_SET_P;

    return dcfg;
}

/**
 * Copies rules between one phase of two configuration contexts,
 * taking exceptions into account.
 */
static void copy_rules_phase(apr_pool_t *mp, apr_array_header_t *parent_phase_arr,
    apr_array_header_t *child_phase_arr, apr_array_header_t *exceptions_arr)
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
                }
            }

            if (copy > 0) {
                /* Copy the rule. */
                *(msre_rule **)apr_array_push(child_phase_arr) = rule;
                if (rule->actionset->is_chained) mode = 2;
            } else {
                if (rule->actionset->is_chained) mode = 1;
            }
        } else {
            if (mode == 2) {
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
static int copy_rules(apr_pool_t *mp, msre_ruleset *parent_ruleset, msre_ruleset *child_ruleset,
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
void *merge_directory_configs(apr_pool_t *mp, void *_parent, void *_child) {
    directory_config *parent = (directory_config *)_parent;
    directory_config *child = (directory_config *)_child;
    directory_config *merged = create_directory_config(mp, NULL);

    #ifdef DEBUG_CONF
    fprintf(stderr, "Merge parent %x child %x RESULT %x\n", _parent, _child, merged);
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
    merged->reqbody_inmemory_limit = (child->reqbody_inmemory_limit == NOT_SET
        ? parent->reqbody_inmemory_limit : child->reqbody_inmemory_limit);
    merged->reqbody_limit = (child->reqbody_limit == NOT_SET
        ? parent->reqbody_limit : child->reqbody_limit);
    merged->resbody_access = (child->resbody_access == NOT_SET
        ? parent->resbody_access : child->resbody_access);

    merged->of_limit = (child->of_limit == NOT_SET
        ? parent->of_limit : child->of_limit);    

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


    /* rule inheritance */
    if ((child->rule_inheritance == NOT_SET)||(child->rule_inheritance == 1)) {
        merged->rule_inheritance = parent->rule_inheritance;
        if ((child->ruleset == NULL)&&(parent->ruleset == NULL)) {
            /* Do nothing, there are no rules in either context. */
        } else
        if (child->ruleset == NULL) {
            /* Copy the rules from the parent context. */
            merged->ruleset = msre_ruleset_create(parent->ruleset->engine, mp);
            copy_rules(mp, parent->ruleset, merged->ruleset, child->rule_exceptions);
        } else
        if (parent->ruleset == NULL) {
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

    /* audit log variables */
    merged->auditlog_flag = (child->auditlog_flag == NOT_SET
        ? parent->auditlog_flag : child->auditlog_flag);
    merged->auditlog_type = (child->auditlog_type == NOT_SET
        ? parent->auditlog_type : child->auditlog_type);    
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

    /* Misc */
    merged->data_dir = (child->data_dir == NOT_SET_P
        ? parent->data_dir : child->data_dir);
    merged->webappid = (child->webappid == NOT_SET_P
        ? parent->webappid : child->webappid);

    return merged;
}

/**
 * Initialise directory configuration. This function is *not* meant
 * to be called for directory configuration instances created during
 * the configuration phase. It can only be called on copies of those
 * (created fresh for every transaction).
 */
void init_directory_config(directory_config *dcfg) {
    if (dcfg == NULL) return;

    if (dcfg->is_enabled == NOT_SET) dcfg->is_enabled = 0;

    if (dcfg->reqbody_access == NOT_SET) dcfg->reqbody_access = 0;
    if (dcfg->reqbody_inmemory_limit == NOT_SET)
        dcfg->reqbody_inmemory_limit = REQUEST_BODY_DEFAULT_INMEMORY_LIMIT;
    if (dcfg->reqbody_limit == NOT_SET) dcfg->reqbody_limit = REQUEST_BODY_DEFAULT_LIMIT;
    if (dcfg->resbody_access == NOT_SET) dcfg->resbody_access = 0;
    if (dcfg->of_limit == NOT_SET) dcfg->of_limit = RESPONSE_BODY_DEFAULT_LIMIT;

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

    if (dcfg->rule_inheritance == NOT_SET) dcfg->rule_inheritance = 1;

    /* audit log variables */
    if (dcfg->auditlog_flag == NOT_SET) dcfg->auditlog_flag = 0;
    if (dcfg->auditlog_type == NOT_SET) dcfg->auditlog_type = AUDITLOG_SERIAL;
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

    /* Misc */
    if (dcfg->data_dir == NOT_SET_P) dcfg->data_dir = NULL;
    if (dcfg->webappid == NOT_SET_P) dcfg->webappid = "default";
}

/**
 * TODO
 */
static const char *add_rule(cmd_parms *cmd, directory_config *dcfg, const char *p1, 
    const char *p2, const char *p3)
{
    char *my_error_msg = NULL;
    msre_rule *rule = NULL;
    extern msc_engine *modsecurity;

    /* Create a ruleset if one does not exist. */
    if ((dcfg->ruleset == NULL)||(dcfg->ruleset == NOT_SET_P)) {
        dcfg->ruleset = msre_ruleset_create(modsecurity->msre, cmd->pool);
        if (dcfg->ruleset == NULL) return FATAL_ERROR;
    }

    /* Create the rule now. */
    rule = msre_rule_create(dcfg->ruleset, p1, p2, p3, &my_error_msg);
    if (rule == NULL) {
        return my_error_msg;
    }

    /* Create default actionset if one does not already exist. */
    if (dcfg->tmp_default_actionset == NULL) {
        dcfg->tmp_default_actionset = msre_actionset_create_default(modsecurity->msre);
        if (dcfg->tmp_default_actionset == NULL) return FATAL_ERROR;
    }

    /* Merge actions with the parent. */
    rule->actionset = msre_actionset_merge(modsecurity->msre, dcfg->tmp_default_actionset,
        rule->actionset, 1);

    if (dcfg->tmp_chain_starter != NULL) {
        /* This rule is part of a chain. */

        /* Must NOT specify a disruptive action. */
        if (rule->actionset->intercept_action == NOT_SET) {
            return apr_psprintf(cmd->pool, "ModSecurity: Disruptive actions can only "
                "be specified by chain starter rules.");
        }

        /* Must NOT specify a phase. */
        if (rule->actionset->phase == NOT_SET) {
            return apr_psprintf(cmd->pool, "ModSecurity: Execution phases can only be "
                "specified by chain starter rules.");
        }

        /* Must NOT use metadata actions. */
        if ((rule->actionset->id != NOT_SET_P)
            ||(rule->actionset->rev != NOT_SET_P)
            ||(rule->actionset->msg != NOT_SET_P))
        {
            return apr_psprintf(cmd->pool, "ModSecurity: Metadata actions (id, rev, msg) "
                " can only be specified by chain starter rules.");
        }

        /* Must NOT use skip. */
        if (rule->actionset->skip_count != NOT_SET) {
            return apr_psprintf(cmd->pool, "ModSecurity: The skip action can only be used "
                " by chain starter rules. ");
        }

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
    if (strcasecmp(rule->op_name, "inspectFile") == 0) {
        dcfg->upload_validates_files = 1;
    }

    /* Add rule to the recipe. */
    if (msre_ruleset_rule_add(dcfg->ruleset, rule, rule->actionset->phase) < 0) {
        return "Internal Error: Failed to add rule to the ruleset.";
    }

    return NULL;    
}

/* -- Configuration directives -- */

static const char *cmd_action(cmd_parms *cmd, void *_dcfg, const char *p1) {
    return add_rule(cmd, (directory_config *)_dcfg, "REQUEST_URI", "@unconditionalMatch", p1);
}

static const char *cmd_argument_separator(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = (directory_config *)_dcfg;

    if (strlen(p1) != 1) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid argument separator: %s", p1);
    }
    
    dcfg->argument_separator = p1[0];

    return NULL;
}

static const char *cmd_audit_engine(cmd_parms *cmd, void *_dcfg, const char *p1) {
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

static const char *cmd_audit_log(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = _dcfg;

    dcfg->auditlog_name = (char *)p1;

    if (dcfg->auditlog_name[0] == '|') {
        const char *pipe_name = ap_server_root_relative(cmd->pool, dcfg->auditlog_name + 1);
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

static const char *cmd_audit_log2(cmd_parms *cmd, void *_dcfg, const char *p1) {
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

static const char *cmd_audit_log_parts(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = _dcfg;

    if (is_valid_parts_specification((char *)p1) != 1) {
        return apr_psprintf(cmd->pool, "Invalid parts specification for SecAuditLogParts: %s", p1);
    }

    dcfg->auditlog_parts = (char *)p1;
    return NULL;
}

static const char *cmd_audit_log_relevant_status(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = _dcfg;

    dcfg->auditlog_relevant_regex = msc_pregcomp(cmd->pool, p1, PCRE_DOTALL, NULL, NULL);
    if (dcfg->auditlog_relevant_regex == NULL) {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid regular expression: %s", p1);
    }

    return NULL;
}

static const char *cmd_audit_log_type(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = _dcfg;

    if (strcasecmp(p1, "Serial") == 0) dcfg->auditlog_type = AUDITLOG_SERIAL;
    else
    if (strcasecmp(p1, "Concurrent") == 0) dcfg->auditlog_type = AUDITLOG_CONCURRENT;
    else
    return (const char *)apr_psprintf(cmd->pool,
        "ModSecurity: Unrecognised parameter value for SecAuditLogType: %s", p1);

    return NULL;
}

static const char *cmd_audit_log_storage_dir(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = _dcfg;

    dcfg->auditlog_storage_dir = ap_server_root_relative(cmd->pool, p1);

    return NULL;
}

static const char *cmd_cookie_format(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = (directory_config *)_dcfg;

    if (strcmp(p1, "0") == 0) dcfg->cookie_format = COOKIES_V0;
    else
    if (strcmp(p1, "1") == 0) dcfg->cookie_format = COOKIES_V1;
    else {
        return apr_psprintf(cmd->pool, "ModSecurity: Invalid cookie format: %s", p1);
    }

    return NULL;
}

static const char *cmd_chroot_dir(cmd_parms *cmd, void *_dcfg, const char *p1) {
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

static const char *cmd_data_dir(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = (directory_config *)_dcfg;

    if (cmd->server->is_virtual) {
        return "ModSecurity: SecDataDir not allowed in VirtualHost.";
    }

    dcfg->data_dir = ap_server_root_relative(cmd->pool, p1);

    return NULL;
}

static const char *cmd_debug_log(cmd_parms *cmd, void *_dcfg, const char *p1) {
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

static const char *cmd_debug_log_level(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = (directory_config *)_dcfg;

    dcfg->debuglog_level = atoi(p1);
    if ((dcfg->debuglog_level >= 0)&&(dcfg->debuglog_level <= 9)) return NULL;

    return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecDebugLogLevel: %s", p1);
}

static const char *cmd_default_action(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = (directory_config *)_dcfg;
    extern msc_engine *modsecurity;
    char *my_error_msg = NULL;

    dcfg->tmp_default_actionset = msre_actionset_create(modsecurity->msre, p1, &my_error_msg);
    if (dcfg->tmp_default_actionset == NULL) {
        if (my_error_msg != NULL) return my_error_msg;
        else return FATAL_ERROR;
    }

    /* Must specify a disruptive action. */
    if (dcfg->tmp_default_actionset->intercept_action == NOT_SET) {
        return apr_psprintf(cmd->pool, "ModSecurity: SecDefaultAction must specify a disruptive action.");
    }

    /* Must specify a phase. */
    if (dcfg->tmp_default_actionset->phase == NOT_SET) {
        return apr_psprintf(cmd->pool, "ModSecurity: SecDefaultAction must specify a phase.");
    }

    /* Must not use metadata actions. */
    if ((dcfg->tmp_default_actionset->id != NOT_SET_P)
        ||(dcfg->tmp_default_actionset->rev != NOT_SET_P)
        ||(dcfg->tmp_default_actionset->msg != NOT_SET_P))
    {
        return apr_psprintf(cmd->pool, "ModSecurity: SecDefaultAction must not "
            "contain any metadata actions (id, rev, msg).");
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

    return NULL;
}

static const char *cmd_guardian_log(cmd_parms *cmd, void *_dcfg, const char *p1, const char *p2) {
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

static const char *cmd_request_body_inmemory_limit(cmd_parms *cmd, void *_dcfg, const char *p1) {
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

static const char *cmd_request_body_limit(cmd_parms *cmd, void *_dcfg, const char *p1) {
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

static const char *cmd_request_body_access(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "on") == 0) dcfg->reqbody_access = 1;
    else
    if (strcasecmp(p1, "off") == 0) dcfg->reqbody_access = 0;
    else
    return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecRequestBodyAccess: %s", p1);

    return NULL;
}

static const char *cmd_response_body_access(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "on") == 0) dcfg->resbody_access = 1;
    else
    if (strcasecmp(p1, "off") == 0) dcfg->resbody_access = 0;
    else
    return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecResponseBodyAccess: %s", p1);

    return NULL;
}

static const char *cmd_response_body_limit(cmd_parms *cmd, void *_dcfg, const char *p1) {
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

static const char *cmd_response_body_mime_type(cmd_parms *cmd, void *_dcfg, const char *_p1) {
    directory_config *dcfg = (directory_config *)_dcfg;
    char *p1 = apr_pstrdup(cmd->pool, _p1);

    // TODO check whether the parameter is a valid MIME type of "null"

    if ((dcfg->of_mime_types == NULL)||(dcfg->of_mime_types == NOT_SET_P)) {
        dcfg->of_mime_types = apr_table_make(cmd->pool, 10);
    }

    strtolower_inplace((unsigned char *)p1);
    apr_table_setn(dcfg->of_mime_types, p1, "1");

    return NULL;
}

static const char *cmd_response_body_mime_types_clear(cmd_parms *cmd, void *_dcfg) {
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    dcfg->of_mime_types_cleared = 1;

    if ((dcfg->of_mime_types != NULL)&&(dcfg->of_mime_types != NOT_SET_P)) {
        apr_table_clear(dcfg->of_mime_types);
    }

    return NULL;
}

static const char *cmd_rule(cmd_parms *cmd, void *_dcfg, const char *p1,
    const char *p2, const char *p3)
{
    return add_rule(cmd, (directory_config *)_dcfg, p1, p2, p3);
}

static const char *cmd_rule_engine(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "on") == 0) dcfg->is_enabled = MODSEC_ENABLED;
    else
    if (strcasecmp(p1, "off") == 0) dcfg->is_enabled = MODSEC_DISABLED;
    else
    if (strcasecmp(p1, "detectiononly") == 0) dcfg->is_enabled = MODSEC_DETECTION_ONLY;
    else
    return apr_psprintf(cmd->pool, "ModSecurity: Invalid value for SecRuleEngine: %s", p1);

    return NULL;
}

/*
static const char *cmd_rule_import_by_id(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = (directory_config *)_dcfg;
    rule_exception *re = apr_pcalloc(cmd->pool, sizeof(rule_exception));
    if (dcfg == NULL) return NULL;

    re->type = RULE_EXCEPTION_IMPORT_ID;
    // TODO verify p1
    re->param = p1;
    *(rule_exception **)apr_array_push(dcfg->rule_exceptions) = re;
    
    return NULL;
}

static const char *cmd_rule_import_by_msg(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = (directory_config *)_dcfg;
    rule_exception *re = apr_pcalloc(cmd->pool, sizeof(rule_exception));
    if (dcfg == NULL) return NULL;

    re->type = RULE_EXCEPTION_IMPORT_MSG;
    // TODO verify p1
    re->param = p1;
    *(rule_exception **)apr_array_push(dcfg->rule_exceptions) = re;    

    return NULL;
}
*/

static const char *cmd_rule_inheritance(cmd_parms *cmd, void *_dcfg, int flag) {
    directory_config *dcfg = (directory_config *)_dcfg;
    if (dcfg == NULL) return NULL;
    dcfg->rule_inheritance = flag;
    return NULL;
}

static const char *cmd_rule_remove_by_id(cmd_parms *cmd, void *_dcfg, const char *p1) {
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

static const char *cmd_rule_remove_by_msg(cmd_parms *cmd, void *_dcfg, const char *p1) {
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
    fprintf(stderr, "Added exception %x (%i %s) to dcfg %x.\n", re, re->type, re->param, dcfg);
    #endif

    return NULL;
}

static const char *cmd_server_signature(cmd_parms *cmd, void *_dcfg, const char *p1) {
    if (cmd->server->is_virtual) {
        return "ModSecurity: SecServerSignature not allowed in VirtualHost";
    }
    new_server_signature = (char *)p1;
    return NULL;
}

static const char *cmd_tmp_dir(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = (directory_config *)_dcfg;

    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "none") == 0) dcfg->tmp_dir = NULL;
    else dcfg->tmp_dir = ap_server_root_relative(cmd->pool, p1);

    return NULL;
}

static const char *cmd_upload_dir(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = (directory_config *)_dcfg;

    if (dcfg == NULL) return NULL;

    if (strcasecmp(p1, "none") == 0) dcfg->upload_dir = NULL;
    else dcfg->upload_dir = ap_server_root_relative(cmd->pool, p1);

    return NULL;
}

static const char *cmd_upload_keep_files(cmd_parms *cmd, void *_dcfg, const char *p1) {
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

static const char *cmd_web_app_id(cmd_parms *cmd, void *_dcfg, const char *p1) {
    directory_config *dcfg = (directory_config *)_dcfg;

    // TODO enforce format (letters, digits, ., _, -)
    dcfg->webappid = p1;

    return NULL;
}


/* -- Configuration directives definitions -- */

#define CMD_SCOPE_MAIN  (RSRC_CONF)
#define CMD_SCOPE_ANY   (RSRC_CONF | ACCESS_CONF)

const command_rec module_directives[] = {

    AP_INIT_TAKE1 (
        "SecAction",
        cmd_action,
        NULL,
        CMD_SCOPE_ANY,
        "" // TODO
    ),
    
    AP_INIT_TAKE1 (
        "SecArgumentSeparator",
        cmd_argument_separator,
        NULL,
        CMD_SCOPE_MAIN,
        "character that will be used as separator when parsing application/x-www-form-urlencoded content."
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
        "The filename of the primary audit log file"
    ),

    AP_INIT_TAKE1 (
        "SecAuditLog2",
        cmd_audit_log2,
        NULL,
        CMD_SCOPE_ANY,
        "The filename of the secondary audit log file"
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
        "SecChrootDir",
        cmd_chroot_dir,
        NULL,
        CMD_SCOPE_MAIN,
        "Path of the directory to which server will be chrooted"
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
        "" // TODO
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
        "SecDefaultAction",
        cmd_default_action,
        NULL,
        CMD_SCOPE_ANY,
        "" // TODO
    ),

    AP_INIT_TAKE12 (
        "SecGuardianLog",
        cmd_guardian_log,
        NULL,
        CMD_SCOPE_MAIN,
        "The filename of the filter debugging log file"
    ),

    AP_INIT_TAKE1 (
        "SecRequestBodyAccess",
        cmd_request_body_access,
        NULL,
        CMD_SCOPE_ANY,
        "On or Off"
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
        "maximum request body size ModSecurity is allowed to access."
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
        "" // TODO
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

    AP_INIT_TAKE23 (
        "SecRule",
        cmd_rule,
        NULL,
        CMD_SCOPE_ANY,
        "" // TODO
    ),

    AP_INIT_TAKE1 (
        "SecRuleEngine",
        cmd_rule_engine,
        NULL,
        CMD_SCOPE_ANY,
        "On or Off"
    ),

    /*
    AP_INIT_TAKE1 (
        "SecRuleImportById",
        cmd_rule_import_by_id,
        NULL,
        CMD_SCOPE_ANY,
        "" // TODO
    ),

    AP_INIT_TAKE1 (
        "SecRuleImportByMsg",
        cmd_rule_import_by_msg,
        NULL,
        CMD_SCOPE_ANY,
        "" // TODO
    ),
    */

    AP_INIT_FLAG (
        "SecRuleInheritance",
        cmd_rule_inheritance,
        NULL,
        CMD_SCOPE_ANY,
        "On or Off"
    ),

    AP_INIT_ITERATE (
        "SecRuleRemoveById",
        cmd_rule_remove_by_id,
        NULL,
        CMD_SCOPE_ANY,
        "" // TODO
    ),

    AP_INIT_ITERATE (
        "SecRuleRemoveByMsg",
        cmd_rule_remove_by_msg,
        NULL,
        CMD_SCOPE_ANY,
        "" // TODO
    ),

    AP_INIT_TAKE1 (
        "SecServerSignature",
        cmd_server_signature,
        NULL,
        CMD_SCOPE_MAIN,
        "The new signature of the server"
    ),

    AP_INIT_TAKE1 (
        "SecTmpDir",
        cmd_tmp_dir,
        NULL,
        CMD_SCOPE_ANY,
        "" // TODO
    ),

    AP_INIT_TAKE1 (
        "SecUploadDir",
        cmd_upload_dir,
        NULL,
        CMD_SCOPE_ANY,
        "" // TODO
    ),

    AP_INIT_TAKE1 (
        "SecUploadKeepFiles",
        cmd_upload_keep_files,
        NULL,
        CMD_SCOPE_ANY,
        "" // TODO
    ),

    AP_INIT_TAKE1 (
        "SecWebAppId",
        cmd_web_app_id,
        NULL,
        CMD_SCOPE_ANY,
        "" // TODO
    ),

    { NULL }
};
