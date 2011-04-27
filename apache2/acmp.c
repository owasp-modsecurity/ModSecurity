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

/* Aho-Corasick Matching  */

#include "acmp.h"

#ifdef ACMP_USE_UTF8
/* UTF support */
#include "utf8tables.h"
#else
/* No UTF support */
#define acmp_utf8_char_t long
#include <apr_lib.h>
#define utf8_lcase(a) apr_tolower(a)
#endif

#include <apr_tables.h>
#include <stdio.h>
#include <string.h>


/*
 *******************************************************************************
 *******************************************************************************
 * Data structures for acmp parser
 */

/**
 * One node in trie
 */
typedef struct acmp_node_t acmp_node_t;
typedef struct acmp_btree_node_t acmp_btree_node_t;
struct acmp_node_t {
    acmp_utf8_char_t letter;
    int  is_last;
    acmp_callback_t callback;
    void *callback_data;
    int depth;

    acmp_node_t *child;
    acmp_node_t *sibling;
    acmp_node_t *fail;
    acmp_node_t *parent;
    acmp_node_t *o_match;

    acmp_btree_node_t *btree;

    apr_size_t hit_count;

    char *text;
    char *pattern;
};

struct acmp_btree_node_t {
    acmp_utf8_char_t letter;
    acmp_btree_node_t *left;
    acmp_btree_node_t *right;
    acmp_node_t *node;
};

/**
 * Data related to parser, not to individual nodes
 */
struct ACMP {
#ifdef ACMP_USE_UTF8
    int is_utf8;
#endif
    int is_case_sensitive;
    apr_pool_t *parent_pool;
    apr_pool_t *pool;

    int dict_count;
    apr_size_t longest_entry;

    acmp_node_t *root_node;

    const char *data_start;
    const char *data_end;
    const char *data_pos;
    apr_size_t data_len;

    apr_size_t *bp_buffer;
    apr_size_t bp_buff_len;

    acmp_node_t *active_node;
    char u8_buff[6];
    apr_size_t  u8buff_len;
    apr_size_t  hit_count;
    int  is_failtree_done;
    int  is_active;
    apr_size_t  byte_pos;
    apr_size_t  char_pos;
};

/*
 *******************************************************************************
 *******************************************************************************
 * Functions for UTF-8 support
 */

#ifdef ACMP_USE_UTF8
/**
 * Returns length of utf-8 sequence based on its first byte
 */
static int utf8_seq_len(const char *first_byte) {
    return utf8_seq_lengths[(unsigned int)(unsigned char)first_byte[0]];
}

/**
 * Returns length of utf8-encoded text
 */
static size_t utf8_strlen(const char *str) {
    int len = 0;
    const char *c = str;
    while (*c != 0) {
        c += utf8_seq_len(c);
        len++;
    }
    return len;
}

/**
 * Returns ucs code for given utf-8 sequence
 */
static acmp_utf8_char_t utf8_decodechar(const char *str) {
    int len = utf8_seq_len(str);
    acmp_utf8_char_t ch = 0;
    switch (len) {
        case 6: ch += (unsigned char)*str++; ch <<= 6;
        case 5: ch += (unsigned char)*str++; ch <<= 6;
        case 4: ch += (unsigned char)*str++; ch <<= 6;
        case 3: ch += (unsigned char)*str++; ch <<= 6;
        case 2: ch += (unsigned char)*str++; ch <<= 6;
        case 1: ch += (unsigned char)*str++;
    }
    ch -= utf8_offsets[len - 1];
    return ch;
}

/**
 * Returns lowercase for given unicode character. Searches through
 *   utf8_lcase_map table, if it doesn't find the code assumes
 *   it doesn't have a lowercase variant and returns code itself.
 */
static long utf8_lcase(acmp_utf8_char_t ucs_code) {
    long mid, left, right;
    left = 1;
    right = UTF8_LCASEMAP_LEN * 2 + 1;

    while (left <= right) {
        mid = (left + right) >> 1;
        mid -= (mid % 2); mid++;
        if (ucs_code > utf8_lcase_map[mid])
            left = mid + 2;
        else if (ucs_code < utf8_lcase_map[mid])
            right = mid - 2;
        else if (ucs_code == utf8_lcase_map[mid])
            return utf8_lcase_map[mid - 1];
    }
    return ucs_code;
}
#endif

/*
 *******************************************************************************
 *******************************************************************************
 * Code for local / static utility functions
 */

/**
 * Returns length of given string for parser's encoding
 */
static size_t acmp_strlen(ACMP *parser, const char *str) {
#ifdef ACMP_USE_UTF8
    return (parser->is_utf8 == 0) ? strlen(str) : utf8_strlen(str);
#else
    return strlen(str);
#endif
}

/**
 * Turns string to array of ucs values, depending on parser's encoding
 *       str - string to convert, doesn't have to be NULL-terminated
 * ucs_chars - where to write ucs values
 *       len - length of input string
 */
static void acmp_strtoucs(ACMP *parser, const char *str, acmp_utf8_char_t *ucs_chars, int len) {
    int i;
    const char *c = str;

#ifdef ACMP_USE_UTF8
    if (parser->is_utf8) {
        for (i = 0; i < len; i++) {
            *(ucs_chars++) = utf8_decodechar(c);
            c += utf8_seq_len(c);
        }
    } else
#endif
    {
        for (i = 0; i < len; i++) {
            *(ucs_chars++) = *(c++);
        }
    }
}

/**
 * Returns node with given letter, or null if not found
 */
static acmp_node_t *acmp_child_for_code(acmp_node_t *parent_node, acmp_utf8_char_t ucs_code) {
    acmp_node_t *node = parent_node->child;
    if (node == NULL) return NULL;
    for (;;) {
        if (node->letter == ucs_code) return node;
        node = node->sibling;
        if (node == NULL) return NULL;
    }
}

/**
 * Adds node to parent node, if it is not already there
 */
static void acmp_add_node_to_parent(acmp_node_t *parent, acmp_node_t *child) {
    acmp_node_t *node = NULL;

    child->parent = parent;
    if (parent->child == NULL) {
        parent->child = child;
        return;
    }

    node = parent->child;
    for (;;) {
        if (node == child) return;
        if (node->sibling == NULL) {
            node->sibling = child;
            return;
        }
        node = node->sibling;
    }
}

/**
 * Copies values from one node to another, without child/sibling/fail pointers
 * and without state variables.
 */
static void acmp_clone_node_no_state(acmp_node_t *from, acmp_node_t *to) {
    memcpy(to, from, sizeof(acmp_node_t));
    to->child = NULL;
    to->sibling = NULL;
    to->fail = NULL;
    to->hit_count = 0;
}

static inline acmp_node_t *acmp_btree_find(acmp_node_t *node, acmp_utf8_char_t letter) {
    acmp_btree_node_t *bnode = node->btree;
    for (;;) {
        if (bnode == NULL) return NULL;
        if (bnode->letter == letter) return bnode->node;
        if (bnode->letter > letter) {
            bnode = bnode->left;
        } else {
            bnode = bnode->right;
        }
    }
}

/**
 *
 */
static inline acmp_node_t *acmp_goto(acmp_node_t *node, acmp_utf8_char_t letter) {
    return acmp_btree_find(node, letter);
}

/**
 * Connects each node with its first fail node that is end of a phrase.
 */
static void acmp_connect_other_matches(ACMP *parser, acmp_node_t *node) {
    acmp_node_t *child, *om;

    for (child = node->child; child != NULL; child = child->sibling) {
        if (child->fail == NULL) continue;
        for (om = child->fail; om != parser->root_node; om = om->fail) {
            if (om->is_last) {
                child->o_match = om;
                break;
            }
        }
    }

    /* Go recursively through children of this node that have a child node */
    for(child = node->child; child != NULL; child = child->sibling) {
        if (child->child != NULL) acmp_connect_other_matches(parser, child);
    }
}

/**
 * Adds leaves to binary tree, working from sorted array of keyword tree nodes
 */
static void acmp_add_btree_leaves(acmp_btree_node_t *node, acmp_node_t *nodes[],
        int pos, int lb, int rb, apr_pool_t *pool) {

    int left = 0, right = 0;
    if ((pos - lb) > 1) {
        left = lb + (pos - lb) / 2;
        node->left = apr_pcalloc(pool, sizeof(acmp_btree_node_t));
        /* ENH: Check alloc succeded */
        node->left->node = nodes[left];
        node->left->letter = nodes[left]->letter;
#ifdef DEBUG_ACMP
        fprintf(stderr, "%lc ->left %lc\n", (wint_t)node->node->letter, (wint_t)node->left->node->letter);
#endif
    }
    if ((rb - pos) > 1) {
        right = pos + (rb - pos) / 2;
        node->right = apr_pcalloc(pool, sizeof(acmp_btree_node_t));
        /* ENH: Check alloc succeded */
        node->right->node = nodes[right];
        node->right->letter = nodes[right]->letter;
#ifdef DEBUG_ACMP
        fprintf(stderr, "%lc ->right %lc\n", (wint_t)node->node->letter, (wint_t)node->right->node->letter);
#endif
    }
    if (node->right != NULL) {
        acmp_add_btree_leaves(node->right, nodes, right, pos, rb, pool);
    }
    if (node->left != NULL) {
        acmp_add_btree_leaves(node->left, nodes, left, lb, pos, pool);
    }
}

/**
 * Builds balanced binary tree from children nodes of given node.
 */
static void acmp_build_binary_tree(ACMP *parser, acmp_node_t *node) {
    apr_size_t count, i, j;
    acmp_node_t *child = node->child;
    acmp_node_t **nodes;
    apr_size_t pos;

    /* Build an array big enough */
    for (count = 0; child != NULL; child = child->sibling) count++;
    nodes = apr_pcalloc(parser->pool, count * sizeof(acmp_node_t *));
    /* ENH: Check alloc succeded */

    /* ENH: Combine this in the loop below - we do not need two loops */
    child = node->child;
    for (i = 0; i < count; i++) {
        nodes[i] = child;
        child = child->sibling;
    };

    /* We have array with all children of the node and number of those children
     */
    for (i = 0; i < count - 1; i++)
        for (j = i + 1; j < count; j++) {
            acmp_node_t *tmp;

            if (nodes[i]->letter < nodes[j]->letter) continue;

            tmp = nodes[i];
            nodes[i] = nodes[j];
            nodes[j] = tmp;
        }
    node->btree = apr_pcalloc(parser->pool, sizeof(acmp_btree_node_t));
    /* ENH: Check alloc succeded */
    pos = count / 2;
    node->btree->node = nodes[pos];
    node->btree->letter = nodes[pos]->letter;
    acmp_add_btree_leaves(node->btree, nodes, pos, -1, count, parser->pool);
    for (i = 0; i < count; i++) {
        if (nodes[i]->child != NULL) acmp_build_binary_tree(parser, nodes[i]);
    }
}

/**
 * Constructs fail paths on keyword trie
 */
static apr_status_t acmp_connect_fail_branches(ACMP *parser) {
    /* Already connected ? */
    acmp_node_t *child, *node, *goto_node;
    apr_array_header_t *arr, *arr2, *tmp;

    if (parser->is_failtree_done != 0) return APR_SUCCESS;

    parser->root_node->text = "";
    arr  = apr_array_make(parser->pool, 32, sizeof(acmp_node_t *));
    arr2 = apr_array_make(parser->pool, 32, sizeof(acmp_node_t *));

    parser->root_node->fail = parser->root_node;

    /* All first-level children will fail back to root node */
    for (child = parser->root_node->child; child != NULL; child = child->sibling) {
        child->fail = parser->root_node;
        *(acmp_node_t **)apr_array_push(arr) = child;
#ifdef DEBUG_ACMP
        fprintf(stderr, "fail direction: *%s* => *%s*\n", child->text, child->fail->text);
#endif
    }

    for (;;) {
        while (apr_is_empty_array(arr) == 0) {
            node = *(acmp_node_t **)apr_array_pop(arr);
            node->fail = parser->root_node;
            if (node->parent != parser->root_node) {
                goto_node = acmp_child_for_code(node->parent->fail, node->letter);
                node->fail = (goto_node != NULL) ? goto_node : parser->root_node;
            }
#ifdef DEBUG_ACMP
            fprintf(stderr, "fail direction: *%s* => *%s*\n", node->text, node->fail->text);
#endif
            child = node->child;
            while (child != NULL) {
                *(acmp_node_t **)apr_array_push(arr2) = child;
                child = child->sibling;
            }
        }
        if (apr_is_empty_array(arr2) != 0) break;

        tmp = arr;
        arr = arr2;
        arr2 = tmp;
    }
    acmp_connect_other_matches(parser, parser->root_node);
    if (parser->root_node->child != NULL) acmp_build_binary_tree(parser, parser->root_node);
    parser->is_failtree_done = 1;
    return APR_SUCCESS;
}

/*
 *******************************************************************************
 *******************************************************************************
 * Code for functions from header file
 */


/**
 * flags - OR-ed values of ACMP_FLAG constants
 * pool  - apr_pool to use as parent pool, can be set to NULL
 */
ACMP *acmp_create(int flags, apr_pool_t *pool) {
    apr_status_t rc;
    apr_pool_t *p;
    ACMP *parser;

    rc = apr_pool_create(&p, pool);
    if (rc != APR_SUCCESS) return NULL;

    parser = apr_pcalloc(p, sizeof(ACMP));
    /* ENH: Check alloc succeded */
    parser->pool = p;
    parser->parent_pool = pool;
#ifdef ACMP_USE_UTF8
    parser->is_utf8 = (flags & ACMP_FLAG_UTF8) == 0 ? 0 : 1;
#endif
    parser->is_case_sensitive = (flags & ACMP_FLAG_CASE_SENSITIVE) == 0 ? 0 : 1;
    parser->root_node = apr_pcalloc(p, sizeof(acmp_node_t));
    /* ENH: Check alloc succeded */
    return parser;
}

/**
 * Creates fail tree and initializes buffer
 */
apr_status_t acmp_prepare(ACMP *parser) {
    apr_status_t st;

    if (parser->bp_buff_len < parser->longest_entry) {
        parser->bp_buff_len = parser->longest_entry * 2;
        parser->bp_buffer = apr_pcalloc(parser->pool, sizeof(apr_size_t) * parser->bp_buff_len);
        /* ENH: Check alloc succeded */
    }

    st = acmp_connect_fail_branches(parser);
    parser->active_node = parser->root_node;
    if (st != APR_SUCCESS) return st;
    parser->is_active = 1;
    return APR_SUCCESS;
}

/**
 * Adds pattern to parser
 * parser - ACMP parser
 * pattern - string with pattern to match
 * callback - Optional, pointer to an acmp_callback_t function
 * data - pointer to data that will be passed to callback function, only used if callback
 *   is supplied
 * len - Length of pattern in characters, if zero string length is used.
 */
apr_status_t acmp_add_pattern(ACMP *parser, const char *pattern,
        acmp_callback_t callback, void *data, apr_size_t len)
{
    size_t length, i, j;
    acmp_utf8_char_t *ucs_chars;
    acmp_node_t *parent, *child;

    if (parser->is_active != 0) return APR_EGENERAL;

    length = (len == 0) ? acmp_strlen(parser, pattern) : len;
    ucs_chars = apr_pcalloc(parser->pool, length * sizeof(acmp_utf8_char_t));
    /* ENH: Check alloc succeded */

    parent = parser->root_node;
    acmp_strtoucs(parser, pattern, ucs_chars, length);

    for (i = 0; i < length; i++) {
        acmp_utf8_char_t letter = ucs_chars[i];
        if (parser->is_case_sensitive == 0) {
            letter = utf8_lcase(letter);
        }
        child = acmp_child_for_code(parent, letter);
        if (child == NULL) {
            child = apr_pcalloc(parser->pool, sizeof(acmp_node_t));
            /* ENH: Check alloc succeded */
            child->pattern = "";
            child->letter = letter;
            child->depth = i;
            child->text = apr_pcalloc(parser->pool, strlen(pattern) + 2);
            /* ENH: Check alloc succeded */
            for (j = 0; j <= i; j++) child->text[j] = pattern[j];
        }
        if (i == length - 1) {
            if (child->is_last == 0) {
                parser->dict_count++;
                child->is_last = 1;
                child->pattern = apr_pcalloc(parser->pool, strlen(pattern) + 2);
                /* ENH: Check alloc succeded */
                strcpy(child->pattern, pattern);
            }
            child->callback = callback;
            child->callback_data = data;
        }
        acmp_add_node_to_parent(parent, child);
        parent = child;
    }
    if (length > parser->longest_entry) parser->longest_entry = length;
    parser->is_failtree_done = 0;

    return APR_SUCCESS;
}

/**
 * Process the data using ACMPT to keep state, and ACMPT's parser to keep the tree
 */
apr_status_t acmp_process_quick(ACMPT *acmpt, const char **match, const char *data, apr_size_t len) {
    ACMP *parser;
    acmp_node_t *node, *go_to;
    const char *end;

    if (acmpt->parser->is_failtree_done == 0) {
        acmp_prepare(acmpt->parser);
    };

    parser = acmpt->parser;
    if (acmpt->ptr == NULL) acmpt->ptr = parser->root_node;
    node = acmpt->ptr;
    end = data + len;

    while (data < end) {
        acmp_utf8_char_t letter = (unsigned char)*data++;

        if (parser->is_case_sensitive == 0) letter = utf8_lcase(letter);

        go_to = NULL;
        while (go_to == NULL) {
            go_to = acmp_goto(node, letter);
            if (go_to != NULL) {
                if (go_to->is_last) {
                    *match = go_to->text;
                    return 1;
                }
            }
            if (node == parser->root_node) break;
            if (go_to == NULL) node = node->fail;
        }
        if (go_to != NULL) node = go_to;

        /* If node has o_match, then we found a pattern */
        if (node->o_match != NULL) {
            *match = node->text;
            return 1;
        }
    }
    acmpt->ptr = node;
    return 0;
}
