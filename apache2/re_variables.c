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

#include "http_core.h"

#include "modsecurity.h"
#include "apache2.h"
#include "re.h"
#include "msc_util.h"

#include "libxml/xpathInternals.h"

/**
 * Generates a variable from a string and a length.
 */
static int var_simple_generate_ex(msre_var *var, apr_table_t *vartab, apr_pool_t *mptmp,
    const char *value, int value_len)
{
    msre_var *rvar = NULL;

    if (value == NULL) return 0;

    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = value;
    rvar->value_len = value_len;
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/**
 * Generates a variable from a NULL-terminated string.
 */
static int var_simple_generate(msre_var *var, apr_table_t *vartab, apr_pool_t *mptmp,
    const char *value)
{
    if (value == NULL) return 0;
    return var_simple_generate_ex(var, vartab, mptmp, value, strlen(value));
}

/**
 * Validate that a target parameter is valid. We only need to take
 * care of the case when the parameter is a regular expression.
 */
static char *var_generic_list_validate(msre_ruleset *ruleset, msre_var *var) {
    /* It's OK if there's no parameter. */
    if (var->param == NULL) return NULL;

    /* Is it a regular expression? */
    if ((strlen(var->param) > 2)&&(var->param[0] == '/')
        &&(var->param[strlen(var->param) - 1] == '/'))
    { /* Regex. */
        msc_regex_t *regex = NULL;
        const char *errptr = NULL;
        const char *pattern = NULL;
        int erroffset;

        pattern = apr_pstrmemdup(ruleset->mp, var->param + 1, strlen(var->param + 1) - 1);
        if (pattern == NULL) return FATAL_ERROR;

        regex = msc_pregcomp(ruleset->mp, pattern, PCRE_DOTALL | PCRE_CASELESS | PCRE_DOLLAR_ENDONLY, &errptr, &erroffset);
        if (regex == NULL) {
            return apr_psprintf(ruleset->mp, "Error compiling pattern (offset %d): %s",
                erroffset, errptr);
        }

        /* Store the compiled regex for later. */
        var->param_data = regex;
    }

    /* Simple string */
    return NULL;
}

/* Custom parameter validation functions */

/* ARGS */

static int var_args_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    /* Loop through the arguments. */
    arr = apr_table_elts(msr->arguments);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_arg *arg = (msc_arg *)te[i].val;
        int match = 0;

        /* Figure out if we want to include this argument. */
        if (var->param == NULL) match = 1; /* Unconditional inclusion. */
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                /* Run the regex against the argument name. */
                if (!(msc_regexec((msc_regex_t *)var->param_data, arg->name,
                    arg->name_len, &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(arg->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = arg->value;
            rvar->value_len = arg->value_len;
            rvar->name = apr_psprintf(mptmp, "ARGS:%s", log_escape_nq_ex(mptmp, arg->name, arg->name_len));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* ARGS_COMBINED_SIZE */

static int var_args_combined_size_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    unsigned int combined_size = 0;
    int i;
    msre_var *rvar = NULL;

    arr = apr_table_elts(msr->arguments);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_arg *arg = (msc_arg *)te[i].val;
        combined_size += arg->name_len;
        combined_size += arg->value_len;
    }

    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = apr_psprintf(mptmp, "%u", combined_size);
    rvar->value_len = strlen(rvar->value);
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* ARGS_NAMES */

static int var_args_names_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    arr = apr_table_elts(msr->arguments);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_arg *arg = (msc_arg *)te[i].val;
        int match = 0;

        /* Figure out if we want to include this variable. */
        if (var->param == NULL) match = 1; /* Unconditional inclusion. */
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, arg->name,
                    arg->name_len, &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(arg->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = arg->name;
            rvar->value_len = arg->name_len;
            rvar->name = apr_psprintf(mptmp, "ARGS_NAMES:%s", log_escape_nq_ex(mptmp, arg->name, arg->name_len));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* ARGS_GET */

static int var_args_get_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    /* Loop through the arguments. */
    arr = apr_table_elts(msr->arguments);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_arg *arg = (msc_arg *)te[i].val;
        int match = 0;

        /* Only QUERY_STRING arguments */
        if (strcmp("QUERY_STRING", arg->origin) != 0) continue;

        /* Figure out if we want to include this argument. */
        if (var->param == NULL) match = 1; /* Unconditional inclusion. */
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                /* Run the regex against the argument name. */
                if (!(msc_regexec((msc_regex_t *)var->param_data, arg->name,
                    arg->name_len, &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(arg->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = arg->value;
            rvar->value_len = arg->value_len;
            rvar->name = apr_psprintf(mptmp, "ARGS_GET:%s", log_escape_nq_ex(mptmp, arg->name, arg->name_len));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* ARGS_GET_NAMES */

static int var_args_get_names_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    arr = apr_table_elts(msr->arguments);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_arg *arg = (msc_arg *)te[i].val;
        int match = 0;

        /* Only QUERY_STRING arguments */
        if (strcmp("QUERY_STRING", arg->origin) != 0) continue;

        /* Figure out if we want to include this variable. */
        if (var->param == NULL) match = 1; /* Unconditional inclusion. */
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, arg->name,
                    arg->name_len, &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(arg->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = arg->name;
            rvar->value_len = arg->name_len;
            rvar->name = apr_psprintf(mptmp, "ARGS_GET_NAMES:%s", log_escape_nq_ex(mptmp, arg->name, arg->name_len));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* ARGS_POST */

static int var_args_post_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    /* Loop through the arguments. */
    arr = apr_table_elts(msr->arguments);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_arg *arg = (msc_arg *)te[i].val;
        int match = 0;

        /* Only BODY arguments */
        if (strcmp("BODY", arg->origin) != 0) continue;

        /* Figure out if we want to include this argument. */
        if (var->param == NULL) match = 1; /* Unconditional inclusion. */
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                /* Run the regex against the argument name. */
                if (!(msc_regexec((msc_regex_t *)var->param_data, arg->name,
                    arg->name_len, &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(arg->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = arg->value;
            rvar->value_len = arg->value_len;
            rvar->name = apr_psprintf(mptmp, "ARGS_POST:%s", log_escape_nq_ex(mptmp, arg->name, arg->name_len));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* ARGS_POST_NAMES */

static int var_args_post_names_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    arr = apr_table_elts(msr->arguments);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_arg *arg = (msc_arg *)te[i].val;
        int match = 0;

        /* Only BODY arguments */
        if (strcmp("BODY", arg->origin) != 0) continue;

        /* Figure out if we want to include this variable. */
        if (var->param == NULL) match = 1; /* Unconditional inclusion. */
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, arg->name,
                    arg->name_len, &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(arg->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = arg->name;
            rvar->value_len = arg->name_len;
            rvar->name = apr_psprintf(mptmp, "ARGS_POST_NAMES:%s", log_escape_nq_ex(mptmp, arg->name, arg->name_len));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* RULE */

static int var_rule_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_actionset *actionset = NULL;

    if (rule == NULL) return 0;

    actionset = rule->actionset;
    if (rule->chain_starter != NULL) actionset = rule->chain_starter->actionset;

    if ((strcasecmp(var->param, "id") == 0)&&(actionset->id != NULL)) {
        return var_simple_generate(var, vartab, mptmp, actionset->id);
    } else
    if ((strcasecmp(var->param, "rev") == 0)&&(actionset->rev != NULL)) {
        return var_simple_generate(var, vartab, mptmp, actionset->rev);
    } else
    if ((strcasecmp(var->param, "severity") == 0)&&(actionset->severity != -1)) {
        char *value = apr_psprintf(mptmp, "%d", actionset->severity);
        return var_simple_generate(var, vartab, mptmp, value);
    } else
    if ((strcasecmp(var->param, "msg") == 0)&&(actionset->msg != NULL)) {
        return var_simple_generate(var, vartab, mptmp, actionset->msg);
    } else
    if ((strcasecmp(var->param, "logdata") == 0)&&(actionset->logdata != NULL)) {
        return var_simple_generate(var, vartab, mptmp, actionset->logdata);
    } else
    if ((strcasecmp(var->param, "ver") == 0)&&(actionset->version != NULL)) {
        return var_simple_generate(var, vartab, mptmp, actionset->version);
    } else
    if ((strcasecmp(var->param, "maturity") == 0)&&(actionset->maturity != -1)) {
        char *value = apr_psprintf(mptmp, "%d", actionset->maturity);
        return var_simple_generate(var, vartab, mptmp, value);
    } else
    if ((strcasecmp(var->param, "accuracy") == 0)&&(actionset->accuracy != -1)) {
        char *value = apr_psprintf(mptmp, "%d", actionset->accuracy);
        return var_simple_generate(var, vartab, mptmp, value);
    }


    return 0;
}

/* ENV */

static char *var_env_validate(msre_ruleset *ruleset, msre_var *var) {
    if (var->param == NULL) {
        return apr_psprintf(ruleset->mp, "Parameter required for ENV.");
    }
    if ((strlen(var->param) > 2)&&(var->param[0] == '/')
        &&(var->param[strlen(var->param) - 1] == '/'))
    {
        return apr_psprintf(ruleset->mp, "Regular expressions not supported in ENV.");
    }
    return NULL;
}

static int var_env_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    char *value = get_env_var(msr->r, (char *)var->param);
    if (value != NULL) {
        return var_simple_generate(var, vartab, mptmp, value);
    }
    return 0;
}

/* REQUEST_URI_RAW */

static int var_request_uri_raw_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, msr->r->unparsed_uri);
}

/* UNIQUE_ID */

static int var_uniqueid_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    char *value = get_env_var(msr->r, "UNIQUE_ID");
    if (value != NULL) {
        return var_simple_generate(var, vartab, mptmp, value);
    }

    return 0;
}


/* REQUEST_URI */

static int var_request_uri_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp) /* dynamic */
{
    char *value = NULL;

    if (msr->r->parsed_uri.query == NULL) value = msr->r->parsed_uri.path;
    else value = apr_pstrcat(mptmp,  msr->r->parsed_uri.path, "?", msr->r->parsed_uri.query, NULL);

    return var_simple_generate(var, vartab, mptmp, value);
}

/* REQBODY_PROCESSOR */

static int var_reqbody_processor_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

    if (msr->msc_reqbody_processor == NULL) {
        rvar->value = apr_pstrdup(mptmp, "");
        rvar->value_len = 0;
    } else {
        rvar->value = apr_pstrdup(mptmp, msr->msc_reqbody_processor);
        rvar->value_len = strlen(rvar->value);
    }

    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* REQBODY_ERROR */

static int var_reqbody_processor_error_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

    rvar->value = apr_psprintf(mptmp, "%d", msr->msc_reqbody_error);
    rvar->value_len = strlen(rvar->value);
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* REQBODY_ERROR_MSG */

static int var_reqbody_processor_error_msg_generate(modsec_rec *msr, msre_var *var,
    msre_rule *rule, apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

    if (msr->msc_reqbody_error_msg == NULL) {
        rvar->value = apr_pstrdup(mptmp, "");
        rvar->value_len = 0;
    } else {
        rvar->value = apr_psprintf(mptmp, "%s", msr->msc_reqbody_error_msg);
        rvar->value_len = strlen(rvar->value);
    }

    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* XML */

static char *var_xml_validate(msre_ruleset *ruleset, msre_var *var) {
    /* It's OK if there's no parameter. */
    if (var->param == NULL) return NULL;

    /* ENH validate XPath expression in advance. */

    return NULL;
}

static int var_xml_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *tarr;
    const apr_table_entry_t *telts;
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;
    xmlNodeSetPtr nodes;
    const xmlChar* xpathExpr = NULL;
    int i, count;

    /* Is there an XML document tree at all? */
    if ((msr->xml == NULL)||(msr->xml->doc == NULL)) {
        /* Sorry, we've got nothing to give! */
        return 0;
    }

    if (var->param == NULL) {
        /* Invocation without an XPath expression makes sense
         * with functions that manipulate the document tree.
         */
        msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

        rvar->value = apr_pstrdup(mptmp, "[XML document tree]");
        rvar->value_len = strlen(rvar->value);
        apr_table_addn(vartab, rvar->name, (void *)rvar);

        return 1;
    }

    /* Process the XPath expression. */

    count = 0;
    xpathExpr = (const xmlChar*)var->param;

    xpathCtx = xmlXPathNewContext(msr->xml->doc);
    if (xpathCtx == NULL) {
        msr_log(msr, 1, "XML: Unable to create new XPath context.");
        return -1;
    }

    /* Look through the actionset of the associated rule
     * for the namespace information. Register them if any are found.
     */
    tarr = apr_table_elts(rule->actionset->actions);
    telts = (const apr_table_entry_t*)tarr->elts;
    for (i = 0; i < tarr->nelts; i++) {
        msre_action *action = (msre_action *)telts[i].val;

        if (strcasecmp(action->metadata->name, "xmlns") == 0) {
            char *prefix, *href;

            if (parse_name_eq_value(mptmp, action->param, &prefix, &href) < 0) return -1;
            if ((prefix == NULL)||(href == NULL)) return -1;

            if(xmlXPathRegisterNs(xpathCtx, (const xmlChar*)prefix, (const xmlChar*)href) != 0) {
                msr_log(msr, 1, "Failed to register XML namespace href \"%s\" prefix \"%s\".",
                    log_escape(mptmp, prefix), log_escape(mptmp, href));
                return -1;
            }

            msr_log(msr, 4, "Registered XML namespace href \"%s\" prefix \"%s\".",
                log_escape(mptmp, prefix), log_escape(mptmp, href));
        }
    }

    /* Initialise XPath expression. */
    xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
    if (xpathObj == NULL) {
        msr_log(msr, 1, "XML: Unable to evaluate xpath expression.");
        xmlXPathFreeContext(xpathCtx);
        return -1;
    }

    /* Evaluate XPath expression. */
    nodes = xpathObj->nodesetval;
    if (nodes == NULL) {
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        return 0;
    }

    /* Create one variable for each node in the result. */
    for(i = 0; i < nodes->nodeNr; i++) {
        msre_var *rvar = NULL;
        char *content = NULL;

        content = (char *)xmlNodeGetContent(nodes->nodeTab[i]);
        if (content != NULL) {
            rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
            rvar->value = apr_pstrdup(mptmp, content);
            xmlFree(content);
            rvar->value_len = strlen(rvar->value);
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
         }
    }

    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);

    return count;
}

/* WEBSERVER_ERROR_LOG */

static int var_webserver_error_log_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = NULL;
    int i, count = 0;

    for(i = 0; i < msr->error_messages->nelts; i++) {
        error_message_t *em = (((error_message_t **)msr->error_messages->elts)[i]);
        char *fem = NULL;

        fem = format_error_log_message(mptmp, em);
        if (fem != NULL) {
            rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
            rvar->value = apr_pstrdup(mptmp, fem);
            rvar->value_len = strlen(rvar->value);
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
static int var_useragent_ip_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, msr->useragent_ip ? msr->useragent_ip : "0.0.0.0");
}
#endif

/* REMOTE_ADDR */

static int var_remote_addr_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, msr->remote_addr);
}

/* REMOTE_HOST */

static int var_remote_host_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const char *value1 = ap_get_remote_host(msr->r->connection, msr->r->per_dir_config,
        REMOTE_NAME, NULL);
    return var_simple_generate(var, vartab, mptmp, value1);
}

/* REMOTE_PORT */

static int var_remote_port_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    char *value = apr_psprintf(mptmp, "%u", msr->remote_port);
    return var_simple_generate(var, vartab, mptmp, value);
}

/* REMOTE_USER */

static int var_remote_user_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, msr->remote_user);
}

/* TX */

static int var_tx_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    arr = apr_table_elts(msr->tx_vars);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_string *str = (msc_string *)te[i].val;
        int match;

        /* Figure out if we want to include this variable. */
        match = 0;
        if (var->param == NULL) match = 1; /* Unconditional inclusion. */
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, str->name,
                    str->name_len, &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(str->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = str->value;
            rvar->value_len = str->value_len;
            rvar->name = apr_psprintf(mptmp, "TX:%s", log_escape_nq_ex(mptmp, str->name, str->name_len));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* GEO */

static int var_geo_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    arr = apr_table_elts(msr->geo_vars);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_string *str = (msc_string *)te[i].val;
        int match;

        /* Figure out if we want to include this variable. */
        match = 0;
        if (var->param == NULL) match = 1; /* Unconditional inclusion. */
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, str->name,
                    str->name_len, &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(str->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = str->value;
            rvar->value_len = str->value_len;
            rvar->name = apr_psprintf(mptmp, "GEO:%s", log_escape_nq_ex(mptmp, str->name, str->name_len));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* HIGHEST_SEVERITY */

static int var_highest_severity_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp,
                               apr_psprintf(mptmp, "%d", msr->highest_severity));
}

/* IP */

static int var_ip_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;
    apr_table_t *target_col = NULL;

    target_col = (apr_table_t *)apr_table_get(msr->collections, "ip");
    if (target_col == NULL) return 0;

    arr = apr_table_elts(target_col);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_string *str = (msc_string *)te[i].val;
        int match;

        /* Figure out if we want to include this variable. */
        match = 0;
        if (var->param == NULL) match = 1; /* Unconditional inclusion. */
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, str->name,
                    str->name_len, &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(str->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = str->value;
            rvar->value_len = str->value_len;
            rvar->name = apr_psprintf(mptmp, "IP:%s", log_escape_nq_ex(mptmp, str->name, str->name_len));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* MATCHED_VAR */

static int var_matched_var_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate_ex(var, vartab, mptmp,
                                  apr_pmemdup(mptmp,
                                      msr->matched_var->value,
                                      msr->matched_var->value_len),
                                  msr->matched_var->value_len);
}

/* MATCHED_VAR_NAME */

static int var_matched_var_name_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate_ex(var, vartab, mptmp,
                                  apr_pmemdup(mptmp,
                                      msr->matched_var->name,
                                      msr->matched_var->name_len),
                                  msr->matched_var->name_len);
}

/* SESSION */

static int var_session_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;
    apr_table_t *target_col = NULL;

    target_col = (apr_table_t *)apr_table_get(msr->collections, "session");
    if (target_col == NULL) return 0;

    arr = apr_table_elts(target_col);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_string *str = (msc_string *)te[i].val;
        int match;

        /* Figure out if we want to include this variable. */
        match = 0;
        if (var->param == NULL) match = 1; /* Unconditional inclusion. */
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, str->name,
                    str->name_len, &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(str->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = str->value;
            rvar->value_len = str->value_len;
            rvar->name = apr_psprintf(mptmp, "SESSION:%s", log_escape_nq_ex(mptmp, str->name, str->name_len));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* USER */

static int var_user_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;
    apr_table_t *target_col = NULL;

    target_col = (apr_table_t *)apr_table_get(msr->collections, "user");
    if (target_col == NULL) return 0;

    arr = apr_table_elts(target_col);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_string *str = (msc_string *)te[i].val;
        int match;

        /* Figure out if we want to include this variable. */
        match = 0;
        if (var->param == NULL) match = 1; /* Unconditional match. */
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, str->name,
                    str->name_len, &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(str->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = str->value;
            rvar->value_len = str->value_len;
            rvar->name = apr_psprintf(mptmp, "USER:%s", log_escape_nq_ex(mptmp, str->name, str->name_len));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* GLOBAL */

static int var_global_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;
    apr_table_t *target_col = NULL;

    target_col = (apr_table_t *)apr_table_get(msr->collections, "global");
    if (target_col == NULL) return 0;

    arr = apr_table_elts(target_col);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_string *str = (msc_string *)te[i].val;
        int match;

        /* Figure out if we want to include this variable. */
        match = 0;
        if (var->param == NULL) match = 1; /* Unconditional match. */
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, str->name,
                    str->name_len, &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(str->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = str->value;
            rvar->value_len = str->value_len;
            rvar->name = apr_psprintf(mptmp, "GLOBAL:%s", log_escape_nq_ex(mptmp, str->name, str->name_len));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* RESOURCE */

static int var_resource_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;
    apr_table_t *target_col = NULL;

    target_col = (apr_table_t *)apr_table_get(msr->collections, "resource");
    if (target_col == NULL) return 0;

    arr = apr_table_elts(target_col);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        msc_string *str = (msc_string *)te[i].val;
        int match;

        /* Figure out if we want to include this variable. */
        match = 0;
        if (var->param == NULL) match = 1; /* Unconditional match. */
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, str->name,
                    str->name_len, &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(str->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = str->value;
            rvar->value_len = str->value_len;
            rvar->name = apr_psprintf(mptmp, "RESOURCE:%s", log_escape_nq_ex(mptmp, str->name, str->name_len));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* FILES_TMPNAMES */

static int var_files_tmpnames_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    multipart_part **parts = NULL;
    int i, count = 0;

    if (msr->mpd == NULL) return 0;

    parts = (multipart_part **)msr->mpd->parts->elts;
    for(i = 0; i < msr->mpd->parts->nelts; i++) {
        if ((parts[i]->type == MULTIPART_FILE)&&(parts[i]->tmp_file_name != NULL)) {
            int match = 0;

            /* Figure out if we want to include this variable. */
            if (var->param == NULL) match = 1;
            else {
                if (var->param_data != NULL) { /* Regex. */
                    char *my_error_msg = NULL;
                    if (!(msc_regexec((msc_regex_t *)var->param_data, parts[i]->name,
                        strlen(parts[i]->name), &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
                } else { /* Simple comparison. */
                    if (strcasecmp(parts[i]->name, var->param) == 0) match = 1;
                }
            }

            /* If we had a match add this argument to the collection. */
            if (match) {
                msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

                rvar->value = parts[i]->tmp_file_name;
                rvar->value_len = strlen(rvar->value);
                rvar->name = apr_psprintf(mptmp, "FILES_TMPNAMES:%s",
                    log_escape_nq(mptmp, parts[i]->name));
                apr_table_addn(vartab, rvar->name, (void *)rvar);

                count++;
            }
        }
    }

    return count;
}

/* FILES */

static int var_files_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    multipart_part **parts = NULL;
    int i, count = 0;

    if (msr->mpd == NULL) return 0;

    parts = (multipart_part **)msr->mpd->parts->elts;
    for(i = 0; i < msr->mpd->parts->nelts; i++) {
        if (parts[i]->type == MULTIPART_FILE) {
            int match = 0;

            /* Figure out if we want to include this variable. */
            if (var->param == NULL) match = 1;
            else {
                if (var->param_data != NULL) { /* Regex. */
                    char *my_error_msg = NULL;
                    if (!(msc_regexec((msc_regex_t *)var->param_data, parts[i]->name,
                        strlen(parts[i]->name), &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
                } else { /* Simple comparison. */
                    if (strcasecmp(parts[i]->name, var->param) == 0) match = 1;
                }
            }

            /* If we had a match add this argument to the collection. */
            if (match) {
                msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

                rvar->value = parts[i]->filename;
                rvar->value_len = strlen(rvar->value);
                rvar->name = apr_psprintf(mptmp, "FILES:%s",
                    log_escape_nq(mptmp, parts[i]->name));
                apr_table_addn(vartab, rvar->name, (void *)rvar);

                count++;
            }
        }
    }

    return count;
}

/* FILES_SIZES */

static int var_files_sizes_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    multipart_part **parts = NULL;
    int i, count = 0;

    if (msr->mpd == NULL) return 0;

    parts = (multipart_part **)msr->mpd->parts->elts;
    for(i = 0; i < msr->mpd->parts->nelts; i++) {
        if (parts[i]->type == MULTIPART_FILE) {
            int match = 0;

            /* Figure out if we want to include this variable. */
            if (var->param == NULL) match = 1;
            else {
                if (var->param_data != NULL) { /* Regex. */
                    char *my_error_msg = NULL;
                    if (!(msc_regexec((msc_regex_t *)var->param_data, parts[i]->name,
                        strlen(parts[i]->name), &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
                } else { /* Simple comparison. */
                    if (strcasecmp(parts[i]->name, var->param) == 0) match = 1;
                }
            }

            /* If we had a match add this argument to the collection. */
            if (match) {
                msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

                rvar->value = apr_psprintf(mptmp, "%u", parts[i]->tmp_file_size);
                rvar->value_len = strlen(rvar->value);
                rvar->name = apr_psprintf(mptmp, "FILES_SIZES:%s",
                    log_escape_nq(mptmp, parts[i]->name));
                apr_table_addn(vartab, rvar->name, (void *)rvar);

                count++;
            }
        }
    }

    return count;
}

/* FILES_NAMES */

static int var_files_names_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    multipart_part **parts = NULL;
    int i, count = 0;

    if (msr->mpd == NULL) return 0;

    parts = (multipart_part **)msr->mpd->parts->elts;
    for(i = 0; i < msr->mpd->parts->nelts; i++) {
        if (parts[i]->type == MULTIPART_FILE) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = parts[i]->name;
            rvar->value_len = strlen(rvar->value);
            rvar->name = apr_psprintf(mptmp, "FILES_NAMES:%s",
                log_escape_nq_ex(mptmp, parts[i]->name, rvar->value_len));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* FILES_COMBINED_SIZE */

static int var_files_combined_size_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    multipart_part **parts = NULL;
    msre_var *rvar = NULL;
    unsigned int combined_size = 0;
    int i;

    if (msr->mpd != NULL) {
        parts = (multipart_part **)msr->mpd->parts->elts;
        for(i = 0; i < msr->mpd->parts->nelts; i++) {
            if (parts[i]->type == MULTIPART_FILE) {
                combined_size += parts[i]->tmp_file_size;
            }
        }
    }

    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = apr_psprintf(mptmp, "%u", combined_size);
    rvar->value_len = strlen(rvar->value);
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* MODSEC_BUILD */

static int var_modsec_build_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, modsec_build(mptmp));
}

/* MULTIPART_FILENAME */

static int var_multipart_filename_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, msr->multipart_filename);
}

/* MULTIPART_NAME */

static int var_multipart_name_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, msr->multipart_name);
}

/* MULTIPART_BOUNDARY_QUOTED */

static int var_multipart_boundary_quoted_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if ((msr->mpd != NULL)&&(msr->mpd->flag_boundary_quoted != 0)) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* MULTIPART_BOUNDARY_WHITESPACE */

static int var_multipart_boundary_whitespace_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if ((msr->mpd != NULL)&&(msr->mpd->flag_boundary_whitespace != 0)) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* MULTIPART_DATA_AFTER */

static int var_multipart_data_after_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if ((msr->mpd != NULL)&&(msr->mpd->flag_data_after != 0)) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* MULTIPART_DATA_BEFORE */

static int var_multipart_data_before_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if ((msr->mpd != NULL)&&(msr->mpd->flag_data_before != 0)) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* MULTIPART_HEADER_FOLDING */

static int var_multipart_header_folding_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if ((msr->mpd != NULL)&&(msr->mpd->flag_header_folding != 0)) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* MULTIPART_CRLF_LINE */

static int var_multipart_crlf_line_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if ((msr->mpd != NULL)&&(msr->mpd->flag_crlf_line != 0)) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* MULTIPART_CRLF_LF_LINES */

static int var_multipart_crlf_lf_lines_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if ((msr->mpd != NULL)&&(msr->mpd->flag_lf_line != 0)&&(msr->mpd->flag_crlf_line != 0)) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* MULTIPART_LF_LINE */

static int var_multipart_lf_line_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if ((msr->mpd != NULL)&&(msr->mpd->flag_lf_line != 0)) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* MULTIPART_MISSING_SEMICOLON */

static int var_multipart_missing_semicolon_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if ((msr->mpd != NULL)&&(msr->mpd->flag_missing_semicolon != 0)) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* MULTIPART_INVALID_PART */

static int var_multipart_invalid_part_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if ((msr->mpd != NULL)&&(msr->mpd->flag_invalid_part != 0)) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* MULTIPART_INVALID_QUOTING */

static int var_multipart_invalid_quoting_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if ((msr->mpd != NULL)&&(msr->mpd->flag_invalid_quoting != 0)) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* MULTIPART_INVALID_HEADER_FOLDING */

static int var_multipart_invalid_header_folding_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if ((msr->mpd != NULL)&&(msr->mpd->flag_invalid_header_folding != 0)) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* MULTIPART_FILE_LIMIT_EXCEEDED */

static int var_multipart_file_limit_exceeded_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if ((msr->mpd != NULL)&&(msr->mpd->flag_file_limit_exceeded != 0)) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* MULTIPART_STRICT_ERROR */

static int var_multipart_strict_error_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if (msr->mpd != NULL) {
        /* Respond positive if at least one of the multipart flags is raised. */
        if (  (msr->mpd->flag_error)
            ||(msr->mpd->flag_boundary_quoted != 0)
            ||(msr->mpd->flag_boundary_whitespace != 0)
            ||(msr->mpd->flag_data_before != 0)
            ||(msr->mpd->flag_data_after != 0)
            ||(msr->mpd->flag_header_folding != 0)
            ||(msr->mpd->flag_lf_line != 0)
            ||(msr->mpd->flag_missing_semicolon != 0)
            ||(msr->mpd->flag_invalid_quoting != 0)
            ||(msr->mpd->flag_invalid_part != 0)
            ||(msr->mpd->flag_invalid_header_folding != 0)
            ||(msr->mpd->flag_file_limit_exceeded != 0)
        ) {
            return var_simple_generate(var, vartab, mptmp, "1");
        }
    }

    return var_simple_generate(var, vartab, mptmp, "0");
}

/* MULTIPART_UNMATCHED_BOUNDARY */

static int var_multipart_unmatched_boundary_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if ((msr->mpd != NULL)&&(msr->mpd->flag_unmatched_boundary != 0)) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* URLENCODED_ERROR */

static int var_urlencoded_error_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if (msr->urlencoded_error) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* INBOUND_DATA_ERROR */

static int var_inbound_error_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if (msr->inbound_error) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

/* OUTBOUND_DATA_ERROR */

static int var_outbound_error_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if (msr->outbound_error) {
        return var_simple_generate(var, vartab, mptmp, "1");
    } else {
        return var_simple_generate(var, vartab, mptmp, "0");
    }
}

static apr_time_t calculate_perf_combined(modsec_rec *msr) {
    return msr->time_phase1 + msr->time_phase2 + msr->time_phase3 + msr->time_phase4
        + msr->time_phase5 + msr->time_storage_write /* time_storage_read is already
        included in phases */ + msr->time_logging + msr->time_gc;
}

char *format_all_performance_variables(modsec_rec *msr, apr_pool_t *mp) {
    return apr_psprintf(mp, "combined=%" APR_TIME_T_FMT ", p1=%" APR_TIME_T_FMT
        ", p2=%" APR_TIME_T_FMT ", p3=%" APR_TIME_T_FMT ", p4=%" APR_TIME_T_FMT
        ", p5=%" APR_TIME_T_FMT ", sr=%" APR_TIME_T_FMT ", sw=%" APR_TIME_T_FMT
        ", l=%" APR_TIME_T_FMT ", gc=%" APR_TIME_T_FMT, calculate_perf_combined(msr),
        msr->time_phase1, msr->time_phase2, msr->time_phase3, msr->time_phase4,
        msr->time_phase5, msr->time_storage_read, msr->time_storage_write,
        msr->time_logging, msr->time_gc);
}

static int generate_performance_variable(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp, apr_time_t value)
{
    msre_var *rvar = NULL;

    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = apr_psprintf(mptmp, "%" APR_TIME_T_FMT, value);
    rvar->value_len = strlen(rvar->value);
    
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* PERF_ALL */

static int var_perf_all_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = NULL;

    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = format_all_performance_variables(msr, mptmp);
    rvar->value_len = strlen(rvar->value);
    
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* PERF_COMBINED */

static int var_perf_combined_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return generate_performance_variable(msr, var, rule, vartab, mptmp, calculate_perf_combined(msr));
}

/* PERF_GC */

static int var_perf_gc_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return generate_performance_variable(msr, var, rule, vartab, mptmp, msr->time_gc);
}

/* PERF_PHASE1 */

static int var_perf_phase1_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return generate_performance_variable(msr, var, rule, vartab, mptmp, msr->time_phase1);
}

/* PERF_PHASE2 */

static int var_perf_phase2_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return generate_performance_variable(msr, var, rule, vartab, mptmp, msr->time_phase2);
}

/* PERF_PHASE3 */

static int var_perf_phase3_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return generate_performance_variable(msr, var, rule, vartab, mptmp, msr->time_phase3);
}

/* PERF_PHASE4 */

static int var_perf_phase4_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return generate_performance_variable(msr, var, rule, vartab, mptmp, msr->time_phase4);
}

/* PERF_PHASE5 */

static int var_perf_phase5_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return generate_performance_variable(msr, var, rule, vartab, mptmp, msr->time_phase5);
}

/* PERF_SREAD */

static int var_perf_sread_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return generate_performance_variable(msr, var, rule, vartab, mptmp, msr->time_storage_read);
}

/* PERF_SWRITE */

static int var_perf_swrite_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return generate_performance_variable(msr, var, rule, vartab, mptmp, msr->time_storage_write);
}

/* PERF_LOGGING */

static int var_perf_logging_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return generate_performance_variable(msr, var, rule, vartab, mptmp, msr->time_logging);
}


/* PERF_RULES */

static int var_perf_rules_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
        apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    arr = apr_table_elts(msr->perf_rules);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        int match = 0;

        /* Figure out if we want to include this variable. */
        if (var->param == NULL) match = 1;
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, te[i].key,
                                strlen(te[i].key), &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(te[i].key, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = te[i].val;
            rvar->value_len = strlen(rvar->value);
            rvar->name = apr_psprintf(mptmp, "PERF_RULES:%s",
                log_escape_nq(mptmp, te[i].key));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* DURATION */

static int var_duration_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = NULL;

    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = apr_psprintf(mptmp, "%" APR_TIME_T_FMT,
        (apr_time_usec(apr_time_now() - msr->r->request_time)));
    rvar->value_len = strlen(rvar->value);
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* TIME */

static int var_time_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = NULL;
    struct tm *tm;
    time_t tc;

    tc = time(NULL);
    tm = localtime(&tc);
    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = apr_psprintf(mptmp, "%02d%02d%02d%02d%02d%02d%02d",
        (tm->tm_year / 100) + 19, (tm->tm_year % 100),
         tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min,
         tm->tm_sec);
    rvar->value_len = strlen(rvar->value);
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* TIME_YEAR */

static int var_time_year_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = NULL;
    struct tm *tm;
    time_t tc;

    tc = time(NULL);
    tm = localtime(&tc);
    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = apr_psprintf(mptmp, "%02d%02d",
        (tm->tm_year / 100) + 19,
         tm->tm_year % 100);
    rvar->value_len = strlen(rvar->value);
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* TIME_WDAY */

static int var_time_wday_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = NULL;
    struct tm *tm;
    time_t tc;

    tc = time(NULL);
    tm = localtime(&tc);
    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = apr_psprintf(mptmp, "%d", tm->tm_wday);
    rvar->value_len = strlen(rvar->value);
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* TIME_SEC */

static int var_time_sec_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = NULL;
    struct tm *tm;
    time_t tc;

    tc = time(NULL);
    tm = localtime(&tc);
    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = apr_psprintf(mptmp, "%02d", tm->tm_sec);
    rvar->value_len = strlen(rvar->value);
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* TIME_MIN */

static int var_time_min_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = NULL;
    struct tm *tm;
    time_t tc;

    tc = time(NULL);
    tm = localtime(&tc);
    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = apr_psprintf(mptmp, "%02d", tm->tm_min);
    rvar->value_len = strlen(rvar->value);
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* TIME_HOUR */
static int var_time_hour_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = NULL;
    struct tm *tm;
    time_t tc;

    tc = time(NULL);
    tm = localtime(&tc);
    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = apr_psprintf(mptmp, "%02d", tm->tm_hour);
    rvar->value_len = strlen(rvar->value);
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* TIME_MON */

static int var_time_mon_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = NULL;
    struct tm *tm;
    time_t tc;

    tc = time(NULL);
    tm = localtime(&tc);
    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = apr_psprintf(mptmp, "%02d", tm->tm_mon + 1);
    rvar->value_len = strlen(rvar->value);
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* TIME_DAY */

static int var_time_day_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = NULL;
    struct tm *tm;
    time_t tc;

    tc = time(NULL);
    tm = localtime(&tc);
    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = apr_psprintf(mptmp, "%02d", tm->tm_mday);
    rvar->value_len = strlen(rvar->value);
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* TIME_EPOCH */

static int var_time_epoch_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    msre_var *rvar = NULL;
    time_t tc;

    tc = time(NULL);
    rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));
    rvar->value = apr_psprintf(mptmp, "%ld", (long)tc);
    rvar->value_len = strlen(rvar->value);
    apr_table_addn(vartab, rvar->name, (void *)rvar);

    return 1;
}

/* QUERY_STRING */

static int var_query_string_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, msr->query_string);
}

/* REQUEST_BASENAME */

static int var_request_basename_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    char *value = file_basename(mptmp, msr->r->parsed_uri.path);
    return var_simple_generate(var, vartab, mptmp, value);
}

/* REQUEST_BODY */

static int var_request_body_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if (msr->msc_reqbody_buffer != NULL) {
        return var_simple_generate_ex(var, vartab, mptmp,
            msr->msc_reqbody_buffer, msr->msc_reqbody_length);
    }
    return 0;
}

/* REQUEST_BODY_LENGTH */

static int var_request_body_length_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    char *value = apr_psprintf(mptmp, "%d", msr->msc_reqbody_length);
    return var_simple_generate(var, vartab, mptmp, value);
}

/* MATCHED_VARS_NAMES */

static int var_matched_vars_names_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    arr = apr_table_elts(msr->matched_vars);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        int match = 0;
        msc_string *str = (msc_string *)te[i].val;

        /* Figure out if we want to include this variable. */
        if (var->param == NULL) match = 1;
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, str->name,
                    strlen(str->name), &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(str->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match && (strncmp(str->name,"MATCHED_VARS:",13) != 0) && (strncmp(str->name,"MATCHED_VARS_NAMES:",19))) {

            msre_var *rvar = apr_palloc(mptmp, sizeof(msre_var));

            rvar->value = apr_pstrndup(mptmp, str->name, strlen(str->name));
            rvar->value_len = strlen(rvar->value);
            rvar->name = apr_psprintf(mptmp, "%s",
                    log_escape_nq(mptmp, str->name));

            if(var->is_counting == 0)
                rvar->is_counting = 0;
            else
                rvar->is_counting = 1;

            if(var->is_negated == 0)
                rvar->is_negated = 0;
            else
                rvar->is_negated = 1;

            apr_table_addn(vartab, rvar->name, (void *)rvar);

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Set variable \"%s\" size %d to collection.", rvar->name,rvar->value_len);
            }

            count++;
        }
    }

    return count;
}

/* MATCHED_VARS */

static int var_matched_vars_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
        apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    arr = apr_table_elts(msr->matched_vars);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        int match = 0;
        msc_string *str = (msc_string *)te[i].val;

        /* Figure out if we want to include this variable. */
        if (var->param == NULL) match = 1;
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, str->name,
                                strlen(str->name), &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(str->name, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match && (strncmp(str->name,"MATCHED_VARS:",13) != 0) && (strncmp(str->name,"MATCHED_VARS_NAMES:",19))) {

            msre_var *rvar = apr_palloc(mptmp, sizeof(msre_var));

            rvar->value = apr_pstrndup(mptmp, str->value, str->value_len);
            rvar->value_len = str->value_len;
            rvar->name = apr_psprintf(mptmp, "%s",
                    log_escape_nq(mptmp, str->name));

            if(var->is_counting == 0)
                rvar->is_counting = 0;
            else
                rvar->is_counting = 1;

            if(var->is_negated == 0)
                rvar->is_negated = 0;
            else
                rvar->is_negated = 1;

            apr_table_addn(vartab, rvar->name, (void *)rvar);

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Set variable \"%s\" value \"%s\" size %d to collection.", rvar->name, rvar->value, rvar->value_len);
            }

            count++;
        }
    }

    return count;
}

/* REQUEST_COOKIES */

static int var_request_cookies_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
        apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    arr = apr_table_elts(msr->request_cookies);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        int match = 0;

        /* Figure out if we want to include this variable. */
        if (var->param == NULL) match = 1;
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, te[i].key,
                                strlen(te[i].key), &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(te[i].key, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = te[i].val;
            rvar->value_len = strlen(rvar->value);
            rvar->name = apr_psprintf(mptmp, "REQUEST_COOKIES:%s",
                log_escape_nq(mptmp, te[i].key));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* REQUEST_COOKIES_NAMES */

static int var_request_cookies_names_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    arr = apr_table_elts(msr->request_cookies);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        int match = 0;

        /* Figure out if we want to include this variable. */
        if (var->param == NULL) match = 1;
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, te[i].key,
                    strlen(te[i].key), &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(te[i].key, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = te[i].key;
            rvar->value_len = strlen(rvar->value);
            rvar->name = apr_psprintf(mptmp, "REQUEST_COOKIES_NAMES:%s",
                log_escape_nq(mptmp, te[i].key));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* REQUEST_HEADERS */

static int var_request_headers_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    arr = apr_table_elts(msr->request_headers);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        int match = 0;

        /* Figure out if we want to include this variable. */
        if (var->param == NULL) match = 1;
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, te[i].key,
                    strlen(te[i].key), &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(te[i].key, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = te[i].val;
            rvar->value_len = strlen(rvar->value);
            rvar->name = apr_psprintf(mptmp, "REQUEST_HEADERS:%s",
                log_escape_nq(mptmp, te[i].key));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* REQUEST_HEADERS_NAMES */

static int var_request_headers_names_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    arr = apr_table_elts(msr->request_headers);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        int match = 0;

        /* Figure out if we want to include this variable. */
        if (var->param == NULL) match = 1;
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, te[i].key,
                    strlen(te[i].key), &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(te[i].key, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = te[i].key;
            rvar->value_len = strlen(rvar->value);
            rvar->name = apr_psprintf(mptmp, "REQUEST_HEADERS_NAMES:%s",
                log_escape_nq(mptmp, te[i].key));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* REQUEST_FILENAME */

static int var_request_filename_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, msr->r->parsed_uri.path);
}

/* REQUEST_LINE */

static int var_request_line_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, msr->request_line);
}

/* REQUEST_METHOD */

static int var_request_method_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, msr->request_method);
}

/* REQUEST_PROTOCOL */

static int var_request_protocol_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, msr->request_protocol);
}

/* SERVER_ADDR */

static int var_server_addr_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, msr->local_addr);
}

/* SERVER_NAME */

static int var_server_name_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, msr->hostname);
}

/* SERVER_PORT */

static int var_server_port_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    char *value = apr_psprintf(mptmp, "%u", msr->local_port);
    return var_simple_generate(var, vartab, mptmp, value);
}

/* SCRIPT_BASENAME */

static int var_script_basename_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    char *value = file_basename(mptmp, msr->r->filename);
    return var_simple_generate(var, vartab, mptmp, value);
}

/* SCRIPT_FILENAME */

static int var_script_filename_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    char *value = msr->r->filename;
    return var_simple_generate(var, vartab, mptmp, value);
}

/* SCRIPT_GID */

static int var_script_gid_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    char *value = apr_psprintf(mptmp, "%ld", (long)msr->r->finfo.group);
    return var_simple_generate(var, vartab, mptmp, value);
}

/* SCRIPT_GROUPNAME */

static int var_script_groupname_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    char *value = NULL;
    if (apr_gid_name_get(&value, msr->r->finfo.group, mptmp) == APR_SUCCESS) {
        return var_simple_generate(var, vartab, mptmp, value);
    }
    return 0;
}

/* SCRIPT_MODE */

static int var_script_mode_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    char *value = apr_psprintf(mptmp, "%04x", msr->r->finfo.protection);
    return var_simple_generate(var, vartab, mptmp, value);
}

/* SCRIPT_UID */

static int var_script_uid_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    char *value = apr_psprintf(mptmp, "%ld", (long)msr->r->finfo.user);
    return var_simple_generate(var, vartab, mptmp, value);
}

/* SCRIPT_USERNAME */

static int var_script_username_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    char *value = NULL;
    if (apr_uid_name_get(&value, msr->r->finfo.user, mptmp) == APR_SUCCESS) {
        return var_simple_generate(var, vartab, mptmp, value);
    }
    return 0;
}

/* AUTH_TYPE */

static int var_auth_type_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    char *value = msr->r->ap_auth_type;
    return var_simple_generate(var, vartab, mptmp, value);
}

/* PATH_INFO */

static int var_path_info_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const char *value = msr->r->path_info;
    return var_simple_generate(var, vartab, mptmp, value);
}

/* STREAM_OUTPUT_BODY */

static int var_stream_output_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if (msr->stream_output_data != NULL) {
        return var_simple_generate_ex(var, vartab, mptmp,
            msr->stream_output_data, msr->stream_output_length);
    }

    return 0;
}

/* STREAM_INPUT_BODY */

static int var_stream_input_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if (msr->stream_input_data != NULL) {
        return var_simple_generate_ex(var, vartab, mptmp,
            msr->stream_input_data, msr->stream_input_length);
    }

    return 0;
}

/* RESPONSE_BODY */

static int var_response_body_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    if (msr->resbody_data != NULL) {
        return var_simple_generate_ex(var, vartab, mptmp,
            msr->resbody_data, msr->resbody_length);
    }

    return 0;
}

/* RESPONSE_HEADERS */

static int var_response_headers_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    if (msr->response_headers == NULL) return 0;

    arr = apr_table_elts(msr->response_headers);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        int match = 0;

        /* Figure out if we want to include this variable. */
        if (var->param == NULL) match = 1;
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, te[i].key,
                    strlen(te[i].key), &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(te[i].key, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = te[i].val;
            rvar->value_len = strlen(rvar->value);
            rvar->name = apr_psprintf(mptmp, "RESPONSE_HEADERS:%s",
                log_escape_nq(mptmp, te[i].key));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* RESPONSE_HEADERS_NAMES */

static int var_response_headers_names_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const apr_array_header_t *arr = NULL;
    const apr_table_entry_t *te = NULL;
    int i, count = 0;

    arr = apr_table_elts(msr->response_headers);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        int match = 0;

        /* Figure out if we want to include this variable. */
        if (var->param == NULL) match = 1;
        else {
            if (var->param_data != NULL) { /* Regex. */
                char *my_error_msg = NULL;
                if (!(msc_regexec((msc_regex_t *)var->param_data, te[i].key,
                    strlen(te[i].key), &my_error_msg) == PCRE_ERROR_NOMATCH)) match = 1;
            } else { /* Simple comparison. */
                if (strcasecmp(te[i].key, var->param) == 0) match = 1;
            }
        }

        /* If we had a match add this argument to the collection. */
        if (match) {
            msre_var *rvar = apr_pmemdup(mptmp, var, sizeof(msre_var));

            rvar->value = te[i].key;
            rvar->value_len = strlen(rvar->value);
            rvar->name = apr_psprintf(mptmp, "RESPONSE_HEADERS_NAMES:%s",
                log_escape_nq(mptmp, te[i].key));
            apr_table_addn(vartab, rvar->name, (void *)rvar);

            count++;
        }
    }

    return count;
}

/* STATUS_LINE */

static int var_status_line_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const char *value = msr->status_line;
    return var_simple_generate(var, vartab, mptmp, value);
}

/* RESPONSE_PROTOCOL */

static int var_response_protocol_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const char *value = msr->response_protocol;
    return var_simple_generate(var, vartab, mptmp, value);
}

/* RESPONSE_STATUS */

static int var_response_status_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const char *value = apr_psprintf(mptmp, "%u", msr->response_status);
    return var_simple_generate(var, vartab, mptmp, value);
}

/* RESPONSE_CONTENT_TYPE */

static int var_response_content_type(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    return var_simple_generate(var, vartab, mptmp, msr->r->content_type);
}

/* RESPONSE_CONTENT_LENGTH */

static int var_response_content_length(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const char *value = apr_psprintf(mptmp, "%" APR_OFF_T_FMT, msr->r->clength);
    return var_simple_generate(var, vartab, mptmp, value);
}

/* USERID */

static int var_userid_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const char *value = msr->userid;
    return var_simple_generate(var, vartab, mptmp, value);
}

/* SESSIONID */

static int var_sessionid_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const char *value = msr->sessionid;
    return var_simple_generate(var, vartab, mptmp, value);
}

/* WEBAPPID */

static int var_webappid_generate(modsec_rec *msr, msre_var *var, msre_rule *rule,
    apr_table_t *vartab, apr_pool_t *mptmp)
{
    const char *value = msr->txcfg->webappid;
    return var_simple_generate(var, vartab, mptmp, value);
}

/* ---------------------------------------------- */

/**
 *
 */
void msre_engine_variable_register(msre_engine *engine, const char *name,
    unsigned int type, unsigned int argc_min, unsigned int argc_max,
    fn_var_validate_t validate, fn_var_generate_t generate,
    unsigned int is_cacheable, unsigned int availability)
{
    msre_var_metadata *metadata = (msre_var_metadata *)apr_pcalloc(engine->mp,
        sizeof(msre_var_metadata));
    if (metadata == NULL) return;

    metadata->name = name;
    metadata->type = type;
    metadata->argc_min = argc_min;
    metadata->argc_max = argc_max;
    metadata->validate = validate;
    metadata->generate = generate;
    metadata->is_cacheable = is_cacheable;
    metadata->availability = availability;

    apr_table_setn(engine->variables, name, (void *)metadata);
}

/**
 *
 */
void msre_engine_register_default_variables(msre_engine *engine) {

    /* ARGS */
    msre_engine_variable_register(engine,
        "ARGS",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_args_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* ARGS_COMBINED_SIZE */
    msre_engine_variable_register(engine,
        "ARGS_COMBINED_SIZE",
        VAR_LIST,
        0, 0,
        NULL,
        var_args_combined_size_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* ARGS_GET */
    msre_engine_variable_register(engine,
        "ARGS_GET",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_args_get_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* ARGS_GET_NAMES */
    msre_engine_variable_register(engine,
        "ARGS_GET_NAMES",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_args_get_names_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* ARGS_NAMES */
    msre_engine_variable_register(engine,
        "ARGS_NAMES",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_args_names_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* ARGS_POST */
    msre_engine_variable_register(engine,
        "ARGS_POST",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_args_post_generate,
        VAR_CACHE,
        PHASE_REQUEST_BODY
    );

    /* ARGS_POST_NAMES */
    msre_engine_variable_register(engine,
        "ARGS_POST_NAMES",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_args_post_names_generate,
        VAR_CACHE,
        PHASE_REQUEST_BODY
    );

    /* AUTH_TYPE */
    msre_engine_variable_register(engine,
        "AUTH_TYPE",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_auth_type_generate,
        VAR_CACHE,
        PHASE_REQUEST_BODY
    );

    /* ENV */
    msre_engine_variable_register(engine,
        "ENV",
        VAR_LIST,
        0, 1,
        var_env_validate,
        var_env_generate,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* FILES */
    msre_engine_variable_register(engine,
        "FILES",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_files_generate,
        VAR_CACHE,
        PHASE_REQUEST_BODY
    );

    /* FILES_COMBINED_SIZE */
    msre_engine_variable_register(engine,
        "FILES_COMBINED_SIZE",
        VAR_LIST,
        0, 0,
        NULL,
        var_files_combined_size_generate,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_REQUEST_BODY
    );

    /* FILES_NAMES */
    msre_engine_variable_register(engine,
        "FILES_NAMES",
        VAR_LIST,
        0, 0,
        NULL,
        var_files_names_generate,
        VAR_CACHE,
        PHASE_REQUEST_BODY
    );

    /* FILES_SIZES */
    msre_engine_variable_register(engine,
        "FILES_SIZES",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_files_sizes_generate,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_REQUEST_BODY
    );

    /* FILES_TMPNAMES */
    msre_engine_variable_register(engine,
        "FILES_TMPNAMES",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_files_tmpnames_generate,
        VAR_CACHE,
        PHASE_REQUEST_BODY
    );

    /* GEO */
    msre_engine_variable_register(engine,
        "GEO",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_geo_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* GLOBAL */
    msre_engine_variable_register(engine,
        "GLOBAL",
        VAR_LIST,
        1, 1,
        var_generic_list_validate,
        var_global_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* HIGHEST_SEVERITY */
    msre_engine_variable_register(engine,
        "HIGHEST_SEVERITY",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_highest_severity_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* IP */
    msre_engine_variable_register(engine,
        "IP",
        VAR_LIST,
        1, 1,
        var_generic_list_validate,
        var_ip_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* MATCHED_VAR */
    msre_engine_variable_register(engine,
        "MATCHED_VAR",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_matched_var_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* MATCHED_VAR_NAME */
    msre_engine_variable_register(engine,
        "MATCHED_VAR_NAME",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_matched_var_name_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* MODSEC_BUILD */
    msre_engine_variable_register(engine,
        "MODSEC_BUILD",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_modsec_build_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* MULTIPART_FILENAME */
    msre_engine_variable_register(engine,
        "MULTIPART_FILENAME",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_filename_generate,
        VAR_CACHE,
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_NAME */
    msre_engine_variable_register(engine,
        "MULTIPART_NAME",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_name_generate,
        VAR_CACHE,
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_BOUNDARY_QUOTED */
    msre_engine_variable_register(engine,
        "MULTIPART_BOUNDARY_QUOTED",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_boundary_quoted_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_BOUNDARY_WHITESPACE */
    msre_engine_variable_register(engine,
        "MULTIPART_BOUNDARY_WHITESPACE",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_boundary_whitespace_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_DATA_AFTER */
    msre_engine_variable_register(engine,
        "MULTIPART_DATA_AFTER",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_data_after_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_DATA_BEFORE */
    msre_engine_variable_register(engine,
        "MULTIPART_DATA_BEFORE",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_data_before_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_HEADER_FOLDING */
    msre_engine_variable_register(engine,
        "MULTIPART_HEADER_FOLDING",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_header_folding_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_CRLF_LINE */
    msre_engine_variable_register(engine,
        "MULTIPART_CRLF_LINE",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_crlf_line_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_CRLF_LF_LINES */
    msre_engine_variable_register(engine,
        "MULTIPART_CRLF_LF_LINES",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_crlf_lf_lines_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_LF_LINE */
    msre_engine_variable_register(engine,
        "MULTIPART_LF_LINE",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_lf_line_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_MISSING_SEMICOLON */
    msre_engine_variable_register(engine,
        "MULTIPART_MISSING_SEMICOLON",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_missing_semicolon_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_INVALID_PART */
    msre_engine_variable_register(engine,
        "MULTIPART_INVALID_PART",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_invalid_part_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_INVALID_QUOTING */
    msre_engine_variable_register(engine,
        "MULTIPART_INVALID_QUOTING",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_invalid_quoting_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_INVALID_HEADER_FOLDING */
    msre_engine_variable_register(engine,
        "MULTIPART_INVALID_HEADER_FOLDING",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_invalid_header_folding_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_FILE_LIMIT_EXCEEDED */
    msre_engine_variable_register(engine,
        "MULTIPART_FILE_LIMIT_EXCEEDED",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_file_limit_exceeded_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_STRICT_ERROR */
    msre_engine_variable_register(engine,
        "MULTIPART_STRICT_ERROR",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_strict_error_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* MULTIPART_UNMATCHED_BOUNDARY */
    msre_engine_variable_register(engine,
        "MULTIPART_UNMATCHED_BOUNDARY",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_multipart_unmatched_boundary_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* PATH_INFO */
    msre_engine_variable_register(engine,
        "PATH_INFO",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_path_info_generate,
        VAR_CACHE,
        PHASE_REQUEST_BODY
    );

    /* QUERY_STRING */
    msre_engine_variable_register(engine,
        "QUERY_STRING",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_query_string_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
    /* USERAGENT_IP */
    msre_engine_variable_register(engine,
        "USERAGENT_IP",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_useragent_ip_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );
#endif

    /* REMOTE_ADDR */
    msre_engine_variable_register(engine,
        "REMOTE_ADDR",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_remote_addr_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* REMOTE_HOST */
    msre_engine_variable_register(engine,
        "REMOTE_HOST",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_remote_host_generate,
        VAR_CACHE,
        PHASE_REQUEST_BODY
    );

    /* REMOTE_PORT */
    msre_engine_variable_register(engine,
        "REMOTE_PORT",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_remote_port_generate,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_REQUEST_BODY
    );

    /* REMOTE_USER */
    msre_engine_variable_register(engine,
        "REMOTE_USER",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_remote_user_generate,
        VAR_CACHE,
        PHASE_REQUEST_BODY
    );

    /* RESOURCE */
    msre_engine_variable_register(engine,
        "RESOURCE",
        VAR_LIST,
        1, 1,
        var_generic_list_validate,
        var_resource_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* REQBODY_PROCESSOR */
    msre_engine_variable_register(engine,
        "REQBODY_PROCESSOR",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_reqbody_processor_generate,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_REQUEST_HEADERS
    );

    /* REQBODY_PROCESSOR_ERROR - Deprecated */
    msre_engine_variable_register(engine,
        "REQBODY_PROCESSOR_ERROR",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_reqbody_processor_error_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_BODY
    );

    /* REQBODY_PROCESSOR_ERROR_MSG - Deprecated */
    msre_engine_variable_register(engine,
        "REQBODY_PROCESSOR_ERROR_MSG",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_reqbody_processor_error_msg_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_BODY
    );

    /* REQBODY_ERROR */
    msre_engine_variable_register(engine,
        "REQBODY_ERROR",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_reqbody_processor_error_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_BODY
    );

    /* REQBODY_ERROR_MSG */
    msre_engine_variable_register(engine,
        "REQBODY_ERROR_MSG",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_reqbody_processor_error_msg_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_BODY
    );

    /* REQUEST_BASENAME */
    msre_engine_variable_register(engine,
        "REQUEST_BASENAME",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_request_basename_generate,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_REQUEST_HEADERS
    );

    /* REQUEST_BODY */
    msre_engine_variable_register(engine,
        "REQUEST_BODY",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_request_body_generate,
        VAR_CACHE,
        PHASE_REQUEST_BODY
    );

    /* REQUEST_BODY_LENGTH */
    msre_engine_variable_register(engine,
        "REQUEST_BODY_LENGTH",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_request_body_length_generate,
        VAR_CACHE,
        PHASE_REQUEST_BODY
    );

    /* MATCHED_VARS_NAMES */
    msre_engine_variable_register(engine,
        "MATCHED_VARS_NAMES",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_matched_vars_names_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* MATCHED_VARS */
    msre_engine_variable_register(engine,
        "MATCHED_VARS",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_matched_vars_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* REQUEST_COOKIES */
    msre_engine_variable_register(engine,
        "REQUEST_COOKIES",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_request_cookies_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* REQUEST_COOKIES_NAMES */
    msre_engine_variable_register(engine,
        "REQUEST_COOKIES_NAMES",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_request_cookies_names_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* REQUEST_FILENAME */
    msre_engine_variable_register(engine,
        "REQUEST_FILENAME",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_request_filename_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* REQUEST_HEADERS */
    msre_engine_variable_register(engine,
        "REQUEST_HEADERS",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_request_headers_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* REQUEST_HEADERS_NAMES */
    msre_engine_variable_register(engine,
        "REQUEST_HEADERS_NAMES",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_request_headers_names_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* REQUEST_LINE */
    msre_engine_variable_register(engine,
        "REQUEST_LINE",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_request_line_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* REQUEST_METHOD */
    msre_engine_variable_register(engine,
        "REQUEST_METHOD",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_request_method_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* REQUEST_PROTOCOL */
    msre_engine_variable_register(engine,
        "REQUEST_PROTOCOL",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_request_protocol_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* REQUEST_URI */
    msre_engine_variable_register(engine,
        "REQUEST_URI",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_request_uri_generate,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_REQUEST_HEADERS
    );

    /* REQUEST_URI_RAW */
    msre_engine_variable_register(engine,
        "REQUEST_URI_RAW",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_request_uri_raw_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* UNIQUE_ID */
    msre_engine_variable_register(engine,
        "UNIQUE_ID",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_uniqueid_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );


    /* STREAM_OUTPUT_BODY */
    msre_engine_variable_register(engine,
        "STREAM_OUTPUT_BODY",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_stream_output_generate,
        VAR_CACHE,
        PHASE_RESPONSE_BODY
    );

    /* STREAM_INPUT_BODY */
    msre_engine_variable_register(engine,
        "STREAM_INPUT_BODY",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_stream_input_generate,
        VAR_CACHE,
        PHASE_FIRST
    );


    /* RESPONSE_BODY */
    msre_engine_variable_register(engine,
        "RESPONSE_BODY",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_response_body_generate,
        VAR_CACHE,
        PHASE_RESPONSE_BODY
    );

    /* RESPONSE_CONTENT_LENGTH */
    msre_engine_variable_register(engine,
        "RESPONSE_CONTENT_LENGTH",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_response_content_length,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_RESPONSE_HEADERS
    );

    /* RESPONSE_CONTENT_TYPE */
    msre_engine_variable_register(engine,
        "RESPONSE_CONTENT_TYPE",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_response_content_type,
        VAR_CACHE,
        PHASE_RESPONSE_HEADERS
    );

    /* RESPONSE_HEADERS */
    msre_engine_variable_register(engine,
        "RESPONSE_HEADERS",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_response_headers_generate,
        VAR_CACHE,
        PHASE_RESPONSE_HEADERS
    );

    /* RESPONSE_HEADERS_NAMES */
    msre_engine_variable_register(engine,
        "RESPONSE_HEADERS_NAMES",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_response_headers_names_generate,
        VAR_CACHE,
        PHASE_RESPONSE_HEADERS
    );

    /* RESPONSE_PROTOCOL */
    msre_engine_variable_register(engine,
        "RESPONSE_PROTOCOL",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_response_protocol_generate,
        VAR_CACHE,
        PHASE_RESPONSE_HEADERS
    );

    /* RESPONSE_STATUS */
    msre_engine_variable_register(engine,
        "RESPONSE_STATUS",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_response_status_generate,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_RESPONSE_HEADERS
    );

    /* RULE */
    msre_engine_variable_register(engine,
        "RULE",
        VAR_LIST,
        1, 1,
        NULL,
        var_rule_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_RESPONSE_HEADERS
    );

    /* SCRIPT_GID */
    msre_engine_variable_register(engine,
        "SCRIPT_GID",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_script_gid_generate,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_REQUEST_BODY
    );

    /* SCRIPT_BASENAME */
    msre_engine_variable_register(engine,
        "SCRIPT_BASENAME",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_script_basename_generate,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_REQUEST_BODY
    );

    /* SCRIPT_FILENAME */
    msre_engine_variable_register(engine,
        "SCRIPT_FILENAME",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_script_filename_generate,
        VAR_CACHE,
        PHASE_REQUEST_BODY
    );

    /* SCRIPT_GROUPNAME */
    msre_engine_variable_register(engine,
        "SCRIPT_GROUPNAME",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_script_groupname_generate,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_REQUEST_BODY
    );

    /* SCRIPT_MODE */
    msre_engine_variable_register(engine,
        "SCRIPT_MODE",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_script_mode_generate,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_REQUEST_BODY
    );

    /* SCRIPT_UID */
    msre_engine_variable_register(engine,
        "SCRIPT_UID",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_script_uid_generate,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_REQUEST_BODY
    );

    /* SCRIPT_USERNAME */
    msre_engine_variable_register(engine,
        "SCRIPT_USERNAME",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_script_username_generate,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_REQUEST_BODY
    );

    /* SERVER_ADDR */
    msre_engine_variable_register(engine,
        "SERVER_ADDR",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_server_addr_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* SERVER_NAME */
    msre_engine_variable_register(engine,
        "SERVER_NAME",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_server_name_generate,
        VAR_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* SERVER_PORT */
    msre_engine_variable_register(engine,
        "SERVER_PORT",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_server_port_generate,
        VAR_DONT_CACHE, /* temp copy */
        PHASE_REQUEST_HEADERS
    );

    /* SESSION */
    msre_engine_variable_register(engine,
        "SESSION",
        VAR_LIST,
        1, 1,
        var_generic_list_validate,
        var_session_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* SESSIONID */
    msre_engine_variable_register(engine,
        "SESSIONID",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_sessionid_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_RESPONSE_HEADERS
    );

    /* STATUS_LINE */
    msre_engine_variable_register(engine,
        "STATUS_LINE",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_status_line_generate,
        VAR_CACHE,
        PHASE_RESPONSE_HEADERS
    );

    /* URLENCODED_ERROR */
    msre_engine_variable_register(engine,
        "URLENCODED_ERROR",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_urlencoded_error_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_HEADERS
    );

    /* INBOUND_DATA_ERROR */
    msre_engine_variable_register(engine,
        "INBOUND_DATA_ERROR",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_inbound_error_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_REQUEST_BODY
    );

    /* OUTBOUND_DATA_ERROR */
    msre_engine_variable_register(engine,
        "OUTBOUND_DATA_ERROR",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_outbound_error_generate,
        VAR_DONT_CACHE, /* flag */
        PHASE_RESPONSE_BODY
    );

    /* USER */
    msre_engine_variable_register(engine,
        "USER",
        VAR_LIST,
        1, 1,
        var_generic_list_validate,
        var_user_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* USERID */
    msre_engine_variable_register(engine,
        "USERID",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_userid_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_RESPONSE_HEADERS
    );

    /* PERF_RULES */
    msre_engine_variable_register(engine,
        "PERF_RULES",
        VAR_LIST,
        0, 1,
        var_generic_list_validate,
        var_perf_rules_generate,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* PERF_ALL */
    msre_engine_variable_register(engine,
        "PERF_ALL",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_perf_all_generate,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* PERF_COMBINED */
    msre_engine_variable_register(engine,
        "PERF_COMBINED",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_perf_combined_generate,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* PERF_GC */
    msre_engine_variable_register(engine,
        "PERF_GC",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_perf_gc_generate,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* PERF_LOGGING */
    msre_engine_variable_register(engine,
        "PERF_LOGGING",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_perf_logging_generate,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* PERF_PHASE1 */
    msre_engine_variable_register(engine,
        "PERF_PHASE1",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_perf_phase1_generate,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* PERF_PHASE2 */
    msre_engine_variable_register(engine,
        "PERF_PHASE2",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_perf_phase2_generate,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* PERF_PHASE3 */
    msre_engine_variable_register(engine,
        "PERF_PHASE3",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_perf_phase3_generate,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* PERF_PHASE4 */
    msre_engine_variable_register(engine,
        "PERF_PHASE4",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_perf_phase4_generate,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* PERF_PHASE5 */
    msre_engine_variable_register(engine,
        "PERF_PHASE5",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_perf_phase5_generate,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* PERF_SREAD */
    msre_engine_variable_register(engine,
        "PERF_SREAD",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_perf_sread_generate,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* PERF_SWRITE */
    msre_engine_variable_register(engine,
        "PERF_SWRITE",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_perf_swrite_generate,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* DURATION */
    msre_engine_variable_register(engine,
        "DURATION",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_duration_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* TIME */
    msre_engine_variable_register(engine,
        "TIME",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_time_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* TIME_DAY */
    msre_engine_variable_register(engine,
        "TIME_DAY",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_time_day_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* TIME_EPOCH */
    msre_engine_variable_register(engine,
        "TIME_EPOCH",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_time_epoch_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* TIME_HOUR */
    msre_engine_variable_register(engine,
        "TIME_HOUR",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_time_hour_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* TIME_MIN */
    msre_engine_variable_register(engine,
        "TIME_MIN",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_time_min_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* TIME_MON */
    msre_engine_variable_register(engine,
        "TIME_MON",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_time_mon_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* TIME_SEC */
    msre_engine_variable_register(engine,
        "TIME_SEC",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_time_sec_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* TIME_WDAY */
    msre_engine_variable_register(engine,
        "TIME_WDAY",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_time_wday_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* TIME_YEAR */
    msre_engine_variable_register(engine,
        "TIME_YEAR",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_time_year_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* TX */
    msre_engine_variable_register(engine,
        "TX",
        VAR_LIST,
        1, 1,
        var_generic_list_validate,
        var_tx_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* WEBAPPID */
    msre_engine_variable_register(engine,
        "WEBAPPID",
        VAR_SIMPLE,
        0, 0,
        NULL,
        var_webappid_generate,
        VAR_DONT_CACHE,
        PHASE_RESPONSE_HEADERS
    );

    /* WEBSERVER_ERROR_LOG */
    msre_engine_variable_register(engine,
        "WEBSERVER_ERROR_LOG",
        VAR_LIST,
        0, 0,
        NULL,
        var_webserver_error_log_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_HEADERS
    );

    /* XML */
    msre_engine_variable_register(engine,
        "XML",
        VAR_LIST,
        0, 1,
        var_xml_validate,
        var_xml_generate,
        VAR_DONT_CACHE, /* dynamic */
        PHASE_REQUEST_BODY
    );
}
