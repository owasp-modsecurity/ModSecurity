#ifndef _AG_MDB_EXTERNAL_HEADER
#define _AG_MDB_EXTERNAL_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"

/**
 **========================================================
 ** AG Memory Database Limitation Definition
 **========================================================
 */
#define AGMDB_MAX_ENTRY_SIZE 1024



/**
 **========================================================
 ** AG Memory Database Status Definition
 **========================================================
 */
/**
 ** First Digit: equals 1 if it is an error code.
 ** Second Digit: the module where the error happens. 
 **               0 means lock, 1 means shared momory, 2 means the database.
 ** Third Digit: the platform where the error happens. 
 **              0 means general, 1 means LINUX, 2 means WINDOWS.
 ** 4th & 5th Digit: the detailed error number.
 */
#define AGMDB_SUCCESS                                           0        
#define AGMDB_ERROR                                             10000

#define AGMDB_ERROR_LOCK_OP_NEGATIVE_VAL                        10001

#define AGMDB_ERROR_LOCK_LINUX_SEM_CREATE_FAIL                  10100
#define AGMDB_ERROR_LOCK_LINUX_SEM_OPEN_FAIL                    10101
#define AGMDB_ERROR_LOCK_LINUX_SEM_INIT_FAIL                    10102
#define AGMDB_ERROR_LOCK_LINUX_SEM_MODIFY_FAIL                  10103

#define AGMDB_ERROR_LOCK_WIN_NAME_INVALID_STRING                10200
#define AGMDB_ERROR_LOCK_WIN_MUTEX_CREATE_FAIL                  10201
#define AGMDB_ERROR_LOCK_WIN_ONLY_ONE_LOCK_EXISTS               10202
#define AGMDB_ERROR_LOCK_WIN_GET_MUTEX_FAIL                     10203
#define AGMDB_ERROR_LOCK_WIN_RELEASE_MUTEX_FAIL                 10204

#define AGMDB_ERROR_SHM_BASE_NULL                               11000
#define AGMDB_ERROR_SHM_SIZE_TOO_SMALL                          11001
#define AGMDB_ERROR_SHM_NAME_INVALID_STRING                     11002
#define AGMDB_ERROR_SHM_ENTRY_NUM_NEGATIVE                      11003

#define AGMDB_ERROR_SHM_LINUX_CREATE_FAIL                       11100
#define AGMDB_ERROR_SHM_LINUX_OPEN_FAIL                         11101
#define AGMDB_ERROR_SHM_LINUX_OPEN_ACCESS_FAIL                  11102
#define AGMDB_ERROR_SHM_LINUX_MAP_FAIL                          11103

#define AGMDB_ERROR_SHM_WIN_CREATE_FAIL                         11200
#define AGMDB_ERROR_SHM_WIN_MAP_FAIL                            11201

#define AGMDB_ERROR_INSERT_INVALID_ENTRY_INTO_SPARELIST         12000
#define AGMDB_ERROR_INSERT_BUSY_ENTRY_INTO_SPARELIST            12001

#define AGMDB_ERROR_INSERT_INVALID_ENTRY_INTO_HASHLIST          12002
#define AGMDB_ERROR_INSERT_BUSY_ENTRY_INTO_HASHLIST             12003

#define AGMDB_ERROR_REMOVE_INVALID_ENTRY_FROM_HASHLIST          12004
#define AGMDB_ERROR_NONHEAD_ENTRY_IN_HASHLIST_WITHOUT_PREV      12005

#define AGMDB_ERROR_INSERT_INVALID_ENTRY_INTO_TIMELIST          12006
#define AGMDB_ERROR_REMOVE_INVALID_ENTRY_FROM_TIMELIST          12007

#define AGMDB_ERROR_SET_KEYVAL_OF_INVALID_ENTRY                 12008

#define AGMDB_ERROR_KEY_INVALID_STRING                          12009
#define AGMDB_ERROR_KEY_TOO_LONG                                12010
#define AGMDB_ERROR_VALUE_INVALID_STRING                        12011
#define AGMDB_ERROR_VALUE_TOO_LONG                              12012

#define AGMDB_ERROR_HANDLE_NULL                                 12013
#define AGMDB_ERROR_DELETE_INVALID_ENTRY                        12014
#define AGMDB_ERROR_NAME_NULL                                   12015
#define AGMDB_ERROR_NAME_INVALID_STRING                         12016

#define AGMDB_ERROR_SET_NEGATIVE_EXIPRE_TIME                    12017

#define AGMDB_ERROR_GET_BUFFER_NULL                             12018
#define AGMDB_ERROR_GET_INVALID_BUFFER_LEN                      12019
#define AGMDB_ERROR_GET_BUFFER_TOO_SMALL                        12020

#define AGMDB_ERROR_TIMELIST_LONG_NOTEQUEAL_SHM_CNT             12021

#define AGMDB_ERROR_GETALL_ARRAY_TOO_SMALL                      12022

/**
 **========================================================
 ** AG Memory Database Handler Definition
 **========================================================
 */

/**
 ** The structure stores the Read/Write locks information. 
 ** In Linux, a set of semaphores is used to implement the Read/Write locks.
 ** In Windows, The Windows Mutex is used to implement the Read/Write locks.
 ** @param sem_id: The identifier of semaphore set in Linux.
 ** @param read_lock_handle: The handler of the read locks in Windows.
 ** @param write_lock_handle: The handler of the write lock in Windows.
 */ 
struct agmdb_lock{
#ifndef _WIN32
    int sem_id;
#else
    HANDLE read_lock_handle;
    HANDLE write_lock_handle;
#endif
};

/**
 ** Handler of AG-MDB, which contains a block of shared momory and the lock structure.
 ** @param shm_base: The base address of the shared memory.
 ** @param db_lock: The structure that stores the lock information.
 */ 
struct agmdb_handler{
    const void* shm_base;
    struct agmdb_lock db_lock;
};

/**
 **========================================================
 ** AG Memory Database Lock API
 **========================================================
 */

/**
 ** Get a shared lock for read only.
 ** @param dbm: the database you want to lock.
 ** return: AGMDB_SUCCESS if successfully created or AGMDB_ERROR if failed.
 */
int AGMDB_getSharedLock(struct agmdb_handler *dbm);

/**
 ** Get a exclusive lock for read and write.
 ** @param dbm: the database you want to lock.
 ** return: AGMDB_SUCCESS if successfully created or AGMDB_ERROR if failed.
 */
int AGMDB_getExclusiveLock(struct agmdb_handler *dbm);

/**
 ** Free a shared lock that you have got before.
 ** @param dbm: the database you want to return the lock.
 ** return: AGMDB_SUCCESS if successfully created or AGMDB_ERROR if failed.
 */
int AGMDB_freeSharedLock(struct agmdb_handler *dbm);

/**
 ** Free a exclusive lock that you have got before.
 ** @param dbm: the database you want to return the lock.
 ** return: AGMDB_SUCCESS if successfully created or AGMDB_ERROR if failed.
 */
int AGMDB_freeExclusiveLock(struct agmdb_handler *dbm);

/**
 **========================================================
 ** AG Memory Database Operation API
 **========================================================
 */

/**
 ** Open a database with given name, and intialize the agmdb handler for further operation.
 ** If the database doesn't exist, a new database will be created.
 ** @param dbm: a created sturcture to save the database information.
 ** @param db_name: the unique identifier of a database.
 ** @param db_name_length: the length of the unique_name.
 ** @param entry_num: The maximum number of entries in the database.
 ** return: AGMDB_SUCCESS if successfully created or AGMDB_ERROR if failed.
 */
int AGMDB_openDB(struct agmdb_handler* dbm, const char* db_name, int db_name_length, unsigned int entry_num);

/**
 ** Close and destroy a database. 
 ** @param dbm: the database you want to destroy.
 ** return: AGMDB_SUCCESS if successfully destroyed or AGMDB_ERROR if failed.
 */
int AGMDB_destroyDB(struct agmdb_handler *dbm);

/**
 ** Close a database, but does not destroy it. 
 ** @param dbm: the database you want to close.
 ** return: AGMDB_SUCCESS if successfully closed or AGMDB_ERROR if failed.
 */
int AGMDB_closeDB(struct agmdb_handler *dbm);

/**
 ** Remove all entries in a database.
 ** You have to get EXCLUSIVE LOCK of the database before calling this function.
 ** @param dbm: the database you want to modify.
 ** return: AGMDB_SUCCESS if success
 **      or AGMDB_ERROR if failed.
 */
int AGMDB_cleanDB(struct agmdb_handler *dbm);

/**
 ** Reset the expiration time of the database.
 ** You have to get EXCLUSIVE LOCK of the database before calling this function.
 ** Default expiration time is 3600 seconds.
 ** @param expire_time: how many seconds all entries in database will be kept. 
 */
int AGMDB_setExpireTime(struct agmdb_handler *dbm, unsigned int expire_time); 
 
/**
 ** Get the value from a database with the given key.
 ** You have to get SHARED or EXCLUSIVE LOCK of the database before calling this function.
 ** @param dbm: the database you want to read.
 ** @param key: the key string.
 ** @param key_len: the length of key string.
 ** @param buffer: An created buffer to save the value.
 ** @param buffer_len: the length of buffer array.
 ** @param val_len: the length of value if the key is fetched or not in the database (buffer will be NULL and return value will be 0 if key does not exist)
 ** return: AGMDB_SUCCESS if success
 **      or AGMDB_ERROR if failed (cannot get access to database, or the buffer is not long enough).
 */
int AGMDB_get(struct agmdb_handler *dbm, const char* key, int key_len, char* buffer, int buffer_len, int* val_len);

/**
 ** Set the value into database with given key.
 ** You have to get EXCLUSIVE LOCK of the database before calling this function.
 ** @param dbm: the database you want to modify.
 ** @param key: the key string.
 ** @param key_len: the length of key string.
 ** @param value: the value string.
 ** @param value: the length of value string.
 ** return: AGMDB_SUCCESS if successfully set
 **      or AGMDB_ERROR if failed.
 */
int AGMDB_set(struct agmdb_handler *dbm, const char* key, int key_len, const char* value, int value_len);

/**
 ** Delete an entry with a given key in a database.
 ** You have to get EXCLUSIVE LOCK of the database before calling this function.
 ** @param dbm: the database you want to modify.
 ** @param key: the key string.
 ** @param key_len: the length of key string.
 ** return: AGMDB_SUCCESS if the key is deleted or not in the database
 **      or AGMDB_ERROR if failed.
 */
int AGMDB_delete(struct agmdb_handler *dbm, const char* key, int key_len);

/**
 ** Remove all staled entries.
 ** You have to get EXCLUSIVE LOCK of the database before calling this function.
 ** @param dbm: the database you want to remove the stale entries.
 ** return: AGMDB_SUCCESS if success
 **      or AGMDB_ERROR if failed.
 */
int AGMDB_removeStale(struct agmdb_handler *dbm);

/**
 **========================================================
 ** AG Memory Database Debug API
 **========================================================
 */
/**
 ** Get the detail information of an return code.
 ** @param return_code: the code returned by a function.
 ** return: The string including the detailed information.
 */
const char* AGMDB_getErrorInfo(int return_code);

/**
 ** Check whether a return_code is an error.
 ** @param return_code: the code returned by a AGMDB function.
 ** return: True if there is an error;
            False if not.
 */
bool AGMDB_isError(int return_code);
/**
 ** Get the number of keys in a database.
 ** You have to get SHARED or EXCLUSIVE LOCK of the database before calling this function.
 ** @param dbm: the database you want to read.
 ** @param keynum: the value to store the number of keys in the database 
 ** return: AGMDB_SUCCESS if no error
 **      or AGMDB_ERROR if failed.
 */
int AGMDB_getKeyNum(struct agmdb_handler *dbm, int* keynum);

/**
 ** Use AGMDB's hash function to hash a string into an integer.
 ** @param key: the key string.
 ** @param key_len: the length of the key string.
 ** @param hash_nums: the range of hash.
 ** return: hash result if success
 **      or AGMDB_ERROR if failed.
 */
int AGMDB_getHashValue(const char* key, int key_len, int hash_nums);

/**
 ** Get the const pointers of all keys and values in a database.
 ** You have to get SHARED or EXCLUSIVE LOCK of the database before calling this function.
 ** You should call AGMDB_getKeyNum() before to get the number of keys, then assign proper space for keys and values. Although, you can just assign a large enough space for these two arrays.
 ** @param dbm:         the database you want to read.
 ** @param array_size:  the maximum size of key array and value array 
 ** @param keys:        the array to store pointers of keys.
 ** @param values:      the array to store pointers of values.
 ** @param vals_len:    the array to store length of values.
 ** return: AGMDB_SUCCESS if no error
 **      or AGMDB_ERROR if failed.
 */
int AGMDB_getAllKeysValues(struct agmdb_handler *dbm, int array_size, const char* keys[], const char * values[], unsigned int vals_len[]);

#ifdef __cplusplus
}
#endif

#endif


