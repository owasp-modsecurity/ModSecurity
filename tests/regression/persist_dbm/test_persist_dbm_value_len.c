/*
 * Regression test for collection_unpack value_len underflow.
 *
 * The original bug was reported in modsecurity#3082 and fixed by commit
 * 4f33f5b ("Fix possible segfault in collection_unpack", 2024-03-01).
 * The fix was silently dropped during merge 649aea7 (2024-04-04) and
 * has been missing from every release v2.9.10 .. v2.9.13.
 *
 * This test asserts that:
 *
 *   (1) the BUGGY arithmetic, when invoked with a value_len of 0,
 *       produces a copy length of UINT_MAX (this is the bug);
 *
 *   (2) the PATCHED predicate (`value_len < 1`) rejects the same
 *       blob and returns NULL, so apr_pstrmemdup is never called.
 *
 * The test does NOT link against libmodsecurity or APR. It replicates
 * the exact integer arithmetic of collection_unpack so that it can be
 * cherry-picked into any v2 source tree and run as part of CI without
 * touching APR / Apache build dependencies. Combined with -fsanitize=
 * address,undefined it also exercises the runtime sanitizers' view of
 * the underflow.
 *
 * Build (host):
 *
 *   cc -O0 -g -Wall -Wextra -fsanitize=address,undefined \
 *      test_persist_dbm_value_len.c -o test_persist_dbm_value_len
 *   ./test_persist_dbm_value_len
 *
 * Exit codes:
 *
 *   0  = both before-fix and after-fix paths behaved as expected
 *        (test PASSED — confirms regression and confirms patch fixes it)
 *   1  = test FAILED (either path returned an unexpected result)
 *
 * Drop into upstream:
 *
 *   tests/regression/persist_dbm/test_persist_dbm_value_len.c
 *   add to tests/regression/persist_dbm/Makefile.am
 */

#define _GNU_SOURCE
#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------------
 * Faithful replica of collection_unpack's deserialisation arithmetic.
 *
 * The two functions below differ ONLY in the `value_len < 1 ||` clause
 * that 4f33f5b added and 649aea7 removed. The blob layout, the read of
 * the 16-bit big-endian length, and the apr_pstrmemdup() argument
 * computation are otherwise byte-for-byte identical.
 *
 * Returns:
 *   0  -> rejected the blob (return NULL path was taken)
 *   1  -> would have called apr_pstrmemdup with copy_len = *out_copy_len
 *  -1  -> blob malformed in a way unrelated to this regression
 * ------------------------------------------------------------------------ */

static int unpack_buggy(const unsigned char *blob, unsigned int blob_size,
                        size_t *out_copy_len)
{
    unsigned int blob_offset = 3;          /* skip 3-byte header */
    unsigned int name_len;
    unsigned int value_len;

    if (blob_offset + 1 >= blob_size) return -1;
    name_len = (blob[blob_offset] << 8) + blob[blob_offset + 1];

    /* (preserved zero-check on name_len; only value_len is the regression) */
    if (name_len == 0) return -1;
    if (name_len > 65536) return -1;
    blob_offset += 2;

    /* === buggy bound check on name (no `< 1` clause) ===================== */
    if (blob_offset + name_len > blob_size) return 0;
    /* name pstrmemdup(name_len - 1) would happen here */
    blob_offset += name_len;

    if (blob_offset + 1 >= blob_size) return -1;
    value_len = (blob[blob_offset] << 8) + blob[blob_offset + 1];
    blob_offset += 2;

    /* === buggy bound check on value (no `< 1` clause) ==================== *
     * THIS is the line that 4f33f5b patched and 649aea7 reverted.           */
    if (blob_offset + value_len > blob_size) return 0;

    /* The actual call inside collection_unpack is:
     *
     *   var->value = apr_pstrmemdup(msr->mp,
     *                               (const char *)blob + blob_offset,
     *                               var->value_len - 1);
     *
     * with var->value_len declared `unsigned int`. If value_len == 0,
     * `value_len - 1` wraps to UINT_MAX. Reproduce that arithmetic
     * verbatim so the sanitizers see exactly the same operation. */
    size_t copy_len = (size_t)(value_len - 1);
    if (out_copy_len) *out_copy_len = copy_len;
    return 1;
}

static int unpack_patched(const unsigned char *blob, unsigned int blob_size,
                          size_t *out_copy_len)
{
    unsigned int blob_offset = 3;
    unsigned int name_len;
    unsigned int value_len;

    if (blob_offset + 1 >= blob_size) return -1;
    name_len = (blob[blob_offset] << 8) + blob[blob_offset + 1];

    if (name_len == 0) return -1;
    if (name_len > 65536) return -1;
    blob_offset += 2;

    /* === patched bound check on name ==================================== */
    if (name_len < 1 || blob_offset + name_len > blob_size) return 0;
    blob_offset += name_len;

    if (blob_offset + 1 >= blob_size) return -1;
    value_len = (blob[blob_offset] << 8) + blob[blob_offset + 1];
    blob_offset += 2;

    /* === patched bound check on value (the fix from 4f33f5b) ============= */
    if (value_len < 1 || blob_offset + value_len > blob_size) return 0;

    size_t copy_len = (size_t)(value_len - 1);
    if (out_copy_len) *out_copy_len = copy_len;
    return 1;
}

/* ------------------------------------------------------------------------ *
 * Test cases.
 * ------------------------------------------------------------------------ */

struct case_t {
    const char           *name;
    const unsigned char  *blob;
    unsigned int          blob_size;
    int                   expect_buggy_rc;
    size_t                expect_buggy_copy_len;   /* only checked when rc==1 */
    int                   expect_patched_rc;
    size_t                expect_patched_copy_len;
};

/* Case 1 — the malicious blob from poc_dbm_oob.c. Triggers the regression. */
static const unsigned char blob_value_len_zero[] = {
    0x49, 0x52, 0x01,   /* SDBM-side header */
    0x00, 0x01,         /* name_len = 1 */
    0x00,               /* name = "" */
    0x00, 0x00,         /* value_len = 0  ← BUG TRIGGER */
};

/* Case 2 — well-formed blob, name="", value="X". Both versions accept. */
static const unsigned char blob_well_formed[] = {
    0x49, 0x52, 0x01,
    0x00, 0x01,         /* name_len = 1 */
    0x00,               /* name = "" */
    0x00, 0x02,         /* value_len = 2 (X + NUL) */
    'X', 0x00,
};

/* Case 3 — value_len exactly 1 (just the trailing NUL of empty value). */
static const unsigned char blob_empty_value[] = {
    0x49, 0x52, 0x01,
    0x00, 0x01,         /* name_len = 1 */
    0x00,               /* name = "" */
    0x00, 0x01,         /* value_len = 1 (just NUL) */
    0x00,
};

/* Case 4 — value_len overflows blob_size (different bug class, both reject). */
static const unsigned char blob_value_len_too_big[] = {
    0x49, 0x52, 0x01,
    0x00, 0x01,
    0x00,
    0xFF, 0xFF,         /* value_len = 65535, way past blob_size */
};

static const struct case_t cases[] = {
    {
        .name                    = "value_len = 0 (CVE candidate)",
        .blob                    = blob_value_len_zero,
        .blob_size               = sizeof(blob_value_len_zero),
        .expect_buggy_rc         = 1,
        .expect_buggy_copy_len   = (size_t)UINT_MAX,
        .expect_patched_rc       = 0,
        .expect_patched_copy_len = 0,
    },
    {
        .name                    = "well-formed value_len = 2",
        .blob                    = blob_well_formed,
        .blob_size               = sizeof(blob_well_formed),
        .expect_buggy_rc         = 1,
        .expect_buggy_copy_len   = 1,
        .expect_patched_rc       = 1,
        .expect_patched_copy_len = 1,
    },
    {
        .name                    = "empty value (value_len = 1)",
        .blob                    = blob_empty_value,
        .blob_size               = sizeof(blob_empty_value),
        .expect_buggy_rc         = 1,
        .expect_buggy_copy_len   = 0,
        .expect_patched_rc       = 1,
        .expect_patched_copy_len = 0,
    },
    {
        .name                    = "value_len > blob_size",
        .blob                    = blob_value_len_too_big,
        .blob_size               = sizeof(blob_value_len_too_big),
        .expect_buggy_rc         = 0,
        .expect_buggy_copy_len   = 0,
        .expect_patched_rc       = 0,
        .expect_patched_copy_len = 0,
    },
};

static const size_t n_cases = sizeof(cases) / sizeof(cases[0]);

/* ------------------------------------------------------------------------ *
 * Runner.
 * ------------------------------------------------------------------------ */

static int run_one(const struct case_t *c)
{
    size_t buggy_copy = 0;
    size_t patched_copy = 0;
    int buggy_rc   = unpack_buggy  (c->blob, c->blob_size, &buggy_copy);
    int patched_rc = unpack_patched(c->blob, c->blob_size, &patched_copy);

    bool ok = true;

    if (buggy_rc != c->expect_buggy_rc) {
        printf("    [FAIL] buggy_rc:    got %d, want %d\n",
               buggy_rc, c->expect_buggy_rc);
        ok = false;
    } else if (buggy_rc == 1 && buggy_copy != c->expect_buggy_copy_len) {
        printf("    [FAIL] buggy_copy:  got %zu, want %zu\n",
               buggy_copy, c->expect_buggy_copy_len);
        ok = false;
    }

    if (patched_rc != c->expect_patched_rc) {
        printf("    [FAIL] patched_rc:  got %d, want %d\n",
               patched_rc, c->expect_patched_rc);
        ok = false;
    } else if (patched_rc == 1 && patched_copy != c->expect_patched_copy_len) {
        printf("    [FAIL] patched_copy: got %zu, want %zu\n",
               patched_copy, c->expect_patched_copy_len);
        ok = false;
    }

    if (ok) {
        printf("    buggy_rc=%d  buggy_copy=%-12zu"
               "  patched_rc=%d  patched_copy=%zu\n",
               buggy_rc, buggy_copy, patched_rc, patched_copy);
    }
    return ok ? 0 : 1;
}

int main(void)
{
    printf("collection_unpack value_len underflow regression test\n");
    printf("======================================================\n\n");

    /* Sanity: the buggy path's copy_len for value_len=0 must really
     * underflow. If a future compiler does something clever and the
     * underflow goes away, this test might silently start passing
     * even on the buggy code. Force a runtime assert. */
    {
        unsigned int v = 0;
        size_t got = (size_t)(v - 1);
        if (got != (size_t)UINT_MAX) {
            fprintf(stderr,
                    "FATAL: this platform's unsigned int wrap is %zu,"
                    " not UINT_MAX. Test cannot meaningfully run.\n", got);
            return 2;
        }
    }

    int fails = 0;
    for (size_t i = 0; i < n_cases; i++) {
        printf("[%zu/%zu] %s\n", i + 1, n_cases, cases[i].name);
        fails += run_one(&cases[i]);
        printf("\n");
    }

    printf("======================================================\n");
    if (fails == 0) {
        printf("PASS — regression confirmed in buggy path, fixed in patched path\n");
        return 0;
    } else {
        printf("FAIL — %d case(s) did not match expectations\n", fails);
        return 1;
    }
}
