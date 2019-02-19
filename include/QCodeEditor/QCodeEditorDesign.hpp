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
#ifndef QCODEEDITOR_QCODEEDITORDESIGN_H
#define QCODEEDITOR_QCODEEDITORDESIGN_H

#include <QColor>
#include <QFont>
#include <QMargins>
#include <QSize>

#include <QCodeEditor/Config.hpp>

/**
 * @author Nicolas Kogler
 * @date October 4th, 2016
 * @class QCodeEditorDesign
 * @brief Defines the visual appearance of the code editor.
 */
class QCODEEDITOR_API QCodeEditorDesign
{
public:
    QCodeEditorDesign();
    ~QCodeEditorDesign() = default;
    QCodeEditorDesign(const QCodeEditorDesign& design) = default;
    QCodeEditorDesign& operator=(const QCodeEditorDesign& design) = default;

    /**
     * Loads a code editor design from a file.
     * @param path Path to the XML design file
     */
    QCodeEditorDesign(const QString& path);

    const QColor& editorBackColor() const;
    const QColor& editorTextColor() const;
    const QColor& editorBorderColor() const;
    const QColor& lineColumnBackColor() const;
    const QColor& lineColumnSeparatorColor() const;
    const QColor& lineColumnTextColor() const;
    const QColor& activeLineColor() const;
    const QColor& intelliBoxBackColor() const;
    const QColor& intelliBoxTextColor() const;
    const QColor& intelliBoxBorderColor() const;
    const QColor& intelliBoxSelectionBackColor() const;
    const QColor& intelliBoxSelectionBorderColor() const;
    const QColor& intelliBoxPressBackColor() const;
    const QColor& intelliBoxPressBorderColor() const;
    const QFont& editorFont() const;
    const QFont& intelliBoxFont() const;
    const QMargins& editorBorder() const;
    const QMargins& intelliBoxBorder() const;
    const QMargins& lineColumnPadding() const;
    const QSize& popupSize() const;
    bool isLineColumnVisible() const;
    bool hasFocusRect() const;
    bool startsWithOne() const;
    void setEditorBackColor(const QColor& color);
    void setEditorTextColor(const QColor& color);
    void setEditorBorderColor(const QColor& color);
    void setLineColumnBackColor(const QColor& color);
    void setLineColumnSeparatorColor(const QColor& color);
    void setLineColumnTextColor(const QColor& color);
    void setActiveLineColor(const QColor& color);
    void setIntelliBoxBackColor(const QColor& color);
    void setIntelliBoxTextColor(const QColor& color);
    void setIntelliBoxBorderColor(const QColor& color);
    void setIntelliBoxSelectionBackColor(const QColor &color);
    void setIntelliBoxSelectionBorderColor(const QColor& color);
    void setIntelliBoxPressBackColor(const QColor& color);
    void setIntelliBoxPressBorderColor(const QColor& color);
    void setEditorFont(const QFont& font);
    void setIntelliBoxFont(const QFont& font);
    void setEditorBorder(const QMargins& border);
    void setIntelliBoxBorder(const QMargins& border);
    void setLineColumnPadding(const QMargins& padding);
    void setPopupSize(const QSize& size);
    void setLineColumnVisible(bool visible);
    void showFocusRect(bool show);
    void setFirstLineAsOne(bool one);

private:
    QColor _editorBackColor;
    QColor _editorTextColor;
    QColor _editorBorderColor;
    QColor _lineColumnBackColor;
    QColor _lineColumnTextColor;
    QColor _lineColumnSeparatorColor;
    QColor _activeLineColor;
    QColor _intelliBoxBackColor;
    QColor _intelliBoxTextColor;
    QColor _intelliBoxBorderColor;
    QColor _intelliBoxSelectionBackColor;
    QColor _intelliBoxSelectionBorderColor;
    QColor _intelliBoxPressBackColor;
    QColor _intelliBoxPressBorderColor;
    QFont _editorFont;
    QFont _intelliBoxFont;
    QMargins _editorBorder;
    QMargins _intelliBoxBorder;
    QMargins _lineColumnPadding;
    QSize _popupSize;
    bool _hasLineColumn;
    bool _showFocusRect;
    bool _firstLineOne;
};

#endif
