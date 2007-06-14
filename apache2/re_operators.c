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
#include "msc_pcre.h"
#include "apr_strmatch.h"

/**
 *
 */
void msre_engine_op_register(msre_engine *engine, const char *name,
    FN_OP_PARAM_INIT(fn1), FN_OP_EXECUTE(fn2))
{
    msre_op_metadata *metadata = (msre_op_metadata *)apr_pcalloc(engine->mp,
        sizeof(msre_op_metadata));
    if (metadata == NULL) return;

    metadata->name = name;
    metadata->param_init = fn1;
    metadata->execute = fn2;
    apr_table_setn(engine->operators, name, (void *)metadata);
}

/**
 *
 */
msre_op_metadata *msre_engine_op_resolve(msre_engine *engine, const char *name) {
    return (msre_op_metadata *)apr_table_get(engine->operators, name);
}



/* -- Operators -- */

/* unconditionalMatch */

static int msre_op_unconditionalmatch_execute(modsec_rec *msr, msre_rule *rule,
    msre_var *var, char **error_msg)
{
    *error_msg = "Unconditional match in SecAction.";

    /* Always match. */
    return 1;
}

/* rx */

static int msre_op_rx_param_init(msre_rule *rule, char **error_msg) {
    const char *errptr = NULL;
    int erroffset;
    msc_regex_t *regex;
    const char *pattern = rule->op_param;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    /* Compile pattern */
    regex = msc_pregcomp(rule->ruleset->mp, pattern, PCRE_DOTALL | PCRE_DOLLAR_ENDONLY, &errptr, &erroffset);
    if (regex == NULL) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Error compiling pattern (pos %i): %s",
            erroffset, errptr);
        return 0;
    }

    rule->op_param_data = regex;

    return 1; /* OK */
}

static int msre_op_rx_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    msc_regex_t *regex = (msc_regex_t *)rule->op_param_data;
    const char *target;
    unsigned int target_length;
    char *my_error_msg = NULL;
    int ovector[33];
    int rc;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (regex == NULL) {
        *error_msg = "Internal Error: regex data is null.";
        return -1;
    }

    /* If the given target is null run against an empty
     * string. This is a behaviour consistent with previous
     * releases.
     */
    if (var->value == NULL) {
        target = "";
        target_length = 0;
    } else {
        target = var->value;
        target_length = var->value_len;
    }

    /* IMP1 Can we tell the regex engine not to do any captures if we have no use for them? */
    rc = msc_regexec_capture(regex, target, target_length, ovector, 30, &my_error_msg);
    if (rc < -1) {
        *error_msg = apr_psprintf(msr->mp, "Regex execution failed: %s", my_error_msg);
        return -1;
    }

    /* Handle captured subexpressions. */
    if (rc > 0) {
        int capture = 0;
        const apr_array_header_t *tarr;
        const apr_table_entry_t *telts;
        int i;

        /* Are we supposed to store the captured subexpressions? */
        /* IMP1 Can we use a flag to avoid having to iterate through the list every time. */
        tarr = apr_table_elts(rule->actionset->actions);
        telts = (const apr_table_entry_t*)tarr->elts;
        for (i = 0; i < tarr->nelts; i++) {
            msre_action *action = (msre_action *)telts[i].val;
            if (strcasecmp(action->metadata->name, "capture") == 0) {
                capture = 1;
                break;
            }
        }

        if (capture) {
            int k;

            /* Use the available captures. */
            for(k = 0; k < rc; k++) {
                msc_string *s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
                if (s == NULL) return -1;
                s->name = apr_psprintf(msr->mp, "%i", k);
                s->value = apr_pstrmemdup(msr->mp,
                    target + ovector[2*k], ovector[2*k + 1] - ovector[2*k]);
                s->value_len = (ovector[2*k + 1] - ovector[2*k]);
                if ((s->name == NULL)||(s->value == NULL)) return -1;
                apr_table_setn(msr->tx_vars, s->name, (void *)s);
                msr_log(msr, 9, "Adding regex subexpression to TXVARS (%i): %s", k,
                    log_escape_nq(msr->mp, s->value));
            }

            /* Unset the remaining ones (from previous invocations). */
            for(k = rc; k <= 9; k++) {
                char buf[24];
                apr_snprintf(buf, sizeof(buf), "%i", k);
                apr_table_unset(msr->tx_vars, buf);
            }
        }
    }

    /*
    if ( ((rc == PCRE_ERROR_NOMATCH)&&(rule->op_negated == 1))
        || ((rc != PCRE_ERROR_NOMATCH)&&(rule->op_negated == 0)) )
    {
    */
    if (rc != PCRE_ERROR_NOMATCH) { /* Match. */
        char *pattern_escaped = log_escape(msr->mp, regex->pattern);

        /* This message will be logged. */
        if (strlen(pattern_escaped) > 252) {
            *error_msg = apr_psprintf(msr->mp, "Pattern match \"%.252s ...\" at %s.",
                pattern_escaped, var->name);
        } else {
            *error_msg = apr_psprintf(msr->mp, "Pattern match \"%s\" at %s.",
                pattern_escaped, var->name);
        }

        return 1;
    }

    /* No match. */
    return 0;
}

/* m */

static int msre_op_m_param_init(msre_rule *rule, char **error_msg) {
    const apr_strmatch_pattern *compiled_pattern;
    const char *pattern = rule->op_param;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    /* Compile pattern */
    compiled_pattern = apr_strmatch_precompile(rule->ruleset->mp, pattern, 1);
    if (compiled_pattern == NULL) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Error compiling pattern: %s", pattern);
        return 0;
    }

    rule->op_param_data = (void *)compiled_pattern;

    return 1; /* OK */
}

static int msre_op_m_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    apr_strmatch_pattern *compiled_pattern = (apr_strmatch_pattern *)rule->op_param_data;
    const char *target;
    unsigned int target_length;
    const char *rc;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (compiled_pattern == NULL) {
        *error_msg = "Internal Error: strnmatch data is null.";
        return -1;
    }

    /* If the given target is null run against an empty
     * string. This is a behaviour consistent with previous
     * releases.
     */
    if (var->value == NULL) {
        target = "";
        target_length = 0;
    } else {
        target = var->value;
        target_length = var->value_len;
    }

    rc = apr_strmatch(compiled_pattern, target, target_length);
    if (rc == NULL) {
        /* No match. */
        return 0;
    }

    *error_msg = apr_psprintf(msr->mp, "Pattern match \"%s\" at %s.",
        log_escape(msr->mp, rule->op_param), var->name);

    /* Match. */
    return 1;
}

#ifdef WITH_LIBXML2

/* validateDTD */

static int msre_op_validateDTD_init(msre_rule *rule, char **error_msg) {
    /* ENH Verify here the file actually exists. */
    return 1;
}

static int msre_op_validateDTD_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    xmlValidCtxtPtr cvp;
    xmlDtdPtr dtd;

    if ((msr->xml == NULL)||(msr->xml->doc == NULL)) {
        *error_msg = apr_psprintf(msr->mp, "XML document tree could not be found for "
            "DTD validation.");
        return -1;
    }

    dtd = xmlParseDTD(NULL, (const xmlChar *)rule->op_param); /* EHN support relative filenames */
    if (dtd == NULL) {
        *error_msg = apr_psprintf(msr->mp, "XML: Failed to load DTD: %s", rule->op_param);
        return -1;
    }

    cvp = xmlNewValidCtxt();
    if (cvp == NULL) {
        *error_msg = "XML: Failed to create a validation context.";
        xmlFreeDtd(dtd);
        return -1;
    }

    if (!xmlValidateDtd(cvp, msr->xml->doc, dtd)) {
        *error_msg = "XML: DTD validation failed.";
        xmlFreeValidCtxt(cvp);
        xmlFreeDtd(dtd);
        return 1; /* No match. */
    }

    msr_log(msr, 4, "XML: Successfully validated payload against DTD: %s", rule->op_param);

    xmlFreeValidCtxt(cvp);
    xmlFreeDtd(dtd);

    /* Match. */
    return 0;
}

/* validateSchema */

static int msre_op_validateSchema_init(msre_rule *rule, char **error_msg) {
    /* ENH Verify here the file actually exists. */
    return 1;
}

static int msre_op_validateSchema_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    xmlSchemaParserCtxtPtr parserCtx;
    xmlSchemaValidCtxtPtr validCtx;
    xmlSchemaPtr schema;
    int rc;

    if ((msr->xml == NULL)||(msr->xml->doc == NULL)) {
        *error_msg = apr_psprintf(msr->mp, "XML document tree could not be found for "
            "Schema validation.");
        return -1;
    }

    parserCtx = xmlSchemaNewParserCtxt(rule->op_param); /* ENH support relative filenames */
    if (parserCtx == NULL) {
        *error_msg = apr_psprintf(msr->mp, "XML: Failed to load Schema from file: %s",
            rule->op_param);
        return -1;
    }

    schema = xmlSchemaParse(parserCtx);
    if (schema == NULL) {
        *error_msg = apr_psprintf(msr->mp, "XML: Failed to load Schema: %s", rule->op_param);
        xmlSchemaFreeParserCtxt(parserCtx);
        return -1;
    }

    validCtx = xmlSchemaNewValidCtxt(schema);
    if (validCtx == NULL) {
        *error_msg = "XML: Failed to create validation context.";
        xmlSchemaFree(schema);
        xmlSchemaFreeParserCtxt(parserCtx);
        return -1;
    }

    rc = xmlSchemaValidateDoc(validCtx, msr->xml->doc);
    if (rc != 0) {
        *error_msg = "XML: Schema validation failed.";
        xmlSchemaFree(schema);
        xmlSchemaFreeParserCtxt(parserCtx);
        return 1; /* No match. */
    }

    msr_log(msr, 4, "XML: Successfully validated payload against Schema: %s", rule->op_param);

    xmlSchemaFree(schema);
    xmlSchemaFreeValidCtxt(validCtx);

    return 0;
}

#endif

/* rbl */

static int msre_op_rbl_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    unsigned int h0, h1, h2, h3;
    char *name_to_check = NULL;
    char *target = NULL;
    apr_sockaddr_t *sa = NULL;
    apr_status_t rc;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    /* ENH Add IPv6 support. */

    target = apr_pstrmemdup(msr->mp, var->value, var->value_len);
    if (target == NULL) return -1;

    /* Construct the host name we want to resolve. */
    if (sscanf(target, "%d.%d.%d.%d", &h0, &h1, &h2, &h3) == 4) {
        /* IPv4 address */
        name_to_check = apr_psprintf(msr->mp, "%i.%i.%i.%i.%s", h3, h2, h1, h0, rule->op_param);
    } else {
        /* Assume the input is a domain name. */
        name_to_check = apr_psprintf(msr->mp, "%s.%s", target, rule->op_param);
    }

    if (name_to_check == NULL) return -1;

    rc = apr_sockaddr_info_get(&sa, name_to_check,
        APR_UNSPEC/*msr->r->connection->remote_addr->family*/, 0, 0, msr->mp);
    if (rc == APR_SUCCESS) {
        *error_msg = apr_psprintf(msr->r->pool, "RBL lookup of %s succeeded.",
            log_escape_nq(msr->mp, name_to_check));
        return 1; /* Match. */
    }

    msr_log(msr, 5, "RBL lookup of %s failed.", log_escape_nq(msr->mp, name_to_check));

    /* No match. */
    return 0;
}

/* inspectFile */
static int msre_op_inspectFile_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    char *script_output = NULL;
    char const *argv[5];
    const char *approver_script = rule->op_param;
    const char *target_file = apr_pstrmemdup(msr->mp, var->value, var->value_len);

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    msr_log(msr, 4, "Executing %s to inspect %s.", approver_script, target_file);

    argv[0] = approver_script;
    argv[1] = target_file;
    argv[2] = NULL;    

    if (apache2_exec(msr, approver_script, (const char **)argv, &script_output) <= 0) {
        *error_msg = apr_psprintf(msr->mp, "Execution of the approver script \"%s\" failed (invocation failed).",
            log_escape(msr->mp, approver_script));
        return -1;
    }

    if (script_output == NULL) {
        *error_msg = apr_psprintf(msr->mp, "Execution of the approver script \"%s\" failed (no output).",
            log_escape(msr->mp, approver_script));
        return -1;
    }

    if (script_output[0] != '1') {
        *error_msg = apr_psprintf(msr->mp, "File \"%s\" rejected by the approver script \"%s\": %s",
            log_escape(msr->mp, target_file), log_escape(msr->mp, approver_script),
            log_escape_nq(msr->mp,  script_output));
        return 1; /* Match. */
    }

    /* No match. */
    return 0;
}

/* validateByteRange */

static int msre_op_validateByteRange_init(msre_rule *rule, char **error_msg) {
    char *p = NULL, *saveptr = NULL;
    char *table = NULL, *data = NULL;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (rule->op_param == NULL) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Missing parameter for validateByteRange.");
        return -1;
    }

    /* Initialise. */
    data = apr_pstrdup(rule->ruleset->mp, rule->op_param);
    rule->op_param_data = apr_pcalloc(rule->ruleset->mp, 32);
    if ((data == NULL)||(rule->op_param_data == NULL)) return -1;
    table = rule->op_param_data;

    /* Extract parameters and update table. */
    p = apr_strtok(data, ",", &saveptr);
    while(p != NULL) {
        char *s = strstr(p, "-");
        if (s == NULL) {
            /* Single value. */
            int x = atoi(p);
            if ((x < 0)||(x > 255)) {
                *error_msg = apr_psprintf(rule->ruleset->mp, "Invalid range value: %i", x);
                return 0;
            }
            table[x>>3] = (table[x>>3] | (1 << (x & 0x7)));
        } else {
            /* Range. */
            int start = atoi(p);
            int end = atoi(s + 1);

            if ((start < 0)||(start > 255)) {
                *error_msg = apr_psprintf(rule->ruleset->mp, "Invalid range start value: %i",
                    start);
                return 0;
            }
            if ((end < 0)||(end > 255)) {
                *error_msg = apr_psprintf(rule->ruleset->mp, "Invalid range end value: %i", end);
                return 0;
            }
            if (start > end) {
                *error_msg = apr_psprintf(rule->ruleset->mp, "Invalid range: %i-%i", start, end);
                return 0;
            }

            while(start <= end) {
                table[start >> 3] = (table[start >> 3] | (1 << (start & 0x7)));
                start++;
            }
        }

        p = apr_strtok(NULL, ",", &saveptr);
    }

    return 1;
}

static int msre_op_validateByteRange_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    char *table = rule->op_param_data;
    unsigned int i, count;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (table == NULL) {
        *error_msg = apr_psprintf(msr->mp, "Internal Error: validateByteRange table not "
            "initialised.");
        return -1;
    }

    /* Check every byte of the target to detect characters that are not allowed. */

    count = 0;
    for(i = 0; i < var->value_len; i++) {
        int x = ((unsigned char *)var->value)[i];
        if (!(table[x >> 3] & (1 << (x & 0x7)))) {
            msr_log(msr, 9, "Value %i outside range: %s", x, rule->op_param);
            count++;
        }
    }

    if (count == 0) return 0; /* Valid - no match. */

    *error_msg = apr_psprintf(msr->mp, "Found %i byte(s) outside range: %s.",
        count, rule->op_param);

    return 1; /* Invalid - match.*/
}

/* validateUrlEncoding */

static int validate_url_encoding(const char *input, long int input_length) {
    int i;

    if ((input == NULL)||(input_length < 0)) return -1;

    i = 0;
    while (i < input_length) {
        if (input[i] == '%') {
            if (i + 2 >= input_length) {
                /* Not enough bytes. */
                return -3;
            }
            else {
                /* Here we only decode a %xx combination if it is valid,
                 * leaving it as is otherwise.
                 */
                char c1 = input[i + 1];
                char c2 = input[i + 2];

                if ( (((c1 >= '0')&&(c1 <= '9')) || ((c1 >= 'a')&&(c1 <= 'f')) || ((c1 >= 'A')&&(c1 <= 'F')))
                    && (((c2 >= '0')&&(c2 <= '9')) || ((c2 >= 'a')&&(c2 <= 'f')) || ((c2 >= 'A')&&(c2 <= 'F'))) )
                {
                    i += 3;
                } else {
                    /* Non-hexadecimal characters used in encoding. */
                    return -2;
                }
            }
        } else {
            i++;
        }
    }

    return 1;
}

static int msre_op_validateUrlEncoding_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    int rc = validate_url_encoding(var->value, var->value_len);
    switch(rc) {
        case 1 :
            return 0; /* Encoding is valid, no match. */
            break;
        case -2 :
            *error_msg = apr_psprintf(msr->mp, "Invalid URL Encoding: Non-hexadecimal "
                "digits used.");
            return 1; /* Invalid, match. */
            break;
        case -3 :
            *error_msg = apr_psprintf(msr->mp, "Invalid URL Encoding: Not enough characters "
                "at the end of input.");
            return 1; /* Invalid, match. */
            break;
        case -1 :
        default :
            *error_msg = apr_psprintf(msr->mp, "Invalid URL Encoding: Internal Error (rc = %i)", rc);
            return -1;
            break;

    }

    /* No match. */
    return 0;
}

/* validateUtf8Encoding */

#define UNICODE_ERROR_CHARACTERS_MISSING    -1
#define UNICODE_ERROR_INVALID_ENCODING      -2
#define UNICODE_ERROR_OVERLONG_CHARACTER    -3

static int detect_utf8_character(const char *p_read, unsigned int length) {
    int unicode_len = 0;
    unsigned int d = 0;
    unsigned char c;

    if (p_read == NULL) return 0;
    c = *p_read;
    if (c == 0) return 0;

    if ((c & 0xE0) == 0xC0) {
        /* two byte unicode */
        if (length < 2) unicode_len = UNICODE_ERROR_CHARACTERS_MISSING;
        else
        if (((*(p_read + 1)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else {
            unicode_len = 2;
            d = ((c & 0x1F) << 6) | (*(p_read + 1) & 0x3F);
        }
    }
    else if ((c & 0xF0) == 0xE0) {
        /* three byte unicode */
        if (length < 3) unicode_len = UNICODE_ERROR_CHARACTERS_MISSING;
        else
        if (((*(p_read + 1)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else
        if (((*(p_read + 2)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else {
            unicode_len = 3;
            d = ((c & 0x0F) << 12) | ((*(p_read + 1) & 0x3F) << 6) | (*(p_read + 2) & 0x3F);
        }
    }
    else if ((c & 0xF8) == 0xF0) {
        /* four byte unicode */
        if (length < 4) unicode_len = UNICODE_ERROR_CHARACTERS_MISSING;
        else
        if (((*(p_read + 1)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else
        if (((*(p_read + 2)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else
        if (((*(p_read + 3)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else {
            d = ((c & 0x07) << 18) | ((*(p_read + 1) & 0x3F) << 12) | ((*(p_read + 2) & 0x3F) < 6) | (*(p_read + 3) & 0x3F);
            unicode_len = 4;
        }
    }
    else if ((c & 0xFC) == 0xF8) {
        /* five byte unicode */
        if (length < 5) unicode_len = UNICODE_ERROR_CHARACTERS_MISSING;
        else
        if (((*(p_read + 1)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else
        if (((*(p_read + 2)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else
        if (((*(p_read + 3)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else
        if (((*(p_read + 4)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else {
            d = ((c & 0x03) << 24) | ((*(p_read + 1) & 0x3F) << 18) | ((*(p_read + 2) & 0x3F) << 12) | ((*(p_read + 3) & 0x3F) << 6) | (*(p_read + 4) & 0x3F);
            unicode_len = 5;
        }
    }
    else if ((c & 0xFE) == 0xFC) {
        /* six byte unicode */
        if (length < 6) unicode_len = UNICODE_ERROR_CHARACTERS_MISSING;
        else
        if (((*(p_read + 1)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else
        if (((*(p_read + 2)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else
        if (((*(p_read + 3)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else
        if (((*(p_read + 4)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else
        if (((*(p_read + 5)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else {
            d = ((c & 0x01) << 30) | ((*(p_read + 1) & 0x3F) << 24) | ((*(p_read + 2) & 0x3F) << 18) | ((*(p_read + 3) & 0x3F) << 12) | ((*(p_read + 4) & 0x3F) << 6) | (*(p_read + 5) & 0x3F);
            unicode_len = 6;
        }
    }

    if ((unicode_len > 1)&&((d & 0x7F) == d)) {
        unicode_len = UNICODE_ERROR_OVERLONG_CHARACTER;
    }

    return unicode_len;
}

static int msre_op_validateUtf8Encoding_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    unsigned int i, bytes_left;

    bytes_left = var->value_len;
    for(i = 0; i < var->value_len; i++) {
        int rc = detect_utf8_character(&var->value[i], bytes_left);
        switch(rc) {
            case UNICODE_ERROR_CHARACTERS_MISSING :
                *error_msg = apr_psprintf(msr->mp, "Invalid UTF-8 encoding: not enough bytes in "
                    "character.");
                return 1;
                break;
            case UNICODE_ERROR_INVALID_ENCODING :
                *error_msg = apr_psprintf(msr->mp, "Invalid Unicode encoding: invalid byte value "
                    "in character.");
                return 1;
                break;
            case UNICODE_ERROR_OVERLONG_CHARACTER :
                *error_msg = apr_psprintf(msr->mp, "Invalid Unicode encoding: overlong "
                    "character detected.");
                return 1;
                break;
        }

        bytes_left--;
    }

    return 0;
}

/* eq */

static int msre_op_eq_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    int left, right;
    char *target = NULL;

    if ((var->value == NULL)||(rule->op_param == NULL)) {
        /* NULL values do not match anything. */
        return 0;
    }

    target = apr_pstrmemdup(msr->mp, var->value, var->value_len);
    if (target == NULL) return -1;
    left = atoi(target);
    right = atoi(rule->op_param);

    if (left != right) {
        /* No match. */
        return 0;
    }
    else {
        *error_msg = apr_psprintf(msr->mp, "Operator EQ match: %i.", right);
        /* Match. */
        return 1;
    }
}

/* gt */

static int msre_op_gt_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    int left, right;
    char *target = NULL;

    if ((var->value == NULL)||(rule->op_param == NULL)) {
        /* NULL values do not match anything. */
        return 0;
    }

    target = apr_pstrmemdup(msr->mp, var->value, var->value_len);
    if (target == NULL) return -1;
    left = atoi(target);
    right = atoi(rule->op_param);

    if (left <= right) {
        /* No match. */
        return 0;
    }
    else {
        *error_msg = apr_psprintf(msr->mp, "Operator GT match: %i.", right);
        /* Match. */
        return 1;
    }
}

/* lt */

static int msre_op_lt_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    int left, right;
    char *target = NULL;

    if ((var->value == NULL)||(rule->op_param == NULL)) {
        /* NULL values do not match anything. */
        return 0;
    }
    
    target = apr_pstrmemdup(msr->mp, var->value, var->value_len);
    if (target == NULL) return -1;
    left = atoi(target);
    right = atoi(rule->op_param);

    if (left >= right) {
        /* No match. */
        return 0;
    }
    else {
        *error_msg = apr_psprintf(msr->mp, "Operator LT match: %i.", right);
        /* Match. */
        return 1;
    }
}

/* ge */

static int msre_op_ge_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    int left, right;
    char *target = NULL;

    if ((var->value == NULL)||(rule->op_param == NULL)) {
        /* NULL values do not match anything. */
        return 0;
    }

    target = apr_pstrmemdup(msr->mp, var->value, var->value_len);
    if (target == NULL) return -1;
    left = atoi(target);
    right = atoi(rule->op_param);

    if (left < right) {
        /* No match. */
        return 0;
    }
    else {
        *error_msg = apr_psprintf(msr->mp, "Operator GE match: %i.", right);
        /* Match. */
        return 1;
    }
}

/* le */

static int msre_op_le_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    int left, right;
    char *target = NULL;

    if ((var->value == NULL)||(rule->op_param == NULL)) {
        /* NULL values do not match anything. */
        return 0;
    }

    target = apr_pstrmemdup(msr->mp, var->value, var->value_len);
    if (target == NULL) return -1;
    left = atoi(target);
    right = atoi(rule->op_param);

    if (left > right) {
        /* No match. */
        return 0;
    }
    else {
        *error_msg = apr_psprintf(msr->mp, "Operator LE match: %i.", right);
        /* Match. */
        return 1;
    }
}

/* ------------------------------------------------------------------------------- */

/**
 *
 */
void msre_engine_register_default_operators(msre_engine *engine) {
    /* unconditionalMatch */
    msre_engine_op_register(engine,
        "unconditionalMatch",
        NULL,
        msre_op_unconditionalmatch_execute
    );

    /* rx */
    msre_engine_op_register(engine,
        "rx",
        msre_op_rx_param_init,
        msre_op_rx_execute
    );

    /* m */
    msre_engine_op_register(engine,
        "m",
        msre_op_m_param_init,
        msre_op_m_execute
    );

    #ifdef WITH_LIBXML2

    /* validateDTD */
    msre_engine_op_register(engine,
        "validateDTD",
        msre_op_validateDTD_init,
        msre_op_validateDTD_execute
    );

    /* validateSchema */
    msre_engine_op_register(engine,
        "validateSchema",
        msre_op_validateSchema_init,
        msre_op_validateSchema_execute
    );

    #endif

    /* rbl */
    msre_engine_op_register(engine,
        "rbl",
        NULL, /* ENH init function to validate DNS server */
        msre_op_rbl_execute
    );

    /* inspectFile */
    msre_engine_op_register(engine,
        "inspectFile",
        NULL,
        msre_op_inspectFile_execute
    );

    /* validateByteRange */
    msre_engine_op_register(engine,
        "validateByteRange",
        msre_op_validateByteRange_init,
        msre_op_validateByteRange_execute
    );

    /* validateUrlEncoding */
    msre_engine_op_register(engine,
        "validateUrlEncoding",
        NULL,
        msre_op_validateUrlEncoding_execute
    );

    /* validateUtf8Encoding */
    msre_engine_op_register(engine,
        "validateUtf8Encoding",
        NULL,
        msre_op_validateUtf8Encoding_execute
    );

    /* eq */
    msre_engine_op_register(engine,
        "eq",
        NULL,
        msre_op_eq_execute
    );

    /* gt */
    msre_engine_op_register(engine,
        "gt",
        NULL,
        msre_op_gt_execute
    );

    /* lt */
    msre_engine_op_register(engine,
        "lt",
        NULL,
        msre_op_lt_execute
    );

    /* le */
    msre_engine_op_register(engine,
        "le",
        NULL,
        msre_op_le_execute
    );

    /* ge */
    msre_engine_op_register(engine,
        "ge",
        NULL,
        msre_op_ge_execute
    );
}
