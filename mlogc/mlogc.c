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
#include <apr_errno.h>
#include <apr_general.h>
#include <apr_file_io.h>
#include <apr_file_info.h>
#include <apr_hash.h>
#include <apr_lib.h>
#include <apr_strings.h>
#include <apr_signal.h>
#include <apr_thread_proc.h>
#include <apr_global_mutex.h>
#include <apr_getopt.h>
#include <apr_version.h>
#if APR_HAVE_UNISTD_H
#include <unistd.h>         /* for getpid() */
#endif
#include <pcre.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "msc_release.h"

static void logc_shutdown(int rc);
static void create_new_worker(int lock);
static void error_log(int level, void *thread,
                      const char *text, ...) PRINTF_ATTRIBUTE(3,4);


/* -- Constants -- */

/* Error log levels. */
#define LOG_ERROR           1
#define LOG_WARNING         2
#define LOG_NOTICE          3
#define LOG_DEBUG           4
#define LOG_DEBUG2          5

/* The management thread will wake up every five seconds. */
#define MANAGER_SLEEP       5000000
#define MANAGER_SUBSLEEP    10000

/* Hack to allow multiple mlogc with single delete */
#define KEEP_ENTRIES_REMOVE_HACK   2600
#define KEEP_ENTRIES_REMOVE_TIME   0l
#ifdef TEST_HACK
#define TEST_WITH_RAND_SLEEP(n) \
do { \
    int sec = rand()/(RAND_MAX/n); \
    error_log(LOG_DEBUG2, NULL, "TEST_HACK: Sleeping for %ds", sec); \
    apr_sleep(apr_time_from_sec(sec)); \
} while(0)
#else
#define TEST_WITH_RAND_SLEEP(n)
#endif

#define CAPTUREVECTORSIZE   60
#define PIPE_BUF_SIZE       65536
#define MEMALLOC_ERROR_MSG  "Memory allocation failed!"
#define VERSION             MODSEC_VERSION

#define CMDLINE_OPTS        "fvh"

#define TXIN            0
#define TXOUT           1

#define STATUSBUF_SIZE      256

#define ISHEXCHAR(X) (   ((X >= '0')&&(X <= '9')) \
                      || ((X >= 'a')&&(X <= 'f')) \
                      || ((X >= 'A')&&(X <= 'F')) )

/* -- Regex Patterns -- */

/**
 * This regular expression is used to parse the entire
 * log line we receive from Apache. The REQUEST_LINE is
 * treated as a single parameter to allow for invalid
 * requests.
 */
static const char logline_pattern[] =
    "^(\\S+)"
    "\\ (\\S+)\\ (\\S+)\\ (\\S+)"
    "\\ \\[([^:]+):(\\d+:\\d+:\\d+)\\ ([^\\]]+)\\]"
    "\\ \"(.*)\""
    "\\ (\\d+)\\ (\\S+)"
    "\\ \"(.*)\"\\ \"(.*)\""
    "\\ (\\S+)\\ \"(.*)\""
    "\\ /?(\\S+)\\ (\\d+)\\ (\\d+)"
    "\\ (\\S+)"
    "(.*)$";


/**
 * This regular expression can be used to parse
 * a REQUEST_LINE field into method, URI, and
 * protocol.
 */
static const char requestline_pattern[] =
    "(\\S+)\\ (.*?)\\ (\\S+)";


/* -- Structures -- */

typedef struct {
    unsigned long int        id;
    const char              *line;
    apr_size_t               line_size;
} entry_t;


/* -- Global variables -- */

static pid_t                  logc_pid = 0;
static const char            *conffile = NULL;
static const char            *lockfile = NULL;
static int                    have_read_data = 0;
static int                    checkpoint_interval = 60;
static apr_time_t             checkpoint_time_last = 0;
static const char            *collector_root = NULL;
static apr_table_t           *conf = NULL;
static const char            *console_uri = NULL;
static apr_array_header_t    *curl_handles = NULL;
static int                    current_workers = 0;
static int                    management_thread_active = 0;
static unsigned long int      entry_counter = 1;
static const char            *error_log_path = NULL;
static apr_file_t            *error_log_fd = NULL;
static int                    error_log_level = 2;
static apr_hash_t            *in_progress = NULL;
static int                    keep_alive = 150;           /* Not used yet. */
static int                    keep_alive_timeout = 300;   /* Not used yet. */
static int                    keep_entries = 0;
static const char            *log_repository = NULL;
static void                  *logline_regex = NULL;
static int                    max_connections = 10;
static int                    max_worker_requests = 1000;
static apr_global_mutex_t    *gmutex = NULL;
static apr_thread_mutex_t    *mutex = NULL;
static apr_pool_t            *pool = NULL;
static apr_pool_t            *thread_pool = NULL;
static apr_pool_t            *recv_pool = NULL;
static apr_array_header_t    *queue = NULL;
static const char            *queue_path = NULL;
/* static apr_time_t             queue_time = 0; */
static void                  *requestline_regex = NULL;
static int                    running = 0;
static const char            *sensor_password = NULL;
static const char            *sensor_username = NULL;
static int                    server_error = 0;
static apr_time_t             server_error_last_check_time = 0;
static int                    server_error_timeout = 60;
static int                    startup_delay = 100;
static int                    transaction_delay = 100;
static const char            *transaction_log_path = NULL;
static apr_file_t            *transaction_log_fd = NULL;


/* -- Commandline opts -- */
static int                    opt_force = 0;

/* -- Code -- */

static char *_log_escape(apr_pool_t *mp, const char *input,
                         apr_size_t input_len)
{
    static const char c2x_table[] = "0123456789abcdef";
    unsigned char *d = NULL;
    char *ret = NULL;
    unsigned long int i;

    if (input == NULL) return NULL;

    ret = apr_palloc(mp, input_len * 4 + 1);
    if (ret == NULL) return NULL;
    d = (unsigned char *)ret;

    i = 0;
    while(i < input_len) {
        switch(input[i]) {
            case '"' :
                *d++ = '\\';
                *d++ = '"';
                break;
            case '\b' :
                *d++ = '\\';
                *d++ = 'b';
                break;
            case '\n' :
                *d++ = '\\';
                *d++ = 'n';
                break;
            case '\r' :
                *d++ = '\\';
                *d++ = 'r';
                break;
            case '\t' :
                *d++ = '\\';
                *d++ = 't';
                break;
            case '\v' :
                *d++ = '\\';
                *d++ = 'v';
                break;
            case '\\' :
                *d++ = '\\';
                *d++ = '\\';
                break;
            default :
                if ((input[i] <= 0x1f)||(input[i] >= 0x7f)) {
                    *d++ = '\\';
                    *d++ = 'x';
                    *d++ = c2x_table[input[i] >> 4];
                    *d++ = c2x_table[input[i] & 0x0f];
                } else {
                    *d++ = input[i];
                }
                break;
        }

        i++;
    }

    *d = 0;

    return ret;
}

/**
 * Converts a byte given as its hexadecimal representation
 * into a proper byte. Handles uppercase and lowercase letters
 * but does not check for overflows.
 */
static unsigned char x2c(unsigned char *what) {
    register unsigned char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A') + 10 : (what[1] - '0'));

    return digit;
}

/**
 * URL Decodes a string in-place
 */
static int urldecode_inplace(unsigned char *input, apr_size_t input_len) {
    unsigned char *d = (unsigned char *)input;
    apr_size_t i;

    if (input == NULL) return 0;

    i = 0;
    while (i < input_len) {
        if (input[i] == '%') {
            /* Character is a percent sign. */

            /* Are there enough bytes available? */
            if (i + 2 < input_len) {
                char c1 = input[i + 1];
                char c2 = input[i + 2];

                if (ISHEXCHAR(c1) && ISHEXCHAR(c2)) {
                    /* Valid encoding - decode it. */
                    *d++ = x2c(&input[i + 1]);
                    i += 3;
                } else {
                    /* Not a valid encoding, skip this % */
                    *d++ = input[i++];
                }
            } else {
                /* Not enough bytes available, copy the raw bytes. */
                *d++ = input[i++];
            }
        } else {
            /* Character is not a percent sign. */
            if (input[i] == '+') {
                *d++ = ' ';
            } else {
                *d++ = input[i];
            }
            i++;
        }
    }

    *d = '\0';

    return 1;
}

/**
 * Detect a relative path and merge it with the collector root
 * path. Leave absolute paths as they are.
 */
static const char *file_path(const char *path)
{
    char *newpath = NULL;
    apr_status_t rc;

    if (path == NULL) return NULL;

    rc = apr_filepath_merge(&newpath, collector_root,
                            path, APR_FILEPATH_TRUENAME, pool);
    if ((newpath != NULL) && (rc == APR_SUCCESS || APR_STATUS_IS_EPATHWILD(rc)
        || APR_STATUS_IS_ENOENT(rc) || APR_STATUS_IS_ENOTDIR(rc)))
    {
        return newpath;
    }
    else {
        return NULL;
    }
}


/**
 * Returns the current datetime as a string.
 */
static char *current_logtime(char *dest, int dlen)
{
    apr_time_exp_t t;
    apr_size_t len;

    apr_time_exp_lt(&t, apr_time_now());
    apr_strftime(dest, &len, dlen, "%a %b %d %H:%M:%S %Y", &t);

    return dest;
}


/**
 * Logs error to the error log (if available) or
 * to the stderr.
 */
static void error_log(int level, void *thread, const char *text, ...)
{
    char msg1[4096] = "";
    char msg2[4096] = "";
    char datetime[100];
    va_list ap;

    if (level > error_log_level) return;

    va_start(ap, text);

    apr_vsnprintf(msg1, sizeof(msg1), text, ap);
    apr_snprintf(msg2, sizeof(msg2), "[%s] [%d] [%" APR_PID_T_FMT "/%pp] %s\n",
                 current_logtime(datetime, sizeof(datetime)),
                 level, logc_pid, (thread ? thread : 0), msg1);

    if (error_log_fd != NULL) {
        apr_size_t nbytes_written;
        apr_size_t nbytes = strlen(msg2);
        apr_file_write_full(error_log_fd, msg2, nbytes, &nbytes_written);
    }
    else {
        fprintf(stderr, "%s", msg2);
    }

    va_end(ap);
}


/**
 * Adds one entry to the internal queue. It will (optionally) start
 * a new thread to handle it.
 */
static void add_entry(const char *data, int start_worker)
{
    entry_t *entry = NULL;

    entry = (entry_t *)malloc(sizeof(entry_t));
    entry->id = 0;
    entry->line = strdup(data);
    entry->line_size = strlen(entry->line);

    error_log(LOG_DEBUG, NULL, "Queue locking thread mutex.");
    if (APR_STATUS_IS_EBUSY(apr_thread_mutex_trylock(mutex))) {
        error_log(LOG_DEBUG, NULL, "Queue waiting on thread mutex.");
        apr_thread_mutex_lock(mutex);
    }

    /* Assign unique ID to this log entry. */
    entry->id = entry_counter++;

    /* Add the new audit log entry to the queue. */
    *(entry_t **)apr_array_push(queue) = entry;

    /* Create a new worker if we can, but not if there is a
     * known problem with the server.
     */
    if (   (start_worker != 0)
        && (current_workers < max_connections)&&(server_error == 0))
    {
        create_new_worker(0);
    }

    error_log(LOG_DEBUG, NULL, "Queue unlocking thread mutex.");
    apr_thread_mutex_unlock(mutex);
}


/**
 * Read the queue entries.
 */
static int read_queue_entries(apr_file_t *fd, apr_time_t *queue_time)
{
    char linebuf[4100];
    int line_count = -1;
    int line_size = 0;
    apr_status_t rc = 0;
    char *p = NULL;

    for(;;) {
        memset(linebuf, 0, 4100);
        rc = apr_file_gets(linebuf, 4096, fd);

        if (rc == APR_EOF) break;
        if (rc != APR_SUCCESS) {
            error_log(LOG_ERROR, NULL, "Error reading from the queue file.");
            logc_shutdown(1);
        }

        if (line_count < 0) {
            /* First line contains the queue time. */
            *queue_time = (apr_time_t)apr_atoi64(linebuf);
            line_count = 0;
            continue;
        }

        p = &linebuf[0];
        line_size = strlen(p);

        /* Remove the \n from the end of the line. */
        while(*p != '\0' && line_size > 0) {
            if (*p == '\n') {
                *p = '\0';
                break;
            }
            p++;
            line_size--;
        }

        if (linebuf[0] == '#') { /* Ignore comments. */
            continue;
        }

        add_entry((const char *)&linebuf, 0);

        line_count++;
    }

    apr_file_close(fd);

    return line_count;
}


/**
 * Initialise the transaction log. This code should be
 * executed only once at startup.
 */
static void transaction_log_init(void)
{
    /* ENH: These big enough? */
    char new_queue_path[256];
    char old_queue_path[256];
    apr_file_t *queue_fd = NULL;
    apr_time_t queue_time;

    apr_snprintf(new_queue_path, sizeof(new_queue_path), "%s.new", queue_path);
    apr_snprintf(old_queue_path, sizeof(old_queue_path), "%s.old", queue_path);

    /* Put a lock in place to ensure exclusivity. */
    error_log(LOG_DEBUG, NULL,
              "Transaction initialization locking global mutex.");
    if (APR_STATUS_IS_EBUSY(apr_global_mutex_trylock(gmutex))) {
        error_log(LOG_DEBUG, NULL,
                  "Transaction initialization waiting on global mutex.");
        apr_global_mutex_lock(gmutex);
    }

    error_log(LOG_DEBUG, NULL, "Transaction initialization started.");

    /* Delete .new file if there is one. */
    apr_file_remove(new_queue_path, pool);

    /* Read in the data from the queue. */
    if (apr_file_open(&queue_fd, queue_path, APR_READ | APR_FILE_NOCLEANUP,
        0, pool) == APR_SUCCESS)
    {
        int line_count = read_queue_entries(queue_fd, &queue_time);

        apr_file_close(queue_fd);

        if (line_count > 0) {
            error_log(LOG_NOTICE, NULL,
                      "Loaded %d entries from the queue file.", line_count);
        }
    }
    /* Try the old queue file. */
    else if (apr_file_open(&queue_fd, old_queue_path,
                           APR_READ | APR_FILE_NOCLEANUP,
                           0, pool) == APR_SUCCESS)
    {
        int line_count = read_queue_entries(queue_fd, &queue_time);
        apr_file_close(queue_fd);
        error_log(LOG_NOTICE, NULL,
                  "Loaded %d entries from the OLD queue file.", line_count);
        apr_file_rename(old_queue_path, queue_path, pool);
    }
    else {
        error_log(LOG_NOTICE, NULL,
                  "Queue file not found. New one will be created.");
    }

    /* Delete the old queue file. */
    apr_file_remove(old_queue_path, pool);

    checkpoint_time_last = apr_time_now();

    /* Start fresh with the transaction log. Do note that
     * we do not truncate the transaction log on purpose. Apache
     * will start copies of piped logging binaries during configuration
     * testing. Truncating would erase the log of a currently running
     * instance.
     */
    if (apr_file_open(&transaction_log_fd, transaction_log_path,
                      APR_WRITE | APR_CREATE | APR_APPEND | APR_XTHREAD,
                      APR_OS_DEFAULT, pool) != APR_SUCCESS)
    {
        error_log(LOG_ERROR, NULL,
                  "Failed to open the transaction log: %s\n",
                  transaction_log_path);
        error_log(LOG_DEBUG, NULL,
                  "Transaction initialization unlocking global mutex.");
        apr_global_mutex_unlock(gmutex);
        logc_shutdown(1);
    }

    error_log(LOG_DEBUG, NULL, "Transaction initialization completed.");

    /* Unlock */
    error_log(LOG_DEBUG, NULL,
              "Transaction initialization unlocking global mutex.");
    apr_global_mutex_unlock(gmutex);
}


/**
 * Log entry event (incoming or outgoing) to the transaction log.
 */
static void transaction_log(int direction, const char *entry)
{
    apr_size_t nbytes, nbytes_written;
    char msg[8196] = "";

    apr_snprintf(msg, sizeof(msg), "%u %s: %s\n",
                 (unsigned int)apr_time_sec(apr_time_now()),
        (direction == TXIN ? "IN" : "OUT"), entry);
    nbytes = strlen(msg);
    apr_file_write_full(transaction_log_fd, msg, nbytes, &nbytes_written);
}


/**
 * Executes a checkpoint, which causes the current queue to be
 * written to a file and the transaction log to be truncated.
 */
static void transaction_checkpoint(void)
{
    /* ENH: These big enough? */
    char new_queue_path[256];
    char old_queue_path[256];
    apr_file_t *queue_fd = NULL;
    apr_hash_index_t *hi = NULL;
    char msg[256];
    int i;
    apr_pool_t *cpool;

    apr_snprintf(new_queue_path, sizeof(new_queue_path), "%s.new", queue_path);
    apr_snprintf(old_queue_path, sizeof(old_queue_path), "%s.old", queue_path);
    apr_snprintf(msg, sizeof(msg), "%u\n",
                 (unsigned int)apr_time_sec(apr_time_now()));

    if (! have_read_data) {
        error_log(LOG_DEBUG, NULL, "Checkpoint not required.");
        return;
    }

    /* Put a lock in place to ensure exclusivity. */
    error_log(LOG_DEBUG, NULL, "Checkpoint locking global mutex.");
    if (APR_STATUS_IS_EBUSY(apr_global_mutex_trylock(gmutex))) {
        error_log(LOG_DEBUG, NULL, "Checkpoint waiting on global mutex.");
        apr_global_mutex_lock(gmutex);
    }

    error_log(LOG_DEBUG, NULL, "Checkpoint started.");

    apr_pool_create(&cpool, NULL);

    /* Dump active entries into a new queue file. */
    if (apr_file_open(&queue_fd, new_queue_path,
                      APR_WRITE | APR_CREATE | APR_EXCL |
                      APR_TRUNCATE | APR_FILE_NOCLEANUP,
                      APR_OS_DEFAULT, cpool) != APR_SUCCESS)
    {
        error_log(LOG_ERROR, NULL, "Failed to create file: %s", new_queue_path);
        error_log(LOG_DEBUG, NULL, "Checkpoint unlocking global mutex.");
        apr_pool_destroy(cpool);
        apr_global_mutex_unlock(gmutex);
        return;
    }

    /* Write the time first. */
    apr_file_write_full(queue_fd, msg, strlen(msg), NULL);

    /* Dump the entries sitting in the queue first. */
    for (i = 0; i < queue->nelts; i++) {
        entry_t *entry = ((entry_t **)queue->elts)[i];
        apr_file_write_full(queue_fd, entry->line, entry->line_size, NULL);
        apr_file_write_full(queue_fd, &"\n", 1, NULL);
    }
    error_log(LOG_DEBUG2, NULL,
              "Checkpoint wrote %d queued entries to new queue.", i);

    /* Then dump the ones that are currently being processed. */
    i = 0;
    for (hi = apr_hash_first(NULL, in_progress);
         hi != NULL; hi = apr_hash_next(hi))\
    {
        void *e;
        entry_t *entry = NULL;

        i++;
        apr_hash_this(hi, NULL, NULL, &e);
        entry = e; /* quiet type-punned warning */
        apr_file_write_full(queue_fd, entry->line, entry->line_size, NULL);
        apr_file_write_full(queue_fd, &"\n", 1, NULL);
    }
    error_log(LOG_DEBUG2, NULL,
              "Checkpoint wrote %d additional entries to new queue.", i);

    apr_file_close(queue_fd);

    /* Switch the files and truncate the transaction log file. */
    apr_file_remove(old_queue_path, cpool);
    apr_file_rename(queue_path, old_queue_path, cpool);
    apr_file_rename(new_queue_path, queue_path, cpool);
    apr_file_remove(old_queue_path, cpool);
    apr_file_trunc(transaction_log_fd, 0);

    error_log(LOG_DEBUG, NULL, "Checkpoint completed.");

    apr_pool_destroy(cpool);

    /* Unlock and exit. */
    error_log(LOG_DEBUG, NULL, "Checkpoint unlocking global mutex.");
    apr_global_mutex_unlock(gmutex);
}


/**
 * Parse one confguration line and add it to the
 * configuration table.
 */
static void parse_configuration_line(const char *line, int line_count)
{
    char *start = NULL, *command = NULL;
    char *p = NULL;

    /* Remove the trailing newline character. */
    p = (char *)line;
    while(*p != '\0') p++;
    if ((p > start)&&(*(p - 1) == '\n')) *(p - 1) = '\0';

    p = (char *)line;
    /* Ignore whitespace at the beginning of the line. */
    while(apr_isspace(*p)) p++;

    /* Ignore empty lines and comments. */
    if ((*p == '\0')||(*p == '#')) return;

    start = p;
    while(!apr_isspace(*p)&&(*p != '\0')) p++;

    command = apr_pstrmemdup(pool, start, p - start);

    while(apr_isspace(*p)) p++;

    /* Remove whitespace at the end. */
    start = p;
    while(*p != '\0') p++;
    if (p > start) {
        p--;
        while(apr_isspace(*p)) {
            *p-- = '\0';
        }
    }

    /* Remove quotes, but only if we have matching */
    if ((*start == '"') && (p > start) && (*p == '"')) {
        start++;
        *p-- = '\0';
    }

    /* Take the last directive */
    /* ENH: Error on dup directives? */
    apr_table_set(conf, command, start);
}


/**
 * Reads configuration from a file.
 */
static void read_configuration(void)
{
    char linebuf[4096];
    apr_status_t rc;
    apr_file_t *fd;
    int line_count;

    conf = apr_table_make(pool, 32);
    if (conf == NULL) {
        error_log(LOG_ERROR, NULL, MEMALLOC_ERROR_MSG);
        logc_shutdown(1);
    }

    rc = apr_file_open(&fd, conffile, APR_READ | APR_FILE_NOCLEANUP, 0, pool);
    if (rc != APR_SUCCESS) {
        error_log(LOG_ERROR, NULL,
                  "Unable to open configuration file: %s", conffile);
        logc_shutdown(1);
    }

    line_count = 0;
    for(;;) {
        rc = apr_file_gets(linebuf, 4096, fd);
        if (rc == APR_EOF) return;
        if (rc != APR_SUCCESS) {
            error_log(LOG_ERROR, NULL,
                      "Error reading from the configuration file.");
            logc_shutdown(1);
        }

        line_count++;
        parse_configuration_line(linebuf, line_count);
    }

    apr_file_close(fd);
}


/**
 * Initialize the configuration.
 */
static void init_configuration(void)
{
    char errstr[1024];
    apr_status_t rc = 0;
    const char *s = NULL;

    /* Other values may be based off the collector root. */
    s = apr_table_get(conf, "CollectorRoot");
    if (s != NULL) {
        collector_root = s;
    }

    /* Error Log */
    s = apr_table_get(conf, "ErrorLog");
    if (s != NULL) {
        error_log_path = file_path(s);
    }

    s = apr_table_get(conf, "ErrorLogLevel");
    if (s != NULL) {
        error_log_level = atoi(s);
    }

    if ((rc = apr_file_open(&error_log_fd, error_log_path,
                            APR_WRITE | APR_CREATE | APR_APPEND,
                            APR_OS_DEFAULT, pool)) != APR_SUCCESS)
    {
        error_log(LOG_ERROR, NULL, "Failed to open the error log %s: %s\n",
            error_log_path, apr_strerror(rc, errstr, 1024));
        logc_shutdown(1);
    }

    error_log(LOG_NOTICE, NULL,
              "Configuring ModSecurity Audit Log Collector %s.", VERSION);

    /* Startup Delay */
    s = apr_table_get(conf, "StartupDelay");
    if (s != NULL) {
        startup_delay = atoi(s);
    }

    if ( startup_delay > 0 ) {
        error_log(LOG_NOTICE, NULL,
                  "Delaying execution for %dms.", startup_delay);
        apr_sleep(startup_delay * 1000);
        error_log(LOG_DEBUG, NULL,
                  "Continuing execution after %dms delay.", startup_delay);
    }

    /* Remaining Configuration */

    error_log(LOG_DEBUG2, NULL, "CollectorRoot=%s", collector_root);
    error_log(LOG_DEBUG2, NULL, "ErrorLog=%s", error_log_path);
    error_log(LOG_DEBUG2, NULL, "ErrorLogLevel=%d", error_log_level);
    error_log(LOG_DEBUG2, NULL, "StartupDelay=%d", startup_delay);

    s = apr_table_get(conf, "CheckpointInterval");
    if (s != NULL) {
        checkpoint_interval = atoi(s);
        error_log(LOG_DEBUG2, NULL,
                  "CheckpointInterval=%d", checkpoint_interval);
    }

    s = apr_table_get(conf, "QueuePath");
    if (s != NULL) {
        queue_path = file_path(s);
        error_log(LOG_DEBUG2, NULL, "QueuePath=%s", queue_path);
    }
    else {
        error_log(LOG_ERROR, NULL,
                  "QueuePath not defined in the configuration file.");
        logc_shutdown(1);
    }

    s = apr_table_get(conf, "LockFile");
    if (s != NULL) {
        lockfile = file_path(s);
        error_log(LOG_DEBUG2, NULL, "LockFile=%s", lockfile);
    }

    s = apr_table_get(conf, "ServerErrorTimeout");
    if (s != NULL) {
        server_error_timeout = atoi(s);
        error_log(LOG_DEBUG2, NULL,
                  "ServerErrorTimeout=%d", server_error_timeout);
    }

    s = apr_table_get(conf, "TransactionDelay");
    if (s != NULL) {
        transaction_delay = atoi(s);
        error_log(LOG_DEBUG2, NULL, "TransactionDelay=%d", transaction_delay);
    }

    s = apr_table_get(conf, "TransactionLog");
    if (s != NULL) {
        transaction_log_path = file_path(s);
        error_log(LOG_DEBUG2, NULL, "TransactionLog=%s", transaction_log_path);
    }

    s = apr_table_get(conf, "MaxConnections");
    if (s != NULL) {
        int v = atoi(s);
        if (v >= 0) max_connections = v;
        error_log(LOG_DEBUG2, NULL, "MaxConnections=%d", max_connections);
    }

    s = apr_table_get(conf, "MaxWorkerRequests");
    if (s != NULL) {
        int v = atoi(s);
        if (v >= 0) max_worker_requests = v;
        error_log(LOG_DEBUG2, NULL,
                  "MaxWorkerRequests=%d", max_worker_requests);
    }

    s = apr_table_get(conf, "KeepAlive");
    if (s != NULL) {
        int v = atoi(s);
        if (v >= 0) keep_alive = v;
        error_log(LOG_DEBUG2, NULL, "KeepAlive=%d", keep_alive);
    }

    s = apr_table_get(conf, "KeepAliveTimeout");
    if (s != NULL) {
        int v = atoi(s);
        if (v >= 0) keep_alive_timeout = v;
        error_log(LOG_DEBUG2, NULL, "KeepAliveTimeout=%d", keep_alive_timeout);
    }

    s = apr_table_get(conf, "LogStorageDir");
    if (s != NULL) {
        log_repository = file_path(s);
        error_log(LOG_DEBUG2, NULL, "LogStorageDir=%s", log_repository);
    }
    else {
        error_log(LOG_ERROR, NULL,
                  "Missing mandatory parameter LogStorageDir.\n");
        logc_shutdown(1);
    }

    s = apr_table_get(conf, "ConsoleURI");
    if (s != NULL) {
        console_uri = s;
        error_log(LOG_DEBUG2, NULL, "ConsoleURI=%s", console_uri);
    }
    else {
        error_log(LOG_ERROR, NULL, "Missing mandatory parameter ConsoleURI.\n");
        logc_shutdown(1);
    }

    s = apr_table_get(conf, "SensorUsername");
    if (s != NULL) {
        sensor_username = s;
        error_log(LOG_DEBUG2, NULL, "SensorUsername=%s", sensor_username);
    }
    else {
        error_log(LOG_ERROR, NULL,
                  "Missing mandatory parameter SensorUsername.\n");
        logc_shutdown(1);
    }

    s = apr_table_get(conf, "SensorPassword");
    if (s != NULL) {
        sensor_password = s;
        error_log(LOG_DEBUG2, NULL, "SensorPassword=%s", sensor_password);
    }
    else {
        error_log(LOG_ERROR, NULL,
                  "Missing mandatory parameter SensorPassword.\n");
        logc_shutdown(1);
    }

    s = apr_table_get(conf, "KeepEntries");
    if (s != NULL) {
        keep_entries = atoi(s);
    }
    else {
        keep_entries = 0;
    }
    error_log(LOG_DEBUG2, NULL, "KeepEntries=%d", keep_entries);
}


/**
 * Clean-up resources before process shutdown.
 */
static void logc_cleanup(void)
{
    curl_global_cleanup();
}


/**
 * Shutdown the logger.
 */
static void logc_shutdown(int rc)
{
    /* Tell the threads to shut down. */
    running = 0;

    error_log(LOG_DEBUG, NULL, "Shutting down");

    /* Wait for the management thread to stop */
    /* ENH: Need a fixed timeout if this never happens */
    while(management_thread_active != 0) {
        apr_sleep(10 * 1000);
    }

    if (rc == 0) {
        error_log(LOG_NOTICE, NULL,
                  "ModSecurity Audit Log Collector %s terminating normally.",
                  VERSION);
    }
    else {
        error_log(LOG_NOTICE, NULL,
                  "ModSecurity Audit Log Collector %s "
                  "terminating with error %d", VERSION, rc);
    }

    if (error_log_fd != NULL) {
        apr_file_flush(error_log_fd);
    }

    exit(rc);
}


/**
 * Handle signals.
 */
static int handle_signals(int signum)
{
    switch (signum) {
        case SIGINT:
            error_log(LOG_NOTICE, NULL, "Caught SIGINT, shutting down.");
            logc_shutdown(0);
        case SIGTERM:
            error_log(LOG_NOTICE, NULL, "Caught SIGTERM, shutting down.");
            logc_shutdown(0);
#ifndef WIN32
        case SIGHUP:
            error_log(LOG_NOTICE, NULL, "Caught SIGHUP, ignored.");
            /* ENH: reload config? */
            return 0;
        case SIGALRM:
            error_log(LOG_DEBUG, NULL, "Caught SIGALRM, ignored.");
            return 0;
        case SIGTSTP:
            error_log(LOG_DEBUG, NULL, "Caught SIGTSTP, ignored.");
            return 0;
#endif /* WIN32 */
    }
#ifndef WIN32
    error_log(LOG_NOTICE, NULL,
              "Caught unexpected signal %d: %s",
              signum, apr_signal_description_get(signum));
#else
    error_log(LOG_NOTICE, NULL, "Caught unexpected signal %d", signum);
#endif /* WIN32 */
    logc_shutdown(1);

    return 0; /* should never reach */
}

#ifdef WIN32
/**
 * This function is invoked by Curl to read the source file on Windows
 */
static size_t curl_readfunction(void *ptr, size_t size,
                                 size_t nmemb, void *stream)
{
        return fread(ptr, size, nmemb, (FILE *)stream);
}
#endif

/**
 * This function is invoked by Curl to read the response
 * body. Since we don't care about the response body the function
 * pretends it is retrieving data where it isn't.
 */
static size_t curl_writefunction(void *ptr, size_t size,
                                 size_t nmemb, void *stream)
{
    unsigned char *data = (unsigned char *)ptr;
    unsigned char *status = (unsigned char *)stream;

    /* Grab the status line text from the first line of output */
    if ((status[0] == 0) && (status[1] == 1)) {
        apr_size_t i, j;
        int ismsg = 0;

        status[1] = 0; /* reset hidden init flag */

        for (i = 0, j = 0; i < STATUSBUF_SIZE; i++) {
            /* We found a line ending so we are done */
            if ( data[i] == '\r' ) {
                break;
            }
            /* Skip to after the first space (where msg is) */
            if (ismsg < 3) {
                if ((ismsg == 1) && !isspace(data[i])) {
                    ismsg++;
                }
                else if (isspace(data[i])) {
                    ismsg++;
                }
                continue;
            }

            /* Copy data (msg) from data to status */
            status[j++] = data[i];
        }
        status[j] = '\0';
        urldecode_inplace(status, j);
    }

    /* do nothing */
    return (size * nmemb);
}


/**
 * This function is invoked by Curl whenever it has something
 * to say. We forward its messages to the error log at level
 * DEBUG or DEBUG2 depending on the verbosity.
 */
static int curl_debugfunction(CURL *curl, curl_infotype infotype,
                              char *data, size_t datalen, void *ourdata)
{
    apr_size_t i, effectivelen;
    apr_thread_t *thread = (apr_thread_t *)ourdata;

    if (error_log_level < LOG_DEBUG) return 0;

    effectivelen = datalen;
    for(i = 0; i < datalen; i++) {
        if ((data[i] == 0x0a)||(data[i] == 0x0d)) {
            effectivelen = i;
            break;
        }
    }

    switch (infotype) {
        case CURLINFO_TEXT:
            /* More verbose data starts with an indent */
            if (apr_isspace(data[0])) {
                char *dataptr = data + 1;
                
                /* Skip initial whitespace (indent) */
                while (   ((size_t)(dataptr - data) > datalen)
                       && apr_isspace(*dataptr)) dataptr++;
                dataptr++;
                error_log(LOG_DEBUG2, thread, "CURL: %s",
                          _log_escape(apr_thread_pool_get(thread), dataptr,
                                      effectivelen - (dataptr - data)));
            }
            else {
                error_log(LOG_DEBUG, thread, "CURL: %s",
                          _log_escape(apr_thread_pool_get(thread), data,
                                      effectivelen));
            }
            break;
        case CURLINFO_HEADER_IN:
            error_log(LOG_DEBUG, thread, "CURL: HEADER_IN %s",
                      _log_escape(apr_thread_pool_get(thread), data,
                                  effectivelen));
            break;
        case CURLINFO_HEADER_OUT:
            error_log(LOG_DEBUG, thread, "CURL: HEADER_OUT %s",
                      _log_escape(apr_thread_pool_get(thread), data,
                                  effectivelen));
            break;
        case CURLINFO_DATA_IN:
            error_log(LOG_DEBUG2, thread, "CURL: DATA_IN %s",
                      _log_escape(apr_thread_pool_get(thread), data,
                                  effectivelen));
            break;
        case CURLINFO_DATA_OUT:
            error_log(LOG_DEBUG2, thread, "CURL: DATA_OUT %s",
                      _log_escape(apr_thread_pool_get(thread), data,
                                  effectivelen));
            break;
        default:
            /* Ignore anything else */
            break;
    }

    return 0;
}


/**
 * Initialise the necessary resources and structures.
 */
static void logc_init(void)
{
    char errstr[1024];
    apr_status_t rc = 0;
    const char *errptr = NULL;
    int i, erroffset;

    queue = apr_array_make(pool, 64, sizeof(entry_t *));
    if (queue == NULL) {
        error_log(LOG_ERROR, NULL, MEMALLOC_ERROR_MSG);
        logc_shutdown(1);
    }

    in_progress = apr_hash_make(pool);
    if (in_progress == NULL) {
        error_log(LOG_ERROR, NULL, MEMALLOC_ERROR_MSG);
        logc_shutdown(1);
    }

    if ((rc = apr_global_mutex_create(&gmutex, lockfile,
                                      APR_LOCK_DEFAULT, pool)) != APR_SUCCESS)
    {
        error_log(LOG_ERROR, NULL, "Failed to create global mutex: %s",
            apr_strerror(rc, errstr, 1024));
        logc_shutdown(1);
    }

    if ((rc = apr_thread_mutex_create(&mutex, APR_THREAD_MUTEX_UNNESTED,
                                      pool)) != APR_SUCCESS)
    {
        error_log(LOG_ERROR, NULL, "Failed to create thread mutex: %s",
            apr_strerror(rc, errstr, 1024));
        logc_shutdown(1);
    }

    entry_counter = 1;

    curl_handles = apr_array_make(pool, max_connections, sizeof(CURL *));
    if (curl_handles == NULL) {
        error_log(LOG_ERROR, NULL, MEMALLOC_ERROR_MSG);
        logc_shutdown(1);
    }

    /* Initialise a number of Curl handles. */
    for(i = 0; i < max_connections; i++) {
        CURL *curl = NULL;

        /* Create cURL handle. */
        curl = curl_easy_init();

        /* Pre-configure the handle. */
        curl_easy_setopt(curl, CURLOPT_UPLOAD, TRUE);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, (char *)NULL);
        curl_easy_setopt(curl, CURLOPT_URL, console_uri);
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        /* SSLv3 works better overall as some servers have issues with TLS */
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_SSLv3);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 15);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, TRUE);
        curl_easy_setopt(curl, CURLOPT_HEADER, TRUE);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writefunction);

        *(CURL **)apr_array_push(curl_handles) = curl;
    }

    logline_regex = pcre_compile(logline_pattern, PCRE_CASELESS,
                                 &errptr, &erroffset, NULL);
    if (logline_regex == NULL) {
        error_log(LOG_ERROR, NULL,
                  "Failed to compile pattern: %s\n", logline_pattern);
        logc_shutdown(1);
    }

    requestline_regex = pcre_compile(requestline_pattern,
                                     PCRE_CASELESS, &errptr, &erroffset, NULL);
    if (requestline_regex == NULL) {
        error_log(LOG_ERROR, NULL,
                  "Failed to compile pattern: %s\n", requestline_pattern);
        logc_shutdown(1);
    }
}


/**
 * HACK: To allow two mlogcs running against a single dataset we use the
 * mtime as a flag for deletion.
 *
 *  1) Check file date.
 *  2) If it is KEEP_ENTRIES_REMOVE_TIME, then remove the file.
 *  3) Otherwise set the date and let the other mlogc remove it.
 */
static void keep_entries_hack(apr_pool_t *mp,
                              apr_thread_t *thread, const char *fn)
{
    apr_file_t *f = NULL;
    apr_finfo_t finfo;
    char errstr[1024];
    apr_status_t rc;

    /* Opening for write as required for exclusive lock */
    if ((rc = apr_file_open(&f, fn,
                            APR_READ|APR_WRITE|APR_APPEND,
                            APR_OS_DEFAULT, mp)) != APR_SUCCESS)
    {
        error_log(LOG_ERROR, thread,
                  "Could not open \"%s\": %s",
                  fn, apr_strerror(rc, errstr, 1024));
        return;
    }

    if ((rc = apr_file_lock(f,
                  APR_FLOCK_EXCLUSIVE|APR_FLOCK_NONBLOCK)) != APR_SUCCESS)
    {
        error_log(LOG_DEBUG2, thread,
                  "Waiting for lock on \"%s\": %s",
                  fn, apr_strerror(rc, errstr, 1024));
        if ((rc = apr_file_lock(f, APR_FLOCK_EXCLUSIVE)) != APR_SUCCESS) {
            error_log(LOG_ERROR, thread,
                      "Could not lock \"%s\": %s",
                      fn, apr_strerror(rc, errstr, 1024));
            apr_file_close(f);
            return;
        }
    }
    error_log(LOG_DEBUG2, thread, "Locked: %s", fn);

    /* For testing only */
    TEST_WITH_RAND_SLEEP(2);

    if ((rc = apr_stat(&finfo, fn, APR_FINFO_MIN, mp)) != APR_SUCCESS) {
        error_log(LOG_ERROR, thread,
                  "Could not stat \"%s\": %s",
                  fn, apr_strerror(rc, errstr, 1024));
        error_log(LOG_DEBUG2, thread, "Unlocked: %s", fn);
        apr_file_close(f);
        return;
    }

    if (error_log_level >= LOG_DEBUG) {
        error_log(LOG_DEBUG, thread,
                  "STAT \"%s\" {"
                  "uid=%d; gid=%d; size=%" APR_OFF_T_FMT "; "
                  "csize=%" APR_OFF_T_FMT "; atime=%" APR_TIME_T_FMT "; "
                  "ctime=%" APR_TIME_T_FMT "; mtime=%" APR_TIME_T_FMT
                  "}",
                  fn, finfo.user, finfo.group, finfo.size,
                  finfo.csize, finfo.atime, finfo.ctime, finfo.mtime);
    }

    if (finfo.mtime != KEEP_ENTRIES_REMOVE_TIME) {
        error_log(LOG_DEBUG2, thread, "Set mtime: %s", fn);
        if ((rc = apr_file_mtime_set(fn,
                        (apr_time_t)KEEP_ENTRIES_REMOVE_TIME, mp))
                  != APR_SUCCESS)
        {
            error_log(LOG_ERROR, thread,
                      "Could not set mtime on \"%s\": %s",
                      fn, apr_strerror(rc, errstr, 1024));
        }
        error_log(LOG_DEBUG2, thread, "Unlocked: %s", fn);
        apr_file_close(f);
        return;
    }


    error_log(LOG_DEBUG, thread, "Removing: %s", fn);
    error_log(LOG_DEBUG2, thread, "Unlocked: %s", fn);
    apr_file_close(f);
    apr_file_remove(fn, mp);
}


/**
 * Worker thread. Works in a loop, fetching jobs from the queue,
 * until the queue is empty or it is otherwise told to quit.
 */
static void * APR_THREAD_FUNC thread_worker(apr_thread_t *thread, void *data)
{
    unsigned int loop_count = 0;
    CURL *curl = (CURL *)data;
    entry_t **entryptr = NULL;
    entry_t *entry = NULL;
    apr_status_t rc;
    apr_finfo_t finfo;
    int capturevector[CAPTUREVECTORSIZE];
    int take_new = 1;
    apr_pool_t *tpool;
    struct curl_slist *headerlist = NULL;
    char curl_error_buffer[CURL_ERROR_SIZE] = "";
    int num_requests = 0;

    /* There is no need to do the sleep if this was an invalid entry
     * as the sleep is just to protect flooding the console server
     * with rapid requests.  With an invalid entry we never hit the
     * server, so we should not delay processing the next event.
     */
    int nodelay = 0;


    error_log(LOG_DEBUG, thread, "Worker thread starting.");

    /* Each worker uses its own pool to manage memory. To avoid
     * memory leaks the pool is cleared after each processed
     * entry.
     */
    apr_pool_create(&tpool, thread_pool);

    /* Process jobs in a queue until there are no more jobs to process. */
    for(;;) {
        nodelay = 0;

        /* Do we need to shut down? */
        if (running == 0) {
            error_log(LOG_DEBUG, thread, "We were told to shut down.");
            goto THREAD_SHUTDOWN;
        }

        /* Is there a problem with the server? We need
         * to shut down if there is. Except that we don't
         * want to shut down if we were launched to investigate
         * if the server came back online (loop_count will be
         * zero in that case).
         */
        if ((server_error == 1)&&(loop_count != 0)) {
            error_log(LOG_DEBUG, thread, "Shutting down due to server error.");
            goto THREAD_SHUTDOWN;
        }

        loop_count++;

        /* Get a new entry, but only if we need one. */
        if (take_new) {
            error_log(LOG_DEBUG, thread, "Worker fetch locking thread mutex.");
            if (APR_STATUS_IS_EBUSY(apr_thread_mutex_trylock(mutex))) {
                error_log(LOG_DEBUG, thread,
                          "Worker fetch waiting on thread mutex.");
                apr_thread_mutex_lock(mutex);
            }

            error_log(LOG_DEBUG, thread, "Worker fetch started.");

            /* Deal with the previous entry. */
            if (entry != NULL) {
                error_log(LOG_DEBUG, thread,
                          "Removing previous entry from storage.");
                transaction_log(TXOUT, entry->line);

                /* Remove previous entry from storage. */
                apr_hash_set(in_progress, &entry->id, sizeof(entry->id), NULL);

                /* Release the memory it used to occupy. */
                free((void *)entry->line);
                free(entry);
                entry = NULL;
            }

            error_log(LOG_DEBUG, thread, "Getting one entry from the queue.");

            /* Get one entry. */
            entryptr = (entry_t **)apr_array_pop(queue);
            if (entryptr == NULL) {
                error_log(LOG_DEBUG, thread,
                          "Worker fetch unlocking thread mutex.");
                apr_thread_mutex_unlock(mutex);
                error_log(LOG_DEBUG, thread,
                          "No more work for this thread, exiting.");

                goto THREAD_SHUTDOWN;
            }

            entry = *entryptr;
            apr_hash_set(in_progress, &entry->id, sizeof(entry->id), entry);

            error_log(LOG_DEBUG, thread, "Worker fetch completed.");

            error_log(LOG_DEBUG, thread,
                      "Worker fetch unlocking thread mutex.");
            apr_thread_mutex_unlock(mutex);
        }

        /* Send one entry. */

        error_log(LOG_DEBUG, thread, "Processing entry.");
        take_new = 0;

        /* Keep track of requests processed if we need to */
        if (max_worker_requests > 0) {
            num_requests++;
        }

        rc = pcre_exec(logline_regex, NULL, entry->line, entry->line_size, 0, 0,
            capturevector, CAPTUREVECTORSIZE);
        if (rc == PCRE_ERROR_NOMATCH) { /* No match. */
            error_log(LOG_WARNING, thread,
                      "Invalid entry (failed to match regex): %s",
                      _log_escape(tpool, entry->line, entry->line_size));
            take_new = 1;
            nodelay = 1;
        }
        else if (rc < 0) { /* Error condition. */
            error_log(LOG_WARNING, thread,
                      "Invalid entry (PCRE error %d): %s",
                      rc, _log_escape(tpool, entry->line, entry->line_size));
            take_new = 1;
            nodelay = 1;
        }
        else { /* We have a match. */
            char *uniqueid = NULL;
            char *auditlogentry = NULL;
            char *hash = NULL;
            char *summary = NULL;
            char *credentials = NULL;

            error_log(LOG_DEBUG, thread, "Regular expression matched.");

            /* For testing only */
            TEST_WITH_RAND_SLEEP(2);

            uniqueid = apr_psprintf(tpool, "%.*s",
                (capturevector[2*13+1] - capturevector[2*13]),
                (entry->line + capturevector[2*13]));
            auditlogentry = apr_psprintf(tpool, "%s/%.*s", log_repository,
                (capturevector[2*15+1] - capturevector[2*15]),
                (entry->line + capturevector[2*15]));
            hash = apr_psprintf(tpool, "X-Content-Hash: %.*s",
                (capturevector[2*18+1] - capturevector[2*15]),
                (entry->line + capturevector[2*18]));
            summary = apr_psprintf(tpool, "X-ForensicLog-Summary: %s",
                    entry->line);
            credentials = apr_psprintf(tpool, "%s:%s",
                                       sensor_username, sensor_password);

            rc = apr_stat(&finfo, auditlogentry, APR_FINFO_SIZE, tpool);
            if (rc == APR_SUCCESS) {
                FILE *hd_src;
                char response_buf[STATUSBUF_SIZE];
                CURLcode res;

                if (error_log_level >= LOG_DEBUG) {
                    error_log(LOG_DEBUG, thread,
                              "STAT \"%s\" {"
                              "uid=%d; gid=%d; size=%" APR_OFF_T_FMT "; "
                              "csize=%" APR_OFF_T_FMT "; "
                              "atime=%" APR_TIME_T_FMT "; "
                              "ctime=%" APR_TIME_T_FMT "; "
                              "mtime=%" APR_TIME_T_FMT
                              "}",
                              auditlogentry, finfo.user, finfo.group,
                              finfo.size, finfo.csize, finfo.atime,
                              finfo.ctime, finfo.mtime);
                }

                /* Initialize the respone buffer with a hidden value */
                response_buf[0] = 0;
                response_buf[1] = 1;

                if (finfo.size == 0) {
                    error_log(LOG_WARNING, thread,
                              "File found (%" APR_OFF_T_FMT
                              " bytes), skipping.", finfo.size);
                    take_new = 1;
                    nodelay = 1;
                    goto THREAD_CLEANUP;
                }
                else {
                    error_log(LOG_DEBUG, thread,
                              "File found (%" APR_OFF_T_FMT
                              " bytes), activating cURL.", finfo.size);
                }


                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
                curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION,
                                 curl_debugfunction);
                curl_easy_setopt(curl, CURLOPT_DEBUGDATA, thread);
                curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_error_buffer);
                curl_easy_setopt(curl, CURLOPT_USERPWD, credentials);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, (char *)response_buf);

                headerlist = curl_slist_append(headerlist, "Expect:");
                headerlist = curl_slist_append(headerlist, hash);
                headerlist = curl_slist_append(headerlist, summary);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

                hd_src = fopen(auditlogentry, "rb");
                if (hd_src == NULL) {
                    error_log(LOG_WARNING, thread,
                              "Invalid entry (failed to open file for "
                              "reading): %s", auditlogentry);
                    take_new = 1;
                    nodelay = 1;
                    goto THREAD_CLEANUP;
                }

                curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
                curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, finfo.size);

#ifdef WIN32
                /* Mandatory on win32 */
                curl_easy_setopt(curl, CURLOPT_READFUNCTION, curl_readfunction);
#endif

                res = curl_easy_perform(curl);

                fclose(hd_src);

                if (res == 0) {
                    long response_code = 0;

                    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE,
                                            &response_code);
                    error_log(LOG_DEBUG, thread,
                              "Request returned with status \"%ld %s\": %s",
                              response_code, response_buf, uniqueid);


                    if (response_code == 0) {
                        /* Assume problem with connection */
                        error_log(LOG_WARNING, thread,
                                  "Flagging server as errored after failure "
                                  "to retrieve response code for entry %s "
                                  "(cURL code %d): Possible SSL negotiation "
                                  "error",
                                  uniqueid, res);
                        apr_sleep(1000 * 1000);
                        take_new = 0;
                        server_error = 1;
                        server_error_last_check_time = apr_time_now();
                    }
                    else if (res != 0) {
                        error_log(LOG_WARNING, thread,
                                  "Flagging server as errored after failure "
                                  "to retrieve response code for entry %s "
                                  "(cURL code %d): %s",
                                  uniqueid, res, curl_error_buffer);
                        apr_sleep(1000 * 1000);
                        take_new = 0;
                        server_error = 1;
                        server_error_last_check_time = apr_time_now();
                    }
                    else {
                        if (response_code == 200) {
                            double total_time, upload_size;

                            if (server_error == 1) {
                                error_log(LOG_NOTICE, thread,
                                          "Clearing the server error flag "
                                          "after successful entry "
                                          "submission: %s", uniqueid);
                            }
                            server_error = 0;
                            server_error_last_check_time = 0;

                            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME,
                                              &total_time);
                            curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD,
                                              &upload_size);

                            if (!keep_entries) {
                                error_log(LOG_DEBUG, thread,
                                          "Removing: %s", auditlogentry);
                                apr_file_remove(auditlogentry, tpool);
                            }
                            else if (keep_entries == KEEP_ENTRIES_REMOVE_HACK)
                            {
                                keep_entries_hack(tpool, thread, auditlogentry);
                            }

                            error_log(LOG_NOTICE, thread,
                                      "Entry completed (%.3f seconds, %.0f "
                                      "bytes): %s",
                                      total_time, upload_size,
                                      uniqueid);
                            take_new = 1;
                        }
                        else if (response_code == 409) {
                            /* Assume problem with audit log entry. */
                            error_log(LOG_WARNING, thread,
                                      "Failed to submit entry with "
                                      "\"409 %s\": %s",
                                      response_buf, uniqueid);
                            take_new = 1;
                        }
                        else {
                            /* Assume problem with server. */
                            error_log(LOG_WARNING, thread,
                                      "Flagging server as errored after "
                                      "failure to submit entry %s with "
                                      "HTTP response code %ld: %s",
                                      uniqueid, response_code, response_buf);
                            server_error = 1;
                            server_error_last_check_time = apr_time_now();
                            take_new = 0;
                        }
                    }
                }
                else { /* Something isn't right. */
                    error_log(LOG_WARNING, thread,
                              "Flagging server as errored after "
                              "failure to submit entry %s "
                              "(cURL code %d): %s",
                              uniqueid, res, curl_error_buffer);
                    server_error = 1;
                    server_error_last_check_time = apr_time_now();
                    take_new = 0;

                }
            }
            else {
                error_log(LOG_WARNING, thread,
                          "Invalid entry (file not found %d): %s",
                          rc, auditlogentry);
                take_new = 1;
                nodelay = 1;
            }

            /* If we are tracking num_requests, then shutdown if we are
             * over our threshold.
             */
            if (num_requests && (num_requests >= max_worker_requests)) {
                error_log(LOG_NOTICE, thread,
                          "Reached max requests (%d) for this worker, exiting.",
                          max_worker_requests);

                goto THREAD_SHUTDOWN;
            }
        }

        THREAD_CLEANUP:

        /* Sleep if we sent data to the server so we do not flood */
        /* ENH: Need to sleep for 1ms in a loop checking for shutdown */
        if ((nodelay == 0) && (transaction_delay > 0)) {
            error_log(LOG_DEBUG, thread,
                      "Sleeping for %d msec.", transaction_delay);
            apr_sleep(transaction_delay * 1000);
        }

        if (headerlist != NULL) {
            curl_slist_free_all(headerlist);
            headerlist = NULL;
        }

        apr_pool_clear(tpool);

        error_log(LOG_DEBUG, thread, "Worker processing completed.");
    }

    THREAD_SHUTDOWN:

    error_log(LOG_DEBUG, thread, "Worker shutdown locking thread mutex.");
    if (APR_STATUS_IS_EBUSY(apr_thread_mutex_trylock(mutex))) {
        error_log(LOG_DEBUG, thread,
                  "Worker shutdown waiting on thread mutex.");
        apr_thread_mutex_lock(mutex);
    }

    /* Deal with the previous entry, if any. */
    if (entry != NULL) {
        apr_hash_set(in_progress, &entry->id, sizeof(entry->id), NULL);

        if (take_new == 0) { /* Not done. */
            *(entry_t **)apr_array_push(queue) = entry;
        }
        else {
            transaction_log(TXOUT, entry->line);
            free((void *)entry->line);
            free(entry);
        }

        entry = NULL;
    }

    /* Return curl handle to the pool for reuse. */
    *(CURL **)apr_array_push(curl_handles) = curl;

    /* No more work, exit. */
    current_workers--;

    error_log(LOG_DEBUG, thread, "Worker shutdown unlocking thread mutex.");
    apr_thread_mutex_unlock(mutex);

    apr_pool_destroy(tpool);

    error_log(LOG_DEBUG, thread, "Worker thread completed.");

    apr_thread_exit(thread, 0);

    return NULL;
}


/**
 * Creates one new worker, giving it one of the available
 * Curl handles to work with.
 */
static void create_new_worker(int lock)
{
    apr_thread_t *thread = NULL;
    CURL **curlptr = NULL;

    if (lock) {
        error_log(LOG_DEBUG, NULL, "Worker creation locking thread mutex.");
        if (APR_STATUS_IS_EBUSY(apr_thread_mutex_trylock(mutex))) {
            error_log(LOG_DEBUG, NULL,
                      "Worker creation waiting on thread mutex.");
            apr_thread_mutex_lock(mutex);
        }
    }

    error_log(LOG_DEBUG, NULL, "Worker creation started.");

    /* A sanity check: this part executes under lock and
     * we want to make *sure* we don't create more threads
     * than we are allowed.
     */
    if (current_workers >= max_connections) {
        if (lock) {
            error_log(LOG_DEBUG, NULL,
                      "Worker creation unlocking thread mutex.");
            apr_thread_mutex_unlock(mutex);
        }
        return;
    }

    /* Cleanup thread pool when idle */
    if (current_workers <= 0)  {
        if (thread_pool != NULL) {
            error_log(LOG_DEBUG, NULL, "Destroying thread_pool.");
            apr_pool_destroy(thread_pool);
            thread_pool = NULL;
        }
        error_log(LOG_DEBUG, NULL, "Creating thread_pool.");
        apr_pool_create(&thread_pool, NULL);
    }

    curlptr = (CURL **)apr_array_pop(curl_handles);
    if (curlptr != NULL) {
        apr_threadattr_t *thread_attrs;
        apr_status_t rc;

        apr_threadattr_create(&thread_attrs, thread_pool);
        apr_threadattr_detach_set(thread_attrs, 1);
        apr_threadattr_stacksize_set(thread_attrs, 1024);

        rc = apr_thread_create(&thread, thread_attrs,
                               thread_worker, *curlptr, thread_pool);
        if (rc != APR_SUCCESS) {
            if (lock) {
                error_log(LOG_DEBUG, NULL,
                          "Worker creation unlocking thread mutex.");
                apr_thread_mutex_unlock(mutex);
            }
            error_log(LOG_ERROR, NULL,
                      "Failed to create new worker thread: %d", rc);
            logc_shutdown(1);
        }

        current_workers++;
    }
    else {
        if (lock) {
            error_log(LOG_DEBUG, NULL,
                      "Worker creation unlocking thread mutex.");
            apr_thread_mutex_unlock(mutex);
        }
        error_log(LOG_ERROR, NULL, "No more cURL handles (Internal Error).");
        logc_shutdown(1);
    }

    error_log(LOG_DEBUG, NULL, "Worker creation completed: %pp", thread);

    if (lock) {
        error_log(LOG_DEBUG, NULL, "Worker creation unlocking thread mutex.");
        apr_thread_mutex_unlock(mutex);
    }
}


/**
 * This function implements the management thread.
 */
static void * APR_THREAD_FUNC thread_manager(apr_thread_t *thread, void *data)
{
    apr_time_t last = 0;
    apr_time_t now = 0;

    error_log(LOG_DEBUG, thread, "Management thread: Starting.");

    for(;;) {
        now = apr_time_now();

        /* Should we stop running? */
        if (running == 0) {
            /* We need to be last */
            error_log(LOG_DEBUG, thread,
                      "Management thread: Waiting for worker "
                      "threads to finish.");
            while(current_workers > 0) {
                apr_sleep(10 * 1000);
            }

            if (have_read_data) {
                error_log(LOG_NOTICE, thread,
                          "Running final transaction checkpoint.");
                transaction_checkpoint();
            }

            error_log(LOG_DEBUG, thread, "Management thread: Exiting.");
            management_thread_active = 0;
            apr_thread_exit(thread, 0);
        }

        /* Sleep for a while, but wake up often to check running status */
        if ((last > 0) && ((now - last) < MANAGER_SLEEP)) {
            apr_sleep(MANAGER_SUBSLEEP);
            continue;
        }
        last = now;

        error_log(LOG_DEBUG2, thread, "Management thread: Processing");

        /* When the server is flagged errored we need to
         * create a worker thread from time to time to
         * investigate.
         */
        if (server_error) {
            if ((current_workers == 0)&&
                (apr_time_sec(now - server_error_last_check_time)
                    > server_error_timeout))
            {
                server_error_last_check_time = now;
                error_log(LOG_DEBUG, thread,
                          "Management thread: Creating worker thread to "
                          "investigate server.");
                create_new_worker(1);
            }
        }
        else {
            if (   (current_workers < max_connections)
                && (queue->nelts > current_workers) )
            {
                error_log(LOG_DEBUG, thread,
                          "Management thread: Creating worker thread to "
                          "catch up with the queue.");
                create_new_worker(1);
            }
        }

        /* Initiate a transaction log checkpoint if enough time passed
         * since the last one.
         */
        if (apr_time_sec(now - checkpoint_time_last) > checkpoint_interval) {
            error_log(LOG_DEBUG, thread,
                      "Management thread: Initiating a checkpoint "
                      "(previous was %" APR_TIME_T_FMT " seconds ago).",
                      apr_time_sec(now - checkpoint_time_last));
            checkpoint_time_last = now;
            transaction_checkpoint();
        }
        else {
            error_log(LOG_DEBUG2, thread,
                      "Management thread: Last checkpoint was %" APR_TIME_T_FMT
                      " seconds ago.",
                      apr_time_sec(now - checkpoint_time_last));
        }
    }

    return NULL;
}

#ifndef WIN32
/**
 * Thread to handle all signals
 */
static void * APR_THREAD_FUNC thread_signals(apr_thread_t *thread, void *data)
{
    apr_status_t rc;

    error_log(LOG_DEBUG, thread, "Signal thread: Starting.");
    rc = apr_signal_thread(handle_signals);
    if (rc != APR_SUCCESS) {
        error_log(LOG_DEBUG, thread, "Signal thread: Error %d", rc);
        logc_shutdown(1);
    }

    return NULL;
}
#endif /* WIN32 */

/**
 * The main loop where we receive log entries from
 * Apache and add them to the queue, sometimes creating
 * new worker threads to handle them.
 */
static void receive_loop(void) {
    apr_file_t *fd_stdin;
    apr_size_t nbytes = PIPE_BUF_SIZE;
    char *buf = apr_palloc(pool, PIPE_BUF_SIZE + 1);
    char errstr[1024];
    apr_size_t evnt = 0; /* Index in buf to first event char */
    apr_size_t curr = 0; /* Index in buf to current processing char */
    apr_size_t next = 0; /* Index in buf to next unused char */
    int done = 0;
    int drop_next = 0;
    int buffered_events = 0;
    int count = 0;
    apr_pool_t *tmp_pool;

    /* Open stdin. */
    if (apr_file_open_stdin(&fd_stdin, pool) != APR_SUCCESS) {
        error_log(LOG_ERROR, NULL, "Unable to open stdin for reading");
        logc_shutdown(1);
    }

    /* Always want this NUL terminated */
    buf[PIPE_BUF_SIZE] = '\0';

    apr_pool_create(&tmp_pool, NULL);

    /* Loop forever receiving entries from stdin. */
    while(!done || (curr < next)) {
        apr_status_t rc;

        if (error_log_level >= LOG_DEBUG2) {
            error_log(LOG_DEBUG2, NULL,
                      "Internal state: "
                      "[evnt \"%" APR_SIZE_T_FMT "\"]"
                      "[curr \"%" APR_SIZE_T_FMT "\"]"
                      "[next \"%" APR_SIZE_T_FMT "\"]"
                      "[nbytes \"%" APR_SIZE_T_FMT "\"]",
                      evnt, curr, next, nbytes);
        }

        /* If we are not done and have the space, read more */
        if (!done && (nbytes > 0)) {
            buffered_events = 0;
            nbytes = PIPE_BUF_SIZE - next;
            rc = apr_file_read(fd_stdin, (buf + next), &nbytes);
            if (rc != APR_SUCCESS) {
                if (have_read_data) {
                    error_log(LOG_NOTICE, NULL,
                              "No more data to read, emptying buffer: %s",
                              apr_strerror(rc, errstr, 1024));
                }
                done = 1;
            }
            else {
                have_read_data = 1;
                if (error_log_level == LOG_DEBUG) {
                    error_log(LOG_DEBUG, NULL,
                              "Read %" APR_SIZE_T_FMT " bytes from pipe",
                              nbytes);
                }
                else {
                    error_log(LOG_DEBUG2, NULL,
                              "Read %" APR_SIZE_T_FMT " bytes from pipe: `%s'",
                              nbytes,
                              _log_escape(tmp_pool, (buf + next), nbytes));
                }
            }

            next += nbytes;
        }

        /**
         * Each chunk of data we receive can contain one or more lines for
         * which we need to find the EOL marker and then queue the event
         * up to that.  So, find/queue as many lines in the buffer as we
         * can.  Any remaining data will get shifted back to the beginning
         * of the buffer and the buffer size for the next read adjusted.
         */
        while(curr < next) {
            /* Look for EOL so we can parse the event */
            while((curr < next) && (buf[curr] != 0x0a)) {
                curr++;
            }
            if (buf[curr] == 0x0a) {
                buf[curr] = '\0';

                /* We may have to drop this one if it previously failed */
                if (drop_next) {
                    error_log(LOG_ERROR, NULL,
                              "Dropping remaining portion of failed "
                              "event: `%s'",
                              _log_escape(tmp_pool,
                                          (buf + evnt), (curr - evnt)));
                    drop_next = 0;
                }
                else {
                    transaction_log(TXIN, buf + evnt);
                    error_log(LOG_DEBUG2, NULL,
                              "Received audit log entry "
                              "(count %lu queue %d workers %d): %s",
                              entry_counter, queue->nelts,
                              current_workers,
                              _log_escape(tmp_pool,
                                          (buf + evnt), strlen(buf + evnt)));
                    add_entry(buf + evnt, 1);
                    buffered_events++;
                }

                /* Advance indexes to next event in buf */
                evnt = curr = curr + 1;
            }
            else {
                error_log(LOG_DEBUG2, NULL,
                          "Event buffer contains partial event: `%s'",
                          _log_escape(tmp_pool, (buf + evnt), (next - evnt)));
                break;
            }
        }


        if (buffered_events > 0) {
            error_log(LOG_DEBUG, NULL,
                      "Processed %d entries from buffer.", buffered_events);

            /* Move the unused portion of the buffer to the beginning */
            next -= evnt;
            curr -= evnt;
            memmove(buf, (buf + evnt), next);

            error_log(LOG_DEBUG2, NULL,
                      "Shifted buffer back %" APR_SIZE_T_FMT 
                      " and offset %" APR_SIZE_T_FMT 
                      " bytes for next read: `%s'",
                      evnt, next, _log_escape(tmp_pool, buf, next));

            evnt = 0;
        }
        else if (next == PIPE_BUF_SIZE) {
            /**
             * There is a chance we could fill the buffer, but not have finished
             * reading the event (no EOL yet), so we need to say so and drop
             * all data until we find the end of the event that is too large.
             */

            if (drop_next) {
                error_log(LOG_ERROR, NULL,
                          "Event continuation too large, "
                          "dropping it as well: `%s'",
                          _log_escape(tmp_pool, buf, PIPE_BUF_SIZE));
            }
            else {
                error_log(LOG_ERROR, NULL,
                          "Event too large, dropping event: `%s'",
                          _log_escape(tmp_pool, buf, PIPE_BUF_SIZE));
            }

            /* Rewind buf and mark that we need to drop up to the next event */
            evnt = curr = next = 0;
            drop_next = 1;
        }

        nbytes = PIPE_BUF_SIZE - next;

        if (count++ > 1000) {
            count = 0;
            error_log(LOG_DEBUG, NULL, "Recycling tmp_pool.");
            apr_pool_destroy(tmp_pool);
            apr_pool_create(&tmp_pool, NULL);
        }
        else {
            apr_pool_clear(tmp_pool);
        }
    }

    /* Wait for queue to empty if specified */
    if ((server_error == 0) && (opt_force != 0) && (queue->nelts > 0)) {
        error_log(LOG_NOTICE, NULL,
                  "Waiting for queue to empty (%d active).", queue->nelts);
        while ((server_error == 0) && (opt_force != 0) && (queue->nelts > 0)) {
            apr_sleep(10 * 1000);
        }
        if (queue->nelts > 0) {
            error_log(LOG_ERROR, NULL,
                      "Could not empty queue (%d active).", queue->nelts);
        }
    }
}


/**
 * Creates the management thread.
 */
static void start_management_thread(void)
{
    apr_thread_t *thread = NULL;
    apr_threadattr_t *thread_attrs;
    apr_status_t rc;

    apr_threadattr_create(&thread_attrs, pool);
    apr_threadattr_detach_set(thread_attrs, 1);
    apr_threadattr_stacksize_set(thread_attrs, 1024);

    management_thread_active = 1;

    rc = apr_thread_create(&thread, thread_attrs, thread_manager, NULL, pool);
    if (rc != APR_SUCCESS) {
        error_log(LOG_ERROR, NULL,
                  "Failed to create new management thread: %d", rc);
        management_thread_active = 0;
        logc_shutdown(1);
    }
}
#ifndef WIN32
/**
 * Creates a thread to handle all signals
 */
static void start_signal_thread(void)
{
    apr_thread_t *thread = NULL;
    apr_threadattr_t *thread_attrs;
    apr_status_t rc;

    apr_threadattr_create(&thread_attrs, pool);
    apr_threadattr_detach_set(thread_attrs, 1);
    apr_threadattr_stacksize_set(thread_attrs, 1024);

    rc = apr_thread_create(&thread, thread_attrs, thread_signals, NULL, pool);
    if (rc != APR_SUCCESS) {
        error_log(LOG_ERROR, NULL,
                  "Failed to create new signal thread: %d", rc);
        logc_shutdown(1);
    }
}
#endif /* WIN32 */

/**
 * Usage text.
 */
static void usage(void) {
    fprintf(stderr, "ModSecurity Log Collector (mlogc) v%s\n", VERSION);
    fprintf(stderr, "  Usage: mlogc [options] /path/to/the/mlogc.conf\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  Options:\n");
    fprintf(stderr, "    -f        Force depletion of queue on exit\n");
    fprintf(stderr, "    -v        Version information\n");
    fprintf(stderr, "    -h        This help\n\n");
}

/**
 * Version text.
 */
static void version(void) {
    fprintf(stderr,
            "ModSecurity Log Collector (mlogc) v%s\n", VERSION);
    fprintf(stderr,
            "   APR: compiled=\"%s\"; "
            "loaded=\"%s\"\n", APR_VERSION_STRING, apr_version_string());
    fprintf(stderr,
            "  PCRE: compiled=\"%d.%d\"; "
            "loaded=\"%s\"\n", PCRE_MAJOR, PCRE_MINOR, pcre_version());
    fprintf(stderr,
            "  cURL: compiled=\"%s\"; "
            "loaded=\"%s\"\n", LIBCURL_VERSION, curl_version());
    fprintf(stderr, "\n");
}

/**
 * This is the main entry point.
 */
int main(int argc, const char * const argv[]) {
    apr_getopt_t *opt;
    apr_status_t rc;

    apr_app_initialize(&argc, &argv, NULL);
    atexit(apr_terminate);

    curl_global_init(CURL_GLOBAL_ALL);
    atexit(logc_cleanup);

    logc_pid = getpid();
    apr_pool_create(&pool, NULL);
    apr_pool_create(&recv_pool, NULL);
	
#ifndef WIN32
    apr_setup_signal_thread();
#else
	apr_signal(SIGINT, handle_signals);
	apr_signal(SIGTERM, handle_signals);
#endif /* WIN32 */

    if (argc < 2) {
        usage();
        logc_shutdown(1);
    }

    /* Commandline opts */
    rc = apr_getopt_init(&opt, pool, argc, argv);
    if (rc != APR_SUCCESS) {
        usage();
        logc_shutdown(1);
    }

    do {
        char  ch;
        const char *val;
        rc = apr_getopt(opt, CMDLINE_OPTS, &ch, &val);
        switch (rc) {
            case APR_SUCCESS:
                switch (ch) {
                    case 'f':
                        opt_force = 1;
                        break;
                    case 'v':
                        version();
                        logc_shutdown(0);
                    case 'h':
                        usage();
                        logc_shutdown(0);
                }
                break;
            case APR_BADCH:
            case APR_BADARG:
                usage();
                logc_shutdown(1);
        }
    } while (rc != APR_EOF);

    /* Conf file is last */
    conffile = argv[argc - 1];

    read_configuration();
    init_configuration();

    logc_init();
    transaction_log_init();

    running = 1;
    server_error = 0;

    start_management_thread();
#ifndef WIN32
    start_signal_thread();
#endif /* WIN32 */

    /* Process stdin until EOF */
    receive_loop();

    logc_shutdown(0);

    return 0;
}
