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

#include <QFile>

#include <QCodeEditor/QCodeEditorDesign.hpp>
#include "QCodeEditorStyleSheets.hpp"


QString QCodeEditorStyleSheets::border(const QCodeEditorDesign &design) {
    QFile file(":/editor.css");
    file.open(QIODevice::ReadOnly);
    auto sheet = QString(file.readAll());

    const auto& border = design.editorBorder();
    sheet.replace("%t", QString::number(border.top()));
    sheet.replace("%r", QString::number(border.right()));
    sheet.replace("%b", QString::number(border.bottom()));
    sheet.replace("%l", QString::number(border.left()));
    sheet.replace("%c", QString::number(design.editorBorderColor().rgba(), 16));

    return sheet;
}

QString QCodeEditorPopupStyleSheets::hover(const QCodeEditorDesign &design) {
    QFile file(":/popup.css");
    file.open(QIODevice::ReadOnly);
    auto sheet = QString(file.readAll());

    sheet.replace("%border", QString::number(design.intelliBoxBorderColor().rgba(), 16));
    sheet.replace("%back", QString::number(design.intelliBoxBackColor().rgba(), 16));
    sheet.replace("%text", QString::number(design.intelliBoxTextColor().rgba(), 16));
    sheet.replace("%focus", design.hasFocusRect() ? "dotted" : "none");
    sheet.replace("%selbrd", QString::number(design.intelliBoxSelectionBorderColor().rgba(), 16));
    sheet.replace("%selback", QString::number(design.intelliBoxSelectionBackColor().rgba(), 16));
    sheet.replace("%t", QString::number(design.intelliBoxBorder().top()));
    sheet.replace("%r", QString::number(design.intelliBoxBorder().right()));
    sheet.replace("%b", QString::number(design.intelliBoxBorder().bottom()));
    sheet.replace("%l", QString::number(design.intelliBoxBorder().left()));

    return sheet;
}

QString QCodeEditorPopupStyleSheets::press(const QCodeEditorDesign &design) {
    QFile file(":/popup.css");
    file.open(QIODevice::ReadOnly);
    auto sheet = QString(file.readAll());

    sheet.replace("%border", QString::number(design.intelliBoxBorderColor().rgba(), 16));
    sheet.replace("%back", QString::number(design.intelliBoxBackColor().rgba(), 16));
    sheet.replace("%text", QString::number(design.intelliBoxTextColor().rgba(), 16));
    sheet.replace("%focus", design.hasFocusRect() ? "dotted" : "none");
    sheet.replace("%selbrd", QString::number(design.intelliBoxPressBorderColor().rgba(), 16));
    sheet.replace("%selback", QString::number(design.intelliBoxPressBackColor().rgba(), 16));
    sheet.replace("%t", QString::number(design.intelliBoxBorder().top()));
    sheet.replace("%r", QString::number(design.intelliBoxBorder().right()));
    sheet.replace("%b", QString::number(design.intelliBoxBorder().bottom()));
    sheet.replace("%l", QString::number(design.intelliBoxBorder().left()));

    return sheet;
}
