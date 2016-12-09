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

#include "src/debug_log/debug_log_writer.h"

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>

namespace modsecurity {
namespace debug_log {

debug_log_file_handler_t *DebugLogWriter::find_handler(
    const std::string &fileName) {
    debug_log_file_handler_t *current = m_first;
    while (current != NULL) {
        if (current->file_name == fileName) {
            return current;
        }
        current = reinterpret_cast<debug_log_file_handler_t *>(current->next);
    }

    return NULL;
}


debug_log_file_handler_t *DebugLogWriter::add_new_handler(
    const std::string &fileName, std::string *error) {
    debug_log_file_handler_t *current = m_first;
    int shm_id;
    key_t mem_key_structure;
    key_t mem_key_file_name;
    debug_log_file_handler_t *new_debug_log;
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

    shm_id = shmget(mem_key_structure, sizeof (debug_log_file_handler_t),
        IPC_CREAT | 0666);
    if (shm_id < 0) {
        error->assign("Failed to allocate shared memory (1): ");
        error->append(strerror(errno));
        goto err_shmget1;
    }

    new_debug_log = reinterpret_cast<debug_log_file_handler_t *>(
        shmat(shm_id, NULL, 0));
    if ((reinterpret_cast<char *>(new_debug_log)[0]) == -1) {
        error->assign("Failed to attach shared memory (1): ");
        error->append(strerror(errno));
        goto err_shmat1;
    }
    memset(new_debug_log, '\0', sizeof(debug_log_file_handler_t));

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
                current = reinterpret_cast<debug_log_file_handler_t *>(
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


int DebugLogWriter::open(const std::string& fileName, std::string *error) {
    debug_log_file_handler_t *a = find_handler(fileName);
    if (a == NULL) {
        a = add_new_handler(fileName, error);
        if (error->size() > 0) {
            return -1;
        }
    }
    if (a == NULL) {
        error->assign("Not able to open DebugLog: " + fileName);
        return -1;
    }

    a->using_it++;

    return 0;
}


void DebugLogWriter::close(const std::string& fileName) {
    debug_log_file_handler_t *a;

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
        debug_log_file_handler_t *p , *n;
        pthread_mutex_lock(&a->lock);
        fclose(a->fp);

        p = reinterpret_cast<debug_log_file_handler_t *>(a->previous);
        n = reinterpret_cast<debug_log_file_handler_t *>(a->next);
        if (p != NULL) {
            p->next = reinterpret_cast<debug_log_file_handler_t *>(n);
        }
        if (n != NULL) {
            n->previous = reinterpret_cast<debug_log_file_handler_t *>(p);
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


void DebugLogWriter::write_log(const std::string& fileName,
    const std::string &msg) {
    std::string lmsg = msg + "\n";
    size_t wrote;

    debug_log_file_handler_t *a = find_handler(fileName);
    if (a == NULL) {
        std::cerr << "debug log file is not open: " << msg << std::endl;
        return;
    }

    pthread_mutex_lock(&a->lock);
    wrote = write(a->file_handler, reinterpret_cast<const char *>(lmsg.c_str()),
        lmsg.size());
    if (wrote < msg.size()) {
        std::cerr << "failed to write debug log: " << msg;
    }
    pthread_mutex_unlock(&a->lock);
}


}  // namespace debug_log
}  // namespace modsecurity
