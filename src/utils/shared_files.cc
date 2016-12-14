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

#include "src/utils/shared_files.h"

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#include <fstream>
#include <string>

namespace modsecurity {
namespace utils {


msc_file_handler_t *SharedFiles::find_handler(
    const std::string &fileName) {
    msc_file_handler_t *current = m_first;
    while (current != NULL) {
        if (current->file_name == fileName) {
            return current;
        }
        current = reinterpret_cast<msc_file_handler_t *>(current->next);
    }

    return NULL;
}


msc_file_handler_t *SharedFiles::add_new_handler(
    const std::string &fileName, std::string *error) {
    msc_file_handler_t *current = m_first;
    int shm_id;
    key_t mem_key_structure;
    key_t mem_key_file_name;
    msc_file_handler_t *new_debug_log;
    char *shm_ptr2;
    FILE *fp;

    fp = fopen(fileName.c_str(), "a");
    if (fp == 0) {
        error->assign("Failed to open file: " + fileName);
        goto err_fh;
    }

    mem_key_structure = ftok(fileName.c_str(), 1);
    if (mem_key_structure < 0) {
        error->assign("Failed to select key for the shared memory (1): ");
        error->append(strerror(errno));
        goto err_mem_key;
    }

    mem_key_file_name = ftok(fileName.c_str(), 2);
    if (mem_key_file_name < 0) {
        error->assign("Failed to select key for the shared memory (2): ");
        error->append(strerror(errno));
        goto err_mem_key;
    }

    shm_id = shmget(mem_key_structure, sizeof (msc_file_handler_t),
        IPC_CREAT | 0666);
    if (shm_id < 0) {
        error->assign("Failed to allocate shared memory (1): ");
        error->append(strerror(errno));
        goto err_shmget1;
    }

    new_debug_log = reinterpret_cast<msc_file_handler_t *>(
        shmat(shm_id, NULL, 0));
    if ((reinterpret_cast<char *>(new_debug_log)[0]) == -1) {
        error->assign("Failed to attach shared memory (1): ");
        error->append(strerror(errno));
        goto err_shmat1;
    }
    memset(new_debug_log, '\0', sizeof(msc_file_handler_t));

    pthread_mutex_init(&new_debug_log->lock, NULL);
    new_debug_log->fp = fp;
    new_debug_log->file_handler = fileno(new_debug_log->fp);
    new_debug_log->next = NULL;
    new_debug_log->previous = NULL;
    new_debug_log->shm_id_structure = shm_id;
    shm_id = shmget(mem_key_file_name, (fileName.size() + 1 * sizeof(char)),
        IPC_CREAT | 0666);
    if (shm_id < 0) {
        error->assign("Failed to allocate shared memory (2): ");
        error->append(strerror(errno));
        goto err_shmget2;
    }
    new_debug_log->shm_id_file_name = shm_id;
    shm_ptr2 = reinterpret_cast<char *>(shmat(shm_id, NULL, 0));
    if (shm_ptr2[0] == -1) {
        error->assign("Failed to attach shared memory (2): ");
        error->append(strerror(errno));
        goto err_shmat2;
    }
    memcpy(shm_ptr2, fileName.c_str(), fileName.size());
    shm_ptr2[fileName.size()] = '\0';

    new_debug_log->file_name = shm_ptr2;

    if (m_first == NULL) {
        m_first = new_debug_log;
    } else {
        current = m_first;
        while (current != NULL) {
            if (current->next == NULL) {
                current->next = new_debug_log;
                new_debug_log->previous = current;
                new_debug_log->next = NULL;
                current = NULL;
            } else {
                current = reinterpret_cast<msc_file_handler_t *>(
                    current->next);
            }
        }
    }

    return new_debug_log;
err_shmget2:
err_shmat2:
    shmdt(shm_ptr2);
    fclose(new_debug_log->fp);
err_shmget1:
err_shmat1:
    shmdt(new_debug_log);
err_mem_key:
err_fh:
    return NULL;
}


bool SharedFiles::open(const std::string& fileName, std::string *error) {
    msc_file_handler_t *a = find_handler(fileName);
    if (a == NULL) {
        a = add_new_handler(fileName, error);
        if (error->size() > 0) {
            return false;
        }
    }
    if (a == NULL) {
        error->assign("Not able to open: " + fileName);
        return false;
    }

    a->using_it++;

    return true;
}


void SharedFiles::close(const std::string& fileName) {
    msc_file_handler_t *a;

    if (fileName.empty()) {
        return;
    }

    a = find_handler(fileName);
    if (a == NULL) {
        return;
    }

    a->using_it--;

    if (a->using_it == 0) {
        bool first = false;
        int shm_id1 = a->shm_id_structure;
        int shm_id2 = a->shm_id_file_name;
        msc_file_handler_t *p , *n;
        pthread_mutex_lock(&a->lock);
        fclose(a->fp);

        p = reinterpret_cast<msc_file_handler_t *>(a->previous);
        n = reinterpret_cast<msc_file_handler_t *>(a->next);
        if (p != NULL) {
            p->next = reinterpret_cast<msc_file_handler_t *>(n);
        }
        if (n != NULL) {
            n->previous = reinterpret_cast<msc_file_handler_t *>(p);
        }
        a->previous = NULL;
        a->next = NULL;
        pthread_mutex_unlock(&a->lock);
        pthread_mutex_destroy(&a->lock);

        if (a->file_name == m_first->file_name) {
            first = true;
        }

        shmdt(a->file_name);
        shmdt(a);

        shmctl(shm_id1, IPC_RMID, NULL);
        shmctl(shm_id2, IPC_RMID, NULL);

        if (first) {
            m_first = NULL;
        }
        a = NULL;
    }
}


bool SharedFiles::write(const std::string& fileName,
    const std::string &msg, std::string *error) {
    std::string lmsg = msg;
    size_t wrote;
    bool ret = true;

    msc_file_handler_t *a = find_handler(fileName);
    if (a == NULL) {
        error->assign("file is not open: " + fileName);
        return false;
    }

    pthread_mutex_lock(&a->lock);
    wrote = fwrite(reinterpret_cast<const char *>(lmsg.c_str()), 1,
        lmsg.size(), a->fp);
    if (wrote < msg.size()) {
        error->assign("failed to write: " + fileName);
        ret = false;
    }
    pthread_mutex_unlock(&a->lock);

    return ret;
}


}  // namespace utils
}  // namespace modsecurity
