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
    for (const auto &i: m_handlers) {
        if (i.first == fileName) {
            return i.second;
        }
    }
    return NULL;
}


msc_file_handler_t *SharedFiles::add_new_handler(
    const std::string &fileName, std::string *error) {
    int shm_id;
    key_t mem_key_structure;
    msc_file_handler_t *new_debug_log;
    FILE *fp;
    bool toBeCreated = true;

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

    shm_id = shmget(mem_key_structure, sizeof (msc_file_handler_t) + fileName.size() + 1,
        IPC_CREAT | IPC_EXCL | 0666);
    if (shm_id < 0) {
        shm_id = shmget(mem_key_structure, sizeof (msc_file_handler_t)
            + fileName.size() + 1, IPC_CREAT | 0666);
        toBeCreated = false;
        if (shm_id < 0) {
            error->assign("Failed to allocate shared memory (1): ");
            error->append(strerror(errno));
            goto err_shmget1;
        }
    }

    new_debug_log = reinterpret_cast<msc_file_handler_t *>(
            shmat(shm_id, NULL, 0));
    if ((reinterpret_cast<char *>(new_debug_log)[0]) == -1) {
        error->assign("Failed to attach shared memory (1): ");
        error->append(strerror(errno));
        goto err_shmat1;
    }

    if (toBeCreated) {
        memset(new_debug_log, '\0', sizeof(msc_file_handler_t));
        pthread_mutex_init(&new_debug_log->lock, NULL);
        new_debug_log->fp = fp;
        new_debug_log->file_handler = fileno(new_debug_log->fp);
        new_debug_log->shm_id_structure = shm_id;
        memcpy(new_debug_log->file_name, fileName.c_str(), fileName.size());
        new_debug_log->file_name[fileName.size()] = '\0';
    }
    m_handlers.push_back(std::make_pair(fileName, new_debug_log));

    return new_debug_log;
err_shmget1:
err_shmat1:
    shmdt(new_debug_log);
err_mem_key:
    fclose(fp);
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
    int j = 0;

    if (fileName.empty()) {
        return;
    }

    a = find_handler(fileName);
    if (a == NULL) {
        return;
    }

    a->using_it--;

    if (a->using_it == 0) {
        int shm_id1 = a->shm_id_structure;
        msc_file_handler_t *p , *n;
        pthread_mutex_lock(&a->lock);
        fclose(a->fp);
        pthread_mutex_unlock(&a->lock);
        pthread_mutex_destroy(&a->lock);
        shmdt(a);
        shmctl(shm_id1, IPC_RMID, NULL);
    }

    for (const auto &i: m_handlers) {
        if (i.first == fileName) {
            j++;
        }
    }

    m_handlers.erase(m_handlers.begin() + j, m_handlers.begin() + j + 1);
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
