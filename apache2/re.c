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

#include "re.h"


/* -- Actions, variables, functions and operator functions ----------------- */

/**
 * Creates msre_var instances (rule variables) out of the 
 * given text string and places them into the supplied table.
 */
apr_status_t msre_parse_targets(msre_ruleset *ruleset, const char *text,
    apr_array_header_t *arr, char **error_msg)
{
    const apr_array_header_t *tarr;
    const apr_table_entry_t *telts;
    apr_table_t *vartable;
    unsigned int count = 0;
    apr_status_t rc;
    msre_var *var;
    int i;
    
    if (text == NULL) return -1;

    /* Extract name & value pairs first */
    vartable = apr_table_make(ruleset->mp, 10);
    if (vartable == NULL) return -1;
    rc = msre_parse_generic(ruleset->mp, text, vartable, error_msg);
    if (rc < 0) return rc;    

    /* Loop through the table and create variables */
    tarr = apr_table_elts(vartable);
    telts = (const apr_table_entry_t*)tarr->elts;
    for (i = 0; i < tarr->nelts; i++) {
        var = msre_create_var(ruleset, telts[i].key, telts[i].val, NULL, error_msg);
        if (var == NULL) return -1;
        *(msre_var **)apr_array_push(arr) = var;
        count++;
    }

    return count;
}

/**
 * Creates msre_action instances by parsing the given string, placing
 * them into the supplied array.
 */
apr_status_t msre_parse_actions(msre_engine *engine, msre_actionset *actionset,
    const char *text, char **error_msg)
{
    const apr_array_header_t *tarr;
    const apr_table_entry_t *telts;
    apr_table_t *vartable;
    unsigned int count = 0;
    apr_status_t rc;
    msre_action *action;
    int i;

    if (text == NULL) return -1;    

    /* Extract name & value pairs first */
    vartable = apr_table_make(engine->mp, 10);
    if (vartable == NULL) return -1;
    rc = msre_parse_generic(engine->mp, text, vartable, error_msg);
    if (rc < 0) return rc;    

    /* Loop through the table and create actions */
    tarr = apr_table_elts(vartable);
    telts = (const apr_table_entry_t*)tarr->elts;
    for (i = 0; i < tarr->nelts; i++) {
        /* Create action. */
        action = msre_create_action(engine, telts[i].key, telts[i].val, error_msg);
        if (action == NULL) return -1;

        /* Initialise action (option). */
        if (action->metadata->init != NULL) {
            action->metadata->init(engine, actionset, action);
        }

        if (action->metadata->cardinality == ACTION_CARDINALITY_ONE) {
            /* One action per actionlist. */
            apr_table_setn(actionset->actions, action->metadata->name, (void *)action);
        } else {
            /* Multiple actions per actionlist. */
            apr_table_addn(actionset->actions, action->metadata->name, (void *)action);
        }

        count++;
    }

    return count;
}

/**
 * Locates variable metadata given the variable name.
 */
msre_var_metadata *msre_resolve_var(msre_engine *engine, const char *name) {
    return (msre_var_metadata *)apr_table_get(engine->variables, name);    
}

/**
 * Locates action metadata given the action name.
 */
msre_action_metadata *msre_resolve_action(msre_engine *engine, const char *name) {
    return (msre_action_metadata *)apr_table_get(engine->actions, name);    
}

/**
 * Creates a new variable instance given the variable name
 * and an (optional) parameter.
 */
msre_var *msre_create_var_ex(apr_pool_t *pool, msre_engine *engine, const char *name, const char *param,
    modsec_rec *msr, char **error_msg)
{
    msre_var *var = apr_pcalloc(pool, sizeof(msre_var));
    if (var == NULL) return NULL;

    if (error_msg == NULL) return NULL;
    *error_msg = NULL;

    /* Handle negation and member counting */
    if (name[0] == '!') {
        var->is_negated = 1;
        var->name = name + 1;
    }
    else
    if (name[0] == '&') {
        var->is_counting = 1;
        var->name = name + 1;
    }
    else {
        var->name = name;
    }

    /* CGI HTTP variables emulation. */
    if (strncasecmp(var->name, "HTTP_", 5) == 0) {    
        if (var->param != NULL) {
            *error_msg = apr_psprintf(pool, "Variable %s does not support parameters.",
                var->name);
            return NULL;
        }
        
        var->param = var->name + 5;
        var->name = "REQUEST_HEADERS";
    }

    /* Resolve variable */
    var->metadata = msre_resolve_var(engine, var->name);
    if (var->metadata == NULL) {
        *error_msg = apr_psprintf(pool, "Unknown variable: %s", name);
        return NULL;
    }

    /* The counting operator "&" can only be used against collections. */
    if (var->is_counting) {
        if (var->metadata->type == VAR_SIMPLE) {
            *error_msg = apr_psprintf(pool, "The & modificator does not apply to "
                "non-collection variables.");
            return NULL;
        }
    }

    /* Check the parameter. */
    if (param == NULL) {
        if (var->metadata->argc_min > 0) {
            *error_msg = apr_psprintf(pool, "Missing mandatory parameter for variable %s.",
                name);
            return NULL;
        }
    } else { /* Parameter present */

        /* Do we allow a parameter? */
        if (var->metadata->argc_max == 0) {
            *error_msg = apr_psprintf(pool, "Variable %s does not support parameters.",
                name);
            return NULL;
        }

        var->param = param;
    }

    return var;
}

/**
 * Create a new variable object from the provided name and value.
 *
 * NOTE: this allocates out of the global pool and should not be used
 *       per-request
 */
msre_var *msre_create_var(msre_ruleset *ruleset, const char *name, const char *param,
    modsec_rec *msr, char **error_msg)
{
    msre_var *var = msre_create_var_ex(ruleset->engine->mp, ruleset->engine, name, param, msr, error_msg);
    if (var == NULL) return NULL;

    /* Validate & initialise variable */
    if (var->metadata->validate != NULL) {
        *error_msg = var->metadata->validate(ruleset, var);
        if (*error_msg != NULL) {
            /* ENH Shouldn't we log the problem? */
            return NULL;
        }
    }

    return var;
}

/**
 * Creates a new action instance given its name and an (optional) parameter.
 */
msre_action *msre_create_action(msre_engine *engine, const char *name, const char *param,
    char **error_msg)
{
    msre_action *action = apr_pcalloc(engine->mp, sizeof(msre_action));
    if (action == NULL) return NULL;

    if (error_msg == NULL) return NULL;
    *error_msg = NULL;

    /* Resolve action */
    action->metadata = msre_resolve_action(engine, name);
    if (action->metadata == NULL) {
        *error_msg = apr_psprintf(engine->mp, "Unknown action: %s", name);
        return NULL;
    }

    if (param == NULL) { /* Parameter not present */
        if (action->metadata->argc_min > 0) {
            *error_msg = apr_psprintf(engine->mp, "Missing mandatory parameter for action %s",
                name);
            return NULL;
        }
    } else { /* Parameter present */

        /* Should we allow the parameter? */
        if (action->metadata->argc_max == 0) {
            *error_msg = apr_psprintf(engine->mp, "Extra parameter provided to action %s", name);
            return NULL;
        }

        /* Handle +/- modificators */
        if ((param[0] == '+')||(param[0] == '-')) {
            if (action->metadata->allow_param_plusminus == 0) {
                *error_msg = apr_psprintf(engine->mp,
                     "Action %s does not allow +/- modificators.", name);
                return NULL;
            }
            else { /* Modificators allowed. */
                if (param[0] == '+') {
                    action->param = param + 1;
                    action->param_plusminus = POSITIVE_VALUE;
                } else
                if (param[0] == '-') {
                    action->param = param + 1;
                    action->param_plusminus = NEGATIVE_VALUE;
                }
            }
        } else {
            action->param = param;
        }

        /* Validate parameter */
        if (action->metadata->validate != NULL) {
            *error_msg = action->metadata->validate(engine, action);
            if (*error_msg != NULL) return NULL;
        }
    }

    return action;
}

/**
 * Generic parser that is used as basis for target and action parsing.
 * It breaks up the input string into name-parameter pairs and places
 * them into the given table.
 */
int msre_parse_generic(apr_pool_t *mp, const char *text, apr_table_t *vartable,
    char **error_msg)
{
    char *p = (char *)text;
    int count = 0;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    count = 0;
    while(*p != '\0') {
        char *name = NULL, *value = NULL;

        /* ignore whitespace */
        while(isspace(*p)) p++;
        if (*p == '\0') return count;

        /* we are at the beginning of the name */
        name = p;
        while((*p != '\0')&&(*p != '|')&&(*p != ':')&&(*p != ',')&&(!isspace(*p))) p++; // ENH replace with isvarnamechar()

        /* get the name */
        name = apr_pstrmemdup(mp, name, p - name);

        if (*p != ':') { /* we don't have a parameter */
            /* add to the table with no value */
            apr_table_addn(vartable, name, NULL);
            count++;

            /* go over any whitespace present */
            while(isspace(*p)) p++;
    
            /* we're done */
            if (*p == '\0') {
                return count;
            }

            /* skip over the separator character and continue */
            if ((*p == ',')||(*p == '|')) {
                p++;
                continue;
            }

            *error_msg = apr_psprintf(mp, "Unexpected character at position %i: %s",
                (int)(p - text), text);
            return -1;
        }

        /* we have a parameter */

        p++; /* move over the colon */

        /* we'll allow empty values */
        if (*p == '\0') {
            apr_table_addn(vartable, name, NULL);
            count++;
            return count;
        }

        if ((*p == ',')||(*p == '|')) {
            apr_table_addn(vartable, name, NULL);
            count++;
            /* move over the separator char and continue */
            p++;
            continue;
        }

        /* we really have a parameter */

        if (*p == '\'') { /* quoted value */
            char *d = NULL;

            p++; /* go over the openning quote */
            value = d = strdup(p);
            if (d == NULL) return -1;

            for(;;) {
                if (*p == '\0') {
                    *error_msg = apr_psprintf(mp, "Missing closing quote at position %i: %s",
                        (int)(p - text), text);
                    free(value);
                    return -1;
                } else
                if (*p == '\\') {
                    if ( (*(p + 1) == '\0') || ((*(p + 1) != '\'')&&(*(p + 1) != '\\')) ) {
                        *error_msg = apr_psprintf(mp, "Invalid quoted pair at position %i: %s",
                            (int)(p - text), text);
                        free(value);
                        return -1;
                    }
                    p++;
                    *d++ = *p++;
                } else
                if (*p == '\'') {
                    *d = '\0';
                    p++;
                    break;
                }
                else {
                    *d++ = *p++;
                }
            }

            d = value;
            value = apr_pstrdup(mp, d);
            free(d);
        } else { /* non-quoted value */
            value = p;
            while((*p != '\0')&&(*p != ',')&&(*p != '|')&&(!isspace(*p))) p++;
            value = apr_pstrmemdup(mp, value, p - value);
        }

        /* add to table */
        apr_table_addn(vartable, name, value);
        count++;

        /* move to the first character of the next name-value pair */
        while(isspace(*p)||(*p == ',')||(*p == '|')) p++;
    }
    
    return count;
}


/* -- Actionset functions -------------------------------------------------- */

/**
 * Creates an actionset instance and (as an option) populates it by
 * parsing the given string which contains a list of actions.
 */
msre_actionset *msre_actionset_create(msre_engine *engine, const char *text,
    char **error_msg)
{
    msre_actionset *actionset = (msre_actionset *)apr_pcalloc(engine->mp,
        sizeof(msre_actionset));
    if (actionset == NULL) return NULL;

    actionset->actions = apr_table_make(engine->mp, 25);
    if (actionset->actions == NULL) return NULL;

    /* Metadata */
    actionset->id = NOT_SET_P;
    actionset->rev = NOT_SET_P;
    actionset->msg = NOT_SET_P;
    actionset->phase = NOT_SET;
    actionset->severity = -1;

    /* Flow */
    actionset->is_chained = NOT_SET;
    actionset->skip_count = NOT_SET;

    /* Disruptive */
    actionset->intercept_action = NOT_SET;
    actionset->intercept_uri = NOT_SET_P;
    actionset->intercept_status = NOT_SET;
    actionset->intercept_pause = NOT_SET;

    /* Other */
    actionset->auditlog = NOT_SET;
    actionset->log = NOT_SET;

    /* Parse the list of actions, if it's present */
    if (text != NULL) {
        if (msre_parse_actions(engine, actionset, text, error_msg) < 0) {
            return NULL;
        }
    }

    return actionset;    
}

/**
 * Create a (shallow) copy of the supplied actionset.
 */
static msre_actionset *msre_actionset_copy(apr_pool_t *mp, msre_actionset *orig) {
    msre_actionset *copy = NULL;

    if (orig == NULL) return NULL;
    copy = (msre_actionset *)apr_pmemdup(mp, orig, sizeof(msre_actionset));
    if (copy == NULL) return NULL;
    copy->actions = apr_table_copy(mp, orig->actions);

    return copy;
}

/**
 * Merges two actionsets into one.
 */
msre_actionset *msre_actionset_merge(msre_engine *engine, msre_actionset *parent,
    msre_actionset *child, int inherit_by_default)
{
    msre_actionset *merged = NULL;
    const apr_array_header_t *tarr;
    const apr_table_entry_t *telts;
    int i;

    if (inherit_by_default == 0) {
        /* There is nothing to merge in this case. */
        return msre_actionset_copy(engine->mp, child);
    }

    /* Start with a copy of the parent configuration. */
    merged = msre_actionset_copy(engine->mp, parent);
    if (merged == NULL) return NULL;

    if (child == NULL) {
        /* The child actionset does not exist, hence
         * go with the parent one.
         */
        return merged;
    }

    /* First merge the hard-coded stuff. */

    /* Metadata */
    if (child->id != NOT_SET_P) merged->id = child->id;
    if (child->rev != NOT_SET_P) merged->rev = child->rev;
    if (child->msg != NOT_SET_P) merged->msg = child->msg;
    if (child->severity != NOT_SET) merged->severity = child->severity;
    if (child->phase != NOT_SET) merged->phase = child->phase;

    /* Flow */
    merged->is_chained = child->is_chained;
    if (child->skip_count != NOT_SET) merged->skip_count = child->skip_count;

    /* Disruptive */
    if (child->intercept_action != NOT_SET) {
        merged->intercept_action = child->intercept_action;
        merged->intercept_uri = child->intercept_uri;
    }

    if (child->intercept_status != NOT_SET) merged->intercept_status = child->intercept_status;
    if (child->intercept_pause != NOT_SET) merged->intercept_pause = child->intercept_pause;

    /* Other */
    if (child->auditlog != NOT_SET) merged->auditlog = child->auditlog;
    if (child->log != NOT_SET) merged->log = child->log;


    /* Now merge the actions. */

    tarr = apr_table_elts(child->actions);
    telts = (const apr_table_entry_t*)tarr->elts;
    for (i = 0; i < tarr->nelts; i++) {
        msre_action *action = (msre_action *)telts[i].val;
        if (action->metadata->cardinality == ACTION_CARDINALITY_ONE) {
            apr_table_setn(merged->actions, action->metadata->name, (void *)action);
        } else {
            apr_table_addn(merged->actions, action->metadata->name, (void *)action);
        }
    }

    return merged;
}

/**
 * Creates an actionset that contains a default list of actions.
 */
msre_actionset *msre_actionset_create_default(msre_engine *engine) {
    char  *my_error_msg = NULL;
    return msre_actionset_create(engine,
        "log,auditlog,deny,status:403,phase:2,t:lowercase,t:replaceNulls,t:compressWhitespace",
        &my_error_msg);
}

/**
 * Sets the default values for the hard-coded actionset configuration.
 */
static void msre_actionset_set_defaults(msre_actionset *actionset) {
    /* Metadata */
    if (actionset->id == NOT_SET_P) actionset->id = NULL;
    if (actionset->rev == NOT_SET_P) actionset->rev = NULL;
    if (actionset->msg == NOT_SET_P) actionset->msg = NULL;
    if (actionset->phase == NOT_SET) actionset->phase = 2;
    if (actionset->severity == -1); /* leave at -1 */

    /* Flow */
    if (actionset->is_chained == NOT_SET) actionset->is_chained = 0;
    if (actionset->skip_count == NOT_SET) actionset->skip_count = 0;

    /* Disruptive */
    if (actionset->intercept_action == NOT_SET) actionset->intercept_action = ACTION_NONE;
    if (actionset->intercept_uri == NOT_SET_P) actionset->intercept_uri = NULL;
    if (actionset->intercept_status == NOT_SET) actionset->intercept_status = 403;
    if (actionset->intercept_pause == NOT_SET) actionset->intercept_pause = 0;

    /* Other */
    if (actionset->auditlog == NOT_SET) actionset->auditlog = 1;
    if (actionset->log == NOT_SET) actionset->log = 1;
}

/* -- Engine functions ----------------------------------------------------- */

/**
 * Creates a new engine instance.
 */
msre_engine *msre_engine_create(apr_pool_t *parent_pool) {
    msre_engine *engine;
    apr_pool_t *mp;

    /* Create new memory pool */
    if (apr_pool_create(&mp, parent_pool) != APR_SUCCESS) return NULL;

    /* Init fields */
    engine = apr_pcalloc(mp, sizeof(msre_engine));
    if (engine == NULL) return NULL;
    engine->mp = mp;
    engine->tfns = apr_table_make(mp, 25);
    if (engine->tfns == NULL) return NULL;
    engine->operators = apr_table_make(mp, 25);
    if (engine->operators == NULL) return NULL;
    engine->variables = apr_table_make(mp, 25);
    if (engine->variables == NULL) return NULL;
    engine->actions = apr_table_make(mp, 25);
    if (engine->actions == NULL) return NULL;

    return engine;    
}

/**
 * Destroys an engine instance, releasing the consumed memory.
 */
void msre_engine_destroy(msre_engine *engine) {
    /* Destroyed automatically by the parent pool.
     * apr_pool_destroy(engine->mp);
     */
}


/* -- Recipe functions ----------------------------------------------------- */

#define NEXT_CHAIN  1
#define NEXT_RULE   2
#define SKIP_RULES  3

/**
 * Default implementation of the ruleset phase processing; it processes
 * the rules in the ruleset attached to the currently active
 * transaction phase.
 */
apr_status_t msre_ruleset_process_phase(msre_ruleset *ruleset, modsec_rec *msr) {
    apr_array_header_t *arr = NULL;
    msre_rule **rules;
    apr_status_t rc;
    int i, mode, skip;

    /* First determine which set of rules we need to use. */
    switch (msr->phase) {
        case PHASE_REQUEST_HEADERS :
            arr = ruleset->phase_request_headers;
            break;
        case PHASE_REQUEST_BODY :
            arr = ruleset->phase_request_body;
            break;
        case PHASE_RESPONSE_HEADERS :
            arr = ruleset->phase_response_headers;
            break;
        case PHASE_RESPONSE_BODY :
            arr = ruleset->phase_response_body;
            break;
        case PHASE_LOGGING :
            arr = ruleset->phase_logging;
            break;
        default :
            /* ENH Log a warning message here. */
            return -1;
    }

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "This phase consists of %i rule(s).", arr->nelts);
    }

    /* Loop through the rules in the selected set. */
    skip = 0;
    mode = NEXT_RULE;
    rules = (msre_rule **)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msre_rule *rule = rules[i];

        /* NEXT_CHAIN is used when one of the rules in a chain
         * fails to match and then we need to skip the remaining
         * rules in that chain in order to get to the next
         * rule that can execute.
         */
        if (mode == NEXT_CHAIN) {
            if (rule->actionset->is_chained == 0) {
                mode = NEXT_RULE;
            }

            /* Go to the next rule. */
            continue;
        }

        /* If we are here that means the mode is NEXT_RULE, which
         * then means we have done processing any chains. However,
         * if the "skip" parameter is set we need to skip over.
         */
        if ((mode == NEXT_RULE)&&(skip > 0)) {
            /* Decrement the skip counter by one. */
            skip--;

            /* If the current rule is part of a chain then
             * we need to skip over the entire chain. Thus
             * we change the mode to NEXT_CHAIN. The skip
             * counter will not decrement as we are moving
             * over the rules belonging to the chain.
             */
            if (rule->actionset->is_chained) {
                mode = NEXT_CHAIN;
            }

            /* Go to the next rule. */
            continue;
        }

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Recipe: Invoking rule %x.", rule);
        }

        rc = msre_rule_process(rule, msr);

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Rule returned %i.", rc);
        }

        if (rc == RULE_NO_MATCH) {
            if (rule->actionset->is_chained) {
                /* If the current rule is part of a chain then
                 * we need to skip over all the rules in the chain.
                 */
                mode = NEXT_CHAIN;
                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "No match, chained -> mode NEXT_CHAIN.");
                }
            } else {
                /* This rule is not part of a chain so we simply
                 * move to the next rule.
                 */
                mode = NEXT_RULE;
                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "No match, not chained -> mode NEXT_RULE.");
                }
            }
        }
        else
        if (rc == RULE_MATCH) {
            if (msr->was_intercepted) {
                /* If the transaction was intercepted we will
                 * go back. Do note that we are relying on the
                 * rule to know if it is a part of a chain and
                 * not intercept if it is.
                 */
                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "Match, intercepted -> returning.");
                }
                return 1;
            }

            /* We had a match but the transaction was not
             * intercepted. In that case we proceed with the
             * next rule...
             */
            mode = NEXT_RULE;
            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Match -> mode NEXT_RULE.");
            }

            /* ...unless we need to skip, in which case we
             * determine how many rules/chains we need to
             * skip and configure the counter accordingly.
             */
            if (rule->actionset->is_chained == 0) {
                if (rule->chain_starter != NULL) {
                    if (rule->chain_starter->actionset->skip_count > 0) {
                        skip = rule->chain_starter->actionset->skip_count;
                        if (msr->txcfg->debuglog_level >= 4) {
                            msr_log(msr, 4, "Skipping %i rules/chains (from a chain).", skip);
                        }
                    }
                }
                else if (rule->actionset->skip_count > 0) {
                    skip = rule->actionset->skip_count;
                    if (msr->txcfg->debuglog_level >= 4) {
                        msr_log(msr, 4, "Skipping %i rules/chains.", skip);
                    }
                }
            }
        }
        else {
            msr_log(msr, 1, "Unknown rule processing return code: %i.", rc);
            return -1;
        }
    }    

    /* ENH warn if chained rules are missing. */

    return 0;
}

/**
 * Creates a ruleset that will be handled by the default
 * implementation.
 */
msre_ruleset *msre_ruleset_create(msre_engine *engine, apr_pool_t *mp) {
    msre_ruleset *ruleset;

    ruleset = apr_pcalloc(mp, sizeof(msre_ruleset));
    if (ruleset == NULL) return NULL;
    ruleset->mp = mp;
    ruleset->engine = engine;

    ruleset->phase_request_headers = apr_array_make(ruleset->mp, 25, sizeof(const msre_rule *));
    ruleset->phase_request_body = apr_array_make(ruleset->mp, 25, sizeof(const msre_rule *));
    ruleset->phase_response_headers = apr_array_make(ruleset->mp, 25, sizeof(const msre_rule *));
    ruleset->phase_response_body = apr_array_make(ruleset->mp, 25, sizeof(const msre_rule *));
    ruleset->phase_logging = apr_array_make(ruleset->mp, 25, sizeof(const msre_rule *));

    return ruleset;
}

/**
 * Adds one rule to the given phase of the ruleset.
 */
int msre_ruleset_rule_add(msre_ruleset *ruleset, msre_rule *rule, int phase) {
    apr_array_header_t *arr = NULL;

    switch (phase) {
        case PHASE_REQUEST_HEADERS :
            arr = ruleset->phase_request_headers;
            break;
        case PHASE_REQUEST_BODY :
            arr = ruleset->phase_request_body;
            break;
        case PHASE_RESPONSE_HEADERS :
            arr = ruleset->phase_response_headers;
            break;
        case PHASE_RESPONSE_BODY :
            arr = ruleset->phase_response_body;
            break;
        case PHASE_LOGGING :
            arr = ruleset->phase_logging;
            break;
        default :
            return -1;
    }

    /* ENH verify the rule's use of targets is consistent with
     * the phase it selected to run at.
     */

    msre_actionset_set_defaults(rule->actionset);
    *(const msre_rule **)apr_array_push(arr) = rule;

    return 1;
}

static int msre_ruleset_phase_rule_remove_with_exception(msre_ruleset *ruleset, rule_exception *re,
    apr_array_header_t *phase_arr)
{
    msre_rule **rules;
    int i, j, mode, removed_count;

    j = 0;
    mode = 0;
    removed_count = 0;
    rules = (msre_rule **)phase_arr->elts;
    for (i = 0; i < phase_arr->nelts; i++) {
        msre_rule *rule = (msre_rule *)rules[i];

        if (mode == 0) { /* Looking for next rule. */
            int remove_rule = 0;

            switch(re->type) {
                case RULE_EXCEPTION_REMOVE_ID :
                    if ((rule->actionset != NULL)&&(rule->actionset->id != NULL)) {
                        int ruleid = atoi(rule->actionset->id);

                        if (rule_id_in_range(ruleid, re->param)) {
                            remove_rule = 1;
                        }
                    }

                    break;

                case RULE_EXCEPTION_REMOVE_MSG :
                    if ((rule->actionset != NULL)&&(rule->actionset->msg != NULL)) {
                        char *my_error_msg = NULL;

                        int rc = msc_regexec(re->param_data,
                            rule->actionset->msg, strlen(rule->actionset->msg),
                            &my_error_msg);
                        if (rc >= 0) {
                            remove_rule = 1;
                        }
                    }

                    break;
            }

            if (remove_rule) {
                /* Do not increment j. */
                removed_count++;
                if (rule->actionset->is_chained) mode = 2; /* Remove rules in this chain. */
            } else {
                if (rule->actionset->is_chained) mode = 1; /* Keep rules in this chain. */
                rules[j++] = rules[i];
            }            
        } else { /* Handling rule that is part of a chain. */
            if (mode == 2) { /* We want to remove the rule. */
                /* Do not increment j. */
                removed_count++;
            } else {
                rules[j++] = rules[i];
            }

            if ((rule->actionset == NULL)||(rule->actionset->is_chained == 0)) mode = 0;
        }
    }

    /* Update the number of rules in the array. */
    phase_arr->nelts -= removed_count;

    return 0;
}

/**
 * Removes from the ruleset all rules that match the given exception.
 */
int msre_ruleset_rule_remove_with_exception(msre_ruleset *ruleset, rule_exception *re) {
    int count = 0;

    if (ruleset == NULL) return 0;

    count += msre_ruleset_phase_rule_remove_with_exception(ruleset, re, ruleset->phase_request_headers);
    count += msre_ruleset_phase_rule_remove_with_exception(ruleset, re, ruleset->phase_request_body);
    count += msre_ruleset_phase_rule_remove_with_exception(ruleset, re, ruleset->phase_response_headers);
    count += msre_ruleset_phase_rule_remove_with_exception(ruleset, re, ruleset->phase_response_body);

    return count;    
}


/* -- Rule functions ------------------------------------------------------- */

/**
 * Returns the name of the supplied severity level.
 */
static const char *msre_format_severity(int severity) {
    static const char *const severities[] = {
        "EMERGENCY",
        "ALERT",
        "CRITICAL",
        "ERROR",
        "WARNING",
        "NOTICE",
        "INFO",
        "DEBUG",
        NULL,
    };

    if ((severity >= 0)&&(severity <= 7)) {
        return severities[severity];
    }
    else {
        return "(invalid value)";
    }
}

/**
 * Creates a string containing the metadata of the supplied rule.
 */
char *msre_format_metadata(modsec_rec *msr, msre_actionset *actionset) {
    const apr_array_header_t *tarr;
    const apr_table_entry_t *telts;
    char *id = "";
    char *rev = "";
    char *msg = "";
    char *severity = "";
    char *tags = "";
    int k;

    if (actionset == NULL) return "";

    if (actionset->id != NULL) id = apr_psprintf(msr->mp, " [id \"%s\"]",
        log_escape(msr->mp, actionset->id));
    if (actionset->rev != NULL) rev = apr_psprintf(msr->mp, " [rev \"%s\"]",
        log_escape(msr->mp, actionset->rev));
    if (actionset->msg != NULL) {
        /* Expand variables in the message string. */
        msc_string *var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
        var->value = (char *)actionset->msg;
        var->value_len = strlen(actionset->msg);
        expand_macros(msr, var, NULL, msr->mp);

        msg = apr_psprintf(msr->mp, " [msg \"%s\"]",
            log_escape_ex(msr->mp, var->value, var->value_len));
    }
    if ((actionset->severity >= 0)&&(actionset->severity <= 7)) {
        severity = apr_psprintf(msr->mp, " [severity \"%s\"]",
            msre_format_severity(actionset->severity));
    }

    /* Extract rule tags from the action list. */
    tarr = apr_table_elts(actionset->actions);
    telts = (const apr_table_entry_t*)tarr->elts;

    for (k = 0; k < tarr->nelts; k++) {
        msre_action *action = (msre_action *)telts[k].val;
        if (strcmp(telts[k].key, "tag") == 0) {
            tags = apr_psprintf(msr->mp, "%s [tag \"%s\"]", tags,
                log_escape(msr->mp, action->param));
        }
    }
    
    return apr_pstrcat(msr->mp, id, rev, msg, severity, tags, NULL);
}

/**
 * Assembles a new rule using the strings that contain a list
 * of targets (variables), argumments, and actions.
 */
msre_rule *msre_rule_create(msre_ruleset *ruleset, const char *targets,
    const char *args, const char *actions, char **error_msg)
{
    msre_rule *rule;
    char *my_error_msg;
    const char *argsp;
    int rc;

    if (error_msg == NULL) return NULL;
    *error_msg = NULL;

    rule = (msre_rule *)apr_pcalloc(ruleset->mp, sizeof(msre_rule));
    if (rule == NULL) return NULL;
    rule->ruleset = ruleset;
    rule->targets = apr_array_make(ruleset->mp, 10, sizeof(const msre_var *));

    /* Parse targets */
    rc = msre_parse_targets(ruleset, targets, rule->targets, &my_error_msg);
    if (rc < 0) {
        *error_msg = apr_psprintf(ruleset->mp, "Error creating rule: %s", my_error_msg);
        return NULL;
    }

    /* Parse args */
    argsp = args;

    /* Is negation used? */    
    if (*argsp == '!') {
        rule->op_negated = 1;
        argsp++;
        while((isspace(*argsp))&&(*argsp != '\0')) argsp++;
    }

    /* Is the operator explicitly selected? */
    if (*argsp != '@') {
        /* Go with a regular expression. */
        rule->op_name = "rx";
        rule->op_param = argsp;
    } else  {
        /* Explicitly selected operator. */
        char *p = (char *)(argsp + 1);
        while((!isspace(*p))&&(*p != '\0')) p++;
        rule->op_name = apr_pstrmemdup(ruleset->mp, argsp + 1, p - (argsp + 1));
        while(isspace(*p)) p++; /* skip over the whitespace at the end*/
        rule->op_param = p; /* IMP1 So we always have a parameter even when it's empty? */
    }

    /* Find the operator. */
    rule->op_metadata = msre_engine_op_resolve(ruleset->engine, rule->op_name);
    if (rule->op_metadata == NULL) {
        *error_msg = apr_psprintf(ruleset->mp,
            "Error creating rule: Failed to resolve operator: %s", rule->op_name);
        return NULL;
    }

    /* Initialise & validate parameter */
    if (rule->op_metadata->param_init != NULL) {
        if (rule->op_metadata->param_init(rule, &my_error_msg) <= 0) {
            *error_msg = apr_psprintf(ruleset->mp, "Error creating rule: %s", my_error_msg);
            return NULL;
        }
    }

    /* Parse actions */
    if (actions != NULL) {
        /* Create per-rule actionset */
        rule->actionset = msre_actionset_create(ruleset->engine, actions, &my_error_msg);
        if (rule->actionset == NULL) {
            *error_msg = apr_psprintf(ruleset->mp, "Error parsing actions: %s", my_error_msg);
            return NULL;
        }
    }

    return rule;
}

/**
 * Perform non-disruptive actions associated with the provided actionset.
 */
static void msre_perform_nondisruptive_actions(modsec_rec *msr, msre_rule *rule,
    msre_actionset *actionset, apr_pool_t *mptmp)
{
    const apr_array_header_t *tarr;
    const apr_table_entry_t *telts;
    int i;

    tarr = apr_table_elts(actionset->actions);
    telts = (const apr_table_entry_t*)tarr->elts;
    for (i = 0; i < tarr->nelts; i++) {
        msre_action *action = (msre_action *)telts[i].val;
        if (action->metadata->type == ACTION_NON_DISRUPTIVE) {
            if (action->metadata->execute != NULL) {
                action->metadata->execute(msr, mptmp, rule, action);
            }
        }
    }
}

/**
 * Perform the disruptive actions associated with the given actionset.
 */
static void msre_perform_disruptive_actions(modsec_rec *msr, msre_rule *rule,
    msre_actionset *actionset, apr_pool_t *mptmp, const char *message)
{
    const apr_array_header_t *tarr;
    const apr_table_entry_t *telts;
    int i;

    /* Execute the disruptive actions. Do note that this does
     * not mean the request will be interrupted straight away. All
     * disruptive actions need to do here is update the information
     * that will be used to act later.
     */
    tarr = apr_table_elts(actionset->actions);
    telts = (const apr_table_entry_t*)tarr->elts;
    for (i = 0; i < tarr->nelts; i++) {
        msre_action *action = (msre_action *)telts[i].val;
        if (action->metadata->type == ACTION_DISRUPTIVE) {
            if (action->metadata->execute != NULL) {
                action->metadata->execute(msr, mptmp, rule, action);
            }
        }
    }

    /* If "noauditlog" was used do not mark the transaction for audit logging. */
    if (actionset->auditlog == 1) {
        msr->is_relevant++;
    }

    /* We only do stuff when in ONLINE mode. In all other
     * cases we only emit warnings.
     */
    if ((msr->phase == PHASE_LOGGING)
        || (msr->txcfg->is_enabled == MODSEC_DETECTION_ONLY)
        || (msr->modsecurity->processing_mode == MODSEC_OFFLINE)
        || (actionset->intercept_action == ACTION_NONE))
    {
        /* If "nolog" was used log at a higher level. */
        msc_alert(msr, (actionset->log == 0 ? 4 : 2), actionset,
            "Warning.", message);
        return;
    }

    /* Signal to the engine we need to intercept this
     * transaction, and rememer the rule that caused it.
     */
    msr->was_intercepted = 1;
    msr->intercept_phase = msr->phase;
    msr->intercept_actionset = actionset;
    msr->intercept_message = message;
}

/**
 * Invokes the rule operator against the given value.
 */
static int execute_operator(msre_var *var, msre_rule *rule, modsec_rec *msr,
    msre_actionset *acting_actionset, apr_pool_t *mptmp)
{
    apr_time_t time_before_regex = 0;
    char *my_error_msg = NULL;
    int rc;

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Executing operator %s%s with param \"%s\" against %s.",
            (rule->op_negated ? "!" : ""), rule->op_name,
            log_escape(msr->mp, rule->op_param), var->name);
    }

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "Target value: %s", log_escape_nq_ex(msr->mp, var->value,
            var->value_len));
    }
        
    if (msr->txcfg->debuglog_level >= 4) {
        time_before_regex = apr_time_now();
    }

    rc = rule->op_metadata->execute(msr, rule, var, &my_error_msg);

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Operator completed in %" APR_TIME_T_FMT " usec.",
            (apr_time_now() - time_before_regex));
    }

    if (rc < 0) {
        msr_log(msr, 4, "Operator error: %s", my_error_msg);
        return -1;
    }

    if (((rc == 0)&&(rule->op_negated == 0))||((rc == 1)&&(rule->op_negated == 1))) {
        /* No match, do nothing. */
        return RULE_NO_MATCH;
    }
    else {
        /* Match. */

        if (rc == 0) {
            /* Operator did not match so we need to provide a message. */
            my_error_msg = apr_psprintf(msr->mp, "Match of \"%s %s\" against \"%s\" required.",
                log_escape(msr->mp, rule->op_name), log_escape(msr->mp, rule->op_param),
                log_escape(msr->mp, var->name));
        }

        msr->matched_var = apr_pstrdup(msr->mp, var->name);

        /* Perform non-disruptive actions. */
        msre_perform_nondisruptive_actions(msr, rule, rule->actionset, mptmp);

        /* Perform disruptive actions, but only if
         * this rule is not part of a chain.
         */
        if (rule->actionset->is_chained == 0) {
            msre_perform_disruptive_actions(msr, rule, acting_actionset, mptmp, my_error_msg);
        }

        return RULE_MATCH;
    }
}

/**
 * Executes rule against the given transaction.
 */
apr_status_t msre_rule_process(msre_rule *rule, modsec_rec *msr) {
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    msre_actionset *acting_actionset = NULL;
    msre_var **targets = NULL;
    apr_pool_t *mptmp = NULL;
    apr_table_t *tartab = NULL;
    apr_table_t *vartab = NULL;
    int i, rc, match_count = 0;
    int invocations = 0;
    int multi_match = 0;    

    /* Choose the correct metadata/disruptive action actionset. */
    acting_actionset = rule->actionset;
    if (rule->chain_starter != NULL) {
        acting_actionset = rule->chain_starter->actionset;
    }

    /* Configure recursive matching. */
    if (apr_table_get(rule->actionset->actions, "multiMatch") != NULL) {
        multi_match = 1;
    }

    /* Use a fresh memory sub-pool for processing each rule */
    if (msr->msc_rule_mptmp == NULL) {
        if (apr_pool_create(&msr->msc_rule_mptmp, msr->mp) != APR_SUCCESS) {
            return -1;
        }
        mptmp = msr->msc_rule_mptmp;
    }
    else {
        mptmp = msr->msc_rule_mptmp;
        apr_pool_clear(mptmp);
    }

    tartab = apr_table_make(mptmp, 24);
    if (tartab == NULL) return -1;
    vartab = apr_table_make(mptmp, 24);
    if (vartab == NULL) return -1;

    /* Expand variables to create a list of targets. */

    targets = (msre_var **)rule->targets->elts;
    for (i = 0; i < rule->targets->nelts; i++) {
        int j, list_count;

        apr_table_clear(vartab);

        /* ENH Introduce a new variable hook that would allow the code
         *     behind the variable to return the size of the collection
         *     without having to generate the variables.
         */

        /* Expand individual variables first. */
        list_count = targets[i]->metadata->generate(msr, targets[i], rule, vartab, mptmp);

        if (targets[i]->is_counting) {
            /* Count how many there are and just add the score to the target list. */
            msre_var *newvar = (msre_var *)apr_pmemdup(mptmp, targets[i], sizeof(msre_var));
            newvar->value = apr_psprintf(mptmp, "%i", list_count);
            newvar->value_len = strlen(newvar->value);
            apr_table_addn(tartab, newvar->name, (void *)newvar);
        } else {
            /* And either add them or remove from the final target list. */
            arr = apr_table_elts(vartab);
            te = (apr_table_entry_t *)arr->elts;
            for(j = 0; j < arr->nelts; j++) {
                if (targets[i]->is_negated == 0) {
                    apr_table_addn(tartab, te[j].key, te[j].val);
                } else {
                    apr_table_unset(tartab, te[j].key);
                }
            }
        }
    }

    /* Loop through targets on the final target list,
     * perform transformations as necessary, and invoke
     * the operator.
     */

    arr = apr_table_elts(tartab);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        int changed;
       
        /* Take one target. */ 
        msre_var *var = (msre_var *)te[i].val;

        /* Transform target. */
        {
            const apr_array_header_t *tarr;
            const apr_table_entry_t *telts;
            apr_table_t *normtab;
            int k;
            msre_action *action;
            msre_tfn_metadata *metadata;

            normtab = apr_table_make(mptmp, 10);
            if (normtab == NULL) return -1;
            tarr = apr_table_elts(rule->actionset->actions);
            telts = (const apr_table_entry_t*)tarr->elts;

            /* Build the final list of transformation functions. */
            for (k = 0; k < tarr->nelts; k++) {
                action = (msre_action *)telts[k].val;
                if (strcmp(telts[k].key, "t") == 0) {
                    if (strcmp(action->param, "none") == 0) {
                        apr_table_clear(normtab);
                        continue;
                    }

                    if (action->param_plusminus == NEGATIVE_VALUE) {
                        apr_table_unset(normtab, action->param);
                    } else {
                        apr_table_addn(normtab, action->param, (void *)action);
                    }
                }
            }

            /* Perform transformations. */

            tarr = apr_table_elts(normtab);

            /* Make a copy of the variable value so that
             * we can change it in-place.
             */
            if (tarr->nelts) {
                var->value = apr_pstrmemdup(mptmp, var->value, var->value_len);
                /* var->value_len remains the same */
            }

            /* Execute transformations in a loop. */

            changed = 1;
            telts = (const apr_table_entry_t*)tarr->elts;
            for (k = 0; k < tarr->nelts; k++) {
                char *rval = NULL;
                long int rval_length = -1;

                /* In multi-match mode we execute the operator
                 * once at the beginning and then once every
                 * time the variable is changed by the transformation
                 * function.
                 */
                if (multi_match && changed) {
                    invocations++;

                    rc = execute_operator(var, rule, msr, acting_actionset, mptmp);

                    if (rc < 0) {
                        return -1;
                    }

                    if (rc == RULE_MATCH) {
                        match_count++;
        
                        /* Return straight away if the transaction
                        * was intercepted - no need to process the remaining
                        * targets.
                        */
                        if (msr->was_intercepted) {
                            return RULE_MATCH;
                        }
                    }
                }

                /* Perform one transformation. */
                action = (msre_action *)telts[k].val;
                metadata = (msre_tfn_metadata *)action->param_data;

                rc = metadata->execute(mptmp, (unsigned char *)var->value, var->value_len,
                    &rval, &rval_length);
                if (rc < 0) {
                    return -1;
                }

                changed = rc;

                var->value = rval;
                var->value_len = rval_length;

                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "T (%i) %s: %s", rc, metadata->name,
                        log_escape_nq_ex(mptmp, var->value, var->value_len));
                }
            }
        }

        /* Execute operator if multi-matching is not enabled,
         * or if it is and we need to process the result of the
         * last transformation.
         */
        if (!multi_match || changed) {
            invocations++;

            rc = execute_operator(var, rule, msr, acting_actionset, mptmp);

            if (rc < 0) {
                return -1;
            }

            if (rc == RULE_MATCH) {
                match_count++;
        
                /* Return straight away if the transaction
                 * was intercepted - no need to process the remaining
                 * targets.
                 */
                if (msr->was_intercepted) {
                    return RULE_MATCH;
                }
            }
        }
    }


    return (match_count ? RULE_MATCH : RULE_NO_MATCH);
}

/**
 * Checks whether the given rule ID is in the given range.
 */
int rule_id_in_range(int ruleid, const char *range) {
    char *p = NULL, *saveptr = NULL;
    char *data = NULL;

    if (range == NULL) return 0;
    data = strdup(range);
    if (data == NULL) return 0;
    
    p = apr_strtok(data, ",", &saveptr);
    while(p != NULL) {
        char *s = strstr(p, "-");
        if (s == NULL) {
            if (ruleid == atoi(p)) {
                free(data);
                return 1;
            }
        } else {
            int start = atoi(p);
            int end = atoi(s + 1);
            if ((ruleid >= start)&&(ruleid <= end)) {
                free(data);
                return 1;
            }
        }
        p = apr_strtok(NULL, ",", &saveptr);
    }

    free(data);

    return 0;
}
