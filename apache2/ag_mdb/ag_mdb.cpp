#include"ag_mdb_external.h"
#include"ag_mdb_internal.h"


/**
**========================================================
** AG Memory Database Internal Function
**========================================================
*/

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
int Lock_create(struct agmdb_lock *new_lock, const char* lock_name, int lock_name_length) {
#ifndef _WIN32
    union semun sem_union;
    int lock_id = -1;
#else
    int read_lock_name_len = 0;
    int write_lock_name_len = 0;
    char* read_lock_name = NULL;
    char* write_lock_name = NULL;
    bool lock_exists = false;
#endif
    if (new_lock == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
#ifndef _WIN32
    lock_id = AGMDB_hash(lock_name, lock_name_length, DEFAULT_AGMDB_ID_RANGE);
    new_lock->sem_id = semget(lock_id, SEM_NUMBERS, IPC_CREAT | IPC_EXCL);
    if (new_lock->sem_id != -1) {
        // A new semaphore set is created, need to initialize the semaphore set
        sem_union.val = SEM_READ_INITVAL;
        if (semctl(new_lock->sem_id, SEM_ID_READ, SETVAL, sem_union) == -1) {
            Lock_destroy(new_lock);
            return AGMDB_ERROR_LOCK_LINUX_SEM_CREATE_FAIL;
        }

        sem_union.val = SEM_WRITE_INITVAL;
        if (semctl(new_lock->sem_id, SEM_ID_WRITE, SETVAL, sem_union) == -1) {
            // If failed, destroy the lock
            Lock_destroy(new_lock);
            return AGMDB_ERROR_LOCK_LINUX_SEM_INIT_FAIL;
        }
        return AGMDB_SUCCESS_LOCK_CREATE;
    }
    else {
        new_lock->sem_id = semget(lock_id, SEM_NUMBERS, IPC_CREAT);
        if (new_lock->sem_id == -1) {
            return AGMDB_ERROR_LOCK_LINUX_SEM_OPEN_FAIL;
        }
        return AGMDB_SUCCESS_LOCK_OPEN;
    }
#else
    read_lock_name_len = (strlen(READ_LOCK_PREFIX) + strlen(lock_name) + 1) * sizeof(char);
    write_lock_name_len = (strlen(WRITE_LOCK_PREFIX) + strlen(lock_name) + 1) * sizeof(char);

    if (AGMDB_isstring(lock_name, lock_name_length) != AGMDB_SUCCESS)
        return AGMDB_ERROR_LOCK_WIN_NAME_INVALID_STRING;

    read_lock_name = (char *)malloc(read_lock_name_len * sizeof(char));
    sprintf_s(read_lock_name, read_lock_name_len, "%s%s", READ_LOCK_PREFIX, lock_name);
    new_lock->read_lock_handle = CreateMutex(
        NULL,               // Default security settings.
        FALSE,              // Do not take the lock after created.
        read_lock_name);    // The name of read lock.
    free(read_lock_name);

    if (new_lock->read_lock_handle == NULL) {
        new_lock->read_lock_handle = INVALID_HANDLE_VALUE;
        return AGMDB_ERROR_LOCK_WIN_MUTEX_CREATE_FAIL;
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS)
        lock_exists = true;

    write_lock_name = (char *)malloc(write_lock_name_len * sizeof(char));
    sprintf_s(write_lock_name, write_lock_name_len, "%s%s", WRITE_LOCK_PREFIX, lock_name);
    new_lock->write_lock_handle = CreateMutex(
        NULL,               // Default security settings.
        FALSE,              // Do not take the lock after created.
        write_lock_name);   // The name of write lock.
    free(write_lock_name);

    if (new_lock->write_lock_handle == NULL) {
        CloseHandle(new_lock->read_lock_handle);
        new_lock->read_lock_handle = INVALID_HANDLE_VALUE;
        new_lock->write_lock_handle = INVALID_HANDLE_VALUE;
        return AGMDB_ERROR_LOCK_WIN_MUTEX_CREATE_FAIL;
    }

    if ((GetLastError() == ERROR_ALREADY_EXISTS) != lock_exists) {
        // One lock exists, another not.
        CloseHandle(new_lock->read_lock_handle);
        new_lock->read_lock_handle = INVALID_HANDLE_VALUE;
        CloseHandle(new_lock->write_lock_handle);
        new_lock->write_lock_handle = INVALID_HANDLE_VALUE;
        return AGMDB_ERROR_LOCK_WIN_ONLY_ONE_LOCK_EXISTS;
    }

    if (lock_exists == true)
        return AGMDB_SUCCESS_LOCK_OPEN;
    else
        return AGMDB_SUCCESS_LOCK_CREATE;
#endif
    // Should never get here
    return AGMDB_SUCCESS_LOCK_CREATE;
}

/**
** Destroy the lock of AG Memory Database
** @param db_lock: The agmdb_lock sturcture
** return: AGMDB_SUCCESS if successfully destroy the lock,
**         AGMDB_ERROR if failed.
*/
int Lock_destroy(struct agmdb_lock *db_lock) {
    int rc = AGMDB_SUCCESS;
    if (db_lock == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
    rc = Lock_close(db_lock);
    if (AGMDB_isError(rc))
        return rc;
#ifndef _WIN32
    rc = semctl(db_lock->sem_id, 0, IPC_RMID);
    if (rc == -1)
        return AGMDB_ERROR_LOCK_LINUX_SEM_DESTROY_FAIL;
    else
        db_lock->sem_id = -1;
    return AGMDB_SUCCESS;
#else
    /* Locks destroy doesn't support on Windows */
    return AGMDB_ERROR_LOCK_WIN_DESTROY_NOT_SUPPORT;
#endif
}

/**
** Close the lock of AG Memory Database
** @param db_lock: The agmdb_lock sturcture
** return: AGMDB_SUCCESS if successfully close the lock,
**         AGMDB_ERROR if failed.
*/
int Lock_close(struct agmdb_lock *db_lock) {
    if (db_lock == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
    /* Linux doesn't do anything */
#ifdef _WIN32
    BOOL rc_read = 1;
    BOOL rc_write = 1;
    if (db_lock->read_lock_handle != INVALID_HANDLE_VALUE)
    {
        rc_read = CloseHandle(db_lock->read_lock_handle);
        if (rc_read != 0)
            db_lock->read_lock_handle = INVALID_HANDLE_VALUE;
    }

    if (db_lock->write_lock_handle != INVALID_HANDLE_VALUE)
    {
        rc_write = CloseHandle(db_lock->write_lock_handle);
        if (rc_write != 0)
            db_lock->write_lock_handle = INVALID_HANDLE_VALUE;
    }

    if (rc_read == 0 || rc_write == 0)
        return AGMDB_ERROR_LOCK_WIN_CLOSE_MUTEX_FAIL;
#endif
    return AGMDB_SUCCESS;
}

/**
** Decrease a lock's value by a given number.
** @param db_lock: The agmdb_lock sturcture
** @param index:   The index of atom lock (read or write) .
** @param val:     The value you want to decrease from the lock.
** return: AGMDB_SUCCESS if success;
**         or AGMDB_ERROR if failed
*/
int Lock_P(const struct agmdb_lock *db_lock, int index, int val) {
#ifndef _WIN32
    struct sembuf sem_op;
    if (val < 0)
        return AGMDB_ERROR_LOCK_OP_NEGATIVE_VAL;

    sem_op.sem_num = index;
    sem_op.sem_op = -val;
    sem_op.sem_flg = SEM_UNDO;
    if (semop(db_lock->sem_id, &sem_op, 1) == -1)
        return AGMDB_ERROR_LOCK_LINUX_SEM_MODIFY_FAIL;
    return AGMDB_SUCCESS;
#else
    int rc;
    HANDLE lock_handle;
    if (val < 0)
        return AGMDB_ERROR_LOCK_OP_NEGATIVE_VAL;

    if (index == SEM_ID_READ)
        lock_handle = db_lock->read_lock_handle;
    else
        lock_handle = db_lock->write_lock_handle;
    rc = WaitForSingleObject(lock_handle, INFINITE);
    if (rc != WAIT_OBJECT_0)
        return AGMDB_ERROR_LOCK_WIN_GET_MUTEX_FAIL;
    return AGMDB_SUCCESS;
#endif
}

/**
** Increase a lock's value by a given number.
** @param db_lock: The agmdb_lock sturcture
** @param index:   The index of atom lock (read or write) .
** @param val:     The value you want to add to the lock.
** return: AGMDB_SUCCESS if success;
**         or AGMDB_ERROR if failed
*/
int Lock_V(const struct agmdb_lock *db_lock, int index, int val) {
#ifndef _WIN32
    struct sembuf sem_op;
    if (val < 0)
        return AGMDB_ERROR_LOCK_OP_NEGATIVE_VAL;

    sem_op.sem_num = index;
    sem_op.sem_op = val;
    sem_op.sem_flg = SEM_UNDO;
    if (semop(db_lock->sem_id, &sem_op, 1) == -1)
        return AGMDB_ERROR_LOCK_LINUX_SEM_MODIFY_FAIL;
    return AGMDB_SUCCESS;
#else
    int rc;
    HANDLE lock_handle;
    if (val < 0)
        return AGMDB_ERROR_LOCK_OP_NEGATIVE_VAL;

    if (index == SEM_ID_READ)
        lock_handle = db_lock->read_lock_handle;
    else
        lock_handle = db_lock->write_lock_handle;
    rc = ReleaseMutex(lock_handle);
    if (rc == 0)
        return AGMDB_ERROR_LOCK_WIN_RELEASE_MUTEX_FAIL;
    return AGMDB_SUCCESS;
#endif
}

/**
** Initialize the shared memory of AG Memory Database
** @param shm_base: The address of the shared memory.
** @param shm_size: The size of the shared memory.
** @param entry_num:The number of entries in the shared memory.
** return: AGMDB_SUCCESS if success,
**         AGMDB_ERROR if failed.
*/
int SHM_init(PTR_VOID shm_base, unsigned int shm_size, unsigned int entry_num) {
    PTR_OFFSET* spare_head;
    PTR_OFFSET* hash_heads;
    unsigned int hash_list_index;
    PTR_VOID   entry_array;
    PTR_VOID   current_entry_addr;
    PTR_VOID   previous_entry_addr;
    PTR_OFFSET current_entry_id;
    PTR_OFFSET previous_entry_id;
    unsigned int entry_size;

    if (shm_base == NULL)
        return AGMDB_ERROR_SHM_BASE_NULL;
    if (shm_size < SHM_ENTRIES_OFFSET)
        return AGMDB_ERROR_SHM_SIZE_TOO_SMALL;
    memset(shm_base, 0, shm_size);

    // Initialize SHM information
    *SHM_SIZE(shm_base) = shm_size;
    *SHM_HASH_LISTS_NUM(shm_base) = DEFAULT_HASH_LISTS_NUM;
    *SHM_ENTRIES_NUM(shm_base) = entry_num;
    *SHM_ENTRY_SIZE(shm_base) = DEFAULT_ENTRY_SIZE;
    *SHM_BUSY_ENTRY_CNT(shm_base) = 0;
    *SHM_BUSY_HASH_CNT(shm_base) = 0;

    // Initialize the expiration time and its linklist
    *SHM_EXPIRE_TIME_LIST_HEAD(shm_base) = AGMDB_INVALID_INDEX;
    *SHM_EXPIRE_TIME_LIST_TAIL(shm_base) = AGMDB_INVALID_INDEX;
    *SHM_EXPIRE_TIME(shm_base) = DEFAULT_EXPIRE_TIME;

    // Initialize the spare link list
    spare_head = SHM_SPARE_LIST_HEAD(shm_base);
    entry_size = *SHM_ENTRY_SIZE(shm_base);
    if (shm_size < SHM_ENTRIES_OFFSET + entry_size)
        *spare_head = AGMDB_INVALID_INDEX;
    else
        *spare_head = SHM_ENTRIES_OFFSET;

    // Initialize the hash linklists heads 
    hash_heads = SHM_HASH_LISTS_HEADS(shm_base);
    for (hash_list_index = 0; hash_list_index < DEFAULT_HASH_LISTS_NUM; hash_list_index++) {
        hash_heads[hash_list_index] = AGMDB_INVALID_INDEX;
    }

    // Initialize all entries 
    entry_array = SHM_ENTRIES(shm_base);
    current_entry_id = SHM_ENTRIES_OFFSET;
    current_entry_addr = entry_array;
    previous_entry_id = AGMDB_INVALID_INDEX;
    previous_entry_addr = AGMDB_INVALID_ADDR;
    while (current_entry_id + entry_size <= shm_size) {
        if (previous_entry_id != AGMDB_INVALID_INDEX)
            *ENTRY_NEXT(previous_entry_addr) = current_entry_id;
        // Initialize the entry
        *ENTRY_PREV(current_entry_addr) = previous_entry_id;
        *ENTRY_NEXT(current_entry_addr) = AGMDB_INVALID_INDEX;
        *ENTRY_TIME_PREV(current_entry_addr) = AGMDB_INVALID_INDEX;
        *ENTRY_TIME_NEXT(current_entry_addr) = AGMDB_INVALID_INDEX;
        previous_entry_addr = current_entry_addr;
        previous_entry_id = current_entry_id;
        current_entry_addr += entry_size;
        current_entry_id += entry_size;
    }
    return AGMDB_SUCCESS;
}

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
int SHM_create(struct agmdb_handler* dbm, const char* db_name, int db_name_length, unsigned int entry_num) {
    unsigned int shm_size = SHM_ENTRIES_OFFSET + entry_num * DEFAULT_ENTRY_SIZE;
    int rc;
#ifndef _WIN32    
    int shm_id;
    int shm_key;
#else
    bool shm_is_create = true;
#endif
    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
#ifndef _WIN32
    if (AGMDB_isstring(db_name, db_name_length) != AGMDB_SUCCESS)
        return AGMDB_ERROR_SHM_NAME_INVALID_STRING;

    shm_key = AGMDB_hash(db_name, db_name_length, DEFAULT_AGMDB_ID_RANGE);
    if (entry_num < 0)
        return AGMDB_ERROR_SHM_ENTRY_NUM_NEGATIVE;

    shm_id = shmget(shm_key, shm_size, IPC_CREAT | IPC_EXCL);
    if (shm_id == -1) {
        if (errno == EEXIST) { // There has been a SHM with the given name 
            shm_id = shmget(shm_key, shm_size, IPC_CREAT);
            if (shm_id == -1)
                return AGMDB_ERROR_SHM_LINUX_CREATE_FAIL;

            //  Map the SHM into the address space of this process 
            dbm->linux_shm_id = shm_id;
            dbm->shm_base = (PTR_VOID)shmat(shm_id, NULL, 0);
            if (dbm->shm_base == (PTR_VOID)-1) {
                dbm->shm_base = NULL;
                SHM_close(dbm);
                return AGMDB_ERROR_SHM_LINUX_MAP_FAIL;
            }
            return AGMDB_SUCCESS_SHM_OPEN;
        }
        else if (errno == EACCES) {
            return AGMDB_ERROR_SHM_LINUX_OPEN_ACCESS_FAIL;
        }
        else {
            return AGMDB_ERROR_SHM_LINUX_OPEN_FAIL;
        }
    }
    else { // Map the new SHM into the address space of this process 
        dbm->linux_shm_id = shm_id;
        dbm->shm_base = (PTR_VOID)shmat(shm_id, NULL, 0);
        if (dbm->shm_base == (PTR_VOID)-1) {
            dbm->shm_base = NULL;
            SHM_destroy(dbm);
            return AGMDB_ERROR_SHM_LINUX_MAP_FAIL;
        }
        rc = SHM_init((PTR_VOID)dbm->shm_base, shm_size, entry_num);
        if (rc != AGMDB_SUCCESS) {
            SHM_destroy(dbm);
            return rc;
        }
        return AGMDB_SUCCESS_SHM_CREATE;
    }
#else
    if (AGMDB_isstring(db_name, db_name_length) != AGMDB_SUCCESS)
        return AGMDB_ERROR_SHM_NAME_INVALID_STRING;

    dbm->win_shm_handle = CreateFileMapping(
        INVALID_HANDLE_VALUE,   // A memory, not a real file. 
        NULL,                   // Defaut security setting.
        PAGE_READWRITE,         // Read and Write permission.
        0,                      // High 32-bit of shm size.
        shm_size,               // Low 32-bit of shm size.
        db_name);               // Name of shared memory.
    if (dbm->win_shm_handle == NULL) {
        dbm->win_shm_handle = INVALID_HANDLE_VALUE;
        return AGMDB_ERROR_SHM_WIN_CREATE_FAIL;
    }
    else if (GetLastError() == ERROR_ALREADY_EXISTS) {
        // Existing Shared Memory
        shm_is_create = false;
    }

    dbm->shm_base = (PTR_VOID)MapViewOfFile(
        dbm->win_shm_handle,    // Handle of file.
        FILE_MAP_ALL_ACCESS,    // All permissioon.
        0,                      // Map from the beginning of the shared memory.
        0,                      // Map from the beginning of the shared memory.
        0);                     // Map entire shared memory into address space.
    if (dbm->shm_base == NULL) {
        if (shm_is_create)
            SHM_destroy(dbm);
        else
            SHM_close(dbm);
        return AGMDB_ERROR_SHM_WIN_MAP_FAIL;
    }

    if (shm_is_create) { // A New Shared Memory
        rc = SHM_init((PTR_VOID)dbm->shm_base, shm_size, entry_num);
        if (rc != AGMDB_SUCCESS) {
            if (shm_is_create)
                SHM_destroy(dbm);
            else
                SHM_close(dbm);
            return rc;
        }
        return AGMDB_SUCCESS_SHM_CREATE;
    }
    else { // There has been a SHM with the given name
        return AGMDB_SUCCESS_SHM_OPEN;
    }
#endif
}

/**
** Destroy the shared memory of AG Memory Database
** @param dbm: The handler of the database.
** return: AGMDB_SUCCESS if successfully destroy shared memory,
**         AGMDB_ERROR if failed.
*/
int SHM_destroy(struct agmdb_handler *dbm) {
    int rc;
    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
    rc = SHM_close(dbm);
    if (AGMDB_isError(rc))
        return rc;
    
#ifndef _WIN32
    if (dbm->linux_shm_id != -1) {
        rc = shmctl(dbm->linux_shm_id, IPC_RMID, 0);
        if (rc == -1)
            return AGMDB_ERROR_SHM_LINUX_DESTROY_FAIL;
        dbm->linux_shm_id = -1;
    }
    return AGMDB_SUCCESS;
#else
    /* Shared memory destroy doesn't support on Windows */
    return AGMDB_ERROR_SHM_WIN_DESTROY_NOT_SUPPORT;
#endif
}

/**
** Close the shared memory of AG Memory Database
** @param dbm: The handler of the database.
** return: AGMDB_SUCCESS if successfully close shared memory,
**         AGMDB_ERROR if failed.
*/
int SHM_close(struct agmdb_handler* dbm) {
#ifndef _WIN32
    int rc = 0;
#else
    BOOL rc = 0;
    int ret_win = AGMDB_SUCCESS;
#endif        
    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
#ifndef _WIN32
    if (dbm->shm_base != NULL) {
        rc = shmdt(dbm->shm_base);
        if (rc == -1)
            return AGMDB_ERROR_SHM_LINUX_DETACH_FAIL;
        dbm->shm_base = NULL;
    }
    return AGMDB_SUCCESS;
#else
    if (dbm->shm_base != NULL) {
        rc = UnmapViewOfFile(dbm->shm_base);
        if (rc == 0)
            ret_win = AGMDB_ERROR_SHM_WIN_UNMAP_FAIL;
        else
            dbm->shm_base = NULL;
    }

    if (dbm->win_shm_handle != INVALID_HANDLE_VALUE) {
        rc = CloseHandle(dbm->win_shm_handle);
        if (rc == 0) {
            if (ret_win == AGMDB_ERROR_SHM_WIN_UNMAP_FAIL)
                ret_win = AGMDB_ERROR_SHM_WIN_UNMAP_AND_CLOSE_HANDLE_FAIL;
            else
                ret_win = AGMDB_ERROR_SHM_WIN_CLOSE_HANDLE_FAIL;
        }
        else {
            dbm->win_shm_handle = INVALID_HANDLE_VALUE;
        }
    }
    return ret_win;
#endif
}

/**
** Insert an entry into spare linklist.
** @param shm_base:        The base address of shared memory.
** @param spare_list_head: The head of spare linklist.
** @param entry_id:        The offset of the entry you want to insert.
** return: AGMDB_SUCCESS if success.
*/
int Entry_insertSpareList(CPTR_VOID shm_base, PTR_OFFSET* spare_list_head, PTR_OFFSET entry_id) {
    PTR_VOID entry_addr = ENTRY_ADDR(shm_base, entry_id);
    if (entry_id == AGMDB_INVALID_INDEX)
        return AGMDB_ERROR_INSERT_INVALID_ENTRY_INTO_SPARELIST;
    if (*ENTRY_PREV(entry_addr) != AGMDB_INVALID_INDEX ||
        *ENTRY_NEXT(entry_addr) != AGMDB_INVALID_INDEX)
        return AGMDB_ERROR_INSERT_BUSY_ENTRY_INTO_SPARELIST;
    *ENTRY_PREV(entry_addr) = AGMDB_INVALID_INDEX;
    *ENTRY_NEXT(entry_addr) = *spare_list_head;
    if (*spare_list_head != AGMDB_INVALID_INDEX)
        *ENTRY_PREV(ENTRY_ADDR(shm_base, *spare_list_head)) = entry_id;
    *spare_list_head = entry_id;
    return AGMDB_SUCCESS;
}

/**
** Remove an entry from spare linklist.
** @param shm_base:        The base address of shared memory.
** @param spare_list_head: The head of spare linklist.
** @param entry_id:        The offset of gotten entry.
** return: AGMDB_SUCCESS if success,
AGMDB_ERROR if failed.
*/
int Entry_removeSpareList(CPTR_VOID shm_base, PTR_OFFSET* spare_list_head, PTR_OFFSET* entry_id) {
    PTR_VOID entry_addr;

    // Get the first entry in the spare list
    *entry_id = *spare_list_head;
    entry_addr = ENTRY_ADDR(shm_base, *entry_id);

    if (*ENTRY_NEXT(entry_addr) != AGMDB_INVALID_INDEX)
        *ENTRY_PREV(ENTRY_ADDR(shm_base, *ENTRY_NEXT(entry_addr))) = AGMDB_INVALID_INDEX;
    *spare_list_head = *ENTRY_NEXT(entry_addr);
    *ENTRY_PREV(entry_addr) = AGMDB_INVALID_INDEX;
    *ENTRY_NEXT(entry_addr) = AGMDB_INVALID_INDEX;
    return AGMDB_SUCCESS;
}

/**
** Insert an entry into hash linklist and increase the busy_entry_cnt by 1.
** @param shm_base:        The base address of shared memory.
** @param hash_list_head:  The head of spare linklist.
** @param entry_id:        The offset of entry you want to insert.
** return: AGMDB_SUCCESS if success,
AGMDB_ERROR if failed.
*/
int Entry_insertHashList(CPTR_VOID shm_base, PTR_OFFSET* hash_list_head, PTR_OFFSET entry_id) {
    PTR_VOID entry_addr = ENTRY_ADDR(shm_base, entry_id);
    if (entry_id == AGMDB_INVALID_INDEX)
        return AGMDB_ERROR_INSERT_INVALID_ENTRY_INTO_HASHLIST;
    if (*ENTRY_NEXT(entry_addr) != AGMDB_INVALID_INDEX || *ENTRY_PREV(entry_addr) != AGMDB_INVALID_INDEX)
        return AGMDB_ERROR_INSERT_BUSY_ENTRY_INTO_HASHLIST;

    *ENTRY_NEXT(entry_addr) = *hash_list_head;
    if (*hash_list_head == AGMDB_INVALID_INDEX)
        (*SHM_BUSY_HASH_CNT(shm_base))++;
    else
        *ENTRY_PREV(ENTRY_ADDR(shm_base, *hash_list_head)) = entry_id;
    *hash_list_head = entry_id;
    (*SHM_BUSY_ENTRY_CNT(shm_base))++;
    return AGMDB_SUCCESS;
}

/**
** Remove an entry from hash linklist and decrease the busy_entry_cnt by 1.
** @param shm_base:    The base address of shared memory.
** @param entry_id:    The offset of entry you want to remove.
** return: AGMDB_SUCCESS if success,
AGMDB_ERROR if failed.
*/
int Entry_removeHashList(CPTR_VOID shm_base, PTR_OFFSET entry_id) {
    PTR_OFFSET* hash_list_head;
    PTR_VOID entry_addr = ENTRY_ADDR(shm_base, entry_id);
    PTR_OFFSET prev_id;
    PTR_OFFSET next_id;
    PTR_VOID prev_addr;
    PTR_VOID next_addr;
    if (entry_id == AGMDB_INVALID_INDEX)
        return AGMDB_ERROR_REMOVE_INVALID_ENTRY_FROM_HASHLIST;
    prev_id = *ENTRY_PREV(entry_addr);
    next_id = *ENTRY_NEXT(entry_addr);
    prev_addr = ENTRY_ADDR(shm_base, prev_id);
    next_addr = ENTRY_ADDR(shm_base, next_id);

    // Delete the entry and maintain hash linklist
    if (prev_id == AGMDB_INVALID_INDEX) { // the first entry in hash link list
        hash_list_head = SHM_HASH_LISTS_HEADS(shm_base) + AGMDB_hash(ENTRY_KEY(entry_addr), (int)strlen(ENTRY_KEY(entry_addr)), DEFAULT_HASH_LISTS_NUM);
        if (*hash_list_head != entry_id)
            return AGMDB_ERROR_NONHEAD_ENTRY_IN_HASHLIST_WITHOUT_PREV;
        *hash_list_head = next_id;
        if (*hash_list_head != AGMDB_INVALID_INDEX)
            *ENTRY_PREV(next_addr) = AGMDB_INVALID_INDEX;
        else
            (*SHM_BUSY_HASH_CNT(shm_base))--;
    }
    else {
        *ENTRY_NEXT(prev_addr) = next_id;
        if (next_id != AGMDB_INVALID_INDEX)
            *ENTRY_PREV(next_addr) = prev_id;
    }
    *ENTRY_PREV(entry_addr) = AGMDB_INVALID_INDEX;
    *ENTRY_NEXT(entry_addr) = AGMDB_INVALID_INDEX;
    (*SHM_BUSY_ENTRY_CNT(shm_base))--;
    return AGMDB_SUCCESS;
}

/**
** Insert an entry into the tail of expiration time linklist.
** @param shm_base:        The base address of shared memory.
** @param entry_id:        The offset of entry you want to insert.
** @param time_list_head:  The head of expiration time linklist.
** @param time_list_tail:  The tail of expiration time linklist.
** return: AGMDB_SUCCESS if success,
AGMDB_ERROR if failed.
*/
int Entry_insertTimeList(CPTR_VOID shm_base, PTR_OFFSET entry_id, PTR_OFFSET* time_list_head, PTR_OFFSET* time_list_tail) {
    PTR_VOID entry_addr = ENTRY_ADDR(shm_base, entry_id);
    PTR_OFFSET time_prev_id;
    PTR_OFFSET time_next_id;
    PTR_VOID time_prev_addr;
    PTR_VOID time_next_addr;
    if (entry_id == AGMDB_INVALID_INDEX) {
        return AGMDB_ERROR_INSERT_INVALID_ENTRY_INTO_TIMELIST;
    }
    time_prev_id = *ENTRY_TIME_PREV(entry_addr);
    time_next_id = *ENTRY_TIME_NEXT(entry_addr);
    time_prev_addr = ENTRY_ADDR(shm_base, time_prev_id);
    time_next_addr = ENTRY_ADDR(shm_base, time_next_id);

    if (entry_id == *time_list_tail) { // The entry has been at the tail
        return AGMDB_SUCCESS;
    }
    else {
        // This entry is in the time list and not at the tail. Remove it first.
        if (time_next_id != AGMDB_INVALID_INDEX) {
            if (entry_id == *time_list_head)
                *time_list_head = time_next_id;
            else
                *ENTRY_TIME_NEXT(time_prev_addr) = time_next_id;
            *ENTRY_TIME_PREV(time_next_addr) = time_prev_id;
        }
        // Add it as a new entry
        *ENTRY_TIME_PREV(entry_addr) = *time_list_tail;
        *ENTRY_TIME_NEXT(entry_addr) = AGMDB_INVALID_INDEX;
        if (*time_list_tail != AGMDB_INVALID_INDEX)
            *ENTRY_TIME_NEXT(ENTRY_ADDR(shm_base, *time_list_tail)) = entry_id;
        *time_list_tail = entry_id;
        // If the time linklist is empty before insert this entry
        if (*time_list_head == AGMDB_INVALID_INDEX)
            *time_list_head = entry_id;
    }
    return AGMDB_SUCCESS;
}

/**
** Insert an entry into the tail of expiration time linklist.
** @param shm_base:        The base address of shared memory.
** @param entry_id:        The offset of entry you want to insert.
** @param time_list_head:  The head of expiration time linklist.
** @param time_list_tail:  The tail of expiration time linklist.
** return: AGMDB_SUCCESS if success,
AGMDB_ERROR if failed.
*/
int Entry_removeTimeList(CPTR_VOID shm_base, PTR_OFFSET entry_id, PTR_OFFSET* time_list_head, PTR_OFFSET* time_list_tail) {
    PTR_VOID entry_addr = ENTRY_ADDR(shm_base, entry_id);
    PTR_OFFSET time_prev_id;
    PTR_OFFSET time_next_id;
    PTR_VOID time_prev_addr;
    PTR_VOID time_next_addr;
    if (entry_id == AGMDB_INVALID_INDEX) {
        return AGMDB_ERROR_REMOVE_INVALID_ENTRY_FROM_TIMELIST;
    }
    time_prev_id = *ENTRY_TIME_PREV(entry_addr);
    time_next_id = *ENTRY_TIME_NEXT(entry_addr);
    time_prev_addr = ENTRY_ADDR(shm_base, time_prev_id);
    time_next_addr = ENTRY_ADDR(shm_base, time_next_id);

    if (entry_id == *time_list_head) {
        *time_list_head = time_next_id;
        if (time_next_id != AGMDB_INVALID_INDEX)
            *ENTRY_TIME_PREV(time_next_addr) = AGMDB_INVALID_INDEX;
    }
    else {
        *ENTRY_TIME_NEXT(time_prev_addr) = time_next_id;
    }

    if (entry_id == *time_list_tail) {
        *time_list_tail = time_prev_id;
    }
    else {
        *ENTRY_TIME_PREV(time_next_addr) = time_prev_id;
    }

    *ENTRY_TIME_PREV(entry_addr) = AGMDB_INVALID_INDEX;
    *ENTRY_TIME_NEXT(entry_addr) = AGMDB_INVALID_INDEX;
    return AGMDB_SUCCESS;
}

/**
** Search the entry with given key in a linklist.
** @param shm_base:    The base address of shared memory.
** @param key:         The key you want to search.
** @param hashmap_index: The hash index of the linklist you want to search.
** return: The entry's index if found,
or AGMDB_INVALID_INDEX if not found.
*/
PTR_OFFSET Entry_searchHashlist(CPTR_VOID shm_base, const char* key, unsigned int hashmap_index) {
    PTR_OFFSET entry_id;
    PTR_VOID entry_addr;
    PTR_OFFSET* hashlist_array;

    if (hashmap_index >= DEFAULT_HASH_LISTS_NUM)
        return AGMDB_INVALID_INDEX;

    hashlist_array = SHM_HASH_LISTS_HEADS(shm_base);
    entry_id = *(hashlist_array + hashmap_index);
    while (entry_id != AGMDB_INVALID_INDEX) {
        entry_addr = ENTRY_ADDR(shm_base, entry_id);
        if (strcmp(ENTRY_KEY(entry_addr), key) == 0)
            break;
        entry_id = *ENTRY_NEXT(entry_addr);
    }
    return entry_id;
}

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
int Entry_setKeyValue(CPTR_VOID shm_base, PTR_OFFSET entry_id, const char* key, int key_len, const char* value, int value_len) {
    PTR_VOID entry_addr = ENTRY_ADDR(shm_base, entry_id);
    if (entry_id == AGMDB_INVALID_INDEX)
        return AGMDB_ERROR_SET_KEYVAL_OF_INVALID_ENTRY;
    if ((AGMDB_isstring(key, key_len) != AGMDB_SUCCESS))
        return AGMDB_ERROR_KEY_INVALID_STRING;
    if (key_len + 1 > MAX_KEY_SIZE)
        return AGMDB_ERROR_KEY_TOO_LONG;
    if (value_len + 1 > ENTRY_MAX_DATA_SIZE)
        return AGMDB_ERROR_VALUE_TOO_LONG;

    // Copy the key and value into the entry
    *ENTRY_LENG(entry_addr) = value_len;
    memcpy(ENTRY_KEY(entry_addr), key, key_len);
    *(ENTRY_KEY(entry_addr) + key_len) = '\0';
    memcpy(ENTRY_DATA(entry_addr), value, value_len);
    *(ENTRY_DATA(entry_addr) + value_len) = '\0';
    *ENTRY_MODIFYTIME(entry_addr) = (int)time(NULL);
    return AGMDB_SUCCESS;
}

/**
** Check the string format.
** @param str: the string you want to check.
** @param str_len: given string length (not include '\0').
** return: AGMDB_SUCCESS if the string length equals str_len;
**             AGMDB_ERROR if not
*/
int AGMDB_isstring(const char* str, int str_len) {
    int i;
    for (i = 0;i < str_len;i++)
        if (str[i] == '\0')
            return AGMDB_ERROR;
    if (str[i] != '\0')
        return AGMDB_ERROR;
    return AGMDB_SUCCESS;
}

/**
** Use AGMDB's hash function to hash a string into an integer.
** @param key: the key string.
** @param key_len: the length of the key string.
** @param output_val_range: the range of output value.
** return: hash result if success
**      or AGMDB_ERROR if failed.
*/
unsigned int AGMDB_hash(const char* key, int key_len, unsigned int output_val_range) {
    unsigned char buf[DEFAULT_HASH_VALUE_STR_LENGTH] = { 0 };
    unsigned int hash = 0;

    MurmurHash3_x86_32(key, key_len, DEFAULT_HASH_MAGIC_SEED, buf);
    hash = ((buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3]) % (output_val_range);
    return hash;
}

/**
** Initialize the handle of AG Memory Database.
** @param dbm: the handle of AG Memory Database.
** return:  if AGMDB_SUCCESS if success
**          or AGMDB_ERROR if the handle is NULL.
*/
int Handle_init(struct agmdb_handler* dbm) {
    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
    dbm->shm_base = NULL;
#ifndef _WIN32
    dbm->linux_shm_id = -1;
    dbm->db_lock.sem_id = -1;
#else
    dbm->win_shm_handle = INVALID_HANDLE_VALUE;
    dbm->db_lock.read_lock_handle = INVALID_HANDLE_VALUE;
    dbm->db_lock.write_lock_handle = INVALID_HANDLE_VALUE;
#endif
    return AGMDB_SUCCESS;
}

/**
**========================================================
** AG Memory Database External Function
**========================================================
*/

/**
** Get a shared lock for read only.
** @param dbm: the database you want to lock.
** return: AGMDB_SUCCESS if successfully created or AGMDB_ERROR if failed.
*/
int AGMDB_getSharedLock(struct agmdb_handler *dbm) {
    struct agmdb_lock *db_lock;
    int rc;
    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
    db_lock = (struct agmdb_lock *)&(dbm->db_lock);

    rc = Lock_P(db_lock, SEM_ID_WRITE, 1);
    if (rc != AGMDB_SUCCESS)
        return rc;

    rc = Lock_P(db_lock, SEM_ID_READ, 1);
    if (rc != AGMDB_SUCCESS) {
        Lock_V(db_lock, SEM_ID_WRITE, 1);
        return rc;
    }

    rc = Lock_V(db_lock, SEM_ID_WRITE, 1);
    if (rc != AGMDB_SUCCESS) {
        Lock_V(db_lock, SEM_ID_READ, 1);
        return rc;
    }

    return AGMDB_SUCCESS;
}

/**
** Get a exclusive lock for read and write.
** @param dbm: the database you want to lock.
** return: AGMDB_SUCCESS if successfully created or AGMDB_ERROR if failed.
*/
int AGMDB_getExclusiveLock(struct agmdb_handler *dbm) {
    int rc;
    struct agmdb_lock *db_lock;
    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
    db_lock = (struct agmdb_lock *)&(dbm->db_lock);

    rc = Lock_P(db_lock, SEM_ID_WRITE, 1);
    if (rc != AGMDB_SUCCESS)
        return rc;

    rc = Lock_P(db_lock, SEM_ID_READ, SEM_READ_INITVAL);
    if (rc != AGMDB_SUCCESS) {
        Lock_V(db_lock, SEM_ID_WRITE, 1);
        return rc;
    }

    return AGMDB_SUCCESS;
}

/**
** Free a shared lock that you have got before.
** @param dbm: the database you want to return the lock.
** return: AGMDB_SUCCESS if successfully created or AGMDB_ERROR if failed.
*/
int AGMDB_freeSharedLock(struct agmdb_handler *dbm) {
    int rc;
    struct agmdb_lock *db_lock;
    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
    db_lock = (struct agmdb_lock *)&(dbm->db_lock);

    rc = Lock_V(db_lock, SEM_ID_READ, 1);
    if (rc != AGMDB_SUCCESS)
        return rc;

    return AGMDB_SUCCESS;
}

/**
** Free a exclusive lock that you have got before.
** @param dbm: the database you want to return the lock.
** return: AGMDB_SUCCESS if successfully created or AGMDB_ERROR if failed.
*/
int AGMDB_freeExclusiveLock(struct agmdb_handler *dbm) {
    int rc;
    struct agmdb_lock *db_lock;
    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
    db_lock = (struct agmdb_lock *)&(dbm->db_lock);

    rc = Lock_V(db_lock, SEM_ID_READ, SEM_READ_INITVAL);
    if (rc != AGMDB_SUCCESS)
        return AGMDB_ERROR;
    rc = Lock_V(db_lock, SEM_ID_WRITE, 1);
    if (rc != AGMDB_SUCCESS)
        return AGMDB_ERROR;

    return AGMDB_SUCCESS;
}

/**
** Search the entry with given key in a hash linklists.
** @param shm_base:    The base address of shared memory.
** @param key:         The key you want to search.
** @param list_head:   The head of linklist you want to search.
** return: The entry's index if successfully got,
or AGMDB_INVALID_INDEX if not found,
*/
inline PTR_OFFSET AGMDB_getEntry(CPTR_VOID shm_base, const char * key, int key_len) {
    return Entry_searchHashlist(shm_base, key, AGMDB_hash(key, key_len, DEFAULT_HASH_LISTS_NUM));
}

/**
** Set the key and value into an existed entry, then adjust the expiration time linklist.
** @param shm_base:The base address of shared memory.
** @param entry_id:The offset of the entry you want to set.
** @param key:     The key string.
** @param key_len: The length of key string.
** @param value:   The value string.
** @param value:   Tthe length of value string.
** return: AGMDB_SUCCESS if success
or AGMDB_ERROR if fail
*/
inline int AGMDB_setEntry(CPTR_VOID shm_base, PTR_OFFSET entry_id,
    const char* key, int key_len, const char* value, int value_len) {
    int rc;

    rc = Entry_setKeyValue(shm_base, entry_id, key, key_len, value, value_len);
    if (rc != AGMDB_SUCCESS)
        return rc;
    rc = Entry_insertTimeList(shm_base, entry_id, SHM_EXPIRE_TIME_LIST_HEAD(shm_base), SHM_EXPIRE_TIME_LIST_TAIL(shm_base));
    if (rc != AGMDB_SUCCESS)
        return rc;
    return AGMDB_SUCCESS;
}

/**
** Set the key and value into a spare entry, then adjust the hash linklist and expiration time linklist.
** @param shm_base:    The base address of shared memory.
** @param key:         The key string.
** @param key_len:     The length of key string.
** @param value:       The value string.
** @param value:       Tthe length of value string.
** return: AGMDB_SUCCESS if success
or AGMDB_ERROR if fail
*/
inline int AGMDB_setSpareEntry(CPTR_VOID shm_base, const char* key, int key_len, const char* value, int value_len) {
    PTR_OFFSET entry_id;
    int rc;
    PTR_OFFSET* hash_list_head;
    if ((AGMDB_isstring(key, key_len) != AGMDB_SUCCESS))
        return AGMDB_ERROR_KEY_INVALID_STRING;
    if (key_len + 1 > MAX_KEY_SIZE)
        return AGMDB_ERROR_KEY_TOO_LONG;
    if (value_len + 1 > ENTRY_MAX_DATA_SIZE)
        return AGMDB_ERROR_VALUE_TOO_LONG;
    hash_list_head = SHM_HASH_LISTS_HEADS(shm_base) + AGMDB_hash(key, key_len, DEFAULT_HASH_LISTS_NUM);

    rc = Entry_removeSpareList(shm_base, SHM_SPARE_LIST_HEAD(shm_base), &entry_id);
    if (rc != AGMDB_SUCCESS)
        return rc;
    rc = Entry_insertHashList(shm_base, hash_list_head, entry_id);
    if (rc != AGMDB_SUCCESS) {
        Entry_insertSpareList(shm_base, SHM_SPARE_LIST_HEAD(shm_base), entry_id);
        return rc;
    }

    rc = AGMDB_setEntry(shm_base, entry_id, key, key_len, value, value_len);
    if (rc != AGMDB_SUCCESS) {
        Entry_removeHashList(shm_base, entry_id);
        Entry_insertSpareList(shm_base, SHM_SPARE_LIST_HEAD(shm_base), entry_id);
        return rc;
    }
    return AGMDB_SUCCESS;
}

/**
** Delete an entry with given index.
** @param shm_base:The base address of shared memory.
** @param entry_id:The offset of the entry you want to delete.
** return: AGMDB_SUCCESS if success,
or AGMDB_ERROR if fail to delete.
*/
inline int AGMDB_deleteEntry(CPTR_VOID shm_base, PTR_OFFSET entry_id) {
    int rc;
    if (shm_base == NULL)
        return AGMDB_ERROR_SHM_BASE_NULL;
    if (entry_id == AGMDB_INVALID_INDEX)
        return AGMDB_ERROR_DELETE_INVALID_ENTRY;

    // Maintain the expiration time linklist
    rc = Entry_removeTimeList(shm_base, entry_id, SHM_EXPIRE_TIME_LIST_HEAD(shm_base), SHM_EXPIRE_TIME_LIST_TAIL(shm_base));
    if (rc != AGMDB_SUCCESS)
        return rc;

    // Delete the entry and maintain hash linklist   
    rc = Entry_removeHashList(shm_base, entry_id);
    if (rc != AGMDB_SUCCESS)
        return rc;
    // Maintain the spare linklist
    return Entry_insertSpareList(shm_base, SHM_SPARE_LIST_HEAD(shm_base), entry_id);
}

/**
** Open a database with given name, and intialize the agmdb handler for further operation.
** If the database doesn't exist, a new database will be created.
** @param dbm: a created sturcture to save the database information.
** @param db_name: the unique identifier of a database.
** @param db_name_length: the length of the unique_name.
** @param entry_num: The maximum number of entries in the database.
** return: AGMDB_SUCCESS if successfully created or AGMDB_ERROR if failed.
*/
int AGMDB_openDB(struct agmdb_handler* dbm, const char* db_name, int db_name_length, unsigned int entry_num) {
    int rc = AGMDB_SUCCESS;
    int rc_shm = AGMDB_SUCCESS;

    rc = Handle_init(dbm);
    if (rc != AGMDB_SUCCESS) {
        return rc;
    }

    /* Check the format of db_name */
    if (db_name == NULL) {
        return AGMDB_ERROR_NAME_NULL;
    }
    else if (db_name_length <= 0) {
        return AGMDB_ERROR_NAME_INVALID_STRING;
    }
    else if (AGMDB_isstring(db_name, db_name_length) == AGMDB_ERROR) {
        return AGMDB_ERROR_NAME_INVALID_STRING;
    }

    /* Create or open the shared memory */
    rc = SHM_create(dbm, db_name, db_name_length, entry_num);
    if (AGMDB_isError(rc)) {
        return rc;
    }
    else if (rc != AGMDB_SUCCESS_SHM_CREATE && rc != AGMDB_SUCCESS_SHM_OPEN) {
        return AGMDB_ERROR_UNEXPECTED;
    }
    rc_shm = rc;

    /* Create or open the lock */
    rc = Lock_create(&(dbm->db_lock), db_name, db_name_length);
    if (AGMDB_isError(rc))
    {
        if (rc_shm == AGMDB_SUCCESS_SHM_CREATE) {
            SHM_destroy(dbm);
        }
        else if (rc_shm == AGMDB_SUCCESS_SHM_OPEN) {
            SHM_close(dbm);
        }
        return rc;
    }
    else if (rc != AGMDB_SUCCESS_LOCK_CREATE && rc != AGMDB_SUCCESS_LOCK_OPEN) {
        if (rc_shm == AGMDB_SUCCESS_SHM_CREATE) {
            SHM_destroy(dbm);
        }
        else if (rc_shm == AGMDB_SUCCESS_SHM_OPEN) {
            SHM_close(dbm);
        }
        return AGMDB_ERROR_UNEXPECTED;
    }

    if (rc_shm == AGMDB_SUCCESS_SHM_CREATE) {
        return AGMDB_SUCCESS_DB_CREATE;
    }
    else {
        return AGMDB_SUCCESS_DB_OPEN;
    }
}

/**
** Close and destroy a database.
** @param dbm: the database you want to destroy.
** return: AGMDB_SUCCESS if successfully destroyed or AGMDB_ERROR if failed.
*/
int AGMDB_destroyDB(struct agmdb_handler *dbm) {
    int rc = AGMDB_SUCCESS;
    int rc_shm = AGMDB_SUCCESS;
    int rc_lock = AGMDB_SUCCESS;

    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;

    rc = AGMDB_closeDB(dbm);
    if (AGMDB_isError(rc))
        return rc;

    rc_shm = SHM_destroy(dbm);
    rc_lock = Lock_destroy(&(dbm->db_lock));

    /* Database destroy doesn't support on Windows */
    if(rc_shm == AGMDB_ERROR_SHM_WIN_DESTROY_NOT_SUPPORT && rc_lock == AGMDB_ERROR_LOCK_WIN_DESTROY_NOT_SUPPORT)
        return AGMDB_ERROR_DB_WIN_DESTROY_NOT_SUPPORT;

    if (AGMDB_isError(rc_shm))
        return rc_shm;
    else if (AGMDB_isError(rc_lock))
        return rc_lock;
    else
        return AGMDB_SUCCESS;
}

/**
** Close a database, but does not destroy it.
** @param dbm: the database you want to close.
** return: AGMDB_SUCCESS if successfully closed or AGMDB_ERROR if failed.
*/
int AGMDB_closeDB(struct agmdb_handler *dbm) {
    int rc_shm = AGMDB_SUCCESS;
    int rc_lock = AGMDB_SUCCESS;

    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
        
    rc_shm = SHM_close(dbm);
    rc_lock = Lock_close(&(dbm->db_lock));
    if (AGMDB_isError(rc_shm))
        return rc_shm;
    else if (AGMDB_isError(rc_lock))
        return rc_lock;
    else
        return AGMDB_SUCCESS;
}

/**
** Remove all entry_array in a database.
** You have to get EXCLUSIVE LOCK of the database before calling this function.
** @param dbm: the database you want to modify.
** return: AGMDB_SUCCESS if success
**      or AGMDB_ERROR if failed.
*/
int AGMDB_cleanDB(struct agmdb_handler *dbm) {
    PTR_VOID shm_base;
    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
    shm_base = (PTR_VOID)(dbm->shm_base);

    return SHM_init(shm_base, *SHM_SIZE(shm_base), *SHM_ENTRIES_NUM(shm_base));
}

/**
** Reset the expiration time of the database.
** You have to get EXCLUSIVE LOCK of the database before calling this function.
** Default expiration time is 3600 seconds.
** @param expire_time: how many seconds all entry_array in database will be kept.
*/
int AGMDB_setExpireTime(struct agmdb_handler *dbm, unsigned int expire_time) {
    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;

    if (expire_time < 0)
        return AGMDB_ERROR_SET_NEGATIVE_EXIPRE_TIME;
    *SHM_EXPIRE_TIME((const PTR_VOID)(dbm->shm_base)) = expire_time;
    return AGMDB_SUCCESS;
}

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

int AGMDB_get(struct agmdb_handler *dbm, const char* key, int key_len, char* buffer, int buffer_len, int* val_len) {
    CPTR_VOID shm_base;
    int data_len;
    PTR_OFFSET entry_id;
    PTR_VOID entry_addr;
    *val_len = 0;

    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
    if (key == NULL)
        return AGMDB_ERROR_KEY_INVALID_STRING;
    if (buffer == NULL)
        return AGMDB_ERROR_GET_BUFFER_NULL;
    if (key_len <= 0)
        return AGMDB_ERROR_KEY_INVALID_STRING;
    if (buffer_len <= 0)
        return AGMDB_ERROR_GET_INVALID_BUFFER_LEN;
    shm_base = (CPTR_VOID)(dbm->shm_base);

    // Check the format of key
    if (AGMDB_isstring(key, key_len) == AGMDB_ERROR) {
        return AGMDB_ERROR_KEY_INVALID_STRING;
    }

    // Hash the key, and search the key in the corresponding hash linlist
    entry_id = AGMDB_getEntry(shm_base, key, key_len);
    if (entry_id == AGMDB_INVALID_INDEX) {
        buffer[0] = '\0';
        return 0;
    }
    entry_addr = ENTRY_ADDR(shm_base, entry_id);

    // Check the capacity of buffer and copy the data
    data_len = *ENTRY_LENG(entry_addr);
    if (data_len + 1 > buffer_len)
        return AGMDB_ERROR_GET_BUFFER_TOO_SMALL;
    memcpy(buffer, ENTRY_DATA(entry_addr), data_len);
    buffer[data_len] = '\0';
    *val_len = data_len;
    return AGMDB_SUCCESS;
}

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
int AGMDB_set(struct agmdb_handler *dbm, const char* key, int key_len, const char* value, int value_len) {
    CPTR_VOID shm_base;
    PTR_OFFSET entry_id;

    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
    if (key == NULL)
        return AGMDB_ERROR_KEY_INVALID_STRING;
    if (value == NULL)
        return AGMDB_ERROR_VALUE_INVALID_STRING;
    if (key_len <= 0)
        return AGMDB_ERROR_KEY_INVALID_STRING;
    if (value_len <= 0)
        return AGMDB_ERROR_VALUE_INVALID_STRING;
    shm_base = (CPTR_VOID)(dbm->shm_base);
    if (shm_base == NULL)
        return AGMDB_ERROR_SHM_BASE_NULL;

    // Check the format of key & value in setEntry and setSpareEntry
    // Hash the key, and search the key in the corresponding hash linklist
    entry_id = AGMDB_getEntry(shm_base, key, key_len);
    if (entry_id == AGMDB_INVALID_INDEX) // Cannot find this key, need a new entry
        return AGMDB_setSpareEntry(shm_base, key, key_len, value, value_len);
    else
        return AGMDB_setEntry(shm_base, entry_id, key, key_len, value, value_len);
}

/**
** Delete an entry with a given key in a database.
** You have to get EXCLUSIVE LOCK of the database before calling this function.
** @param dbm: the database you want to modify.
** @param key: the key string.
** @param key_len: the length of key string.
** return: AGMDB_SUCCESS if the key is deleted or not in the database
**      or AGMDB_ERROR if failed.
*/
int AGMDB_delete(struct agmdb_handler *dbm, const char* key, int key_len) {
    CPTR_VOID shm_base;
    unsigned int hashlist_index;
    PTR_OFFSET entry_id;

    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
    if (key == NULL)
        return AGMDB_ERROR_KEY_INVALID_STRING;
    if (key_len <= 0)
        return AGMDB_ERROR_KEY_INVALID_STRING;
    shm_base = (CPTR_VOID)(dbm->shm_base);
    if (shm_base == NULL)
        return AGMDB_ERROR_SHM_BASE_NULL;

    // Check the format of key
    if (AGMDB_isstring(key, key_len) == AGMDB_ERROR) {
        //msr_log_error(msr, "[ERROR]In AGMDB_delete: Invalid key.");
        return AGMDB_ERROR_KEY_INVALID_STRING;
    }

    // Hash the key, and search the key in the corresponding hash linklist
    hashlist_index = AGMDB_hash(key, key_len, *SHM_HASH_LISTS_NUM(shm_base));
    entry_id = Entry_searchHashlist(shm_base, key, hashlist_index);
    if (entry_id == AGMDB_INVALID_INDEX) // If not exist
        return AGMDB_SUCCESS;

    // Delete the entry and maintain hash linklist
    AGMDB_deleteEntry(shm_base, entry_id);

    return AGMDB_SUCCESS;
}

/**
** Remove all staled entry_array.
** You have to get EXCLUSIVE LOCK of the database before calling this function.
** @param dbm: the database you want to remove the stale entry_array.
** return: AGMDB_SUCCESS if success
**      or AGMDB_ERROR if failed.
*/
int AGMDB_removeStale(struct agmdb_handler *dbm) {
    CPTR_VOID shm_base;
    PTR_VOID entry_addr;
    PTR_OFFSET  entry_id;
    PTR_OFFSET  entry_id_next;
    PTR_OFFSET* time_list_head;
    unsigned int expire_modify_time;

    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
    shm_base = (CPTR_VOID)(dbm->shm_base);
    time_list_head = SHM_EXPIRE_TIME_LIST_HEAD(shm_base);

    // Entriy is staled if its modify time is before this time.
    expire_modify_time = (int)time(NULL) - *SHM_EXPIRE_TIME(shm_base);

    entry_id = *time_list_head;
    while (entry_id != AGMDB_INVALID_INDEX) {
        entry_addr = ENTRY_ADDR(shm_base, entry_id);
        if (*ENTRY_MODIFYTIME(entry_addr) > expire_modify_time)
            break;
        entry_id_next = *ENTRY_TIME_NEXT(entry_addr);
        AGMDB_deleteEntry(shm_base, entry_id);
        entry_id = entry_id_next;
    }

    return AGMDB_SUCCESS;
}

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
const char* AGMDB_getErrorInfo(int error_no) {
    switch (error_no) {
    case AGMDB_SUCCESS:
        return "Success!";
    case AGMDB_SUCCESS_DB_CREATE:
        return "Create the memory database successfully!";
    case AGMDB_SUCCESS_DB_OPEN:
        return "Open the memory database successfully!";
    case AGMDB_SUCCESS_SHM_CREATE:
        return "Create the shared memory successfully!";
    case AGMDB_SUCCESS_SHM_OPEN:
        return "Open the shared memory successfully!";

    case AGMDB_SUCCESS_LOCK_CREATE:
        return "Create the lock successfully!";
    case AGMDB_SUCCESS_LOCK_OPEN:
        return "Open the lock successfully!";

    case AGMDB_ERROR_INSERT_INVALID_ENTRY_INTO_SPARELIST:
        return "The AGMDB is inserting an invalid entry into the spare list.";
    case AGMDB_ERROR_INSERT_BUSY_ENTRY_INTO_SPARELIST:
        return "The AGMDB is inserting a busy entry into the spare list. Should remove the entry from hashlist first.";

    case AGMDB_ERROR_INSERT_INVALID_ENTRY_INTO_HASHLIST:
        return "The AGMDB is inserting an invalid entry into the hashlist.";
    case AGMDB_ERROR_INSERT_BUSY_ENTRY_INTO_HASHLIST:
        return "The AGMDB is inserting a busy entry into the hashlist.";

    case AGMDB_ERROR_REMOVE_INVALID_ENTRY_FROM_HASHLIST:
        return "The AGMDB is removing an invalid entry from hashlist";
    case AGMDB_ERROR_NONHEAD_ENTRY_IN_HASHLIST_WITHOUT_PREV:
        return "An entry in hashlist without previous entry is not the head of the hashlist.";

    case AGMDB_ERROR_INSERT_INVALID_ENTRY_INTO_TIMELIST:
        return "The AGMDB is inserting an invalid entry into the expire time list.";
    case AGMDB_ERROR_REMOVE_INVALID_ENTRY_FROM_TIMELIST:
        return "The AGMDB is removing an invalid entry into the expire time list.";

    case AGMDB_ERROR_SET_KEYVAL_OF_INVALID_ENTRY:
        return "The AGMDB is setting key-value to an invalid entry.";

    case AGMDB_ERROR_KEY_INVALID_STRING:
        return "The key is not a valid string.";
    case AGMDB_ERROR_KEY_TOO_LONG:
        return "The key string is too long.";
    case AGMDB_ERROR_VALUE_INVALID_STRING:
        return "The value is not a valid string.";
    case AGMDB_ERROR_VALUE_TOO_LONG:
        return "The value string is too long.";

    case AGMDB_ERROR_HANDLE_NULL:
        return "The agmdb_handler is NULL.";
    case AGMDB_ERROR_DELETE_INVALID_ENTRY:
        return "The AGMDB is deleting an invalid entry.";
    case AGMDB_ERROR_NAME_NULL:
        return "The name of AGMDB is NULL";
    case AGMDB_ERROR_NAME_INVALID_STRING:
        return "The name of AGMDB is not a valid string.";

    case AGMDB_ERROR_SET_NEGATIVE_EXIPRE_TIME:
        return "Set the expire time to a negative value.";

    case AGMDB_ERROR_GET_BUFFER_NULL:
        return "In get function, the buffer is NULL.";
    case AGMDB_ERROR_GET_INVALID_BUFFER_LEN:
        return "In get function, the length of buffer is an invalid number.";
    case AGMDB_ERROR_GET_BUFFER_TOO_SMALL:
        return "In get function, the buffer is too small to save the data.";

    case AGMDB_ERROR_TIMELIST_LONG_NOTEQUEAL_SHM_CNT:
        return "The size of expire time list does not equal to the entry counter in DB control block.";
    case AGMDB_ERROR_GETALL_ARRAY_TOO_SMALL:
        return "In getAll function, the array is too samll to save the data.";

    case AGMDB_ERROR_DB_WIN_DESTROY_NOT_SUPPORT:
        return "In Windows system, the destroy operation doesn't supported. Just close the database.";

    case AGMDB_ERROR_UNEXPECTED:
        return "Unexpected error happens.";

    case AGMDB_ERROR_SHM_BASE_NULL:
        return "The shared memory base address is NULL. Please check the agmdb_handler object.";
    case AGMDB_ERROR_SHM_SIZE_TOO_SMALL:
        return "The size setting of shared memory is too small to save the DB control information.";
    case AGMDB_ERROR_SHM_NAME_INVALID_STRING:
        return "The name of database is not a valid string. Please check the data_dir setting in configuration file.";
    case AGMDB_ERROR_SHM_ENTRY_NUM_NEGATIVE:
        return "The setting of DB's entry number is negative!";

    case AGMDB_ERROR_SHM_LINUX_CREATE_FAIL:
        return "In Linux system, failed when creating the shared memory.";
    case AGMDB_ERROR_SHM_LINUX_OPEN_ACCESS_FAIL:
        return "In Linux system, no access permission when opening the shared memory.";
    case AGMDB_ERROR_SHM_LINUX_OPEN_FAIL:
        return "In Linux system, failed when opening the shared memory.";
    case AGMDB_ERROR_SHM_LINUX_MAP_FAIL:
        return "In Linux system, failed when mapping the shared memory into the process's address space.";
    case AGMDB_ERROR_SHM_LINUX_DETACH_FAIL:
        return "In Linux system, faild when detaching the shared memory.";
    case AGMDB_ERROR_SHM_LINUX_DESTROY_FAIL:
        return "In Linux system, faild when deleting the shared memory.";

    case AGMDB_ERROR_SHM_WIN_CREATE_FAIL:
        return "In Windows system, failed when creating the shared memory.";
    case AGMDB_ERROR_SHM_WIN_MAP_FAIL:
        return "In Windows system, failed when mapping the shared memory into the process's address space.";
    case AGMDB_ERROR_SHM_WIN_UNMAP_FAIL:
        return "In Windows system, faild when unmap the shared memory. Call GetLastError() for more information.";
    case AGMDB_ERROR_SHM_WIN_CLOSE_HANDLE_FAIL:
        return "In Windows system, faild when close the shared memory handle. Call GetLastError() for more information.";
    case AGMDB_ERROR_SHM_WIN_UNMAP_AND_CLOSE_HANDLE_FAIL:
        return "In Windows system, faild when unmap the shared memory and close the shared memory handle. Call GetLastError() for more information.";
    case AGMDB_ERROR_SHM_WIN_DESTROY_NOT_SUPPORT:
        return "In Windows system, the shared memory destroy operation doesn't supported. Just close the shared memory.";

    case AGMDB_ERROR_LOCK_OP_NEGATIVE_VAL:
        return "When operating the lock, the operation value is negative!";

    case AGMDB_ERROR_LOCK_LINUX_SEM_CREATE_FAIL:
        return "In Linux system, failed when creating the semaphore. Please check the semaphore limit of the system.";
    case AGMDB_ERROR_LOCK_LINUX_SEM_OPEN_FAIL:
        return "In Linux system, failed when opening an existed semaphore. Please check the semaphore limit and permission settings.";
    case AGMDB_ERROR_LOCK_LINUX_SEM_INIT_FAIL:
        return "In Linux system, failed when initializing the semaphore value. Please check the existence and permission of the semaphore.";
    case AGMDB_ERROR_LOCK_LINUX_SEM_MODIFY_FAIL:
        return "In Linux system, failed when modifying the semaphore value. Please check the existence and permission of the semaphore.";
    case AGMDB_ERROR_LOCK_LINUX_SEM_DESTROY_FAIL:
        return "In Linux system, failed when deleting the semaphore.";

    case AGMDB_ERROR_LOCK_WIN_NAME_INVALID_STRING:
        return "In Windows system, the name of lock is not a valid string. Please check the data_dir setting in configuration file.";
    case AGMDB_ERROR_LOCK_WIN_MUTEX_CREATE_FAIL:
        return "In Windows system, failed when creating the mutex object.";
    case AGMDB_ERROR_LOCK_WIN_ONLY_ONE_LOCK_EXISTS:
        return "In Windows system, one and only one lock has existed.";
    case AGMDB_ERROR_LOCK_WIN_GET_MUTEX_FAIL:
        return "In Windows system, failed when getting the mutex object. Possible reason is deadlock or permission issue.";
    case AGMDB_ERROR_LOCK_WIN_RELEASE_MUTEX_FAIL:
        return "In Windows system, failed when releasing the mutex object.";
    case AGMDB_ERROR_LOCK_WIN_CLOSE_MUTEX_FAIL:
        return "In Windows system, failed when close the mutex object.";
    case AGMDB_ERROR_LOCK_WIN_DESTROY_NOT_SUPPORT:
        return "In Windows system, the lock destroy operation doesn't supported. Just close the locks.";

    default:
        return "Error code is not found.";
    }
}

/**
** Check whether a return_code is an error.
** @param return_code: the code returned by a AGMDB function.
** return: True if there is an error;
False if not.
*/
bool AGMDB_isError(int return_code) {
    if (return_code < AGMDB_ERROR)
        return false;
    else
        return true;
}

/**
** Get the number of keys in a database.
** You have to get SHARED or EXCLUSIVE LOCK of the database before calling this function.
** @param dbm: the database you want to read.
** @param keynum: the value to store the number of keys in the database
** return: AGMDB_SUCCESS if no error
**      or AGMDB_ERROR if failed.
*/
int AGMDB_getKeyNum(struct agmdb_handler *dbm, int* keynum) {
    CPTR_VOID shm_base;
    unsigned int timelist_cnt = 0;
    PTR_VOID entry_addr;
    PTR_OFFSET entry_id;

    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;

    shm_base = (CPTR_VOID)(dbm->shm_base);
    entry_id = *SHM_EXPIRE_TIME_LIST_HEAD(shm_base);
    while (entry_id != AGMDB_INVALID_INDEX) {
        entry_addr = ENTRY_ADDR(shm_base, entry_id);
        timelist_cnt++;
        entry_id = *ENTRY_TIME_NEXT(entry_addr);
    }
    *keynum = timelist_cnt;
    // Check the length of expirartion time linklist and the busy entry counter;
    if (timelist_cnt == *SHM_BUSY_ENTRY_CNT(shm_base))
        return AGMDB_SUCCESS;
    else
        return AGMDB_ERROR_TIMELIST_LONG_NOTEQUEAL_SHM_CNT;
}

/**
** Use AGMDB's hash function to hash a string into an integer.
** @param key: the key string.
** @param key_len: the length of the key string.
** @param output_val_range: the range of output value.
** return: hash result if success
**      or AGMDB_ERROR if failed.
*/
int AGMDB_getHashValue(const char* key, int key_len, int output_val_range) {
    if (key == NULL)
        return AGMDB_ERROR_KEY_INVALID_STRING;
    if (AGMDB_isstring(key, key_len) != AGMDB_SUCCESS)
        return AGMDB_ERROR_KEY_INVALID_STRING;
    return AGMDB_hash(key, key_len, output_val_range);
}

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
int AGMDB_getAllKeysValues(struct agmdb_handler *dbm, int array_size, const char* keys[], const char * values[], unsigned int vals_len[]) {
    CPTR_VOID shm_base;
    PTR_VOID entry_addr;
    PTR_OFFSET  entry_id;
    PTR_OFFSET* time_list_head;
    int keys_counter = 0;

    if (dbm == NULL)
        return AGMDB_ERROR_HANDLE_NULL;
    shm_base = (CPTR_VOID)(dbm->shm_base);
    time_list_head = SHM_EXPIRE_TIME_LIST_HEAD(shm_base);

    entry_id = *time_list_head;
    while (entry_id != AGMDB_INVALID_INDEX) {
        if (keys_counter >= array_size)
            return AGMDB_ERROR_GETALL_ARRAY_TOO_SMALL;
        entry_addr = ENTRY_ADDR(shm_base, entry_id);
        keys[keys_counter] = ENTRY_KEY(entry_addr);
        values[keys_counter] = ENTRY_DATA(entry_addr);
        vals_len[keys_counter] = *ENTRY_LENG(entry_addr);
        entry_id = *ENTRY_TIME_NEXT(entry_addr);
        keys_counter++;
    }

    return AGMDB_SUCCESS;
}