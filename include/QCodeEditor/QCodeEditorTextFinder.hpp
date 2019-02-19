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
 * along with QCodeEditor.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __KGL_QCODEEDITORTEXTFINDER_HPP__
#define __KGL_QCODEEDITORTEXTFINDER_HPP__

#include <QCodeEditor/Config.hpp>
#include <QCodeEditor/QCodeEditor.hpp>
#include <QWidget>
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QObject>
#include <QList>
#include <QScrollBar>

///
///  @file      QCodeEditorTextFinder.hpp
///  @author    Marek Bula
///  @date      February 16th, 2086
///  @class     QCodeEditorTextFinder
///  @brief     Simple 'search and replace' dialog widget.
///
class QCODEEDITOR_API QCodeEditorTextFinder : public QWidget{
Q_OBJECT
public:

    ///
    ///  @fn    Constructor
    ///  @brief Initializes a new instance of QCodeEditorTextFinder.
    ///
    QCodeEditorTextFinder(QWidget *parent, QCodeEditor *codeEditor);

    ///
    ///  @fn    Destructor
    ///  @brief Frees all resources allocated by QCodeEditor.
    ///
    ~QCodeEditorTextFinder();

    ///
    ///  @fn      makeDialog : static
    ///  @brief   Make dialog that wraps search and replace widget.
    ///  @returns a pointer to a new dialog instance.
    ///
    static QDialog *makeDialog(QCodeEditor *codeEditor);

    ///
    ///  @fn      setReplaceAsFocusProxy
    ///  @brief   Allows choose if find or replace QLineEdit is this widgets focus proxy.
    ///  @param   `true` to make replace, `false` to make find as the widgets focus proxy.
    ///
    void setReplaceAsFocusProxy(bool value);

private slots:

    ///
    ///  @fn    onTextToFindChanged
    ///  @brief Clear current selections, find and select text and update info labels.
    ///
    void onTextToFindChanged(const QString& text);

    ///
    ///  @fn    onTextToReplaceChanged
    ///  @brief Activate/deactivate  'replace' buttons.
    ///
    void onTextToReplaceChanged(const QString& text);

    ///
    ///  @fn    findNext
    ///  @brief Go to next occurance of searched phrase.
    ///
    void findNext();

    ///
    ///  @fn    findPrev
    ///  @brief Go to previous occurance of searched phrase.
    ///
    void findPrev();

    ///
    ///  @fn    clearSelections
    ///  @brief Clear all selections of previously found phrase.
    ///
    void clearSelections();

    ///
    ///  @fn    replaceText
    ///  @brief Replace current selection with text from 'm_TextToReplace'.
    ///
    void replaceText();

    ///
    ///  @fn    replaceTextAll
    ///  @brief Replace all selections with text from 'm_TextToReplace'.
    ///
    void replaceTextAll();

private:

    ///
    ///  @fn    highlightText
    ///  @brief Select and highlight text with specified color and save its selection to 'm_Selections'.
    ///  @param begin index start position of highlighting text
    ///  @param end index finish position of highlighting text
    ///  @param color highlight color
    ///
    void highlightText(int begin, int end, QColor color);

    ///
    ///  @fn    findText
    ///  @brief Find specified text, select it and save its selection to 'm_Selections'.
    ///  @param text phrase to find in editor text
    ///
    void findText(QString text);

    ///
    ///  @fn    updateCurrentSelection
    ///  @brief Update current selection according to m_CurrentSelectionIdx.
    ///
    void updateCurrentSelection();

    ///
    ///  @fn    updateInfoLabel
    ///  @brief Show updated information about search results.
    ///
    void updateInfoLabel();

    QCodeEditor *m_Editor;
    QGridLayout *m_DialogLayout;
    QPushButton *m_ReplaceButton;
    QPushButton *m_ReplaceAllButton;
    QLabel *m_FindLabel;
    QLineEdit *m_TextToFind;
    QLabel *m_ReplaceLabel;
    QLineEdit *m_TextToReplace;
    QLabel *m_InfoLabel;
    QPushButton *m_FindNextButton;
    QPushButton *m_FindPrevButton;
    QList<QTextEdit::ExtraSelection> m_Selections;  ///< holds selections for all occurences of searched phrase
    int m_CurrentSelectionIdx;                      ///< index of selection that is currently selected
    bool m_CurrentSelectionRemoved;                 ///< indicates whether current selection has been removed
    int m_FoundCount;                               ///< number of occurences of searched phrase

};

#endif
