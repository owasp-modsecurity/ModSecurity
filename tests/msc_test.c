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

#include <apr.h>
#include <apr_getopt.h>

#include "modsecurity.h"
#include "re.h"

#define ISHEX(X) (((X >= '0')&&(X <= '9')) || ((X >= 'a')&&(X <= 'f')) || ((X >= 'A')&&(X <= 'F')))

#define BUFLEN                   32768

#define RESULT_SUCCESS           0
#define RESULT_ERROR            -1
#define RESULT_MISMATCHED       -2
#define RESULT_WRONGSIZE        -3
#define RESULT_WRONGRET         -4

#define DEFAULT_ACTION           "phase:2,log,auditlog,pass"

#define CMDLINE_OPTS             "t:n:p:P:r:I:D:Nh"

/* Types */
typedef struct tfn_data_t tfn_data_t;
typedef struct op_data_t op_data_t;
typedef struct action_data_t action_data_t;

struct tfn_data_t {
    const char               *name;
    const char               *param;
    unsigned char            *input;
    apr_size_t                input_len;
    msre_tfn_metadata        *metadata;
};

struct op_data_t {
    const char               *name;
    const char               *param;
    unsigned char            *input;
    apr_size_t                input_len;
    msre_ruleset             *ruleset;
    msre_rule                *rule;
    msre_var                 *var;
    msre_op_metadata         *metadata;
};

struct action_data_t {
    const char               *name;
    unsigned char            *input;
    apr_size_t                input_len;
    msre_ruleset             *ruleset;
    msre_rule                *rule;
    msre_var                 *var;
    msre_actionset           *actionset;
    msre_action              *action;
};


/* Globals */
static int debuglog_level = 0;
static char *test_name = NULL;
static apr_pool_t *g_mp = NULL;
static modsec_rec *g_msr = NULL;
static unsigned char buf[BUFLEN];
msc_engine *modsecurity = NULL;
unsigned long int DSOLOCAL msc_pcre_match_limit = 0;
unsigned long int DSOLOCAL msc_pcre_match_limit_recursion = 0;

/* Stubs */
char *format_error_log_message(apr_pool_t *mp, error_message_t *em) {
    return "FAKE ERROR LOG MESSAGE";
}

apr_status_t send_error_bucket(modsec_rec *msr, ap_filter_t *f, int status) {
    return APR_SUCCESS;
}

int apache2_exec(modsec_rec *msr, const char *command, const char **argv, char **output) {
    return 0;
}

char *get_apr_error(apr_pool_t *p, apr_status_t rc) {
    char *text = apr_pcalloc(p, 201);
    if (text == NULL) return NULL;
    apr_strerror(rc, text, 200);
    return text;
}

void msr_log(modsec_rec *msr, int level, const char *text, ...) {
    va_list ap;
    char str1[1024] = "";
    char str2[1256] = "";

    if ((msr == NULL) || (level > msr->txcfg->debuglog_level)) {
        return;
    }
    if (msr->txcfg->debuglog_fd == NOT_SET_P) {
        if (apr_file_open(&msr->txcfg->debuglog_fd, msr->txcfg->debuglog_name, APR_READ|APR_WRITE|APR_CREATE|APR_APPEND|APR_BINARY, APR_OS_DEFAULT, g_mp) != APR_SUCCESS) {
            fprintf(stderr, "ERROR: failed to create unit test debug log \"%s\".\n", msr->txcfg->debuglog_name);
            msr->txcfg->debuglog_fd = NULL;
        }
    }

    va_start(ap, text);
    if (msr->txcfg->debuglog_fd != NULL) {
        apr_size_t nbytes_written = 0;
        apr_vsnprintf(str1, sizeof(str1), text, ap);
        apr_snprintf(str2, sizeof(str2), "%lu: [%d] [%s] %s\n", (unsigned long)getpid(), level, test_name, str1);

        apr_file_write_full(msr->txcfg->debuglog_fd, str2, strlen(str2), &nbytes_written);
    }
    va_end(ap);
}

void msr_log_error(modsec_rec *msr, const char *text, ...) {
    va_list ap;
    int level = 3;
    char str1[1024] = "";
    char str2[1256] = "";

    if ((msr == NULL) || (level > msr->txcfg->debuglog_level)) {
        return;
    }
    if (msr->txcfg->debuglog_fd == NOT_SET_P) {
        if (apr_file_open(&msr->txcfg->debuglog_fd, msr->txcfg->debuglog_name, APR_READ|APR_WRITE|APR_CREATE|APR_APPEND|APR_BINARY, APR_OS_DEFAULT, g_mp) != APR_SUCCESS) {
            fprintf(stderr, "ERROR: failed to create unit test debug log \"%s\".\n", msr->txcfg->debuglog_name);
            msr->txcfg->debuglog_fd = NULL;
        }
    }

    va_start(ap, text);
    if (msr->txcfg->debuglog_fd != NULL) {
        apr_size_t nbytes_written = 0;
        apr_vsnprintf(str1, sizeof(str1), text, ap);
        apr_snprintf(str2, sizeof(str2), "%lu: [%d] [%s] %s\n", (unsigned long)getpid(), level, test_name, str1);

        apr_file_write_full(msr->txcfg->debuglog_fd, str2, strlen(str2), &nbytes_written);
    }
    va_end(ap);
}

void msr_log_warn(modsec_rec *msr, const char *text, ...) {
    va_list ap;
    int level = 4;
    char str1[1024] = "";
    char str2[1256] = "";

    if ((msr == NULL) || (level > msr->txcfg->debuglog_level)) {
        return;
    }
    if (msr->txcfg->debuglog_fd == NOT_SET_P) {
        if (apr_file_open(&msr->txcfg->debuglog_fd, msr->txcfg->debuglog_name, APR_READ|APR_WRITE|APR_CREATE|APR_APPEND|APR_BINARY, APR_OS_DEFAULT, g_mp) != APR_SUCCESS) {
            fprintf(stderr, "ERROR: failed to create unit test debug log \"%s\".\n", msr->txcfg->debuglog_name);
            msr->txcfg->debuglog_fd = NULL;
        }
    }

    va_start(ap, text);
    if (msr->txcfg->debuglog_fd != NULL) {
        apr_size_t nbytes_written = 0;
        apr_vsnprintf(str1, sizeof(str1), text, ap);
        apr_snprintf(str2, sizeof(str2), "%lu: [%d] [%s] %s\n", (unsigned long)getpid(), level, test_name, str1);

        apr_file_write_full(msr->txcfg->debuglog_fd, str2, strlen(str2), &nbytes_written);
    }
    va_end(ap);
}

const char *ap_get_remote_host(conn_rec *conn, void *dir_config, int type, int *str_is_ip) {
    return "FAKE-REMOTE-HOST";
}

char *get_env_var(request_rec *r, char *name) {
    return "FAKE-ENV-VAR";
}

apr_status_t unixd_set_global_mutex_perms(apr_global_mutex_t *gmutex) {
    return APR_SUCCESS;
}

apr_status_t unixd_set_proc_mutex_perms(apr_proc_mutex_t *pmutex) {
    return APR_SUCCESS;
}


/* Escaping functions */

static unsigned char hex2dec(unsigned char *what) {
    register unsigned char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A') + 10 : (what[1] - '0'));

    return digit;
}

static unsigned char *unescape_inplace(unsigned char *str, apr_size_t *len)
{
    apr_size_t i, j;
    for (i = j = 0; i < *len; j++) {
        if ((str[i] == '\\') && (i + 3 < *len) && (str[i + 1] == 'x') && ISHEX(str[i + 2]) && ISHEX(str[i + 3]) ) {
            str[j] = hex2dec(str + i + 2);
            i += 4;
        }
        else {
            str[j] = str[i++];
        }
    }
    *len = j;

    while (j < i) {
        str[j++] = '\0';
    }

    return str;
}

static char *escape(unsigned char *str, apr_size_t *len)
{
    char *new = apr_pcalloc(g_mp, (*len * 4) + 1);
    apr_size_t i, j;
    for (i = j = 0; i < *len; i++) {
        if ((str[i] >= 0x20) && (str[i] <= 0x7e)) {
            new[j++] = str[i];
        }
        else {
            sprintf(new + j, "\\x%02x", str[i]);
            j += 4;
        }
    }
    *len = j;

    return new;
}


/* Testing functions */

static int init_tfn(tfn_data_t *data, const char *name, unsigned char *input, apr_size_t input_len, char **errmsg) {
    *errmsg = NULL;

    data->name = name;
    data->input = apr_pmemdup(g_mp, input, input_len);
    data->input_len = input_len;
    data->metadata = msre_engine_tfn_resolve(modsecurity->msre, name);
    if (data->metadata == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to fetch tfn \"%s\".", name);
        return -1;
    }

    return 0;
}

static int test_tfn(tfn_data_t *data, unsigned char **rval, apr_size_t *rval_len, char **errmsg)
{
    int rc = -1;

    *errmsg = NULL;

    /* Execute the tfn */
    rc = data->metadata->execute(g_mp, data->input, (long)(data->input_len), (char **)rval, (long *)rval_len);
    if (rc < 0) {
        *errmsg = apr_psprintf(g_mp, "Failed to execute tfn \"%s\".", data->name);
    }

    return rc;
}

static int init_op(op_data_t *data, const char *name, const char *param, unsigned char *input, apr_size_t input_len, char **errmsg) {
    const char *args = apr_psprintf(g_mp, "@%s %s", name, param);
    char *conf_fn;
    int rc = -1;

    *errmsg = NULL;

    data->name = name;
    data->param = param;
    data->input = input;
    data->input_len = input_len;

    if ( apr_filepath_merge(&conf_fn, NULL, "unit-test.conf", APR_FILEPATH_TRUENAME, g_mp) != APR_SUCCESS) {
        *errmsg = apr_psprintf(g_mp, "Failed to build a conf filename.");
        return -1;
    }

    /* Register UNIT_TEST variable */
    msre_engine_variable_register(modsecurity->msre,
        "UNIT_TEST",
        VAR_SIMPLE,
        0, 0,
        NULL,
        NULL,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* Lookup the operator */
    data->metadata = msre_engine_op_resolve(modsecurity->msre, name);
    if (data->metadata == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to fetch op \"%s\".", name);
        return -1;
    }

    /* Create a ruleset/rule */
    data->ruleset = msre_ruleset_create(modsecurity->msre, g_mp);
    if (data->ruleset == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to create ruleset for op \"%s\".", name);
        return -1;
    }
    data->rule = msre_rule_create(data->ruleset, RULE_TYPE_NORMAL, conf_fn, 1, "UNIT_TEST", args, DEFAULT_ACTION, errmsg);
    if (data->rule == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to create rule for op \"%s\": %s", name, *errmsg);
        return -1;
    }

    /* Create a fake variable */
    data->var = (msre_var *)apr_pcalloc(g_mp, sizeof(msre_var));
    data->var->name = "UNIT_TEST";
    data->var->value = apr_pstrmemdup(g_mp, (char *)input, input_len);
    data->var->value_len = input_len;
    data->var->metadata = msre_resolve_var(modsecurity->msre, data->var->name);
    if (data->var->metadata == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to resolve variable for op \"%s\": %s", name, data->var->name);
        return -1;
    }

    /* Initialize the operator parameter */
    if (data->metadata->param_init != NULL) {
        rc = data->metadata->param_init(data->rule, errmsg);
        if (rc <= 0) {
            *errmsg = apr_psprintf(g_mp, "Failed to init op \"%s\": %s", name, *errmsg);
            return rc;
        }
    }

    return 0;
}

static int test_op(op_data_t *data, char **errmsg)
{
    int rc = -1;

    *errmsg = NULL;

    /* Execute the operator */
    if (data->metadata->execute != NULL) {
        rc = data->metadata->execute(g_msr, data->rule, data->var, errmsg);
        if (rc < 0) {
            *errmsg = apr_psprintf(g_mp, "Failed to execute op \"%s\": %s", data->name, *errmsg);
        }
    }

    return rc;
}

static int init_action(action_data_t *data, const char *name, const char *param, char **errmsg)
{
    const char *action_string = NULL;
    char *conf_fn;

    *errmsg = NULL;

    if ((param == NULL) || (strcmp("", param) == 0)) {
        action_string = apr_psprintf(g_mp, "%s", name);
    }
    else {
        action_string = apr_psprintf(g_mp, "%s:%s", name, param);
    }
    if (action_string == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to build action string for action: \"%s\".", name);
        return -1;
    }

    if ( apr_filepath_merge(&conf_fn, NULL, "unit-test.conf", APR_FILEPATH_TRUENAME, g_mp) != APR_SUCCESS) {
        *errmsg = apr_psprintf(g_mp, "Failed to build a conf filename.");
        return -1;
    }

    /* Register UNIT_TEST variable */
    msre_engine_variable_register(modsecurity->msre,
        "UNIT_TEST",
        VAR_SIMPLE,
        0, 0,
        NULL,
        NULL,
        VAR_DONT_CACHE,
        PHASE_REQUEST_HEADERS
    );

    /* Create a ruleset/rule */
    data->ruleset = msre_ruleset_create(modsecurity->msre, g_mp);
    if (data->ruleset == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to create ruleset for action \"%s\".", name);
        return -1;
    }
    data->rule = msre_rule_create(data->ruleset, RULE_TYPE_NORMAL, conf_fn, 1, "UNIT_TEST", "@unconditionalMatch", action_string, errmsg);
    if (data->rule == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to create rule for action \"%s\": %s", name, *errmsg);
        return -1;
    }

    /* Get the actionset/action */
    data->actionset = data->rule->actionset;
    if (data->actionset == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to fetch actionset for action \"%s\"", name);
        return -1;
    }
    data->action = (msre_action *)apr_table_get(data->actionset->actions, name);
    if (data->action == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to fetch action for action \"%s\"", name);
        return -1;
    }

    return 0;
}

static int test_action(action_data_t *data, char **errmsg)
{
    int rc = -1;

    *errmsg = NULL;

    /* Execute the action */
    if (data->action->metadata->execute != NULL) {
        rc = data->action->metadata->execute(g_msr, g_mp, data->rule, data->action);
        if (rc < 0) {
            *errmsg = apr_psprintf(g_mp, "Failed to execute action \"%s\": %d", data->name, rc);
        }
    }

    return rc;
}


/* Initialization */
static void init_msr(void)
{
    directory_config *dcfg = NULL;
    request_rec *r = NULL;
    r = (request_rec *)apr_pcalloc(g_mp, sizeof(request_rec));

    dcfg = (directory_config *)apr_pcalloc(g_mp, sizeof(directory_config));
    dcfg->is_enabled = 0;
    dcfg->reqbody_access = 0;
    dcfg->reqbody_buffering = 0;
    dcfg->reqbody_inmemory_limit = REQUEST_BODY_DEFAULT_INMEMORY_LIMIT;
    dcfg->reqbody_limit = REQUEST_BODY_DEFAULT_LIMIT;
    dcfg->reqbody_no_files_limit = REQUEST_BODY_NO_FILES_DEFAULT_LIMIT;
    dcfg->resbody_access = 0;
    dcfg->of_limit = RESPONSE_BODY_DEFAULT_LIMIT;
    dcfg->of_limit_action = RESPONSE_BODY_LIMIT_ACTION_REJECT;
    dcfg->debuglog_fd = NOT_SET_P;
    dcfg->debuglog_name = "msc-test-debug.log";
    dcfg->debuglog_level = debuglog_level;
    dcfg->cookie_format = 0;
    dcfg->argument_separator = '&';
    dcfg->rule_inheritance = 0;
    dcfg->auditlog_flag = 0;
    dcfg->auditlog_type = AUDITLOG_SERIAL;
    dcfg->auditlog_fd = NULL;
    dcfg->auditlog2_fd = NULL;
    dcfg->auditlog_name = NULL;
    dcfg->auditlog2_name = NULL;
    dcfg->auditlog_storage_dir = NULL;
    dcfg->auditlog_parts = "ABCFHZ";
    dcfg->auditlog_relevant_regex = NULL;
    dcfg->tmp_dir = guess_tmp_dir(g_mp);
    dcfg->upload_dir = NULL;
    dcfg->upload_keep_files = KEEP_FILES_OFF;
    dcfg->upload_validates_files = 0;
    dcfg->data_dir = ".";
    dcfg->webappid = "default";
    dcfg->content_injection_enabled = 0;
    dcfg->geo = NULL;
    dcfg->cache_trans = MODSEC_CACHE_ENABLED;
    dcfg->cache_trans_min = 15;
    dcfg->cache_trans_max = 0;
    dcfg->request_encoding = NULL;

    g_msr = (modsec_rec *)apr_pcalloc(g_mp, sizeof(modsec_rec));
    g_msr->modsecurity = modsecurity;
    g_msr->mp = g_mp;
    g_msr->r = r;
    g_msr->r_early = r;
    g_msr->request_time = apr_time_now();
    g_msr->dcfg1 = NULL;
    g_msr->usercfg = NULL;
    g_msr->txcfg = dcfg;
    g_msr->txid = "FAKE-TXID";
    g_msr->error_messages = NULL;
    g_msr->alerts = NULL;
    g_msr->server_software = "FAKE-SERVER-SOFTWARE";
    g_msr->local_addr = "127.0.0.1";
    g_msr->local_port = 80;
    g_msr->remote_addr = "127.0.0.1";
    g_msr->remote_port = 1080;
    g_msr->request_line = "GET /unit-tests HTTP/1.1";
    g_msr->request_uri = "http://localhost/unit-tests";
    g_msr->request_method = "GET";
    g_msr->query_string = "";
    g_msr->request_protocol = "HTTP/1.1";
    g_msr->request_headers = NULL;
    g_msr->hostname = "localhost";
    g_msr->msc_rule_mptmp = g_mp;
    g_msr->tx_vars = apr_table_make(g_mp, 1);
    g_msr->collections_original = apr_table_make(g_mp, 1);
    g_msr->collections = apr_table_make(g_mp, 1);
    g_msr->collections_dirty = apr_table_make(g_mp, 1);
}

/**
 * Usage text.
 */
static void usage(void)
{
    fprintf(stderr, "ModSecurity Unit Tester v%s\n", MODSEC_VERSION);
    fprintf(stderr, "  Usage: msc_test [options]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  Options:\n");
    fprintf(stderr, "    -t        Type (required)\n");
    fprintf(stderr, "    -n        Name (required)\n");
    fprintf(stderr, "    -p        Parameter (required)\n");
    fprintf(stderr, "    -P        Prerun (optional for actions)\n");
    fprintf(stderr, "    -r        Function return code (required for some types)\n");
    fprintf(stderr, "    -I        Iterations (default 1)\n");
    fprintf(stderr, "    -D        Debug log level (default 0)\n");
    fprintf(stderr, "    -N        No input on stdin.\n\n");
    fprintf(stderr, "    -h        This help\n\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Input is from stdin unless -N is used.\n");
}



/* Main */

int main(int argc, const char * const argv[])
{
    apr_getopt_t *opt;
    apr_file_t *fd;
    apr_size_t nbytes = 0;
    const char *type = NULL;
    const char *name = NULL;
    unsigned char *param = NULL;
    unsigned char *prerun = NULL;
    const char *returnval = NULL;
    int iterations = 1;
    char *errmsg = NULL;
    unsigned char *out = NULL;
    apr_size_t param_len = 0;
    apr_size_t prerun_len = 0;
    apr_size_t out_len = 0;
    int noinput = 0;
    int rc = 0;
    int result = 0;
    int ec = 0;
    int i;
    apr_time_t T0 = 0;
    apr_time_t T1 = 0;
    tfn_data_t tfn_data;
    op_data_t op_data;
    action_data_t action_data;
    int ret = 0;

    memset(&tfn_data, 0, sizeof(tfn_data_t));
    memset(&op_data, 0, sizeof(op_data_t));
    memset(&action_data, 0, sizeof(action_data_t));

    apr_app_initialize(&argc, &argv, NULL);
    atexit(apr_terminate);

    apr_pool_create(&g_mp, NULL);

    rc = apr_getopt_init(&opt, g_mp, argc, argv);
    if (rc != APR_SUCCESS) {
        fprintf(stderr, "Failed to initialize.\n\n");
        usage();
        exit(1);
    }

    do {
        char  ch;
        const char *val;
        rc = apr_getopt(opt, CMDLINE_OPTS, &ch, &val);
        switch (rc) {
        case APR_SUCCESS:
            switch (ch) {
                case 't':
                    type = val;
                    break;
                case 'n':
                    name = val;
                    break;
                case 'p':
                    param_len = strlen(val);
                    param = apr_pmemdup(g_mp, val, param_len + 1);
                    unescape_inplace(param, &param_len);
                    break;
                case 'P':
                    prerun_len = strlen(val);
                    prerun = apr_pmemdup(g_mp, val, prerun_len + 1);
                    unescape_inplace(prerun, &prerun_len);
                    break;
                case 'r':
                    returnval = val;
                    break;
                case 'I':
                    iterations = atoi(val);
                    break;
                case 'D':
                    debuglog_level = atoi(val);
                    break;
                case 'N':
                    noinput = 1;
                    break;
                case 'h':
                    usage();
                    exit(0);
            }
            break;
        case APR_BADCH:
        case APR_BADARG:
            usage();
            exit(1);
        }
    } while (rc != APR_EOF);

    rc = apr_getopt_init(&opt, g_mp, argc, argv);
    if (!type || !name || !param) {
        usage();
        exit(1);
    }

    modsecurity = modsecurity_create(g_mp, MODSEC_OFFLINE);
    test_name = apr_psprintf(g_mp, "%s/%s", type, name);

    if (noinput == 0) {
        if (apr_file_open_stdin(&fd, g_mp) != APR_SUCCESS) {
            fprintf(stderr, "Failed to open stdin\n");
            exit(1);
        }

        /* Read in the input */
        nbytes = BUFLEN;
        memset(buf, 0, nbytes);
        rc = apr_file_read(fd, buf, &nbytes);
        if ((rc != APR_EOF) && (rc != APR_SUCCESS)) {
            fprintf(stderr, "Failed to read data\n");
            exit(1);
        }

        if (nbytes < 0) {
            fprintf(stderr, "Error reading data\n");
            exit(1);
        }

        apr_file_close(fd);
    }

    if (strcmp("tfn", type) == 0) {
        ret = returnval ? atoi(returnval) : -8888;

        rc = init_tfn(&tfn_data, name, buf, nbytes, &errmsg);
        if (rc < 0) {
            fprintf(stderr, "ERROR: %s\n", errmsg);
            result = RESULT_ERROR;
        }
    }
    else if (strcmp("op", type) == 0) {
        if (!returnval) {
            fprintf(stderr, "Return value required for type \"%s\"\n", type);
            exit(1);
        }
        ret = atoi(returnval);

        init_msr();

        rc = init_op(&op_data, name, (const char *)param, buf, nbytes, &errmsg);
        if (rc < 0) {
            fprintf(stderr, "ERROR: %s\n", errmsg);
            result = RESULT_ERROR;
        }
    }
    else if (strcmp("action", type) == 0) {
        if (!returnval) {
            fprintf(stderr, "Return value required for type \"%s\"\n", type);
            exit(1);
        }
        ret = atoi(returnval);

        init_msr();

        if (prerun) {
            action_data_t paction_data;
            char *pname = apr_pstrdup(g_mp, (const char *)prerun);
            char *pparam = NULL;

            if ((pparam = strchr((const char *)pname, ':'))) {
                pparam[0] = '\0';
                pparam++;
            }

            rc = init_action(&paction_data, pname, (const char *)pparam, &errmsg);
            if (rc < 0) {
                fprintf(stderr, "ERROR: prerun - %s\n", errmsg);
                exit(1);
            }

            rc = test_action(&paction_data, &errmsg);
            if (rc < 0) {
                fprintf(stderr, "ERROR: prerun - %s\n", errmsg);
                exit(1);
            }
        }

        rc = init_action(&action_data, name, (const char *)param, &errmsg);
        if (rc < 0) {
            fprintf(stderr, "ERROR: %s\n", errmsg);
            result = RESULT_ERROR;
        }
    }

    if (iterations > 1) {
        apr_time_clock_hires (g_mp);
        T0 = apr_time_now();
    }

    for (i = 1; i <= iterations; i++) {
        #ifdef VERBOSE
        if (i % 100 == 0) {
            if (i == 100) {
                fprintf(stderr, "Iterations/100: .");
            }
            else {
                fprintf(stderr, ".");
            }
        }
        #endif

        if (strcmp("tfn", type) == 0) {
            /* Transformations */
            rc = test_tfn(&tfn_data, &out, &out_len, &errmsg);
            if (rc < 0) {
                fprintf(stderr, "ERROR: %s\n", errmsg);
                result = RESULT_ERROR;
            }
            else if ((ret != -8888) && (rc != ret)) {
                fprintf(stderr, "Returned %d (expected %d)\n", rc, ret);
                result = RESULT_WRONGRET;
            }
            else if (param_len != out_len) {
                fprintf(stderr, "Length %" APR_SIZE_T_FMT " (expected %" APR_SIZE_T_FMT ")\n", out_len, param_len);
                result = RESULT_WRONGSIZE;
            }
            else {
                result = memcmp(param, out, param_len) ? RESULT_MISMATCHED : RESULT_SUCCESS;
            }

            if (result != RESULT_SUCCESS) {
                apr_size_t s0len = nbytes;
                const char *s0 = escape(buf, &s0len);
                apr_size_t s1len = out_len;
                const char *s1 = escape(out, &s1len);
                apr_size_t s2len = param_len;
                const char *s2 = escape(param, &s2len);

                fprintf(stderr, " Input: '%s' len=%" APR_SIZE_T_FMT "\n"
                                "Output: '%s' len=%" APR_SIZE_T_FMT "\n"
                                "Expect: '%s' len=%" APR_SIZE_T_FMT "\n",
                                s0, nbytes, s1, out_len, s2, param_len);
                ec = 1;
            }
        }
        else if (strcmp("op", type) == 0) {
            /* Operators */
            rc = test_op(&op_data, &errmsg);
            if (rc < 0) {
                fprintf(stderr, "ERROR: %s\n", errmsg);
                result = RESULT_ERROR;
            }
            else if (rc != ret) {
                fprintf(stderr, "Returned %d (expected %d)\n", rc, ret);
                result = RESULT_WRONGRET;
            }
            else {
                result = RESULT_SUCCESS;
            }

            if (result != RESULT_SUCCESS) {
                apr_size_t s0len = nbytes;
                const char *s0 = escape(buf, &s0len);

                fprintf(stderr, " Test: '@%s %s'\n"
                                "Input: '%s' len=%" APR_SIZE_T_FMT "\n",
                                name, param, s0, nbytes);
                ec = 1;
            }
        }
        else if (strcmp("action", type) == 0) {
            /* Actions */
            int n;
            const apr_array_header_t *arr;
            apr_table_entry_t *te;

            rc = test_action(&action_data, &errmsg);
            if (rc < 0) {
                fprintf(stderr, "ERROR: %s\n", errmsg);
                result = RESULT_ERROR;
            }
            else if (rc != ret) {
                fprintf(stderr, "Returned %d (expected %d)\n", rc, ret);
                result = RESULT_WRONGRET;
            }
            else {
                result = RESULT_SUCCESS;
            }

            if (result != RESULT_SUCCESS) {
                fprintf(stderr, "  Test: '%s:%s'\n"
                                "Prerun: '%s'\n",
                                name, param, (prerun ? (const char *)prerun : ""));
                ec = 1;
            }

            /* Store any collections that were initialized and changed */
            arr = apr_table_elts(g_msr->collections);
            te = (apr_table_entry_t *)arr->elts;
            for (n = 0; n < arr->nelts; n++) {
                apr_table_t *col = (apr_table_t *)te[n].val;
//                apr_table_t *orig_col = NULL;

                if (g_msr->txcfg->debuglog_level >= 9) {
                    msr_log(g_msr, 9, "Found loaded collection: %s", te[n].key);
                }
                /* Only store those collections that changed. */
                if (apr_table_get(g_msr->collections_dirty, te[n].key)) {
                    int x = collection_store(g_msr, col);

                    if (g_msr->txcfg->debuglog_level >= 9) {
                        msr_log(g_msr, 9, "Stored collection: %s (%d)", te[n].key, x);
                    }
                }
#if 0
                /* Re-populate the original values with the new ones. */
                if ((orig_col = (apr_table_t *)apr_table_get(g_msr->collections_original, te[n].key)) != NULL) {
                    const apr_array_header_t *orig_arr = apr_table_elts(orig_col);
                    apr_table_entry_t *orig_te = (apr_table_entry_t *)orig_arr->elts;
                    int m;

                    for (m = 0; m < orig_arr->nelts; m++) {
                        msc_string *mstr = (msc_string *)apr_table_get(col, orig_te[m].key);

                        if (g_msr->txcfg->debuglog_level >= 9) {
                            msr_log(g_msr, 9, "Updating original collection: %s.%s=%s", te[n].key, mstr->name, mstr->value);
                        }
                        //apr_table_setn(orig_col, orig_te[m].key, (void *)mstr );
                        collection_original_setvar(g_msr, te[n].key, mstr);

                        
                    }
                }
#endif
            }
            apr_table_clear(g_msr->collections_dirty);
            apr_table_clear(g_msr->collections_original);
        }
        else {
            fprintf(stderr, "Unknown type: \"%s\"\n", type);
            exit(1);
        }

        if (ec != 0) {
            fprintf(stdout, "%s\n", errmsg ? errmsg : "");
            return ec;
        }
    }

    if (iterations > 1) {
        double dT;
        T1 = apr_time_now();

        dT = apr_time_as_msec(T1 - T0);

        #ifdef VERBOSE
        if (i >= 100) {
            fprintf(stderr, "\n");
        }
        #endif

        fprintf(stdout, "%d @ %.4f msec per iteration.\n", iterations, dT / iterations);
    }
    fprintf(stdout, "%s\n", errmsg ? errmsg : "");

    return ec;
}


