/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2008 Breach Security, Inc. (http://www.breach.com/)
 *
 * You should have received a copy of the licence along with this
 * program (stored in the file "LICENSE"). If the file is missing,
 * or if you have any other questions related to the licence, please
 * write to Breach Security, Inc. at support@breach.com.
 *
 */
#include "acmp.h"
#include "utf8tables.h"
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
    int is_utf8;
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

/*
 *******************************************************************************
 *******************************************************************************
 * Code for local / static utility functions
 */

/**
 * Returns length of given string for parser's encoding
 */
static size_t acmp_strlen(ACMP *parser, const char *str) {
    return (parser->is_utf8 == 0) ? strlen(str) : utf8_strlen(str);
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

    if (parser->is_utf8 == 0) {
        for (i = 0; i < len; i++) {
            *(ucs_chars++) = *(c++);
        }
    } else {
        for (i = 0; i < len; i++) {
            *(ucs_chars++) = utf8_decodechar(c);
            c += utf8_seq_len(c);
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
    child->parent = parent;
    if (parent->child == NULL) {
        parent->child = child;
        return;
    }
    acmp_node_t *node = parent->child;
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

/**
 * Copies sibling nodes and child node for from given "from" node to "to" node.
 *   Both nodes must already exist.
 */
static void acmp_copy_nodes_recursive(acmp_node_t *from, acmp_node_t *to, apr_pool_t *pool) {
    acmp_node_t *old_node = from->child, *new_node, *nn2;
    if (old_node == NULL) return;
    nn2 = apr_pcalloc(pool, sizeof(acmp_node_t));
    acmp_clone_node_no_state(old_node, nn2);
    nn2->parent = to;
    to->child = nn2;
    acmp_copy_nodes_recursive(from->child, to->child, pool);

    for (;;) {
        old_node = old_node->sibling;
        if (old_node == NULL) break;
        new_node = apr_pcalloc(pool, sizeof(acmp_node_t));
        acmp_clone_node_no_state(old_node, new_node);
        new_node->parent = to;
        nn2->sibling = new_node;
        nn2 = new_node;
        acmp_copy_nodes_recursive(old_node, new_node, pool);
    }
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
        node->left->node = nodes[left];
        node->left->letter = nodes[left]->letter;
        #ifdef DEBUG_ACMP
        fprintf(stderr, "%lc ->left %lc\n", (wint_t)node->node->letter, (wint_t)node->left->node->letter);
        #endif
    }
    if ((rb - pos) > 1) {
        right = pos + (rb - pos) / 2;
        node->right = apr_pcalloc(pool, sizeof(acmp_btree_node_t));
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

    for (count = 0; child != NULL; child = child->sibling) count++;
    acmp_node_t *nodes[count];
    child = node->child;
    for (i = 0; i < count; i++) {
        nodes[i] = child;
        child = child->sibling;
    };
    /* We have array with all children of the node and number of those children
     */
    for (i = 0; i < count - 1; i++)
        for (j = i + 1; j < count; j++) {
            if (nodes[i]->letter < nodes[j]->letter) continue;
            acmp_node_t *tmp = nodes[i];
            nodes[i] = nodes[j];
            nodes[j] = tmp;
        }
    node->btree = apr_pcalloc(parser->pool, sizeof(acmp_btree_node_t));
    apr_size_t pos = count / 2;
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
    if (parser->is_failtree_done != 0) return APR_SUCCESS;
    acmp_node_t *child, *node, *goto_node;
    apr_array_header_t *arr, *arr2, *tmp;

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

/**
 * Clears hit count of each node, called from acmp_reset()
 */
static void acmp_clear_hit_count_recursive(acmp_node_t *node) {
    for (; node != NULL; node = node->sibling) {
        node->hit_count = 0;
        if (node->child != NULL) acmp_clear_hit_count_recursive(node->child);
    }
}

/**
 * Called when a match is found
 */
static void acmp_found(ACMP *parser, acmp_node_t *node) {
    if (node->callback) {
        node->callback(parser, node->callback_data,
            parser->bp_buffer[(parser->char_pos - node->depth - 1) % parser->bp_buff_len],
            parser->char_pos - node->depth - 1);
    }
    node->hit_count++;
    parser->hit_count++;
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
    rc = apr_pool_create(&p, pool);
    if (rc != APR_SUCCESS) return NULL;

    ACMP *parser = apr_pcalloc(p, sizeof(ACMP));
    parser->pool = p;
    parser->parent_pool = pool;
    parser->is_utf8 = (flags & ACMP_FLAG_UTF8) == 0 ? 0 : 1;
    parser->is_case_sensitive = (flags & ACMP_FLAG_CASE_SENSITIVE) == 0 ? 0 : 1;
    parser->root_node = apr_pcalloc(p, sizeof(acmp_node_t));
    return parser;
}

/**
 * Destroys previously created parser
 */
void acmp_destroy(ACMP *parser) {
    /*
     * All data is kept in parser's pool (including parser struct itself), so
     * destroying the pool will destroy everything
     */
    apr_pool_destroy(parser->pool);
}

/**
 * Creates parser with same options and same patterns
 * parser - ACMP parser to duplicate
 * pool - parent pool to use, if left as NULL original parser's parent pool is used
 */
ACMP *acmp_duplicate(ACMP *parser, apr_pool_t *pool) {
    apr_status_t rc;
    apr_pool_t *p;

    if (pool == NULL) pool = parser->parent_pool;
    rc = apr_pool_create(&p, pool);
    if (rc != APR_SUCCESS) return NULL;

    ACMP *new_parser = apr_pcalloc(p, sizeof(ACMP));
    new_parser->pool = p;
    new_parser->parent_pool = pool;
    new_parser->is_utf8 = parser->is_utf8;
    new_parser->is_case_sensitive = parser->is_case_sensitive;
    new_parser->root_node = apr_pcalloc(p, sizeof(acmp_node_t));
    new_parser->dict_count = parser->dict_count;
    new_parser->longest_entry = parser->longest_entry;
    acmp_copy_nodes_recursive(parser->root_node, new_parser->root_node, new_parser->pool);
    acmp_prepare(new_parser);
    return new_parser;
}

/**
 * Creates fail tree and initializes buffer
 */
apr_status_t acmp_prepare(ACMP *parser) {
    if (parser->bp_buff_len < parser->longest_entry) {
        parser->bp_buff_len = parser->longest_entry * 2;
        parser->bp_buffer = apr_pcalloc(parser->pool, sizeof(apr_size_t) * parser->bp_buff_len);
    }
    apr_status_t st = acmp_connect_fail_branches(parser);
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
    if (parser->is_active != 0) return APR_EGENERAL;
    size_t length = (len == 0) ? acmp_strlen(parser, pattern) : len;
    size_t i, j;
    acmp_utf8_char_t ucs_chars[length];

    acmp_node_t *parent = parser->root_node, *child;
    acmp_strtoucs(parser, pattern, ucs_chars, length);

    for (i = 0; i < length; i++) {
        acmp_utf8_char_t letter = ucs_chars[i];
        if (parser->is_case_sensitive == 0) {
            letter = utf8_lcase(letter);
        }
        child = acmp_child_for_code(parent, letter);
        if (child == NULL) {
            child = apr_pcalloc(parser->pool, sizeof(acmp_node_t));
            child->pattern = "";
            child->letter = letter;
            child->depth = i;
            child->text = apr_pcalloc(parser->pool, strlen(pattern) + 2);
            for (j = 0; j <= i; j++) child->text[j] = pattern[j];
        }
        if (i == length - 1) {
            if (child->is_last == 0) {
                parser->dict_count++;
                child->is_last = 1;
                child->pattern = apr_pcalloc(parser->pool, strlen(pattern) + 2);
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
 * Called to process incoming data stream
 * data - ptr to incoming data
 * len - size of data in bytes
 */
apr_status_t acmp_process(ACMP *parser, const char *data, apr_size_t len) {
    if (parser->is_failtree_done == 0) acmp_prepare(parser);
    acmp_node_t *node = parser->active_node, *go_to;
    apr_size_t seq_length;
    const char *end = (data + len);

    while (data < end) {
        parser->bp_buffer[parser->char_pos % parser->bp_buff_len] = parser->byte_pos;
        acmp_utf8_char_t letter;
        if (parser->is_utf8) {
            if (parser->u8buff_len > 0) {
                /* Resuming partial utf-8 sequence */
                seq_length = utf8_seq_len(parser->u8_buff);
                for (;;) {
                    parser->u8_buff[parser->u8buff_len++] = *data++;
                    if (parser->u8buff_len == seq_length) {
                        parser->u8buff_len = 0;
                        letter = utf8_decodechar(parser->u8_buff);
                        parser->byte_pos += seq_length;
                        parser->char_pos++;
                        break;
                    }
                }
            } else {
                /* not resuming partial sequence, reading from the stream */
                seq_length = utf8_seq_len(data);
                if ((data + seq_length) > end) {
                    while (data < end) parser->u8_buff[parser->u8buff_len++] = *data++;
                    return APR_SUCCESS;
                } else {
                    letter = utf8_decodechar(data);
                    data += seq_length;
                    parser->byte_pos += seq_length;
                    parser->char_pos++;
                }
            }
        } else {
            letter = *data++;
            parser->byte_pos++;
            parser->char_pos++;
        }
        if (parser->is_case_sensitive == 0) letter = utf8_lcase(letter);

        go_to = NULL;
        while (go_to == NULL) {
            acmp_node_t *n2 = acmp_goto(node, letter);
            go_to = acmp_child_for_code(node, letter);
            if (n2 != go_to) {
                n2 = acmp_goto(node, letter);
            };
            if (go_to != NULL) {
                if (go_to->is_last) {
                    acmp_found(parser, go_to);
                }
            }
            if (node == parser->root_node) break;
            if (go_to == NULL) node = node->fail;
        }
        if (go_to != NULL) node = go_to;

        /* We need to collect other nodes that are last letters of phrase. These
         *   will be fail node of current node if it has is_last flag set, and
         *   fail node of that node, recursively down to root node.
         */
        go_to = node;
        if (go_to != parser->root_node) {
            for (go_to = go_to->o_match; go_to != NULL; go_to = go_to->o_match) {
                acmp_found(parser, go_to);
            }
        }
    }
    parser->active_node = node;
    return parser->hit_count > 0 ? 1 : 0;
}

/**
 * Resets the state of parser so you can start using it with new set of data.
 *
 * No need to clear buffer since it will be re-initialized at first run of
 * acmp_process
 */
void acmp_reset(ACMP *parser) {
    parser->is_active = 0;
    parser->byte_pos = 0;
    parser->char_pos = 0;
    parser->hit_count = 0;
    parser->u8buff_len = 0;
    acmp_clear_hit_count_recursive(parser->root_node);
}

/**
 * Creates an ACMPT struct that will use parser's tree, without duplicating its data
 */
ACMPT *acmp_duplicate_quick(ACMP *parser, apr_pool_t *pool) {
    apr_pool_t *p = (pool != NULL) ? pool : parser->pool;
    ACMPT *dup = apr_pcalloc(p, sizeof(ACMPT));
    dup->parser = parser;
    return dup;
}

/**
 * Process the data using ACMPT to keep state, and ACMPT's parser to keep the tree
 */
apr_status_t acmp_process_quick(ACMPT *acmpt, const char **match, const char *data, apr_size_t len) {
    if (acmpt->parser->is_failtree_done == 0) {
        acmp_prepare(acmpt->parser);
    };
    ACMP *parser = acmpt->parser;
    if (acmpt->ptr == NULL) acmpt->ptr = parser->root_node;
    acmp_node_t *node = acmpt->ptr, *go_to;
    const char *end = (data + len);

    while (data < end) {
        acmp_utf8_char_t letter = (unsigned char)*data++;
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
