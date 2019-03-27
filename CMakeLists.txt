#[[

  Lesser General Public License 3.0
  Copyright (c) 2016-2018 Nicolas Kogler

  QCodeEditor - Widget to highlight and auto-complete code.

  QCodeEditor is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with QCodeEditor. If not, see <http://www.gnu.org/licenses/>.

]]

cmake_minimum_required(VERSION 3.10)

project(QCodeEditor)

set(QCODEEDITOR_VERSION_MAJOR "1")
set(QCODEEDITOR_VERSION_MINOR "0")
set(QCODEEDITOR_VERSION_PATCH "0")
set(QCODEEDITOR_VERSION "${QCODEEDITOR_VERSION_MAJOR}.${QCODEEDITOR_VERSION_MINOR}")
set(QCODEEDITOR_VERSION_FULL "${QCODEEDITOR_VERSION}.${QCODEEDITOR_VERSION_PATCH}")

option(QCODEEDITOR_BUILD_SHARED "Build as shared library" ON)
option(QCODEEDITOR_BUILD_EXAMPLES "Build the examples" ON)
#option(QCODEEDITOR_BUILD_DOCS "Build the documentation" OFF)

if (QCODEEDITOR_BUILD_SHARED AND UNIX)
    # unix can make use of links for shared libraries.
    set(QCODEEDITOR_LIBRARY qcodeeditor)
else()
    # windows needs static library names. We use the major version as suffix
    # since we want to keep QCodeEditor binary compatible within each major version.
    set(QCODEEDITOR_LIBRARY qcodeeditor${QCODEEDITOR_VERSION_MAJOR})
endif()

set(QCODEEDITOR_INCLUDE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(QCODEEDITOR_RESOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/resources)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(QCODEEDITOR_COMPILE_FEATURES cxx_std_11 cxx_auto_type)

if (QCODEEDITOR_BUILD_SHARED)
    set(QCODEEDITOR_COMPILE_DEFINITIONS QCODEEDITOR_BUILD_SHARED)
    set(QCODEEDITOR_LIBRARY_TYPE SHARED)
else()
    set(QCODEEDITOR_COMPILE_DEFINITIONS QCODEEDITOR_BUILD_STATIC)
    set(QCODEEDITOR_LIBRARY_TYPE STATIC)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

if (QCODEEDITOR_BUILD_EXAMPLES)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
endif()

#if (QCODEEDITOR_BUILD_DOCS)
#    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/docs)
#endif()
