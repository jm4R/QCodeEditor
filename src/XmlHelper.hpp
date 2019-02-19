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

#ifndef QCODEEDITOR_XMLHELPER_H
#define QCODEEDITOR_XMLHELPER_H

#include <QColor>
#include <QFont>
#include <QStringList>
#include <QMargins>
#include <QSize>

class QXmlStreamReader;

/**
 * Provides several functions for parsing XML elements.
 * @author Nicolas Kogler
 * @date October 18th, 2016
 * @class XmlHelper
 */
class XmlHelper
{
public:
    /**
     * Reads an XML element and converts it to a boolean.
     * @param reader Current XML reader
     * @return True if element is string "true".
     */
    static bool readBool(QXmlStreamReader& reader);

    /**
     * Reads an XML element and converts it to a color.
     * @param reader Current XML reader
     * @return Color representing the inline text.
     *
     * Supported:
     * Hexadecimal notation (e.g. '#abcdef')
     * Color strings (e.g. 'red')
     * RGBA notation (e.g. 'rgba(r, g, b, a)
     */
    static QColor readColor(QXmlStreamReader& reader);

    /**
     * Reads an XML element and converts it to a list of strings.
     * @param reader Current XML reader.
     * @returns All words separated by whitespace.
     */
    static QStringList readKeywords(QXmlStreamReader& reader);

    /**
     * Reads a margin with 4 values.
     * @param reader Current XML reader.
     * @return A QMargins structure.
     */
    static QMargins readMargin(QXmlStreamReader& reader);

    /**
     * Reads a size with 2 values.
     * @param reader Current XML reader.
     * @return A QSize structure.
     */
    static QSize readSize(QXmlStreamReader& reader);

    /**
     * Reads a font structure from the XML reader.
     * @param reader Current XML reader.
     * @param defaultFont Default font, in case no family specified.
     * @return A QFont structure.
     */
    static QFont readFont(QXmlStreamReader& reader, const QFont& defaultFont);
};

#endif
