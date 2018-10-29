// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#ifndef _WAF_LOCK_INTERNAL_HEADER
#define _WAF_LOCK_INTERNAL_HEADER

#ifdef _WIN32
#ifdef inline
#undef inline
#endif
#include <windows.h>
#include<Synchapi.h>
#else
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include <errno.h>
#include<pwd.h>
#include<grp.h>
#endif
#include<cstring>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include"waf_lock_external.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 **========================================================
 ** Semaphore Operator Structure and Function
 **========================================================
 */

#define DEFAULT_WAF_LOCK_ID_RANGE  0x8fffffff  // the default range of lock ID mapped by lock's name

/**
 **========================================================
 ** Defaut Setting of Semaphore Set
 **========================================================
 */

#define SEM_NUMBERS             2  // the number of semaphores in a semaphore set
#define SEM_READ_INITVAL        10 // the initial value of read semaphore
#define SEM_WRITE_INITVAL       1  // the initial value of write semaphore

#define SEM_ID_READ             0  // the index of read semaphore in a semaphore set
#define SEM_ID_WRITE            1  // the index of write semaphore in a semaphore set

/**
 ** Define the name suffix of Read/Write lock on Windows
 */
#ifdef _WIN32
#define READ_LOCK_SUFFIX "_DB_LOCK_READ"
#define WRITE_LOCK_SUFFIX "_DB_LOCK_WRITE"
#endif

/**
 ** Required by Linux.
 ** @param val:
 ** @param buf:
 ** @param array:
 */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

/**
 ** Create and initialize a waf_lock, if the lock with given key exists, just link to the lock.
 ** The new_lock should have been created before calling this function.
 ** @param new_lock:        The pointer to save the information of the lock.
 ** @param new_lock_args:   The pointer to lock properties needed to create the lock .
 ** return: WAF_SUCCESS_LOCK_CREATE if successfully created a new lock,
 **         WAF_SUCCESS_LOCK_OPEN if successfully link to an existed lock,
 **         WAF_LOCK_ERROR if failed.
 */
int lock_create(struct waf_lock *new_lock, struct waf_lock_args *new_lock_args);

/**
 ** Destroy the lock
 ** @param waf_lock: The waf_lock sturcture
 ** return: WAF_LOCK_SUCCESS if successfully destroy the lock,
 **         WAF_LOCK_ERROR if failed.
 */
int lock_destroy(struct waf_lock *waf_lock);

/**
 ** Close the lock
 ** @param waf_lock: The waf_lock sturcture
 ** return: WAF_LOCK_SUCCESS if successfully close the lock,
 **         WAF_LOCK_ERROR if failed.
 */
int lock_close(struct waf_lock *waf_lock);

/**
 ** Decrease a lock's value by a given number.
 ** @param waf_lock: The waf_lock sturcture
 ** @param index:   The index of atom lock (read or write) .
 ** @param val:     The value you want to decrease from the lock.
 ** return: WAF_LOCK_SUCCESS if success;
 **         or WAF_LOCK_ERROR if failed
 */
int lock_P(const struct waf_lock *waf_lock, int index, int val);

/**
 ** Increase a lock's value by a given number.
 ** @param waf_lock: The waf_lock sturcture
 ** @param index:   The index of atom lock (read or write) .
 ** @param val:     The value you want to add to the lock.
 ** return: WAF_LOCK_SUCCESS if success;
 **         or WAF_LOCK_ERROR if failed
 */
int lock_V(const struct waf_lock *waf_lock, int index, int val);

/**
 **========================================================
 ** Other Function
 **========================================================
 */

/**
 ** Check the string format.
 ** @param str: the string you want to check.
 ** @param str_len: given string length (not include '\0').
 ** return: WAF_LOCK_SUCCESS if the string length equals str_len;
 **             WAF_LOCK_ERROR if not
 */
int Waf_lock_isstring(const char* str, int str_len);

/**
** Initialize the handle of lock.
** @param waf_lock: the handle of Lock.
** return:  if WAF_LOCK_SUCCESS if success
**          or WAF_LOCK_ERROR if the handle is NULL.
*/
int Waf_lock_init(struct waf_lock* waf_lock);

#ifndef _WIN32
/**
** Get user id by name.
** @param name: name.
** return:  if WAF_LOCK_SUCCESS if success
**          or WAF_LOCK_ERROR if the handle is NULL.
*/
int GetUserId(const char* name, uid_t* id);
 /**
** Get group id by name.
** @param name: name.
** return:  if WAF_LOCK_SUCCESS if success
**          or WAF_LOCK_ERROR if the handle is NULL.
*/
int GetGroupId(const char* name, gid_t* id);
#endif // _WIN32

#ifdef __cplusplus
}
#endif

#endif

