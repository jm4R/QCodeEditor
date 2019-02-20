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

qt5_add_resources(RESOURCES resources.qrc)
add_executable(SimpleCodeEditor main.cpp ${RESOURCES})
target_link_libraries(SimpleCodeEditor ${QCODEEDITOR_LIBRARY})
