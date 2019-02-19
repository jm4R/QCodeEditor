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

#ifndef QCODEEDITOR_QCODEEDITORPOPUP_H
#define QCODEEDITOR_QCODEEDITORPOPUP_H

#include <QListView>

#include <QCodeEditor/Config.hpp>

class QCodeEditor;

/**
 * @author Nicolas Kogler
 * @date October 6th, 2016
 * @class QCodeEditorPopup
 */
class QCODEEDITOR_API QCodeEditorPopup : public QListView
{
public:
    QCodeEditorPopup(QCodeEditor* parent);
    ~QCodeEditorPopup() = default;

protected:
    bool viewportEvent(QEvent*) Q_DECL_OVERRIDE;
    bool event(QEvent*) Q_DECL_OVERRIDE;

private:
    QString _styleSheetNormal;
    QString _styleSheetPress;
};

#endif
