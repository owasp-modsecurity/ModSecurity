/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2013 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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

#ifndef _MODSECURITY_H_
#define _MODSECURITY_H_

#include <stdio.h>
#include <stdlib.h>

#include <limits.h>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>

typedef struct rule_exception rule_exception;
typedef struct rule_exception hash_method;
typedef struct modsec_rec modsec_rec;
typedef struct directory_config directory_config;
typedef struct error_message_t error_message_t;
typedef struct msc_engine msc_engine;
typedef struct msc_data_chunk msc_data_chunk;
typedef struct msc_arg msc_arg;
typedef struct msc_string msc_string;
typedef struct msc_parm msc_parm;

#include "msc_release.h"
#include "msc_logging.h"
#include "msc_multipart.h"
#include "msc_pcre.h"
#include "msc_util.h"
#include "msc_json.h"
#include "msc_xml.h"
#include "msc_tree.h"
#include "msc_geo.h"
#include "msc_gsb.h"
#include "msc_unicode.h"
#include "re.h"
#include "msc_crypt.h"
#include "msc_remote_rules.h"

#include "ap_config.h"
#include "apr_md5.h"
#include "apr_strings.h"
#include "apr_hash.h"
#include "httpd.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"

#if defined(WITH_LUA)
#include "msc_lua.h"
#endif

#define PHASE_REQUEST_HEADERS       1
#define PHASE_REQUEST_BODY          2
#define PHASE_RESPONSE_HEADERS      3
#define PHASE_RESPONSE_BODY         4
#define PHASE_LOGGING               5
#define PHASE_FIRST                 PHASE_REQUEST_HEADERS
#define PHASE_LAST                  PHASE_LOGGING

#define NOT_SET                    -1l
#define NOT_SET_P         ((void *)-1l)

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
#define REQUEST_BODY_NO_FILES_DEFAULT_LIMIT     1048576
#define REQUEST_BODY_JSON_DEPTH_DEFAULT_LIMIT   10000
#define RESPONSE_BODY_DEFAULT_LIMIT             524288
#define RESPONSE_BODY_HARD_LIMIT                1073741824L

#define RESPONSE_BODY_LIMIT_ACTION_REJECT       0
#define RESPONSE_BODY_LIMIT_ACTION_PARTIAL      1

#define REQUEST_BODY_FORCEBUF_OFF               0
#define REQUEST_BODY_FORCEBUF_ON                1

#define REQUEST_BODY_LIMIT_ACTION_REJECT       0
#define REQUEST_BODY_LIMIT_ACTION_PARTIAL      1

#define SECACTION_TARGETS                       "REMOTE_ADDR"
#define SECACTION_ARGS                          "@unconditionalMatch"

#define SECMARKER_TARGETS                       "REMOTE_ADDR"
#define SECMARKER_ARGS                          "@noMatch"
#define SECMARKER_BASE_ACTIONS                  "t:none,pass,marker:"

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

static char auditlog_lock_name[L_tmpnam];
static char geo_lock_name[L_tmpnam];
#ifdef GLOBAL_COLLECTION_LOCK
static char dbm_lock_name[L_tmpnam];
#endif

extern DSOLOCAL char *new_server_signature;
extern DSOLOCAL char *real_server_signature;
extern DSOLOCAL char *chroot_dir;

extern module AP_MODULE_DECLARE_DATA security2_module;

extern DSOLOCAL const command_rec module_directives[];

extern DSOLOCAL unsigned long int msc_pcre_match_limit;

extern DSOLOCAL unsigned long int msc_pcre_match_limit_recursion;

#ifdef WITH_REMOTE_RULES
extern DSOLOCAL msc_remote_rules_server *remote_rules_server;
#endif
extern DSOLOCAL int remote_rules_fail_action;
extern DSOLOCAL char *remote_rules_fail_message;

extern DSOLOCAL int status_engine_state;

extern DSOLOCAL int conn_limits_filter_state;

extern DSOLOCAL unsigned long int conn_read_state_limit;
extern DSOLOCAL TreeRoot *conn_read_state_whitelist;
extern DSOLOCAL TreeRoot *conn_read_state_suspicious_list;

extern DSOLOCAL unsigned long int conn_write_state_limit;
extern DSOLOCAL TreeRoot *conn_write_state_whitelist;
extern DSOLOCAL TreeRoot *conn_write_state_suspicious_list;

extern DSOLOCAL unsigned long int unicode_codepage;

extern DSOLOCAL int *unicode_map_table;

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
#define ACTION_ALLOW_REQUEST            6
#define ACTION_ALLOW_PHASE              7
#define ACTION_PAUSE                    8

#define MODSEC_DISABLED                 0
#define MODSEC_DETECTION_ONLY           1
#define MODSEC_ENABLED                  2

#define STATUS_ENGINE_ENABLED           1
#define STATUS_ENGINE_DISABLED          0

#define REMOTE_RULES_ABORT_ON_FAIL	0
#define REMOTE_RULES_WARN_ON_FAIL	1

#define HASH_DISABLED             0
#define HASH_ENABLED              1

#define HASH_URL_HREF_HASH_RX     0
#define HASH_URL_HREF_HASH_PM     1
#define HASH_URL_FACTION_HASH_RX  2
#define HASH_URL_FACTION_HASH_PM  3
#define HASH_URL_LOCATION_HASH_RX 4
#define HASH_URL_LOCATION_HASH_PM 5
#define HASH_URL_IFRAMESRC_HASH_RX 6
#define HASH_URL_IFRAMESRC_HASH_PM 7
#define HASH_URL_FRAMESRC_HASH_RX 8
#define HASH_URL_FRAMESRC_HASH_PM 9

#define HASH_KEYONLY              0
#define HASH_SESSIONID            1
#define HASH_REMOTEIP             2

#define MODSEC_CACHE_DISABLED           0
#define MODSEC_CACHE_ENABLED            1

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
#define RULE_EXCEPTION_REMOVE_TAG       5

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
    unsigned int         if_seen_eos;
    unsigned int         if_status;
    unsigned int         if_started_forwarding;

    apr_size_t           reqbody_length;

    apr_bucket_brigade  *of_brigade;
    unsigned int         of_status;
    unsigned int         of_done_reading;
    unsigned int         of_skipping;
    unsigned int         of_partial;
    unsigned int         of_is_error;

    unsigned int         resbody_status;
    apr_size_t           resbody_length;
    char                *resbody_data;
    unsigned int         resbody_contains_html;

    apr_size_t           stream_input_length;
#ifdef MSC_LARGE_STREAM_INPUT
    apr_size_t           stream_input_allocated_length;
#endif

    char                *stream_input_data;
    apr_size_t           stream_output_length;
    char                *stream_output_data;
    unsigned int        of_stream_changed;
    unsigned int        if_stream_changed;

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

    /* useragent */
    const char          *useragent_ip;

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
    apr_table_t         *arguments_to_sanitize;
    apr_table_t         *request_headers_to_sanitize;
    apr_table_t         *response_headers_to_sanitize;
    apr_table_t         *request_cookies;
    apr_table_t         *pattern_to_sanitize;

    unsigned int        urlencoded_error;
    unsigned int        inbound_error;
    unsigned int        outbound_error;

    unsigned int         is_relevant;

    apr_table_t         *tx_vars;

    /* ENH: refactor to allow arbitrary var tables */
    apr_table_t         *geo_vars;

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

    apr_size_t           msc_reqbody_no_files_length;

    char		*msc_full_request_buffer;
    int			msc_full_request_length;

    char                *multipart_filename;
    char                *multipart_name;
    multipart_data      *mpd;                        /* MULTIPART processor data structure */

    xml_data            *xml;                        /* XML processor data structure       */
#ifdef WITH_YAJL
    json_data           *json;                       /* JSON processor data structure      */
#endif

    /* audit logging */
    char                *new_auditlog_boundary;
    char                *new_auditlog_filename;
    apr_file_t          *new_auditlog_fd;
    unsigned int         new_auditlog_size;
    apr_md5_ctx_t        new_auditlog_md5ctx;

    unsigned int         was_intercepted;
    unsigned int         rule_was_intercepted;
    unsigned int         intercept_phase;
    msre_actionset      *intercept_actionset;
    const char          *intercept_message;

    /* performance measurement */
    apr_time_t       request_time;
    apr_time_t		 time_phase1;
    apr_time_t		 time_phase2;
    apr_time_t		 time_phase3;
    apr_time_t		 time_phase4;
    apr_time_t		 time_phase5;
    apr_time_t		 time_storage_read;
    apr_time_t		 time_storage_write;
    apr_time_t		 time_logging;
    apr_time_t		 time_gc;
    apr_table_t      *perf_rules;

    apr_array_header_t  *matched_rules;
    msc_string          *matched_var;
    int                  highest_severity;

    /* upload */
    int                  upload_extract_files;
    int                  upload_remove_files;
    int                  upload_files_count;

    /* other */
    apr_table_t         *collections_original;
    apr_table_t         *collections;
    apr_table_t         *collections_dirty;

    /* rule processing temp pool */
    apr_pool_t          *msc_rule_mptmp;

    /* content injection */
    const char          *content_prepend;
    apr_off_t            content_prepend_len;
    const char          *content_append;
    apr_off_t            content_append_len;

    /* data cache */
    apr_hash_t          *tcache;
    apr_size_t           tcache_items;

    /* removed rules */
    apr_array_header_t  *removed_rules;
    apr_array_header_t  *removed_rules_tag;
    apr_array_header_t  *removed_rules_msg;

    /* removed targets */
    apr_table_t         *removed_targets;

    /* When "allow" is executed the variable below is
     * updated to contain the scope of the allow action. Set
     * at 0 by default, it will have ACTION_ALLOW if we are
     * to allow phases 1-4 and ACTION_ALLOW_REQUEST if we
     * are to allow phases 1-2 only.
     */
    unsigned int         allow_scope;

    /* matched vars */
    apr_table_t         *matched_vars;

    /* Generic request body processor context to be used by custom parsers. */
    void                *reqbody_processor_ctx;

    htmlDocPtr          crypto_html_tree;
#if defined(WITH_LUA)
    #ifdef CACHE_LUA
    lua_State           *L;
    #endif
#endif

    int                 msc_sdbm_delete_error;
};

struct directory_config {
    apr_pool_t          *mp;

    msre_ruleset        *ruleset;

    int                  is_enabled;
    int                  reqbody_access;
    int                  reqintercept_oe;
    int                  reqbody_buffering;
    long int             reqbody_inmemory_limit;
    long int             reqbody_limit;
    long int             reqbody_no_files_limit;
    long int             reqbody_json_depth_limit;
    int                  resbody_access;

    long int             of_limit;
    apr_table_t         *of_mime_types;
    int                  of_mime_types_cleared;
    int                  of_limit_action;
    int                  if_limit_action;

    const char          *debuglog_name;
    int                  debuglog_level;
    apr_file_t          *debuglog_fd;

    int                  cookie_format;
    int                  argument_separator;
    const char           *cookiev0_separator;

    int                  rule_inheritance;
    apr_array_header_t  *rule_exceptions;


    /* -- Audit log -- */

    /* Max rule time */
    int                  max_rule_time;

    /* Whether audit log should be enabled in the context or not */
    int                  auditlog_flag;

    /* AUDITLOG_SERIAL (single file) or AUDITLOG_CONCURRENT (multiple files) */
    int                  auditlog_type;

#ifdef WITH_YAJL
    /* AUDITLOGFORMAT_NATIVE or AUDITLOGFORMAT_JSON */
    int                  auditlog_format;
#endif

    /* Mode for audit log directories and files */
    apr_fileperms_t      auditlog_dirperms;
    apr_fileperms_t      auditlog_fileperms;

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
    int                  upload_filemode; /* int only so NOT_SET works */
    int                  upload_file_limit;

    /* Used only in the configuration phase. */
    msre_rule           *tmp_chain_starter;
    msre_actionset      *tmp_default_actionset;
    apr_table_t         *tmp_rule_placeholders;

    /* Misc */
    const char          *data_dir;
    const char          *webappid;
    const char          *sensor_id;
    const char          *httpBlkey;

    /* Content injection. */
    int                  content_injection_enabled;

    /* Stream Inspection */
    int                 stream_inbody_inspection;
    int                 stream_outbody_inspection;

    /* Geo Lookup */
    geo_db              *geo;

    /* Gsb Lookup */
    gsb_db              *gsb;

    /* Unicode map */
    unicode_map         *u_map;

    /* Cache */
    int                  cache_trans;
    int                  cache_trans_incremental;
    apr_size_t           cache_trans_min;
    apr_size_t           cache_trans_max;
    apr_size_t           cache_trans_maxitems;

    /* Array to hold signatures of components, which will
     * appear in the ModSecurity signature in the audit log.
     */
    apr_array_header_t  *component_signatures;

    /* Request character encoding. */
    const char          *request_encoding;

    int disable_backend_compression;

    /* Collection timeout */
    int col_timeout;

    /* hash of ids */
    apr_hash_t          *rule_id_htab;

    /* Hash */
    apr_array_header_t  *hash_method;
    const char          *crypto_key;
    int                 crypto_key_len;
    const char          *crypto_param_name;
    int                 hash_is_enabled;
    int                 hash_enforcement;
    int                 crypto_key_add;
    int                 crypto_hash_href_rx;
    int                 crypto_hash_faction_rx;
    int                 crypto_hash_location_rx;
    int                 crypto_hash_iframesrc_rx;
    int                 crypto_hash_framesrc_rx;
    int                 crypto_hash_href_pm;
    int                 crypto_hash_faction_pm;
    int                 crypto_hash_location_pm;
    int                 crypto_hash_iframesrc_pm;
    int                 crypto_hash_framesrc_pm;

    /* xml */
    int                 xml_external_entity;

    /* This will be used whenever ModSecurity will be ready
     * to ask the server for newer rules.
     */
#if 0
    msc_remote_rules_server *remote_rules;
    int remote_timeout;
#endif
};

struct error_message_t {
    const char          *file;
    int                  line;
    int                  level;
    apr_status_t         status;
    const char          *message;
};

struct msc_engine {
    apr_pool_t              *mp;
    apr_global_mutex_t      *auditlog_lock;
    apr_global_mutex_t      *geo_lock;
#ifdef GLOBAL_COLLECTION_LOCK
    apr_global_mutex_t      *dbm_lock;
#endif
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

struct msc_parm {
    char                    *value;
    int                     pad_1;
    int                     pad_2;
};

/* Engine functions */

msc_engine DSOLOCAL *modsecurity_create(apr_pool_t *mp, int processing_mode);

int DSOLOCAL modsecurity_init(msc_engine *msce, apr_pool_t *mp);

void DSOLOCAL modsecurity_child_init(msc_engine *msce);

void DSOLOCAL modsecurity_shutdown(msc_engine *msce);

apr_status_t DSOLOCAL modsecurity_tx_init(modsec_rec *msr);

apr_status_t DSOLOCAL modsecurity_process_phase(modsec_rec *msr, unsigned int phase);


/* Request body functions */

apr_status_t DSOLOCAL modsecurity_request_body_start(modsec_rec *msr, char **error_msg);

apr_status_t DSOLOCAL modsecurity_request_body_store(modsec_rec *msr,
    const char *data, apr_size_t length, char **error_msg);

apr_status_t DSOLOCAL modsecurity_request_body_end(modsec_rec *msr, char **error_msg);

apr_status_t DSOLOCAL modsecurity_request_body_to_stream(modsec_rec *msr, const char *buffer, int buflen, char **error_msg);

apr_status_t DSOLOCAL modsecurity_request_body_retrieve_start(modsec_rec *msr, char **error_msg);

apr_status_t DSOLOCAL modsecurity_request_body_retrieve_end(modsec_rec *msr);

/* Retrieves up to nbytes bytes of the request body. Returns 1 on
 * success, 0 when there is no more data, or -1 on error. On return
 * nbytes will contain the number of bytes stored in the buffer.
 */
apr_status_t DSOLOCAL modsecurity_request_body_retrieve(modsec_rec *msr, msc_data_chunk **chunk,
    long int nbytes, char **error_msg);

void DSOLOCAL msc_add(modsec_rec *msr, int level, msre_actionset *actionset,
    const char *action_message, const char *rule_message);

const char DSOLOCAL *msc_alert_message(modsec_rec *msr, msre_actionset *actionset, const char *action_message,
    const char *rule_message);

void DSOLOCAL msc_alert(modsec_rec *msr, int level, msre_actionset *actionset, const char *action_message,
    const char *rule_message);

apr_status_t DSOLOCAL modsecurity_request_body_clear(modsec_rec *msr, char **error_msg);

#endif
