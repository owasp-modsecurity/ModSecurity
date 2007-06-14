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
#include "re.h"
#include <ctype.h>

/**
 * Register action with the engine.
 */
static void msre_engine_action_register(msre_engine *engine, const char *name, unsigned int type,
    unsigned int argc_min, unsigned int argc_max, unsigned int allow_param_plusminus,
    unsigned int cardinality, FN_ACTION_VALIDATE(validate), FN_ACTION_INIT(init),
    FN_ACTION_EXECUTE(execute))
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
    metadata->validate = validate;
    metadata->init = init;
    metadata->execute = execute;

    apr_table_setn(engine->actions, name, (void *)metadata);
}

/**
 * Generates a single variable (from the supplied metadata).
 */
static msre_var *generate_single_var(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_pool_t *mptmp)
{
    apr_table_t *vartab = NULL;
    const apr_table_entry_t *te = NULL;
    const apr_array_header_t *arr = NULL;

    if (var->metadata->generate == NULL) return NULL;
    vartab = apr_table_make(mptmp, 16);
    var->metadata->generate(msr, var, rule, vartab, mptmp);
    arr = apr_table_elts(vartab);
    if (arr->nelts == 0) return NULL;
    te = (apr_table_entry_t *)arr->elts;    

    return (msre_var *)te[0].val;
}

/**
 * Expands macros ("%{NAME}" entities) if present
 * in the given variable.
 */
int DSOLOCAL expand_macros(modsec_rec *msr, msc_string *var, msre_rule *rule, apr_pool_t *mptmp) {
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

                    /* ENH Do we want to support %{DIGIT} as well? */

                    next_text_start = t + 1; /* *t was '}' */
                } else {
                    next_text_start = t; /* *t was '\0' */
                }
            } else
            if ((*(p + 1) >= '0')&&(*(p + 1) <= '9')) {
                /* Special case for regex captures. */
                var_name = "TX";
                var_value = apr_pstrmemdup(mptmp, p + 1, 1);
                next_text_start = p + 2;
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
                    var_generated = generate_single_var(msr, var_resolved, rule, mptmp);
                    if (var_generated != NULL) {
                        part = (msc_string *)apr_pcalloc(mptmp, sizeof(msc_string));
                        if (part == NULL) return -1;
                        part->value_len = var_generated->value_len;
                        part->value = (char *)var_generated->value;
                        *(msc_string **)apr_array_push(arr) = part;
                    }
                } else {
                    /* ENH Should we log something because the macro could not be resolved? */
                }
            } else {
                /* We could not identify a valid macro so add it as text. */
                part = (msc_string *)apr_pcalloc(mptmp, sizeof(msc_string));
                if (part == NULL) return -1;
                part->value_len = p - text_start;
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

/* id */

static apr_status_t msre_action_id_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->id = action->param;
    return 1;
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

/* deny */
static apr_status_t msre_action_deny_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->intercept_action = ACTION_DENY;
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
    actionset->intercept_pause = atoi(action->param);
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

/* allow */

static apr_status_t msre_action_allow_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
    actionset->intercept_action = ACTION_ALLOW;
    return 1;
}

/* phase */

static char *msre_action_phase_validate(msre_engine *engine, msre_action *action) {
    /* ENH Add validation. */
    return NULL;
}

static apr_status_t msre_action_phase_init(msre_engine *engine, msre_actionset *actionset,
    msre_action *action)
{
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
    if (strcmp(name, "ruleEngine") == 0) {
        if (strcasecmp(value, "on") == 0) return NULL;
        if (strcasecmp(value, "off") == 0) return NULL;
        if (strcasecmp(value, "detectiononly") == 0) return NULL;
        return apr_psprintf(engine->mp, "Invalid setting for ctl name ruleEngine: %s", value);
    } else
    if (strcmp(name, "requestBodyAccess") == 0) {
        if (parse_boolean(value) == -1) {
            return apr_psprintf(engine->mp, "Invalid setting for ctl name "
                " requestBodyAccess: %s", value);
        }
        return NULL;
    } else
    if (strcmp(name, "requestBodyProcessor") == 0) {
        /* ENH We will accept anything for now but it'd be nice
         * to add a check here that the processor name is a valid one.
         */
        return NULL;
    } else
    if (strcmp(name, "responseBodyAccess") == 0) {
        if (parse_boolean(value) == -1) {
            return apr_psprintf(engine->mp, "Invalid setting for ctl name "
                " responseBodyAccess: %s", value);
        }
        return NULL;
    } else
    if (strcmp(name, "auditEngine") == 0) {
        if (strcasecmp(value, "on") == 0) return NULL;
        if (strcasecmp(value, "off") == 0) return NULL;
        if (strcasecmp(value, "relevantonly") == 0) return NULL;
        return apr_psprintf(engine->mp, "Invalid setting for ctl name "
            " auditEngine: %s", value);
    } else
    if (strcmp(name, "auditLogParts") == 0) {
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
    if (strcmp(name, "debugLogLevel") == 0) {
        if ((atoi(value) >= 0)&&(atoi(value) <= 9)) return NULL;
        return apr_psprintf(engine->mp, "Invalid setting for ctl name "
            "debugLogLevel: %s", value);
    } else
    if (strcmp(name, "requestBodyLimit") == 0) {
        long int limit = strtol(value, NULL, 10);

        if ((limit == LONG_MAX)||(limit == LONG_MIN)||(limit <= 0)) {
            return apr_psprintf(engine->mp, "Invalid setting for ctl name "
                "requestBodyLimit: %s", value);
        }

        if (limit > REQUEST_BODY_HARD_LIMIT) {
            return apr_psprintf(engine->mp, "Request size limit cannot exceed "
                "the hard limit: %li", RESPONSE_BODY_HARD_LIMIT);
        }

        return NULL;
    } else
    if (strcmp(name, "responseBodyLimit") == 0) {
        long int limit = strtol(value, NULL, 10);

        if ((limit == LONG_MAX)||(limit == LONG_MIN)||(limit <= 0)) {
            return apr_psprintf(engine->mp, "Invalid setting for ctl name "
                "responseBodyLimit: %s", value);
        }

        if (limit > RESPONSE_BODY_HARD_LIMIT) {
            return apr_psprintf(engine->mp, "Response size limit cannot exceed "
                "the hard limit: %li", RESPONSE_BODY_HARD_LIMIT);
        }

        return NULL;
    }
    else {
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
    if (strcmp(name, "ruleEngine") == 0) {
        if (strcasecmp(value, "on") == 0) {
            msr->txcfg->is_enabled = MODSEC_ENABLED;
            msr->usercfg->is_enabled = MODSEC_ENABLED;
        }

        if (strcasecmp(value, "off") == 0) {
            msr->txcfg->is_enabled = MODSEC_DISABLED;
            msr->usercfg->is_enabled = MODSEC_DISABLED;
        }

        if (strcasecmp(value, "detectiononly") == 0) {
            msr->txcfg->is_enabled = MODSEC_DETECTION_ONLY;
            msr->usercfg->is_enabled = MODSEC_DETECTION_ONLY;
        }

        return 1;
    } else
    if (strcmp(name, "requestBodyAccess") == 0) {
        int pv = parse_boolean(value);

        if (pv == -1) return -1;
        msr->txcfg->reqbody_access = pv;
        msr->usercfg->reqbody_access = pv;
        msr_log(msr, 4, "Ctl: Set requestBodyAccess to %i.", pv);

        return 1;
    } else
    if (strcmp(name, "requestBodyProcessor") == 0) {
        msr->msc_reqbody_processor = value;
        msr_log(msr, 4, "Ctl: Set requestBodyProcessor to %s.", value);

        return 1;
    } else
    if (strcmp(name, "responseBodyAccess") == 0) {
        int pv = parse_boolean(value);

        if (pv == -1) return -1;
        msr->txcfg->resbody_access = pv;
        msr->usercfg->resbody_access = pv;
        msr_log(msr, 4, "Ctl: Set responseBodyAccess to %i.", pv);

        return 1;
    } else
    if (strcmp(name, "auditEngine") == 0) {
        if (strcasecmp(value, "on") == 0) {
            msr->txcfg->auditlog_flag = AUDITLOG_ON;
            msr->usercfg->auditlog_flag = AUDITLOG_ON;
        }

        if (strcasecmp(value, "off") == 0) {
            msr->txcfg->auditlog_flag = AUDITLOG_OFF;
            msr->usercfg->auditlog_flag = AUDITLOG_OFF;
        }

        if (strcasecmp(value, "relevantonly") == 0) {
            msr->txcfg->auditlog_flag = AUDITLOG_RELEVANT;
            msr->usercfg->auditlog_flag = AUDITLOG_RELEVANT;
        }

        msr_log(msr, 4, "Ctl: Set auditEngine to %i.", msr->txcfg->auditlog_flag); // TODO

        return 1;
    } else
    if (strcmp(name, "auditLogParts") == 0) {
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
                        *d++ = *s++;
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
        msr_log(msr, 4, "Ctl: Set auditLogParts to %s.", msr->txcfg->auditlog_parts);

        return 1;
    } else
    if (strcmp(name, "debugLogLevel") == 0) {
        msr->txcfg->debuglog_level = atoi(value);
        msr->usercfg->debuglog_level = atoi(value);
        msr_log(msr, 4, "Ctl: Set debugLogLevel to %i.", msr->txcfg->debuglog_level);

        return 1;
    } else
    if (strcmp(name, "requestBodyLimit") == 0) {
        long int limit = strtol(value, NULL, 10);

        /* ENH Accept only in correct phase warn otherwise. */
        msr->txcfg->reqbody_limit = limit;
        msr->usercfg->reqbody_limit = limit;

        return 1;
    } else
    if (strcmp(name, "responseBodyLimit") == 0) {
        long int limit = strtol(value, NULL, 10);

        /* ENH Accept only in correct phase warn otherwise. */
        msr->txcfg->of_limit = limit;
        msr->usercfg->of_limit = limit;

        return 1;
    }
    else {
        /* ENH Should never happen, but log if it does. */
        return -1;
    }    
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

/* sanitiseArg */
static apr_status_t msre_action_sanitiseArg_execute(modsec_rec *msr, apr_pool_t *mptmp,
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
            apr_table_addn(msr->arguments_to_sanitise, arg->name, (void *)arg);
        }
    }

    return 1;
}

#define SANITISE_ARG                1
#define SANITISE_REQUEST_HEADER     2
#define SANITISE_RESPONSE_HEADER    3

/* sanitiseMatched */
static apr_status_t msre_action_sanitiseMatched_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    const char *sargname = NULL;
    const apr_array_header_t *tarr;
    const apr_table_entry_t *telts;
    int i, type = 0;

    if (msr->matched_var == NULL) return 0;

    /* IMP1 We need to extract the variable name properly here,
     *      taking into account it may have been escaped.
     */
    if (strncmp(msr->matched_var, "ARGS:", 5) == 0) {
        sargname = apr_pstrdup(msr->mp, msr->matched_var + 5);
        type = SANITISE_ARG;
    } else
    if (strncmp(msr->matched_var, "ARGS_NAMES:", 11) == 0) {
        sargname = apr_pstrdup(msr->mp, msr->matched_var + 11);
        type = SANITISE_ARG;
    } else
    if (strncmp(msr->matched_var, "REQUEST_HEADERS:", 16) == 0) {
        sargname = apr_pstrdup(msr->mp, msr->matched_var + 16);
        type = SANITISE_REQUEST_HEADER;
    } else
    if (strncmp(msr->matched_var, "REQUEST_HEADERS_NAMES:", 22) == 0) {
        sargname = apr_pstrdup(msr->mp, msr->matched_var + 22);
        type = SANITISE_REQUEST_HEADER;
    } else
    if (strncmp(msr->matched_var, "RESPONSE_HEADERS:", 17) == 0) {
        sargname = apr_pstrdup(msr->mp, msr->matched_var + 17);
        type = SANITISE_RESPONSE_HEADER;
    } else
    if (strncmp(msr->matched_var, "RESPONSE_HEADERS_NAMES:", 23) == 0) {
        sargname = apr_pstrdup(msr->mp, msr->matched_var + 23);
        type = SANITISE_RESPONSE_HEADER;
    }
    else {
        msr_log(msr, 3, "sanitiseMatched: Don't know how to handle variable: %s",
            msr->matched_var);
        return 0;
    }

    switch(type) {
        case SANITISE_ARG :
            tarr = apr_table_elts(msr->arguments);
            telts = (const apr_table_entry_t*)tarr->elts;
            for (i = 0; i < tarr->nelts; i++) {
                msc_arg *arg = (msc_arg *)telts[i].val;
                if (strcasecmp(sargname, arg->name) == 0) {
                    apr_table_addn(msr->arguments_to_sanitise, arg->name, (void *)arg);
                }
            }
            break;

        case SANITISE_REQUEST_HEADER :
            apr_table_set(msr->request_headers_to_sanitise, sargname, "1");
            break;

        case SANITISE_RESPONSE_HEADER :
            apr_table_set(msr->response_headers_to_sanitise, sargname, "1");
            break;

        default :
            /* do nothing */
            break;
    }

    return 1;
}

/* sanitiseRequestHeader */
static apr_status_t msre_action_sanitiseRequestHeader_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    apr_table_set(msr->request_headers_to_sanitise, action->param, "1");
    return 1;
}

/* sanitiseResponseHeader */
static apr_status_t msre_action_sanitiseResponseHeader_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    apr_table_set(msr->response_headers_to_sanitise, action->param, "1");
    return 1;
}

/* setenv */
static apr_status_t msre_action_setenv_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    char *data = apr_pstrdup(mptmp, action->param);
    char *env_name = NULL, *env_value = NULL;
    char *s = NULL;

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

    /* Execute the requested action. */
    if (env_name[0] == '!') {
        /* Delete */
        apr_table_unset(msr->r->subprocess_env, env_name + 1);
    } else {
        /* Set */
        apr_table_set(msr->r->subprocess_env, env_name, env_value);
    }

    return 1;
}

/* setvar */
static apr_status_t msre_action_setvar_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    char *data = apr_pstrdup(mptmp, action->param);
    char *col_name = NULL, *var_name = NULL, *var_value = NULL;
    char *s = NULL;
    apr_table_t *target_col = NULL;
    int is_negated = 0;

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
        /* ENH Log warning detected variable name but no collection. */
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
            msr_log(msr, 3, "Could not set variable \"%s.%s\" as the collection does not exist.",
                log_escape(msr->mp, col_name), log_escape(msr->mp, var_name));
            return 0;
        }
    }        

    if (is_negated) {
        /* Unset variable. */

        /* ENH Refuse to remove certain variables, e.g. TIMEOUT, internal variables, etc... */

        apr_table_unset(target_col, var_name);
        msr_log(msr, 9, "Unset variable \"%s.%s\".", log_escape(mptmp, col_name),
            log_escape(mptmp, var_name));
    } else {
        /* Set or change variable. */

        if ((var_value[0] == '+')||(var_value[0] == '-')) {
            /* Relative change. */
            msc_string *var = NULL;
            int value = 0;

            /* Retrieve  variable or generate (if it does not exist). */
            var = (msc_string *)apr_table_get(target_col, var_name);
            if (var == NULL) {
                var = apr_pcalloc(msr->mp, sizeof(msc_string));
                var->name = apr_pstrdup(msr->mp, var_name);
                var->name_len = strlen(var->name);
                value = 0;
            } else {
                value = atoi(var->value);
            }

            /* Change value. */
            value += atoi(var_value);
            if (value < 0) value = 0; /* Counters never go below zero. */

            /* Put the variable back. */
            var->value = apr_psprintf(msr->mp, "%i", value);
            var->value_len = strlen(var->value);
            apr_table_setn(target_col, var->name, (void *)var);

            msr_log(msr, 9, "Set variable \"%s.%s\" to \"%s\".", log_escape(mptmp, col_name),
                log_escape(mptmp, var->name), log_escape(mptmp, var->value));
        } else {
            /* Absolute change. */

            msc_string *var = NULL;

            var = apr_pcalloc(msr->mp, sizeof(msc_string));
            var->name = apr_pstrdup(msr->mp, var_name);
            var->name_len = strlen(var->name);
            var->value = apr_pstrdup(msr->mp, var_value);
            var->value_len = strlen(var->value);
            expand_macros(msr, var, rule, mptmp);
            apr_table_setn(target_col, var->name, (void *)var);

            msr_log(msr, 9, "Set variable \"%s.%s\" to \"%s\".", log_escape(mptmp, col_name),
                log_escape(mptmp, var->name), log_escape(mptmp, var->value));
        }
    }

    /* Make note of the change so that we know later
     * we need to persist the collection.
     */
    apr_table_set(msr->collections_dirty, col_name, "1");

    return 1;
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
            msr_log(msr, 3, "Could not set variable \"%s.%s\" as the collection does not exist.",
                log_escape(msr->mp, col_name), log_escape(msr->mp, var_name));
            return 0;
        }
    } else {
        /* ENH Log warning detected variable name but no collection. */
        return 0;
    }

    /* To expire a variable we just place a special variable into
     * the collection. Expiry actually happens when the collection
     * is retrieved from storage the next time.
     */
    var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    var->name = apr_psprintf(msr->mp, "__expire_%s", var_name);
    var->name_len = strlen(var->name);
    var->value = apr_psprintf(msr->mp, "%i", (int)(apr_time_sec(msr->request_time)
        + atoi(var_value)));
    var->value_len = strlen(var->value);
    apr_table_setn(target_col, var->name, (void *)var);

    msr_log(msr, 4, "Variable \"%s.%s\" set to expire in %s seconds.", col_name,
        var_name, var_value);

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
    unsigned int last_update_time, current_time;
    long int current_value, new_value;

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

    /* Choose the collection to work with. */
    s = strstr(var_name, ".");
    if (s != NULL) {
        col_name = var_name;
        var_name = s + 1;
        *s = '\0';

        /* IMP1 Add message TX variables cannot deprecate in value. */

        target_col = (apr_table_t *)apr_table_get(msr->collections, col_name);
        if (target_col == NULL) {
            msr_log(msr, 3, "Could not deprecate variable \"%s.%s\" as the collection does "
                "not exist.", log_escape(msr->mp, col_name), log_escape(msr->mp, var_name));
            return 0;
        }
    } else {
        /* ENH Log warning detected variable name but no collection. */
        return 0;
    }

    /* Find the current value. */
    var = (msc_string *)apr_table_get(target_col, var_name);
    if (var == NULL) {
        msr_log(msr, 9, "Asked to deprecate variable \"%s.%s\" but it does not exist.",
            log_escape(msr->mp, col_name), log_escape(msr->mp, var_name));
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

    current_time = (unsigned int)apr_time_sec(apr_time_now());
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
        ((current_time - last_update_time) * atoi(var_value) / atoi(s));
    if (new_value < 0) new_value = 0;

    /* Only change the value if it differs. */
    if (new_value != current_value) {
        var->value = apr_psprintf(msr->mp, "%i", (int)new_value);
        var->value_len = strlen(var->value);

        msr_log(msr, 4, "Deprecated variable \"%s.%s\" from %li to %li (%i seconds since "
            "last update).", log_escape(msr->mp, col_name), log_escape(msr->mp, var_name),
            current_value, new_value, current_time - last_update_time);

        apr_table_set(msr->collections_dirty, col_name, "1");
    } else {
        msr_log(msr, 9, "Not deprecating variable \"%s.%s\" because the new value (%li) is "
            "the same as the old one (%li) (%i seconds since last update).",
            log_escape(msr->mp, col_name), log_escape(msr->mp, var_name), current_value,
            new_value, current_time - last_update_time);
    }
    
    return 1;
}

static apr_status_t init_collection(modsec_rec *msr, const char *real_col_name,
    const char *col_name, const char *col_key, unsigned int col_key_len)
{
    apr_table_t *table = NULL;

    /* IMP1 Cannot initialise the built-in collections this way. */

    /* Does the collection exist already? */
    if (apr_table_get(msr->collections, col_name) != NULL) {
        /* ENH Warn about this. */
        return 0;
    }

    /* Init collection from storage. */
    table = collection_retrieve(msr, real_col_name, col_key, col_key_len);

    if (table == NULL) {
        msc_string *var = NULL;

        /* Does not exist yet - create new. */
        msr_log(msr, 4, "Creating collection (name \"%s\", key \"%s\").",
            real_col_name, col_key);

        table = apr_table_make(msr->mp, 24);    

        /* IMP1 Is the timeout hard-coded to 3600? */

        /* Add default timeout. */
        var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
        var->name = "__expire_KEY";
        var->name_len = strlen(var->name);
        var->value = apr_psprintf(msr->mp, "%i", (int)(apr_time_sec(msr->request_time) + 3600));
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
        var->value = apr_psprintf(msr->mp, "%i", 3600);
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
        var->value = apr_psprintf(msr->mp, "%i", (int)apr_time_sec(msr->request_time));
        var->value_len = strlen(var->value);
        apr_table_setn(table, var->name, (void *)var);

        /* Update counter. */
        var = apr_pcalloc(msr->mp, sizeof(msc_string));
        var->name = "UPDATE_COUNTER";
        var->name_len = strlen(var->name);
        var->value = "0";
        var->value_len = strlen(var->value);
        apr_table_setn(table, var->name, (void *)var);
    }

    /* Add the collection to the list. */
    apr_table_setn(msr->collections, apr_pstrdup(msr->mp, col_name), (void *)table);

    if (strcmp(col_name, real_col_name) != 0) {
        msr_log(msr, 4, "Added collection \"%s\" to the list as \"%s\".",
            log_escape(msr->mp, real_col_name), log_escape(msr->mp, col_name));
    } else {
        msr_log(msr, 4, "Added collection \"%s\" to the list.",
            log_escape(msr->mp, real_col_name));
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
    col_name = data;
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

/* exec */
static apr_status_t msre_action_exec_execute(modsec_rec *msr, apr_pool_t *mptmp,
    msre_rule *rule, msre_action *action)
{
    char *script_output = NULL;

    int rc = apache2_exec(msr, action->param, NULL, &script_output);
    if (rc != 1) {
        msr_log(msr, 1, "Failed to execute: %s", action->param);
        return 0;
    }

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
        NULL,
        msre_action_id_init,
        NULL
    );

    /* rev */
    msre_engine_action_register(engine,
        "rev",
        ACTION_METADATA,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
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
        NULL,
        msre_action_msg_init,
        NULL
    );

    /* severity */
    msre_engine_action_register(engine,
        "severity",
        ACTION_METADATA,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
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
        NULL,
        msre_action_noauditlog_init,
        NULL
    );

    /* deny */
    msre_engine_action_register(engine,
        "deny",
        ACTION_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
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
        msre_action_skip_validate,
        msre_action_skip_init,
        NULL
    );

    /* allow */
    msre_engine_action_register(engine,
        "allow",
        ACTION_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
        NULL,
        msre_action_allow_init,
        NULL
    );

    /* phase */
    msre_engine_action_register(engine,
        "phase",
        ACTION_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
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
        NULL,
        NULL,
        msre_action_sanitiseArg_execute
    );

    /* sanitiseMatched */
    msre_engine_action_register(engine,
        "sanitiseMatched",
        ACTION_NON_DISRUPTIVE,
        0, 0,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        NULL,
        NULL,
        msre_action_sanitiseMatched_execute
    );

    /* sanitiseRequestHeader */
    msre_engine_action_register(engine,
        "sanitiseRequestHeader",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        NULL,
        NULL,
        msre_action_sanitiseRequestHeader_execute
    );

    /* sanitiseResponseHeader */
    msre_engine_action_register(engine,
        "sanitiseResponseHeader",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        NULL,
        NULL,
        msre_action_sanitiseResponseHeader_execute
    );

    /* setenv */
    msre_engine_action_register(engine,
        "setenv",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
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
        NULL,
        NULL,
        msre_action_setvar_execute
    );

    /* expirevar */
    msre_engine_action_register(engine,
        "expirevar",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
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
        NULL,
        NULL,
        msre_action_setsid_execute
    );

    /* setuid */
    msre_engine_action_register(engine,
        "setuid",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_ONE,
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
        NULL,
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
        NULL,
        NULL,
        NULL
    );

    /* tag */
    msre_engine_action_register(engine,
        "tag",
        ACTION_NON_DISRUPTIVE,
        1, 1,
        NO_PLUS_MINUS,
        ACTION_CARDINALITY_MANY,
        NULL,
        NULL,
        NULL
    );
}
