cmake_minimum_required(VERSION 3.24)

# -------------------------------------------------------
# Project
# -------------------------------------------------------
project(BimQAT2
    VERSION 1.0.0
    LANGUAGES CXX
)

# -------------------------------------------------------
# C++ Standard
# -------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------------------------------------
# Build Configurations (Multi-config)
# -------------------------------------------------------
if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES
        Debug
        RelWithDebInfo
        Release
        MinSizeRel
        BIM
        CACHE STRING "" FORCE
    )
endif()

# -------------------------------------------------------
# Legacy: Environment variable (reserved for reference)
# -------------------------------------------------------
set(YBimSdkDir $ENV{YBimSdkDir})
# 原输出方式（已废弃，仅作参考）
# set(OUTPUT_DIR "${YBimSdkDir}/run/${Platform}/Bim/products/BIM_QAT")

# -------------------------------------------------------
# Output Directories (auto by config & platform)
# -------------------------------------------------------
if(MSVC)
    set(PROJECT_PLATFORM ${CMAKE_VS_PLATFORM_NAME})
else()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "64")
        set(PROJECT_PLATFORM x64)
    else()
        set(PROJECT_PLATFORM x86)
    endif()
endif()

set(BASE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/Bin")

foreach(CONFIG Debug Release RelWithDebInfo MinSizeRel BIM)
    string(TOUPPER ${CONFIG} CONFIG_UPPER)

    set(OUTPUT_DIR_${CONFIG_UPPER}
        "${BASE_OUTPUT_DIR}/${CONFIG}/${PROJECT_PLATFORM}"
    )

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${OUTPUT_DIR_${CONFIG_UPPER}})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${OUTPUT_DIR_${CONFIG_UPPER}})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${OUTPUT_DIR_${CONFIG_UPPER}})
endforeach()

if(NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
        "${BASE_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_PLATFORM}"
    )
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
        "${BASE_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_PLATFORM}"
    )
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
        "${BASE_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_PLATFORM}"
    )
endif()

# -------------------------------------------------------
# MSVC Runtime Library (MFC: /MDd / MD)
# -------------------------------------------------------
if(MSVC)
    # Debug  -> /MDd
    # Others -> /MD
    set(CMAKE_MSVC_RUNTIME_LIBRARY
        "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    )
endif()

# -------------------------------------------------------
# Compiler Options (project-wide, opt-in)
# -------------------------------------------------------
add_library(project_options INTERFACE)
add_library(project_warnings INTERFACE)

target_compile_options(project_options INTERFACE
    $<$<CONFIG:Debug>:-DDEBUG>
    $<$<CONFIG:RelWithDebInfo>:-DNDEBUG>
    $<$<CONFIG:Release>:-DNDEBUG>
    $<$<CONFIG:BIM>:-DBIM>
)

if(MSVC)
    target_compile_options(project_warnings INTERFACE
        /W3
        /permissive-
        /utf-8
    )
else()
    target_compile_options(project_warnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# -------------------------------------------------------
# Windows / MFC Macros
# -------------------------------------------------------
if(WIN32)
    add_compile_definitions(
        UNICODE
        _UNICODE
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
endif()

# -------------------------------------------------------
# BIM Configuration
# -------------------------------------------------------
add_library(project_bim INTERFACE)

target_compile_definitions(project_bim INTERFACE
    $<$<CONFIG:BIM>:BIM_BUILD>
)

if(MSVC)
    target_compile_options(project_bim INTERFACE
        $<$<CONFIG:BIM>:/O2>
    )
else()
    target_compile_options(project_bim INTERFACE
        $<$<CONFIG:BIM>:-O2>
    )
endif()

# -------------------------------------------------------
# Subdirectories
# -------------------------------------------------------
add_subdirectory(extern/src/GridCtrl)

# -------------------------------------------------------
# Usage Example (for targets)
# -------------------------------------------------------
# target_link_libraries(your_mfc_target
#     PRIVATE
#         project_options
#         project_warnings
#         project_bim
# )
