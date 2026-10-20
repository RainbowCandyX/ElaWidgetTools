cmake_minimum_required(VERSION 3.10...3.30)

if (NOT DEFINED CMAKE_POLICY_VERSION_MINIMUM)
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
endif()

project(ElaFramework VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f00)

# 配置 QT 路径，三种方式任选其一（优先级从高到低）：
#   1) 命令行 -DQT_SDK_DIR=/path/to/Qt
#   2) 命令行 -DCMAKE_PREFIX_PATH=/path/to/Qt （CMake 标准方式，Qt Creator/CLion 默认）
#   3) 直接修改下方默认值
# 示例: D:/Qt/6.10.3/msvc2022_64
if (NOT CMAKE_PREFIX_PATH AND DEFINED ENV{CMAKE_PREFIX_PATH})
    file(TO_CMAKE_PATH "$ENV{CMAKE_PREFIX_PATH}" CMAKE_PREFIX_PATH)
endif()

if (QT_SDK_DIR)
    set(_qt_path_source "QT_SDK_DIR")
elseif (CMAKE_PREFIX_PATH)
    set(_qt_path_source "CMAKE_PREFIX_PATH")
    list(GET CMAKE_PREFIX_PATH 0 QT_SDK_DIR)
else()
    set(QT_SDK_DIR "D:/Qt/6.10.3/msvc2022_64")
    set(_qt_path_source "QT_SDK_DIR (默认值)")
endif()
# 统一转换为正斜杠路径。Windows 原生路径中的反斜杠若被拼进 install(CODE) 字符串,
# 生成的 cmake_install.cmake 会把 \a 之类的序列当作非法转义, 导致 install 阶段解析失败
# 这里不用 file(TO_CMAKE_PATH): 它在非 Windows 宿主上会把 : 当作路径列表分隔符
string(REPLACE "\\" "/" QT_SDK_DIR "${QT_SDK_DIR}")
message("当前 QT 路径来源: ${_qt_path_source}=${QT_SDK_DIR}")
unset(_qt_path_source)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/Install CACHE PATH "Installation path" FORCE)

set(BUILD_ELAPACKETIO OFF CACHE BOOL "Build ElaPacketIO" FORCE)
list(APPEND CMAKE_PREFIX_PATH ${QT_SDK_DIR})
list(REMOVE_DUPLICATES CMAKE_PREFIX_PATH)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if (QT_VERSION VERSION_GREATER_EQUAL 6.10)
    set(QT_NO_PRIVATE_MODULE_WARNING ON)
    find_package(Qt6 REQUIRED COMPONENTS WidgetsPrivate)
endif ()

if (${QT_VERSION} VERSION_GREATER_EQUAL 6.5.3 AND ${QT_VERSION} VERSION_LESS_EQUAL 6.6.1)
    message("你当前使用的QT版本为不推荐版本！可能出现使用时异常或崩溃问题，推荐QT版本为QT5.15.2 QT6.6.2 QT6.6.3！请知悉~")
    message("The QT version you are currently using is not recommended! It may cause exceptions or crashes during operation. The recommended QT versions are QT5.15.2, QT6.6.2, and QT6.6.3! Please be advised")
endif ()

if (NOT WIN32)
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_link_options(-Wl,--disable-new-dtags)
    endif()
    set(CMAKE_SKIP_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH "${QT_SDK_DIR}/lib:${CMAKE_INSTALL_PREFIX}/ElaWidgetTools/lib")
endif ()

add_subdirectory(ElaWidgetTools)
if (WIN32 AND BUILD_ELAPACKETIO)
    add_definitions(-DBUILD_WITH_ELAPACKETIO)
    add_subdirectory(ElaPacketIO)
endif ()
add_subdirectory(ElaWidgetToolsExample)

option(BUILD_PYTHON_BINDINGS "Build PySide6/Shiboken6 Python bindings" OFF)
if (BUILD_PYTHON_BINDINGS)
    add_subdirectory(bindings)
endif ()

if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ElaWidgetTools)
endif ()
