/*
 * ModSecurity, http://www.modsecurity.org/
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 */

/* C API test for msc_get_tx() / msc_free(). */

#include <stdio.h>
#include <string.h>

#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"


static int failed = 0;

static void check(int ok, const char *what) {
    printf(":test-result: %s: msc_get_tx: %s\n", ok ? "PASS" : "FAIL", what);
    if (!ok) {
        failed = 1;
    }
}


int main(void) {
    const char *error = NULL;
    ModSecurity *modsec;
    RulesSet *rules;
    Transaction *t;
    char *value;
    size_t len;
    int rc;

    modsec = msc_init();
    rules = msc_create_rules_set();

    if (msc_rules_add(rules,
        "SecRuleEngine On\n"
        "SecAction \"id:1,phase:1,pass,nolog,setvar:tx.Tag=admin\"\n"
        "SecRule ARGS:bin \"@rx ^(.*)$\" "
            "\"id:2,phase:1,pass,nolog,capture,setvar:tx.bin=%{TX.1}\"\n",
        &error) < 0) {
        printf(":test-result: FAIL: msc_get_tx: rules: %s\n", error);
        return 1;
    }

    t = msc_new_transaction(modsec, rules, NULL);
    msc_process_connection(t, "127.0.0.1", 12345, "127.0.0.1", 80);
    msc_process_uri(t, "/?bin=foo%00bar", "GET", "1.1");
    msc_process_request_headers(t);

    rc = msc_get_tx(t, "tag", &value, &len);
    check(rc == 1 && len == 5 && strcmp(value, "admin") == 0,
        "found, case-insensitive");
    msc_free(value);

    rc = msc_get_tx(t, "BIN", &value, &len);
    check(rc == 1 && len == 7 && memcmp(value, "foo\0bar", 8) == 0,
        "embedded NUL kept");
    msc_free(value);

    rc = msc_get_tx(t, "missing", &value, &len);
    check(rc == 0 && value == NULL, "unset returns 0");

    rc = msc_get_tx(t, "tag", &value, NULL);
    check(rc == 1 && strcmp(value, "admin") == 0, "len is optional");
    msc_free(value);

    check(msc_get_tx(NULL, "tag", &value, &len) == -1 && value == NULL,
        "NULL transaction");
    check(msc_get_tx(t, NULL, &value, &len) == -1, "NULL name");
    check(msc_get_tx(t, "tag", NULL, &len) == -1, "NULL value");

    msc_free(NULL);

    msc_transaction_cleanup(t);
    msc_rules_cleanup(rules);
    msc_cleanup(modsec);

    return failed;
}
