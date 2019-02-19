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
#include <QRgb>
#include <QXmlStreamReader>

#include <QCodeEditor/QCodeEditorDesign.hpp>
#include "XmlHelper.hpp"

QCodeEditorDesign::QCodeEditorDesign()
    : _editorBackColor(0xffffffff)
    , _editorTextColor(0xff333333)
    , _editorBorderColor(0xffb9b9b9)
    , _lineColumnBackColor(0xffe9e9e9)
    , _lineColumnTextColor(0xff6a9fc6)
    , _lineColumnSeparatorColor(0xffb9b9b9)
    , _activeLineColor(0xfffc9100)
    , _intelliBoxBackColor(0xfffafafa)
    , _intelliBoxTextColor(0xff333333)
    , _intelliBoxBorderColor(0xffb9b9b9)
    , _intelliBoxSelectionBackColor(0xffc0dcf3)
    , _intelliBoxSelectionBorderColor(0xff90c8f6)
    , _intelliBoxPressBackColor(0xff90c8f6)
    , _intelliBoxPressBorderColor(0xff60b0f9)
    , _editorBorder(QMargins(0,0,0,0))
    , _intelliBoxBorder(QMargins(1,1,1,1))
    , _popupSize(200, 200)
    , _hasLineColumn(true)
    , _showFocusRect(false)
    , _firstLineOne(true)
{
    // Tries to find a monospace font
#ifndef Q_OS_WIN32
    _editorFont.setFamily("Monospace");
    _intelliBoxFont.setFamily("Monospace");
    _editorFont.setStyleHint(QFont::TypeWriter);
    _intelliBoxFont.setStyleHint(QFont::TypeWriter);
#else
    _editorFont.setFamily("Consolas");
    _intelliBoxFont.setFamily("Consolas");
#endif
    _editorFont.setPointSize(9);
    _intelliBoxFont.setPointSize(9);
}

QCodeEditorDesign::QCodeEditorDesign(const QString &path) : QCodeEditorDesign()
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug("kgl::QCodeEditorDesign: Cannot open XML file.");
        return;
    }

    QXmlStreamReader xmlReader(&file);
    if (!xmlReader.readNextStartElement() && xmlReader.name() != "design") {
        qDebug(QString("%0: top tag is not <design>.").arg(path).toStdString().c_str());
        return;
    }

    while (!xmlReader.hasError() && !xmlReader.atEnd()) {
        if (!xmlReader.readNextStartElement()) {
            continue;
        }

        QString name = xmlReader.name().toString().toLower();
        if (name == "editorbackcolor") {
            _editorBackColor = XmlHelper::readColor(xmlReader);
        } else if (name == "editortextcolor") {
            _editorTextColor = XmlHelper::readColor(xmlReader);
        } else if (name == "editorbordercolor") {
            _editorBorderColor = XmlHelper::readColor(xmlReader);
        } else if (name == "linecolumnbackcolor") {
            _lineColumnBackColor = XmlHelper::readColor(xmlReader);
        } else if (name == "linecolumntextcolor") {
            _lineColumnTextColor = XmlHelper::readColor(xmlReader);
        } else if (name == "linecolumnseparatorcolor") {
            _lineColumnSeparatorColor = XmlHelper::readColor(xmlReader);
        } else if (name == "activelinecolor") {
            _activeLineColor = XmlHelper::readColor(xmlReader);
        } else if (name == "intelliboxbackcolor") {
            _intelliBoxBackColor = XmlHelper::readColor(xmlReader);
        } else if (name == "intelliboxtextcolor") {
            _intelliBoxTextColor = XmlHelper::readColor(xmlReader);
        } else if (name == "intelliboxbordercolor") {
            _intelliBoxBorderColor = XmlHelper::readColor(xmlReader);
        } else if (name == "intelliboxselectionbackcolor") {
            _intelliBoxSelectionBackColor = XmlHelper::readColor(xmlReader);
        } else if (name == "intelliboxselectionbordercolor") {
            _intelliBoxSelectionBorderColor = XmlHelper::readColor(xmlReader);
        } else if (name == "intelliboxpressbackcolor") {
            _intelliBoxPressBackColor = XmlHelper::readColor(xmlReader);
        } else if (name == "intelliboxpressbordercolor") {
            _intelliBoxPressBorderColor = XmlHelper::readColor(xmlReader);
        } else if (name == "editorborder") {
            _editorBorder = XmlHelper::readMargin(xmlReader);
        } else if (name == "intelliboxborder") {
            _intelliBoxBorder = XmlHelper::readMargin(xmlReader);
        } else if (name == "linecolumnpadding") {
            auto size = XmlHelper::readSize(xmlReader);
            _lineColumnPadding = QMargins(size.width(), 0, size.height(), 0);
        } else if (name == "popupsize") {
            _popupSize = XmlHelper::readSize(xmlReader);
        } else if (name == "haslinecolumn") {
            _hasLineColumn = XmlHelper::readBool(xmlReader);
        } else if (name == "showfocusrect") {
            _showFocusRect = XmlHelper::readBool(xmlReader);
        } else if (name == "firstlineone") {
            _firstLineOne = XmlHelper::readBool(xmlReader);
        } else if (name == "editorfont") {
            _editorFont = XmlHelper::readFont(xmlReader, QFont("Monospace"));
        } else if (name == "intelliboxfont") {
            _intelliBoxFont = XmlHelper::readFont(xmlReader, QFont("Monospace"));
        } else {
            QString msg("%0: element '%1' is unknown");
            qDebug(msg.arg(path, name).toStdString().c_str());
        }
    }
}

const QColor& QCodeEditorDesign::editorBackColor() const
{
    return _editorBackColor;
}

const QColor& QCodeEditorDesign::editorTextColor() const
{
    return _editorTextColor;
}

const QColor &QCodeEditorDesign::editorBorderColor() const
{
    return _editorBorderColor;
}

const QColor& QCodeEditorDesign::lineColumnBackColor() const
{
    return _lineColumnBackColor;
}

const QColor& QCodeEditorDesign::lineColumnTextColor() const
{
    return _lineColumnTextColor;
}

const QColor& QCodeEditorDesign::lineColumnSeparatorColor() const
{
    return _lineColumnSeparatorColor;
}

const QColor& QCodeEditorDesign::activeLineColor() const
{
    return _activeLineColor;
}

const QColor& QCodeEditorDesign::intelliBoxBackColor() const
{
    return _intelliBoxBackColor;
}

const QColor& QCodeEditorDesign::intelliBoxTextColor() const
{
    return _intelliBoxTextColor;
}

const QColor& QCodeEditorDesign::intelliBoxBorderColor() const
{
    return _intelliBoxBorderColor;
}

const QColor& QCodeEditorDesign::intelliBoxSelectionBackColor() const
{
    return _intelliBoxSelectionBackColor;
}

const QColor& QCodeEditorDesign::intelliBoxSelectionBorderColor() const
{
    return _intelliBoxSelectionBorderColor;
}

const QColor& QCodeEditorDesign::intelliBoxPressBackColor() const
{
    return _intelliBoxPressBackColor;
}

const QColor& QCodeEditorDesign::intelliBoxPressBorderColor() const
{
    return _intelliBoxPressBorderColor;
}

const QFont& QCodeEditorDesign::editorFont() const
{
    return _editorFont;
}

const QFont& QCodeEditorDesign::intelliBoxFont() const
{
    return _intelliBoxFont;
}

const QMargins& QCodeEditorDesign::editorBorder() const
{
    return _editorBorder;
}

const QMargins& QCodeEditorDesign::intelliBoxBorder() const
{
    return _intelliBoxBorder;
}

const QMargins& QCodeEditorDesign::lineColumnPadding() const
{
    return _lineColumnPadding;
}

const QSize& QCodeEditorDesign::popupSize() const
{
    return _popupSize;
}

bool QCodeEditorDesign::isLineColumnVisible() const
{
    return _hasLineColumn;
}

bool QCodeEditorDesign::hasFocusRect() const
{
    return _showFocusRect;
}

bool QCodeEditorDesign::startsWithOne() const
{
    return _firstLineOne;
}


void QCodeEditorDesign::setEditorBackColor(const QColor& color)
{
    _editorBackColor = color;
}

void QCodeEditorDesign::setEditorTextColor(const QColor& color)
{
    _editorTextColor = color;
}

void QCodeEditorDesign::setEditorBorderColor(const QColor& color)
{
    _editorBorderColor = color;
}

void QCodeEditorDesign::setLineColumnBackColor(const QColor& color)
{
    _lineColumnBackColor = color;
}

void QCodeEditorDesign::setLineColumnTextColor(const QColor& color)
{
    _lineColumnTextColor = color;
}

void QCodeEditorDesign::setActiveLineColor(const QColor& color)
{
    _activeLineColor = color;
}

void QCodeEditorDesign::setLineColumnSeparatorColor(const QColor& color)
{
    _lineColumnSeparatorColor = color;
}

void QCodeEditorDesign::setIntelliBoxBackColor(const QColor& color)
{
    _intelliBoxBackColor = color;
}

void QCodeEditorDesign::setIntelliBoxTextColor(const QColor& color)
{
    _intelliBoxTextColor = color;
}

void QCodeEditorDesign::setIntelliBoxBorderColor(const QColor& color)
{
    _intelliBoxBorderColor = color;
}

void QCodeEditorDesign::setIntelliBoxSelectionBackColor(const QColor& color)
{
    _intelliBoxSelectionBackColor = color;
}

void QCodeEditorDesign::setIntelliBoxSelectionBorderColor(const QColor& color)
{
    _intelliBoxSelectionBorderColor = color;
}

void QCodeEditorDesign::setIntelliBoxPressBackColor(const QColor& color)
{
    _intelliBoxPressBackColor = color;
}

void QCodeEditorDesign::setIntelliBoxPressBorderColor(const QColor& color)
{
    _intelliBoxPressBorderColor = color;
}

void QCodeEditorDesign::setEditorFont(const QFont& font)
{
    _editorFont = font;
}

void QCodeEditorDesign::setIntelliBoxFont(const QFont& font)
{
    _intelliBoxFont = font;
}

void QCodeEditorDesign::setEditorBorder(const QMargins& border)
{
    _editorBorder = border;
}

void QCodeEditorDesign::setIntelliBoxBorder(const QMargins& border)
{
    _intelliBoxBorder = border;
}

void QCodeEditorDesign::setLineColumnPadding(const QMargins& padding)
{
    _lineColumnPadding = padding;
}

void QCodeEditorDesign::setLineColumnVisible(bool visible)
{
    _hasLineColumn = visible;
}

void QCodeEditorDesign::showFocusRect(bool show)
{
    _showFocusRect = show;
}

void QCodeEditorDesign::setFirstLineAsOne(bool one)
{
    _firstLineOne = one;
}
