cmake_minimum_required(VERSION 3.15)

project(ModSecurityIIS C CXX)

find_package(LibXml2 CONFIG REQUIRED)
find_package(PCRE2 CONFIG REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(APR CONFIG REQUIRED)

set(IIS_MODULE_NAME "modsecurityiis")

set(IIS_APACHE_SOURCES
    ../apache2/mod_security2.c
    ../apache2/apache2_config.c
    ../apache2/apache2_io.c
    ../apache2/apache2_util.c
    ../apache2/re.c
    ../apache2/re_operators.c
    ../apache2/re_actions.c
    ../apache2/re_tfns.c
    ../apache2/re_variables.c
    ../apache2/msc_logging.c
    ../apache2/msc_xml.c
    ../apache2/msc_multipart.c
    ../apache2/modsecurity.c
    ../apache2/msc_parsers.c
    ../apache2/msc_util.c
    ../apache2/msc_pcre.c
    ../apache2/persist_dbm.c
    ../apache2/msc_reqbody.c
    ../apache2/msc_geo.c
    ../apache2/msc_gsb.c
    ../apache2/msc_crypt.c
    ../apache2/msc_tree.c
    ../apache2/msc_unicode.c
    ../apache2/acmp.c
    ../apache2/msc_lua.c
    ../apache2/msc_release.c
    ../apache2/msc_status_engine.c
    ../apache2/msc_remote_rules.c
    ../apache2/msc_json.c
    ../apache2/libinjection/libinjection_html5.c
    ../apache2/libinjection/libinjection_sqli.c
    ../apache2/libinjection/libinjection_xss.c
)

set(IIS_STANDALONE_SOURCES
    ../standalone/api.c
    ../standalone/buckets.c
    ../standalone/config.c
    ../standalone/filters.c
    ../standalone/hooks.c
    ../standalone/regex.c
    ../standalone/server.c
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x64")
else()
    set(ARCHITECTURE "x86")
endif()

set(IIS_RESOURCE_MC "${CMAKE_CURRENT_SOURCE_DIR}/ModSecurityIISMessage.mc")

set(MC_GENERATED_RC "${CMAKE_CURRENT_BINARY_DIR}/ModSecurityIISMessage.rc")
set(MC_GENERATED_H "${CMAKE_CURRENT_BINARY_DIR}/ModSecurityIISMessage.h")
add_custom_command(
    OUTPUT ${MC_GENERATED_RC} ${MC_GENERATED_H}
    COMMAND mc.exe
    ARGS -U -h "${CMAKE_CURRENT_BINARY_DIR}/" -r "${CMAKE_CURRENT_BINARY_DIR}/" "${IIS_RESOURCE_MC}"
    DEPENDS "${IIS_RESOURCE_MC}"
    COMMENT "Generating resource files from ${IIS_RESOURCE_MC}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Source files for IIS-specific components
set(IIS_MODULE_SOURCES
    main.cpp
    moduleconfig.cpp
    mymodule.cpp
    mymodule.def
    ${MC_GENERATED_RC}
)

set_source_files_properties(
    ${MC_GENERATED_RC}
    ${MC_GENERATED_H}
    PROPERTIES GENERATED TRUE
)

add_library(${IIS_MODULE_NAME} SHARED 
    ${IIS_APACHE_SOURCES}
    ${IIS_STANDALONE_SOURCES}
    ${IIS_MODULE_SOURCES}
)

# Set the output name and extension
set_target_properties(${IIS_MODULE_NAME} PROPERTIES
    OUTPUT_NAME ${IIS_MODULE_NAME}
    PREFIX ""
    SUFFIX ".dll"
)

target_include_directories(${IIS_MODULE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../apache2
    ${CMAKE_CURRENT_SOURCE_DIR}/../apache2/libinjection
    ${LIBXML2_INCLUDE_DIR}/libxml
    ${CMAKE_CURRENT_SOURCE_DIR}/../standalone
    ${PCRE2_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    ${APR_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

if(APACHE_ROOT)
    if(NOT EXISTS "${APACHE_ROOT}")
        message(FATAL_ERROR "APACHE_ROOT is defined but the directory '${APACHE_ROOT}' does not exist. Please set APACHE_ROOT to a valid Apache installation directory.")
    endif()
    if(NOT EXISTS "${APACHE_ROOT}/lib")
        message(FATAL_ERROR "APACHE_ROOT/lib directory does not exist. Expected: '${APACHE_ROOT}/lib'. Please ensure Apache libraries are available.")
    endif()

    file(TO_CMAKE_PATH "${APACHE_ROOT}" APACHE_ROOT)

    # Create imported targets for Apache libraries
    add_library(Apache::httpd SHARED IMPORTED)
    set_target_properties(Apache::httpd PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${APACHE_ROOT}/include"
        IMPORTED_IMPLIB "${APACHE_ROOT}/lib/libhttpd.lib"
        IMPORTED_LOCATION "${APACHE_ROOT}/bin/libhttpd.dll"
    )

    add_library(Apache::apr SHARED IMPORTED)
    set_target_properties(Apache::apr PROPERTIES
        IMPORTED_IMPLIB "${APACHE_ROOT}/lib/libapr-1.lib"
        IMPORTED_LOCATION "${APACHE_ROOT}/bin/libapr-1.dll"
    )

    add_library(Apache::aprutil SHARED IMPORTED)
    set_target_properties(Apache::aprutil PROPERTIES
        IMPORTED_IMPLIB "${APACHE_ROOT}/lib/libaprutil-1.lib"
        IMPORTED_LOCATION "${APACHE_ROOT}/bin/libaprutil-1.dll"
    )

    add_library(Apache::apriconv SHARED IMPORTED)
    set_target_properties(Apache::apriconv PROPERTIES
        IMPORTED_IMPLIB "${APACHE_ROOT}/lib/libapriconv-1.lib"
        IMPORTED_LOCATION "${APACHE_ROOT}/bin/libapriconv-1.dll"
    )

    target_include_directories(${IIS_MODULE_NAME} PRIVATE
        ${APACHE_ROOT}/include
    )
endif()

set(MODSECURITY_VERSION_FLAG "VERSION_IIS") # Define the version flag string
target_compile_definitions(${IIS_MODULE_NAME} PRIVATE
    inline=APR_INLINE
    AP_DECLARE_STATIC
    WITH_CURL
    WITH_REMOTE_RULES
    MSC_LARGE_STREAM_INPUT
    WITH_YAJL
    ${MODSECURITY_VERSION_FLAG}
)

option(WITH_LUA "Enable Lua support" OFF)
if(WITH_LUA)
    find_package(Lua CONFIG REQUIRED)
    target_compile_definitions(${IIS_MODULE_NAME} PRIVATE WITH_LUA)
    target_include_directories(${IIS_MODULE_NAME} PRIVATE ${LUA_INCLUDE_DIR})
endif()

option(WITH_YAJL "Enable YAJL support" OFF)
if(WITH_YAJL)
    # Manually find YAJL if config.cmake is not available (e.g., from vcpkg)
    find_path(YAJL_INCLUDE_DIR yajl/yajl_common.h
        PATHS "${CMAKE_CURRENT_SOURCE_DIR}/build/vcpkg_installed/${ARCHITECTURE}-windows/include"
        NO_DEFAULT_PATH
    )
    find_library(YAJL_LIBRARY NAMES yajl
        PATHS "${CMAKE_CURRENT_SOURCE_DIR}/build/vcpkg_installed/${ARCHITECTURE}-windows/lib"
        NO_DEFAULT_PATH
    )

    if(YAJL_INCLUDE_DIR AND YAJL_LIBRARY)
        set(YAJL_INCLUDE_DIRS ${YAJL_INCLUDE_DIR})
        set(YAJL_LIBRARIES ${YAJL_LIBRARY})
        target_compile_definitions(${IIS_MODULE_NAME} PRIVATE WITH_YAJL)
        target_include_directories(${IIS_MODULE_NAME} PRIVATE ${YAJL_INCLUDE_DIRS})
    else()
        message(WARNING "YAJL not found. YAJL_INCLUDE_DIR: '${YAJL_INCLUDE_DIR}', YAJL_LIBRARY: '${YAJL_LIBRARY}'. Please ensure yajl is installed via vcpkg in the vcpkg_installed directory. Disabling YAJL support.")
        option(WITH_YAJL "Enable YAJL support" OFF)
    endif() 
endif()

option(WITH_SSDEEP "Enable SSDEEP support" OFF)
if(WITH_SSDEEP)

    if(NOT EXISTS "${SSDEEP_ROOT}")
        message(WARNING "SSDEEP_ROOT is not defined or path does not exist. Current SSDEEP_ROOT: '${SSDEEP_ROOT}'. Please set SSDEEP_ROOT to the ssdeep installation directory. Disabling SSDEEP support.")
        set(WITH_SSDEEP OFF CACHE BOOL "Enable SSDEEP support" FORCE)
    else()

        file(TO_CMAKE_PATH "${SSDEEP_ROOT}" SSDEEP_ROOT)

        find_path(SSDEEP_INCLUDE_DIR fuzzy.h
            PATHS "${SSDEEP_ROOT}"
            NO_DEFAULT_PATH
        )
        
        if(SSDEEP_INCLUDE_DIR)
            target_compile_definitions(${IIS_MODULE_NAME} PRIVATE WITH_SSDEEP)
            target_include_directories(${IIS_MODULE_NAME} PRIVATE ${SSDEEP_INCLUDE_DIR})

            set(SSDEEP_DEF_FILE "${SSDEEP_ROOT}/fuzzy.def")
            if(NOT EXISTS "${SSDEEP_DEF_FILE}")
                message(WARNING "fuzzy.def not found at ${SSDEEP_DEF_FILE}. Disabling SSDEEP support.")
                set(WITH_SSDEEP OFF CACHE BOOL "Enable SSDEEP support" FORCE)
            else()
                set(SSDEEP_GENERATED_LIB "${CMAKE_CURRENT_BINARY_DIR}/fuzzy.lib")
                set(SSDEEP_GENERATED_dll "${CMAKE_CURRENT_BINARY_DIR}/fuzzy.dll")
                
                add_custom_command(
                    OUTPUT ${SSDEEP_GENERATED_LIB}
                    COMMAND lib.exe /machine:${ARCHITECTURE} /def:${SSDEEP_DEF_FILE} /out:${SSDEEP_GENERATED_LIB}
                    DEPENDS "${SSDEEP_DEF_FILE}"
                    COMMENT "Generating SSDEEP .lib from .def for MSVC"
                    VERBATIM
                )
                
                set_source_files_properties(${SSDEEP_GENERATED_LIB} PROPERTIES GENERATED TRUE)
                
                add_custom_target(generate_ssdeep_lib ALL
                    DEPENDS ${SSDEEP_GENERATED_LIB}
                    COMMENT "Ensuring ssdeep lib is generated"
                )
                
                add_dependencies(${IIS_MODULE_NAME} generate_ssdeep_lib)
                
                add_library(SSDEEP::fuzzy SHARED IMPORTED)
                set_target_properties(SSDEEP::fuzzy PROPERTIES
                    INTERFACE_INCLUDE_DIRECTORIES "${SSDEEP_INCLUDE_DIR}"
                    IMPORTED_LOCATION "${SSDEEP_GENERATED_dll}"
                    IMPORTED_IMPLIB "${SSDEEP_GENERATED_LIB}"
                )
                 
             endif()
        else()
             message(WARNING "fuzzy.h not found at ${SSDEEP_INCLUDE_DIR}. Disabling SSDEEP support.")
             set(WITH_SSDEEP OFF CACHE BOOL "Enable SSDEEP support" FORCE)
        endif()
    endif()
endif()

if(MSVC)
    target_compile_options(${IIS_MODULE_NAME} PRIVATE
        /nologo
        /W3
        /wd4244
        /wd4018
    )
endif()

target_link_libraries(${IIS_MODULE_NAME} PRIVATE
    LibXml2::LibXml2
    PCRE2::8BIT
    CURL::libcurl
    ws2_32
    iphlpapi
)

if(APACHE_ROOT)
    target_link_libraries(${IIS_MODULE_NAME} PRIVATE
        Apache::httpd
        Apache::apr
        Apache::aprutil
        Apache::apriconv
    )
else()
    message(WARNING "APACHE_ROOT is not defined or path does not exist. Current APACHE_ROOT: '${APACHE_ROOT}'. Please set APACHE_ROOT to the Apache installation directory.")
endif()

if(WITH_LUA)
    target_link_libraries(${IIS_MODULE_NAME} PRIVATE ${LUA_LIBRARIES})
endif()

if(WITH_YAJL)
    target_link_libraries(${IIS_MODULE_NAME} PRIVATE ${YAJL_LIBRARIES})
endif()

if(WITH_SSDEEP AND SSDEEP_INCLUDE_DIR AND SSDEEP_GENERATED_LIB)
    target_link_libraries(${IIS_MODULE_NAME} PRIVATE SSDEEP::fuzzy)
endif()

if(APACHE_ROOT AND EXISTS "${APACHE_ROOT}/bin")
    add_custom_command(TARGET ${IIS_MODULE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${APACHE_ROOT}/bin/libhttpd.dll"
            $<TARGET_FILE_DIR:${IIS_MODULE_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${APACHE_ROOT}/bin/libaprutil-1.dll"
            $<TARGET_FILE_DIR:${IIS_MODULE_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${APACHE_ROOT}/bin/libapriconv-1.dll"
            $<TARGET_FILE_DIR:${IIS_MODULE_NAME}>
        COMMENT "Copying Apache DLLs to output directory"
    )
else()
    message(WARNING "APACHE_ROOT is not defined or path does not exist. Current APACHE_ROOT: '${APACHE_ROOT}'. Please set APACHE_ROOT to the Apache installation directory.")
endif()

if(WITH_SSDEEP AND SSDEEP_ROOT AND EXISTS "${SSDEEP_ROOT}/fuzzy.dll")
    add_custom_command(TARGET ${IIS_MODULE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SSDEEP_ROOT}/fuzzy.dll"
            $<TARGET_FILE_DIR:${IIS_MODULE_NAME}>
        COMMENT "Copying SSDEEP DLL to output directory"
    )
endif()


# Install target - copy to release files directory
install(TARGETS ${IIS_MODULE_NAME}
    RUNTIME DESTINATION .
    LIBRARY DESTINATION .
)

if(APACHE_ROOT AND EXISTS "${APACHE_ROOT}/bin")
    install(FILES
        "${APACHE_ROOT}/bin/libhttpd.dll"
        "${APACHE_ROOT}/bin/libaprutil-1.dll"
        "${APACHE_ROOT}/bin/libapriconv-1.dll"
        DESTINATION .
    )
endif()

if(WITH_SSDEEP AND SSDEEP_ROOT AND EXISTS "${SSDEEP_ROOT}/fuzzy.dll")
    install(FILES
        "${SSDEEP_ROOT}/fuzzy.dll"
        DESTINATION .
    )
endif()

# Also install the PDB file if it's generated
install(FILES $<TARGET_PDB_FILE:${IIS_MODULE_NAME}> DESTINATION . OPTIONAL)