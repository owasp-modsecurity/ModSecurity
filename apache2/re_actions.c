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

#include "re.h"
#include <ctype.h>
#include "apr_lib.h"
#include "apr_strmatch.h"


/**
 * Register action with the engine.
 */
static void msre_engine_action_register(msre_engine *engine, const char *name,
    unsigned int type, unsigned int argc_min, unsigned int argc_max,
    unsigned int allow_param_plusminus, unsigned int cardinality,
    unsigned int cardinality_group, fn_action_validate_t validate,
    fn_action_init_t init, fn_action_execute_t execute)
{
    msre_action_metadata *metadata = (msre_action_metadata *)apr_pcalloc(engine->mp,
        sizeof(msre_action_metadata));
    if (metadata == NULL) return;

    metadata->name = name;
    metadata->type = type;
    metadata->argc_min = argc_min;
    metadata->argc_max = argc_max;
    metadata->allow_param_plusminus = allow_param_plusminus;
    metadata->cardinality = cardinality;
    metadata->cardinality_group = cardinality_group;
    metadata->validate = validate;
    metadata->init = init;
    metadata->execute = execute;

    apr_table_setn(engine->actions, name, (void *)metadata);
}

/**
 * Generates a single variable (from the supplied metadata).
 */
msre_var *generate_single_var(modsec_rec *msr, msre_var *var, apr_array_header_t *tfn_arr,
    msre_rule *rule, apr_pool_t *mptmp)
{
    apr_table_t *vartab = NULL;
    const apr_table_entry_t *te = NULL;
    const apr_array_header_t *arr = NULL;
    msre_var *rvar = NULL;
    int i;

    /* Sanity check. */
    if ((var == NULL)||(var->metadata == NULL)||(var->metadata->generate == NULL)) return NULL;

    vartab = apr_table_make(mptmp, 16);
    var->metadata->generate(msr, var, rule, vartab, mptmp);

    arr = apr_table_elts(vartab);
    if (arr->nelts == 0) return NULL;
    te = (apr_table_entry_t *)arr->elts;

    rvar = (msre_var *)te[0].val;

    /* Return straight away if there were no
     * transformation functions supplied.
     */
    if ((tfn_arr == NULL)||(tfn_arr->nelts == 0)) {
        return rvar;
    }

    /* Copy the value so that we can transform it in place. */
    rvar->value = apr_pstrndup(mptmp, rvar->value, rvar->value_len);

    /* Transform rvar in a loop. */
    for (i = 0; i < tfn_arr->nelts; i++) {
        msre_tfn_metadata *tfn = ((msre_tfn_metadata **)tfn_arr->elts)[i];
        char *rval;
        int rc;
        long int rval_len;

        rc = tfn->execute(mptmp, (unsigned char *)rvar->value,
                    rvar->value_len, &rval, &rval_len);

        rvar->value = rval;
        rvar->value_len = rval_len;

        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "T (%d) %s: \"%s\"", rc, tfn->name,
                log_escape_nq_ex(mptmp, rvar->value, rvar->value_len));
        }
    }

    return rvar;
}

#if defined(WITH_LUA)
/**
 *
 */
apr_table_t *generate_multi_var(modsec_rec *msr, msre_var *var, apr_array_header_t *tfn_arr,
    msre_rule *rule, apr_pool_t *mptmp)
{
    const apr_array_header_t *tarr;
    const apr_table_entry_t *telts;
    apr_table_t *vartab = NULL, *tvartab = NULL;
    msre_var *rvar = NULL;
    int i, j;

    /* Sanity check. */
    if ((var == NULL)||(var->metadata == NULL)||(var->metadata->generate == NULL)) return NULL;

    /* Generate variables. */
    vartab = apr_table_make(mptmp, 16);
    var->metadata->generate(msr, var, rule, vartab, mptmp);

    /* Return straight away if there were no
     * transformation functions supplied.
     */
    if ((tfn_arr == NULL)||(tfn_arr->nelts == 0)) {
        return vartab;
    }

    tvartab = apr_table_make(mptmp, 16);

    tarr = apr_table_elts(vartab);
    telts = (const apr_table_entry_t*)tarr->elts;
    for (j = 0; j < tarr->nelts; j++) {
        rvar = (msre_var *)telts[j].val;

        /* Copy the value so that we can transform it in place. */
        rvar->value = apr_pstrndup(mptmp, rvar->value, rvar->value_len);

        /* Transform rvar in a loop. */
        for (i = 0; i < tfn_arr->nelts; i++) {
            msre_tfn_metadata *tfn = ((msre_tfn_metadata **)tfn_arr->elts)[i];
            char *rval;
            int rc;
            long int rval_len;

            rc = tfn->execute(mptmp, (unsigned char *)rvar->value,
                rvar->value_len, &rval, &rval_len);

            rvar->value = rval;
            rvar->value_len = rval_len;

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "T (%d) %s: \"%s\"", rc, tfn->name,
                    log_escape_nq_ex(mptmp, rvar->value, rvar->value_len));
            }
        }

        apr_table_addn(tvartab, rvar->name, (void *)rvar);
    }

    return tvartab;
}
#endif

/**
 * Expands macros ("%{NAME}" entities) if present
 * in the given variable.
 */
int expand_macros(modsec_rec *msr, msc_string *var, msre_rule *rule, apr_pool_t *mptmp) {
    char *data = NULL;
    apr_array_header_t *arr = NULL;
    char *p = NULL, *q = NULL, *t = NULL;
    char *text_start = NULL, *next_text_start = NULL;
    msc_string *part = NULL;
    int i, offset = 0;

    if (var->value == NULL) return 0;

    /* IMP1 Duplicate the string and create the array on
     *      demand, thus not having to do it if there are
     *      no macros in the input data.
     */

    data = apr_pstrdup(mptmp, var->value); /* IMP1 Are we modifying data anywhere? */
    arr = apr_array_make(mptmp, 16, sizeof(msc_string *));
    if ((data == NULL)||(arr == NULL)) return -1;

    text_start = next_text_start = data;
    do {
        text_start = next_text_start;
        p = strstr(text_start, "%");
        if (p != NULL) {
            char *var_name = NULL;
            char *var_value = NULL;

            if ((*(p + 1) == '{')&&(*(p + 2) != '\0')) {
                char *var_start = p + 2;

                t = var_start;
                while((*t != '\0')&&(*t != '}')) t++;
                if (*t == '}') {
                    /* Named variable. */

                    var_name = apr_pstrmemdup(mptmp, var_start, t - var_start);
                    q = strstr(var_name, ".");
                    if (q != NULL) {
                        var_value = q + 1;
                        *q = '\0';
                    }

                    next_text_start = t + 1; /* *t was '}' */
                } else {
                    /* Warn about a possiblly forgotten '}' */
                    if (msr->txcfg->debuglog_level >= 9) {
                        msr_log(msr, 9, "Warning: Possibly unterminated macro: \"%s\"",
                            log_escape_ex(mptmp, var_start - 2, t - var_start + 2));
                    }

                    next_text_start = t; /* *t was '\0' */
                }
            }

            if (var_name != NULL) {
                char *my_error_msg = NULL;
                msre_var *var_generated = NULL;
                msre_var *var_resolved = NULL;

                /* Add the text part before the macro to the array. */
                part = (msc_string *)apr_pcalloc(mptmp, sizeof(msc_string));
                if (part == NULL) return -1;
                part->value_len = p - text_start;
                part->value = apr_pstrmemdup(mptmp, text_start, part->value_len);
                *(msc_string **)apr_array_push(arr) = part;

                /* Resolve the macro and add that to the array. */
                var_resolved = msre_create_var_ex(mptmp, msr->modsecurity->msre, var_name, var_value,
                    msr, &my_error_msg);
                if (var_resolved != NULL) {
                    var_generated = generate_single_var(msr, var_resolved, NULL, rule, mptmp);
                    if (var_generated != NULL) {
                        part = (msc_string *)apr_pcalloc(mptmp, sizeof(msc_string));
                        if (part == NULL) return -1;
                        part->value_len = var_generated->value_len;
                        part->value = (char *)var_generated->value;
                        *(msc_string **)apr_array_push(arr) = part;
                        if (msr->txcfg->debuglog_level >= 9) {
                            msr_log(msr, 9, "Resolved macro %%{%s%s%s} to: %s",
                                var_name,
                                (var_value ? "." : ""),
                                (var_value ? var_value : ""),
                                log_escape_nq_ex(mptmp, part->value, part->value_len));
                        }
                    }
                } else {
                    if (msr->txcfg->debuglog_level >= 4) {
                        msr_log(msr, 4, "Failed to resolve macro %%{%s%s%s}: %s",
                            var_name,
                            (var_value ? "." : ""),
                            (var_value ? var_value : ""),
                            my_error_msg);
                    }
                }
            } else {
                /* We could not identify a valid macro so add it as text. */
                part = (msc_string *)apr_pcalloc(mptmp, sizeof(msc_string));
                if (part == NULL) return -1;
                part->value_len = p - text_start + 1; /* len(text)+len("%") */
                part->value = apr_pstrmemdup(mptmp, text_start, part->value_len);
                *(msc_string **)apr_array_push(arr) = part;

                next_text_start = p + 1;
            }
        } else {
            /* Text part. */
            part = (msc_string *)apr_pcalloc(mptmp, sizeof(msc_string));
            part->value = apr_pstrdup(mptmp, text_start);
            part->value_len = strlen(part->value);
            *(msc_string **)apr_array_push(arr) = part;
        }
    } while (p != NULL);

    /* If there's more than one member of the array that
     * means there was at least one macro present. Combine
     * text parts into a single string now.
     */
    if (arr->nelts > 1) {
        /* Figure out the required size for the string. */
        var->value_len = 0;
        for(i = 0; i < arr->nelts; i++) {
            part = ((msc_string **)arr->elts)[i];
            var->value_len += part->value_len;
        }

        /* Allocate the string. */
        var->value = apr_palloc(msr->mp, var->value_len + 1);
        if (var->value == NULL) return -1;

        /* Combine the parts. */
        offset = 0;
        for(i = 0; i < arr->nelts; i++) {
            part = ((msc_string **)arr->elts)[i];
            memcpy((char *)(var->value + offset), part->value, part->value_len);
            offset += part->value_len;
        }
        var->value[offset] = '\0';
    }

    return 1;
}

/**
 * Record the original collection values to use to calculate deltas.
 * This can be called multiple times and will not overwrite the first
 * value that is set.
 */
apr_status_t collection_original_setvar(modsec_rec *msr, const char *col_name, const msc_string *orig_var) {
    apr_table_t *table = NULL;
    msc_string *var = NULL;
    const char *var_name = NULL;

    if (orig_var == NULL) {
        msr_log(msr, 1, "Internal Error: Attempt to record NULL original variable.");
        return -1;
    }

    var_name = orig_var->name;
    table = (apr_table_t *)apr_table_get(msr->collections_original, col_name);

    /* Does the collection exist already? */
    if (table == NULL) {
        table = apr_table_make(msr->mp, 24);
        if (table == NULL) {
            msr_log(msr, 1, "Failed to allocate space for original collection.");
            return -1;
        }
        apr_table_setn(msr->collections_original, apr_pstrdup(msr->mp, col_name), (void *)table);
    }
    else {
        /* Does the variable exist already? */
        var = (msc_string *)apr_table_get(table, var_name);
        if (var != NULL) {
            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Original collection variable: %s.%s = \"%s\"", col_name, var_name,
                    log_escape_ex(msr->mp, orig_var->value, orig_var->value_len));
            }
            return 1;
        }
    }

    var = (msc_string *)apr_palloc(msr->mp, sizeof(msc_string));
    if (var == NULL) {
        msr_log(msr, 1, "Failed to allocate space for original collection variable.");
        return -1;
    }

    /* Copy the original var and add to collection. */
    var->name = orig_var->name ? apr_pstrmemdup(msr->mp, orig_var->name, orig_var->name_len) : NULL;
    var->name_len = orig_var->name_len;
    var->value = orig_var->value ? apr_pstrmemdup(msr->mp, orig_var->value, orig_var->value_len) : NULL;
    var->value_len = orig_var->value_len;
    apr_table_setn(table, apr_pstrmemdup(msr->mp, var->name, var->name_len), (void *)var);

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "Recorded original collection variable: %s.%s = \"%s\"", col_name, var_name,
            log_escape_ex(msr->mp, var->value, var->value_len));
    }

    return 0;
}

/* marker */
static apr_status_t msre_action_marker_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->id = action->param;
    return 1;
}

/* id */

static apr_status_t msre_action_id_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->id = action->param;
    return 1;
}

static char *msre_action_id_validate(msre_engine *engine, msre_action *action) {
    int id;

    if(action != NULL && action->param != NULL) {
        for(id=0;id<strlen(action->param);id++) {
            if(!apr_isdigit(action->param[id]))
                return apr_psprintf(engine->mp, "ModSecurity: Invalid value for action ID: %s", action->param);
        }
        id = atoi(action->param);
        if ((id == LONG_MAX)||(id == LONG_MIN)||(id <= 0)) {
            return apr_psprintf(engine->mp, "ModSecurity: Invalid value for action ID: %s", action->param);
        }
    }

    return NULL;
}

/* rev */

static apr_status_t msre_action_rev_init(msre_engine *engine, msre_actionset *actionset,
        msre_action *action)
{
    actionset->rev = action->param;
    return 1;
}

/* msg */

static apr_status_t msre_action_msg_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->msg = action->param;
    return 1;
}

/* logdata */

static apr_status_t msre_action_logdata_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->logdata = action->param;
    return 1;
}

/* SanitizeMatchedBytes init */

static apr_status_t msre_action_sanitizeMatchedBytes_init(msre_engine *engine,
        msre_actionset *actionset, msre_action *action)
{
    char *parse_parm = NULL;
    char *ac_param = NULL;
    char *savedptr = NULL;
    int arg_min = 0;
    int arg_max = 0;

    if (action->param != NULL && strlen(action->param) == 3)   {

        ac_param = apr_pstrdup(engine->mp, action->param);
        parse_parm = apr_strtok(ac_param,"/",&savedptr);

        if(apr_isdigit(*parse_parm) && apr_isdigit(*savedptr))    {
            arg_max = atoi(parse_parm);
            arg_min = atoi(savedptr);
        }
    }

    actionset->arg_min = arg_min;
    actionset->arg_max = arg_max;

    return 1;
}

/* accuracy */

static apr_status_t msre_action_accuracy_init(msre_engine *engine,
    msre_actionset *actionset, msre_action *action)
{
    actionset->accuracy = atoi(action->param);
    return 1;
}

/* maturity */

static apr_status_t msre_action_maturity_init(msre_engine *engine,
    msre_actionset *actionset, msre_action *action)
{
    actionset->maturity = atoi(action->param);
    return 1;
}

/* ver */

static apr_status_t msre_action_ver_init(msre_engine *engine,
    msre_actionset *actionset, msre_action *action)
{
    actionset->version = action->param;
    return 1;
}

/* severity */

static apr_status_t msre_action_severity_init(msre_engine *engine,
    msre_actionset *actionset, msre_action *action)
{
    actionset->severity = atoi(action->param);
    return 1;
}

/* chain */

static apr_status_t msre_action_chain_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->is_chained = 1;
    return 1;
}

/* log */
static apr_status_t msre_action_log_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->log = 1;
    return 1;
}

/* nolog */
static apr_status_t msre_action_nolog_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->log = 0;
    actionset->auditlog = 0;
    return 1;
}

/* auditlog */
static apr_status_t msre_action_auditlog_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->auditlog = 1;
    return 1;
}

/* noauditlog */
static apr_status_t msre_action_noauditlog_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->auditlog = 0;
    return 1;
}

/* block */
static apr_status_t msre_action_block_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    /* Right now we just set a flag and inherit the real disruptive action */
    actionset->block = 1;
    return 1;
}

/* deny */
static apr_status_t msre_action_deny_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->intercept_action = ACTION_DENY;
    actionset->intercept_action_rec = action;
    return 1;
}

/* status */
static char *msre_action_status_validate(msre_engine *engine, msre_action *action) {
    /* ENH action->param must be a valid HTTP status code. */
    return NULL;
}

static apr_status_t msre_action_status_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->intercept_status = atoi(action->param);
    return 1;
}

/* drop */
static apr_status_t msre_action_drop_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->intercept_action = ACTION_DROP;
    actionset->intercept_action_rec = action;
    return 1;
}

/* pause */
static char *msre_action_pause_validate(msre_engine *engine, msre_action *action) {
    /* ENH Validate a positive number. */
    return NULL;
}

static apr_status_t msre_action_pause_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->intercept_action = ACTION_PAUSE;
    actionset->intercept_pause = action->param;
    return 1;
}

/* redirect */

static char *msre_action_redirect_validate(msre_engine *engine, msre_action *action) {
    /* ENH Add validation. */
    return NULL;
}

static apr_status_t msre_action_redirect_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->intercept_action = ACTION_REDIRECT;
    actionset->intercept_uri = action->param;
    actionset->intercept_action_rec = action;
    return 1;
}

static apr_status_t msre_action_redirect_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    msc_string *var = NULL;

    var = apr_pcalloc(mptmp, sizeof(msc_string));
    if (var == NULL) return -1;
    var->value = (char *)action->param;
    var->value_len = strlen(var->value);
    expand_macros(msr, var, rule, mptmp);

    rule->actionset->intercept_uri = apr_pstrmemdup(msr->mp, var->value, var->value_len);

    return 1;
}

/* proxy */

static char *msre_action_proxy_validate(msre_engine *engine, msre_action *action) {
    /* ENH Add validation. */
    return NULL;
}

static apr_status_t msre_action_proxy_init(msre_engine *engine, msre_actionset *actionset,
        msre_action *action)
{
    actionset->intercept_action = ACTION_PROXY;
    actionset->intercept_uri = action->param;
    actionset->intercept_action_rec = action;
    return 1;
}

static apr_status_t msre_action_proxy_execute(modsec_rec *msr, apr_pool_t *mptmp,
        msre_rule *rule, msre_action *action)
{
    msc_string *var = NULL;

    var = apr_pcalloc(mptmp, sizeof(msc_string));
    if (var == NULL) return -1;
    var->value = (char *)action->param;
    var->value_len = strlen(var->value);
    expand_macros(msr, var, rule, mptmp);

    rule->actionset->intercept_uri = apr_pstrmemdup(msr->mp, var->value, var->value_len);

    return 1;
}

/* pass */

static apr_status_t msre_action_pass_init(msre_engine *engine, msre_actionset *actionset,
        msre_action *action)
{
    actionset->intercept_action = ACTION_NONE;
    actionset->intercept_action_rec = action;
    return 1;
}

/* skip */

static char *msre_action_skip_validate(msre_engine *engine, msre_action *action) {
    /* ENH Add validation. */
    return NULL;
}

static apr_status_t msre_action_skip_init(msre_engine *engine, msre_actionset *actionset,
        msre_action *action)
{
    actionset->skip_count = atoi(action->param);
    if (actionset->skip_count <= 0) actionset->skip_count = 1;
    return 1;
}

/* skipAfter */

static char *msre_action_skipAfter_validate(msre_engine *engine, msre_action *action) {
    /* ENH Add validation. */
    return NULL;
}

static apr_status_t msre_action_skipAfter_init(msre_engine *engine, msre_actionset *actionset,
        msre_action *action)
{
    actionset->skip_after = action->param;
    return 1;
}

/* allow */

static apr_status_t msre_action_allow_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->intercept_action = ACTION_ALLOW;
    actionset->intercept_action_rec = action;

    if (action->param != NULL) {
        if (strcasecmp(action->param, "phase") == 0) {
            actionset->intercept_action = ACTION_ALLOW_PHASE;
        } else
        if (strcasecmp(action->param, "request") == 0) {
            actionset->intercept_action = ACTION_ALLOW_REQUEST;
        }
    }

    return 1;
}

static char *msre_action_allow_validate(msre_engine *engine, msre_action *action) {
    if (action->param != NULL) {
        if (strcasecmp(action->param, "phase") == 0) {
            return NULL;
        } else
        if (strcasecmp(action->param, "request") == 0) {
            return NULL;
        } else {
            return apr_psprintf(engine->mp, "Invalid parameter for allow: %s", action->param);
        }
    }

    return NULL;
}

/* phase */

static char *msre_action_phase_validate(msre_engine *engine, msre_action *action) {
    /* ENH Add validation. */
    return NULL;
}

static apr_status_t msre_action_phase_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    if(strcasecmp(action->param,"request") == 0)
        actionset->phase = 2;
    else if(strcasecmp(action->param,"response") == 0)
        actionset->phase = 4;
    else if(strcasecmp(action->param,"logging") == 0)
        actionset->phase = 5;
    else
        actionset->phase = atoi(action->param);

    return 1;
}

/* t */

static char *msre_action_t_validate(msre_engine *engine, msre_action *action) {
    msre_tfn_metadata *metadata = NULL;
    metadata = msre_engine_tfn_resolve(engine, action->param);
    if (metadata == NULL) return apr_psprintf(engine->mp, "Invalid transformation function: %s",
        action->param);
    action->param_data = metadata;
    return NULL;
}

static apr_status_t msre_action_t_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    msre_tfn_metadata *metadata = (msre_tfn_metadata *)action->param_data;
    action->param_data = metadata;
    return 1;
}

/* ctl */
static char *msre_action_ctl_validate(msre_engine *engine, msre_action *action) {
    char *name = NULL;
    char *value = NULL;

    /* Parse first. */
    if (parse_name_eq_value(engine->mp, action->param, &name, &value) < 0) {
        return FATAL_ERROR;
    }
    if (value == NULL) {
        return apr_psprintf(engine->mp, "Missing ctl value for name: %s", name);
    }

    /* Validate value. */
    if (strcasecmp(name, "ruleEngine") == 0) {
        if (strcasecmp(value, "on") == 0) return NULL;
        if (strcasecmp(value, "off") == 0) return NULL;
        if (strcasecmp(value, "detectiononly") == 0) return NULL;
        return apr_psprintf(engine->mp, "Invalid setting for ctl name ruleEngine: %s", value);
    } else
    if (strcasecmp(name, "ruleRemoveById") == 0) {
        /* ENH nothing yet */
        return NULL;
    } else
    if (strcasecmp(name, "ruleRemoveByTag") == 0) {
        if (!msc_pregcomp(engine->mp, value, 0, NULL, NULL))
           return apr_psprintf(engine->mp, "ModSecurity: Invalid regular expression \"%s\"", value);
        return NULL;
    } else
    if (strcasecmp(name, "ruleRemoveByMsg") == 0) {
       if (!msc_pregcomp(engine->mp, value, 0, NULL, NULL))
           return apr_psprintf(engine->mp, "ModSecurity: Invalid regular expression \"%s\"", value);
        return NULL;
    } else
    if (strcasecmp(name, "requestBodyAccess") == 0) {
        if (parse_boolean(value) == -1) {
            return apr_psprintf(engine->mp, "Invalid setting for ctl name "
                " requestBodyAccess: %s", value);
        }
        return NULL;
    } else
    if (strcasecmp(name, "requestBodyProcessor") == 0) {
        /* ENH We will accept anything for now but it'd be nice
         * to add a check here that the processor name is a valid one.
         */
        return NULL;
    } else
    if (strcasecmp(name, "forceRequestBodyVariable") == 0) {
        if (strcasecmp(value, "on") == 0) return NULL;
        if (strcasecmp(value, "off") == 0) return NULL;
        return apr_psprintf(engine->mp, "Invalid setting for ctl name "
            " forceRequestBodyVariable: %s", value);
    } else
    if (strcasecmp(name, "responseBodyAccess") == 0) {
        if (parse_boolean(value) == -1) {
            return apr_psprintf(engine->mp, "Invalid setting for ctl name "
                " responseBodyAccess: %s", value);
        }
        return NULL;
    } else
    if (strcasecmp(name, "auditEngine") == 0) {
        if (strcasecmp(value, "on") == 0) return NULL;
        if (strcasecmp(value, "off") == 0) return NULL;
        if (strcasecmp(value, "relevantonly") == 0) return NULL;
        return apr_psprintf(engine->mp, "Invalid setting for ctl name "
            " auditEngine: %s", value);
    } else
    if (strcasecmp(name, "auditLogParts") == 0) {
        if ((value[0] == '+')||(value[0] == '-')) {
            if (is_valid_parts_specification(value + 1) != 1) {
            return apr_psprintf(engine->mp, "Invalid setting for ctl name "
                "auditLogParts: %s", value);
            }
        }
        else
        if (is_valid_parts_specification(value) != 1) {
            return apr_psprintf(engine->mp, "Invalid setting for ctl name "
                "auditLogParts: %s", value);
        }
        return NULL;
    } else
    if (strcasecmp(name, "debugLogLevel") == 0) {
        if ((atoi(value) >= 0)&&(atoi(value) <= 9)) return NULL;
        return apr_psprintf(engine->mp, "Invalid setting for ctl name "
            "debugLogLevel: %s", value);
    } else
    if (strcasecmp(name, "requestBodyLimit") == 0) {
        long int limit = strtol(value, NULL, 10);

        if ((limit == LONG_MAX)||(limit == LONG_MIN)||(limit <= 0)) {
            return apr_psprintf(engine->mp, "Invalid setting for ctl name "
                "requestBodyLimit: %s", value);
        }

        if (limit > REQUEST_BODY_HARD_LIMIT) {
            return apr_psprintf(engine->mp, "Request size limit cannot exceed "
                "the hard limit: %ld", RESPONSE_BODY_HARD_LIMIT);
        }

        return NULL;
    } else
    if (strcasecmp(name, "responseBodyLimit") == 0) {
        long int limit = strtol(value, NULL, 10);

        if ((limit == LONG_MAX)||(limit == LONG_MIN)||(limit <= 0)) {
            return apr_psprintf(engine->mp, "Invalid setting for ctl name "
                "responseBodyLimit: %s", value);
        }

        if (limit > RESPONSE_BODY_HARD_LIMIT) {
            return apr_psprintf(engine->mp, "Response size limit cannot exceed "
                "the hard limit: %ld", RESPONSE_BODY_HARD_LIMIT);
        }

        return NULL;
    } else
        if  (strcasecmp(name, "ruleRemoveTargetById") == 0) {
                char *parm = NULL;
                char *savedptr = NULL;

                parm = apr_strtok(value,";",&savedptr);

                if(parm == NULL && savedptr == NULL)
                    return apr_psprintf(engine->mp, "ruleRemoveTargetById must has at least id;VARIABLE");

        return NULL;
    } else
        if (strcasecmp(name,"ruleRemoveTargetByTag") == 0) {
                char *parm = NULL;
                char *savedptr = NULL;

                parm = apr_strtok(value,";",&savedptr);
                if(parm == NULL && savedptr == NULL)
                    return apr_psprintf(engine->mp, "ruleRemoveTargetByTag must has at least tag;VARIABLE");
            if (!msc_pregcomp(engine->mp, parm, 0, NULL, NULL)) {
                return apr_psprintf(engine->mp, "ModSecurity: Invalid regular expression \"%s\"", parm);
            }
        return NULL;
    } else
        if (strcasecmp(name,"ruleRemoveTargetByMsg") == 0) {
                char *parm = NULL;
                char *savedptr = NULL;

                parm = apr_strtok(value,";",&savedptr);
                if(parm == NULL && savedptr == NULL)
                    return apr_psprintf(engine->mp, "ruleRemoveTargetByMsg must has at least msg;VARIABLE");
            if (!msc_pregcomp(engine->mp, parm, 0, NULL, NULL)) {
                return apr_psprintf(engine->mp, "ModSecurity: Invalid regular expression \"%s\"", parm);
            }
        return NULL;
     } else
        if (strcasecmp(name, "HashEnforcement") == 0) {
        if (strcasecmp(value, "on") == 0) return NULL;
        if (strcasecmp(value, "off") == 0) return NULL;
        return apr_psprintf(engine->mp, "Invalid setting for ctl name HashEnforcement: %s", value);
     } else
        if (strcasecmp(name, "HashEngine") == 0) {
        if (strcasecmp(value, "on") == 0) return NULL;
        if (strcasecmp(value, "off") == 0) return NULL;
        return apr_psprintf(engine->mp, "Invalid setting for ctl name HashEngine: %s", value);
     } else {
            return apr_psprintf(engine->mp, "Invalid ctl name setting: %s", name);
     }
}

static apr_status_t msre_action_ctl_init(msre_engine *engine, msre_actionset *actionset,
        msre_action *action)
{
    /* Do nothing. */
    return 1;
}

static apr_status_t msre_action_ctl_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    char *name = NULL;
    char *value = NULL;

    /* Parse first. */
    if (parse_name_eq_value(msr->mp, action->param, &name, &value) < 0) return -1;
    if (value == NULL) return -1;

    /* Validate value. */
    if (strcasecmp(name, "ruleEngine") == 0) {
        if (strcasecmp(value, "on") == 0) {
            msr->txcfg->is_enabled = MODSEC_ENABLED;
            msr->usercfg->is_enabled = MODSEC_ENABLED;
        }
        else
        if (strcasecmp(value, "off") == 0) {
            msr->txcfg->is_enabled = MODSEC_DISABLED;
            msr->usercfg->is_enabled = MODSEC_DISABLED;
        }
        else
        if (strcasecmp(value, "detectiononly") == 0) {
            msr->txcfg->is_enabled = MODSEC_DETECTION_ONLY;
            msr->usercfg->is_enabled = MODSEC_DETECTION_ONLY;
            msr->txcfg->if_limit_action = REQUEST_BODY_LIMIT_ACTION_PARTIAL;
            msr->usercfg->if_limit_action = REQUEST_BODY_LIMIT_ACTION_PARTIAL;
            msr->txcfg->of_limit_action = REQUEST_BODY_LIMIT_ACTION_PARTIAL;
            msr->usercfg->of_limit_action = REQUEST_BODY_LIMIT_ACTION_PARTIAL;
        }

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Set ruleEngine to %s.", value);
        }

        return 1;
    } else
    if (strcasecmp(name, "HashEnforcement") == 0) {
        if (strcasecmp(value, "on") == 0) {
            msr->txcfg->hash_enforcement = HASH_ENABLED;
            msr->usercfg->hash_enforcement = HASH_ENABLED;
        }
        if (strcasecmp(value, "off") == 0) {
            msr->txcfg->hash_enforcement = HASH_DISABLED;
            msr->usercfg->hash_enforcement = HASH_DISABLED;
        }
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Set HashEnforcement to %s.", value);
        }
        return 1;
    } else
    if (strcasecmp(name, "HashEngine") == 0) {
        if (strcasecmp(value, "on") == 0) {
            msr->txcfg->hash_is_enabled = HASH_ENABLED;
            msr->usercfg->hash_is_enabled = HASH_ENABLED;
        }
        if (strcasecmp(value, "off") == 0) {
            msr->txcfg->hash_is_enabled = HASH_DISABLED;
            msr->usercfg->hash_is_enabled = HASH_DISABLED;
        }
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Set HashEngine to %s.", value);
        }
        return 1;
    } else
    if (strcasecmp(name, "ruleRemoveById") == 0) {
        *(const char **)apr_array_push(msr->removed_rules) = (const char *)apr_pstrdup(msr->mp, value);

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Removed rule by id : %s.", value);
        }
        return 1;
    } else
    if (strcasecmp(name, "ruleRemoveByTag") == 0) {
        rule_exception *re = apr_pcalloc(msr->mp, sizeof(rule_exception));
        re->type = RULE_EXCEPTION_REMOVE_TAG;
        re->param = (const char *)apr_pstrdup(msr->mp, value);
        re->param_data = msc_pregcomp(msr->mp, re->param, 0, NULL, NULL);
        if (re->param_data == NULL) {
            msr_log(msr, 1, "ModSecurity: Invalid regular expression \"%s\"", re->param);
            return -1;
        }

        *(rule_exception **)apr_array_push(msr->removed_rules_tag) = re;

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Removed rule by tag : %s.", value);
        }

        return 1;
    } else
    if (strcasecmp(name, "ruleRemoveByMsg") == 0) {
        rule_exception *re = apr_pcalloc(msr->mp, sizeof(rule_exception));
        re->type = RULE_EXCEPTION_REMOVE_MSG;
        re->param = (const char *)apr_pstrdup(msr->mp, value);
        re->param_data = msc_pregcomp(msr->mp, re->param, 0, NULL, NULL);
        if (re->param_data == NULL) {
            msr_log(msr, 1, "ModSecurity: Invalid regular expression \"%s\"", re->param);
            return -1;
        }

        *(rule_exception **)apr_array_push(msr->removed_rules_msg) = re;

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Removed rule by msg : %s.", value);
        }

        return 1;
        } else
    if (strcasecmp(name, "requestBodyAccess") == 0) {
        int pv = parse_boolean(value);

        if (pv == -1) return -1;
        msr->txcfg->reqbody_access = pv;
        msr->usercfg->reqbody_access = pv;

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Set requestBodyAccess to %d.", pv);
        }

        return 1;
    } else
    if (strcasecmp(name, "forceRequestBodyVariable") == 0) {
        if (strcasecmp(value, "on") == 0) {
            msr->txcfg->reqbody_buffering = REQUEST_BODY_FORCEBUF_ON;
            msr->usercfg->reqbody_buffering = REQUEST_BODY_FORCEBUF_ON;
        }
        else
        if (strcasecmp(value, "off") == 0) {
            msr->txcfg->reqbody_buffering = REQUEST_BODY_FORCEBUF_OFF;
            msr->usercfg->reqbody_buffering = REQUEST_BODY_FORCEBUF_OFF;
        }

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Set requestBodyAccess to %d.", msr->txcfg->reqbody_buffering);
        }

        return 1;
    } else
    if (strcasecmp(name, "requestBodyProcessor") == 0) {
        msr->msc_reqbody_processor = value;

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Set requestBodyProcessor to %s.", value);
        }

        return 1;
    } else
    if (strcasecmp(name, "responseBodyAccess") == 0) {
        int pv = parse_boolean(value);

        if (pv == -1) return -1;
        msr->txcfg->resbody_access = pv;
        msr->usercfg->resbody_access = pv;

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Set responseBodyAccess to %d.", pv);
        }

        return 1;
    } else
    if (strcasecmp(name, "auditEngine") == 0) {
        if (strcasecmp(value, "on") == 0) {
            msr->txcfg->auditlog_flag = AUDITLOG_ON;
            msr->usercfg->auditlog_flag = AUDITLOG_ON;
        }
        else
        if (strcasecmp(value, "off") == 0) {
            msr->txcfg->auditlog_flag = AUDITLOG_OFF;
            msr->usercfg->auditlog_flag = AUDITLOG_OFF;
        }
        else
        if (strcasecmp(value, "relevantonly") == 0) {
            msr->txcfg->auditlog_flag = AUDITLOG_RELEVANT;
            msr->usercfg->auditlog_flag = AUDITLOG_RELEVANT;
        }

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Set auditEngine to %d.", msr->txcfg->auditlog_flag);
        }

        return 1;
    } else
    if (strcasecmp(name, "auditLogParts") == 0) {
        char *new_value = value;

        if (value[0] == '+') {
            /* Add the listed parts. */
            new_value = apr_pstrcat(msr->mp, msr->txcfg->auditlog_parts, value + 1, NULL);
        }
        else
        if (value[0] == '-') { /* Remove the listed parts. */
            char c, *t = value + 1;

            /* Start with the current value. */
            new_value = apr_pstrdup(msr->mp, msr->txcfg->auditlog_parts);

            while((c = *t++) != '\0') {
                char *s = new_value;
                char *d = new_value;

                while(*s != '\0') {
                    if (*s != c) {
                        *(d++) = *(s++);
                    } else {
                        s++;
                    }
                }
                *d = '\0';
            }
        }

        /* Set the new value. */
        msr->txcfg->auditlog_parts = new_value;
        msr->usercfg->auditlog_parts = new_value;

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Set auditLogParts to %s.", msr->txcfg->auditlog_parts);
        }

        return 1;
    } else
    if (strcasecmp(name, "debugLogLevel") == 0) {
        msr->txcfg->debuglog_level = atoi(value);
        msr->usercfg->debuglog_level = atoi(value);

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Set debugLogLevel to %d.", msr->txcfg->debuglog_level);
        }

        return 1;
    } else
    if (strcasecmp(name, "requestBodyLimit") == 0) {
        long int limit = strtol(value, NULL, 10);

        /* ENH Accept only in correct phase warn otherwise. */
        msr->txcfg->reqbody_limit = limit;
        msr->usercfg->reqbody_limit = limit;

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Set requestBodyLimit to %ld.", limit);
        }

        return 1;
    } else
    if (strcasecmp(name, "responseBodyLimit") == 0) {
        long int limit = strtol(value, NULL, 10);

        /* ENH Accept only in correct phase warn otherwise. */
        msr->txcfg->of_limit = limit;
        msr->usercfg->of_limit = limit;

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: Set responseBodyLimit to %ld.", limit);
        }

        return 1;
    } else
    if (strcasecmp(name, "ruleRemoveTargetById") == 0)  {
        rule_exception *re = NULL;
        char *p1 = NULL, *p2 = NULL;
        char *savedptr = NULL;

        p1 = apr_strtok(value,";",&savedptr);

        p2 = apr_strtok(NULL,";",&savedptr);

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: ruleRemoveTargetById id=%s targets=%s", p1, p2);
        }
    re = apr_pcalloc(msr->mp, sizeof(rule_exception));
    re->type = RULE_EXCEPTION_REMOVE_ID;
    re->param = (const char *)apr_pstrdup(msr->mp, p1);
    apr_table_addn(msr->removed_targets, apr_pstrdup(msr->mp, p2), (void *)re);
    return 1;
    } else
    if (strcasecmp(name, "ruleRemoveTargetByTag") == 0)  {
        rule_exception *re = NULL;
        char *p1 = NULL, *p2 = NULL;
        char *savedptr = NULL;

        p1 = apr_strtok(value,";",&savedptr);

        p2 = apr_strtok(NULL,";",&savedptr);

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: ruleRemoveTargetByTag tag=%s targets=%s", p1, p2);
        }

    re = apr_pcalloc(msr->mp, sizeof(rule_exception));
    re->type = RULE_EXCEPTION_REMOVE_TAG;
    re->param = (const char *)apr_pstrdup(msr->mp, p1);
    re->param_data = msc_pregcomp(msr->mp, p1, 0, NULL, NULL);
    if (re->param_data == NULL) {
        msr_log(msr, 1, "ModSecurity: Invalid regular expression \"%s\"", p1);
        return -1;
    }
    apr_table_addn(msr->removed_targets, apr_pstrdup(msr->mp, p2), (void *)re);
    return 1;
    } else
    if (strcasecmp(name, "ruleRemoveTargetByMsg") == 0)  {
        rule_exception *re = NULL;
        char *p1 = NULL, *p2 = NULL;
        char *savedptr = NULL;

        p1 = apr_strtok(value,";",&savedptr);

        p2 = apr_strtok(NULL,";",&savedptr);

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Ctl: ruleRemoveTargetByMsg msg=%s targets=%s", p1, p2);
        }

    re = apr_pcalloc(msr->mp, sizeof(rule_exception));
    re->type = RULE_EXCEPTION_REMOVE_MSG;
    re->param = apr_pstrdup(msr->mp, p1);
    re->param_data = msc_pregcomp(msr->mp, p1, 0, NULL, NULL);
    if (re->param_data == NULL) {
        msr_log(msr, 1, "ModSecurity: Invalid regular expression \"%s\"", p1);
        return -1;
    }
    apr_table_addn(msr->removed_targets, apr_pstrdup(msr->mp, p2), (void *)re);
    return 1;
    }
    else {
        /* Should never happen, but log if it does. */
        msr_log(msr, 1, "Internal Error: Unknown ctl action \"%s\".", name);
        return -1;
    }

    return -1;
}

/* xmlns */
static char *msre_action_xmlns_validate(msre_engine *engine, msre_action *action) {
    char *name = NULL;
    char *value = NULL;

    /* Parse first. */
    if (parse_name_eq_value(engine->mp, action->param, &name, &value) < 0) {
        return FATAL_ERROR;
    }
    if (value == NULL) {
        return apr_psprintf(engine->mp, "Missing xmlns href for prefix: %s", name);
    }

    /* Don't do anything else right now, we are just storing
     * the value for the variable, which is the real consumer
     * for the namespace information.
     */

    return NULL;
}

/* sanitizeArg */
static apr_status_t msre_action_sanitizeArg_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    const char *sargname = NULL;
    const apr_array_header_t *tarr;
    const apr_table_entry_t *telts;
    int i;

    sargname = action->param;

    tarr = apr_table_elts(msr->arguments);
    telts = (const apr_table_entry_t*)tarr->elts;
    for (i = 0; i < tarr->nelts; i++) {
        msc_arg *arg = (msc_arg *)telts[i].val;

        if (strcasecmp(sargname, arg->name) == 0) {
            apr_table_addn(msr->arguments_to_sanitize, arg->name, (void *)arg);
        }
    }

    return 1;
}

#define SANITISE_ARG                1
#define SANITISE_REQUEST_HEADER     2
#define SANITISE_RESPONSE_HEADER    3

/* sanitizeMatched */
static apr_status_t msre_action_sanitizeMatched_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    const char *sargname = NULL;
    const apr_array_header_t *tarr;
    const apr_table_entry_t *telts;
    int i, type = 0;
    msc_string *mvar = msr->matched_var;

    if (mvar->name_len == 0) return 0;

    /* IMP1 We need to extract the variable name properly here,
     *      taking into account it may have been escaped.
     */
    if ((mvar->name_len > 5) && (strncmp(mvar->name, "ARGS:", 5) == 0)) {
        sargname = apr_pstrdup(msr->mp, mvar->name + 5);
        type = SANITISE_ARG;
    } else
    if ((mvar->name_len > 11) && (strncmp(mvar->name, "ARGS_NAMES:", 11) == 0)) {
        sargname = apr_pstrdup(msr->mp, mvar->name + 11);
        type = SANITISE_ARG;
    } else
    if ((mvar->name_len > 16) && (strncmp(mvar->name, "REQUEST_HEADERS:", 16) == 0)) {
        sargname = apr_pstrdup(msr->mp, mvar->name + 16);
        type = SANITISE_REQUEST_HEADER;
    } else
    if ((mvar->name_len > 22) && (strncmp(mvar->name, "REQUEST_HEADERS_NAMES:", 22) == 0)) {
        sargname = apr_pstrdup(msr->mp, mvar->name + 22);
        type = SANITISE_REQUEST_HEADER;
    } else
    if ((mvar->name_len > 17) && (strncmp(mvar->name, "RESPONSE_HEADERS:", 17) == 0)) {
        sargname = apr_pstrdup(msr->mp, mvar->name + 17);
        type = SANITISE_RESPONSE_HEADER;
    } else
    if ((mvar->name_len > 23) && (strncmp(mvar->name, "RESPONSE_HEADERS_NAMES:", 23) == 0)) {
        sargname = apr_pstrdup(msr->mp, mvar->name + 23);
        type = SANITISE_RESPONSE_HEADER;
    }
    else {
        if (msr->txcfg->debuglog_level >= 3) {
            msr_log(msr, 3, "sanitizeMatched: Don't know how to handle variable: %s",
                mvar->name);
        }
        return 0;
    }

    switch(type) {
        case SANITISE_ARG :
            tarr = apr_table_elts(msr->arguments);
            telts = (const apr_table_entry_t*)tarr->elts;
            for (i = 0; i < tarr->nelts; i++) {
                msc_arg *arg = (msc_arg *)telts[i].val;
                if (strcasecmp(sargname, arg->name) == 0) {
                    apr_table_addn(msr->arguments_to_sanitize, arg->name, (void *)arg);
                }
            }
            break;

        case SANITISE_REQUEST_HEADER :
            apr_table_set(msr->request_headers_to_sanitize, sargname, "1");
            break;

        case SANITISE_RESPONSE_HEADER :
            apr_table_set(msr->response_headers_to_sanitize, sargname, "1");
            break;

        default :
            /* do nothing */
            break;
    }

    return 1;
}

/* sanitizeRequestHeader */
static apr_status_t msre_action_sanitizeRequestHeader_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    apr_table_set(msr->request_headers_to_sanitize, action->param, "1");
    return 1;
}

/* sanitizeResponseHeader */
static apr_status_t msre_action_sanitizeResponseHeader_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    apr_table_set(msr->response_headers_to_sanitize, action->param, "1");
    return 1;
}

/* setenv */
static apr_status_t msre_action_setenv_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    char *data = apr_pstrdup(mptmp, action->param);
    char *env_name = NULL, *env_value = NULL;
    char *s = NULL;
    msc_string *env = NULL;

    /* Extract the name and the value. */
    /* IMP1 We have a function for this now, parse_name_eq_value? */
    s = strstr(data, "=");
    if (s == NULL) {
        env_name = data;
        env_value = "1";
    } else {
        env_name = data;
        env_value = s + 1;
        *s = '\0';
    }

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "Setting env variable: %s=%s", env_name, env_value);
    }

    /* Expand and escape any macros in the name */
    env = apr_palloc(msr->mp, sizeof(msc_string));
    if (env == NULL) {
        msr_log(msr, 1, "Failed to allocate space to expand name macros");
        return -1;
    }
    env->value = env_name;
    env->value_len = strlen(env->value);
    expand_macros(msr, env, rule, mptmp);
    env_name = log_escape_nq_ex(msr->mp, env->value, env->value_len);

    /* Execute the requested action. */
    if (env_name[0] == '!') {
        /* Delete */
        apr_table_unset(msr->r->subprocess_env, env_name + 1);

        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "Unset env variable \"%s\".", env_name);
        }
    } else {
        /* Set */
        char * val_value = NULL;
        msc_string *val = apr_palloc(msr->mp, sizeof(msc_string));
        if (val == NULL) {
            msr_log(msr, 1, "Failed to allocate space to expand value macros");
            return -1;
        }

        /* Expand values in value */
        val->value = env_value;
        val->value_len = strlen(val->value);
        expand_macros(msr, val, rule, mptmp);

        /* To be safe, we escape NULs as it goes in subprocess_env. */
        val_value = log_escape_nul(msr->mp, (const unsigned char *)val->value, val->value_len);

        apr_table_set(msr->r->subprocess_env, env_name, val_value);

        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "Set env variable \"%s\" to: %s",
                env_name,
                log_escape_nq(mptmp, val_value));
        }
    }

    return 1;
}

/* setvar */
apr_status_t msre_action_setvar_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, char *var_name, char *var_value)
{
    char *col_name = NULL;
    char *s = NULL;
    apr_table_t *target_col = NULL;
    int is_negated = 0;
    msc_string *var = NULL;

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "Setting variable: %s=%s", var_name, var_value);
    }

    /* Expand and escape any macros in the name */
    var = apr_palloc(msr->mp, sizeof(msc_string));
    if (var == NULL) {
        msr_log(msr, 1, "Failed to allocate space to expand name macros");
        return -1;
    }
    var->value = var_name;
    var->value_len = strlen(var->value);
    expand_macros(msr, var, rule, mptmp);
    var_name = log_escape_nq_ex(msr->mp, var->value, var->value_len);

    /* Handle the exclamation mark. */
    if (var_name[0] == '!') {
        var_name = var_name + 1;
        is_negated = 1;
    }

    /* ENH Not possible to use ! and = at the same time. */
    /* ENH Not possible to change variable "KEY".        */

    /* Figure out the collection name. */
    target_col = msr->tx_vars;
    s = strstr(var_name, ".");
    if (s == NULL) {
        if (msr->txcfg->debuglog_level >= 3) {
            msr_log(msr, 3, "Asked to set variable \"%s\", but no collection name specified. ",
                log_escape(msr->mp, var_name));
        }

        return 0;
    }

    col_name = var_name;
    var_name = s + 1;
    *s = '\0';

    /* Locate the collection. */
    if (strcasecmp(col_name, "tx") == 0) { /* Special case for TX variables. */
        target_col = msr->tx_vars;
    } else {
        target_col = (apr_table_t *)apr_table_get(msr->collections, col_name);
        if (target_col == NULL) {
            if (msr->txcfg->debuglog_level >= 3) {
                msr_log(msr, 3, "Could not set variable \"%s.%s\" as the collection does not exist.",
                    log_escape(msr->mp, col_name), log_escape(msr->mp, var_name));
            }

            return 0;
        }
    }

    if (is_negated) {
        /* Unset variable. */

        /* ENH Refuse to remove certain variables, e.g. TIMEOUT, internal variables, etc... */

        apr_table_unset(target_col, var_name);

        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "Unset variable \"%s.%s\".", col_name, var_name);
        }
    }
    else {
        /* Set or change variable. */

        if ((var_value[0] == '+')||(var_value[0] == '-')) {
            /* Relative change. */
            msc_string *rec = NULL;
            msc_string *val = apr_palloc(msr->mp, sizeof(msc_string));
            int value = 0;

            if (val == NULL) {
                msr_log(msr, 1, "Failed to allocate space to expand value macros");
                return -1;
            }

            /* Retrieve  variable or generate (if it does not exist). */
            rec = (msc_string *)apr_table_get(target_col, var_name);
            if (rec == NULL) {
                rec = var; /* use the already allocated space for var */
                rec->name = apr_pstrdup(msr->mp, var_name);
                rec->name_len = strlen(rec->name);
                value = 0;
                rec->value = apr_psprintf(msr->mp, "%d", value);
                rec->value_len = strlen(rec->value);
            }
            else {
                value = atoi(rec->value);
            }

            /* Record the original value before we change it */
            collection_original_setvar(msr, col_name, rec);

            /* Expand values in value */
            val->value = var_value;
            val->value_len = strlen(val->value);
            expand_macros(msr, val, rule, mptmp);
            var_value = val->value;

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Relative change: %s=%d%s", var_name, value, var_value);
            }

            /* Change value. */
            value += atoi(var_value);
            if (value < 0) value = 0; /* Counters never go below zero. */

            /* Put the variable back. */
            rec->value = apr_psprintf(msr->mp, "%d", value);
            rec->value_len = strlen(rec->value);
            apr_table_setn(target_col, rec->name, (void *)rec);

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Set variable \"%s.%s\" to \"%s\".",
                    col_name, rec->name,
                    log_escape_ex(mptmp, rec->value, rec->value_len));
            }
        }
        else {
            /* Absolute change. */

            var->name = apr_pstrdup(msr->mp, var_name);
            var->name_len = strlen(var->name);
            var->value = apr_pstrdup(msr->mp, var_value);
            var->value_len = strlen(var->value);
            expand_macros(msr, var, rule, mptmp);
            apr_table_setn(target_col, var->name, (void *)var);

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Set variable \"%s.%s\" to \"%s\".",
                    log_escape(mptmp, col_name),
                    log_escape_ex(mptmp, var->name, var->name_len),
                    log_escape_ex(mptmp, var->value, var->value_len));
            }
        }
    }

    /* Make note of the change so that we know later
     * we need to persist the collection.
     */
    apr_table_set(msr->collections_dirty, col_name, "1");

    return 1;
}

/*
* \brief Parse fuction for setvar input
*
* \param msr Pointer to the engine
* \param mptmp Pointer to the pool
* \param rule Pointer to rule struct
* \param action input data
*
* \retval -1 On failure
* \retval 0 On Collection failure
* \retval 1 On Success
*/
static apr_status_t msre_action_setvar_parse(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    char *data = apr_pstrdup(mptmp, action->param);
    char *var_name = NULL, *var_value = NULL;
    char *s = NULL;

    /* Extract the name and the value. */
    /* IMP1 We have a function for this now, parse_name_eq_value? */
    s = strstr(data, "=");
    if (s == NULL) {
        var_name = data;
        var_value = "1";
    } else {
        var_name = data;
        var_value = s + 1;
        *s = '\0';

        while ((*var_value != '\0')&&(isspace(*var_value))) var_value++;
    }

    return msre_action_setvar_execute(msr,mptmp,rule,var_name,var_value);
}

/* expirevar */
static apr_status_t msre_action_expirevar_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    char *data = apr_pstrdup(mptmp, action->param);
    char *col_name = NULL, *var_name = NULL, *var_value = NULL;
    char *s = NULL;
    apr_table_t *target_col = NULL;
    msc_string *var = NULL;

    /* Extract the name and the value. */
    /* IMP1 We have a function for this now, parse_name_eq_value? */
    s = strstr(data, "=");
    if (s == NULL) {
        var_name = data;
        var_value = "1";
    } else {
        var_name = data;
        var_value = s + 1;
        *s = '\0';
    }

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "Expiring variable: %s=%s", var_name, var_value);
    }

    /* Expand and escape any macros in the name */
    var = apr_palloc(msr->mp, sizeof(msc_string));
    if (var == NULL) {
        msr_log(msr, 1, "Failed to allocate space to expand name macros");
        return -1;
    }
    var->value = var_name;
    var->value_len = strlen(var->value);
    expand_macros(msr, var, rule, mptmp);
    var_name = log_escape_nq_ex(msr->mp, var->value, var->value_len);

    /* Choose the collection to work with. */
    s = strstr(var_name, ".");
    if (s != NULL) {
        col_name = var_name;
        var_name = s + 1;
        *s = '\0';

        /* IMP1 No need to handle TX here because TX variables cannot expire,
         *      but we definitely need to have a better error message.
         */

        target_col = (apr_table_t *)apr_table_get(msr->collections, col_name);
        if (target_col == NULL) {
            if (msr->txcfg->debuglog_level >= 3) {
                msr_log(msr, 3, "Could not expire variable \"%s.%s\" as the collection does not exist.",
                    log_escape(msr->mp, col_name), log_escape(msr->mp, var_name));
            }
            
            return 0;
        }
    } else {
        if (msr->txcfg->debuglog_level >= 3) {
            msr_log(msr, 3, "Asked to expire variable \"%s\", but no collection name specified. ",
                log_escape(msr->mp, var_name));
        }
        
        return 0;
    }

    /* To expire a variable we just place a special variable into
     * the collection. Expiry actually happens when the collection
     * is retrieved from storage the next time.
     */
    var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    var->name = apr_psprintf(msr->mp, "__expire_%s", var_name);
    var->name_len = strlen(var->name);

    /* Expand macros in value */
    var->value = var_value;
    var->value_len = strlen(var->value);
    expand_macros(msr, var, rule, msr->mp);
    var_value = var->value;

    /* Calculate with the expanded value */
    var->value = apr_psprintf(msr->mp, "%" APR_TIME_T_FMT, (apr_time_t)(apr_time_sec(msr->request_time)
        + atoi(var_value)));
    var->value_len = strlen(var->value);

    apr_table_setn(target_col, var->name, (void *)var);

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Variable \"%s.%s\" set to expire in %s seconds.", col_name,
           var_name, var_value);
    }

    apr_table_set(msr->collections_dirty, col_name, "1");

    return 1;
}

/* deprecatevar */
static apr_status_t msre_action_deprecatevar_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    char *data = apr_pstrdup(mptmp, action->param);
    char *col_name = NULL, *var_name = NULL, *var_value = NULL;
    char *s = NULL;
    apr_table_t *target_col = NULL;
    msc_string *var = NULL, *var_last_update_time = NULL;
    apr_time_t last_update_time, current_time;
    long current_value, new_value;

    /* Extract the name and the value. */
    /* IMP1 We have a function for this now, parse_name_eq_value? */
    s = strstr(data, "=");
    if (s == NULL) {
        var_name = data;
        var_value = "1";
    } else {
        var_name = data;
        var_value = s + 1;
        *s = '\0';
    }

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "Deprecating variable: %s=%s", var_name, var_value);
    }

    /* Expand and escape any macros in the name */
    var = apr_palloc(msr->mp, sizeof(msc_string));
    if (var == NULL) {
        msr_log(msr, 1, "Failed to allocate space to expand name macros");
        return -1;
    }
    var->value = var_name;
    var->value_len = strlen(var->value);
    expand_macros(msr, var, rule, mptmp);
    var_name = log_escape_nq_ex(msr->mp, var->value, var->value_len);

    /* Expand macros in value */
    var->value = var_value;
    var->value_len = strlen(var->value);
    expand_macros(msr, var, rule, msr->mp);
    var_value = var->value;

    /* Choose the collection to work with. */
    s = strstr(var_name, ".");
    if (s != NULL) {
        col_name = var_name;
        var_name = s + 1;
        *s = '\0';

        /* IMP1 Add message TX variables cannot deprecate in value. */

        target_col = (apr_table_t *)apr_table_get(msr->collections, col_name);
        if (target_col == NULL) {
            if (msr->txcfg->debuglog_level >= 3) {
                msr_log(msr, 3, "Could not deprecate variable \"%s.%s\" as the collection does "
                    "not exist.", log_escape(msr->mp, col_name), log_escape(msr->mp, var_name));
            }
            
            return 0;
        }
    } else {
        if (msr->txcfg->debuglog_level >= 3) {
            msr_log(msr, 3, "Asked to deprecate variable \"%s\", but no collection name specified. ",
                log_escape(msr->mp, var_name));
        }
        
        return 0;
    }

    /* Find the current value. */
    var = (msc_string *)apr_table_get(target_col, var_name);
    if (var == NULL) {
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "Asked to deprecate variable \"%s.%s\", but it does not exist.",
                log_escape(msr->mp, col_name), log_escape(msr->mp, var_name));
        }
        return 0;
    }
    current_value = atoi(var->value);

    /* Find the last update time (of the collection). */
    var_last_update_time = (msc_string *)apr_table_get(target_col, "LAST_UPDATE_TIME");
    if (var_last_update_time == NULL) {
        /* This is all right. If collection was created (and not restored from
         * storage) then it won't have LAST_UPDATE_TIME - it was never updated.
         */
        return 0;
    }

    current_time = apr_time_sec(apr_time_now());
    last_update_time = atoi(var_last_update_time->value);

    s = strstr(var_value, "/");
    if (s == NULL) {
        msr_log(msr, 3, "Incorrect format for the deprecatevar argument: \"%s\"",
            log_escape(msr->mp, var_value));
        return 0;
    }
    *s = '\0';
    s++;

    /* Deprecate the value using the given speed and the
     * time elapsed since the last update.
     */
    new_value = current_value -
        (atol(var_value) * ((current_time - last_update_time) / atol(s)));
    if (new_value < 0) new_value = 0;

    /* Only change the value if it differs. */
    if (new_value != current_value) {
        var->value = apr_psprintf(msr->mp, "%ld", new_value);
        var->value_len = strlen(var->value);

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Deprecated variable \"%s.%s\" from %ld to %ld (%" APR_TIME_T_FMT " seconds since "
                "last update).", log_escape(msr->mp, col_name), log_escape(msr->mp, var_name),
                current_value, new_value, (apr_time_t)(current_time - last_update_time));
        }

        apr_table_set(msr->collections_dirty, col_name, "1");
    } else {
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "Not deprecating variable \"%s.%s\" because the new value (%ld) is "
                "the same as the old one (%ld) (%" APR_TIME_T_FMT " seconds since last update).",
                log_escape(msr->mp, col_name), log_escape(msr->mp, var_name), current_value,
                new_value, (apr_time_t)(current_time - last_update_time));
        }
    }

    return 1;
}

static apr_status_t init_collection(modsec_rec *msr, const char *real_col_name,
    const char *col_name, const char *col_key, unsigned int col_key_len)
{
    apr_table_t *table = NULL;
    msc_string *var = NULL;

    /* IMP1 Cannot initialise the built-in collections this way. */

    /* Does the collection exist already? */
    if (apr_table_get(msr->collections, col_name) != NULL) {
        /* ENH Warn about this. */
        return 0;
    }

    /* Init collection from storage. */
    table = collection_retrieve(msr, real_col_name, col_key, col_key_len);

    if (table == NULL) {
        /* Does not exist yet - create new. */

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Creating collection (name \"%s\", key \"%s\").",
               real_col_name, col_key);
        }

        table = apr_table_make(msr->mp, 24);
        if (table == NULL) return -1;

        /* IMP1 Is the timeout hard-coded to 3600? */

       if(msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Setting default timeout collection value %d.",msr->txcfg->col_timeout);
        }

        /* Add default timeout. */
        var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
        var->name = "__expire_KEY";
        var->name_len = strlen(var->name);
        var->value = apr_psprintf(msr->mp, "%" APR_TIME_T_FMT, (apr_time_t)(apr_time_sec(msr->request_time) + msr->txcfg->col_timeout));
        var->value_len = strlen(var->value);
        apr_table_setn(table, var->name, (void *)var);

        /* Remember the key. */
        var = apr_pcalloc(msr->mp, sizeof(msc_string));
        var->name = "KEY";
        var->name_len = strlen(var->name);
        var->value = apr_pstrmemdup(msr->mp, col_key, col_key_len);
        var->value_len = col_key_len;
        apr_table_setn(table, var->name, (void *)var);

        /* The timeout. */
        var = apr_pcalloc(msr->mp, sizeof(msc_string));
        var->name = "TIMEOUT";
        var->name_len = strlen(var->name);
        var->value = apr_psprintf(msr->mp, "%d", msr->txcfg->col_timeout);
        var->value_len = strlen(var->value);
        apr_table_setn(table, var->name, (void *)var);

        /* We may want to allow the user to unset KEY
         * but we still need to preserve value to identify
         * the collection in storage.
         */

        /* IMP1 Actually I want a better way to delete collections,
         *      perhaps a dedicated action.
         */

        var = apr_pcalloc(msr->mp, sizeof(msc_string));
        var->name = "__key";
        var->name_len = strlen(var->name);
        var->value = apr_pstrmemdup(msr->mp, col_key, col_key_len);
        var->value_len = col_key_len;
        apr_table_setn(table, var->name, (void *)var);

        /* Peristence code will need to know the name of the collection. */
        var = apr_pcalloc(msr->mp, sizeof(msc_string));
        var->name = "__name";
        var->name_len = strlen(var->name);
        var->value = apr_pstrdup(msr->mp, real_col_name);
        var->value_len = strlen(var->value);
        apr_table_setn(table, var->name, (void *)var);

        /* Create time. */
        var = apr_pcalloc(msr->mp, sizeof(msc_string));
        var->name = "CREATE_TIME";
        var->name_len = strlen(var->name);
        var->value = apr_psprintf(msr->mp, "%" APR_TIME_T_FMT, (apr_time_t)apr_time_sec(msr->request_time));
        var->value_len = strlen(var->value);
        apr_table_setn(table, var->name, (void *)var);

        /* Update counter. */
        var = apr_pcalloc(msr->mp, sizeof(msc_string));
        var->name = "UPDATE_COUNTER";
        var->name_len = strlen(var->name);
        var->value = "0";
        var->value_len = strlen(var->value);
        apr_table_setn(table, var->name, (void *)var);

        /* This is a new collection. */
        var = apr_pcalloc(msr->mp, sizeof(msc_string));
        var->name = "IS_NEW";
        var->name_len = strlen(var->name);
        var->value = "1";
        var->value_len = strlen(var->value);
        apr_table_setn(table, var->name, (void *)var);
    }

    /* Record the original counter value before we change it */
    var = (msc_string *)apr_table_get(table, "UPDATE_COUNTER");
    if (var != NULL) {
        collection_original_setvar(msr, col_name, var);
    }

    /* Add the collection to the list. */
    apr_table_setn(msr->collections, apr_pstrdup(msr->mp, col_name), (void *)table);

    if (msr->txcfg->debuglog_level >= 4) {
        if (strcmp(col_name, real_col_name) != 0) {
            msr_log(msr, 4, "Added collection \"%s\" to the list as \"%s\".",
                log_escape(msr->mp, real_col_name), log_escape(msr->mp, col_name));
        } else {
            msr_log(msr, 4, "Added collection \"%s\" to the list.",
                log_escape(msr->mp, real_col_name));
        }
    }

    return 1;
}

/* initcol */
static apr_status_t msre_action_initcol_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    char *data = apr_pstrdup(msr->mp, action->param);
    char *col_name = NULL, *col_key = NULL;
    unsigned int col_key_len;

    msc_string *var = NULL;
    char *s = NULL;

    /* Extract the name and the value. */
    /* IMP1 We have a function for this now, parse_name_eq_value? */
    s = strstr(data, "=");
    if (s == NULL) return 0;
    col_name = strtolower_inplace((unsigned char *)data);
    col_key = s + 1;
    *s = '\0';

    /* Expand the key and init collection from storage. */
    var = apr_pcalloc(mptmp, sizeof(msc_string));
    var->value = col_key;
    var->value_len = strlen(var->value);
    expand_macros(msr, var, rule, mptmp);

    col_key = var->value;
    col_key_len = var->value_len;

    return init_collection(msr, col_name, col_name, col_key, col_key_len);
}

/* setsid */
static apr_status_t msre_action_setsid_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    msc_string *var = NULL;
    char *real_col_name = NULL, *col_key = NULL;
    unsigned int col_key_len;

    /* Construct session ID. */
    var = apr_pcalloc(mptmp, sizeof(msc_string));
    var->value = (char *)action->param;
    var->value_len = strlen(var->value);
    expand_macros(msr, var, rule, mptmp);
    msr->sessionid = apr_pstrdup(msr->mp, var->value);

    /* Construct collection name. */
    col_key = var->value;
    col_key_len = var->value_len;
    real_col_name = apr_psprintf(mptmp, "%s_SESSION", msr->txcfg->webappid);

    /* Initialise collection. */
    return init_collection(msr, real_col_name, "SESSION", col_key, col_key_len);
}

/* setuid */
static apr_status_t msre_action_setuid_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    msc_string *var = NULL;
    char *real_col_name = NULL, *col_key = NULL;
    unsigned int col_key_len;

    /* Construct user ID. */
    var = apr_pcalloc(mptmp, sizeof(msc_string));
    var->value = (char *)action->param;
    var->value_len = strlen(var->value);
    expand_macros(msr, var, rule, mptmp);
    msr->userid = apr_pstrdup(msr->mp, var->value);

    /* Construct collection name. */
    col_key = var->value;
    col_key_len = var->value_len;
    real_col_name = apr_psprintf(mptmp, "%s_USER", msr->txcfg->webappid);

    /* Initialise collection. */
    return init_collection(msr, real_col_name, "USER", col_key, col_key_len);
}

/* setrsc */
static apr_status_t msre_action_setrsc_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    msc_string *var = NULL;
    char *real_col_name = NULL, *col_key = NULL;
    unsigned int col_key_len;

    /* Construct user ID. */
    var = apr_pcalloc(mptmp, sizeof(msc_string));
    var->value = (char *)action->param;
    var->value_len = strlen(var->value);
    expand_macros(msr, var, rule, mptmp);

    /* Construct collection name. */
    col_key = var->value;
    col_key_len = var->value_len;
    real_col_name = apr_psprintf(mptmp, "%s_RESOURCE", msr->txcfg->webappid);

    /* Initialise collection. */
    return init_collection(msr, real_col_name, "RESOURCE", col_key, col_key_len);
}

/* exec */
static char *msre_action_exec_validate(msre_engine *engine, msre_action *action) {
    #if defined(WITH_LUA)
    char *filename = (char *)action->param;

    /* TODO Support relative filenames. */

    /* Process Lua scripts internally. */
    if (strlen(filename) > 4) {
        char *p = filename + strlen(filename) - 4;
        if ((p[0] == '.')&&(p[1] == 'l')&&(p[2] == 'u')&&(p[3] == 'a')) {
            /* It's a Lua script. */
            msc_script *script = NULL;

            /* Compile script. */
            char *msg = lua_compile(&script, filename, engine->mp);
            if (msg != NULL) return msg;

            action->param_data = script;
        }
    }
    #endif

    return NULL;
}

static apr_status_t msre_action_exec_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    #if defined(WITH_LUA)
    if (action->param_data != NULL) { /* Lua */
        msc_script *script = (msc_script *)action->param_data;
        char *my_error_msg = NULL;

        if (lua_execute(script, NULL, msr, rule, &my_error_msg) < 0) {
            msr_log(msr, 1, "%s", my_error_msg);
            return 0;
        }
    } else
    #endif
    { /* Execute as shell script. */
        char *script_output = NULL;

        int rc = apache2_exec(msr, action->param, NULL, &script_output);
        if (rc != 1) {
            msr_log(msr, 1, "Failed to execute: %s", action->param);
            return 0;
        }
    }

    return 1;
}

/* prepend */
static apr_status_t msre_action_prepend_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    msc_string *var = NULL;

    /* Expand any macros in the text */
    var = apr_pcalloc(mptmp, sizeof(msc_string));
    if (var == NULL) return -1;
    var->value = (char *)action->param;
    var->value_len = strlen(var->value);
    expand_macros(msr, var, rule, mptmp);

    /* ENH: Verify we really have to dup the data here. */
    msr->content_prepend = apr_pstrndup(msr->mp, var->value, var->value_len);
    msr->content_prepend_len = var->value_len;

    return 1;
}

/* append */
static apr_status_t msre_action_append_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    msc_string *var = NULL;

    /* Expand any macros in the text */
    var = apr_pcalloc(mptmp, sizeof(msc_string));
    if (var == NULL) return -1;
    var->value = (char *)action->param;
    var->value_len = strlen(var->value);
    expand_macros(msr, var, rule, mptmp);

    /* ENH: Verify we really have to dup the data here. */
    msr->content_append = apr_pstrndup(msr->mp, var->value, var->value_len);
    msr->content_append_len = var->value_len;

    return 1;
}

/* -- */

/**
 *
 */
void msre_engine_register_default_actions(msre_engine *engine) {

    /* id */
    msre_engine_action_register(engine,
        "id",
        ACTION_METADATA,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        msre_action_id_validate,
        msre_action_id_init,
        NULL
    );

    /* marker */
    msre_engine_action_register(engine,
        "marker",
        ACTION_METADATA,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        msre_action_marker_init,
        NULL
    );

    /* rev */
    msre_engine_action_register(engine,
        "rev",
        ACTION_METADATA,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        msre_action_rev_init,
        NULL
    );

    /* msg */
    msre_engine_action_register(engine,
        "msg",
        ACTION_METADATA,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        msre_action_msg_init,
        NULL
    );

    /* logdata */
    msre_engine_action_register(engine,
        "logdata",
        ACTION_METADATA,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        msre_action_logdata_init,
        NULL
    );

    /* accuracy */
    msre_engine_action_register(engine,
        "accuracy",
        ACTION_METADATA,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        msre_action_accuracy_init,
        NULL
    );

    /* maturity */
    msre_engine_action_register(engine,
        "maturity",
        ACTION_METADATA,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        msre_action_maturity_init,
        NULL
    );

    /* ver */
    msre_engine_action_register(engine,
        "ver",
        ACTION_METADATA,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        msre_action_ver_init,
        NULL
    );

    /* severity */
    msre_engine_action_register(engine,
        "severity",
        ACTION_METADATA,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        msre_action_severity_init,
        NULL
    );

    /* chain */
    msre_engine_action_register(engine,
        "chain",
        ACTION_FLOW,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        msre_action_chain_init,
        NULL
    );

    /* log */
    msre_engine_action_register(engine,
        "log",
        ACTION_NON_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_LOG,
        NULL,
        msre_action_log_init,
        NULL
    );

    /* nolog */
    msre_engine_action_register(engine,
        "nolog",
        ACTION_NON_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_LOG,
        NULL,
        msre_action_nolog_init,
        NULL
    );

    /* auditlog */
    msre_engine_action_register(engine,
        "auditlog",
        ACTION_NON_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_AUDITLOG,
        NULL,
        msre_action_auditlog_init,
        NULL
    );

    /* noauditlog */
    msre_engine_action_register(engine,
        "noauditlog",
        ACTION_NON_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_AUDITLOG,
        NULL,
        msre_action_noauditlog_init,
        NULL
    );

    /* deny */
    msre_engine_action_register(engine,
        "block",
        ACTION_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_DISRUPTIVE,
        NULL,
        msre_action_block_init,
        NULL
    );

    /* deny */
    msre_engine_action_register(engine,
        "deny",
        ACTION_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_DISRUPTIVE,
        NULL,
        msre_action_deny_init,
        NULL
    );

    /* status */
    msre_engine_action_register(engine,
        "status",
        ACTION_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        msre_action_status_validate,
        msre_action_status_init,
        NULL
    );

    /* drop */
    msre_engine_action_register(engine,
        "drop",
        ACTION_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_DISRUPTIVE,
        NULL,
        msre_action_drop_init,
        NULL
    );

    /* pause */
    msre_engine_action_register(engine,
        "pause",
        ACTION_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        msre_action_pause_validate,
        msre_action_pause_init,
        NULL
    );

    /* redirect */
    msre_engine_action_register(engine,
        "redirect",
        ACTION_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_DISRUPTIVE,
        msre_action_redirect_validate,
        msre_action_redirect_init,
        msre_action_redirect_execute
    );

    /* proxy */
    msre_engine_action_register(engine,
        "proxy",
        ACTION_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_DISRUPTIVE,
        msre_action_proxy_validate,
        msre_action_proxy_init,
        msre_action_proxy_execute
    );

    /* pass */
    msre_engine_action_register(engine,
        "pass",
        ACTION_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_DISRUPTIVE,
        NULL,
        msre_action_pass_init,
        NULL
    );

    /* skip */
    msre_engine_action_register(engine,
        "skip",
        ACTION_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_DISRUPTIVE,
        msre_action_skip_validate,
        msre_action_skip_init,
        NULL
    );

    /* skipAfter */
    msre_engine_action_register(engine,
        "skipAfter",
        ACTION_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_DISRUPTIVE,
        msre_action_skipAfter_validate,
        msre_action_skipAfter_init,
        NULL
    );

    /* allow */
    msre_engine_action_register(engine,
        "allow",
        ACTION_DISRUPTIVE,
        0, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_DISRUPTIVE,
        msre_action_allow_validate,
        msre_action_allow_init,
        NULL
    );

    /* phase */
    /* ENH: This should be ACTION_NON_DISRUPTIVE or ACTION_FLOW??? */
    msre_engine_action_register(engine,
        "phase",
        ACTION_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        msre_action_phase_validate,
        msre_action_phase_init,
        NULL
    );

    /* t */
    msre_engine_action_register(engine,
        "t",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        ALLOW_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        msre_action_t_validate,
        msre_action_t_init,
        NULL
    );

    /* ctl */
    msre_engine_action_register(engine,
        "ctl",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        msre_action_ctl_validate,
        msre_action_ctl_init,
        msre_action_ctl_execute
    );

    /* xmlns */
    msre_engine_action_register(engine,
        "xmlns",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        msre_action_xmlns_validate,
        NULL,
        NULL
    );

    /* capture */
    msre_engine_action_register(engine,
        "capture",
        ACTION_NON_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        NULL
    );

    /* sanitiseArg */
    msre_engine_action_register(engine,
        "sanitiseArg",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_sanitizeArg_execute
    );

    /* sanitiseMatchedBytes */
    msre_engine_action_register(engine,
        "sanitiseMatchedBytes",
        ACTION_NON_DISRUPTIVE,
        0, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        msre_action_sanitizeMatchedBytes_init,
        msre_action_sanitizeMatched_execute
    );

    /* sanitizeMatchedBytes */
    msre_engine_action_register(engine,
        "sanitizeMatchedBytes",
        ACTION_NON_DISRUPTIVE,
        0, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        msre_action_sanitizeMatchedBytes_init,
        msre_action_sanitizeMatched_execute
    );

    /* sanitizeArg */
    msre_engine_action_register(engine,
        "sanitizeArg",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_sanitizeArg_execute
    );

    /* sanitiseMatched */
    msre_engine_action_register(engine,
        "sanitiseMatched",
        ACTION_NON_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_sanitizeMatched_execute
    );

    /* sanitizeMatched */
    msre_engine_action_register(engine,
        "sanitizeMatched",
        ACTION_NON_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_sanitizeMatched_execute
    );

    /* sanitiseRequestHeader */
    msre_engine_action_register(engine,
        "sanitiseRequestHeader",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_sanitizeRequestHeader_execute
    );
    
    /* sanitizeRequestHeader */
    msre_engine_action_register(engine,
        "sanitizeRequestHeader",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_sanitizeRequestHeader_execute
    );

    /* sanitiseResponseHeader */
    msre_engine_action_register(engine,
        "sanitiseResponseHeader",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_sanitizeResponseHeader_execute
    );
    
    /* sanitizeResponseHeader */
    msre_engine_action_register(engine,
        "sanitizeResponseHeader",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_sanitizeResponseHeader_execute
    );

    /* setenv */
    msre_engine_action_register(engine,
        "setenv",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_setenv_execute
    );

    /* setvar */
    msre_engine_action_register(engine,
        "setvar",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_setvar_parse
    );

    /* expirevar */
    msre_engine_action_register(engine,
        "expirevar",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_expirevar_execute
    );

    /* deprecatevar */
    msre_engine_action_register(engine,
        "deprecatevar",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_deprecatevar_execute
    );

    /* initcol */
    msre_engine_action_register(engine,
        "initcol",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_initcol_execute
    );

    /* setsid */
    msre_engine_action_register(engine,
        "setsid",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_setsid_execute
    );

    /* setuid */
    msre_engine_action_register(engine,
        "setrsc",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_setrsc_execute
    );

    /* setuid */
    msre_engine_action_register(engine,
        "setuid",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_setuid_execute
    );

    /* exec */
    msre_engine_action_register(engine,
        "exec",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        msre_action_exec_validate,
        NULL,
        msre_action_exec_execute
    );

    /* multiMatch */
    msre_engine_action_register(engine,
        "multiMatch",
        ACTION_NON_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        NULL
    );

    /* tag */
    msre_engine_action_register(engine,
        "tag",
        ACTION_METADATA,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        NULL
    );

    /* prepend */
    msre_engine_action_register(engine,
        "prepend",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_prepend_execute
    );

    /* append */
    msre_engine_action_register(engine,
        "append",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        ACTION_CGROUP_NONE,
        NULL,
        NULL,
        msre_action_append_execute
    );
}
