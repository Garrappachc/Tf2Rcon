project (Tf2Rcon)
cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

# Automatically link Qt executables to qtmain target on Windows
if (POLICY CMP0020)
    cmake_policy (SET CMP0020 NEW)
endif ()

# Double colon in target name means ALIAS or IMPORTED target.
if (POLICY CMP0028)
    cmake_policy (SET CMP0028 NEW)
endif ()

# Disallow use of the LOCATION target property.
if (POLICY CMP0026)
    cmake_policy (SET CMP0026 OLD)
endif ()

if (UNIX AND NOT APPLE)
    set (LINUX 1)
endif ()

set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

# set configuration types (msvc/xcode)
if (CMAKE_CONFIGURATION_TYPES)
    set (CMAKE_CONFIGURATION_TYPES Debug Release)
    set (CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
        "Reset the configurations to what we actually need" FORCE
)
endif()

# set CMAKE_BUILD_TYPE (makefiles)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set (CMAKE_BUILD_TYPE Debug)
else ()
    set (CMAKE_BUILD_TYPE Release)
endif ()

# set CXX_FLAGS
if (CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    set (CMAKE_CXX_FLAGS_RELEASE "-DQT_NO_DEBUG -DQT_NO_DEBUG_OUTPUT")
    set (CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wpedantic -Wextra")
    set (CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
elseif (MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set (CMAKE_CXX_FLAGS_DEBUG "/W4")
    set (CMAKE_CXX_FLAGS_RELEASE "/DQT_NO_DEBUG /DQT_NO_DEBUG_OUTPUT")
endif ()

set (CMAKE_CXX_STANDARD "14")

# find version
if (NOT tf2rcon_VERSION)
    file (READ ${CMAKE_CURRENT_SOURCE_DIR}/VERSION tf2rcon_VERSION)
    string (REPLACE "\n" "" tf2rcon_VERSION ${tf2rcon_VERSION})
    
    include(GetGitRevisionDescription)
    get_git_head_revision(GIT_REFSPEC GIT_SHA1)
    
    if (NOT "${GIT_SHA1}" STREQUAL "GITDIR-NOTFOUND")
        # shorten the sha
        string (SUBSTRING "${GIT_SHA1}" 0 10 GIT_HASH)
        set (tf2rcon_VERSION ${tf2rcon_VERSION}-${GIT_HASH}-git)
    endif ()
endif ()

set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory (src)

# uninstall target
configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
)

add_custom_target (
    uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)
