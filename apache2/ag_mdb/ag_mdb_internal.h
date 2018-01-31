#ifndef _AG_MDB_INTERNAL_HEADER
#define _AG_MDB_INTERNAL_HEADER

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
#endif
#include<cstring>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include"murmur3.h"
#include"ag_mdb_external.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 **========================================================
 ** AG Memory Database Introduction
 **========================================================
 ** AG memory database includes two parts:
 ** 1. A block of shared memory (SHM): maintain the basic information of agdbm and the entries
 ** 2. A set of semaphores (SEM): ensure that multi-user access the database correctly
 **
 ** Data organization of AG Memory Database:
 ** Three kinds of linklist: spare linklist, hash linklist and expire-time linklist
 **
 ** The spare linklist contains entries that have not been used.
 ** Use two macros ENTRY_PREV() and ENTRY_NEXT() to save the pointer of the spare linklist.
 **
 ** A hash linklist contains entries whose key belongs to a same hashed value.
 ** The number of hash linklists defined by DEFAULT_HASH_LISTS_NUM.
 ** Use two macros ENTRY_PREV() and ENTRY_NEXT() to save the pointer of the hash linklist.
 **
 ** The expire-time linklist, contains all busy entries, is used to maintain the expire-time and remove the staled entries.
 ** The head of expire-time linklist is the oldest entry, and the next one is the second oldest entry. The tail of expire-time linklist is the newest entry.
 ** Use two macros ENTRY_TIME_PREV() and ETNRY_NEXT_PREV() to save the pointer of the expire-time linklist.
 */

 /**
  **========================================================
  ** AG Memory Database Default Setting
  **========================================================
  */

#define DEFAULT_AGMDB_ID_RANGE  0x8fffffff  // the default range of database ID mapped by database's name

  /* Hash Function Default Setting */
#define DEFAULT_HASH_MAGIC_SEED 0           // the seed of hash function in AGMDB_hash()
#define DEFAULT_HASH_VALUE_STR_LENGTH 4     // the length of the result value from hash function 

/* Internal Invalid Value */
#define AGMDB_INVALID_INDEX (unsigned long long)  -1  // invalid entry index
#define AGMDB_INVALID_ADDR  (PTR_VOID)      NULL// invalid address
#define AGMDB_INVALID_TIME  (unsigned int)  -1  // invalid expire time

/* Alternative defition of void* */
typedef char* PTR_VOID;
typedef const char* CPTR_VOID;
typedef unsigned long long PTR_OFFSET;


/**
 **========================================================
 ** Shared Memory (SHM)
 **========================================================
 */

 /* SHM Structure and Operation */
 /*
 +----------+------------------------+
 |0x00000000|SHM_SIZE                |
 +----------+------------------------+
 |0x00000004|HASH_LISTS_NUM          |
 +----------+------------------------+
 |0x00000008|ENTRIES_NUM             |
 +----------+------------------------+
 |0x0000000c|ENTRY_SIZE              |
 +----------+------------------------+
 |0x00000010|BUSY_ENTRY_CNT          |
 +----------+------------------------+
 |0x00000014|BUSY_HASH_CNT           |
 +----------+------------------------+
 |0x00000018|EXPIRE_TIME             |
 +----------+------------------------+
 |0x0000001c|EXPIRE_TIME_LIST_HEAD   |
 +----------+------------------------+
 |0x00000024|EXPIRE_TIME_LIST_TAIL   |
 +----------+------------------------+
 |0x0000002c|SPARE_LIST_HEAD         |
 +----------+------------------------+
 |0x00000034|Reserved                |
 |0x000000ff|                        |
 +----------+------------------------+
 |0x00000100|HASH_LISTS_HEADS        |
 |0x002000ff|                        |
 +----------+------------------------+
 |0x00200100|ENTRIES                 |
 |END       |                        |
 +----------+------------------------+
 */

#define SHM_SIZE_OFFSET                     0x000000000 // the offset of AG Memory Database size
#define SHM_HASH_LISTS_NUM_OFFSET           0x000000004 // the offset of number of hashes 
#define SHM_ENTRIES_NUM_OFFSET              0x000000008 // the offset of number of entries
#define SHM_ENTRY_SIZE_OFFSET               0x00000000c // the offset of the size of each entry
#define SHM_BUSY_ENTRY_CNT_OFFSET           0x000000010 // the offset of number of busy entries
#define SHM_BUSY_HASH_CNT_OFFSET            0x000000014 // the offset of number of busy hash lists
#define SHM_EXPIRE_TIME_OFFSET              0x000000018 // the offset of head of spare entry
#define SHM_EXPIRE_TIME_LIST_HEAD_OFFSET    0x00000001c // the offset of head of expire-time linklist 
#define SHM_EXPIRE_TIME_LIST_TAIL_OFFSET    0x000000024 // the offset of tail of expire-time linklist
#define SHM_SPARE_LIST_HEAD_OFFSET          0x00000002c // the offset of head of spare entry 
#define SHM_HASH_LISTS_HEADS_OFFSET         0x000000100 // the offset of heads of hash linklists
#define SHM_ENTRIES_OFFSET                  0x000200100 // the offset of entries

 /* SHM Operation */
#define SHM_SIZE(base)                  ((unsigned int *)(base + SHM_SIZE_OFFSET))
#define SHM_HASH_LISTS_NUM(base)        ((unsigned int *)(base + SHM_HASH_LISTS_NUM_OFFSET))
#define SHM_ENTRIES_NUM(base)           ((unsigned int *)(base + SHM_ENTRIES_NUM_OFFSET))
#define SHM_ENTRY_SIZE(base)            ((unsigned int *)(base + SHM_ENTRY_SIZE_OFFSET))
#define SHM_BUSY_ENTRY_CNT(base)        ((unsigned int *)(base + SHM_BUSY_ENTRY_CNT_OFFSET))
#define SHM_BUSY_HASH_CNT(base)         ((unsigned int *)(base + SHM_BUSY_HASH_CNT_OFFSET))
#define SHM_EXPIRE_TIME(base)           ((unsigned int *)(base + SHM_EXPIRE_TIME_OFFSET))
#define SHM_EXPIRE_TIME_LIST_HEAD(base) ((PTR_OFFSET *)(base + SHM_EXPIRE_TIME_LIST_HEAD_OFFSET))
#define SHM_EXPIRE_TIME_LIST_TAIL(base) ((PTR_OFFSET *)(base + SHM_EXPIRE_TIME_LIST_TAIL_OFFSET))
#define SHM_SPARE_LIST_HEAD(base)       ((PTR_OFFSET *)(base + SHM_SPARE_LIST_HEAD_OFFSET))
#define SHM_HASH_LISTS_HEADS(base)      ((PTR_OFFSET *)(base + SHM_HASH_LISTS_HEADS_OFFSET))
#define SHM_ENTRIES(base)               ((PTR_VOID )     (base + SHM_ENTRIES_OFFSET))

/* SHM Default Setting */
#define DEFAULT_ENTRY_SIZE      AGMDB_MAX_ENTRY_SIZE        // default size of each entry (1024B)
#define MAX_KEY_SIZE            128         // maximum key length (128B)

#define DEFAULT_SHM_SIZE        0x40000000  // the default size of database (256MB)
#define DEFAULT_HASH_LISTS_NUM  0x40000     // the number of hashes in database (256K)
#define DEFAULT_ENTRY_NUM       ((DEFAULT_SHM_SIZE - SHM_ENTRIES_OFFSET) / DEFAULT_ENTRY_SIZE) // the total number of entries
#define DEFAULT_EXPIRE_TIME     3600        // the default expire time (seconds) 

/* Shared Memory Functions */
/**
 ** Initialize the shared memory of AG Memory Database
 ** @param shm_base: The address of the shared memory.
 ** @param shm_size: The size of the shared memory.
 ** @param entry_num:The number of entries in the shared memory.
 ** return: AGMDB_SUCCESS if success,
 **         AGMDB_ERROR if failed.
 */
int SHM_init(PTR_VOID shm_base, unsigned int shm_size, unsigned int entry_num);

/**
 ** Create the shared memory of AG Memory Database if it doesn't exist
 ** Open the shared memory of AG Memory Database if it exists
 ** @param dbm:        The hanlder of the database.
 ** @param db_name:         The name of the database.
 ** @param db_name_length:  The length of db_name.
 ** @param entry_num:       The number of entries in the shared memory.
 ** return: AGMDB_SUCCESS_SHM_CREATE if successfully created a new shared memory,
 **         AGMDB_SUCCESS_SHM_OPEN if successfully link to an existed shared memory,
 **         AGMDB_ERROR if failed.
 */
int SHM_create(struct agmdb_handler* dbm, const char* db_name, int db_name_length, unsigned int entry_num);

/**
 ** Destroy the shared memory of AG Memory Database
 ** @param dbm: The handler of the database.
 ** return: AGMDB_SUCCESS if successfully destroy shared memory,
 **         AGMDB_ERROR if failed.
 */
int SHM_destroy(struct agmdb_handler *dbm);

/**
 ** Close the shared memory of AG Memory Database
 ** @param dbm: The handler of the database.
 ** return: AGMDB_SUCCESS if successfully close shared memory,
 **         AGMDB_ERROR if failed.
 */
int SHM_close(struct agmdb_handler* dbm);

/* Entry Structure and Addressing Macros */
/*
+----------+------------------------+
|0x00000000|ENTRY_STAT              |
+----------+------------------------+
|0x00000004|ENTRY_MODIFYTIME        |
+----------+------------------------+
|0x00000008|ENTRY_PREV              |
+----------+------------------------+
|0x00000010|ENTRY_NEXT              |
+----------+------------------------+
|0x00000018|ENTRY_TIME_NEXT         |
+----------+------------------------+
|0x00000020|ENTRY_TIME_PREV         |
+----------+------------------------+
|0x00000028|ENTRY_LENG              |
+----------+------------------------+
|0x0000002c|ENTRY_KEY               |
+----------+------------------------+
|0x000000ac|ENTRY_DATA              |
+----------+------------------------+
*/

#define ENTRY_STAT_OFFSET       0x00000
#define ENTRY_MODIFYTIME_OFFSET 0x00004
#define ENTRY_PREV_OFFSET       0x00008
#define ENTRY_NEXT_OFFSET       0x00010
#define ENTRY_TIME_NEXT_OFFSET  0x00018
#define ENTRY_TIME_PREV_OFFSET  0x00020
#define ENTRY_LENG_OFFSET       0x00028
#define ENTRY_KEY_OFFSET        0x0002c
#define ENTRY_DATA_OFFSET       (ENTRY_KEY_OFFSET+MAX_KEY_SIZE)

/* Entry Addressing Macros */

#define ENTRY_STAT(entry)       ((unsigned int *)(entry + ENTRY_STAT_OFFSET))        //status of this entry (reserved).
#define ENTRY_MODIFYTIME(entry) ((unsigned int *)(entry + ENTRY_MODIFYTIME_OFFSET))  //expire time of this entry
#define ENTRY_PREV(entry)       ((PTR_OFFSET *)  (entry + ENTRY_PREV_OFFSET))        //Relative addr of previous entry in hash linklist or spare linklist
#define ENTRY_NEXT(entry)       ((PTR_OFFSET *)  (entry + ENTRY_NEXT_OFFSET))        //Relative addr of next entry in hash linklist or spare linklist
#define ENTRY_TIME_PREV(entry)  ((PTR_OFFSET *)  (entry + ENTRY_TIME_PREV_OFFSET))   //Relative addr of previous entry in expire-time linklist
#define ENTRY_TIME_NEXT(entry)  ((PTR_OFFSET *)  (entry + ENTRY_TIME_NEXT_OFFSET))   //Relative addr of next entry in expire-time linklist
#define ENTRY_LENG(entry)       ((unsigned int *)(entry + ENTRY_LENG_OFFSET))        //length of data in this entry
#define ENTRY_KEY(entry)        ((char *)        (entry + ENTRY_KEY_OFFSET))         //key of this entry
#define ENTRY_DATA(entry)       ((char *)        (entry + ENTRY_DATA_OFFSET))        //data of this entry

#define ENTRY_ADDR(shm_base, offset) ((PTR_VOID)(shm_base + offset)) //find the address of entry with given index
#define ENTRY_MAX_DATA_SIZE     (DEFAULT_ENTRY_SIZE - ENTRY_DATA_OFFSET)    //max size of data of an entry


/**
 ** Insert an entry into spare linklist.
 ** @param shm_base:        The base address of shared memory.
 ** @param spare_list_head: The head of spare linklist.
 ** @param entry_id:        The index of the entry you want to insert.
 ** return: AGMDB_SUCCESS if success.
 */
int Entry_insertSpareList(CPTR_VOID shm_base, PTR_OFFSET* spare_list_head, PTR_OFFSET entry_id);

/**
 ** Remove an entry from spare linklist.
 ** @param shm_base:        The base address of shared memory.
 ** @param spare_list_head: The head of spare linklist.
 ** @param entry_id:         The index of gotten entry.
 ** return: AGMDB_SUCCESS if success,
            AGMDB_ERROR if failed.
 */
int Entry_removeSpareList(CPTR_VOID shm_base, PTR_OFFSET* spare_list_head, PTR_OFFSET* entry_id);

/**
 ** Insert an entry into hash linklist and increase the busy_entry_cnt by 1.
 ** @param shm_base:        The base address of shared memory.
 ** @param hash_list_head:  The head of spare linklist.
 ** @param entry_id:         The index of entry you want to insert.
 ** return: AGMDB_SUCCESS if success,
            AGMDB_ERROR if failed.
 */
int Entry_insertHashList(CPTR_VOID shm_base, PTR_OFFSET* hash_list_head, PTR_OFFSET entry_id);

/**
 ** Remove an entry from hash linklist and decrease the busy_entry_cnt by 1.
 ** @param shm_base:        The base address of shared memory.
 ** @param entry_id:         The index of entry you want to remove.
 ** return: AGMDB_SUCCESS if success,
            AGMDB_ERROR if failed.
 */
int Entry_removeHashList(CPTR_VOID shm_base, PTR_OFFSET entry_id);

/**
 ** Insert an entry into the tail of expiration time linklist.
 ** @param shm_base:        The base address of shared memory.
 ** @param entry_id:        The index of entry you want to insert.
 ** @param time_list_head:  The head of expiration time linklist.
 ** @param time_list_tail:  The tail of expiration time linklist.
 ** return: AGMDB_SUCCESS if success,
            AGMDB_ERROR if failed.
 */
int Entry_insertTimeList(CPTR_VOID shm_base, PTR_OFFSET entry_id, PTR_OFFSET* time_list_head, PTR_OFFSET* time_list_tail);

/**
 ** Insert an entry into the tail of expiration time linklist.
 ** @param shm_base:        The base address of shared memory.
 ** @param entry_id:        The index of entry you want to insert.
 ** @param time_list_head:  The head of expiration time linklist.
 ** @param time_list_tail:  The tail of expiration time linklist.
 ** return: AGMDB_SUCCESS if success,
            AGMDB_ERROR if failed.
 */
int Entry_removeTimeList(CPTR_VOID shm_base, PTR_OFFSET entry_id, PTR_OFFSET* time_list_head, PTR_OFFSET* time_list_tail);

/**
 ** Search the entry with given key in a linklist.
 ** @param shm_base:    The base address of shared memory.
 ** @param key:         The key you want to search.
 ** @param hashmap_index:   The hash index of the linklist you want to search.
 ** return: The entry's offset if found,
            or AGMDB_ERROR_P if not found.
 */
PTR_OFFSET Entry_searchHashlist(CPTR_VOID shm_base, const char* key, unsigned int hashmap_index);

/**
 ** Set the key and value into given entry.
 ** @param shm_base:    The base address of shared memory.
 ** @param entry_id:    The index of the entry you want to search.
 ** @param key:         The key string.
 ** @param key_len:     The length of key string.
 ** @param value:       The value string.
 ** @param value_len:   The length of value string.
 ** return: AGMDB_SUCCESS if success
            or AGMDB_ERROR if fail
 */
int Entry_setKeyValue(CPTR_VOID shm_base, PTR_OFFSET entry_id, const char* key, int key_len, const char* value, int value_len);

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
 **========================================================
 ** Semaphore Operator Structure and Function
 **========================================================
 */

/**
 ** Define the name prefix of Read/Write lock on Windows
 */
#ifdef _WIN32
#define READ_LOCK_PREFIX "DB_LOCK_READ_"
#define WRITE_LOCK_PREFIX "DB_LOCK_WRITE_"
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
 ** Create and initialize a agmdb_lock, if the lock with given key exists, just link to the lock.
 ** The new_lock should have been created before calling this function.
 ** @param new_lock:        The pointer to save the information of the lock.
 ** @param lock_name:       The name of lock.
 ** @param lock_name_length:The length of the lock_name.
 ** return: AGMDB_SUCCESS_LOCK_CREATE if successfully created a new lock,
 **         AGMDB_SUCCESS_LOCK_OPEN if successfully link to an existed lock,
 **         AGMDB_ERROR if failed.
 */
int Lock_create(struct agmdb_lock *new_lock, const char* lock_name, int lock_name_length);

/**
 ** Destroy the lock of AG Memory Database
 ** @param db_lock: The agmdb_lock sturcture
 ** return: AGMDB_SUCCESS if successfully destroy the lock,
 **         AGMDB_ERROR if failed.
 */
int Lock_destroy(struct agmdb_lock *db_lock);

/**
 ** Close the lock of AG Memory Database
 ** @param db_lock: The agmdb_lock sturcture
 ** return: AGMDB_SUCCESS if successfully close the lock,
 **         AGMDB_ERROR if failed.
 */
int Lock_close(struct agmdb_lock *db_lock);

/**
 ** Decrease a lock's value by a given number.
 ** @param db_lock: The agmdb_lock sturcture
 ** @param index:   The index of atom lock (read or write) .
 ** @param val:     The value you want to decrease from the lock.
 ** return: AGMDB_SUCCESS if success;
 **         or AGMDB_ERROR if failed
 */
int Lock_P(const struct agmdb_lock *db_lock, int index, int val);

/**
 ** Increase a lock's value by a given number.
 ** @param db_lock: The agmdb_lock sturcture
 ** @param index:   The index of atom lock (read or write) .
 ** @param val:     The value you want to add to the lock.
 ** return: AGMDB_SUCCESS if success;
 **         or AGMDB_ERROR if failed
 */
int Lock_V(const struct agmdb_lock *db_lock, int index, int val);

/**
 **========================================================
 ** Other Function
 **========================================================
 */

/**
 ** Check the string format.
 ** @param str: the string you want to check.
 ** @param str_len: given string length (not include '\0').
 ** return: AGMDB_SUCCESS if the string length equals str_len;
 **             AGMDB_ERROR if not
 */
int AGMDB_isstring(const char* str, int str_len);

/**
 ** Use AGMDB's hash function to hash a string into an integer.
 ** @param key: the key string.
 ** @param key_len: the length of the key string.
 ** @param output_val_range: the range of output value.
 ** return: hash result if success
 **      or AGMDB_ERROR if failed.
 */
unsigned int AGMDB_hash(const char* key, int key_len, unsigned int output_val_range);

/**
** Initialize the handle of AG Memory Database.
** @param dbm: the handle of AG Memory Database.
** return:  if AGMDB_SUCCESS if success
**          or AGMDB_ERROR if the handle is NULL.
*/
int Handle_init(struct agmdb_handler* dbm);

#ifdef __cplusplus
}
#endif

#endif