/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2010 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * This product is released under the terms of the General Public Licence,
 * version 2 (GPLv2). Please refer to the file LICENSE (included with this
 * distribution) which contains the complete text of the licence.
 *
 * There are special exceptions to the terms and conditions of the GPL
 * as it is applied to this software. View the full text of the exception in
 * file MODSECURITY_LICENSING_EXCEPTION in the directory of this software
 * distribution.
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address support@trustwave.com.
 *
 */
#include "re.h"
#include "msc_pcre.h"
#include "msc_geo.h"
#include "apr_lib.h"
#include "apr_strmatch.h"
#include "acmp.h"
#include <regex.h>

#define PARSE_REGEX_IP "([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)(?:(\\/[0-9]+))?|([0-9a-f]+\\:[0-9a-f]+\\:[0-9a-f]+\\:[0-9a-f]+\\:[0-9a-f]+\\:[0-9a-f]+\\:[0-9a-f]+\\:[0-9a-f]+)(?:(\\/[0-9]+))?"
#define MAX_SUBSTRINGS 30

/**
 *
 */
void msre_engine_op_register(msre_engine *engine, const char *name,
    fn_op_param_init_t fn1, fn_op_execute_t fn2)
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

/* noMatch */

static int msre_op_nomatch_execute(modsec_rec *msr, msre_rule *rule,
    msre_var *var, char **error_msg)
{
    *error_msg = "No match.";

    /* Never match. */
    return 0;
}

/* ipmatch */

#if !defined(WIN32) || !defined(WINNT)
static int msre_op_ipmatch_param_init(msre_rule *rule, char **error_msg) {
    const char *errptr = NULL;
    int erroffset;
    char *data = NULL;
    const char *str = NULL;
    char *saved = NULL;
    static pcre *parse_regex;
    static pcre_extra *parse_regex_study;
    const char *eb;
    int opts = 0;
    int eo, i, res = 0, j = 0;
    int ret = 0;
    int ov[MAX_SUBSTRINGS];
    const char *str_ptr = NULL;
    const char *mask = NULL;
    const char *type = NULL;
    int ipv = 0;
    struct in_addr addr,netmask;
    struct sockaddr_in6 sa;
    struct sockaddr_in6 mask6;
    msre_ipmatch *ipdata = NULL, *curr_ipmatch = NULL;
    unsigned long ipmask = 0, network = 0, hostmask = 0, broadcast = 0;
    int maskbits = 0;

    parse_regex = pcre_compile(PARSE_REGEX_IP, opts, &eb, &eo, NULL);

    if(parse_regex == NULL) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Error compiling ipmatch operator regex",
                erroffset, errptr);
        return 0;
    }

    parse_regex_study = pcre_study(parse_regex, 0, &eb);
    if(eb != NULL)  {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Error ipmatch operator: pcre_study",
                erroffset, errptr);
        if(parse_regex != NULL) pcre_free(parse_regex);
        return 0;
    }

    data = apr_pstrdup(rule->ruleset->mp, rule->op_param);

    if(strlen(data) < 7)    {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Size is too small. Must enter at least an ip address",
                erroffset, errptr);
        if(parse_regex != NULL) pcre_free(parse_regex);
        return 0;

    }

    str = apr_strtok(data,",",&saved);

    while( str != NULL)  {

        ret = pcre_exec(parse_regex, parse_regex_study, str, strlen(str),
                0, 0, ov, MAX_SUBSTRINGS);
        if (ret < 1) {
            *error_msg = apr_psprintf(rule->ruleset->mp, "Error ipmatch operator: pcre_exec",
                    erroffset, errptr);
            if(parse_regex != NULL) pcre_free(parse_regex);
            return 0;
        }

        for (i = 0; i < (ret - 1); i++) {

            res = pcre_get_substring((char *)str, ov, MAX_SUBSTRINGS,i + 1,
                    &str_ptr);
            if (res < 0) {
                *error_msg = apr_psprintf(rule->ruleset->mp, "Error ipmatch operator: pcre_get_substring",
                        erroffset, errptr);
                if(parse_regex != NULL) pcre_free(parse_regex);
                return 0;
            }

            if(strlen(str_ptr) > 0) {

                type = strchr(str_ptr,':');

                if(type != NULL)
                    ipv = 6;

                type = strchr(str_ptr,'.');

                if(type != NULL)
                    ipv = 4;

                mask = strchr(str_ptr,'/');

                if(mask == NULL)    {

                    if(ipv == 4)    {
                        if (!inet_aton(str_ptr,&addr)) {
                            *error_msg = apr_psprintf(rule->ruleset->mp, "Invalid ip address",
                                    erroffset, errptr);
                            if(parse_regex != NULL) pcre_free(parse_regex);
                            return 0;
                        }

                        network = ntohl(addr.s_addr) -1;
                        broadcast = ntohl(addr.s_addr) + 1;
                    } else if (ipv == 6)    {
                        if (inet_pton(AF_INET6, str_ptr, &(sa.sin6_addr)) != 1) {
                            *error_msg = apr_psprintf(rule->ruleset->mp, "Invalid ip address",
                                    erroffset, errptr);
                            if(parse_regex != NULL) pcre_free(parse_regex);
                            return 0;
                        }

                        j = 0;

                        maskbits = 128;

                        while (maskbits >= 8) {
#ifdef LINUX
                            mask6.sin6_addr.__in6_u.__u6_addr8[j++] = 0xff;
#elif SOLARIS2
                            mask6.sin6_addr.u6_addr.u6_addr8[j++] = 0xff;
#else
                            mask6.sin6_addr.__u6_addr.__u6_addr8[j++] = 0xff;
#endif
                            maskbits -= 8;
                        }

                        while (maskbits-- > 0) {
#ifdef LINUX
                            mask6.sin6_addr.__in6_u.__u6_addr8[j] >>= 1;
                            mask6.sin6_addr.__in6_u.__u6_addr8[j] |= 0x80;
#elif SOLARIS2
                            mask6.sin6_addr.u_addr.u6_addr8[j] >>= 1;
                            mask6.sin6_addr.u_addr.u6_addr8[j] |= 0x80;
#else
                            mask6.sin6_addr.__u6_addr.__u6_addr8[j] >>= 1;
                            mask6.sin6_addr.__u6_addr.__u6_addr8[j] |= 0x80;
#endif
                        }

                        j++;

                       while (j < 16) {
#ifdef LINUX
                            mask6.sin6_addr.__in6_u.__u6_addr8[j++] = 0;
#elif SOLARIS2
                            mask6.sin6_addr.u_addr.u6_addr8[j++] = 0;
#else
                            mask6.sin6_addr.__u6_addr.__u6_addr8[j++] = 0;
#endif
                        }

                        for (j = 0; j < 4; j++) {
#ifdef LINUX
                            sa.sin6_addr.__in6_u.__u6_addr32[j]  &= mask6.sin6_addr.__in6_u.__u6_addr32[j];
#elif SOLARIS2
                            sa.sin6_addr.u6_addr.u6_addr32[j]  &= mask6.sin6_addr.u6_addr.u6_addr32[j];
#else
                            sa.sin6_addr.__u6_addr.__u6_addr32[j]  &= mask6.sin6_addr.__u6_addr.__u6_addr32[j];
#endif
                        }

                    }

                } else  {
                    *mask++;
                    maskbits = atoi(mask);
                    network = 0;
                    broadcast = 0;

                    if(ipv == 4)    {
                        if(maskbits >= 1 && maskbits <= 30) {
                            ipmask = 0;
                            for (j=0 ; j<maskbits; j++ )
                                ipmask |= 1<<(31-j);
                            netmask.s_addr = htonl(ipmask);

                            hostmask = ~ntohl(netmask.s_addr);
                            broadcast = network | hostmask;

                            network = ntohl(addr.s_addr) & ntohl(netmask.s_addr);
                        }
                    } else if (ipv == 6)    {

                        if (inet_pton(AF_INET6, str_ptr, &(sa.sin6_addr)) != 1) {
                            *error_msg = apr_psprintf(rule->ruleset->mp, "Invalid ip address",
                                    erroffset, errptr);
                            if(parse_regex != NULL) pcre_free(parse_regex);
                            return 0;
                        }

                        j = 0;

                        if(maskbits >= 1 && maskbits <= 128)    {

                            while (maskbits >= 8) {
#ifdef LINUX
                                mask6.sin6_addr.__in6_u.__u6_addr8[j++] = 0xff;
#elif SOLARIS2
                                mask6.sin6_addr.u6_addr.u6_addr8[j++] = 0xff;
#else
                                mask6.sin6_addr.__u6_addr.__u6_addr8[j++] = 0xff;
#endif
                                maskbits -= 8;
                            }
                            while (maskbits-- > 0) {
#ifdef LINUX
                                mask6.sin6_addr.__in6_u.__u6_addr8[j] >>= 1;
                                mask6.sin6_addr.__in6_u.__u6_addr8[j] |= 0x80;
#elif SOLARIS2
                                mask6.sin6_addr.u6_addr.u6_addr8[j] >>= 1;
                                mask6.sin6_addr.u6_addr.u6_addr8[j] |= 0x80;
#else
                                mask6.sin6_addr.__u6_addr.__u6_addr8[j] >>= 1;
                                mask6.sin6_addr.__u6_addr.__u6_addr8[j] |= 0x80;
#endif
                            }

                            j++;

                           while (j < 16) {
#ifdef LINUX
                                mask6.sin6_addr.__in6_u.__u6_addr8[j++] = 0;
#elif SOLARIS2
                                mask6.sin6_addr.u6_addr.u6_addr8[j++] = 0;
#else
                                mask6.sin6_addr.__u6_addr.__u6_addr8[j++] = 0;
#endif
                            }


                            for (j = 0; j < 4; j++) {
#ifdef LINUX
                                sa.sin6_addr.__in6_u.__u6_addr32[j]  &= mask6.sin6_addr.__in6_u.__u6_addr32[j];
#elif SOLARIS2
                                sa.sin6_addr.u6_addr.u6_addr32[j]  &= mask6.sin6_addr.u6_addr.u6_addr32[j];
#else
                                sa.sin6_addr.__u6_addr.__u6_addr32[j]  &= mask6.sin6_addr.__u6_addr.__u6_addr32[j];
#endif
                            }

                        }
                    }
                }

            }

        }

        if(rule->ip_op == NULL) {
            rule->ip_op = apr_pcalloc(rule->ruleset->mp, sizeof(msre_ipmatch));

            if(rule->ip_op != NULL) {

                if(ipv == 4)    {
                    rule->ip_op->start = network+1;
                    rule->ip_op->type = 4;
                    if(mask == NULL)
                        rule->ip_op->end = rule->ip_op->start;
                    else
                        rule->ip_op->end = rule->ip_op->start + broadcast - 2;

                    rule->ip_op->netaddr = NULL;
                    rule->ip_op->maskaddr = NULL;
                } else if (ipv == 6)    {
                    rule->ip_op->type = 6;
                    rule->ip_op->start = rule->ip_op->end = 0;
                    rule->ip_op->netaddr = &sa;
                    rule->ip_op->maskaddr = &mask6;
                }
                rule->ip_op->next = NULL;
            } else  {
                *error_msg = apr_psprintf(rule->ruleset->mp, "Error allocating list for ip match",
                        erroffset, errptr);
                if(parse_regex != NULL) pcre_free(parse_regex);
                return 0;
            }
        } else  {
            ipdata = apr_pcalloc(rule->ruleset->mp, sizeof(msre_ipmatch));

            if(ipdata != NULL)  {

                curr_ipmatch = rule->ip_op;

                while (curr_ipmatch->next != NULL)    {
                    curr_ipmatch = curr_ipmatch->next;
                }

                if(ipv == 4)    {
                    ipdata->type = 4;
                    ipdata->start = network+1;
                    if(mask == NULL)
                        ipdata->end = ipdata->start;
                    else
                        ipdata->end = ipdata->start + broadcast - 2;

                    ipdata->netaddr = NULL;
                    ipdata->maskaddr = NULL;
                } else if (ipv == 6)    {
                    ipdata->type = 6;
                    ipdata->start = ipdata->end = 0;
                    ipdata->netaddr = &sa;
                    ipdata->maskaddr = &mask6;
                }

                curr_ipmatch->next = ipdata;

            } else  {
                *error_msg = apr_psprintf(rule->ruleset->mp, "Error allocating memory for ip data",
                        erroffset, errptr);
            }
        }

        str = apr_strtok(NULL,",",&saved);
    }

    return 1;
}

static int msre_op_ipmatch_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    const char *errptr = NULL;
    int erroffset;
    struct in_addr addr;
    struct sockaddr_in6 sa;
    unsigned long ipaddr;
    int i;
    msre_ipmatch *ipdata = rule->ip_op;

    if(var == NULL || (strcmp(var->name,"REMOTE_ADDR") != 0 ) || (strcmp(var->name,"SERVER_ADDR") != 0 ))  {
        msr_log(msr,9,"Operator ipmatch only works with REMOTE_ADDR and SERVER_ADDR variable");
        return -1;
    }

    if(ipdata == NULL) {
        msr_log(msr,9,"Ipmatch operator struct is NULL");
        return -1;
    }

    if (!inet_aton(var->value,&addr)) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Invalid ip address",
                erroffset, errptr);
        return -1;
    }

    ipaddr = ntohl(addr.s_addr) -1;

    for (; ipdata != NULL; ipdata = ipdata->next) {
        if(ipdata->type == 4)   {

            if (!inet_aton(var->value,&addr)) {
                *error_msg = apr_psprintf(rule->ruleset->mp, "Invalid ip address",
                        erroffset, errptr);
                return -1;
            }

            ipaddr = ntohl(addr.s_addr) -1;

            if( ipaddr >= ipdata->start && ipaddr <= ipdata->end)
                return 1;

        } else if (ipdata->type == 6)   {
            if (inet_pton(AF_INET6, var->value, &(sa.sin6_addr)) != 1)  {
                *error_msg = apr_psprintf(rule->ruleset->mp, "Invalid ip6 address",
                        erroffset, errptr);
                return -1;
            }

            if(ipdata->netaddr != NULL && ipdata->maskaddr != NULL) {

                for (i = 0; i < 16; i++)
                {
#ifdef LINUX
                    if (((sa.sin6_addr.__in6_u.__u6_addr8[i] ^ ipdata->netaddr->sin6_addr.__in6_u.__u6_addr8[i]) &
                                ipdata->netaddr->sin6_addr.__in6_u.__u6_addr8[i]) == 0)
#elif SOLARIS2
                    if (((sa.sin6_addr.u6_addr.u6_addr8[i] ^ ipdata->netaddr->sin6_addr.u6_addr.u6_addr8[i]) &
                                ipdata->netaddr->sin6_addr.__u6_addr.__u6_addr8[i]) == 0)
#else
                    if (((sa.sin6_addr.__u6_addr.__u6_addr8[i] ^ ipdata->netaddr->sin6_addr.__u6_addr.__u6_addr8[i]) &
                                ipdata->netaddr->sin6_addr.__u6_addr.__u6_addr8[i]) == 0)
#endif
                        return 1;
                }
            }

        }
    }

    return 0;
}
#endif /* WIN32 | WINNT */

/* rsub */

static int msre_op_rsub_param_init(msre_rule *rule, char **error_msg) {
    const char *errptr = NULL;
    int erroffset;
    ap_regex_t *regex;
    const char *pattern = NULL;
    const char *line = NULL;
    char *reg_pattern = NULL;
    char *replace = NULL;
    char *flags = NULL;
    char *data;
    char delim;
    int ignore_case = 0;

    line = rule->op_param;

    if (apr_tolower(*line) != 's') {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Error rsub operator format, must be s/// pattern",
                erroffset, errptr);
        return 0;
    }

    data = apr_pstrdup(rule->ruleset->mp, line);
    delim = *++data;
    if (delim)
        reg_pattern = ++data;
    if (reg_pattern) {
        if (*data != delim) {
            while (*++data && *data != delim);
        }
        if (*data) {
            *data = '\0';
            replace = ++data;
        }
    }
    if (replace) {
        if (*data != delim) {
            while (*++data && *data != delim);
        }
        if (*data) {
            *data = '\0';
            flags = ++data;
        }
    }

    if (!delim || !reg_pattern || !*reg_pattern || !replace) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Error rsub operator format - must be s/regex/str/[flags]",
                erroffset, errptr);
        return 0;
    }

    if (flags) {
        while (*flags) {
            delim = apr_tolower(*flags);
            if (delim == 'i')
                ignore_case = 1;
            else
                *error_msg = apr_psprintf(rule->ruleset->mp, "Regex flag not supported",
                        erroffset, errptr);
            flags++;
        }
    }

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    pattern = apr_pstrdup(rule->ruleset->mp, reg_pattern);
    rule->sub_str = apr_pstrdup(rule->ruleset->mp, replace);

    regex = ap_pregcomp(rule->ruleset->mp, pattern, AP_REG_EXTENDED |
            (ignore_case ? AP_REG_ICASE : 0));

    rule->sub_regex = regex;
    return 1; /* OK */
}

static int msre_op_rsub_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    ap_regex_t *regex = rule->sub_regex;
    char *offset = NULL;
    int sub = 0, so = 0, p_len = 0;
    char *replace = NULL;
    char *data = NULL;
    int size = var->value_len;
    int output_body = 0, input_body = 0, count = 0;
    ap_regmatch_t pmatch[AP_MAX_REG_MATCH];


    if(strcmp(var->name,"STREAM_OUTPUT_BODY") == 0 )  {
        output_body = 1;
    } else if(strcmp(var->name,"STREAM_INPUT_BODY") == 0 )  {
        input_body = 1;
    } else  {
        msr_log(msr,9,"Operator rsub only works with STREAM_* variables");
        return -1;
    }

    replace = apr_pstrdup(rule->ruleset->mp, rule->sub_str);;
    data = apr_pcalloc(msr->mp, var->value_len+(AP_MAX_REG_MATCH*strlen(replace))+1);

    if(replace == NULL || data == NULL)
        return 0;

    memcpy(data,var->value,var->value_len);
    size += (AP_MAX_REG_MATCH*strlen(replace)+2);

    if (ap_regexec(rule->sub_regex,  data ,AP_MAX_REG_MATCH, pmatch, 0)) return 0;

    for (offset = replace; *offset; offset++)
        if (*offset == '\\' && *(offset + 1) > '0' && *(offset + 1) <= '9') {
            so = pmatch [*(offset + 1) - 48].rm_so;
            p_len = pmatch [*(offset + 1) - 48].rm_eo - so;
            if (so < 0 || strlen (replace) + p_len - 1 > size) return 0;
            memmove (offset + p_len, offset + 2, strlen (offset) - 1);
            memmove (offset, data + so, p_len);
            offset = offset + p_len - 2;
        }

    sub = pmatch [1].rm_so;

    for (offset = data; !ap_regexec(rule->sub_regex,  offset, 1, pmatch, 0); ) {
        p_len = pmatch [0].rm_eo - pmatch [0].rm_so;
        count++;
        offset += pmatch [0].rm_so;
        if (var->value_len - p_len + strlen(replace) + 1 > size) return 0;
        memmove (offset + strlen (replace), offset + p_len, strlen (offset) - p_len + 1);
        memmove (offset, replace, strlen (replace));
        offset += strlen (replace);
    }

    size -= ((AP_MAX_REG_MATCH - count)*(strlen(replace)) + ((strlen(replace) - p_len)*(count+AP_MAX_REG_MATCH) - (AP_MAX_REG_MATCH+4)));

    if(msr->stream_output_data != NULL && output_body == 1) {
        msr->stream_output_data = (char *)realloc(msr->stream_output_data,size);
        msr->stream_output_length = size;
        if (msr->stream_output_data != NULL)    {
            memset(msr->stream_output_data,0,size);
            memcpy(msr->stream_output_data,data,size);
            msr->stream_output_data[msr->stream_output_length] = '\0';
        }
    }

    if(msr->stream_input_data != NULL && input_body == 1) {
        msr->stream_input_data = (char *)realloc(msr->stream_input_data,size);
        msr->stream_input_length = size;
        if (msr->stream_input_data != NULL) {
            memset(msr->stream_input_data,0,size);
            memcpy(msr->stream_input_data,data,size);
            msr->stream_input_data[msr->stream_input_length] = '\0';
        }
    }

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
    regex = msc_pregcomp_ex(rule->ruleset->mp, pattern, PCRE_DOTALL | PCRE_DOLLAR_ENDONLY, &errptr, &erroffset, msc_pcre_match_limit, msc_pcre_match_limit_recursion);
    if (regex == NULL) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Error compiling pattern (offset %d): %s",
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
    int capture = 0;
    int matched_bytes = 0;
    int matched = 0;
    int rc;
    char *qspos = NULL;
    const char *parm = NULL;
    msc_parm *mparm = NULL;

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

    /* Are we supposed to capture subexpressions? */
    capture = apr_table_get(rule->actionset->actions, "capture") ? 1 : 0;
    matched_bytes = apr_table_get(rule->actionset->actions, "sanitizeMatchedBytes") ? 1 : 0;
    matched = apr_table_get(rule->actionset->actions, "sanitizeMatched") ? 1 : 0;

    /* Show when the regex captures but "capture" is not set */
    if (msr->txcfg->debuglog_level >= 6) {
        int capcount = 0;
        rc = msc_fullinfo(regex, PCRE_INFO_CAPTURECOUNT, &capcount);
        if (msr->txcfg->debuglog_level >= 6) {
            if ((capture == 0) && (capcount > 0)) {
                msr_log(msr, 6, "Ignoring regex captures since \"capture\" action is not enabled.");
            }
        }
    }

    /* We always use capture so that ovector can be used as working space
     * and no memory has to be allocated for any backreferences.
     */
    rc = msc_regexec_capture(regex, target, target_length, ovector, 30, &my_error_msg);
    if ((rc == PCRE_ERROR_MATCHLIMIT) || (rc == PCRE_ERROR_RECURSIONLIMIT)) {
        msc_string *s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));

        if (s == NULL) return -1;
        s->name = apr_pstrdup(msr->mp, "MSC_PCRE_LIMITS_EXCEEDED");
        s->name_len = strlen(s->name);
        s->value = apr_pstrdup(msr->mp, "1");
        s->value_len = 1;
        if ((s->name == NULL)||(s->value == NULL)) return -1;
        apr_table_setn(msr->tx_vars, s->name, (void *)s);

        *error_msg = apr_psprintf(msr->mp,
                "Rule %pp [id \"%s\"][file \"%s\"][line \"%d\"] - "
                "Execution error - "
                "PCRE limits exceeded (%d): %s",
                rule,((rule->actionset != NULL)&&(rule->actionset->id != NULL)) ? rule->actionset->id : "-",
                rule->filename != NULL ? rule->filename : "-",
                rule->line_num,rc, my_error_msg);

        msr_log(msr, 3, "%s.", *error_msg);

        return 0; /* No match. */
    }
    else if (rc < -1) {
        *error_msg = apr_psprintf(msr->mp, "Regex execution failed (%d): %s",
                rc, my_error_msg);
        return -1;
    }

    /* Handle captured subexpressions. */
    if (capture && rc > 0) {
        int i;

        /* Unset any of the previously set capture variables. */        
        apr_table_unset(msr->tx_vars, "0");
        apr_table_unset(msr->tx_vars, "1");
        apr_table_unset(msr->tx_vars, "2");
        apr_table_unset(msr->tx_vars, "3");
        apr_table_unset(msr->tx_vars, "4");
        apr_table_unset(msr->tx_vars, "5");
        apr_table_unset(msr->tx_vars, "6");
        apr_table_unset(msr->tx_vars, "7");
        apr_table_unset(msr->tx_vars, "8");
        apr_table_unset(msr->tx_vars, "9");

        /* Use the available captures. */
        for(i = 0; i < rc; i++) {
            msc_string *s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
            if (s == NULL) return -1;
            s->name = apr_psprintf(msr->mp, "%d", i);
            s->name_len = strlen(s->name);
            s->value = apr_pstrmemdup(msr->mp,
                    target + ovector[2 * i], ovector[2 * i + 1] - ovector[2 * i]);

            s->value_len = (ovector[2 * i + 1] - ovector[2 * i]);
            if ((s->name == NULL)||(s->value == NULL)) return -1;

            apr_table_addn(msr->tx_vars, s->name, (void *)s);

            if(((matched == 1) || (matched_bytes == 1)) && (var != NULL) && (var->name != NULL))    {
                qspos = apr_psprintf(msr->mp, "%s", var->name);
                parm = strstr(qspos, ":");
                if (parm != NULL)   {
                    parm++;
                    mparm = apr_palloc(msr->mp, sizeof(msc_parm));
                    if (mparm == NULL)
                        continue;

                    mparm->value = apr_pstrmemdup(msr->mp,s->value,s->value_len);
                    mparm->pad_1 = rule->actionset->arg_min;
                    mparm->pad_2 = rule->actionset->arg_max;
                    apr_table_addn(msr->pattern_to_sanitize, parm, (void *)mparm);
                } else  {
                    mparm = apr_palloc(msr->mp, sizeof(msc_parm));
                    if (mparm == NULL)
                        continue;

                    mparm->value = apr_pstrmemdup(msr->mp,s->value,s->value_len);
                    apr_table_addn(msr->pattern_to_sanitize, qspos, (void *)mparm);
                }
            }

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Added regex subexpression to TX.%d: %s", i,
                        log_escape_nq_ex(msr->mp, s->value, s->value_len));
            }
        }
    }

    if (rc != PCRE_ERROR_NOMATCH) { /* Match. */
        /* We no longer escape the pattern here as it is done when logging */
        char *pattern = apr_pstrdup(msr->mp, regex->pattern);

        /* This message will be logged. */
        if (strlen(pattern) > 252) {
            *error_msg = apr_psprintf(msr->mp, "Pattern match \"%.252s ...\" at %s.",
                    pattern, var->name);
        } else {
            *error_msg = apr_psprintf(msr->mp, "Pattern match \"%s\" at %s.",
                    pattern, var->name);
        }

        return 1;
    }

    /* No match. */
    return 0;
}

/* pm */

static int msre_op_pm_param_init(msre_rule *rule, char **error_msg) {
    ACMP *p;
    const char *phrase;
    const char *next;

    if ((rule->op_param == NULL)||(strlen(rule->op_param) == 0)) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Missing parameter for operator 'pm'.");
        return 0; /* ERROR */
    }

    p = acmp_create(0, rule->ruleset->mp);
    if (p == NULL) return 0;

    phrase = apr_pstrdup(rule->ruleset->mp, rule->op_param);

    /* Loop through phrases */
    /* ENH: Need to allow quoted phrases w/space */
    for (;;) {
        while((apr_isspace(*phrase) != 0) && (*phrase != '\0')) phrase++;
        if (*phrase == '\0') break;
        next = phrase;
        while((apr_isspace(*next) == 0) && (*next != 0)) next++;
        acmp_add_pattern(p, phrase, NULL, NULL, next - phrase);
        phrase = next;
    }
    acmp_prepare(p);
    rule->op_param_data = p;
    return 1;
}

/* pmFromFile */

static int msre_op_pmFromFile_param_init(msre_rule *rule, char **error_msg) {
    char errstr[1024];
    char buf[HUGE_STRING_LEN + 1];
    char *fn;
    char *next;
    char *start;
    char *end;
    const char *rulefile_path;
    apr_status_t rc;
    apr_file_t *fd;
    ACMP *p;

    if ((rule->op_param == NULL)||(strlen(rule->op_param) == 0)) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Missing parameter for operator 'pmFromFile'.");
        return 0; /* ERROR */
    }

    p = acmp_create(0, rule->ruleset->mp);
    if (p == NULL) return 0;

    fn = apr_pstrdup(rule->ruleset->mp, rule->op_param);

    /* Get the path of the rule filename to use as a base */
    rulefile_path = apr_pstrndup(rule->ruleset->mp, rule->filename, strlen(rule->filename) - strlen(apr_filepath_name_get(rule->filename)));

#ifdef DEBUG_CONF
    fprintf(stderr, "Rulefile path: \"%s\"\n", rulefile_path);
#endif

    /* Loop through filenames */
    /* ENH: Need to allow quoted filenames w/space */
    for (;;) {
        const char *rootpath = NULL;
        const char *filepath = NULL;
        int line = 0;

        /* Trim whitespace */
        while((apr_isspace(*fn) != 0) && (*fn != '\0')) fn++;
        if (*fn == '\0') break;
        next = fn;
        while((apr_isspace(*next) == 0) && (*next != '\0')) next++;
        while((apr_isspace(*next) != 0) && (*next != '\0')) *(next++) = '\0';

        /* Add path of the rule filename for a relative phrase filename */
        filepath = fn;
        if (apr_filepath_root(&rootpath, &filepath, APR_FILEPATH_TRUENAME, rule->ruleset->mp) != APR_SUCCESS) {
            /* We are not an absolute path.  It could mean an error, but
             * let that pass through to the open call for a better error */
            apr_filepath_merge(&fn, rulefile_path, fn, APR_FILEPATH_TRUENAME, rule->ruleset->mp);
        }

        /* Open file and read */
        rc = apr_file_open(&fd, fn, APR_READ | APR_BUFFERED | APR_FILE_NOCLEANUP, 0, rule->ruleset->mp);
        if (rc != APR_SUCCESS) {
            *error_msg = apr_psprintf(rule->ruleset->mp, "Could not open phrase file \"%s\": %s", fn, apr_strerror(rc, errstr, 1024));
            return 0;
        }

#ifdef DEBUG_CONF
        fprintf(stderr, "Loading phrase file: \"%s\"\n", fn);
#endif

        /* Read one pattern per line skipping empty/commented */
        for(;;) {
            line++;
            rc = apr_file_gets(buf, HUGE_STRING_LEN, fd);
            if (rc == APR_EOF) break;
            if (rc != APR_SUCCESS) {
                *error_msg = apr_psprintf(rule->ruleset->mp, "Could not read \"%s\" line %d: %s", fn, line, apr_strerror(rc, errstr, 1024));
                return 0;
            }

            /* Trim Whitespace */
            start = buf;
            while ((apr_isspace(*start) != 0) && (*start != '\0')) start++;
            end = buf + strlen(buf);
            if (end > start) end--;
            while ((end > start) && (apr_isspace(*end) != 0)) end--;
            if (end > start) {
                *(++end) = '\0';
            }

            /* Ignore empty lines and comments */
            if ((start == end) || (*start == '#')) continue;

#ifdef DEBUG_CONF
            fprintf(stderr, "Adding phrase file pattern: \"%s\"\n", buf);
#endif

            acmp_add_pattern(p, start, NULL, NULL, (end - start));
        }
        fn = next;
    }
    if (fd != NULL) apr_file_close(fd);
    acmp_prepare(p);
    rule->op_param_data = p;
    return 1;
}

static int msre_op_pm_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    const char *match = NULL;
    apr_status_t rc = 0;
    int capture;
    ACMPT pt;

    /* Nothing to read */
    if ((var->value == NULL) || (var->value_len == 0)) return 0;

    /* Are we supposed to capture subexpressions? */
    capture = apr_table_get(rule->actionset->actions, "capture") ? 1 : 0;

    pt.parser = (ACMP *)rule->op_param_data;
    pt.ptr = NULL;

    rc = acmp_process_quick(&pt, &match, var->value, var->value_len);
    if (rc) {
        char *match_escaped = log_escape(msr->mp, match ? match : "<Unknown Match>");

        /* This message will be logged. */
        if (strlen(match_escaped) > 252) {
            *error_msg = apr_psprintf(msr->mp, "Matched phrase \"%.252s ...\" at %s.",
                    match_escaped, var->name);
        } else {
            *error_msg = apr_psprintf(msr->mp, "Matched phrase \"%s\" at %s.",
                    match_escaped, var->name);
        }

        /* Handle capture as tx.0=match */
        if (capture) {
            int i;
            msc_string *s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));

            if (s == NULL) return -1;

            s->name = "0";
            s->name_len = strlen(s->name);
            s->value = apr_pstrdup(msr->mp, match);
            if (s->value == NULL) return -1;
            s->value_len = strlen(s->value);
            apr_table_setn(msr->tx_vars, s->name, (void *)s);

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Added phrase match to TX.0: %s",
                        log_escape_nq_ex(msr->mp, s->value, s->value_len));
            }

            /* Unset the remaining ones (from previous invocations). */
            for(i = rc; i <= 9; i++) {
                char buf[2];
                apr_snprintf(buf, sizeof(buf), "%d", i);
                apr_table_unset(msr->tx_vars, buf);
            }
        }

        return 1;
    }
    return rc;
}

/* gsbLookup */

static int verify_gsb(gsb_db *gsb, msre_rule *rule, const char *match, unsigned int match_length) {
    apr_md5_ctx_t ctx;
    apr_status_t rc;
    unsigned char digest[APR_MD5_DIGESTSIZE];
    const char *hash = NULL;
    const char *search = NULL;

    memset(digest, 0, sizeof(digest));

    apr_md5_init(&ctx);

    if ((rc = apr_md5_update(&ctx, match, match_length)) != APR_SUCCESS)
        return -1;

    apr_md5_final(digest, &ctx);

    hash = apr_psprintf(rule->ruleset->mp, "%s", bytes2hex(rule->ruleset->mp, digest, 16));

    if ((hash != NULL) && (gsb->gsb_table != NULL))   {
        search = apr_table_get(gsb->gsb_table, hash);

        if (search != NULL)
            return 1;
    }

    return 0;
}

static int msre_op_gsbLookup_param_init(msre_rule *rule, char **error_msg) {
    const char *errptr = NULL;
    int erroffset;
    msc_regex_t *regex;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    /* Compile rule->op_param */
    regex = msc_pregcomp_ex(rule->ruleset->mp, rule->op_param, PCRE_DOTALL | PCRE_MULTILINE, &errptr, &erroffset, msc_pcre_match_limit, msc_pcre_match_limit_recursion);

    if (regex == NULL) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Error compiling pattern (offset %d): %s",
                erroffset, errptr);
        return 0;
    }

    rule->op_param_data = regex;

    return 1; /* OK */
}

static int msre_op_gsbLookup_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    msc_regex_t *regex = (msc_regex_t *)rule->op_param_data;
    char *my_error_msg = NULL;
    int ovector[33];
    int offset = 0;
    gsb_db *gsb = msr->txcfg->gsb;
    const char *match = NULL;
    unsigned int match_length;
    unsigned int canon_length;
    unsigned int base_length;
    int rv, i, ret;
    char *data = NULL;
    unsigned int size = var->value_len;
    char *base = NULL, *canon = NULL, *savedptr = NULL;
    char *str = NULL, *entire = NULL;
    int capture;

    if(regex == NULL)    {
        if (msr->txcfg->debuglog_level >= 8) {
            msr_log(msr, 8, "GSB: regex is null");
        }
        return -1;
    }

    if(gsb == NULL)    {
        if (msr->txcfg->debuglog_level >= 8) {
            msr_log(msr, 8, "GSB: gsb is null");
        }
        return -1;
    }

    data = apr_pcalloc(msr->mp, var->value_len+1);

    if(data == NULL)    {
        if (msr->txcfg->debuglog_level >= 8) {
            msr_log(msr, 8, "GSB: Fail to alloc memory for input data");
        }
        return -1;
    }

    capture = apr_table_get(rule->actionset->actions, "capture") ? 1 : 0;

    memcpy(data,var->value,var->value_len);

    while (offset < size && (rv = msc_regexec_ex(regex, data, size, offset, PCRE_NOTEMPTY, ovector, 30, &my_error_msg)) >= 0)
    {
        for(i = 0; i < rv; ++i)
        {
            match = apr_psprintf(rule->ruleset->mp, "%.*s", ovector[2*i+1] - ovector[2*i], data + ovector[2*i]);

            if (match == NULL)  {

                if (msr->txcfg->debuglog_level >= 8) {
                    msr_log(msr, 8, "GSB: Fail to alloc memory for match string");
                }

                return -1;
            }

            match_length = strlen(match);

            if((strstr(match,"http") == NULL) && (match_length > 0) && (strchr(match,'.')))    {

                /* full url */
                if (msr->txcfg->debuglog_level >= 4) {
                    msr_log(msr, 4, "GSB: Successfully extracted url: %s", match);
                }

                ret = verify_gsb(gsb, rule, match, match_length);

                if(ret > 0) {
                    set_match_to_tx(msr, capture, match);
                    if (! *error_msg) {
                        *error_msg = apr_psprintf(msr->mp, "Gsb lookup for \"%s\" succeeded.",
                                log_escape_nq(msr->mp, match));
                    }
                    return 1;
                }

                /* append / in the end of full url */
                if ((match[match_length -1] != '/') && (strchr(match,'?') == NULL))    {

                    canon = apr_psprintf(rule->ruleset->mp, "%s/", match);
                    if (canon != NULL)  {

                        if (msr->txcfg->debuglog_level >= 4) {
                            msr_log(msr, 4, "GSB: Canonicalize url #1: %s", canon);
                        }

                        canon_length = strlen(canon);
                        ret = verify_gsb(gsb, rule, canon, canon_length);

                        if(ret > 0) {
                            set_match_to_tx(msr, capture, canon);
                            if (! *error_msg) {
                                *error_msg = apr_psprintf(msr->mp, "Gsb lookup for \"%s\" succeeded.",
                                        log_escape_nq(msr->mp, canon));
                            }
                            return 1;
                        }
                    }
                }

                str = apr_pstrdup(rule->ruleset->mp,match);

                /* base url */
                if (str != NULL)    {

                    base = apr_strtok(str,"/",&savedptr);

                    if (base != NULL && (strlen(match) != (strlen(base)+1)))   {

                        canon = apr_psprintf(rule->ruleset->mp, "%s/", base);

                        if (canon != NULL)  {

                            if (msr->txcfg->debuglog_level >= 4) {
                                msr_log(msr, 4, "GSB: Canonicalize url #2: %s", canon);
                            }

                            canon_length = strlen(canon);
                            ret = verify_gsb(gsb, rule, canon, canon_length);

                            if(ret > 0) {
                                set_match_to_tx(msr, capture, canon);
                                if (! *error_msg) {
                                    *error_msg = apr_psprintf(msr->mp, "Gsb lookup for \"%s\" succeeded.",
                                            log_escape_nq(msr->mp, canon));
                                }
                                return 1;
                            }
                        }

                    }

                }
            }
        }

        offset = ovector[1];
    }

    return 0;
}

/* within */

static int msre_op_within_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    msc_string *str = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    const char *match = NULL;
    const char *target;
    unsigned int match_length;
    unsigned int target_length = 0;
    unsigned int i, i_max;

    str->value = (char *)rule->op_param;
    str->value_len = strlen(str->value);

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (str->value == NULL) {
        *error_msg = "Internal Error: match string is null.";
        return -1;
    }

    expand_macros(msr, str, rule, msr->mp);

    match = (const char *)str->value;
    match_length = str->value_len;

    /* If the given target is null we give up without a match */
    if (var->value == NULL) {
        /* No match. */
        return 0;
    }

    target = var->value;
    target_length = var->value_len;

    /* The empty string always matches */
    if (target_length == 0) {
        /* Match. */
        *error_msg = apr_psprintf(msr->mp, "String match within \"\" at %s.",
                var->name);
        return 1;
    }

    /* This is impossible to match */
    if (target_length > match_length) {
        /* No match. */
        return 0;
    }

    /* scan for first character, then compare from there until we
     * have a match or there is no room left in the target
     */
    i_max = match_length - target_length;
    for (i = 0; i <= i_max; i++) {
        if (match[i] == target[0]) {
            if (memcmp((target + 1), (match + i + 1), (target_length - 1)) == 0) {
                /* match. */
                *error_msg = apr_psprintf(msr->mp, "String match within \"%s\" at %s.",
                        log_escape_ex(msr->mp, match, match_length),
                        var->name);
                return 1;
            }
        }
    }

    /* No match. */
    return 0;
}

/* contains */

static int msre_op_contains_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    msc_string *str = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    const char *match = NULL;
    const char *target;
    unsigned int match_length;
    unsigned int target_length = 0;
    unsigned int i, i_max;

    str->value = (char *)rule->op_param;
    str->value_len = strlen(str->value);

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (str->value == NULL) {
        *error_msg = "Internal Error: match string is null.";
        return -1;
    }

    expand_macros(msr, str, rule, msr->mp);

    match = (const char *)str->value;
    match_length = str->value_len;

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

    /* The empty string always matches */
    if (match_length == 0) {
        /* Match. */
        *error_msg = apr_psprintf(msr->mp, "String match \"\" at %s.", var->name);
        return 1;
    }

    /* This is impossible to match */
    if (match_length > target_length) {
        /* No match. */
        return 0;
    }

    /* scan for first character, then compare from there until we
     * have a match or there is no room left in the target
     */
    i_max = target_length - match_length;
    for (i = 0; i <= i_max; i++) {
        /* First character matched - avoid func call */
        if (target[i] == match[0]) {
            /* See if remaining matches */
            if (   (match_length == 1)
                    || (memcmp((match + 1), (target + i + 1), (match_length - 1)) == 0))
            {
                /* Match. */
                *error_msg = apr_psprintf(msr->mp, "String match \"%s\" at %s.",
                        log_escape_ex(msr->mp, match, match_length),
                        var->name);
                return 1;
            }
        }
    }

    /* No match. */
    return 0;
}

/* containsWord */

static int msre_op_containsWord_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    msc_string *str = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    const char *match = NULL;
    const char *target;
    unsigned int match_length;
    unsigned int target_length = 0;
    unsigned int i, i_max;
    int rc = 0;

    str->value = (char *)rule->op_param;
    str->value_len = strlen(str->value);

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (str->value == NULL) {
        *error_msg = "Internal Error: match string is null.";
        return -1;
    }

    expand_macros(msr, str, rule, msr->mp);

    match = (const char *)str->value;
    match_length = str->value_len;

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

    /* The empty string always matches */
    if (match_length == 0) {
        /* Match. */
        *error_msg = apr_psprintf(msr->mp, "String match \"\" at %s.", var->name);
        return 1;
    }

    /* This is impossible to match */
    if (match_length > target_length) {
        /* No match. */
        return 0;
    }

    /* scan for first character, then compare from there until we
     * have a match or there is no room left in the target
     */
    i_max = target_length - match_length;
    for (i = 0; i <= i_max; i++) {

        /* Previous char must have been a start or non-word */
        if ((i > 0) && (apr_isalnum(target[i-1])||(target[i-1] == '_')))
            continue;

        /* First character matched - avoid func call */
        if (target[i] == match[0]) {
            /* See if remaining matches */
            if (   (match_length == 1)
                    || (memcmp((match + 1), (target + i + 1), (match_length - 1)) == 0))
            {
                /* check boundaries */
                if (i == i_max) {
                    /* exact/end word match */
                    rc = 1;
                }
                else if (!(apr_isalnum(target[i + match_length])||(target[i + match_length] == '_'))) {
                    /* start/mid word match */
                    rc = 1;
                }
            }
        }
    }

    if (rc == 1) {
        /* Maybe a match. */
        *error_msg = apr_psprintf(msr->mp, "String match \"%s\" at %s.",
                log_escape_ex(msr->mp, match, match_length),
                var->name);
        return 1;
    }

    /* No match. */
    *error_msg = NULL;
    return 0;
}

/* streq */

static int msre_op_streq_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    msc_string *str = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    const char *match = NULL;
    const char *target;
    unsigned int match_length;
    unsigned int target_length;

    str->value = (char *)rule->op_param;
    str->value_len = strlen(str->value);

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (str->value == NULL) {
        *error_msg = "Internal Error: match string is null.";
        return -1;
    }

    expand_macros(msr, str, rule, msr->mp);

    match = (const char *)str->value;
    match_length = str->value_len;

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

    /* These are impossible to match */
    if (match_length != target_length) {
        /* No match. */
        return 0;
    }

    if (memcmp(match, target, target_length) == 0) {
        /* Match. */
        *error_msg = apr_psprintf(msr->mp, "String match \"%s\" at %s.",
                log_escape_ex(msr->mp, match, match_length),
                var->name);
        return 1;
    }

    /* No match. */
    return 0;
}

/* beginsWith */

static int msre_op_beginsWith_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    msc_string *str = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    const char *match = NULL;
    const char *target;
    unsigned int match_length;
    unsigned int target_length;

    str->value = (char *)rule->op_param;
    str->value_len = strlen(str->value);

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (str->value == NULL) {
        *error_msg = "Internal Error: match string is null.";
        return -1;
    }

    expand_macros(msr, str, rule, msr->mp);

    match = (const char *)str->value;
    match_length = str->value_len;

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

    /* The empty string always matches */
    if (match_length == 0) {
        /* Match. */
        *error_msg = apr_psprintf(msr->mp, "String match \"\" at %s.", var->name);
        return 1;
    }

    /* This is impossible to match */
    if (match_length > target_length) {
        /* No match. */
        return 0;
    }

    if (memcmp(match, target, match_length) == 0) {
        /* Match. */
        *error_msg = apr_psprintf(msr->mp, "String match \"%s\" at %s.",
                log_escape_ex(msr->mp, match, match_length),
                var->name);
        return 1;
    }

    /* No match. */
    return 0;
}

/* endsWith */

static int msre_op_endsWith_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    msc_string *str = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    const char *match = NULL;
    const char *target;
    unsigned int match_length;
    unsigned int target_length;

    str->value = (char *)rule->op_param;
    str->value_len = strlen(str->value);

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (str->value == NULL) {
        *error_msg = "Internal Error: match string is null.";
        return -1;
    }

    expand_macros(msr, str, rule, msr->mp);

    match = (const char *)str->value;
    match_length = str->value_len;

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

    /* The empty string always matches */
    if (match_length == 0) {
        /* Match. */
        *error_msg = apr_psprintf(msr->mp, "String match \"\" at %s.", var->name);
        return 1;
    }

    /* This is impossible to match */
    if (match_length > target_length) {
        /* No match. */
        return 0;
    }

    if (memcmp(match, (target + (target_length - match_length)), match_length) == 0) {
        /* Match. */
        *error_msg = apr_psprintf(msr->mp, "String match \"%s\" at %s.",
                log_escape_ex(msr->mp, match, match_length),
                var->name);
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
        *error_msg = apr_psprintf(msr->mp,
                "XML document tree could not be found for DTD validation.");
        return -1;
    }

    if (msr->xml->well_formed != 1) {
        *error_msg = apr_psprintf(msr->mp,
                "XML: DTD validation failed because content is not well formed.");
        return 1;
    }

    /* Make sure there were no other generic processing errors */
    if (msr->msc_reqbody_error) {
        *error_msg = apr_psprintf(msr->mp,
                "XML: DTD validation could not proceed due to previous"
                " processing errors.");
        return 1;
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

    /* Send validator errors/warnings to msr_log */
    /* NOTE: No xmlDtdSetValidErrors()? */
    cvp->error = (xmlSchemaValidityErrorFunc)msr_log_error;
    cvp->warning = (xmlSchemaValidityErrorFunc)msr_log_warn;
    cvp->userData = msr;

    if (!xmlValidateDtd(cvp, msr->xml->doc, dtd)) {
        *error_msg = "XML: DTD validation failed.";
        xmlFreeValidCtxt(cvp);
        xmlFreeDtd(dtd);
        return 1; /* No match. */
    }

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "XML: Successfully validated payload against DTD: %s", rule->op_param);
    }

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
        *error_msg = apr_psprintf(msr->mp,
                "XML document tree could not be found for schema validation.");
        return -1;
    }

    if (msr->xml->well_formed != 1) {
        *error_msg = apr_psprintf(msr->mp,
                "XML: Schema validation failed because content is not well formed.");
        return 1;
    }

    /* Make sure there were no other generic processing errors */
    if (msr->msc_reqbody_error) {
        *error_msg = apr_psprintf(msr->mp,
                "XML: Schema validation could not proceed due to previous"
                " processing errors.");
        return 1;
    }

    parserCtx = xmlSchemaNewParserCtxt(rule->op_param); /* ENH support relative filenames */
    if (parserCtx == NULL) {
        *error_msg = apr_psprintf(msr->mp, "XML: Failed to load Schema from file: %s",
                rule->op_param);
        return -1;
    }

    /* Send parser errors/warnings to msr_log */
    xmlSchemaSetParserErrors(parserCtx, (xmlSchemaValidityErrorFunc)msr_log_error, (xmlSchemaValidityWarningFunc)msr_log_warn, msr);

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

    /* Send validator errors/warnings to msr_log */
    xmlSchemaSetValidErrors(validCtx, (xmlSchemaValidityErrorFunc)msr_log_error, (xmlSchemaValidityWarningFunc)msr_log_warn, msr);

    rc = xmlSchemaValidateDoc(validCtx, msr->xml->doc);
    if (rc != 0) {
        *error_msg = "XML: Schema validation failed.";
        xmlSchemaFree(schema);
        xmlSchemaFreeParserCtxt(parserCtx);
        return 1; /* No match. */
    }

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "XML: Successfully validated payload against Schema: %s", rule->op_param);
    }

    xmlSchemaFree(schema);
    xmlSchemaFreeValidCtxt(validCtx);

    return 0;
}

/* verifyCC */

/**
 * Luhn Mod-10 Method (ISO 2894/ANSI 4.13)
 */
static int luhn_verify(const char *ccnumber, int len) {
    int sum[2] = { 0, 0 };
    int odd = 0;
    int digits = 0;
    int i;

    /* Weighted lookup table which is just a precalculated (i = index):
     *   i*2 + (( (i*2) > 9 ) ? -9 : 0)
     */
    static int wtable[10] = {0, 2, 4, 6, 8, 1, 3, 5, 7, 9}; /* weight lookup table */

    /* Add up only digits (weighted digits via lookup table)
     * for both odd and even CC numbers to avoid 2 passes.
     */
    for (i = 0; i < len; i++) {
        if (apr_isdigit(ccnumber[i])) {
            sum[0] += (!odd ? wtable[ccnumber[i] - '0'] : (ccnumber[i] - '0'));
            sum[1] += (odd ? wtable[ccnumber[i] - '0'] : (ccnumber[i] - '0'));
            odd = 1 - odd; /* alternate weights */
            digits++;
        }
    }

    /* No digits extracted */
    if (digits == 0) return 0;

    /* Do a mod 10 on the sum */
    sum[odd] %= 10;

    /* If the result is a zero the card is valid. */
    return sum[odd] ? 0 : 1;
}

static int msre_op_verifyCC_init(msre_rule *rule, char **error_msg) {
    const char *errptr = NULL;
    int erroffset;
    msc_regex_t *regex;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    /* Compile rule->op_param */
    regex = msc_pregcomp_ex(rule->ruleset->mp, rule->op_param, PCRE_DOTALL | PCRE_MULTILINE, &errptr, &erroffset, msc_pcre_match_limit, msc_pcre_match_limit_recursion);
    if (regex == NULL) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Error compiling pattern (offset %d): %s",
                erroffset, errptr);
        return 0;
    }

    rule->op_param_data = regex;

    return 1; /* OK */
}

static int msre_op_verifyCC_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    msc_regex_t *regex = (msc_regex_t *)rule->op_param_data;
    const char *target;
    unsigned int target_length;
    char *my_error_msg = NULL;
    int ovector[33];
    int rc;
    int is_cc = 0;
    int offset;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (regex == NULL) {
        *error_msg = "Internal Error: regex data is null.";
        return -1;
    }

    memset(ovector, 0, sizeof(ovector));

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

    for (offset = 0; ((unsigned int)offset < target_length) && (is_cc == 0); offset++) {
        if (msr->txcfg->debuglog_level >= 9) {
            if (offset > 0) {
                msr_log(msr, 9, "Continuing CC# search at target offset %d.", offset);
            }
        }

        rc = msc_regexec_ex(regex, target, target_length, offset, PCRE_NOTEMPTY, ovector, 30, &my_error_msg);

        /* If there was no match, then we are done. */
        if (rc == PCRE_ERROR_NOMATCH) {
            break;
        }

        if (rc < -1) {
            *error_msg = apr_psprintf(msr->mp, "CC# regex execution failed: %s", my_error_msg);
            return -1;
        }

        /* Verify a match. */
        if (rc > 0) {
            const char *match = target + ovector[0];
            int length = ovector[1] - ovector[0];
            int i = 0;

            offset = ovector[2*i];

            /* Check the Luhn using the match string */
            is_cc = luhn_verify(match, length);

            /* Not a CC number, then try another match where we left off. */
            if (!is_cc) {
                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "CC# Luhn check failed at target offset %d: \"%.*s\"", offset, length, match);
                }

                continue;
            }

            /* We have a potential CC number and need to set any captures
             * and we are done.
             */

            if (apr_table_get(rule->actionset->actions, "capture")) {
                for(; i < rc; i++) {
                    msc_string *s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
                    if (s == NULL) return -1;
                    s->name = apr_psprintf(msr->mp, "%d", i);
                    s->name_len = strlen(s->name);
                    s->value = apr_pstrmemdup(msr->mp, match, length);
                    s->value_len = length;
                    if ((s->name == NULL)||(s->value == NULL)) return -1;

                    apr_table_setn(msr->tx_vars, s->name, (void *)s);

                    if (msr->txcfg->debuglog_level >= 9) {
                        msr_log(msr, 9, "Added regex subexpression to TX.%d: %s", i,
                                log_escape_nq_ex(msr->mp, s->value, s->value_len));
                    }
                }
            }

            /* Unset the remaining TX vars (from previous invocations). */
            for(; i <= 9; i++) {
                char buf[24];
                apr_snprintf(buf, sizeof(buf), "%i", i);
                apr_table_unset(msr->tx_vars, buf);
            }

            break;
        }
    }

    if (is_cc) {
        /* Match. */

        /* This message will be logged. */
        *error_msg = apr_psprintf(msr->mp, "CC# match \"%s\" at %s. [offset \"%d\"]",
                regex->pattern, var->name, offset);

        return 1;
    }

    /* No match. */
    return 0;
}

/*
 * \brief Check for a valid CPF
 *
 * \param cpfnumber Pointer to cpf
 * \param len cpf length
 * \param rule Pointer to the rule
 *
 * \retval 0 On Invalid CPF
 * \retval 1 On Valid CPF
 */
static int cpf_verify(const char *cpfnumber, int len, msre_rule *rule) {

    int factor, part_1, part_2, var_len = len;
    int sum = 0, i = 0, cpf_len = 11;
    int cpf[11];
    char s_cpf[11];
    char bad_cpf[11][11] = { "00000000000",
        "01234567890",
        "11111111111",
        "22222222222",
        "33333333333",
        "44444444444",
        "55555555555",
        "66666666666",
        "77777777777",
        "88888888888",
        "99999999999"};

    while((*cpfnumber != '\0') && ( var_len >= 0))  {

        if(*cpfnumber != '-' || *cpfnumber != '.') {
            if(i < cpf_len && isdigit(*cpfnumber))  {
                s_cpf[i] = *cpfnumber;
                cpf[i] = convert_to_int(*cpfnumber);
                i++;
            }
        }
        cpfnumber++;
        var_len--;
    }


    if (strlen(s_cpf) != cpf_len || i != cpf_len-1)
        return 0;
    else {
        for(i = 0; i< cpf_len; i++)   {
            if(strncmp(s_cpf,bad_cpf[i],cpf_len) == 0)    {
                return 0;
            }
        }
    }

    part_1 = convert_to_int(s_cpf[cpf_len-2]);
    part_2 = convert_to_int(s_cpf[cpf_len-1]);

    int c = cpf_len;

    for(i = 0; i < 9; i++) {
        sum += (cpf[i] * --c);
    }

    factor = (sum % cpf_len);

    if(factor < 2) {
        cpf[9] = 0;
    } else {
        cpf[9] = cpf_len-factor;
    }

    sum = 0;
    c = cpf_len;

    for(i = 0;i < 10; i++)
        sum += (cpf[i] * c--);

    factor = (sum % cpf_len);

    if(factor < 2) {
        cpf[10] = 0;
    } else {
        cpf[10] = cpf_len-factor;
    }

    if(part_1 == cpf[9] && part_2 == cpf[10])
        return 1;

    return 0;
}

/*
* \brief Init function to CPF operator
*
* \param rule Pointer to the rule
* \param error_msg Pointer to error msg
*
* \retval 0 On Failure
* \retval 1 On Success
*/
static int msre_op_verifyCPF_init(msre_rule *rule, char **error_msg) {
    const char *errptr = NULL;
    int erroffset;
    msc_regex_t *regex;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    /* Compile rule->op_param */
    regex = msc_pregcomp_ex(rule->ruleset->mp, rule->op_param, PCRE_DOTALL | PCRE_MULTILINE, &errptr, &erroffset, msc_pcre_match_limit, msc_pcre_match_limit_recursion);
    if (regex == NULL) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Error compiling pattern (offset %d): %s",
            erroffset, errptr);
        return 0;
    }

    rule->op_param_data = regex;

    return 1; /* OK */
}

/*
* \brief Execution function to CPF operator
*
* \param msr Pointer internal modsec request structure
* \param rule Pointer to the rule
* \param var Pointer to variable structure
* \param error_msg Pointer to error msg
*
* \retval -1 On Failure
* \retval 1 On Match
* \retval 0 On No Match
*/
static int msre_op_verifyCPF_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    msc_regex_t *regex = (msc_regex_t *)rule->op_param_data;
    const char *target;
    unsigned int target_length;
    char *my_error_msg = NULL;
    int ovector[33];
    int rc;
    int is_cpf = 0;
    int offset;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (regex == NULL) {
        *error_msg = "Internal Error: regex data is null.";
        return -1;
    }

    memset(ovector, 0, sizeof(ovector));

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

    for (offset = 0; ((unsigned int)offset < target_length) && (is_cpf == 0); offset++) {
        if (msr->txcfg->debuglog_level >= 9) {
            if (offset > 0) {
                msr_log(msr, 9, "Continuing CPF# search at target offset %d.", offset);
            }
        }

        rc = msc_regexec_ex(regex, target, target_length, offset, PCRE_NOTEMPTY, ovector, 30, &my_error_msg);

        /* If there was no match, then we are done. */
        if (rc == PCRE_ERROR_NOMATCH) {
            break;
        }

        if (rc < -1) {
            *error_msg = apr_psprintf(msr->mp, "CPF# regex execution failed: %s", my_error_msg);
            return -1;
        }

        /* Verify a match. */
        if (rc > 0) {
            const char *match = target + ovector[0];
            int length = ovector[1] - ovector[0];
            int i = 0;

            offset = ovector[2*i];

            /* Check CPF using the match string */
            is_cpf = cpf_verify(match, length, rule);

            /* Not a CPF number, then try another match where we left off. */
            if (!is_cpf) {
                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "CPF# check failed at target offset %d: \"%.*s\"", offset, length, match);
                }

                continue;
            }

            /* We have a potential CPF number and need to set any captures
             * and we are done.
             */

            if (apr_table_get(rule->actionset->actions, "capture")) {
                for(; i < rc; i++) {
                    msc_string *s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
                    if (s == NULL) return -1;
                    s->name = apr_psprintf(msr->mp, "%d", i);
                    s->name_len = strlen(s->name);
                    s->value = apr_pstrmemdup(msr->mp, match, length);
                    s->value_len = length;
                    if ((s->name == NULL)||(s->value == NULL)) return -1;

                    apr_table_setn(msr->tx_vars, s->name, (void *)s);

                    if (msr->txcfg->debuglog_level >= 9) {
                        msr_log(msr, 9, "Added regex subexpression to TX.%d: %s", i,
                            log_escape_nq_ex(msr->mp, s->value, s->value_len));
                    }
                }
            }

            /* Unset the remaining TX vars (from previous invocations). */
            for(; i <= 9; i++) {
                char buf[24];
                apr_snprintf(buf, sizeof(buf), "%i", i);
                apr_table_unset(msr->tx_vars, buf);
            }

            break;
        }
    }

    if (is_cpf) {
        /* Match. */

        /* This message will be logged. */
        *error_msg = apr_psprintf(msr->mp, "CPF# match \"%s\" at %s. [offset \"%d\"]",
            regex->pattern, var->name, offset);

        return 1;
    }

    /* No match. */
    return 0;
}

/*
 * \brief Check for a valid SSN
 *
 * \param ssnumber Pointer to ssn
 * \param len ssn length
 * \param rule Pointer to the rule
 *
 * \retval 0 On Invalid SSN
 * \retval 1 On Valid SSN
 */
static int ssn_verify(const char *ssnumber, int len, msre_rule *rule) {
    int i;
    int num[9];
    int digits = 0;
    int area, serial, grp;
    int sequencial = 0;
    int repetitions = 0;
    int progression = 0;
    char *str_area;
    char *str_grp;
    char *str_serial;

    for (i = 0; i < len; i++) {
        if (apr_isdigit(ssnumber[i])) {
                num[i] = convert_to_int(ssnumber[i]);
                digits++;
        }
    }

    /* Not a valid number */
    if (digits != 9)
        goto invalid;

    digits = 0;

    for (i=0; i < len-1; i++)   {
        progression = (num[i] - (num[i+1]-1));
        repetitions = (num[i] - num[i+1]);

        if (repetitions != 0 )
            sequencial = 1;

        if (progression == 0)
            digits++;
    }

    /* We are blocking when all numbers were repeated */
    if (sequencial == 0)
        goto invalid;

    if (digits == 8)
        goto invalid;

    str_area = apr_psprintf(rule->ruleset->mp,"%d%d%d",num[0],num[1],num[2]);
    str_grp = apr_psprintf(rule->ruleset->mp,"%d%d",num[3],num[4]);
    str_serial = apr_psprintf(rule->ruleset->mp,"%d%d%d%d",num[5],num[6],num[7],num[8]);

    if(str_area == NULL || str_grp == NULL || str_serial == NULL)
        goto invalid;

    area = atoi(str_area);
    grp = atoi(str_grp);
    serial = atoi(str_serial);

    /* Cannot has seroed fields */
    if (area == 0 || serial == 0 || grp == 0)
        goto invalid;

    /* More tests */
    if (area >= 740 || area == 666)
        goto invalid;

    return 1;

invalid:
    return 0;
}

/*
* \brief Init function to SSN operator
*
* \param rule Pointer to the rule
* \param error_msg Pointer to error msg
*
* \retval 0 On Failure
* \retval 1 On Success
*/
static int msre_op_verifySSN_init(msre_rule *rule, char **error_msg) {
    const char *errptr = NULL;
    int erroffset;
    msc_regex_t *regex;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    /* Compile rule->op_param */
    regex = msc_pregcomp_ex(rule->ruleset->mp, rule->op_param, PCRE_DOTALL | PCRE_MULTILINE, &errptr, &erroffset, msc_pcre_match_limit, msc_pcre_match_limit_recursion);
    if (regex == NULL) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Error compiling pattern (offset %d): %s",
            erroffset, errptr);
        return 0;
    }

    rule->op_param_data = regex;

    return 1; /* OK */
}

/*
* \brief Execution function to SSN operator
*
* \param msr Pointer internal modsec request structure
* \param rule Pointer to the rule
* \param var Pointer to variable structure
* \param error_msg Pointer to error msg
*
* \retval -1 On Failure
* \retval 1 On Match
* \retval 0 On No Match
*/
static int msre_op_verifySSN_execute(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg) {
    msc_regex_t *regex = (msc_regex_t *)rule->op_param_data;
    const char *target;
    unsigned int target_length;
    char *my_error_msg = NULL;
    int ovector[33];
    int rc;
    int is_ssn = 0;
    int offset;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (regex == NULL) {
        *error_msg = "Internal Error: regex data is null.";
        return -1;
    }

    memset(ovector, 0, sizeof(ovector));

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

    for (offset = 0; ((unsigned int)offset < target_length) && (is_ssn == 0); offset++) {
        if (msr->txcfg->debuglog_level >= 9) {
            if (offset > 0) {
                msr_log(msr, 9, "Continuing SSN# search at target offset %d.", offset);
            }
        }

        rc = msc_regexec_ex(regex, target, target_length, offset, PCRE_NOTEMPTY, ovector, 30, &my_error_msg);

        /* If there was no match, then we are done. */
        if (rc == PCRE_ERROR_NOMATCH) {
            break;
        }

        if (rc < -1) {
            *error_msg = apr_psprintf(msr->mp, "SSN# regex execution failed: %s", my_error_msg);
            return -1;
        }

        /* Verify a match. */
        if (rc > 0) {
            const char *match = target + ovector[0];
            int length = ovector[1] - ovector[0];
            int i = 0;

            offset = ovector[2*i];

            /* Check SSN using the match string */
            is_ssn = ssn_verify(match, length, rule);

            /* Not a SSN number, then try another match where we left off. */
            if (!is_ssn) {
                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "SSN# check failed at target offset %d: \"%.*s\"", offset, length, match);
                }

                continue;
            }

            /* We have a potential SSN number and need to set any captures
             * and we are done.
             */

            if (apr_table_get(rule->actionset->actions, "capture")) {
                for(; i < rc; i++) {
                    msc_string *s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
                    if (s == NULL) return -1;
                    s->name = apr_psprintf(msr->mp, "%d", i);
                    s->name_len = strlen(s->name);
                    s->value = apr_pstrmemdup(msr->mp, match, length);
                    s->value_len = length;
                    if ((s->name == NULL)||(s->value == NULL)) return -1;

                    apr_table_setn(msr->tx_vars, s->name, (void *)s);

                    if (msr->txcfg->debuglog_level >= 9) {
                        msr_log(msr, 9, "Added regex subexpression to TX.%d: %s", i,
                            log_escape_nq_ex(msr->mp, s->value, s->value_len));
                    }
                }
            }

            /* Unset the remaining TX vars (from previous invocations). */
            for(; i <= 9; i++) {
                char buf[24];
                apr_snprintf(buf, sizeof(buf), "%i", i);
                apr_table_unset(msr->tx_vars, buf);
            }

            break;
        }
    }

    if (is_ssn) {
        /* Match. */

        /* This message will be logged. */
        *error_msg = apr_psprintf(msr->mp, "SSN# match \"%s\" at %s. [offset \"%d\"]",
            regex->pattern, var->name, offset);

        return 1;
    }

    /* No match. */
    return 0;
}

/**
 * Perform geograpical lookups on an IP/Host.
 */
static int msre_op_geoLookup_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    geo_rec rec;
    geo_db *geo = msr->txcfg->geo;
    const char *geo_host = var->value;
    msc_string *s = NULL;
    int rc;

    *error_msg = NULL;

    if (geo == NULL) {
        msr_log(msr, 1, "Geo lookup for \"%s\" attempted without a database.  Set SecGeoLookupDB.", log_escape(msr->mp, geo_host));
        return 0;
    }


    rc = geo_lookup(msr, &rec, geo_host, error_msg);
    if (rc <= 0) {
        if (! *error_msg) {
            *error_msg = apr_psprintf(msr->mp, "Geo lookup for \"%s\" failed at %s.", log_escape_nq(msr->mp, geo_host), var->name);
        }
        apr_table_clear(msr->geo_vars);
        return rc;
    }
    if (! *error_msg) {
        *error_msg = apr_psprintf(msr->mp, "Geo lookup for \"%s\" succeeded at %s.",
            log_escape_nq(msr->mp, geo_host), var->name);
    }

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "GEO: %s={country_code=%s, country_code3=%s, country_name=%s, country_continent=%s, region=%s, city=%s, postal_code=%s, latitude=%f, longitude=%f, dma_code=%d, area_code=%d}",
                geo_host,
                rec.country_code,
                rec.country_code3,
                rec.country_name,
                rec.country_continent,
                rec.region,
                rec.city,
                rec.postal_code,
                rec.latitude,
                rec.longitude,
                rec.dma_code,
                rec.area_code);
    }

    s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    s->name = apr_pstrdup(msr->mp, "COUNTRY_CODE");
    s->name_len = strlen(s->name);
    s->value = apr_pstrdup(msr->mp, rec.country_code ? rec.country_code : "");
    s->value_len = strlen(s->value);
    apr_table_setn(msr->geo_vars, s->name, (void *)s);

    s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    s->name = apr_pstrdup(msr->mp, "COUNTRY_CODE3");
    s->name_len = strlen(s->name);
    s->value = apr_pstrdup(msr->mp, rec.country_code3 ? rec.country_code3 : "");
    s->value_len = strlen(s->value);
    apr_table_setn(msr->geo_vars, s->name, (void *)s);

    s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    s->name = apr_pstrdup(msr->mp, "COUNTRY_NAME");
    s->name_len = strlen(s->name);
    s->value = apr_pstrdup(msr->mp, rec.country_name ? rec.country_name : "");
    s->value_len = strlen(s->value);
    apr_table_setn(msr->geo_vars, s->name, (void *)s);

    s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    s->name = apr_pstrdup(msr->mp, "COUNTRY_CONTINENT");
    s->name_len = strlen(s->name);
    s->value = apr_pstrdup(msr->mp, rec.country_continent ? rec.country_continent : "");
    s->value_len = strlen(s->value);
    apr_table_setn(msr->geo_vars, s->name, (void *)s);

    s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    s->name = apr_pstrdup(msr->mp, "REGION");
    s->name_len = strlen(s->name);
    s->value = apr_pstrdup(msr->mp, rec.region ? rec.region : "");
    s->value_len = strlen(s->value);
    apr_table_setn(msr->geo_vars, s->name, (void *)s);

    s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    s->name = apr_pstrdup(msr->mp, "CITY");
    s->name_len = strlen(s->name);
    s->value = apr_pstrdup(msr->mp, rec.city ? rec.city : "");
    s->value_len = strlen(s->value);
    apr_table_setn(msr->geo_vars, s->name, (void *)s);

    s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    s->name = apr_pstrdup(msr->mp, "POSTAL_CODE");
    s->name_len = strlen(s->name);
    s->value = apr_pstrdup(msr->mp, rec.postal_code ? rec.postal_code : "");
    s->value_len = strlen(s->value);
    apr_table_setn(msr->geo_vars, s->name, (void *)s);

    s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    s->name = apr_pstrdup(msr->mp, "LATITUDE");
    s->name_len = strlen(s->name);
    s->value = apr_psprintf(msr->mp, "%f", rec.latitude);
    s->value_len = strlen(s->value);
    apr_table_setn(msr->geo_vars, s->name, (void *)s);

    s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    s->name = apr_pstrdup(msr->mp, "LONGITUDE");
    s->name_len = strlen(s->name);
    s->value = apr_psprintf(msr->mp, "%f", rec.longitude);
    s->value_len = strlen(s->value);
    apr_table_setn(msr->geo_vars, s->name, (void *)s);

    s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    s->name = apr_pstrdup(msr->mp, "DMA_CODE");
    s->name_len = strlen(s->name);
    s->value = apr_psprintf(msr->mp, "%d", rec.dma_code);
    s->value_len = strlen(s->value);
    apr_table_setn(msr->geo_vars, s->name, (void *)s);

    s = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    s->name = apr_pstrdup(msr->mp, "AREA_CODE");
    s->name_len = strlen(s->name);
    s->value = apr_psprintf(msr->mp, "%d", rec.area_code);
    s->value_len = strlen(s->value);
    apr_table_setn(msr->geo_vars, s->name, (void *)s);

    return 1;
}

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
        name_to_check = apr_psprintf(msr->mp, "%d.%d.%d.%d.%s", h3, h2, h1, h0, rule->op_param);
    } else {
        /* Assume the input is a domain name. */
        name_to_check = apr_psprintf(msr->mp, "%s.%s", target, rule->op_param);
    }

    if (name_to_check == NULL) return -1;

    rc = apr_sockaddr_info_get(&sa, name_to_check,
        APR_UNSPEC/*msr->r->connection->remote_addr->family*/, 0, 0, msr->mp);
    if (rc == APR_SUCCESS) {
        *error_msg = apr_psprintf(msr->r->pool, "RBL lookup of %s succeeded at %s.",
            log_escape_nq(msr->mp, name_to_check), var->name);
        return 1; /* Match. */
    }

    if (msr->txcfg->debuglog_level >= 5) {
        msr_log(msr, 5, "RBL lookup of %s failed at %s.", log_escape_nq(msr->mp, name_to_check), var->name);
    }

    /* No match. */
    return 0;
}

/* inspectFile */

static int msre_op_inspectFile_init(msre_rule *rule, char **error_msg) {
    char *filename = (char *)rule->op_param;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if ((filename == NULL)||(is_empty_string(filename))) {
        *error_msg = apr_psprintf(rule->ruleset->mp, "Operator @inspectFile requires parameter.");
        return -1;
    }

    filename = resolve_relative_path(rule->ruleset->mp, rule->filename, filename);

    #if defined(WITH_LUA)
    /* ENH Write & use string_ends(s, e). */
    if (strlen(rule->op_param) > 4) {
        char *p = filename + strlen(filename) - 4;
        if ((p[0] == '.')&&(p[1] == 'l')&&(p[2] == 'u')&&(p[3] == 'a'))
        {
            msc_script *script = NULL;

            /* Compile script. */
            *error_msg = lua_compile(&script, filename, rule->ruleset->mp);
            if (*error_msg != NULL) return -1;

            rule->op_param_data = script;
        }
    }
    #endif

    if (rule->op_param_data == NULL) {
        /* ENH Verify the script exists and that we have
         * the rights to execute it.
         */
    }

    return 1;
}

static int msre_op_inspectFile_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (rule->op_param_data == NULL) {
        /* Execute externally, as native binary/shell script. */
        char *script_output = NULL;
        char const *argv[5];
        const char *approver_script = rule->op_param;
        const char *target_file = apr_pstrmemdup(msr->mp, var->value, var->value_len);

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Executing %s to inspect %s.", approver_script, target_file);
        }

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
    }
    #if defined(WITH_LUA)
    else {
        /* Execute internally, as Lua script. */
        char *target = apr_pstrmemdup(msr->mp, var->value, var->value_len);
        msc_script *script = (msc_script *)rule->op_param_data;
        int rc;

        rc = lua_execute(script, target, msr, rule, error_msg);
        if (rc < 0) {
            /* Error. */
            return -1;
        }

        return rc;
    }
    #endif

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
                *error_msg = apr_psprintf(rule->ruleset->mp, "Invalid range value: %d", x);
                return 0;
            }
            table[x>>3] = (table[x>>3] | (1 << (x & 0x7)));
        } else {
            /* Range. */
            int start = atoi(p);
            int end = atoi(s + 1);

            if ((start < 0)||(start > 255)) {
                *error_msg = apr_psprintf(rule->ruleset->mp, "Invalid range start value: %d",
                    start);
                return 0;
            }
            if ((end < 0)||(end > 255)) {
                *error_msg = apr_psprintf(rule->ruleset->mp, "Invalid range end value: %d", end);
                return 0;
            }
            if (start > end) {
                *error_msg = apr_psprintf(rule->ruleset->mp, "Invalid range: %d-%d", start, end);
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
            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Value %d in %s outside range: %s", x, var->name, rule->op_param);
            }
            count++;
        }
    }

    if (count == 0) return 0; /* Valid - no match. */

    *error_msg = apr_psprintf(msr->mp, "Found %d byte(s) in %s outside range: %s.",
        count, var->name, rule->op_param);

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
            /* Encoding is valid */
            *error_msg = apr_psprintf(msr->mp, "Valid URL Encoding at %s.", var->name);
            break;
        case -2 :
            *error_msg = apr_psprintf(msr->mp, "Invalid URL Encoding: Non-hexadecimal "
                "digits used at %s.", var->name);
            return 1; /* Invalid match. */
            break;
        case -3 :
            *error_msg = apr_psprintf(msr->mp, "Invalid URL Encoding: Not enough characters "
                "at the end of input at %s.", var->name);
            return 1; /* Invalid match. */
            break;
        case -1 :
        default :
            *error_msg = apr_psprintf(msr->mp, "Invalid URL Encoding: Internal Error (rc = %d) at %s", rc, var->name);
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
#define UNICODE_ERROR_RESTRICTED_CHARACTER  -4
#define UNICODE_ERROR_DECODING_ERROR        -5

/* NOTE: This is over-commented for ease of verification */
static int detect_utf8_character(const unsigned char *p_read, unsigned int length) {
    int unicode_len = 0;
    unsigned int d = 0;
    unsigned char c;

    if (p_read == NULL) return UNICODE_ERROR_DECODING_ERROR;
    c = *p_read;

    /* If first byte begins with binary 0 it is single byte encoding */
    if ((c & 0x80) == 0) {
        /* single byte unicode (7 bit ASCII equivilent) has no validation */
        return 1;
    }
    /* If first byte begins with binary 110 it is two byte encoding*/
    else if ((c & 0xE0) == 0xC0) {
        /* check we have at least two bytes */
        if (length < 2) unicode_len = UNICODE_ERROR_CHARACTERS_MISSING;
        /* check second byte starts with binary 10 */
        else if (((*(p_read + 1)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else {
            unicode_len = 2;
            /* compute character number */
            d = ((c & 0x1F) << 6) | (*(p_read + 1) & 0x3F);
        }
    }
    /* If first byte begins with binary 1110 it is three byte encoding */
    else if ((c & 0xF0) == 0xE0) {
        /* check we have at least three bytes */
        if (length < 3) unicode_len = UNICODE_ERROR_CHARACTERS_MISSING;
        /* check second byte starts with binary 10 */
        else if (((*(p_read + 1)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        /* check third byte starts with binary 10 */
        else if (((*(p_read + 2)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else {
            unicode_len = 3;
            /* compute character number */
            d = ((c & 0x0F) << 12) | ((*(p_read + 1) & 0x3F) << 6) | (*(p_read + 2) & 0x3F);
        }
    }
    /* If first byte begins with binary 11110 it is four byte encoding */
    else if ((c & 0xF8) == 0xF0) {
        /* restrict characters to UTF-8 range (U+0000 - U+10FFFF)*/
        if (c >= 0xF5) {
            return UNICODE_ERROR_RESTRICTED_CHARACTER;
        }
        /* check we have at least four bytes */
        if (length < 4) unicode_len = UNICODE_ERROR_CHARACTERS_MISSING;
        /* check second byte starts with binary 10 */
        else if (((*(p_read + 1)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        /* check third byte starts with binary 10 */
        else if (((*(p_read + 2)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        /* check forth byte starts with binary 10 */
        else if (((*(p_read + 3)) & 0xC0) != 0x80) unicode_len = UNICODE_ERROR_INVALID_ENCODING;
        else {
            unicode_len = 4;
            /* compute character number */
            d = ((c & 0x07) << 18) | ((*(p_read + 1) & 0x3F) << 12) | ((*(p_read + 2) & 0x3F) < 6) | (*(p_read + 3) & 0x3F);
        }
    }
    /* any other first byte is invalid (RFC 3629) */
    else {
        return UNICODE_ERROR_INVALID_ENCODING;
    }

    /* invalid UTF-8 character number range (RFC 3629) */
    if ((d >= 0xD800) && (d <= 0xDFFF)) {
        return UNICODE_ERROR_RESTRICTED_CHARACTER;
    }

    /* check for overlong */
    if ((unicode_len == 4) && (d < 0x010000)) {
        /* four byte could be represented with less bytes */
        return UNICODE_ERROR_OVERLONG_CHARACTER;
    }
    else if ((unicode_len == 3) && (d < 0x0800)) {
        /* three byte could be represented with less bytes */
        return UNICODE_ERROR_OVERLONG_CHARACTER;
    }
    else if ((unicode_len == 2) && (d < 0x80)) {
        /* two byte could be represented with less bytes */
        return UNICODE_ERROR_OVERLONG_CHARACTER;
    }

    return unicode_len;
}

static int msre_op_validateUtf8Encoding_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    unsigned int i, bytes_left;

    bytes_left = var->value_len;

    for(i = 0; i < var->value_len;) {
        int rc = detect_utf8_character((unsigned char *)&var->value[i], bytes_left);

        switch(rc) {
            case UNICODE_ERROR_CHARACTERS_MISSING :
                *error_msg = apr_psprintf(msr->mp, "Invalid UTF-8 encoding: "
                    "not enough bytes in character "
                    "at %s. [offset \"%d\"]", var->name, i);
                return 1;
                break;
            case UNICODE_ERROR_INVALID_ENCODING :
                *error_msg = apr_psprintf(msr->mp, "Invalid UTF-8 encoding: "
                    "invalid byte value in character "
                    "at %s. [offset \"%d\"]", var->name, i);
                return 1;
                break;
            case UNICODE_ERROR_OVERLONG_CHARACTER :
                *error_msg = apr_psprintf(msr->mp, "Invalid UTF-8 encoding: "
                    "overlong character detected "
                    "at %s. [offset \"%d\"]", var->name, i);
                return 1;
                break;
            case UNICODE_ERROR_RESTRICTED_CHARACTER :
                *error_msg = apr_psprintf(msr->mp, "Invalid UTF-8 encoding: "
                    "use of restricted character "
                    "at %s. [offset \"%d\"]", var->name, i);
                return 1;
                break;
            case UNICODE_ERROR_DECODING_ERROR :
                *error_msg = apr_psprintf(msr->mp, "Error validating UTF-8 decoding "
                    "at %s. [offset \"%d\"]", var->name, i);
                return 1;
                break;
        }

        if (rc <= 0) {
            *error_msg = apr_psprintf(msr->mp, "Internal error during UTF-8 validation "
                "at %s.", var->name);
            return 1;
        }

        i += rc;
        bytes_left -= rc;
    }

    return 0;
}

/* eq */

static int msre_op_eq_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    msc_string str;
    int left, right;
    char *target = NULL;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if ((var->value == NULL)||(rule->op_param == NULL)) {
        /* NULL values do not match anything. */
        return 0;
    }

    str.value = (char *)rule->op_param;
    str.value_len = strlen(str.value);

    expand_macros(msr, &str, rule, msr->mp);

    target = apr_pstrmemdup(msr->mp, var->value, var->value_len);
    if (target == NULL) return -1;
    left = atoi(target);
    right = atoi(str.value);

    if (left != right) {
        /* No match. */
        return 0;
    }
    else {
        *error_msg = apr_psprintf(msr->mp, "Operator EQ matched %d at %s.", right, var->name);
        /* Match. */
        return 1;
    }
}

/* gt */

static int msre_op_gt_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    msc_string str;
    int left, right;
    char *target = NULL;

    if ((var->value == NULL)||(rule->op_param == NULL)) {
        /* NULL values do not match anything. */
        return 0;
    }

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if ((var->value == NULL)||(rule->op_param == NULL)) {
        /* NULL values do not match anything. */
        return 0;
    }

    str.value = (char *)rule->op_param;
    str.value_len = strlen(str.value);

    expand_macros(msr, &str, rule, msr->mp);

    target = apr_pstrmemdup(msr->mp, var->value, var->value_len);
    if (target == NULL) return -1;
    left = atoi(target);
    right = atoi(str.value);

    if (left <= right) {
        /* No match. */
        return 0;
    }
    else {
        *error_msg = apr_psprintf(msr->mp, "Operator GT matched %d at %s.", right, var->name);
        /* Match. */
        return 1;
    }
}

/* lt */

static int msre_op_lt_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    msc_string str;
    int left, right;
    char *target = NULL;

    if ((var->value == NULL)||(rule->op_param == NULL)) {
        /* NULL values do not match anything. */
        return 0;
    }

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if ((var->value == NULL)||(rule->op_param == NULL)) {
        /* NULL values do not match anything. */
        return 0;
    }

    str.value = (char *)rule->op_param;
    str.value_len = strlen(str.value);

    expand_macros(msr, &str, rule, msr->mp);

    target = apr_pstrmemdup(msr->mp, var->value, var->value_len);
    if (target == NULL) return -1;
    left = atoi(target);
    right = atoi(str.value);

    if (left >= right) {
        /* No match. */
        return 0;
    }
    else {
        *error_msg = apr_psprintf(msr->mp, "Operator LT matched %d at %s.", right, var->name);
        /* Match. */
        return 1;
    }
}

/* ge */

static int msre_op_ge_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    msc_string str;
    int left, right;
    char *target = NULL;

    if ((var->value == NULL)||(rule->op_param == NULL)) {
        /* NULL values do not match anything. */
        return 0;
    }

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if ((var->value == NULL)||(rule->op_param == NULL)) {
        /* NULL values do not match anything. */
        return 0;
    }

    str.value = (char *)rule->op_param;
    str.value_len = strlen(str.value);

    expand_macros(msr, &str, rule, msr->mp);

    target = apr_pstrmemdup(msr->mp, var->value, var->value_len);
    if (target == NULL) return -1;
    left = atoi(target);
    right = atoi(str.value);

    if (left < right) {
        /* No match. */
        return 0;
    }
    else {
        *error_msg = apr_psprintf(msr->mp, "Operator GE matched %d at %s.", right, var->name);
        /* Match. */
        return 1;
    }
}

/* le */

static int msre_op_le_execute(modsec_rec *msr, msre_rule *rule, msre_var *var,
    char **error_msg)
{
    msc_string str;
    int left, right;
    char *target = NULL;

    if ((var->value == NULL)||(rule->op_param == NULL)) {
        /* NULL values do not match anything. */
        return 0;
    }

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if ((var->value == NULL)||(rule->op_param == NULL)) {
        /* NULL values do not match anything. */
        return 0;
    }

    str.value = (char *)rule->op_param;
    str.value_len = strlen(str.value);

    expand_macros(msr, &str, rule, msr->mp);

    target = apr_pstrmemdup(msr->mp, var->value, var->value_len);
    if (target == NULL) return -1;
    left = atoi(target);
    right = atoi(str.value);

    if (left > right) {
        /* No match. */
        return 0;
    }
    else {
        *error_msg = apr_psprintf(msr->mp, "Operator LE matched %d at %s.", right, var->name);
        /* Match. */
        return 1;
    }
}

/* -------------------------------------------------------------------------- */

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

    /* noMatch */
    msre_engine_op_register(engine,
        "noMatch",
        NULL,
        msre_op_nomatch_execute
    );

#if !defined(WIN32) || !defined(WINNT)
    /* ipmatch */
    msre_engine_op_register(engine,
        "ipmatch",
        msre_op_ipmatch_param_init,
        msre_op_ipmatch_execute
    );
#endif /* WIN32 | WINNT */

    /* rsub */
    msre_engine_op_register(engine,
        "rsub",
        msre_op_rsub_param_init,
        msre_op_rsub_execute
    );

    /* rx */
    msre_engine_op_register(engine,
        "rx",
        msre_op_rx_param_init,
        msre_op_rx_execute
    );

    /* pm */
    msre_engine_op_register(engine,
        "pm",
        msre_op_pm_param_init,
        msre_op_pm_execute
    );

    /* pmFromFile */
    msre_engine_op_register(engine,
        "pmFromFile",
        msre_op_pmFromFile_param_init,
        msre_op_pm_execute
    );

    /* pmf */
    msre_engine_op_register(engine,
        "pmf",
        msre_op_pmFromFile_param_init,
        msre_op_pm_execute
    );

    /* within */
    msre_engine_op_register(engine,
        "within",
        NULL, /* ENH init function to flag var substitution */
        msre_op_within_execute
    );

    /* contains */
    msre_engine_op_register(engine,
        "contains",
        NULL, /* ENH init function to flag var substitution */
        msre_op_contains_execute
    );

    /* containsWord */
    msre_engine_op_register(engine,
        "containsWord",
        NULL, /* ENH init function to flag var substitution */
        msre_op_containsWord_execute
    );

    /* is */
    msre_engine_op_register(engine,
        "streq",
        NULL, /* ENH init function to flag var substitution */
        msre_op_streq_execute
    );

    /* beginsWith */
    msre_engine_op_register(engine,
        "beginsWith",
        NULL, /* ENH init function to flag var substitution */
        msre_op_beginsWith_execute
    );

    /* endsWith */
    msre_engine_op_register(engine,
        "endsWith",
        NULL, /* ENH init function to flag var substitution */
        msre_op_endsWith_execute
    );

    /* m */
    msre_engine_op_register(engine,
        "m",
        msre_op_m_param_init,
        msre_op_m_execute
    );

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

    /* verifyCC */
    msre_engine_op_register(engine,
        "verifyCC",
        msre_op_verifyCC_init,
        msre_op_verifyCC_execute
    );

    /* verifyCPF */
    msre_engine_op_register(engine,
        "verifyCPF",
        msre_op_verifyCPF_init,
        msre_op_verifyCPF_execute
    );

    /* verifySSN */
    msre_engine_op_register(engine,
        "verifySSN",
        msre_op_verifySSN_init,
        msre_op_verifySSN_execute
    );

    /* geoLookup */
    msre_engine_op_register(engine,
        "geoLookup",
        NULL,
        msre_op_geoLookup_execute
    );

    /* gsbLookup */
    msre_engine_op_register(engine,
        "gsbLookup",
        msre_op_gsbLookup_param_init,
        msre_op_gsbLookup_execute
    );

    /* rbl */
    msre_engine_op_register(engine,
        "rbl",
        NULL, /* ENH init function to validate DNS server */
        msre_op_rbl_execute
    );

    /* inspectFile */
    msre_engine_op_register(engine,
        "inspectFile",
        msre_op_inspectFile_init,
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
