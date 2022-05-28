# project
cmake_minimum_required(VERSION 3.4)
project(CilixRdc VERSION 1.0.0.0 LANGUAGES C)
set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_BUILD_TYPE "Debug")

# cmake incude
include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(CheckCCompilerFlag)
include(GNUInstallDirs)
include(CTest)

# c standard and symbol visibility
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD 99)

#################################################

# Compiler check
string(CONCAT is-msvc $<OR:
    $<C_COMPILER_ID:MSVC>,
    $<STREQUAL:${CMAKE_C_COMPILER_FRONTEND_VARIANT},MSVC>
>)

check_c_compiler_flag(/W4 CILIX_LINT_W4)
check_c_compiler_flag(/wd4100 CILIX_LINT_NO_UNUSED_PARAMETER_MSVC)
check_c_compiler_flag(/wd4127 CILIX_LINT_NO_CONDITIONAL_CONSTANT_MSVC)
check_c_compiler_flag(/wd4201 CILIX_LINT_NO_NONSTANDARD_MSVC)
check_c_compiler_flag(/wd4206 CILIX_LINT_NO_NONSTANDARD_EMPTY_TU_MSVC)
check_c_compiler_flag(/wd4210 CILIX_LINT_NO_NONSTANDARD_FILE_SCOPE_MSVC)
check_c_compiler_flag(/wd4232 CILIX_LINT_NO_NONSTANDARD_NONSTATIC_DLIMPORT_MSVC)
check_c_compiler_flag(/wd4456 CILIX_LINT_NO_HIDES_LOCAL)
check_c_compiler_flag(/wd4457 CILIX_LINT_NO_HIDES_PARAM)
check_c_compiler_flag(/wd4459 CILIX_LINT_NO_HIDES_GLOBAL)
check_c_compiler_flag(/wd4706 CILIX_LINT_NO_CONDITIONAL_ASSIGNMENT_MSVC)
check_c_compiler_flag(/wd4996 CILIX_LINT_NO_UNSAFE_MSVC)

#[[ initialization such as 'struct sth = {0}' is not allowed
check_c_compiler_flag(-Wall CILIX_LINT_WALL) # DO NOT use this under MSVC
]]

# TODO: Place these into its own function
check_c_compiler_flag(-Wno-unused-parameter CILIX_LINT_NO_UNUSED_PARAMETER)
check_c_compiler_flag(-Wstrict-prototypes CILIX_LINT_STRICT_PROTOTYPES)
check_c_compiler_flag(-Wextra CILIX_LINT_EXTRA)

set(lint-no-unused-parameter $<$<BOOL:${CILIX_LINT_NO_UNUSED_PARAMETER}>:-Wno-unused-parameter>)
set(lint-strict-prototypes $<$<BOOL:${CILIX_LINT_STRICT_PROTOTYPES}>:-Wstrict-prototypes>)
set(lint-extra $<$<BOOL:${CILIX_LINT_EXTRA}>:-Wextra>)
set(lint-w4 $<$<BOOL:${CILIX_LINT_W4}>:/W4>)
set(lint-no-unused-parameter-msvc $<$<BOOL:${CILIX_LINT_NO_UNUSED_PARAMETER_MSVC}>:/wd4100>)
set(lint-no-conditional-constant-msvc $<$<BOOL:${CILIX_LINT_NO_CONDITIONAL_CONSTANT_MSVC}>:/wd4127>)
set(lint-no-nonstandard-msvc $<$<BOOL:${CILIX_LINT_NO_NONSTANDARD_MSVC}>:/wd4201>)
set(lint-no-nonstandard-empty-tu-msvc $<$<BOOL:${CILIX_LINT_NO_NONSTANDARD_EMPTY_TU_MSVC}>:/wd4206>)
set(lint-no-nonstandard-file-scope-msvc $<$<BOOL:${CILIX_LINT_NO_NONSTANDARD_FILE_SCOPE_MSVC}>:/wd4210>)
set(lint-no-nonstandard-nonstatic-dlimport-msvc $<$<BOOL:${CILIX_LINT_NO_NONSTANDARD_NONSTATIC_DLIMPORT_MSVC}>:/wd4232>)
set(lint-no-hides-local-msvc $<$<BOOL:${CILIX_LINT_NO_HIDES_LOCAL}>:/wd4456>)
set(lint-no-hides-param-msvc $<$<BOOL:${CILIX_LINT_NO_HIDES_PARAM}>:/wd4457>)
set(lint-no-hides-global-msvc $<$<BOOL:${CILIX_LINT_NO_HIDES_GLOBAL}>:/wd4459>)
set(lint-no-conditional-assignment-msvc $<$<BOOL:${CILIX_LINT_NO_CONDITIONAL_ASSIGNMENT_MSVC}>:/wd4706>)
set(lint-no-unsafe-msvc $<$<BOOL:${CILIX_LINT_NO_UNSAFE_MSVC}>:/wd4996>)

#[[ initialization such as 'struct sth = {0}' is not allowed
# Unfortunately, this one is complicated because MSVC and clang-cl support -Wall
# but using it is like calling -Weverything
string(CONCAT lint-default $<
    $<AND:$<BOOL:${CILIX_LINT_WALL}>,$<NOT:${is-msvc}>>:-Wall
>)
]]

#################################################

# cilix_cflags , cilix_defines , cilix_libraries

list(APPEND cilix_cflags ${lint-strict-prototypes} ${lint-extra} ${lint-default} ${lint-w4})
list(APPEND cilix_cflags ${lint-no-unused-parameter})
list(APPEND cilix_cflags ${lint-no-unused-parameter-msvc})
list(APPEND cilix_cflags ${lint-no-conditional-constant-msvc})
list(APPEND cilix_cflags ${lint-no-nonstandard-msvc})
list(APPEND cilix_cflags ${lint-no-nonstandard-empty-tu-msvc})
list(APPEND cilix_cflags ${lint-no-nonstandard-file-scope-msvc})
list(APPEND cilix_cflags ${lint-no-nonstandard-nonstatic-dlimport-msvc})
list(APPEND cilix_cflags ${lint-no-hides-local-msvc})
list(APPEND cilix_cflags ${lint-no-hides-param-msvc})
list(APPEND cilix_cflags ${lint-no-hides-global-msvc})
list(APPEND cilix_cflags ${lint-no-conditional-assignment-msvc})
list(APPEND cilix_cflags ${lint-no-unsafe-msvc})


if(WIN32)
    list(APPEND cilix_defines WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0600)
    list(APPEND cilix_libraries psapi iphlpapi userenv ws2_32)
else()
	list(APPEND cilix_cflags -Wno-implicit-fallthrough)
	list(APPEND cilix_cflags -Wno-unused-result)
    list(APPEND cilix_defines _FILE_OFFSET_BITS=64 _LARGEFILE_SOURCE)
    if(NOT CMAKE_SYSTEM_NAME MATCHES "Android|OS390")
        # TODO: This should be replaced with find_package(Threads) if possible
        # Android has pthread as part of its c library, not as a separate
        # libpthread.so.
        list(APPEND cilix_libraries pthread)
    endif()
endif()

if(APPLE)
    list(APPEND cilix_defines _DARWIN_UNLIMITED_SELECT=1 _DARWIN_USE_64_BIT_INODE=1)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND cilix_defines _GNU_SOURCE _POSIX_C_SOURCE=200112)
    list(APPEND cilix_libraries dl rt)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    list(APPEND cilix_defines _GNU_SOURCE)
    list(APPEND cilix_libraries dl)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "AIX")
    list(APPEND cilix_defines
        _ALL_SOURCE
        _LINUX_SOURCE_COMPAT
        _THREAD_SAFE
        _XOPEN_SOURCE=500
        HAVE_SYS_AHAFS_EVPRODS_H)
    list(APPEND cilix_libraries perfstat)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "OS390")
    list(APPEND cilix_defines
        PATH_MAX=255
        _AE_BIMODAL
        _ALL_SOURCE
        _ISOC99_SOURCE
        _LARGE_TIME_API
        _OPEN_MSGQ_EXT
        _OPEN_SYS_FILE_EXT
        _OPEN_SYS_IF_EXT
        _OPEN_SYS_SOCK_EXT3
        _OPEN_SYS_SOCK_IPV6
        _UNIX03_SOURCE
        _UNIX03_THREADS
        _UNIX03_WITHDRAWN
        _XOPEN_SOURCE_EXTENDED)
    list(APPEND cilix_cflags -Wc,DLL -Wc,exportall -Wc,xplink)
    list(APPEND cilix_libraries -Wl,xplink)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "OS400")
    list(APPEND cilix_defines
        _ALL_SOURCE
        _LINUX_SOURCE_COMPAT
        _THREAD_SAFE
        _XOPEN_SOURCE=500)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
    list(APPEND cilix_defines __EXTENSIONS__ _XOPEN_SOURCE=500)
    list(APPEND cilix_libraries kstat nsl sendfile socket)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
    list(APPEND cilix_libraries kvm)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "DragonFly")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
endif()

#################################################

# cmake config.h
set(_CILIX_CMAKE_CONFIG_PATH "./build")
configure_file(./Config.h.in ${_CILIX_CMAKE_CONFIG_PATH}/Config.h)
include_directories(${_CILIX_CMAKE_CONFIG_PATH})

# build version
set(_CILIX_BUILD_VERSION "${PROJECT_VERSION}")
set(_CILIX_BUILD_VERSION_3 "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(_CILIX_BUILD_VERSION_4 "${PROJECT_VERSION_TWEAK}")

# macros on building
set(_CILIX_BUILD_MACRO "_CILIX_ON_BUILDING")
add_definitions("-D${_CILIX_BUILD_MACRO}")

# build name
set(_CILIX_BUILD_NAME "libs")

# build folder
set(_CILIX_BUILD_FOLDER "${CMAKE_SYSTEM_NAME}")

# build path
set(_CILIX_BUILD_PATH "${_CILIX_BUILD_NAME}/${_CILIX_BUILD_FOLDER}/${CMAKE_SYSTEM_PROCESSOR}")

# set build path
set(LIBRARY_OUTPUT_PATH "./${_CILIX_BUILD_PATH}")
set(EXECUTABLE_OUTPUT_PATH "./${_CILIX_BUILD_PATH}")

#################################################

# claux/libuv1.38.1
include_directories(./claux/libuv1.38.1/include)
link_directories(./claux/libuv1.38.1/${_CILIX_BUILD_PATH})

# claux/tomlc99
include_directories(./claux/tomlc99)
aux_source_directory(./claux/tomlc99 DIR_TOMLC99)

# claux/base64c
include_directories(./claux/base64c)
aux_source_directory(./claux/base64c DIR_BASE64C)

# claux/cjson
include_directories(./claux/cjson)
aux_source_directory(./claux/cjson DIR_CJSON)

# cilix_uv
include_directories(./cilix_uv)
aux_source_directory(./cilix_uv DIR_CILIXUV)

# headers
include_directories(./cilix_include)

# sp headers
include_directories(./cilix_sp_include)

# os_config headers
include_directories(./cilix_os_config)

# cilix_sp
aux_source_directory(./cilix_sp DIR_CILIXSP)

# cilix_spi_ap
aux_source_directory(./cilix_spi_ap DIR_SPI_AP)
set(cilix_spi_ap_src
    ${DIR_SPI_AP}
    ${DIR_CILIXSP}
    ${DIR_CILIXUV}
    ${DIR_TOMLC99}
    ${DIR_BASE64C}
    ${DIR_CJSON})
add_library(cilix_spi_ap SHARED ${cilix_spi_ap_src})
target_compile_definitions(cilix_spi_ap PRIVATE ${cilix_defines})
target_compile_options(cilix_spi_ap PRIVATE ${cilix_cflags})
target_link_libraries(cilix_spi_ap uv_a ${cilix_libraries})
# set_target_properties(cilix_spi_ap PROPERTIES VERSION ${_CILIX_BUILD_VERSION_3} SOVERSION ${_CILIX_BUILD_VERSION_4})

# cilix_sp_cp
set(cilix_sp_cp_src
    ${DIR_CILIXSP}
    ${DIR_CILIXUV}
    ${DIR_TOMLC99}
    ${DIR_BASE64C}
    ${DIR_CJSON})
add_library(cilix_sp_cp SHARED ${cilix_sp_cp_src})
target_compile_definitions(cilix_sp_cp PRIVATE ${cilix_defines})
target_compile_options(cilix_sp_cp PRIVATE ${cilix_cflags})
target_link_libraries(cilix_sp_cp uv_a ${cilix_libraries})
# set_target_properties(cilix_sp_cp PROPERTIES VERSION ${_CILIX_BUILD_VERSION_3} SOVERSION ${_CILIX_BUILD_VERSION_4})

# cilix_rdc
aux_source_directory(./cilix_test DIR_TEST)
aux_source_directory(./cilix_rdc DIR_RDC)
set(cilix_rdc_src
    ${DIR_TEST}
    ${DIR_RDC}
    ${DIR_CILIXSP}
    ${DIR_CILIXUV}
    ${DIR_TOMLC99}
    ${DIR_BASE64C}
    ${DIR_CJSON})
add_executable(cilix_rdc ${cilix_rdc_src})
target_compile_definitions(cilix_rdc PRIVATE ${cilix_defines})
target_compile_options(cilix_rdc PRIVATE ${cilix_cflags})
target_link_libraries(cilix_rdc uv_a ${cilix_libraries})

#################################################

message(STATUS "
>>>>> [${CMAKE_PROJECT_NAME}] summary start:
    Target system:
        system name:        ${CMAKE_SYSTEM_NAME}
        system version:     ${CMAKE_SYSTEM_VERSION}
        system processor:   ${CMAKE_SYSTEM_PROCESSOR}
    Compiler:
        Build type:         ${CMAKE_BUILD_TYPE}
        C compiler:         ${CMAKE_C_COMPILER}
        CFLAGS:             ${CMAKE_C_FLAGS_${_build_type}} ${CMAKE_C_FLAGS}
        SHARED_PREFIX:      ${CMAKE_SHARED_LIBRARY_PREFIX}
        SHARED_SUFFIX:      ${CMAKE_SHARED_LIBRARY_SUFFIX}
        EXECUTABLE_SUFFIX:  ${CMAKE_EXECUTABLE_SUFFIX}
    Cilix setting:
        cilix_build_ver:    ${_CILIX_BUILD_VERSION}
        cilix_build_path:   ${_CILIX_BUILD_PATH}
        cilix_defines:      ${cilix_defines}
        cilix_cflags:       ${cilix_cflags}
        cilix_libraries:    ${cilix_libraries}
>>>>> [${CMAKE_PROJECT_NAME}] summary end.")
