cmake_minimum_required(VERSION 3.15)
project(lmdb LANGUAGES C)

include(GNUInstallDirs)

# LMDB library:

set(LMDB_SOURCES
    ${LMDB_SRC_DIR}/lmdb.h
    ${LMDB_SRC_DIR}/mdb.c
    ${LMDB_SRC_DIR}/midl.c
    ${LMDB_SRC_DIR}/midl.h
)

# Crypto module loader, added in LMDB 1.0
if(EXISTS ${LMDB_SRC_DIR}/module.c)
    list(APPEND LMDB_SOURCES ${LMDB_SRC_DIR}/module.c)
endif()

add_library(lmdb ${LMDB_SOURCES})

set_target_properties(lmdb PROPERTIES
    PUBLIC_HEADER ${LMDB_SRC_DIR}/lmdb.h
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

find_package(Threads REQUIRED)
target_link_libraries(lmdb PUBLIC Threads::Threads ${CMAKE_DL_LIBS})

install(TARGETS lmdb
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Only defined on platforms that actually consult MDB_USE_ROBUST. Since LMDB 1.0
# mdb.c tests `defined(MDB_USE_ROBUST)` rather than its value to pick the locking
# backend, so defining it as 0 elsewhere would silently override upstream defaults.
if (DEFINED LMDB_ENABLE_ROBUST_MUTEX)
    if (LMDB_ENABLE_ROBUST_MUTEX)
        target_compile_definitions(lmdb PUBLIC MDB_USE_ROBUST=1)
    else ()
        target_compile_definitions(lmdb PUBLIC MDB_USE_ROBUST=0)
    endif ()
endif ()

# LMDB utilities:

if(NOT WIN32)
    foreach(TOOL mdb_stat mdb_copy mdb_dump mdb_load mdb_drop)
        if(EXISTS ${LMDB_SRC_DIR}/${TOOL}.c)
            add_executable(${TOOL} ${LMDB_SRC_DIR}/${TOOL}.c)
            target_link_libraries(${TOOL} lmdb)
            install(TARGETS ${TOOL} DESTINATION ${CMAKE_INSTALL_BINDIR})
        endif()
    endforeach()
endif()
