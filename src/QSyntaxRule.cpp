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

#include <QFont>
#include <QtXml/QtXml>

#include <QCodeEditor/QSyntaxRule.hpp>
#include "XmlHelper.hpp"

QSyntaxRule::QSyntaxRule()
    : _regex("")
    , _backColor(Qt::transparent)
    , _foreColor(Qt::transparent)
    , _id("")
    , _startReg("")
    , _endReg("")
    , _isGlobal(true)
    , _useFont(false)
{
}

const QString& QSyntaxRule::regex() const
{
    return _regex;
}

const QFont& QSyntaxRule::font() const
{
    return _font;
}

const QColor &QSyntaxRule::backColor() const
{
    return _backColor;
}

const QColor& QSyntaxRule::foreColor() const
{
    return _foreColor;
}

const QString& QSyntaxRule::id() const
{
    return _id;
}

const QString& QSyntaxRule::openingRegex() const
{
    return _startReg;
}

const QString& QSyntaxRule::closingRegex() const
{
    return _endReg;
}

bool QSyntaxRule::isGlobal() const
{
    return _isGlobal;
}

bool QSyntaxRule::useFont() const
{
    return _useFont;
}

void QSyntaxRule::setRegex(const QString& regex)
{
    _regex = regex;
}

void QSyntaxRule::setKeywords(const QStringList& keywords, bool lineBegin)
{
    _regex.clear();

    // appends a regex that requires the given keywords either to be at the
    // start of the line or to be the first non-whitespace on the line.
    if (lineBegin) {
        _regex.append("(^|\\s.)(");
    } else {
        _regex.append("\\b(");
    }

    _regex.append(keywords.join('|'));
    _regex.append(")\\b");
}

void QSyntaxRule::setFont(const QFont& font)
{
    _font = font;
    _useFont = true;
}

void QSyntaxRule::setBackColor(const QColor& backColor)
{
    _backColor = backColor;
}

void QSyntaxRule::setForeColor(const QColor& foreColor)
{
    _foreColor = foreColor;
}

void QSyntaxRule::setId(const QString& id)
{
    _id = id;
}

void QSyntaxRule::setOpeningRegex(const QString& regex)
{
    _startReg = regex;
    _regex = regex;
}

void QSyntaxRule::setClosingRegex(const QString& regex)
{
    _endReg = regex;
}

void QSyntaxRule::setGlobal(bool global)
{
    _isGlobal = global;
}

QList<QSyntaxRule> QSyntaxRules::loadFromFile(const QString& path, const QCodeEditorDesign& design)
{
    QList<QSyntaxRule> rules;

    QFile xmlFile(path);
    if (!xmlFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open XML file " << path;
        return rules;
    }

    QXmlStreamReader xmlReader(&xmlFile);
    if (!xmlReader.readNextStartElement() || xmlReader.name() != "rules") {
        qDebug() << "There is no <rules> tag in " << path;
        return rules;
    }

    while (!xmlReader.hasError()) {
        if (!xmlReader.readNextStartElement()) {
            break; // no more elements
        } else if (xmlReader.name() != "rule") {
            break; // no more rule element
        } else if (xmlReader.isComment()) {
            continue; // comments are ignored
        }

        QSyntaxRule rule;
        QXmlStreamAttributes a = xmlReader.attributes();
        if (!a.isEmpty()) {
            auto first = a.at(0);
            if (first.name() == "id") {
                rule.setId(first.value().toString());
            } else if (first.name() == "global") {
                rule.setGlobal((first.value().toString() == "true") ? true : false);
            } if (a.size() > 1) {
                auto second = a.at(1);
                if (second.name() == "id") {
                    rule.setId(second.value().toString());
                } else if (second.name() == "global") {
                    rule.setGlobal((second.value().toString() == "true") ? true : false);
                }
            }
        }

        while (!(xmlReader.isEndElement() && xmlReader.name() == "rule")) {
            if (!xmlReader.readNextStartElement()) break;
            if (xmlReader.isComment()) break;

            auto name = xmlReader.name().toString().toLower();
            if (name == "regex") {
                rule.setRegex(xmlReader.readElementText());
            } else if (name == "startregex") {
                rule.setOpeningRegex(xmlReader.readElementText());
            } else if (name == "closeregex") {
                rule.setClosingRegex(xmlReader.readElementText());
            } else if (name == "keywords") {
                bool firstWord = false;
                auto attr = xmlReader.attributes();
                if (!attr.isEmpty() && attr.at(0).name() == "start") {
                    if (attr.at(0).value() == "true") {
                        firstWord = true;
                    }
                }

                rule.setKeywords(XmlHelper::readKeywords(xmlReader), firstWord);
            } else if (name == "backcolor") {
                rule.setBackColor(XmlHelper::readColor(xmlReader));
            } else if (name == "forecolor") {
                rule.setForeColor(XmlHelper::readColor(xmlReader));
            } else if (name == "font") {
                rule.setFont(XmlHelper::readFont(xmlReader, design.editorFont()));
            } else {
                QString msg("Element '%0' is unknown.");
                qDebug(msg.arg(name).toStdString().c_str());
            }
        }

        // determines if the required element (regex|keywords) was found
        if (rule.regex().isEmpty() && (rule.openingRegex().isEmpty() && rule.closingRegex().isEmpty())) {
            qDebug() << "Required element 'regex' or 'keywords' not found in " << path;
            return rules;
        } else {
            rules.push_back(rule);
        }
    }
    return rules;
}
