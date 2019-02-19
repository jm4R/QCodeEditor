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
#ifndef QCODEEDITOR_QCODEEDITOR_H
#define QCODEEDITOR_QCODEEDITOR_H

#include <QPlainTextEdit>
#include <QTextBlock>

#include <QCodeEditor/Config.hpp>
#include <QCodeEditor/QSyntaxRule.hpp>
#include <QCodeEditor/QCodeEditorDesign.hpp>

class QCompleter;
class QDialog;
class QSortFilterProxyModel;
class QStandardItemModel;

class QCodeEditorLineWidget;
class QCodeEditorHighlighter;
class QCodeEditorPopup;
class QCodeEditorTextFinder;

/**
 * Highlights and auto-completes code.
 * @author Nicolas Kogler
 * @date October 4th, 2016
 * @class QCodeEditor
 */
class QCODEEDITOR_API QCodeEditor : public QPlainTextEdit
{
public:
    QCodeEditor(QWidget* parent = nullptr);
    ~QCodeEditor() = default;

    /**
     * Retrieves all the syntax highlighting rules.
     * @return A list of syntax rules.
     */
    const QList<QSyntaxRule>& rules() const;

    /**
     * Retrieves all the visual properties.
     * @returns A structure containing all visual properties.
     */
    const QCodeEditorDesign& design() const;

    /**
     * Retrieves the amount of lines.
     * @return Amount of lines.
     */
    quint32 lineCount() const;

    /**
     * Retrieves the text on the specified line number.
     * @param index Line index starting from zero.
     * @return String value at line @p index.
     */
    QString textAtLine(quint32 index) const;

    /**
     * Retrieves the syntax highlighter for this editor.
     * @return The sytax highlighter.
     */
    QCodeEditorHighlighter* highlighter() const;

    /**
     * Specifies the syntax highlighting rules.
     * @param rules List of sntax rules.
     */
    void setRules(const QList<QSyntaxRule>& rules);

    /**
     * Specifies the visual properties.
     * @param design A structure containing visual properties.
     */
    void setDesign(const QCodeEditorDesign& design);

    /**
     * @brief Sets the completion trigger.
     * Specifies the amount of characters to be typed in the currently written
     * word until the auto-completion menu is triggered.
     * @param amount Amount of characters to be typed until triggering.
     */
    void setCompletionTrigger(qint32 amount);

    /**
     * @brief Specifies the keywords for auto-completion.
     * The keywords are sorted alphabetically. If you want to add icons or other
     * things to the keywords, you have to specify a custom model through
     * setKeywordModel().
     * @param keywords List of keywords.
     */
    void setKeywords(const QStringList& keywords);

    /**
     * Specifies the auto-complete model directly.
     * @param model QStandardItemModel to use.
     */
    void setKeywordModel(QStandardItemModel* model);

    /**
     * Adds a keyword to the existing model.
     * @param keyword New auto-complete keyword.
     */
    void addKeyword(const QString& keyword);

    /**
     * @brief Removes a keyword from the existing model.
     * @param keyword Keyword to remove from the list.
     */
    void removeKeyword(const QString& keyword);

    /**
     * @param keyword Keyword to check.
     * @return True if the keyword exists.
     */
    bool keywordExists(const QString& keyword);

    /**
     * @brief Applies syntax highlighting manually.
     * Normally one would not need this, because 'setRules'
     * already performs rehighlighting of the QCodeEditor.
     */
    void rehighlight();

    /**
     * Retrieves the width of the line column.
     * @return Width of the line column.
     */
    int lineColumnWidth() const;

    /**
     * @brief Shows find and replace dialog.
     */
    void showTextFinder();

protected:
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent*) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent*) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent*) Q_DECL_OVERRIDE;

signals:
    void lineChanged(QTextBlock block);

private slots:
    void updateLineColumn(int lineCount);
    void scrollLineColumn(QRect view, int scroll);
    void completeWord(const QString& word);
    void textChanged();

private:
    QList<QSyntaxRule> _rules;
    QCodeEditorDesign _design;
    QCodeEditorLineWidget* _lineWidget;
    QCodeEditorPopup* _popup;
    QCodeEditorHighlighter* _highlighter;
    QStandardItemModel* _sourceModel;
    QSortFilterProxyModel* _ruleFilter;
    QCompleter* _autoComplete;
    qint32 _completionTrigger;
    QDialog* _textFinder;

    // Allow the line widget to access vars/funcs while rendering
    friend class QCodeEditorLineWidget;

    Q_OBJECT
};

#endif
