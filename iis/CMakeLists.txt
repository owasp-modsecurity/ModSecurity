cmake_minimum_required(VERSION 3.15)

set(VCPKG_BUILD_TYPE release)

project(ModSecurityIIS C CXX)

find_package(LibXml2 CONFIG REQUIRED)
find_package(PCRE2 CONFIG REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(APR CONFIG REQUIRED)

# iis/CMakeLists.txt
set(IIS_MODULE_NAME "modsecurityiis")  # Name should match the original output

# Source files for IIS module (reusing Apache sources)
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

# Source files for standalone components (if they exist in the project)
set(IIS_STANDALONE_SOURCES
    ../standalone/api.c
    ../standalone/buckets.c
    ../standalone/config.c
    ../standalone/filters.c
    ../standalone/hooks.c
    ../standalone/regex.c
    ../standalone/server.c
)

# Source files for IIS-specific components
set(IIS_MODULE_SOURCES
    main.cpp
    moduleconfig.cpp
    mymodule.cpp
)


# Determine architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x64")
else()
    set(ARCHITECTURE "x86")
endif()

# Check if standalone directory exists, if not, exclude those sources
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../standalone)
    set(IIS_STANDALONE_SOURCES "")
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

set(MC_GENERATED_RES "${CMAKE_CURRENT_BINARY_DIR}/ModSecurityIISMessage.res")
add_custom_command(
    OUTPUT ${MC_GENERATED_RES}
    COMMAND rc.exe
    ARGS /fo "${MC_GENERATED_RES}" "${MC_GENERATED_RC}"
    DEPENDS ${MC_GENERATED_RC}
    COMMENT "Building resource file: ${MC_GENERATED_RES}"
)

set_source_files_properties(
    ${MC_GENERATED_RC}
    ${MC_GENERATED_H}
    ${MC_GENERATED_RES}
    PROPERTIES GENERATED TRUE
)

add_library(${IIS_MODULE_NAME} SHARED 
    ${IIS_APACHE_SOURCES}
    ${IIS_STANDALONE_SOURCES}
    ${IIS_MODULE_SOURCES}
    ${MC_GENERATED_RES}
)

# Set the output name and extension
set_target_properties(${IIS_MODULE_NAME} PROPERTIES
    OUTPUT_NAME ${IIS_MODULE_NAME}
    PREFIX ""
    SUFFIX ".dll"
)

# Include directories
target_include_directories(${IIS_MODULE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../apache2
    ${CMAKE_CURRENT_SOURCE_DIR}/../apache2/libinjection
    ${LIBXML2_INCLUDE_DIR}/libxml
    ${PCRE2_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    ${APR_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Include standalone directory if it exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../standalone)
    target_include_directories(${IIS_MODULE_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../standalone
    )
endif()

# Apache-specific includes
if(APACHE_ROOT)
    if(NOT EXISTS "${APACHE_ROOT}")
        message(FATAL_ERROR "APACHE_ROOT is defined but the directory '${APACHE_ROOT}' does not exist. Please set APACHE_ROOT to a valid Apache installation directory.")
    endif()
    if(NOT EXISTS "${APACHE_ROOT}/lib")
        message(FATAL_ERROR "APACHE_ROOT/lib directory does not exist. Expected: '${APACHE_ROOT}/lib'. Please ensure Apache libraries are available.")
    endif()
    target_include_directories(${IIS_MODULE_NAME} PRIVATE
        ${APACHE_ROOT}/include
    )
endif()

# Compile definitions to match the original Makefile.win
set(MODSECURITY_VERSION_FLAG "VERSION_IIS") # Define the version flag string
target_compile_definitions(${IIS_MODULE_NAME} PRIVATE
    WIN32
    WINNT
    inline=APR_INLINE
    AP_DECLARE_STATIC
    WITH_CURL
    WITH_REMOTE_RULES
    MSC_LARGE_STREAM_INPUT
    WITH_YAJL
    ${MODSECURITY_VERSION_FLAG}  # Use the defined version flag
)

option(WITH_LUA "Enable Lua support" OFF)
# Optional compile definitions
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
        option(WITH_YAJL "Enable YAJL support" OFF) # Disable if not found
    endif() 
endif()

option(WITH_SSDEEP "Enable SSDEEP support" OFF)
if(WITH_SSDEEP)

    set(SSDEEP_ROOT "" CACHE PATH "Path to manually built ssdeep")
    if(NOT SSDEEP_ROOT OR NOT EXISTS "${SSDEEP_ROOT}")
        message(WARNING "SSDEEP_ROOT is not defined or path does not exist. Current SSDEEP_ROOT: '${SSDEEP_ROOT}'. Please set SSDEEP_ROOT to the ssdeep installation directory. Disabling SSDEEP support.")
        set(WITH_SSDEEP OFF CACHE BOOL "Enable SSDEEP support" FORCE)
    else()
        message(STATUS "SSDEEP_ROOT: ${SSDEEP_ROOT}")

        # 查找头文件
        find_path(SSDEEP_INCLUDE_DIR fuzzy.h
            PATHS "${SSDEEP_ROOT}/include"
            NO_DEFAULT_PATH
        )
        
        if(SSDEEP_INCLUDE_DIR)
            message(STATUS "Found manually built ssdeep include: ${SSDEEP_INCLUDE_DIR}")
            target_compile_definitions(${IIS_MODULE_NAME} PRIVATE WITH_SSDEEP)
            target_include_directories(${IIS_MODULE_NAME} PRIVATE ${SSDEEP_INCLUDE_DIR})

            # 检查 fuzzy.def 文件是否存在
            set(SSDEEP_DEF_FILE "${SSDEEP_ROOT}/fuzzy.def")
            if(NOT EXISTS "${SSDEEP_DEF_FILE}")
                message(WARNING "fuzzy.def not found at ${SSDEEP_DEF_FILE}. Disabling SSDEEP support.")
                set(WITH_SSDEEP OFF CACHE BOOL "Enable SSDEEP support" FORCE)
            else()
                set(SSDEEP_GENERATED_LIB "${CMAKE_CURRENT_BINARY_DIR}/fuzzy.lib")
                
                # 添加自定义命令生成 fuzzy.lib
                add_custom_command(
                    OUTPUT ${SSDEEP_GENERATED_LIB}
                    COMMAND lib.exe /machine:${ARCHITECTURE} /def:${SSDEEP_DEF_FILE} /out:${SSDEEP_GENERATED_LIB}
                    DEPENDS "${SSDEEP_DEF_FILE}"
                    COMMENT "Generating SSDEEP .lib from .def for MSVC"
                    VERBATIM
                )
                
                # 确保自定义命令的输出被标记为生成文件
                set_source_files_properties(${SSDEEP_GENERATED_LIB} PROPERTIES GENERATED TRUE)
                
                # 添加自定义目标确保生成 fuzzy.lib
                add_custom_target(generate_ssdeep_lib ALL
                    DEPENDS ${SSDEEP_GENERATED_LIB}
                    COMMENT "Ensuring ssdeep lib is generated"
                )
                
                # 使主目标依赖于 fuzzy.lib 的生成
                add_dependencies(${IIS_MODULE_NAME} generate_ssdeep_lib)
                
            endif()
        else()
            message(WARNING "SSDEEP include (fuzzy.h) not found at ${SSDEEP_ROOT}/include. Disabling SSDEEP support.")
            set(WITH_SSDEEP OFF CACHE BOOL "Enable SSDEEP support" FORCE)
        endif()
    endif()
endif()

# Compiler-specific options for MSVC to match the original Makefile.win
if(MSVC)
    target_compile_options(${IIS_MODULE_NAME} PRIVATE
        /nologo
        /O2
        /W3
        /wd4244
        /wd4018
        /MD
        /Zi
    )
    
    # Linker options to match the original Makefile.win
    set_target_properties(${IIS_MODULE_NAME} PROPERTIES
        LINK_FLAGS "/DEBUG /OPT:REF /OPT:ICF"
    )
endif()

# Link libraries to match the original Makefile.win
target_link_libraries(${IIS_MODULE_NAME} PRIVATE
    LibXml2::LibXml2
    PCRE2::8BIT
    CURL::libcurl
    kernel32
    user32
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32
    ws2_32
    iphlpapi
)

# Apache-specific libraries
if(APACHE_ROOT)
    target_link_libraries(${IIS_MODULE_NAME} PRIVATE
        ${APACHE_ROOT}/lib/libhttpd.lib
        ${APACHE_ROOT}/lib/libapr-1.lib
        ${APACHE_ROOT}/lib/libaprutil-1.lib
    )
endif()

# Optional link libraries
if(WITH_LUA)
    target_link_libraries(${IIS_MODULE_NAME} PRIVATE ${LUA_LIBRARIES})
endif()

if(WITH_YAJL)
    target_link_libraries(${IIS_MODULE_NAME} PRIVATE ${YAJL_LIBRARIES})
endif()

if(WITH_SSDEEP AND SSDEEP_INCLUDE_DIR AND SSDEEP_GENERATED_LIB)
    target_link_libraries(${IIS_MODULE_NAME} PRIVATE ${SSDEEP_GENERATED_LIB})
else()
    message(WARNING "SSDEEP library not found or generated. Disabling SSDEEP support.")
    option(WITH_SSDEEP "Enable SSDEEP support" OFF) # Disable if library not found
endif()

# Install target - copy to release files directory
install(TARGETS ${IIS_MODULE_NAME}
    RUNTIME DESTINATION .
    LIBRARY DESTINATION .
)

# Also install the PDB file if it's generated
install(FILES $<TARGET_PDB_FILE:${IIS_MODULE_NAME}> DESTINATION . OPTIONAL)