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
#ifndef _MODSECURITY_H_
#define _MODSECURITY_H_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct rule_exception rule_exception;
typedef struct modsec_rec modsec_rec;
typedef struct directory_config directory_config;
typedef struct error_message error_message;
typedef struct msc_engine msc_engine;
typedef struct msc_data_chunk msc_data_chunk;
typedef struct msc_arg msc_arg;
typedef struct msc_string msc_string;

#if !(defined(WIN32) || defined(CYGWIN) || defined(NETWARE) || defined(SOLARIS2))
#define DSOLOCAL __attribute__((visibility("hidden")))
#else
#define DSOLOCAL
#endif

#include "msc_logging.h"
#include "msc_multipart.h"
#include "msc_pcre.h"
#include "msc_util.h"
#ifdef WITH_LIBXML2
#include "msc_xml.h"
#endif
#include "re.h"

#include "ap_config.h"
#include "apr_md5.h"
#include "apr_strings.h"
#include "httpd.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"

#define MODULE_NAME "ModSecurity"
#define MODULE_RELEASE "2.1.2"
#define MODULE_NAME_FULL (MODULE_NAME " v" MODULE_RELEASE " (Apache 2.x)")

#define PHASE_REQUEST_HEADERS       1
#define PHASE_REQUEST_BODY          2
#define PHASE_RESPONSE_HEADERS      3
#define PHASE_RESPONSE_BODY         4
#define PHASE_LOGGING               5

#define NOT_SET     -1
#define NOT_SET_P (void *)-1

#define CREATEMODE ( APR_UREAD | APR_UWRITE | APR_GREAD )
#define CREATEMODE_DIR ( APR_UREAD | APR_UWRITE | APR_UEXECUTE | APR_GREAD | APR_GEXECUTE )

#if defined(NETWARE)
#define CREATEMODE_UNISTD ( S_IREAD | S_IWRITE )
#elif defined(WIN32)
#define CREATEMODE_UNISTD ( _S_IREAD | _S_IWRITE )
#else
#define CREATEMODE_UNISTD ( S_IRUSR | S_IWUSR | S_IRGRP )
#endif

#if !defined(O_BINARY)
#define O_BINARY (0)
#endif

#ifndef PIPE_BUF
#define PIPE_BUF (512)
#endif

#define REQUEST_BODY_HARD_LIMIT                 1073741824L
#define REQUEST_BODY_DEFAULT_INMEMORY_LIMIT     131072
#define REQUEST_BODY_DEFAULT_LIMIT              134217728
#define RESPONSE_BODY_DEFAULT_LIMIT             524288
#define RESPONSE_BODY_HARD_LIMIT                1073741824L

#if !defined(OS2) && !defined(WIN32) && !defined(BEOS) && !defined(NETWARE)
#include "unixd.h"
#define __SET_MUTEX_PERMS
#endif

#define COOKIES_V0                      0
#define COOKIES_V1                      1

#ifdef WIN32
#include <direct.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#define NOTE_MSR "modsecurity-tx-context"

#define FATAL_ERROR "ModSecurity: Fatal error (memory allocation or unexpected internal error)!"

extern DSOLOCAL char *new_server_signature;
extern DSOLOCAL char *real_server_signature;
extern DSOLOCAL char *chroot_dir;

extern module AP_MODULE_DECLARE_DATA security2_module;

extern DSOLOCAL const command_rec module_directives[];

#define RESBODY_STATUS_NOT_READ         0   /* we were not configured to read the body */
#define RESBODY_STATUS_ERROR            1   /* error occured while we were reading the body */
#define RESBODY_STATUS_PARTIAL          2   /* partial body content available in the brigade */
#define RESBODY_STATUS_READ_BRIGADE     3   /* body was read but not flattened */
#define RESBODY_STATUS_READ             4   /* body was read and flattened */

#define IF_STATUS_NONE                  0
#define IF_STATUS_WANTS_TO_RUN          1
#define IF_STATUS_COMPLETE              2

#define OF_STATUS_NOT_STARTED           0
#define OF_STATUS_IN_PROGRESS           1
#define OF_STATUS_COMPLETE              2

#define MSC_REQBODY_NONE                0
#define MSC_REQBODY_MEMORY              1
#define MSC_REQBODY_DISK                2

#define ACTION_NONE                     0
#define ACTION_DENY                     1
#define ACTION_REDIRECT                 2
#define ACTION_PROXY                    3
#define ACTION_DROP                     4
#define ACTION_ALLOW                    5

#define MODSEC_DISABLED                 0
#define MODSEC_DETECTION_ONLY           1
#define MODSEC_ENABLED                  2

#define MODSEC_OFFLINE                  0
#define MODSEC_ONLINE                   1

#define REGEX_CAPTURE_BUFLEN            1024

#define KEEP_FILES_OFF                  0
#define KEEP_FILES_ON                   1
#define KEEP_FILES_RELEVANT_ONLY        2

#define RULE_EXCEPTION_IMPORT_ID        1
#define RULE_EXCEPTION_IMPORT_MSG       2
#define RULE_EXCEPTION_REMOVE_ID        3
#define RULE_EXCEPTION_REMOVE_MSG       4

#define NBSP                            160

struct rule_exception {
    int                  type;
    const char          *param;
    void                *param_data;
};

struct modsec_rec {
    apr_pool_t          *mp;
    msc_engine          *modsecurity;

    request_rec         *r_early;
    request_rec         *r;
    directory_config    *dcfg1;
    directory_config    *dcfg2;
    directory_config    *usercfg;
    directory_config    *txcfg;

    unsigned int         reqbody_should_exist;
    unsigned int         reqbody_chunked;

    unsigned int         phase;
    unsigned int         phase_request_headers_complete;
    unsigned int         phase_request_body_complete;

    apr_bucket_brigade  *if_brigade;
    unsigned int         if_status;
    unsigned int         if_started_forwarding;

    apr_size_t           reqbody_length;
    unsigned int         reqbody_status;

    apr_bucket_brigade  *of_brigade;
    unsigned int         of_status;
    unsigned int         of_done_reading;
    unsigned int         of_skipping;

    unsigned int         resbody_status;
    apr_size_t           resbody_length;
    char                *resbody_data;
    unsigned int         resbody_contains_html;

    apr_array_header_t  *error_messages;
    apr_array_header_t  *alerts;

    const char          *txid;
    const char          *sessionid;
    const char          *userid;

    const char          *server_software;
    const char          *local_addr;
    unsigned int         local_port;
    const char          *local_user;

    /* client */

    const char          *remote_addr;
    unsigned int         remote_port;
    const char          *remote_user;

    /* request */

    const char          *request_line;
    const char          *request_method;
    const char          *request_uri;
    const char          *query_string;
    const char          *request_protocol;

    const char          *hostname;
    
    apr_table_t         *request_headers;

    apr_off_t            request_content_length;
    const char          *request_content_type;

    apr_table_t         *arguments;
    apr_table_t         *arguments_to_sanitise;
    apr_table_t         *request_headers_to_sanitise;
    apr_table_t         *response_headers_to_sanitise;
    apr_table_t         *request_cookies;

    unsigned int         is_relevant;

    apr_table_t         *tx_vars;

    /* response */
    unsigned int         response_status;
    const char          *status_line;
    const char          *response_protocol;
    apr_table_t         *response_headers;
    unsigned int         response_headers_sent;
    apr_off_t            bytes_sent;

    /* modsecurity request body processing stuff */

    unsigned int         msc_reqbody_storage;       /* on disk or in memory */
    unsigned int         msc_reqbody_spilltodisk;
    unsigned int         msc_reqbody_read;

    apr_pool_t          *msc_reqbody_mp;             /* this is where chunks are allocated from  */
    apr_array_header_t  *msc_reqbody_chunks;         /* data chunks when stored in memory        */
    unsigned int         msc_reqbody_length;         /* the amount of data received              */
    int                  msc_reqbody_chunk_position; /* used when retrieving the body            */
    unsigned int         msc_reqbody_chunk_offset;   /* offset of the chunk currently in use     */
    msc_data_chunk      *msc_reqbody_chunk_current;  /* current chunk                            */
    char                *msc_reqbody_buffer;
    
    const char          *msc_reqbody_filename;       /* when stored on disk */
    int                  msc_reqbody_fd;
    msc_data_chunk      *msc_reqbody_disk_chunk;

    const char          *msc_reqbody_processor;
    int                  msc_reqbody_error;
    const char          *msc_reqbody_error_msg;

    multipart_data      *mpd;                        /* MULTIPART processor data structure */

    #ifdef WITH_LIBXML2
    xml_data            *xml;                        /* XML processor data structure       */
    #endif

    /* audit logging */
    char                *new_auditlog_boundary;
    char                *new_auditlog_filename;
    apr_file_t          *new_auditlog_fd;
    unsigned int         new_auditlog_size;
    apr_md5_ctx_t        new_auditlog_md5ctx;

    unsigned int         was_intercepted;
    unsigned int         intercept_phase;
    msre_actionset      *intercept_actionset;
    const char          *intercept_message;

    /* performance measurement */
    apr_time_t           request_time;
    apr_time_t           time_checkpoint_1;
    apr_time_t           time_checkpoint_2;
    apr_time_t           time_checkpoint_3;

    const char          *matched_var;

    /* upload */
    int                  upload_extract_files;
    int                  upload_remove_files;

    /* other */
    apr_table_t         *collections;
    apr_table_t         *collections_dirty;

    /* rule processing temp pool */
    apr_pool_t          *msc_rule_mptmp;
};

struct directory_config {
    apr_pool_t          *mp;

    msre_ruleset        *ruleset;

    int                  is_enabled;
    int                  reqbody_access;
    long int             reqbody_inmemory_limit;
    long int             reqbody_limit;
    int                  resbody_access;

    long int             of_limit;
    apr_table_t         *of_mime_types;
    int                  of_mime_types_cleared;

    const char          *debuglog_name;
    int                  debuglog_level;
    apr_file_t          *debuglog_fd;

    int                  cookie_format;
    int                  argument_separator;

    int                  rule_inheritance;
    apr_array_header_t  *rule_exceptions;


    /* -- Audit log -- */

    /* Whether audit log should be enabled in the context or not */
    int                  auditlog_flag;

    /* AUDITLOG_SERIAL (single file) or AUDITLOG_CONCURRENT (multiple files) */
    int                  auditlog_type;

    /* The name of the audit log file (for the old type), or the
     * name of the index file (for the new audit log type)
     */
    char                *auditlog_name;
    /* The name of the secondary index file */
    char                *auditlog2_name;

    /* The file descriptors for the files above */
    apr_file_t          *auditlog_fd;
    apr_file_t          *auditlog2_fd;

    /* For the new-style audit log only, the path where
     * audit log entries will be stored
     */
    char                *auditlog_storage_dir;

    /* A list of parts to include in the new-style audit log
     * entry. By default, it contains 'ABCFHZ'. Have a look at
     * the AUDITLOG_PART_* constants above to decipher the
     * meaning.
     */
    char                *auditlog_parts;

    /* A regular expression that determines if a response
     * status is treated as relevant.
     */
    msc_regex_t         *auditlog_relevant_regex;    

    /* Upload */
    const char          *tmp_dir;
    const char          *upload_dir;
    int                  upload_keep_files;
    int                  upload_validates_files;

    /* Used only in the configuration phase. */
    msre_rule           *tmp_chain_starter;
    msre_actionset      *tmp_default_actionset;

    /* Misc */
    const char          *data_dir;
    const char          *webappid;
};

struct error_message {
    const char          *file;
    int                  line;
    int                  level;
    apr_status_t         status;
    const char          *message;
};

struct msc_engine {
    apr_pool_t              *mp;
    apr_global_mutex_t      *auditlog_lock;
    msre_engine             *msre;
    unsigned int             processing_mode;
};

struct msc_data_chunk {
    char                    *data;
    apr_size_t               length;
    unsigned int             is_permanent;
};

struct msc_arg {
    const char              *name;
    unsigned int             name_len;
    unsigned int             name_origin_offset;
    unsigned int             name_origin_len;
    const char              *value;
    unsigned int             value_len;
    unsigned int             value_origin_offset;
    unsigned int             value_origin_len;
    const char              *origin;
};

struct msc_string {
    char                    *name;
    unsigned int             name_len;
    char                    *value;
    unsigned int             value_len;
};


/* Engine functions */

msc_engine DSOLOCAL *modsecurity_create(apr_pool_t *mp, int processing_mode);

int DSOLOCAL modsecurity_init(msc_engine *msce, apr_pool_t *mp);

void DSOLOCAL modsecurity_child_init(msc_engine *msce);

void DSOLOCAL modsecurity_shutdown(msc_engine *msce);

apr_status_t DSOLOCAL modsecurity_tx_init(modsec_rec *msr);

apr_status_t DSOLOCAL modsecurity_process_phase(modsec_rec *msr, int phase);


/* Request body functions */

apr_status_t DSOLOCAL modsecurity_request_body_start(modsec_rec *msr);

apr_status_t DSOLOCAL modsecurity_request_body_store(modsec_rec *msr,
    const char *data, apr_size_t length);

apr_status_t DSOLOCAL modsecurity_request_body_end(modsec_rec *msr);

apr_status_t DSOLOCAL modsecurity_request_body_retrieve_start(modsec_rec *msr);

apr_status_t DSOLOCAL modsecurity_request_body_retrieve_end(modsec_rec *msr);

/* Retrieves up to nbytes bytes of the request body. Returns 1 on
 * success, 0 when there is no more data, or -1 on error. On return
 * nbytes will contain the number of bytes stored in the buffer.
 */
apr_status_t DSOLOCAL modsecurity_request_body_retrieve(modsec_rec *msr, msc_data_chunk **chunk,
    long int nbytes);

void DSOLOCAL msc_add(modsec_rec *msr, int level, msre_actionset *actionset,
    const char *action_message, const char *rule_message);

void DSOLOCAL msc_alert(modsec_rec *msr, int level, msre_actionset *actionset, const char *action_message,
    const char *rule_message);

apr_status_t DSOLOCAL modsecurity_request_body_clear(modsec_rec *msr);

#endif
