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
#ifndef QCODEEDITOR_QCODEEDITORHIGHLIGHTER_H
#define QCODEEDITOR_QCODEEDITORHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextBlockUserData>
#include <QObject>
#include <QUuid>

#include <QCodeEditor/Config.hpp>
#include <QCodeEditor/QSyntaxRule.hpp>
#include <QCodeEditor/QCodeEditorDesign.hpp>

class QCodeEditor;
class QCodeEditorBlockData;

/**
 * Highlights keywords, functions and more.
 * @author Nicolas Kogler
 * @date October 5th, 2016
 * @class QCodeEditorHighlighter
 */
class QCODEEDITOR_API QCodeEditorHighlighter : public QSyntaxHighlighter
{
public:
    QCodeEditorHighlighter(QCodeEditor* parent);
    ~QCodeEditorHighlighter() = default;

    /**
     * @brief Updates the text char formats.
     * Always call after setting new rules.
     */
    void updateFormats();

    /**
     * Applies some visual styling to text in the current line.
     * @param start Start index to apply styling to.
     * @param length Length of the text to style.
     * @param format Format to apply.
     */
    void highlight(int start, int length, const QTextCharFormat& format);

signals:

    /**
     * @brief Will fire if a match is found.
     * The signal will only fire if the rule containing the match has a valid
     * and unique string identifier (e.g <rule id="foo">)
     * @param rule Rule that emitted this signal.
     * @param sequence Matched sequence.
     * @param block Current QTextBlock.
     */
    void onMatch(const QSyntaxRule& rule, QString sequence, QTextBlock block);

    /**
     * @brief Will fire if a line was highlighted.
     * Gives the possibility to add own highlighting logic.
     * @param highlighter The highlighter instance.
     */
    void onHighlight(QCodeEditorHighlighter* highlighter);

    /**
     * @brief Will fire if a line's highlighting was removed.
     * @param data Data containing the UUID.
     */
    void onRemove(QCodeEditorBlockData* data);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    const QList<QSyntaxRule>* _rules;
    const QCodeEditorDesign* _design;
    const QCodeEditor* _parent;
    QList<QTextCharFormat> _formats;

    Q_OBJECT
};


/**
 * Holds an unique identifier.
 * @author Nicolas Kogler
 * @date October 29th, 2016
 * @class QCodeEditorBlockData
 */
class QCODEEDITOR_API QCodeEditorBlockData : public QTextBlockUserData
{
public:
    QCodeEditorBlockData(QString r);
    ~QCodeEditorBlockData();

    QUuid id;
    QString re;
};


#endif
