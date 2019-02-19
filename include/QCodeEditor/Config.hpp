/**
 * QCodeEditor - Widget to highlight and auto-complete code.
 * Copyright (C) 2016-2018 Nicolas Kogler
 *
 * QCodeEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with QCodeEditor. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef QCODEEDITOR_CONFIG_H
#define QCODEEDITOR_CONFIG_H

#include <qcompilerdetection.h>

#ifndef QCODEEDITOR_STATIC
    #ifdef QCODEEDITOR_BUILD
        #define QCODEEDITOR_API Q_DECL_EXPORT
    #else
        #define QCODEEDITOR_API Q_DECL_IMPORT
    #endif
#else
    #define QCODEEDITOR_API
#endif

/**
 * @def QCODEEDITOR_API
 * @brief Exports or imports dynamic symbols.
 */

#endif
