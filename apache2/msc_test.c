/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2008 Breach Security, Inc. (http://www.breach.com/)
 *
 * You should have received a copy of the licence along with this
 * program (stored in the file "LICENSE"). If the file is missing,
 * or if you have any other questions related to the licence, please
 * write to Breach Security, Inc. at support@breach.com.
 *
 */
#include <apr.h>

#include "modsecurity.h"
#include "re.h"
#include "pdf_protect.h"

#define ISHEX(X) (((X >= '0')&&(X <= '9')) || ((X >= 'a')&&(X <= 'f')) || ((X >= 'A')&&(X <= 'F')))

#define BUFLEN 8192

#define RESULT_SUCCESS           0
#define RESULT_ERROR            -1
#define RESULT_MISMATCHED       -2
#define RESULT_WRONGSIZE        -3
#define RESULT_WRONGRET         -4

/* Globals */
static apr_pool_t *g_mp = NULL;
static modsec_rec *g_msr = NULL;
msc_engine *modsecurity = NULL;


/* Stubs */
char *format_error_log_message(apr_pool_t *mp, error_message *em) {
    return "FAKE ERROR LOG MESSAGE";
}

apr_status_t send_error_bucket(modsec_rec *msr, ap_filter_t *f, int status) {
    return APR_SUCCESS;
}

int apache2_exec(modsec_rec *msr, const char *command, const char **argv, char **output) {
    return 0;
}

char *get_apr_error(apr_pool_t *p, apr_status_t rc) {
    return "FAKE APR ERROR";
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
        apr_snprintf(str2, sizeof(str2), "[%d] %s\n", level, str1);

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

static int test_tfn(const char *name, unsigned char *input, apr_size_t input_len, unsigned char **rval, apr_size_t *rval_len, char **errmsg)
{
    int rc = -1;
    msre_tfn_metadata *metadata = NULL;

    *errmsg = NULL;

    /* Lookup the tfn */
    metadata = msre_engine_tfn_resolve(modsecurity->msre, name);

    if (metadata == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to fetch tfn \"%s\".", name);
        return -1;
    }

    /* Execute the tfn */
    rc = metadata->execute(g_mp, input, (long)input_len, (char **)rval, (long *)rval_len);
    if (rc < 0) {
        *errmsg = apr_psprintf(g_mp, "Failed to execute tfn \"%s\".", name);
    }

    return rc;
}

static int test_op(const char *name, const char *param, const unsigned char *input, apr_size_t input_len, char **errmsg)
{
    const char *args = apr_psprintf(g_mp, "@%s %s", name, param);
    char *conf_fn;
    msre_ruleset *ruleset = NULL;
    msre_rule *rule = NULL;
    msre_var *var = NULL;
    msre_op_metadata *metadata = NULL;
    int rc = -1;

    *errmsg = NULL;

    if ( apr_filepath_merge(&conf_fn, NULL, "t/unit-test.conf", APR_FILEPATH_TRUENAME, g_mp) != APR_SUCCESS) {
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
    metadata = msre_engine_op_resolve(modsecurity->msre, name);
    if (metadata == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to fetch op \"%s\".", name);
        return -1;
    }

    /* Create a ruleset/rule */
    ruleset = msre_ruleset_create(modsecurity->msre, g_mp);
    if (ruleset == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to create ruleset for op \"%s\".", name);
        return -1;
    }
    rule = msre_rule_create(ruleset, RULE_TYPE_NORMAL, conf_fn, 1, "UNIT_TEST", args, "t:none,pass,nolog", errmsg);
    if (rule == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to create rule for op \"%s\": %s", name, *errmsg);
        return -1;
    }

    /* Create a fake variable */
    var = (msre_var *)apr_pcalloc(g_mp, sizeof(msre_var));
    var->name = "UNIT_TEST";
    var->value = apr_pstrmemdup(g_mp, (char *)input, input_len);
    var->value_len = input_len;
    var->metadata = msre_resolve_var(modsecurity->msre, var->name);
    if (var->metadata == NULL) {
        *errmsg = apr_psprintf(g_mp, "Failed to resolve variable for op \"%s\": %s", name, var->name);
        return -1;
    }

    /* Initialize the operator parameter */
    if (metadata->param_init != NULL) {
        rc = metadata->param_init(rule, errmsg);
        if (rc < 0) {
            *errmsg = apr_psprintf(g_mp, "Failed to init op \"%s\": %s", name, *errmsg);
            return rc;
        }
    }

    /* Execute the operator */
    if (metadata->execute != NULL) {
        rc = metadata->execute(g_msr, rule, var, errmsg);
        if (rc < 0) {
            *errmsg = apr_psprintf(g_mp, "Failed to execute op \"%s\": %s", name, *errmsg);
        }
    }

    return rc;
}


/* Initialization */
static void init_msr() {
    directory_config *dcfg = NULL;
    request_rec *r = NULL;
    r = (request_rec *)apr_pcalloc(g_mp, sizeof(request_rec));

    dcfg = (directory_config *)apr_pcalloc(g_mp, sizeof(directory_config));
    dcfg->is_enabled = 0;
    dcfg->reqbody_access = 0;
    dcfg->reqbody_inmemory_limit = REQUEST_BODY_DEFAULT_INMEMORY_LIMIT;
    dcfg->reqbody_limit = REQUEST_BODY_DEFAULT_LIMIT;
    dcfg->reqbody_no_files_limit = REQUEST_BODY_NO_FILES_DEFAULT_LIMIT;
    dcfg->resbody_access = 0;
    dcfg->of_limit = RESPONSE_BODY_DEFAULT_LIMIT;
    dcfg->of_limit_action = RESPONSE_BODY_LIMIT_ACTION_REJECT;
    dcfg->debuglog_fd = NOT_SET_P;
    dcfg->debuglog_name = "msc-test-debug.log";
    dcfg->debuglog_level = 9;
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
    dcfg->data_dir = NULL;
    dcfg->webappid = "default";
    dcfg->content_injection_enabled = 0;
    dcfg->pdfp_enabled = 0;
    dcfg->pdfp_secret = NULL;
    dcfg->pdfp_timeout = 10;
    dcfg->pdfp_token_name = "PDFPTOKEN";
    dcfg->pdfp_only_get = 1;
    dcfg->pdfp_method = PDF_PROTECT_METHOD_TOKEN_REDIRECTION;
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
    g_msr->tx_vars = apr_table_make(g_mp, 10);
}


/* Main */

int main(int argc, const char * const argv[])
{
    apr_file_t *fd;
    unsigned char buf[BUFLEN];
    apr_size_t nbytes = BUFLEN;
    unsigned char input[BUFLEN];
    const char *type = NULL;
    const char *name = NULL;
    unsigned char *param = NULL;
    const char *returnval = NULL;
    char *errmsg = NULL;
    unsigned char *out = NULL;
    apr_size_t input_len = 0;
    apr_size_t param_len = 0;
    apr_size_t out_len = 0;
    int rc = 0;
    int result = 0;

    apr_app_initialize(&argc, &argv, NULL);
    atexit(apr_terminate);

    if (argc < 4) {
        fprintf(stderr, "Usage: %s <type> <name> <param> [<returnval>]\n", argv[0]);
        exit(1);
    }

    apr_pool_create(&g_mp, NULL);
    modsecurity = modsecurity_create(g_mp, MODSEC_OFFLINE);

    type = argv[1];
    name = argv[2];
    param_len = strlen(argv[3]);
    param = apr_pmemdup(g_mp, argv[3], param_len + 1);
    unescape_inplace(param, &param_len);
    if (argc >= 5) {
        returnval = argv[4];
    }

    if (apr_file_open_stdin(&fd, g_mp) != APR_SUCCESS) {
        fprintf(stderr, "Failed to open stdin\n");
        exit(1);
    }

    memset(buf, 0, BUFLEN);
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

    /* Make a copy as transformations are done in-place */
    memcpy(input, buf, BUFLEN);
    input_len = nbytes;

    if (strcmp("tfn", type) == 0) {
        /* Transformations */
        int ret = returnval ? atoi(returnval) : -8888;
        rc = test_tfn(name, input, input_len, &out, &out_len, &errmsg);
        if (rc < 0) {
            fprintf(stderr, "ERROR: %s\n", errmsg);
            result = RESULT_ERROR;
        }
        else if ((ret != -8888) && (rc != ret)) {
            fprintf(stderr, "Returned %d (expected %d)\n", rc, ret);
            result = RESULT_WRONGRET;
        }
        else if (param_len != out_len) {
            fprintf(stderr, "Lenth %" APR_SIZE_T_FMT " (expected %" APR_SIZE_T_FMT ")\n", out_len, param_len);
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
            exit(1);
        }
    }
    else if (strcmp("op", type) == 0) {
        /* Operators */
        int ret = 0;

        if (!returnval) {
            fprintf(stderr, "Return value required for type \"%s\"\n", type);
            exit(1);
        }
        ret = atoi(returnval);

        init_msr();

        rc = test_op(name, (const char *)param, (const unsigned char *)input, input_len, &errmsg);
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
            exit(1);
        }
    }
    else {
        fprintf(stderr, "Unknown type: \"%s\"\n", type);
        exit(1);
    }

    fprintf(stdout, "%s\n", errmsg ? errmsg : "");

    return 0;
}


