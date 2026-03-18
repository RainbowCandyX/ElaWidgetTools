cmake_minimum_required(VERSION 3.18)
project(ElaWidgetToolsPython LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(Shiboken6 REQUIRED)
find_package(PySide6 REQUIRED)

set(BINDINGS_LIBRARY_NAME "ElaWidgetTools")

set(WRAPPED_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/global.h)
set(TYPESYSTEM_FILE ${CMAKE_CURRENT_SOURCE_DIR}/bindings.xml)

get_target_property(PYSIDE6_INCLUDE_DIRS PySide6::pyside6 INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(SHIBOKEN6_INCLUDE_DIRS Shiboken6::libshiboken INTERFACE_INCLUDE_DIRECTORIES)

set(SHIBOKEN_OPTIONS
    --generator-set=shiboken
    --enable-parent-ctor-heuristic
    --enable-return-value-heuristic
    --use-isnull-as-nb-bool
    --avoid-protected-hack
    --enable-pyside-extensions
)

set(GENERATED_SOURCES_DIR ${CMAKE_CURRENT_BINARY_DIR}/ElaWidgetTools)

execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "
import shiboken6_generator
import os
print(os.path.join(os.path.dirname(shiboken6_generator.__file__), 'shiboken6'))
"
    OUTPUT_VARIABLE SHIBOKEN6_GENERATOR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT SHIBOKEN6_GENERATOR)
    find_program(SHIBOKEN6_GENERATOR shiboken6)
endif()

execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "
import PySide6
import os
print(os.path.join(os.path.dirname(PySide6.__file__), 'typesystems'))
"
    OUTPUT_VARIABLE PYSIDE6_TYPESYSTEMS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(SHIBOKEN_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/ElaWidgetTools
    ${PYSIDE6_INCLUDE_DIRS}
    ${PYSIDE6_INCLUDE_DIRS}/QtCore
    ${PYSIDE6_INCLUDE_DIRS}/QtGui
    ${PYSIDE6_INCLUDE_DIRS}/QtWidgets
)

string(REPLACE ";" ${PATH_SEP} SHIBOKEN_INCLUDE_DIRS_STR "${SHIBOKEN_INCLUDE_DIRS}")

set(PATH_SEP ":")
if(WIN32)
    set(PATH_SEP ";")
endif()

list(JOIN SHIBOKEN_INCLUDE_DIRS "${PATH_SEP}" SHIBOKEN_INCLUDE_DIRS_STR)

add_custom_command(
    OUTPUT ${GENERATED_SOURCES_DIR}/elawidgettools_module_wrapper.cpp
    COMMAND ${SHIBOKEN6_GENERATOR}
        ${SHIBOKEN_OPTIONS}
        --typesystem-paths=${PYSIDE6_TYPESYSTEMS}
        --include-paths=${SHIBOKEN_INCLUDE_DIRS_STR}
        --output-directory=${CMAKE_CURRENT_BINARY_DIR}
        ${WRAPPED_HEADER}
        ${TYPESYSTEM_FILE}
    DEPENDS ${WRAPPED_HEADER} ${TYPESYSTEM_FILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running Shiboken6 generator..."
)

file(GLOB_RECURSE GENERATED_SOURCES ${GENERATED_SOURCES_DIR}/*.cpp)
if(NOT GENERATED_SOURCES)
    set(GENERATED_SOURCES ${GENERATED_SOURCES_DIR}/elawidgettools_module_wrapper.cpp)
endif()

Python3_add_library(${BINDINGS_LIBRARY_NAME} MODULE ${GENERATED_SOURCES})

target_include_directories(${BINDINGS_LIBRARY_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/ElaWidgetTools
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GENERATED_SOURCES_DIR}
    ${Python3_INCLUDE_DIRS}
    ${SHIBOKEN6_INCLUDE_DIRS}
    ${PYSIDE6_INCLUDE_DIRS}
    ${PYSIDE6_INCLUDE_DIRS}/QtCore
    ${PYSIDE6_INCLUDE_DIRS}/QtGui
    ${PYSIDE6_INCLUDE_DIRS}/QtWidgets
)

target_link_libraries(${BINDINGS_LIBRARY_NAME} PRIVATE
    ElaWidgetTools
    Qt6::Widgets
    Shiboken6::libshiboken
    PySide6::pyside6
)

set_target_properties(${BINDINGS_LIBRARY_NAME} PROPERTIES
    PREFIX ""
    SUFFIX "${Python3_SOABI}.so"
    OUTPUT_NAME "ElaWidgetTools"
)

install(
    TARGETS ${BINDINGS_LIBRARY_NAME}
    LIBRARY DESTINATION .
)
