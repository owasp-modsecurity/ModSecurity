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

#ifndef SRC_UTILS_SHARED_FILES_H_
#define SRC_UTILS_SHARED_FILES_H_


#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include <string>

#include "modsecurity/transaction.h"
#include "modsecurity/audit_log.h"


namespace modsecurity {
namespace utils {


typedef struct msc_file_handler {
    char *file_name;
    FILE *fp;
    int file_handler;
    int shm_id_file_name;
    int shm_id_structure;
    int using_it;
    pthread_mutex_t lock;
    void *next;
    void *previous;
} msc_file_handler_t;


class SharedFiles {
 public:
    bool open(const std::string& fileName, std::string *error);
    void close(const std::string& fileName);
    bool write(const std::string& fileName, const std::string &msg,
        std::string *error);
    static SharedFiles& getInstance() {
        static SharedFiles instance;
        return instance;
    }

 protected:
    msc_file_handler_t *find_handler(const std::string &fileName);
    msc_file_handler_t *add_new_handler(const std::string &fileName,
        std::string *error);

 private:
    SharedFiles() : m_first(NULL) { }
    ~SharedFiles() { }

    // C++ 03
    // ========
    // Dont forget to declare these two. You want to make sure they
    // are unacceptable otherwise you may accidentally get copies of
    // your singleton appearing.
    SharedFiles(SharedFiles const&);
    void operator=(SharedFiles const&);

    msc_file_handler_t *m_first;
};


}  // namespace utils
}  // namespace modsecurity

#endif  // SRC_UTILS_SHARED_FILES_H_
