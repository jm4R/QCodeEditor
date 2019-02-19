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
#ifndef QCODEEDITOR_QCODEEDITORLINEWIDGET_H
#define QCODEEDITOR_QCODEEDITORLINEWIDGET_H

#include <QWidget>

#include <QCodeEditor/Config.hpp>

class QCodeEditor;

/**
 * Paints the line column and the line numbers.
 * @author Nicolas Kogler
 * @date October 6th, 2016
 * @class QCodeEditorLineWidget
 */
class QCODEEDITOR_API QCodeEditorLineWidget : public QWidget
{
public:
    QCodeEditorLineWidget(QCodeEditor* parent);
    ~QCodeEditorLineWidget() = default;

    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};


#endif
