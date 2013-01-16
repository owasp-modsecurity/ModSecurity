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

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_SEM_H
#include <sys/sem.h>
#endif

//=========

/** The default path for CGI scripts if none is currently set */
#ifndef DEFAULT_PATH
#define DEFAULT_PATH "/bin:/usr/bin:/usr/ucb:/usr/bsd:/usr/local/bin"
#endif

//=========

AP_DECLARE_DATA scoreboard *ap_scoreboard_image = NULL;

AP_DECLARE_DATA apr_array_header_t *ap_server_config_defines = NULL;

AP_DECLARE_DATA const char *ap_server_root = "\\";

static int server_limit, thread_limit, lb_limit;
static apr_size_t scoreboard_size;

void *modsecLogObj = NULL;
void (*modsecLogHook)(void *obj, int level, char *str) = NULL;

//=========

static const char * const status_lines[RESPONSE_CODES] =
{
    "100 Continue",
    "101 Switching Protocols",
    "102 Processing",
#define LEVEL_200  3
    "200 OK",
    "201 Created",
    "202 Accepted",
    "203 Non-Authoritative Information",
    "204 No Content",
    "205 Reset Content",
    "206 Partial Content",
    "207 Multi-Status",
#define LEVEL_300 11
    "300 Multiple Choices",
    "301 Moved Permanently",
    "302 Found",
    "303 See Other",
    "304 Not Modified",
    "305 Use Proxy",
    "306 unused",
    "307 Temporary Redirect",
#define LEVEL_400 19
    "400 Bad Request",
    "401 Authorization Required",
    "402 Payment Required",
    "403 Forbidden",
    "404 Not Found",
    "405 Method Not Allowed",
    "406 Not Acceptable",
    "407 Proxy Authentication Required",
    "408 Request Time-out",
    "409 Conflict",
    "410 Gone",
    "411 Length Required",
    "412 Precondition Failed",
    "413 Request Entity Too Large",
    "414 Request-URI Too Large",
    "415 Unsupported Media Type",
    "416 Requested Range Not Satisfiable",
    "417 Expectation Failed",
    "418 unused",
    "419 unused",
    "420 unused",
    "421 unused",
    "422 Unprocessable Entity",
    "423 Locked",
    "424 Failed Dependency",
    /* This is a hack, but it is required for ap_index_of_response
     * to work with 426.
     */
    "425 No code",
    "426 Upgrade Required",
#define LEVEL_500 46
    "500 Internal Server Error",
    "501 Method Not Implemented",
    "502 Bad Gateway",
    "503 Service Temporarily Unavailable",
    "504 Gateway Time-out",
    "505 HTTP Version Not Supported",
    "506 Variant Also Negotiates",
    "507 Insufficient Storage",
    "508 unused",
    "509 unused",
    "510 Not Extended"
};

AP_DECLARE(int) ap_index_of_response(int status)
{
    static int shortcut[6] = {0, LEVEL_200, LEVEL_300, LEVEL_400,
    LEVEL_500, RESPONSE_CODES};
    int i, pos;

    if (status < 100) {               /* Below 100 is illegal for HTTP status */
        return LEVEL_500;
    }

    for (i = 0; i < 5; i++) {
        status -= 100;
        if (status < 100) {
            pos = (status + shortcut[i]);
            if (pos < shortcut[i + 1]) {
                return pos;
            }
            else {
                return LEVEL_500;            /* status unknown (falls in gap) */
            }
        }
    }
    return LEVEL_500;                         /* 600 or above is also illegal */
}

//=========

static char *http2env(apr_pool_t *a, const char *w)
{
    char *res = (char *)apr_palloc(a, sizeof("HTTP_") + strlen(w));
    char *cp = res;
    char c;

    *cp++ = 'H';
    *cp++ = 'T';
    *cp++ = 'T';
    *cp++ = 'P';
    *cp++ = '_';

    while ((c = *w++) != 0) {
        if (!apr_isalnum(c)) {
            *cp++ = '_';
        }
        else {
            *cp++ = apr_toupper(c);
        }
    }
    *cp = 0;

    return res;
}

AP_DECLARE(char *) ap_escape_html2(apr_pool_t *p, const char *s, int toasc)
{
    int i, j;
    char *x;

    /* first, count the number of extra characters */
    for (i = 0, j = 0; s[i] != '\0'; i++)
        if (s[i] == '<' || s[i] == '>')
            j += 3;
        else if (s[i] == '&')
            j += 4;
        else if (s[i] == '"')
            j += 5;
        else if (toasc && !apr_isascii(s[i]))
            j += 5;

    if (j == 0)
        return apr_pstrmemdup(p, s, i);

    x = apr_palloc(p, i + j + 1);
    for (i = 0, j = 0; s[i] != '\0'; i++, j++)
        if (s[i] == '<') {
            memcpy(&x[j], "&lt;", 4);
            j += 3;
        }
        else if (s[i] == '>') {
            memcpy(&x[j], "&gt;", 4);
            j += 3;
        }
        else if (s[i] == '&') {
            memcpy(&x[j], "&amp;", 5);
            j += 4;
        }
        else if (s[i] == '"') {
            memcpy(&x[j], "&quot;", 6);
            j += 5;
        }
        else if (toasc && !apr_isascii(s[i])) {
            char *esc = apr_psprintf(p, "&#%3.3d;", (unsigned char)s[i]);
            memcpy(&x[j], esc, 6);
            j += 5;
        }
        else
            x[j] = s[i];

    x[j] = '\0';
    return x;
}

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
AP_DECLARE(char *) ap_escape_html(apr_pool_t *p, const char *s)
{
    return ap_escape_html2(p, s, 0);
}
#endif

AP_DECLARE(const char *) ap_psignature(const char *prefix, request_rec *r)
{
	return prefix;
}

AP_DECLARE(const char *) ap_document_root(request_rec *r) /* Don't use this! */
{
	return "\\";
}

AP_DECLARE(apr_port_t) ap_get_server_port(const request_rec *r)
{
	return 80;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
AP_DECLARE(void) ap_log_error(const char *file, int line, int level, 
                             apr_status_t status, const server_rec *s, 
                             const char *fmt, ...)
//			    __attribute__((format(printf,6,7)))
#else
AP_DECLARE(void) ap_log_error_(const char *file, int line, int module_index,
                               int level, apr_status_t status,
                               const server_rec *s, const char *fmt, ...)
//                              __attribute__((format(printf,7,8)))
#endif
{
    va_list args;
    char errstr[MAX_STRING_LEN];

    va_start(args, fmt);

    apr_vsnprintf(errstr, MAX_STRING_LEN, fmt, args);

	va_end(args);

	if(modsecLogHook != NULL)
		modsecLogHook(modsecLogObj, level, errstr);
}

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
AP_DECLARE(void) ap_log_perror(const char *file, int line, int level, 
                             apr_status_t status, apr_pool_t *p, 
                             const char *fmt, ...)
//			    __attribute__((format(printf,6,7)))
#else
AP_DECLARE(void) ap_log_perror_(const char *file, int line, int module_index,
                                int level, apr_status_t status, apr_pool_t *p,
                                const char *fmt, ...)
#endif
{
    va_list args;
    char errstr[MAX_STRING_LEN];

    va_start(args, fmt);

    apr_vsnprintf(errstr, MAX_STRING_LEN, fmt, args);

	va_end(args);

	if(modsecLogHook != NULL)
		modsecLogHook(modsecLogObj, level, errstr);
}

AP_DECLARE(module *) ap_find_linked_module(const char *name)
{
	// only used for ACTION_PROXY to find mod_proxy.c; see mod_security2.c, perform_interception()
	//
	return NULL;
}

AP_DECLARE(const char *) ap_get_server_name(request_rec *r)
{
	return r->server->server_hostname;
}

AP_DECLARE(void) ap_add_version_component(apr_pool_t *pconf, const char *component)
{
	// appends string to server description string
	//
}

AP_DECLARE(const char *) ap_get_status_line(int status)
{
    return status_lines[ap_index_of_response(status)];
}

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
AP_DECLARE(worker_score *) ap_get_scoreboard_worker(int x, int y)
{
    if (((x < 0) || (server_limit < x)) ||
        ((y < 0) || (thread_limit < y))) {
        return(NULL); /* Out of range */
    }
    return &ap_scoreboard_image->servers[x][y];
}
#else
AP_DECLARE(worker_score *) ap_get_scoreboard_worker_from_indexes(int x, int y)
{
    if (((x < 0) || (x >= server_limit)) ||
        ((y < 0) || (y >= thread_limit))) {
        return(NULL); /* Out of range */
    }
    return &ap_scoreboard_image->servers[x][y];
}

AP_DECLARE(worker_score *) ap_get_scoreboard_worker(ap_sb_handle_t *sbh)
{
    //if (!sbh)
    //    return NULL;

    //return ap_get_scoreboard_worker_from_indexes(sbh->child_num,
    //                                             sbh->thread_num);
    return ap_get_scoreboard_worker_from_indexes(0, 0);
}
#endif

AP_DECLARE(apr_status_t) ap_mpm_query(int query_code, int *result)
{
    switch(query_code){
        //case AP_MPMQ_MAX_DAEMON_USED:
        //    *result = MAXIMUM_WAIT_OBJECTS;
        //    return APR_SUCCESS;
        case AP_MPMQ_IS_THREADED:
            *result = AP_MPMQ_STATIC;
            return APR_SUCCESS;
        case AP_MPMQ_IS_FORKED:
            *result = AP_MPMQ_NOT_SUPPORTED;
            return APR_SUCCESS;
        case AP_MPMQ_HARD_LIMIT_DAEMONS:
            *result = 1; //HARD_SERVER_LIMIT;
            return APR_SUCCESS;
        case AP_MPMQ_HARD_LIMIT_THREADS:
            *result = thread_limit;
            return APR_SUCCESS;
        //case AP_MPMQ_MAX_THREADS:
        //    *result = ap_threads_per_child;
        //    return APR_SUCCESS;
        case AP_MPMQ_MIN_SPARE_DAEMONS:
            *result = 0;
            return APR_SUCCESS;
        case AP_MPMQ_MIN_SPARE_THREADS:
            *result = 0;
            return APR_SUCCESS;
        case AP_MPMQ_MAX_SPARE_DAEMONS:
            *result = 0;
            return APR_SUCCESS;
        case AP_MPMQ_MAX_SPARE_THREADS:
            *result = 0;
            return APR_SUCCESS;
        //case AP_MPMQ_MAX_REQUESTS_DAEMON:
        //    *result = ap_max_requests_per_child;
        //    return APR_SUCCESS;
        case AP_MPMQ_MAX_DAEMONS:
            *result = 0;
            return APR_SUCCESS;
        //case AP_MPMQ_MPM_STATE:
        //    *result = winnt_mpm_state;
        //    return APR_SUCCESS;
    }
    return APR_ENOTIMPL;
}

AP_DECLARE(const char *) ap_get_server_banner(void)
{
	return "ModSecurity Standalone";
}

/* Code from Harald Hanche-Olsen <hanche@imf.unit.no> */
static APR_INLINE void do_double_reverse (conn_rec *conn)
{
    apr_sockaddr_t *sa;
    apr_status_t rv;

    if (conn->double_reverse) {
        /* already done */
        return;
    }

    if (conn->remote_host == NULL || conn->remote_host[0] == '\0') {
        /* single reverse failed, so don't bother */
        conn->double_reverse = -1;
        return;
    }

    rv = apr_sockaddr_info_get(&sa, conn->remote_host, APR_UNSPEC, 0, 0, conn->pool);
    if (rv == APR_SUCCESS) {
        while (sa) {
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
            if (apr_sockaddr_equal(sa, conn->remote_addr)) {
#else
	    if (apr_sockaddr_equal(sa, conn->client_addr)) {
#endif
                conn->double_reverse = 1;
                return;
            }

            sa = sa->next;
        }
    }

    conn->double_reverse = -1;
}

#ifndef HOSTNAME_LOOKUP_OFF
#define HOSTNAME_LOOKUP_OFF	0
#define HOSTNAME_LOOKUP_ON	1
#define HOSTNAME_LOOKUP_DOUBLE	2
#define HOSTNAME_LOOKUP_UNSET	3
#endif

AP_DECLARE(void) ap_str_tolower(char *str)
{
    while (*str) {
        *str = apr_tolower(*str);
        ++str;
    }
}

AP_DECLARE(const char *) ap_get_remote_host(conn_rec *conn, void *dir_config, int type, int *str_is_ip)
{
    int hostname_lookups = HOSTNAME_LOOKUP_OFF;
    int ignored_str_is_ip;

    if (!str_is_ip) { /* caller doesn't want to know */
        str_is_ip = &ignored_str_is_ip;
    }
    *str_is_ip = 0;

    /* If we haven't checked the host name, and we want to */
    //if (dir_config) {
    //    hostname_lookups =
    //        ((core_dir_config *)ap_get_module_config(dir_config, &core_module))
    //        ->hostname_lookups;

    //    if (hostname_lookups == HOSTNAME_LOOKUP_UNSET) {
    //        hostname_lookups = HOSTNAME_LOOKUP_OFF;
    //    }
    //}

    if (type != REMOTE_NOLOOKUP
        && conn->remote_host == NULL
        && (type == REMOTE_DOUBLE_REV
        || hostname_lookups != HOSTNAME_LOOKUP_OFF)) {
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
        if (apr_getnameinfo(&conn->remote_host, conn->remote_addr, 0)
            == APR_SUCCESS) {
#else
        if (apr_getnameinfo(&conn->remote_host, conn->client_addr, 0)
            == APR_SUCCESS) {
#endif
            ap_str_tolower(conn->remote_host);

            if (hostname_lookups == HOSTNAME_LOOKUP_DOUBLE) {
                do_double_reverse(conn);
                if (conn->double_reverse != 1) {
                    conn->remote_host = NULL;
                }
            }
        }

        /* if failed, set it to the NULL string to indicate error */
        if (conn->remote_host == NULL) {
            conn->remote_host = "";
        }
    }

    if (type == REMOTE_DOUBLE_REV) {
        do_double_reverse(conn);
        if (conn->double_reverse == -1) {
            return NULL;
        }
    }

    /*
     * Return the desired information; either the remote DNS name, if found,
     * or either NULL (if the hostname was requested) or the IP address
     * (if any identifier was requested).
     */
    if (conn->remote_host != NULL && conn->remote_host[0] != '\0') {
        return conn->remote_host;
    }
    else {
        if (type == REMOTE_HOST || type == REMOTE_DOUBLE_REV) {
            return NULL;
        }
        else {
            *str_is_ip = 1;
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
            return conn->remote_ip;
#else
	    return conn->client_ip;
#endif
        }
    }
}

AP_DECLARE(char *) ap_server_root_relative(apr_pool_t *p, const char *file)
{
    char *newpath = NULL;
    apr_status_t rv;
    rv = apr_filepath_merge(&newpath, ap_server_root, file,
                            APR_FILEPATH_TRUENAME, p);
    if (newpath && (rv == APR_SUCCESS || APR_STATUS_IS_EPATHWILD(rv)
                                      || APR_STATUS_IS_ENOENT(rv)
                                      || APR_STATUS_IS_ENOTDIR(rv))) {
        return newpath;
    }
    else {
        return NULL;
    }
}

AP_DECLARE(piped_log *) ap_open_piped_log(apr_pool_t *p, const char *program)
{
	return NULL;
}

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 3
AP_DECLARE(apr_file_t *) ap_piped_log_write_fd(piped_log *pl)
{
 return NULL;
}
#endif

AP_DECLARE(char **) ap_create_environment(apr_pool_t *p, apr_table_t *t)
{
    const apr_array_header_t *env_arr = apr_table_elts(t);
    const apr_table_entry_t *elts = (const apr_table_entry_t *) env_arr->elts;
    char **env = (char **) apr_palloc(p, (env_arr->nelts + 2) * sizeof(char *));
    int i, j;
    char *tz;
    char *whack;

    j = 0;
    if (!apr_table_get(t, "TZ")) {
        tz = getenv("TZ");
        if (tz != NULL) {
            env[j++] = apr_pstrcat(p, "TZ=", tz, NULL);
        }
    }
    for (i = 0; i < env_arr->nelts; ++i) {
        if (!elts[i].key) {
            continue;
        }
        env[j] = apr_pstrcat(p, elts[i].key, "=", elts[i].val, NULL);
        whack = env[j];
        if (apr_isdigit(*whack)) {
            *whack++ = '_';
        }
        while (*whack != '=') {
            if (!apr_isalnum(*whack) && *whack != '_') {
                *whack = '_';
            }
            ++whack;
        }
        ++j;
    }

    env[j] = NULL;
    return env;
}

/* This "cute" little function comes about because the path info on
 * filenames and URLs aren't always the same. So we take the two,
 * and find as much of the two that match as possible.
 */

AP_DECLARE(int) ap_find_path_info(const char *uri, const char *path_info)
{
    int lu = strlen(uri);
    int lp = strlen(path_info);

    while (lu-- && lp-- && uri[lu] == path_info[lp]) {
        if (path_info[lp] == '/') {
            while (lu && uri[lu-1] == '/') lu--;
        }
    }

    if (lu == -1) {
        lu = 0;
    }

    while (uri[lu] != '\0' && uri[lu] != '/') {
        lu++;
    }
    return lu;
}

/* Obtain the Request-URI from the original request-line, returning
 * a new string from the request pool containing the URI or "".
 */
static char *original_uri(request_rec *r)
{
    char *first, *last;

    if (r->the_request == NULL) {
        return (char *) apr_pcalloc(r->pool, 1);
    }

    first = r->the_request;     /* use the request-line */

    while (*first && !apr_isspace(*first)) {
        ++first;                /* skip over the method */
    }
    while (apr_isspace(*first)) {
        ++first;                /*   and the space(s)   */
    }

    last = first;
    while (*last && !apr_isspace(*last)) {
        ++last;                 /* end at next whitespace */
    }

    return apr_pstrmemdup(r->pool, first, last - first);
}

AP_DECLARE(void) ap_add_cgi_vars(request_rec *r)
{
    apr_table_t *e = r->subprocess_env;

    apr_table_setn(e, "GATEWAY_INTERFACE", "CGI/1.1");
    apr_table_setn(e, "SERVER_PROTOCOL", r->protocol);
    apr_table_setn(e, "REQUEST_METHOD", r->method);
    apr_table_setn(e, "QUERY_STRING", r->args ? r->args : "");
    apr_table_setn(e, "REQUEST_URI", original_uri(r));

    /* Note that the code below special-cases scripts run from includes,
     * because it "knows" that the sub_request has been hacked to have the
     * args and path_info of the original request, and not any that may have
     * come with the script URI in the include command.  Ugh.
     */

    if (!strcmp(r->protocol, "INCLUDED")) {
        apr_table_setn(e, "SCRIPT_NAME", r->uri);
        if (r->path_info && *r->path_info) {
            apr_table_setn(e, "PATH_INFO", r->path_info);
        }
    }
    else if (!r->path_info || !*r->path_info) {
        apr_table_setn(e, "SCRIPT_NAME", r->uri);
    }
    else {
        int path_info_start = ap_find_path_info(r->uri, r->path_info);

        apr_table_setn(e, "SCRIPT_NAME",
                      apr_pstrndup(r->pool, r->uri, path_info_start));

        apr_table_setn(e, "PATH_INFO", r->path_info);
    }

    //if (r->path_info && r->path_info[0]) {
        /*
         * To get PATH_TRANSLATED, treat PATH_INFO as a URI path.
         * Need to re-escape it for this, since the entire URI was
         * un-escaped before we determined where the PATH_INFO began.
         */
    //    request_rec *pa_req;

    //    pa_req = ap_sub_req_lookup_uri(ap_escape_uri(r->pool, r->path_info), r,
    //                                   NULL);

    //    if (pa_req->filename) {
    //        char *pt = apr_pstrcat(r->pool, pa_req->filename, pa_req->path_info,
    //                              NULL);
//#ifdef WIN32
    //        /* We need to make this a real Windows path name */
    //        apr_filepath_merge(&pt, "", pt, APR_FILEPATH_NATIVE, r->pool);
//#endif
    //        apr_table_setn(e, "PATH_TRANSLATED", pt);
    //    }
    //    ap_destroy_sub_req(pa_req);
    //}
}

AP_DECLARE(void) ap_add_common_vars(request_rec *r)
{
    apr_table_t *e;
    server_rec *s = r->server;
    conn_rec *c = r->connection;
    //const char *rem_logname;
    char *env_path;
#if defined(WIN32) || defined(OS2) || defined(BEOS)
    char *env_temp;
#endif
    const char *host;
    const apr_array_header_t *hdrs_arr = apr_table_elts(r->headers_in);
    const apr_table_entry_t *hdrs = (const apr_table_entry_t *) hdrs_arr->elts;
    int i;
    apr_port_t rport;

    /* use a temporary apr_table_t which we'll overlap onto
     * r->subprocess_env later
     * (exception: if r->subprocess_env is empty at the start,
     * write directly into it)
     */
    if (apr_is_empty_table(r->subprocess_env)) {
        e = r->subprocess_env;
    }
    else {
        e = apr_table_make(r->pool, 25 + hdrs_arr->nelts);
    }

    /* First, add environment vars from headers... this is as per
     * CGI specs, though other sorts of scripting interfaces see
     * the same vars...
     */

    for (i = 0; i < hdrs_arr->nelts; ++i) {
        if (!hdrs[i].key) {
            continue;
        }

        /* A few headers are special cased --- Authorization to prevent
         * rogue scripts from capturing passwords; content-type and -length
         * for no particular reason.
         */

        if (!strcasecmp(hdrs[i].key, "Content-type")) {
            apr_table_addn(e, "CONTENT_TYPE", hdrs[i].val);
        }
        else if (!strcasecmp(hdrs[i].key, "Content-length")) {
            apr_table_addn(e, "CONTENT_LENGTH", hdrs[i].val);
        }
        /*
         * You really don't want to disable this check, since it leaves you
         * wide open to CGIs stealing passwords and people viewing them
         * in the environment with "ps -e".  But, if you must...
         */
#ifndef SECURITY_HOLE_PASS_AUTHORIZATION
        else if (!strcasecmp(hdrs[i].key, "Authorization")
                 || !strcasecmp(hdrs[i].key, "Proxy-Authorization")) {
            continue;
        }
#endif
        else {
            apr_table_addn(e, http2env(r->pool, hdrs[i].key), hdrs[i].val);
        }
    }

    if (!(env_path = getenv("PATH"))) {
        env_path = DEFAULT_PATH;
    }
    apr_table_addn(e, "PATH", apr_pstrdup(r->pool, env_path));

#ifdef WIN32
    if ((env_temp = getenv("SystemRoot")) != NULL) {
        apr_table_addn(e, "SystemRoot", env_temp);
    }
    if ((env_temp = getenv("COMSPEC")) != NULL) {
        apr_table_addn(e, "COMSPEC", env_temp);
    }
    if ((env_temp = getenv("PATHEXT")) != NULL) {
        apr_table_addn(e, "PATHEXT", env_temp);
    }
    if ((env_temp = getenv("WINDIR")) != NULL) {
        apr_table_addn(e, "WINDIR", env_temp);
    }
#endif

#ifdef OS2
    if ((env_temp = getenv("COMSPEC")) != NULL) {
        apr_table_addn(e, "COMSPEC", env_temp);
    }
    if ((env_temp = getenv("ETC")) != NULL) {
        apr_table_addn(e, "ETC", env_temp);
    }
    if ((env_temp = getenv("DPATH")) != NULL) {
        apr_table_addn(e, "DPATH", env_temp);
    }
    if ((env_temp = getenv("PERLLIB_PREFIX")) != NULL) {
        apr_table_addn(e, "PERLLIB_PREFIX", env_temp);
    }
#endif

#ifdef BEOS
    if ((env_temp = getenv("LIBRARY_PATH")) != NULL) {
        apr_table_addn(e, "LIBRARY_PATH", env_temp);
    }
#endif

    apr_table_addn(e, "SERVER_SIGNATURE", ap_psignature("", r));
    apr_table_addn(e, "SERVER_SOFTWARE", ap_get_server_banner());
    apr_table_addn(e, "SERVER_NAME",
                   ap_escape_html(r->pool, ap_get_server_name(r)));
    apr_table_addn(e, "SERVER_ADDR", r->connection->local_ip);  /* Apache */
    apr_table_addn(e, "SERVER_PORT",
                  apr_psprintf(r->pool, "%u", ap_get_server_port(r)));
    host = ap_get_remote_host(c, r->per_dir_config, REMOTE_HOST, NULL);
    if (host) {
        apr_table_addn(e, "REMOTE_HOST", host);
    }
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
    apr_table_addn(e, "REMOTE_ADDR", c->remote_ip);
#else
    apr_table_addn(e, "REMOTE_ADDR", c->client_ip);
#endif
    apr_table_addn(e, "DOCUMENT_ROOT", ap_document_root(r));    /* Apache */
    apr_table_addn(e, "SERVER_ADMIN", s->server_admin); /* Apache */
    apr_table_addn(e, "SCRIPT_FILENAME", r->filename);  /* Apache */

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
    rport = c->remote_addr->port;
#else
    rport = c->client_addr->port;
#endif
    apr_table_addn(e, "REMOTE_PORT", apr_itoa(r->pool, rport));

    if (r->user) {
        apr_table_addn(e, "REMOTE_USER", r->user);
    }
    else if (r->prev) {
        request_rec *back = r->prev;

        while (back) {
            if (back->user) {
                apr_table_addn(e, "REDIRECT_REMOTE_USER", back->user);
                break;
            }
            back = back->prev;
        }
    }
    if (r->ap_auth_type) {
        apr_table_addn(e, "AUTH_TYPE", r->ap_auth_type);
    }
    //rem_logname = ap_get_remote_logname(r);
    //if (rem_logname) {
    //    apr_table_addn(e, "REMOTE_IDENT", apr_pstrdup(r->pool, rem_logname));
    //}

    /* Apache custom error responses. If we have redirected set two new vars */

    if (r->prev) {
        if (r->prev->args) {
            apr_table_addn(e, "REDIRECT_QUERY_STRING", r->prev->args);
        }
        if (r->prev->uri) {
            apr_table_addn(e, "REDIRECT_URL", r->prev->uri);
        }
    }

    if (e != r->subprocess_env) {
      apr_table_overlap(r->subprocess_env, e, APR_OVERLAP_TABLES_SET);
    }
}

#ifndef WIN32

unixd_config_rec unixd_config;
const char *ap_server_argv0 = "nginx";

#ifdef HAVE_GETPWNAM
AP_DECLARE(uid_t) ap_uname2id(const char *name)
{
    struct passwd *ent;

    if (name[0] == '#')
        return (atoi(&name[1]));

    if (!(ent = getpwnam(name))) {
        ap_log_error(APLOG_MARK, APLOG_STARTUP, 0, NULL,
                     "%s: bad user name %s", ap_server_argv0, name);
        exit(1);
    }

    return (ent->pw_uid);
}
#endif

#ifdef HAVE_GETGRNAM
AP_DECLARE(gid_t) ap_gname2id(const char *name)
{
    struct group *ent;

    if (name[0] == '#')
        return (atoi(&name[1]));

    if (!(ent = getgrnam(name))) {
        ap_log_error(APLOG_MARK, APLOG_STARTUP, 0, NULL,
                     "%s: bad group name %s", ap_server_argv0, name);
        exit(1);
    }

    return (ent->gr_gid);
}
#endif

AP_DECLARE(void) unixd_pre_config(apr_pool_t *ptemp)
{
    apr_finfo_t wrapper;

    unixd_config.user_name = DEFAULT_USER;
    unixd_config.user_id = ap_uname2id(DEFAULT_USER);
    unixd_config.group_id = ap_gname2id(DEFAULT_GROUP);
    /* unixd_config.chroot_dir = NULL; none */

    /* Check for suexec */
    unixd_config.suexec_enabled = 0;
/*    if ((apr_stat(&wrapper, SUEXEC_BIN,
                  APR_FINFO_NORM, ptemp)) != APR_SUCCESS) {
        return;
    }

    if ((wrapper.protection & APR_USETID) && wrapper.user == 0) {
        unixd_config.suexec_enabled = 1;
    }*/
}

/* XXX move to APR and externalize (but implement differently :) ) */
static apr_lockmech_e proc_mutex_mech(apr_proc_mutex_t *pmutex)
{
    const char *mechname = apr_proc_mutex_name(pmutex);

    if (!strcmp(mechname, "sysvsem")) {
        return APR_LOCK_SYSVSEM;
    }
    else if (!strcmp(mechname, "flock")) {
        return APR_LOCK_FLOCK;
    }
    return APR_LOCK_DEFAULT;
}

AP_DECLARE(apr_status_t) unixd_set_proc_mutex_perms(apr_proc_mutex_t *pmutex)
{
////////////////
    if (unixd_config.user_name == NULL)
    {
        unixd_pre_config(NULL);
    }
////////////////
    if (!geteuid()) {
        apr_lockmech_e mech = proc_mutex_mech(pmutex);

        switch(mech) {
#if APR_HAS_SYSVSEM_SERIALIZE
        case APR_LOCK_SYSVSEM:
        {
            apr_os_proc_mutex_t ospmutex;
#if !APR_HAVE_UNION_SEMUN
            union semun {
                long val;
                struct semid_ds *buf;
                unsigned short *array;
            };
#endif
            union semun ick;
            struct semid_ds buf;
            memset(&buf,0,sizeof(buf));

            apr_os_proc_mutex_get(&ospmutex, pmutex);
            buf.sem_perm.uid = unixd_config.user_id;
            buf.sem_perm.gid = unixd_config.group_id;
            buf.sem_perm.mode = 0600;
            ick.buf = &buf;
            if (semctl(ospmutex.crossproc, 0, IPC_SET, ick) < 0) {
                return errno;
            }
        }
        break;
#endif
#if APR_HAS_FLOCK_SERIALIZE
        case APR_LOCK_FLOCK:
        {
            const char *lockfile = apr_proc_mutex_lockfile(pmutex);

            if (lockfile) {
                if (chown(lockfile, unixd_config.user_id,
                          -1 /* no gid change */) < 0) {
                    return errno;
                }
            }
        }
        break;
#endif
        default:
            /* do nothing */
            break;
        }
    }
    return APR_SUCCESS;
}

AP_DECLARE(apr_status_t) unixd_set_global_mutex_perms(apr_global_mutex_t *gmutex)
{
#if !APR_PROC_MUTEX_IS_GLOBAL
    apr_os_global_mutex_t osgmutex;
    apr_os_global_mutex_get(&osgmutex, gmutex);
    return unixd_set_proc_mutex_perms(osgmutex.proc_mutex);
#else  /* APR_PROC_MUTEX_IS_GLOBAL */
    /* In this case, apr_proc_mutex_t and apr_global_mutex_t are the same. */
    return unixd_set_proc_mutex_perms(gmutex);
#endif /* APR_PROC_MUTEX_IS_GLOBAL */
}

#endif
