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

set(SOURCES
    QCodeEditor.cpp
    QCodeEditorDesign.cpp
    QCodeEditorHighlighter.cpp
    QCodeEditorLineWidget.cpp
    QCodeEditorPopup.cpp
    QCodeEditorStyleSheets.cpp
    QCodeEditorTextFinder.cpp
    QSyntaxRule.cpp
    XmlHelper.cpp
)

set(HEADERS
    ${QCODEEDITOR_INCLUDE_ROOT}/QCodeEditor/Config.hpp
    ${QCODEEDITOR_INCLUDE_ROOT}/QCodeEditor/QCodeEditor.hpp
    ${QCODEEDITOR_INCLUDE_ROOT}/QCodeEditor/QCodeEditorDesign.hpp
    ${QCODEEDITOR_INCLUDE_ROOT}/QCodeEditor/QCodeEditorHighlighter.hpp
    ${QCODEEDITOR_INCLUDE_ROOT}/QCodeEditor/QCodeEditorLineWidget.hpp
    ${QCODEEDITOR_INCLUDE_ROOT}/QCodeEditor/QCodeEditorPopup.hpp
    ${QCODEEDITOR_INCLUDE_ROOT}/QCodeEditor/QCodeEditorTextFinder.hpp
    ${QCODEEDITOR_INCLUDE_ROOT}/QCodeEditor/QSyntaxRule.hpp
    QCodeEditorStyleSheets.hpp
    XmlHelper.hpp
)

qt5_add_resources(RESOURCES ${QCODEEDITOR_RESOURCE_ROOT}/resources.qrc)

add_library(${QCODEEDITOR_LIBRARY}
    ${QCODEEDITOR_LIBRARY_TYPE}
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

target_include_directories(${QCODEEDITOR_LIBRARY}
    PUBLIC
        $<BUILD_INTERFACE:${QCODEEDITOR_INCLUDE_ROOT}>
        $<INSTALL_INTERFACE:../../include>
)

target_compile_features(${QCODEEDITOR_LIBRARY}
    PRIVATE
        ${QCODEEDITOR_COMPILE_FEATURES}
)

target_compile_definitions(${QCODEEDITOR_LIBRARY}
    PRIVATE
        QCODEEDITOR_BUILD
        ${QCODEEDITOR_COMPILE_DEFINITIONS}
)

target_link_libraries(${QCODEEDITOR_LIBRARY}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

install(
    TARGETS ${QCODEEDITOR_LIBRARY}
    EXPORT qcodeeditor-targets
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/QCodeEditor
    DESTINATION include
)

install(
    EXPORT qcodeeditor-targets
    FILE qcodeeditor-targets.cmake
    DESTINATION lib/cmake/qcodeeditor
)
