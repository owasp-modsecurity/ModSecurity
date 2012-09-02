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
#include <stdio.h>

#include "http_core.h"
#include "http_request.h"

#include "modsecurity.h"
#include "apache2.h"
#include "http_main.h"
#include "http_connection.h"

#include "apr_optional.h"
#include "mod_log_config.h"

#include "msc_logging.h"
#include "msc_util.h"

#include "ap_mpm.h"
#include "scoreboard.h"

#include "apr_version.h"

#include "apr_lib.h"
#include "ap_config.h"
#include "http_config.h"


AP_DECLARE(int) ap_cfg_closefile(ap_configfile_t *cfp)
{
#ifdef DEBUG
    ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, NULL,
        "Done with config file %s", cfp->name);
#endif
    return (cfp->close == NULL) ? 0 : cfp->close(cfp->param);
}

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
static apr_status_t cfg_close(void *param)
{
    apr_file_t *cfp = (apr_file_t *) param;
    return (apr_file_close(cfp));
}

static int cfg_getch(void *param)
{
    char ch;
    apr_file_t *cfp = (apr_file_t *) param;
    if (apr_file_getc(&ch, cfp) == APR_SUCCESS)
        return ch;
    return (int)EOF;
}

static void *cfg_getstr(void *buf, size_t bufsiz, void *param)
{
    apr_file_t *cfp = (apr_file_t *) param;
    apr_status_t rv;
    rv = apr_file_gets(buf, bufsiz, cfp);
    if (rv == APR_SUCCESS) {
        return buf;
    }
    return NULL;
}
#else
/* we can't use apr_file_* directly because of linking issues on Windows */
static apr_status_t cfg_close(void *param)
{
    return apr_file_close(param);
}

static apr_status_t cfg_getch(char *ch, void *param)
{
    return apr_file_getc(ch, param);
}

static apr_status_t cfg_getstr(void *buf, apr_size_t bufsiz, void *param)
{
    return apr_file_gets(buf, bufsiz, param);
}
#endif

/* Read one line from open ap_configfile_t, strip LF, increase line number */
/* If custom handler does not define a getstr() function, read char by char */

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
AP_DECLARE(int) ap_cfg_getline(char *buf, size_t bufsize, ap_configfile_t *cfp)
{
    /* If a "get string" function is defined, use it */
    if (cfp->getstr != NULL) {
        char *src, *dst;
        char *cp;
        char *cbuf = buf;
        size_t cbufsize = bufsize;

        while (1) {
            ++cfp->line_number;
            if (cfp->getstr(cbuf, cbufsize, cfp->param) == NULL)
                return 1;

            /*
             *  check for line continuation,
             *  i.e. match [^\\]\\[\r]\n only
             */
            cp = cbuf;
            while (cp < cbuf+cbufsize && *cp != '\0')
                cp++;
            if (cp > cbuf && cp[-1] == LF) {
                cp--;
                if (cp > cbuf && cp[-1] == CR)
                    cp--;
                if (cp > cbuf && cp[-1] == '\\') {
                    cp--;
                    if (!(cp > cbuf && cp[-1] == '\\')) {
                        /*
                         * line continuation requested -
                         * then remove backslash and continue
                         */
                        cbufsize -= (cp-cbuf);
                        cbuf = cp;
                        continue;
                    }
                    else {
                        /*
                         * no real continuation because escaped -
                         * then just remove escape character
                         */
                        for ( ; cp < cbuf+cbufsize && *cp != '\0'; cp++)
                            cp[0] = cp[1];
                    }
                }
            }
            break;
        }

        /*
         * Leading and trailing white space is eliminated completely
         */
        src = buf;
        while (apr_isspace(*src))
            ++src;
        /* blast trailing whitespace */
        dst = &src[strlen(src)];
        while (--dst >= src && apr_isspace(*dst))
            *dst = '\0';
        /* Zap leading whitespace by shifting */
        if (src != buf)
            for (dst = buf; (*dst++ = *src++) != '\0'; )
                ;

#ifdef DEBUG_CFG_LINES
        ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, NULL, "Read config: %s", buf);
#endif
        return 0;
    } else {
        /* No "get string" function defined; read character by character */
        register int c;
        register size_t i = 0;

        buf[0] = '\0';
        /* skip leading whitespace */
        do {
            c = cfp->getch(cfp->param);
        } while (c == '\t' || c == ' ');

        if (c == EOF)
            return 1;

        if(bufsize < 2) {
            /* too small, assume caller is crazy */
            return 1;
        }

        while (1) {
            if ((c == '\t') || (c == ' ')) {
                buf[i++] = ' ';
                while ((c == '\t') || (c == ' '))
                    c = cfp->getch(cfp->param);
            }
            if (c == CR) {
                /* silently ignore CR (_assume_ that a LF follows) */
                c = cfp->getch(cfp->param);
            }
            if (c == LF) {
                /* increase line number and return on LF */
                ++cfp->line_number;
            }
            if (c == EOF || c == 0x4 || c == LF || i >= (bufsize - 2)) {
                /*
                 *  check for line continuation
                 */
                if (i > 0 && buf[i-1] == '\\') {
                    i--;
                    if (!(i > 0 && buf[i-1] == '\\')) {
                        /* line is continued */
                        c = cfp->getch(cfp->param);
                        continue;
                    }
                    /* else nothing needs be done because
                     * then the backslash is escaped and
                     * we just strip to a single one
                     */
                }
                /* blast trailing whitespace */
                while (i > 0 && apr_isspace(buf[i - 1]))
                    --i;
                buf[i] = '\0';
#ifdef DEBUG_CFG_LINES
                ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, NULL,
                             "Read config: %s", buf);
#endif
                return 0;
            }
            buf[i] = c;
            ++i;
            c = cfp->getch(cfp->param);
        }
    }
}
#else
static apr_status_t ap_cfg_getline_core(char *buf, apr_size_t bufsize,
                                        ap_configfile_t *cfp)
{
    apr_status_t rc;
    /* If a "get string" function is defined, use it */
    if (cfp->getstr != NULL) {
        char *cp;
        char *cbuf = buf;
        apr_size_t cbufsize = bufsize;

        while (1) {
            ++cfp->line_number;
            rc = cfp->getstr(cbuf, cbufsize, cfp->param);
            if (rc == APR_EOF) {
                if (cbuf != buf) {
                    *cbuf = '\0';
                    break;
                }
                else {
                    return APR_EOF;
                }
            }
            if (rc != APR_SUCCESS) {
                return rc;
            }

            /*
             *  check for line continuation,
             *  i.e. match [^\\]\\[\r]\n only
             */
            cp = cbuf;
            cp += strlen(cp);
            if (cp > cbuf && cp[-1] == LF) {
                cp--;
                if (cp > cbuf && cp[-1] == CR)
                    cp--;
                if (cp > cbuf && cp[-1] == '\\') {
                    cp--;
                    /*
                     * line continuation requested -
                     * then remove backslash and continue
                     */
                    cbufsize -= (cp-cbuf);
                    cbuf = cp;
                    continue;
                }
            }
            else if (cp - buf >= bufsize - 1) {
                return APR_ENOSPC;
            }
            break;
        }
    } else {
        /* No "get string" function defined; read character by character */
        apr_size_t i = 0;

        if (bufsize < 2) {
            /* too small, assume caller is crazy */
            return APR_EINVAL;
        }
        buf[0] = '\0';

        while (1) {
            char c;
            rc = cfp->getch(&c, cfp->param);
            if (rc == APR_EOF) {
                if (i > 0)
                    break;
                else
                    return APR_EOF;
            }
            if (rc != APR_SUCCESS)
                return rc;
            if (c == LF) {
                ++cfp->line_number;
                /* check for line continuation */
                if (i > 0 && buf[i-1] == '\\') {
                    i--;
                    continue;
                }
                else {
                    break;
                }
            }
            else if (i >= bufsize - 2) {
                return APR_ENOSPC;
            }
            buf[i] = c;
            ++i;
        }
        buf[i] = '\0';
    }
    return APR_SUCCESS;
}

static int cfg_trim_line(char *buf)
{
    char *start, *end;
    /*
     * Leading and trailing white space is eliminated completely
     */
    start = buf;
    while (apr_isspace(*start))
        ++start;
    /* blast trailing whitespace */
    end = &start[strlen(start)];
    while (--end >= start && apr_isspace(*end))
        *end = '\0';
    /* Zap leading whitespace by shifting */
    if (start != buf)
        memmove(buf, start, end - start + 2);
#ifdef DEBUG_CFG_LINES
    ap_log_error(APLOG_MARK, APLOG_NOTICE, 0, NULL, APLOGNO(00555) "Read config: '%s'", buf);
#endif
    return end - start + 1;
}

AP_DECLARE(apr_status_t) ap_cfg_getline(char *buf, apr_size_t bufsize,
		ap_configfile_t *cfp)
{
	apr_status_t rc = ap_cfg_getline_core(buf, bufsize, cfp);
	if (rc == APR_SUCCESS)
		cfg_trim_line(buf);
	return rc;
}
#endif

static char *substring_conf(apr_pool_t *p, const char *start, int len,
                            char quote)
{
    char *result = apr_palloc(p, len + 2);
    char *resp = result;
    int i;

    for (i = 0; i < len; ++i) {
        if (start[i] == '\\' && (start[i + 1] == '\\'
                                 || (quote && start[i + 1] == quote)))
            *resp++ = start[++i];
        else
            *resp++ = start[i];
    }

    *resp++ = '\0';
#if RESOLVE_ENV_PER_TOKEN
    return (char *)ap_resolve_env(p,result);
#else
    return result;
#endif
}

AP_DECLARE(char *) ap_getword_conf(apr_pool_t *p, const char **line)
{
    const char *str = *line, *strend;
    char *res;
    char quote;

    while (*str && apr_isspace(*str))
        ++str;

    if (!*str) {
        *line = str;
        return "";
    }

    if ((quote = *str) == '"' || quote == '\'') {
        strend = str + 1;
        while (*strend && *strend != quote) {
            if (*strend == '\\' && strend[1] &&
                (strend[1] == quote || strend[1] == '\\')) {
                strend += 2;
            }
            else {
                ++strend;
            }
        }
        res = substring_conf(p, str + 1, strend - str - 1, quote);

        if (*strend == quote)
            ++strend;
    }
    else {
        strend = str;
        while (*strend && !apr_isspace(*strend))
            ++strend;

        res = substring_conf(p, str, strend - str, 0);
    }

    while (*strend && apr_isspace(*strend))
        ++strend;
    *line = strend;
    return res;
}

/* Open a ap_configfile_t as FILE, return open ap_configfile_t struct pointer */
AP_DECLARE(apr_status_t) ap_pcfg_openfile(ap_configfile_t **ret_cfg,
                                          apr_pool_t *p, const char *name)
{
    ap_configfile_t *new_cfg;
    apr_file_t *file = NULL;
    apr_finfo_t finfo;
    apr_status_t status;
#ifdef DEBUG
    char buf[120];
#endif

    if (name == NULL) {
        ap_log_error(APLOG_MARK, APLOG_ERR, 0, NULL,
               "Internal error: pcfg_openfile() called with NULL filename");
        return APR_EBADF;
    }

    status = apr_file_open(&file, name, APR_READ | APR_BUFFERED,
                           APR_OS_DEFAULT, p);
#ifdef DEBUG
    ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, NULL,
                "Opening config file %s (%s)",
                name, (status != APR_SUCCESS) ?
                apr_strerror(status, buf, sizeof(buf)) : "successful");
#endif
    if (status != APR_SUCCESS)
        return status;

    status = apr_file_info_get(&finfo, APR_FINFO_TYPE, file);
    if (status != APR_SUCCESS)
        return status;

    if (finfo.filetype != APR_REG &&
#if defined(WIN32) || defined(OS2) || defined(NETWARE)
        strcasecmp(apr_filepath_name_get(name), "nul") != 0) {
#else
        strcmp(name, "/dev/null") != 0) {
#endif /* WIN32 || OS2 */
        ap_log_error(APLOG_MARK, APLOG_ERR, 0, NULL,
                     "Access to file %s denied by server: not a regular file",
                     name);
        apr_file_close(file);
        return APR_EBADF;
    }

#ifdef WIN32
    /* Some twisted character [no pun intended] at MS decided that a
     * zero width joiner as the lead wide character would be ideal for
     * describing Unicode text files.  This was further convoluted to
     * another MSism that the same character mapped into utf-8, EF BB BF
     * would signify utf-8 text files.
     *
     * Since MS configuration files are all protecting utf-8 encoded
     * Unicode path, file and resource names, we already have the correct
     * WinNT encoding.  But at least eat the stupid three bytes up front.
     */
    {
        unsigned char buf[4];
        apr_size_t len = 3;
        status = apr_file_read(file, buf, &len);
        if ((status != APR_SUCCESS) || (len < 3)
              || memcmp(buf, "\xEF\xBB\xBF", 3) != 0) {
            apr_off_t zero = 0;
            apr_file_seek(file, APR_SET, &zero);
        }
    }
#endif

    new_cfg = apr_palloc(p, sizeof(*new_cfg));
    new_cfg->param = file;
    new_cfg->name = apr_pstrdup(p, name);
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
    new_cfg->getch = (int (*)(void *)) cfg_getch;
    new_cfg->getstr = (void *(*)(void *, size_t, void *)) cfg_getstr;
    new_cfg->close = (int (*)(void *)) cfg_close;
#else
    new_cfg->getch = cfg_getch;
    new_cfg->getstr = cfg_getstr;
    new_cfg->close = cfg_close; 
#endif
    new_cfg->line_number = 0;
    *ret_cfg = new_cfg;
    return APR_SUCCESS;
}




AP_CORE_DECLARE(const command_rec *) ap_find_command(const char *name,
                                                     const command_rec *cmds)
{
    while (cmds->name) {
        if (!strcasecmp(name, cmds->name))
            return cmds;

        ++cmds;
    }

    return NULL;
}

#define AP_MAX_ARGC 64

static const char *invoke_cmd(const command_rec *cmd, cmd_parms *parms,
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
            return apr_pstrcat(parms->pool, cmd->name, " takes three arguments",
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

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
static cmd_parms default_parms =
{NULL, 0, -1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
#endif

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 3
static cmd_parms default_parms =
{NULL, 0, 0, NULL, -1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
#endif

const char *process_command_config(server_rec *s,
                                          void *mconfig,
                                          apr_pool_t *p,
                                          apr_pool_t *ptemp,
										  const char *filename)
{
    const char *errmsg;
    cmd_parms parms;
    char *l = apr_palloc (ptemp, MAX_STRING_LEN);
    const char *args = l;
    char *cmd_name;
	const command_rec *cmd;
    apr_array_header_t *arr = apr_array_make(p, 1, sizeof(char *));
	apr_status_t status;
	ap_directive_t *newdir;

    parms = default_parms;
    parms.pool = p;
    parms.temp_pool = ptemp;
    parms.server = s;
    parms.override = (RSRC_CONF | OR_ALL) & ~(OR_AUTHCFG | OR_LIMIT);
    parms.override_opts = OPT_ALL | OPT_SYM_OWNER | OPT_MULTI;

    status = ap_pcfg_openfile(&parms.config_file, p, filename);

	if(status != APR_SUCCESS)
	{
		// cannot open config file
		//
	}

	while (!(ap_cfg_getline(l, MAX_STRING_LEN, parms.config_file))) {
		if (*l == '#' || *l == '\0')
			continue;

		args = l;

		cmd_name = ap_getword_conf(p, &args);
		if (*cmd_name == '\0')
			continue;

		cmd = ap_find_command(cmd_name, security2_module.cmds);

		if(cmd == NULL)
		{
			// unknown command, should error
			//
			printf("Unknown command: %s\n", cmd_name);
			continue;
		}

		newdir = apr_pcalloc(p, sizeof(ap_directive_t));
		newdir->filename = parms.config_file->name;
		newdir->line_num = parms.config_file->line_number;
		newdir->directive = cmd_name;
		newdir->args = apr_pstrdup(p, args);

		parms.directive = newdir;

		errmsg = invoke_cmd(cmd, &parms, mconfig, args);

		if(errmsg != NULL)
			break;
	}

    ap_cfg_closefile(parms.config_file);

    if (errmsg) {
		char *err = (char *)apr_palloc(p, 1024);

		apr_snprintf(err, 1024, "Syntax error in config file %s, line %d: %s", parms.config_file->name,
						parms.config_file->line_number, errmsg);

		return err;
    }

    return NULL;
}
