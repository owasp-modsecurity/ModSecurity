/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <limits.h>
#ifdef __linux__
#include <sys/inotify.h>
#endif
#include <fcntl.h>

#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_set.h"

#ifdef __linux__
#define EVENT_SIZE          ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN       ( 1024 * ( EVENT_SIZE + NAME_MAX + 1) )
#define WATCH_FLAGS         ( IN_MODIFY )
#endif

char main_rule_uri[] = "basic_rules.conf";

int main (int argc, char **argv)
{
    int ret;
    const char *error = NULL;
    ModSecurity *modsec;
    Transaction *transaction = NULL;
    RulesSet *rules;
#ifdef __linux__
    char buffer[EVENT_BUF_LEN];
#endif

    modsec = msc_init();

    msc_set_connector_info(modsec, "ModSecurity-test v0.0.1-alpha (Simple " \
        "example on how to use ModSecurity API");

    rules = msc_create_rules_set();

    ret = msc_rules_add_file(rules, main_rule_uri, &error);
    if (ret < 0) {
        fprintf(stderr, "Problems loading the rules --\n");
        fprintf(stderr, "%s\n", error);
        goto end;
    }
    msc_rules_dump(rules);

    printf("Waiting for changes...\n");

#ifdef __linux__
    fd_set watch_set;
    int fd;
#ifdef IN_NONBLOCK
    fd = inotify_init1( IN_NONBLOCK );
#else
    fd = inotify_init();
#endif
    if ( fd < 0 ) {
        perror( "inotify_init" );
    }

    FD_ZERO( &watch_set );
    FD_SET( fd, &watch_set );

    char **files = NULL;
    int files_num = msc_rules_files_to_watch(rules, &files);
    for (int i = 0; i < files_num; i++) {
        printf("Watching for modifications at: %s\n", files[i]);
        int wd = inotify_add_watch( fd, files[i], WATCH_FLAGS );
    }

    for (;;) {
        select( fd+1, &watch_set, NULL, NULL, NULL );

        int length = read( fd, buffer, EVENT_BUF_LEN );
        if ( length < 0 ) {
            perror( "read" );
        }

        for (int i=0; i < length; ) {
            struct inotify_event *event = ( struct inotify_event * ) &buffer[i];
            if ( event->len ) {
                printf("There was a change at: %s\n", event->name);
            }
            else {
                printf("A file has been changed.\n");
            }
            i += EVENT_SIZE + event->len;
        }

        msc_rules_reload(rules);

        {
            FD_ZERO( &watch_set );
            FD_SET( fd, &watch_set );

            char **files = NULL;
            int files_num = msc_rules_files_to_watch(rules, &files);
            for (int i = 0; i < files_num; i++) {
                printf("Watching for modifications at: %s\n", files[i]);
                int wd = inotify_add_watch( fd, files[i], WATCH_FLAGS );
            }
        }
        printf("Rules are reloaded.\n");
        msc_rules_dump(rules);
    }
#else
    printf("inotify not enable in your platform\n");
#endif
end:
    msc_rules_cleanup(rules);
    msc_cleanup(modsec);

    return 0;
}


