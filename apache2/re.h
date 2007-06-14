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
#ifndef _MSC_RE_H_
#define _MSC_RE_H_

#define ABSOLUTE_VALUE 0
#define POSITIVE_VALUE 1
#define NEGATIVE_VALUE 2

typedef struct msre_engine msre_engine;
typedef struct msre_ruleset msre_ruleset;
typedef struct msre_ruleset_internal msre_ruleset_internal;
typedef struct msre_rule msre_rule;
typedef struct msre_var_metadata msre_var_metadata;
typedef struct msre_var msre_var;
typedef struct msre_op_metadata msre_op_metadata;
typedef struct msre_tfn_metadata msre_tfn_metadata;
typedef struct msre_actionset msre_actionset;
typedef struct msre_action_metadata msre_action_metadata;
typedef struct msre_action msre_action;

#include "apr_general.h"
#include "apr_tables.h"
#include "modsecurity.h"
#include "msc_pcre.h"
#include "persist_dbm.h"
#include "apache2.h"


/* Actions, variables, functions and operator functions */

int DSOLOCAL expand_macros(modsec_rec *msr, msc_string *var, msre_rule *rule, apr_pool_t *mptmp);

apr_status_t msre_parse_targets(msre_ruleset *ruleset, const char *text,
    apr_array_header_t *arr, char **error_msg);

apr_status_t msre_parse_actions(msre_engine *engine, msre_actionset *actionset,
    const char *text, char **error_msg);

msre_var_metadata *msre_resolve_var(msre_engine *engine, const char *name);

msre_action_metadata *msre_resolve_action(msre_engine *engine, const char *name);

msre_var *msre_create_var(msre_ruleset *ruleset, const char *name, const char *param,
    modsec_rec *msr, char **error_msg);

msre_var *msre_create_var_ex(apr_pool_t *pool, msre_engine *engine, const char *name, const char *param,
    modsec_rec *msr, char **error_msg);

msre_action *msre_create_action(msre_engine *engine, const char *name,
    const char *param, char **error_msg);

int msre_parse_generic(apr_pool_t *pool, const char *text, apr_table_t *vartable,
    char **error_msg);

int DSOLOCAL rule_id_in_range(int ruleid, const char *range);


/* Structures with the corresponding functions */

struct msre_engine {
    apr_pool_t              *mp;
    apr_table_t             *variables;
    apr_table_t             *operators;
    apr_table_t             *actions;
    apr_table_t             *tfns;
};

msre_engine *msre_engine_create(apr_pool_t *parent_pool);

void msre_engine_destroy(msre_engine *engine);

msre_op_metadata *msre_engine_op_resolve(msre_engine *engine, const char *name);

struct msre_ruleset {
    apr_pool_t              *mp;
    msre_engine             *engine;
    
    apr_array_header_t      *phase_request_headers;
    apr_array_header_t      *phase_request_body;
    apr_array_header_t      *phase_response_headers;
    apr_array_header_t      *phase_response_body;
    apr_array_header_t      *phase_logging;
};

apr_status_t msre_ruleset_process_phase(msre_ruleset *ruleset, modsec_rec *msr);

apr_status_t msre_ruleset_process_phase_internal(msre_ruleset *ruleset, modsec_rec *msr);

msre_ruleset *msre_ruleset_create(msre_engine *engine, apr_pool_t *mp);

int msre_ruleset_rule_add(msre_ruleset *ruleset, msre_rule *rule, int phase);

int DSOLOCAL msre_ruleset_rule_remove_with_exception(msre_ruleset *ruleset, rule_exception *re);

/*
int msre_ruleset_phase_rule_remove_with_exception(msre_ruleset *ruleset, rule_exception *re,
    apr_array_header_t *phase_arr);
*/

#define RULE_NO_MATCH           0
#define RULE_MATCH              1

struct msre_rule {
    apr_array_header_t      *targets;
    const char              *op_name;
    const char              *op_param;
    void                    *op_param_data;
    msre_op_metadata        *op_metadata;
    unsigned int             op_negated;
    msre_actionset          *actionset;
    
    msre_ruleset            *ruleset;
    msre_rule               *chain_starter;
};

msre_rule *msre_rule_create(msre_ruleset *ruleset, const char *targets,
    const char *args, const char *actions, char **error_msg);

void msre_rule_actionset_init(msre_rule *rule);

apr_status_t msre_rule_process(msre_rule *rule, modsec_rec *msr);

#define VAR_SIMPLE              0    /* REQUEST_URI */
#define VAR_LIST                1

#define PHASE_REQUEST_HEADERS   1
#define PHASE_REQUEST_BODY      2
#define PHASE_RESPONSE_HEADERS  3
#define PHASE_RESPONSE_BODY     4
#define PHASE_LOGGING           5

#define FN_OP_PARAM_INIT(X) int (*X)(msre_rule *rule, char **error_msg)
#define FN_OP_EXECUTE(X)    int (*X)(modsec_rec *msr, msre_rule *rule, msre_var *var, char **error_msg)


struct msre_op_metadata {
    const char              *name;
    FN_OP_PARAM_INIT        (param_init);
    FN_OP_EXECUTE           (execute);
};

#define FN_TFN_EXECUTE(X)    int (*X)(apr_pool_t *pool, unsigned char *input, long int input_length, char **rval, long int *rval_length)

struct msre_tfn_metadata {
    const char              *name;

    /* Functions should populate *rval and return 1 on
     * success, or return -1 on failure (in which case *rval
     * should contain the error message. Strict functions
     * (those that validate in
     * addition to transforming) can return 0 when input
     * fails validation. Functions are free to perform
     * in-place transformation, or to allocate a new buffer
     * from the provideded temporary (per-rule) memory pool.
     *
     * NOTE Strict transformation functions not supported yet.
     */
    FN_TFN_EXECUTE(execute);    
};

void DSOLOCAL msre_engine_tfn_register(msre_engine *engine, const char *name,
    FN_TFN_EXECUTE(execute));

void msre_engine_op_register(msre_engine *engine, const char *name,
    FN_OP_PARAM_INIT(fn1), FN_OP_EXECUTE(fn2));

void DSOLOCAL msre_engine_register_default_tfns(msre_engine *engine);

void DSOLOCAL msre_engine_register_default_variables(msre_engine *engine);

void DSOLOCAL msre_engine_register_default_operators(msre_engine *engine);

void DSOLOCAL msre_engine_register_default_actions(msre_engine *engine);

msre_tfn_metadata DSOLOCAL *msre_engine_tfn_resolve(msre_engine *engine, const char *name);

#define VAR_DONT_CACHE  0
#define VAR_CACHE       1

#define FN_VAR_VALIDATE(X)  char *(*X)(msre_ruleset *ruleset, msre_var *var)
#define FN_VAR_GENERATE(X)  int (*X)(modsec_rec *msr, msre_var *var, msre_rule *rule, apr_table_t *table, apr_pool_t *mptmp)

struct msre_var_metadata {
    const char              *name;
    unsigned int             type;          /* VAR_TYPE_ constants */
    unsigned int             argc_min;
    unsigned int             argc_max;
    FN_VAR_VALIDATE          (validate);
    FN_VAR_GENERATE          (generate);
    unsigned int             is_cacheable;  /* 0 - no, 1 - yes */
    unsigned int             availability;  /* when does this variable become available? */
};

struct msre_var {
    const char              *name;
    const char              *value;
    unsigned int             value_len;
    const char              *param;
    const void              *param_data;
    msre_var_metadata       *metadata;
    msc_regex_t             *param_regex;
    unsigned int             is_negated;
    unsigned int             is_counting;
};


struct msre_actionset {
    apr_table_t             *actions;

    /* Metadata */
    const char              *id;
    const char              *rev;
    const char              *msg;
    int                      severity;
    int                      phase;

    /* Flow */
    int                      is_chained;
    int                      skip_count;

    /* Disruptive */
    int                      intercept_action;
    const char              *intercept_uri;
    int                      intercept_status;
    int                      intercept_pause;

    /* Other */
    int                      log;
    int                      auditlog;
};

msre_actionset *msre_actionset_create(msre_engine *engine, const char *text,
    char **error_msg);

msre_actionset *msre_actionset_merge(msre_engine *engine, msre_actionset *parent,
    msre_actionset *child, int inherit_by_default);

msre_actionset *msre_actionset_create_default(msre_engine *engine);

void msre_actionset_init(msre_actionset *actionset, msre_rule *rule);

#define FN_ACTION_VALIDATE(X)   char *(*X)(msre_engine *engine, msre_action *action)
#define FN_ACTION_INIT(X)       apr_status_t (*X)(msre_engine *engine, msre_actionset *actionset, msre_action *action)
#define FN_ACTION_EXECUTE(X)    apr_status_t (*X)(modsec_rec *msr, apr_pool_t *mptmp, msre_rule *rule, msre_action *action)

#define ACTION_DISRUPTIVE       1
#define ACTION_NON_DISRUPTIVE   2
#define ACTION_METADATA         3
#define ACTION_FLOW             4

#define NO_PLUS_MINUS           0
#define ALLOW_PLUS_MINUS        1

#define ACTION_CARDINALITY_ONE  1
#define ACTION_CARDINALITY_MANY 2

struct msre_action_metadata {
    const char              *name;
    unsigned int             type;
    unsigned int             argc_min;
    unsigned int             argc_max;
    unsigned int             allow_param_plusminus;
    unsigned int             cardinality;
    FN_ACTION_VALIDATE       (validate);
    FN_ACTION_INIT           (init);
    FN_ACTION_EXECUTE        (execute);
};

struct msre_action {
    msre_action_metadata    *metadata;
    const char              *param;
    const void              *param_data;
    unsigned int             param_plusminus; /* ABSOLUTE_VALUE, POSITIVE_VALUE, NEGATIVE_VALUE */
};

/* -- MSRE Function Prototypes ---------------------------------------------- */

msre_var_metadata *msre_resolve_var(msre_engine *engine, const char *name);

int msre_parse_generic(apr_pool_t *pool, const char *text, apr_table_t *vartable,
    char **error_msg);

apr_status_t msre_parse_vars(msre_ruleset *ruleset, const char *text,
    apr_array_header_t *arr, char **error_msg);

char DSOLOCAL *msre_format_metadata(modsec_rec *msr, msre_actionset *actionset);

#endif
