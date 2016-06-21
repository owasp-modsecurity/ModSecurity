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

#include <modsecurity/modsecurity.h>
#include <modsecurity/transaction.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_SIZE 1024
#define FORKS 10
#define TRHREADS 10


char main_rule_uri[] = "basic_rules.conf";
Rules *rules = NULL;
ModSecurity *modsec = NULL;

key_t key;


void process_request (void *ptr)
{
    int i;
    for (i = 0; i < TRHREADS; i++) {
        struct timeval tv;

        int shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);
        char *data;
        int *j;
        data = shmat(shmid, (void *)0, 0);
        j = (int *)data;
        (*j)++;


        Transaction *transaction = NULL;
        transaction = msc_new_transaction(modsec, rules, NULL);

        msc_process_connection(transaction, "127.0.0.1", 12345, "127.0.0.1", 80);
        msc_process_uri(transaction,
            "http://www.modsecurity.org/test?key1=value1&key2=value2&key3=value3",
            "GET", "1.1");
        msc_add_request_header(transaction, "User-Agent",
            "Basic ModSecurity example");
        msc_process_request_headers(transaction);
        msc_process_request_body(transaction);
        msc_add_response_header(transaction, "Content-type", "text/html");
        msc_process_response_headers(transaction, 200, "HTTP 1.0");
        msc_process_response_body(transaction);
        msc_process_logging(transaction);
        msc_transaction_cleanup(transaction);
        tv.tv_sec = 0;
        tv.tv_usec = 1000;
        select(0, NULL, NULL, NULL, &tv);
    }
}


int main (int argc, char **argv)
{
    int ret = 1;
    const char *error = NULL;
    pthread_t thread[TRHREADS*FORKS];
    int i = 0;
    pid_t pid;
    int shmid;
    int h;
    
    modsec = msc_init();

    key = ftok("shmdemo.c", 'R');

    shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);
    char *data = shmat(shmid, (void *)0, 0);
    memset(data, '\0', SHM_SIZE);
    int *z = (int *) data;
    z = 0;

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

    for (h = 0; h < FORKS; h++) {
        pid = fork();
        if (pid == 0) {
            pthread_create (&thread[h*TRHREADS], NULL, (void *) &process_request, (void *) NULL);
            pthread_join(thread[h*TRHREADS], NULL);
            goto child;
        }
    }

    wait(NULL);

    int *j;
    data = shmat(shmid, (void *)0, 0);
    j = (int *) data;
    fprintf(stderr, "The final count is: %d\n", *j);

child:

    if (pid == 0) {
        return 0;
    }
end:
    msc_rules_cleanup(rules);
    msc_cleanup(modsec);

    return 0;
}


