cmake_minimum_required(VERSION 3.14)
project(QtWidgetsExt LANGUAGES CXX)

# 生成 DLL
add_library(QtWidgetsExt SHARED)

# C++ 标准
target_compile_features(QtWidgetsExt PUBLIC cxx_std_17)

# 自动 MOC/UIC/RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Qt5 依赖
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# 源码
target_sources(QtWidgetsExt
    PRIVATE
        HwTableView.cpp
    PUBLIC
        HwTableView.h
)

# 包含目录（外部项目可以 include）
target_include_directories(QtWidgetsExt
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接 Qt 窗口库
target_link_libraries(QtWidgetsExt
    PUBLIC
        Qt5::Widgets
)

# 导出宏（DLL）
target_compile_definitions(QtWidgetsExt
    PRIVATE
        QT_WIDGETS_EXT_BUILD   # 构建 DLL 时使用
    PUBLIC
        QT_WIDGETS_EXT_DLL     # 供外部使用：import
)
