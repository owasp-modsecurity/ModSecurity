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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include <sys/param.h>

#include "alp2.h"

#ifdef DEBUG
#define alp_debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define alp_debug(...)
#endif /* DEBUG */

/**
 * Add one error to the audit log entry.
 */
static void add_error(alp2_t *alp, int is_fatal, const char *text, ...)
{
    char *str = NULL;
    va_list ap;

    if (is_fatal) {
        alp->parse_error = 1;
    }

    va_start(ap, text);
    str = apr_pvsprintf(alp->auditlog->mp, text, ap);
    va_end(ap);

    *(char **)apr_array_push(alp->errors) = str;
}

/**
 * Parse the Response-Body-Transformed trailer header.
 */
static int handle_part_H_parse_ResponseTFN(alp2_t *alp, const char *s)
{
    char *capture = NULL;
    int ovector[33];
    int rc;

    // TODO This header is optional, but is not allowed to appear more than once.


    return 1;
}

/**
 * Parse the Action trailer header.
 */
static int handle_part_H_parse_Action(alp2_t *alp, const char *s)
{
    char *capture = NULL;
    int ovector[33];
    int rc;

    // TODO This header is optional, but is not allowed to appear more than once.

    alp->auditlog->was_intercepted = 1;

    rc = pcre_exec(alp->trailer_action_pattern, NULL, s, strlen(s), 0, 0, ovector, 30);
    if (rc < 0) {
        add_error(alp, 1, "Part H: Failed to parse Action header");
        return -1;
    }

    capture = apr_pstrmemdup(alp->auditlog->mp, s + ovector[2 * 1],
        ovector[2 * 1 + 1] - ovector[2 * 1]);

    alp->auditlog->intercept_phase = atoi(capture);

    return 1;
}

/**
 * Convert two hexadecimal characters into a character.
 */
static uint8_t x2c(uint8_t *what)
{
    register uint8_t digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A') + 10 : (what[1] - '0'));

    return digit;
}

/**
 * Remove a layer of encoding from a string. This function needs to be used
 * for every piece of data ModSecurity encoded for a message.
 */
static int remove_slashes(uint8_t *s)
{
    uint8_t *d = s;

    while(*s != '\0') {
        if ((*s == '\\')&&(*(s + 1) != '\0')) {
            s++;

            switch(*s) {
                case 'b' :
                    *d = '\b';
                    break;
                case 'n' :
                    *d = '\n';
                    break;
                case 'r' :
                    *d = '\r';
                    break;
                case 't' :
                    *d = '\t';
                    break;
                case 'v' :
                    *d = '\v';
                    break;
                case '\\' :
                    *d = '\\';
                    break;
                case '"' :
                    *d = '"';
                    break;
                case 'x' :
                    if (  (*(s + 1) != '\0')
                        &&(*(s + 2) != '\0')
                        &&(isxdigit(*(s + 1)))
                        &&(isxdigit(*(s + 2)))
                    ) {
                        *d = x2c(s + 1);
                        s += 2;
                    }
                    else {
                        /* Invalid encoding. */
                        return -1;
                    }
                    break;
                default :
                    /* Invalid encoding. */
                    return -1;
                    break;
            }
        }
        else {
            *d = *s;
        }

        s++;
        d++;
    }

    *d = '\0';

    return 1;
}

/**
 * Process one (ModSecurity message) meta-data fragment.
 */
static int handle_part_H_parse_Message_meta(alp2_t *alp, alp2_msg_t *message,
    const char *l, const char *string_start, const char *string_end)
{
    const char *value;

    // XXX if ((*string_start != '"')||(*string_end != '"')||(string_end <= string_start)) {
    if (string_end <= string_start) {
        add_error(alp, 1, "Part H: Invalid handle_part_H_parse_Message_meta invocation");
        return -1;
    }

    if ((*string_start != '"')||(*string_end != '"')) {
        value = apr_pstrndup(alp->auditlog->mp, string_start, (string_end - string_start) + 1);
    }
    else {
        value = apr_pstrndup(alp->auditlog->mp, string_start + 1, (string_end - string_start - 1));
    }
    if (value == NULL) {
        return -1;
    }

    if (remove_slashes((uint8_t *)value) < 0) {
        add_error(alp, 1, "Part H: Invalid encoding in meta-data fragment");
        return -1;
    }

    /* Target ( at THE_TARGET) */
    if (strncmp(l, "at ", 3) == 0) {
        if (message->target != NULL) {
            add_error(alp, 1, "Part H: Already seen target");
            return -1;
        }

        message->target = value;

        return 1;
    }

    /* id */
    if (strncmp(l, "id ", 3) == 0) {
        if (message->id != NULL) {
            add_error(alp, 1, "Part H: Already seen meta-data: id");
            return -1;
        }

        message->id = value;

        return 1;
    }

    /* rev */
    if (strncmp(l, "rev ", 4) == 0) {
        if (message->rev != NULL) {
            add_error(alp, 1, "Part H: Already seen meta-data: rev");
            return -1;
        }

        message->rev = value;

        return 1;
    }

    /* msg */
    if (strncmp(l, "msg ", 4) == 0) {
        if (message->msg != NULL) {
            add_error(alp, 1, "Part H: Already seen meta-data: msg");
            return -1;
        }

        message->msg = value;

        return 1;
    }

    /* data */
    if (strncmp(l, "data ", 4) == 0) {
        if (message->data != NULL) {
            add_error(alp, 1, "Part H: Already seen meta-data: data");
            return -1;
        }

        message->data = value;

        return 1;
    }

    /* file */
    if (strncmp(l, "file ", 5) == 0) {
        if (message->file != NULL) {
            add_error(alp, 1, "Part H: Already seen meta-data: file");
            return -1;
        }

        message->file = value;

        return 1;
    }

    /* line */
    if (strncmp(l, "line ", 5) == 0) {
        if (message->file_line != (unsigned long)-1) {
            add_error(alp, 1, "Part H: Already seen meta-data: line");
            return -1;
        }

        // TODO Validate.
        message->file_line = atoi(value);

        return 1;
    }

    /* tag */
    if (strncmp(l, "tag ", 4) == 0) {
        *(char **)apr_array_push(message->tags) = (char *)value;
        return 1;
    }

    /* severity */
    if (strncmp(l, "severity ", 9) == 0) {
        if (  (strcmp(value, "0") == 0)
            ||(strcasecmp(value, "EMERGENCY") == 0))
        {
            message->severity = 0;
            return 1;
        }

        if (  (strcmp(value, "1") == 0)
            ||(strcasecmp(value, "ALERT") == 0))
        {
            message->severity = 1;
            return 1;
        }

        if (  (strcmp(value, "2") == 0)
            ||(strcasecmp(value, "CRITICAL") == 0))
        {
            message->severity = 2;
            return 1;
        }

        if (  (strcmp(value, "3") == 0)
            ||(strcasecmp(value, "ERROR") == 0))
        {
            message->severity = 3;
            return 1;
        }

        if (  (strcmp(value, "4") == 0)
            ||(strcasecmp(value, "WARNING") == 0))
        {
            message->severity = 4;
            return 1;
        }

        if (  (strcmp(value, "5") == 0)
            ||(strcasecmp(value, "NOTICE") == 0))
        {
            message->severity = 5;
            return 1;
        }

        if (  (strcmp(value, "6") == 0)
            ||(strcasecmp(value, "INFO") == 0))
        {
            message->severity = 6;
            return 1;
        }

        if (  (strcmp(value, "7") == 0)
            ||(strcasecmp(value, "DEBUG") == 0))
        {
            message->severity = 7;
            return 1;
        }

        add_error(alp, 1, "Part H: Invalid severity value: %s", value);
        
        return -1;
    }

    /* offset */
    if (strncmp(l, "offset ", 7) == 0) {
        if (message->offset != (size_t)-1) {
            /* Already seen "offset". */
            add_error(alp, 1, "Part H: Already seen fragment offset");
            return -1;
        }

        // TODO Validate.
        message->offset = atoi(value);

        return 1;
    }

    /* Ignore unknown meta-data information. */
    
    return 0;
}

/**
 * Parse the Message trailer header. More than one such header
 * can exist in an audit log, and each represents one ModSecurity
 * message.
 */
static int handle_part_H_parse_Message(alp2_t *alp, const char *s)
{
    alp2_msg_t *message = NULL;
    char *l = (char *)(s + strlen(s) - 1);
    char *engine_message_start = (char *)s;
    char *engine_message_end = NULL;
    char *string_start = NULL, *string_end = NULL;
    char *fragment_end = NULL;
    char *tptr;
    int in_string;
    int done;

    /* Create one new message structure. */
    message = apr_pcalloc(alp->auditlog->mp, sizeof(alp2_msg_t));
    if (message == NULL) {
        return -1;
    }

    message->file_line = (unsigned long)-1;
    message->offset = (size_t)-1;
    message->severity = -1;
    message->warning = 0;

    if (strncasecmp("warning. ", s, 9) == 0) {
        message->warning = 1;
        engine_message_start += 9;
    }

    message->tags = apr_array_make(alp->auditlog->mp, 4, sizeof(const char *));

    /* Start at the end of the message and go back identifying
     * the meta-data fragments as we go. Stop when we find the
     * end of the engine message.
     */
    done = in_string = 0;
    while ((l >= s)&&(!done)) {
        if (in_string == 0) {
            /* Outside string. */

            // TODO Make sure this is not an escaped char
            switch(*l) {
                case ' ' :
                    /* Do nothing. */
                    break;
                case ']' :
                    fragment_end = l;
                    break;
                case '[' :
                    if (fragment_end) {
                        /* Found one meta-data fragment. */
                        // TODO This parser implementation allows for invalid
                        //      meta-data fragments to be accepted. It would be
                        //      nice to check the format of the fragment (e.g.
                        //      by matching it against a regular expression
                        //      pattern) before we accept any data. At this point
                        //      l points to the first byte of the fragment, and
                        //      fragment_end to the last.
                        handle_part_H_parse_Message_meta(alp, message,
                            l + 1, string_start, string_end);

                        fragment_end = NULL;
                        string_start = NULL;
                        string_end = NULL;
                    }
                    break;
                case '"' :
                    /* Found the end of a string. */
                    in_string = 1;
                    string_end = l;
                    break;
                default :
                    if (!fragment_end) {
                        /* There are no more meta-data fragments. */
                        engine_message_end = l;
                        done = 1;
                    }
                    break;
            }
        }
        else {
            /* In string. We are only interested
             * in where the string ends.
             */
            if ((*l == '"')&&((l - 1) >= s)&&(*(l - 1) != '\\')) {
                in_string = 0;
                string_start = l;
            }
        }
    
        l--;
    }

    /* Target is between " at " and "." */
    tptr = engine_message_start;
    while ((tptr = strstr(tptr, " at ")) && (tptr < engine_message_end)) {
        char *tend = strchr(tptr, '.');
        if ((tend <= engine_message_end) && (tend - tptr > 5)) {
            int rc = handle_part_H_parse_Message_meta(alp, message, tptr + 1,
                                                      tptr + 4, tend - 1);
            if (rc == 1) {
                /* Remove the target data from the message */
                engine_message_end = tptr;
            }
        }
        break;
    }

    if (engine_message_end == NULL) {
        add_error(alp, 1, "Part H: Failed parsing ModSecurity message: %s", s);
        return -1;
    }

    message->engine_message = apr_pstrndup(alp->auditlog->mp, engine_message_start, (engine_message_end - engine_message_start + 1));

    /* Add this message to the audit log. */
    *(alp2_msg_t **)apr_array_push(alp->auditlog->messages) = message;

    return 1;
}

/**
 * Parse the Stopwatch trailer header.
 */
static int handle_part_H_parse_Stopwatch(alp2_t *alp, const char *s)
{
    int ovector[33];
    int i, rc;

    // TODO This header is required (a check for its appearance is made when
    //      handling the end of an H part), and is not allowed to appear
    //      more than once.

    rc = pcre_exec(alp->trailer_stopwatch_pattern, NULL, s, strlen(s), 0, 0, ovector, 30);
    if (rc < 0) {
        add_error(alp, 1, "Part H: Failed to parse Stopwatch header");
        return -1;
    }

    /* Loop through the captures. */
    for (i = 0; i < rc; i++) {
        char *capture = apr_pstrmemdup(alp->auditlog->mp, s + ovector[2 * i],
            ovector[2 * i + 1] - ovector[2 * i]);

        switch (i) {
            case 1 : /* timestamp */
                // TODO Validate
                alp->auditlog->timestamp = apr_atoi64(capture);
                break;
            case 2 : /* duration */
                // TODO Validate
                alp->auditlog->duration = apr_atoi64(capture);
                break;
            case 3 : /* ignore (group of three further time elements)*/
                break;
            case 4 : /* t1 */
                break;
            case 5 : /* t2 */
                break;
            case 6 : /* t3 */
                break;
        }
    }

    return 1;
}

/**
 * Parse the WebApp-Info trailer header.
 */
static int handle_part_H_parse_WebAppInfo(alp2_t *alp, const char *s)
{
    int ovector[33];
    int i, rc;

    // TODO This header is optional, but it is not allowed to appear more than once.

    rc = pcre_exec(alp->trailer_webappinfo_pattern, NULL, s, strlen(s), 0, 0, ovector, 30);
    if (rc < 0) {
        add_error(alp, 1, "Part H: Failed to parse WebApp-Info header");
        return -1;
    }

    /* Loop through the captures. */
    for (i = 0; i < rc; i++) {
        char *capture = apr_pstrmemdup(alp->auditlog->mp, s + ovector[2 * i],
            ovector[2 * i + 1] - ovector[2 * i]);

        switch (i) {
            case 1 : /* application ID */
                // TODO Validate
                alp->auditlog->application_id = capture;
                break;
            case 2 : /* session ID */
                // TODO Validate
                if (strcmp(capture, "-") != 0) {
                    alp->auditlog->session_id = capture;
                }
                break;
            case 3 : /* user ID */
                // TODO Validate
                if (strcmp(capture, "-") != 0) {
                    alp->auditlog->user_id = capture;
                }
                break;
        }
    }

    return 1;
}

/**
 * Handle part H events.
 */
static void handle_part_H(alp2_t *alp, int event_type)
{
    /* Part data. */
    if (event_type == ALP2_EVENT_PART_DATA) {
        char *line = alp2_pp_line_chomp(alp->pp);

        /* This part ends with an empty line. */
        if (strlen(line) == 0) {
            alp->part_data_done = 1;
            return;
        }

        /* Extract the header information. */
        {
            char *name = NULL, *value = NULL;
            int ovector[33];
            int i, rc;

            /* Header line. */

            /* Extract the fields. */
            rc = pcre_exec(alp->header_pattern, NULL, line, strlen(line), 0, 0, ovector, 30);
            if (rc < 0) {
                add_error(alp, 1, "Part H: Failed to parse header: %i", rc);
                return;
            }

            /* Loop through the captures. */
            for (i = 0; i < rc; i++) {
                char *capture = apr_pstrmemdup(alp->auditlog->mp, line + ovector[2 * i],
                    ovector[2 * i + 1] - ovector[2 * i]);

                switch(i) {
                    case 1 :
                        name = capture;
                        break;
                    case 2 :
                        value = capture;
                        break;
                }
            }

            /* Add header to the table. */
            apr_table_addn(alp->auditlog->trailer_headers, name, value);
        }

        return;
    }

    /* Part end. */
    if (event_type == ALP2_EVENT_PART_END) {
        const apr_array_header_t *tarr = apr_table_elts(alp->auditlog->trailer_headers);
        apr_table_entry_t *te = NULL;
        const char *s = NULL;
        int stopwatch = 0;
        int rc = 0;
        int i;

        if ((tarr == NULL) || (tarr->nelts == 0)) {
            return;
        }
        
        /* Here we are going to extract certain headers and
         * parse them to populate the corresponding fields in
         * the auditlog structure.
         */

        te = (apr_table_entry_t *)tarr->elts;
        for (i = 0; i < tarr->nelts; i++) {
            const char *key = te[i].key;
            const char *val = te[i].val;

            if ((key == NULL) || (val == NULL)) {
                continue;
            }

            /* Action: optional */
            else if (strcmp("Action", key) == 0) {
                rc = handle_part_H_parse_Action(alp, val);
            }
        
            /* Message: optional */
            else if (strcmp("Message", key) == 0) {
                rc = handle_part_H_parse_Message(alp, val);
            }

            /* Apache-Handler: optional */
            else if (strcmp("Apache-Handler", key) == 0) {
                rc = 0;
                // TODO Only one allowed
                alp->auditlog->handler = apr_pstrdup(alp->auditlog->mp, val);
            }

            /* Producer: optional */
            else if (strcmp("Producer", key) == 0) {
                rc = 0;
                // TODO Only one allowed
                alp->auditlog->producer = apr_pstrdup(alp->auditlog->mp, val);
            }

            /* Server: optional */
            else if (strcmp("Server", key) == 0) {
                rc = 0;
                // TODO Only one allowed
                alp->auditlog->server = apr_pstrdup(alp->auditlog->mp, val);
            }

            /* Response-Body-Transformed: optional */
            else if (strcmp("Response-Body-Transformed", key) == 0) {
                rc = 0;
                // TODO Only one allowed
                alp->auditlog->response_tfn = apr_pstrdup(alp->auditlog->mp, val);
            }

            /* Stopwatch: required */
            else if (strcmp("Stopwatch", key) == 0) {
                stopwatch = 1;
                rc = handle_part_H_parse_Stopwatch(alp, val);
            }

            /* WebApp-Info: optional */
            else if (strcmp("WebApp-Info", key) == 0) {
                rc = handle_part_H_parse_WebAppInfo(alp, val);
            }

            if (rc < 0) {
                /* No need to report anything, it's already been reported. */
            }
        }

        if (stopwatch == 0) {
            add_error(alp, 1, "Part H: Stopwatch header missing");
        }

        return;
    }
}

/**
 * Handle part F events.
 */
static void handle_part_F(alp2_t *alp, int event_type)
{
    /* Part data. */
    if (event_type == ALP2_EVENT_PART_DATA) {
        char *line = alp2_pp_line_chomp(alp->pp);

        /* This part ends with an empty line. */
        if (strlen(line) == 0) {
            alp->part_data_done = 1;
            return;
        }

        /* The first line should be the response line. */
        if (alp->part_line_counter == 1) {
            int ovector[33];
            int i, rc;

            /* Response line. */

            /* Extract the fields. */
            rc = pcre_exec(alp->response_line_pattern, NULL, line, strlen(line), 0, 0, ovector, 30);
            if (rc < 0) {
                add_error(alp, 1, "Part F: Failed to parse response line: %i", rc);
                return;
            }

            /* Loop through the captures. */
            for (i = 0; i < rc; i++) {
                char *capture = apr_pstrmemdup(alp->auditlog->mp, line + ovector[2 * i],
                    ovector[2 * i + 1] - ovector[2 * i]);

                switch(i) {
                    case 1 :
                        alp->auditlog->response_protocol = capture;
                        break;
                    case 2 :
                        alp->auditlog->response_status = atoi(capture);
                        break;
                    case 4 :
                        alp->auditlog->response_message = capture;
                        break;
                        break;
                }
            }
        }
        else {
            char *name = NULL, *value = NULL;
            int ovector[33];
            int i, rc;

            /* Response header line. */

            /* Extract the fields. */
            rc = pcre_exec(alp->header_pattern, NULL, line, strlen(line), 0, 0, ovector, 30);
            if (rc < 0) {
                add_error(alp, 1, "Part F: Failed to parse response header: %i", rc);
                return;
            }

            /* Loop through the captures. */
            for (i = 0; i < rc; i++) {
                char *capture = apr_pstrmemdup(alp->auditlog->mp, line + ovector[2 * i],
                    ovector[2 * i + 1] - ovector[2 * i]);

                switch(i) {
                    case 1 :
                        name = capture;
                        break;
                    case 2 :
                        value = capture;
                        break;
                }
            }

            /* Add header to the table. */
            apr_table_addn(alp->auditlog->response_headers, name, value);
        }

        return;
    }

    /* Part end. */
    if (event_type == ALP2_EVENT_PART_END) {
        /* If any of the response headers need
         * special handling, place the code here.
         */
        return;
    }
}

/**
 * Parse the URI. APR-Util does most of the work here.
 */
static int handle_part_B_parse_uri(alp2_t *alp)
{
    char *u = (char *)alp->auditlog->request_uri;
    apr_uri_t *uri = NULL;

    if ((  alp->auditlog->request_method == NULL)
        ||(alp->auditlog->request_uri == NULL))
    {
        return 0;
    }

    /* Since this is not a proper URI but a path, handle
     * the leading double slash.
     */
    while ((u[0] == '/') && (u[1] == '/')) {
        u++;
    }

    uri = apr_pcalloc(alp->auditlog->mp, sizeof(apr_uri_t));    

    if (strcasecmp(alp->auditlog->request_method, "CONNECT") == 0) {
        if (apr_uri_parse_hostinfo(alp->auditlog->mp, u, uri) != APR_SUCCESS) {
            add_error(alp, 0, "Info: Failed to parse request URI (hostinfo)");
            return -1;
        }
    }
    else {
        if (apr_uri_parse(alp->auditlog->mp, u, uri) != APR_SUCCESS) {
            add_error(alp, 0, "Info: Failed to parse request URI");
            return -1;
        }
    }

    alp->auditlog->parsed_uri = uri;

    return 1;
}

/**
 * Handle part B events.
 */
static void handle_part_B(alp2_t *alp, int event_type)
{
    /* Part data. */
    if (event_type == ALP2_EVENT_PART_DATA) {
        char *line = alp2_pp_line_chomp(alp->pp);

        /* This part ends with an empty line. */
        if (strlen(line) == 0) {
            alp->part_data_done = 1;
            return;
        }

        /* The first line should be the request line. */
        if (alp->part_line_counter == 1) {
            int ovector[33];
            int i, rc;

            /* Request line. */

            /* Extract the fields. */
            rc = pcre_exec(alp->request_line_pattern, NULL, line, strlen(line), 0, 0, ovector, 30);
            if (rc < 0) {
                add_error(alp, 1, "Part B: Failed to parse request line: %i", rc);
                return;
            }

            alp->auditlog->request_line_valid = 1;

            /* Loop through the captures. */
            for (i = 0; i < rc; i++) {
                char *capture = apr_pstrmemdup(alp->auditlog->mp, line + ovector[2 * i],
                    ovector[2 * i + 1] - ovector[2 * i]);

                switch(i) {
                    case 0 :
                        alp->auditlog->request_line = capture;
                        break;
                    case 1 :
                        alp->auditlog->request_method = capture;
                        break;
                    case 2 :
                        alp->auditlog->request_uri = capture;
                        if (handle_part_B_parse_uri(alp) != 1) {
                            // TODO Do we want to do anything on error?
                        }
                        break;
                    case 3 :
                        alp->auditlog->request_protocol = capture;
                        break;
                }
            }
        }
        else {
            char *name = NULL, *value = NULL;
            int ovector[33];
            int i, rc;

            /* Header line. */

            /* Extract the fields. */
            rc = pcre_exec(alp->header_pattern, NULL, line, strlen(line), 0, 0, ovector, 30);
            if (rc < 0) {
                add_error(alp, 1, "Part B: Failed to parse request header: %i", rc);
                return;
            }

            /* Loop through the captures. */
            for (i = 0; i < rc; i++) {
                char *capture = apr_pstrmemdup(alp->auditlog->mp, line + ovector[2 * i],
                    ovector[2 * i + 1] - ovector[2 * i]);

                switch(i) {
                    case 1 :
                        name = capture;
                        break;
                    case 2 :
                        value = capture;
                        break;
                }
            }

            /* ModSecurity 1.9.x adds some requests headers of
             * its own, and we don't want them.
             */
            if (strncmp(name, "mod_security-", 13) != 0) {
                /* Add header to the table. */
                apr_table_addn(alp->auditlog->request_headers, name, value);
            }
        }

        return;
    }

    /* Part end. */
    if (event_type == ALP2_EVENT_PART_END) {
        /* Determine hostname. */

        // TODO I think the right thing to do is use the port numbers
        //      only when the host itself is a numerical IP.

        /* Try the URI first. */
        if (  (alp->auditlog->parsed_uri != NULL)
            &&(alp->auditlog->parsed_uri->hostname != NULL))
        {
            if (   (alp->auditlog->parsed_uri->port != 0)
                && (alp->auditlog->parsed_uri->port != 80)
                && (alp->auditlog->parsed_uri->port != 443) )
            {
                // TODO Do not use the port number if the hostname
                //      is not numeric.
                alp->auditlog->hostname = apr_psprintf(alp->auditlog->mp, "%s:%i", 
                    alp->auditlog->parsed_uri->hostname,  alp->auditlog->parsed_uri->port);
            }
            else {
                // TODO Always use the port number if the hostname
                //      is numeric.
                alp->auditlog->hostname = alp->auditlog->parsed_uri->hostname;
            }
        }
        else {
            /* Try the Host header. */
            char *s = (char *)apr_table_get(alp->auditlog->request_headers, "Host");
            if (s != NULL) {
                // TODO If the hostname is not numeric, remove the port
                //      numbers if present.
                alp->auditlog->hostname = s;
            }
            else {
                /* Use the destination IP and port. */
                alp->auditlog->hostname = apr_psprintf(alp->auditlog->mp, "%s:%i",
                alp->auditlog->dst_ip, alp->auditlog->dst_port);
            }
        }

        return;
    }
}

/**
 * Handle part A events.
 */
static void handle_part_A(alp2_t *alp, int event_type)
{
    /* Part data. */
    if (event_type == ALP2_EVENT_PART_DATA) {
        char *line = alp2_pp_line_chomp(alp->pp);
        int ovector[33];
        int i, rc;

        /* This part can have only one line,
         * so we don't expect to be here again.
         */
        alp->part_data_done = 1;

        /* Extract the fields. */
        rc = pcre_exec(alp->part_a_pattern, NULL, line, strlen(line), 0, 0, ovector, 30);
        if (rc < 0) {
            add_error(alp, 1, "Part A: Parsing failed: %i", rc);
            return;
        }

        /* Loop through the captures. */
        for (i = 0; i < rc; i++) {
            char *capture = apr_pstrmemdup(alp->auditlog->mp, line + ovector[2 * i],
                ovector[2 * i + 1] - ovector[2 * i]);

            switch(i) {
                case 1 : /* timestamp in Apache format */
                    /* We don't need it as we use the one from the H part. */
                    break;
                case 2 : /* transaction ID */
                    alp->auditlog->id = capture;
                    break;
                case 3 : /* source address */
                    // TODO Validate
                    alp->auditlog->src_ip = capture;
                    break;
                case 4 : /* source port */
                    // TODO Validate
                    alp->auditlog->src_port = atoi(capture);
                    break;
                case 5 : /* destination address */
                    // TODO Validate
                    alp->auditlog->dst_ip = capture;
                    break;
                case 6 : /* destinatio port */
                    // TODO Validate
                    alp->auditlog->dst_port = atoi(capture);
                    break;
            }
        }

        return;
    }

    /* Part end. */
    if (event_type == ALP2_EVENT_PART_END) {
        /* Place part post-validation here. */
        return;
    }
}

/**
 * Create a new audit log data structure, allocating
 * memory from the provided memory pool.
 */
auditlog2_t *alp2_auditlog_create(apr_pool_t *mp)
{
    auditlog2_t *al;

    /* Create a new memory pool and the
     * auditlog structure in it. We will use the
     * parent pool of the parser pool, in order to
     * ensure the auditlog memory structures survive
     * the death of the parser.
     */
    al = apr_pcalloc(mp, sizeof(auditlog2_t));
    al->mp = mp;
    

    al->request_headers = apr_table_make(al->mp, 20);
    al->response_headers = apr_table_make(al->mp, 20);
    al->trailer_headers = apr_table_make(al->mp, 20);
    al->messages = apr_array_make(al->mp, 10, sizeof(const alp2_msg_t *));
    al->intercept_phase = -1;

    return al;
}

/**
 * Destroy the provided audit log entry.
 */
void alp2_auditlog_destroy(auditlog2_t *al)
{
    apr_pool_destroy(al->mp);
}

/**
 * Handle ALP2_EVENT_ENTRY_START.
 */
static void handle_entry_start(alp2_t *alp)
{
    /* Create a new data structure to hold the entry. */
    alp->auditlog = alp2_auditlog_create(alp->pp->current_entry->mp);
    alp->auditlog->pp_entry = alp->pp->current_entry;

    /* Reset entry flags. */
    alp->previous_part_id = 0;
    alp->seen_part_h = 0;    
    alp->parse_error = 0;
    alp->errors = apr_array_make(alp->auditlog->mp, 4, sizeof(const char *));
}

/**
 * Handle ALP2_EVENT_ENTRY_END.
 */
static void handle_entry_end(alp2_t *alp)
{
    if (alp->parse_error) {
        /* No need to validate the entry since we've
         * previously encountered a problem with it.
         */
    }
    else {
        /* Final entry validation. */

        /* Have we seen the H part? (We must have seen the A
         * part, otherwise the entry would have begain in
         * the first place.
         */
        if (alp->seen_part_h == 0) {
            add_error(alp, 1, "Entry does not have part H.");
        }
    }

    /* Invoke the upstream callback to handle the entry. */
    if (alp->user_callback(alp) == 0) {
        alp->done = 1;
    }

    /* Upstream owns the audit log entry now. */
    alp->auditlog = NULL;
}

/**
 * Handle ALP2_EVENT_PART_START.
 */
static void handle_part_start(alp2_t *alp)
{
    if (alp->parse_error) {
        return;
    }

    /* Reset part flags. */
    alp->part_line_counter = 0;
    alp->part_data_done = 0;

    /* Is this part allowed/expected? */
    if (alp->previous_part_id == 0) {
        if (alp->pp->current_part->id != 'A') {
            add_error(alp, 1, "Expected part A but got %c.", alp->pp->current_part->id);
            return;
        }
    }

    /* Invoke the appropriate part handler. */
    switch(alp->pp->current_part->id) {
        case 'A' :
            handle_part_A(alp, ALP2_EVENT_PART_START);
            break;
        case 'B' :
            handle_part_B(alp, ALP2_EVENT_PART_START);
            break;
        case 'F' :
            handle_part_F(alp, ALP2_EVENT_PART_START);
            break;
        case 'H' :
            alp->seen_part_h = 1;
            handle_part_H(alp, ALP2_EVENT_PART_START);
            break;
        default :
            /* Ignore unknown part. */
            break;
    }
}

/*
 * Handle ALP2_EVENT_PART_END.
 */
static void handle_part_end(alp2_t *alp)
{
    if (alp->parse_error) {
        return;
    }

    /* Invoke the appropriate part handler. */
    switch(alp->pp->current_part->id) {
        case 'A' :
            handle_part_A(alp, ALP2_EVENT_PART_END);
            break;
        case 'B' :
            handle_part_B(alp, ALP2_EVENT_PART_END);
        case 'F' :
            handle_part_F(alp, ALP2_EVENT_PART_END);
            break;
        case 'H' :
            handle_part_H(alp, ALP2_EVENT_PART_END);
            break;
        default :
            /* Ignore unknown part. */
            break;
    }

    /* Remember the last part processed. */
    alp->previous_part_id = alp->pp->current_part->id;
}

/*
 * Handle ALP2_EVENT_PART_DATA.
 */
static void handle_part_data(alp2_t *alp)
{
    if (alp->parse_error) {
        return;
    }

    alp->part_line_counter++;

    if (alp->part_data_done) {
        add_error(alp, 1, "Unexpected data for part %c.", alp->pp->current_part->id);
        return;
    }

    /* Invoke the appropriate part handler. */
    switch(alp->pp->current_part->id) {
        case 'A' :
            handle_part_A(alp, ALP2_EVENT_PART_DATA);
            break;
        case 'B' :
            handle_part_B(alp, ALP2_EVENT_PART_DATA);
            break;
        case 'F' :
            handle_part_F(alp, ALP2_EVENT_PART_DATA);
            break;
        case 'H' :
            handle_part_H(alp, ALP2_EVENT_PART_DATA);
            break;
        default :
            /* Ignore unknown part. */
            break;
    }
}

/**
 * This function handles callbacks from
 * the lower-level (part) parser.
 */
static int alp2_callback(alp2_pp_t *pp, int event_type)
{
    alp2_t *alp = (alp2_t *)pp->user_data;

    /* Choose where to dispatch the event based
     * on the event type.
     */
    switch(event_type) {
        case ALP2_EVENT_ENTRY_START :
            handle_entry_start(alp);
            break;
        case ALP2_EVENT_ENTRY_END :
            handle_entry_end(alp);
            break;
        case ALP2_EVENT_PART_START :
            handle_part_start(alp);
            break;
        case ALP2_EVENT_PART_END :
            handle_part_end(alp);
            break;
        case ALP2_EVENT_PART_DATA :
            handle_part_data(alp);
            break;
        default :
            /* Unexpected event type. */
            break;
    }

    if (alp->done) {
        /* Stop parsing. */
        return 0; 
    }
    else {
        /* Go on. */
        return 1; 
    }
}

/**
 * Initialise parser.
 */
// XXX Make callback a typedef
int alp2_create(alp2_t **_alp, apr_pool_t *mp,
                void *user_data, int (*user_callback)(alp2_t *alp))
{
    alp2_t *alp;
    apr_pool_t *new_pool;
    const char *errptr = NULL;
    int erroffset;

    /* We require a callback. */
    if (user_callback == NULL) {
        return -1;
    }

    /* We will use our own memory pool. */
    apr_pool_create(&new_pool, mp);
    alp = apr_pcalloc(mp, sizeof(alp2_t));
    *_alp = alp;
    alp->mp = new_pool;

    alp->user_data = user_data;
    alp->user_callback = user_callback;

    /* Initialise the part parser. */
    alp->pp = apr_pcalloc(mp, sizeof(alp2_pp_t));
    if (alp->pp == NULL) return -1;
    if (alp2_pp_init(alp->pp, alp, alp2_callback, mp) < 0) {
        return -2;
    }

    /* Compile the patterns we use for parsing. */

    /* part A pattern */
    if ((alp->part_a_pattern = pcre_compile(
        "^\\[(.+)\\] (\\S+) ([.:0-9a-f]+) (\\d+) ([.:0-9a-f]+) (\\d+)$",
        PCRE_DOTALL, &errptr, &erroffset, NULL)) ==  NULL)
    {
        return -3;
    }

    /* request line pattern */
    if ((alp->request_line_pattern = pcre_compile(
        // TODO Needs improving (e.g. to support simplified HTTP/0.9 requests
        "^(\\S+) (.*?) (HTTP/\\d\\.\\d)$", 
        PCRE_DOTALL, &errptr, &erroffset, NULL)) == NULL)
    {
        return -4;
    }

    /* header pattern */
    if ((alp->header_pattern = pcre_compile(
        "^([^:]+):\\s*(.+)$",
        PCRE_DOTALL, &errptr, &erroffset, NULL)) == NULL)
    {
        return -5;
    }

    /* response line pattern */
    if ((alp->response_line_pattern = pcre_compile(
        "^(HTTP/\\d\\.\\d) (\\d{3})( (.+))?$",
        PCRE_DOTALL, &errptr, &erroffset, NULL)) == NULL)
    {
        return -6;
    }

    /* Action trailer header pattern */
    if ((alp->trailer_action_pattern = pcre_compile(
        "^Intercepted \\(phase (\\d)\\)$",
        PCRE_DOTALL, &errptr, &erroffset, NULL)) == NULL)
    {
        return -7;
    }

    /* Stopwatch trailer header pattern */
    if ((alp->trailer_stopwatch_pattern = pcre_compile(
        "^(\\d+) (\\d+)( \\((-|\\d+)\\*? (-|\\d+) (-|\\d+)\\))?$",
        PCRE_DOTALL, &errptr, &erroffset, NULL)) == NULL)
    {
        return -8;
    }

    /* WebApp-Info trailer header pattern */
    if ((alp->trailer_webappinfo_pattern = pcre_compile(
        "^\"(.*)\" \"(.*)\" \"(.*)\"$",
        PCRE_DOTALL, &errptr, &erroffset, NULL)) == NULL)
    {
        return -9;
    }

    return 1;
}

/**
 * Process a piece of a stream of audit log entries.
 */
int alp2_process(alp2_t *alp, const char *data, size_t len)
{
    alp2_pp_process(alp->pp, data, len);

    if (alp->done) {
        return 0;
    }
    else {
        return 1;
    }
}

/**
 * Destroy the parser.
 */
void alp2_destroy(alp2_t *alp)
{
    apr_pool_destroy(alp->mp);
}

