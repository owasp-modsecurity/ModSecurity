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
#include <apr.h>

#include "modsecurity.h"
#include "re.h"

#define ISHEX(X) (((X >= '0')&&(X <= '9')) || ((X >= 'a')&&(X <= 'f')) || ((X >= 'A')&&(X <= 'F')))

#define TFNS_DIR "t/tfns"

#define BUFLEN 8192

#define RESULT_SUCCESS           0
#define RESULT_ERROR            -1
#define RESULT_MISMATCHED       -2
#define RESULT_WRONGSIZE        -3
#define RESULT_WRONGRET         -4

/* Globals */
apr_pool_t *mp = NULL;
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
    char *new = apr_pcalloc(mp, (*len * 4) + 1);
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

static int test_tfn(const char *name, unsigned char *input, long input_len, unsigned char **rval, long *rval_len, char **errmsg) 
{
    int rc = -1;
    msre_tfn_metadata *metadata = NULL;
    const apr_array_header_t *arr = apr_table_elts(modsecurity->msre->tfns);
    
    metadata = (msre_tfn_metadata *)apr_table_get(modsecurity->msre->tfns, name);

    *errmsg = NULL;

    if (metadata == NULL) {
        *errmsg = apr_psprintf(mp, "Failed to fetch TFN \"%s\".", name);
        return -1;
    }

    rc = metadata->execute(mp, input, input_len, (char **)rval, rval_len);

    if (rc < 0) {
        *errmsg = apr_psprintf(mp, "Failed to execute TFN \"%s\".", name);
    }

    return rc;
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
    unsigned char *expected = NULL;
    const char *returnval = NULL;
    char *p = NULL;
    apr_size_t input_len = 0;
    apr_size_t expected_len = 0;
    char *errmsg = NULL;
    unsigned char *out = NULL;
    long out_len = 0;
    int rc = 0;
    int result = 0;

    apr_app_initialize(&argc, &argv, NULL);
    atexit(apr_terminate);

    apr_pool_create(&mp, NULL);

    if (argc < 4) {
        fprintf(stderr, "Usage: %s <type> <name> <expected> [<returnval>]\n", argv[0]);
        exit(1);
    }

    modsecurity = modsecurity_create(mp, MODSEC_OFFLINE);

    type = argv[1];
    name = argv[2];
    expected_len = strlen(argv[3]);
    expected = apr_pmemdup(mp, argv[3], expected_len);
    unescape_inplace(expected, &expected_len);
    if (argc >= 5) {
        returnval = argv[4];
    }

    if (apr_file_open_stdin(&fd, mp) != APR_SUCCESS) {
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

    if (strcmp("tfns", type) == 0) {
        /* TFNS */
        int ret = returnval ? atoi(returnval) : -8888;
        rc = test_tfn(name, input, input_len, &out, &out_len, &errmsg);
        if ((ret != -8888) && (rc != ret)) {
            fprintf(stderr, "Returned %d (expected %d)\n", rc, ret);
            result = RESULT_WRONGRET;
        }
        else if (expected_len != out_len) {
            fprintf(stderr, "Lenth %" APR_SIZE_T_FMT " (expected %" APR_SIZE_T_FMT ")\n", out_len, expected_len);
            result = RESULT_WRONGSIZE;
        }
        else {
            result = memcmp(expected, out, expected_len) ? RESULT_MISMATCHED : RESULT_SUCCESS;
        }
    }
    else if (strcmp("operators", type) == 0) {
        /* OPERATORS */
        fprintf(stderr, "Type not implemented yet: \"%s\"\n", type);
        exit(1);
    }
    else {
        fprintf(stderr, "Unknown type: \"%s\"\n", type);
        exit(1);
    }

    if (result != RESULT_SUCCESS) {
        apr_size_t s0len = nbytes;
        const char *s0 = escape(buf, &s0len);
        apr_size_t s1len = out_len;
        const char *s1 = escape(out, &s1len);
        apr_size_t s2len = expected_len;
        const char *s2 = escape(expected, &s2len);

        fprintf(stderr, " Input: '%s' len=%" APR_SIZE_T_FMT "\n"
                        "Output: '%s' len=%" APR_SIZE_T_FMT "\n"
                        "Expect: '%s' len=%" APR_SIZE_T_FMT "\n",
                        s0, nbytes, s1, out_len, s2, expected_len);
        exit(1);
    }

    return 0;
}


