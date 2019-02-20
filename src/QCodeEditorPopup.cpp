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

#include <QEvent>

#include <QCodeEditor/QCodeEditor.hpp>
#include <QCodeEditor/QCodeEditorPopup.hpp>
#include "QCodeEditorStyleSheets.hpp"

QCodeEditorPopup::QCodeEditorPopup(QCodeEditor* parent) : QListView(parent)
{
    setMouseTracking(true);
    setFrameShape(QFrame::NoFrame);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

bool QCodeEditorPopup::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::MouseMove) {
        auto me = static_cast<QMouseEvent *>(event);
        auto index = indexAt(me->pos());

        // selects the item at the mouse cursor position
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);

    } else if (event->type() == QEvent::MouseButtonPress) {
        // this is a little hack: there is no 'pressed' pseudo class to style the
        // listview item on mouse-press, therefore we dynamically change the stylesheet.
        if (!selectedIndexes().isEmpty()) {
            setStyleSheet(_styleSheetPress);
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        setStyleSheet(_styleSheetNormal);
    }

    return QListView::viewportEvent(event);
}

bool QCodeEditorPopup::event(QEvent* event)
{
    if (event->type() == QEvent::ParentChange && parentWidget() != nullptr) {
        auto codeEditor = static_cast<QCodeEditor*>(parentWidget());
        if (codeEditor != nullptr) {
            _styleSheetNormal = QCodeEditorPopupStyleSheets::hover(codeEditor->design());
            _styleSheetPress  = QCodeEditorPopupStyleSheets::press(codeEditor->design());

            setFixedWidth(codeEditor->design().popupSize().width());
            setFixedHeight(codeEditor->design().popupSize().height());
            setStyleSheet(_styleSheetNormal);
            setFont(codeEditor->design().intelliBoxFont());
        }
    }

    return QListView::event(event);
}
