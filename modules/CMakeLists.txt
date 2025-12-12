cmake_minimum_required(VERSION 3.24)
project(common-lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# -------------------------------------------------------
# 统一设置输出目录
# -------------------------------------------------------
# 解决方案根目录
set(SOLUTION_DIR "${CMAKE_SOURCE_DIR}")
# 拼接为 Solution/Bin/<Platform>/<Config> 形式
# 对多配置生成器：VS、Xcode 等，最终输出目录必须区分配置
set(OUTPUT_DIR_BASE "${SOLUTION_DIR}/Bin/${Platform}")

# 设置全局输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR_BASE})  # .lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR_BASE})  # .dll
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR_BASE})   # .exe

#定义Build Type,这里删除Debug模式，增加了Bim模式
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel" CACHE STRING "" FORCE)
# 多配置生成器（VS）支持
foreach(OUTPUTCONFIG Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_DIR_BASE}/${OUTPUTCONFIG}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_DIR_BASE}/${OUTPUTCONFIG}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_DIR_BASE}/${OUTPUTCONFIG}")
endforeach()

#设置警告等级为3以及Unicode字符集
if(MSVC)
    add_compile_definitions(UNICODE _UNICODE)
    add_compile_options(/W3)
endif()

if (MSVC)   #Microsoft Visual C++（MSVC）设置
    # -------------------------------------------------------
    # 不同构建类型的优化等级
    # -------------------------------------------------------
    add_compile_options(
        $<$<CONFIG:Debug>:/Od>            # Debug: 禁用优化
        $<$<CONFIG:Release>:/O2>          # Release: 高度优化
        $<$<CONFIG:RelWithDebInfo>:/O2>   # RelWithDebInfo: 优化 + PDB
        $<$<CONFIG:MinSizeRel>:/O1>       # MinSizeRel: 尺寸优化
    )

    # -------------------------------------------------------
    # 统一 PDB 输出路径
    # -------------------------------------------------------
    # /Zi  →  生成 PDB  
    add_compile_options(
        $<$<CONFIG:Debug>:/Zi>
        $<$<CONFIG:RelWithDebInfo>:/Zi>
        $<$<CONFIG:Release>:/Zi>          # Release 也可以生成 PDB（可选）
        $<$<CONFIG:MinSizeRel>:/Zi>
    )

    # 链接器设置：为每个配置指定 PDB 输出位置
    set(CMAKE_LINKER_PDB_OUTPUT_DIRECTORY_DEBUG          "${OUTPUT_DIR_BASE}/Debug")
    set(CMAKE_LINKER_PDB_OUTPUT_DIRECTORY_RELEASE        "${OUTPUT_DIR_BASE}/Release")
    set(CMAKE_LINKER_PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIR_BASE}/RelWithDebInfo")
    set(CMAKE_LINKER_PDB_OUTPUT_DIRECTORY_MINSIZEREL     "${OUTPUT_DIR_BASE}/MinSizeRel")
endif()


add_subdirectory(QtWidgetsExt)
