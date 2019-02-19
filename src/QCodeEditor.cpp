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

#include <QAbstractItemView>
#include <QAbstractProxyModel>
#include <QCompleter>
#include <QScrollBar>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

#include <QCodeEditor/QCodeEditor.hpp>
#include <QCodeEditor/QCodeEditorPopup.hpp>
#include <QCodeEditor/QCodeEditorLineWidget.hpp>
#include <QCodeEditor/QCodeEditorHighlighter.hpp>
#include <QCodeEditor/QCodeEditorTextFinder.hpp>
#include "QCodeEditorStyleSheets.hpp"

QCodeEditor::QCodeEditor(QWidget* parent)
    : QPlainTextEdit(parent)
    , _popup(new QCodeEditorPopup(this))
    , _highlighter(new QCodeEditorHighlighter(this))
    , _sourceModel(new QStandardItemModel())
    , _ruleFilter(new QSortFilterProxyModel(this))
    , _autoComplete(new QCompleter(this))
    , _completionTrigger(3)
    , _textFinder(QCodeEditorTextFinder::makeDialog(this))
{
    QFont monospace("Monospace");
    monospace.setPointSize(10);
    monospace.setKerning(true);
    monospace.setStyleHint(QFont::TypeWriter);
    monospace.setStyleStrategy(QFont::PreferAntialias);

    _autoComplete->setWidget(this);
    _autoComplete->setCompletionMode(QCompleter::PopupCompletion);
    _autoComplete->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    _autoComplete->setPopup(_popup);
    _ruleFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    _ruleFilter->setDynamicSortFilter(false);
    _lineWidget = new QCodeEditorLineWidget(this);

    setFont(monospace);
    setAutoFillBackground(true);
    setFrameStyle(QFrame::NoFrame);
    updateLineColumn(0);

    connect(_autoComplete, SIGNAL(activated(QString)), this, SLOT(completeWord(QString)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(scrollLineColumn(QRect,int)));
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineColumn(int)));
    connect(this, SIGNAL(textChanged()), this, SLOT(textChanged()));
}

const QList<QSyntaxRule> &QCodeEditor::rules() const
{
    return _rules;
}

const QCodeEditorDesign &QCodeEditor::design() const
{
    return _design;
}

quint32 QCodeEditor::lineCount() const
{
    return static_cast<quint32>(document()->blockCount());
}

QString QCodeEditor::textAtLine(quint32 index) const
{
    return document()->findBlockByLineNumber(index).text();
}

QCodeEditorHighlighter *QCodeEditor::highlighter() const
{
    return _highlighter;
}

void QCodeEditor::setRules(const QList<QSyntaxRule>& rules)
{
    _rules = rules;
    _highlighter->updateFormats();
    _highlighter->rehighlight();
}

void QCodeEditor::setDesign(const QCodeEditorDesign& design)
{
    _design = design;
    setFont(design.editorFont());

    QPalette palette;
    palette.setColor(QPalette::Base, design.editorBackColor());
    palette.setColor(QPalette::Text, design.editorTextColor());
    setPalette(palette);

    setStyleSheet(QCodeEditorStyleSheets::border(design));
    _highlighter->updateFormats();
}

void QCodeEditor::setCompletionTrigger(qint32 amount)
{
    _completionTrigger = amount;
}

void QCodeEditor::setKeywords(const QStringList& keywords)
{
    qint32 size = keywords.size();

    for (int i = 0; i < size; i++) {
        auto item = new QStandardItem;
        item->setText(keywords.at(i));
        _sourceModel->appendRow(item);
    }

    _ruleFilter->setSourceModel(_sourceModel);
    _ruleFilter->sort(0);
    _autoComplete->setModel(_ruleFilter);
}

void QCodeEditor::setKeywordModel(QStandardItemModel* model)
{
    _ruleFilter->setSourceModel(model);
    _autoComplete->setModel(_ruleFilter);
}

void QCodeEditor::addKeyword(const QString& keyword)
{
    if (!keywordExists(keyword)) {
        auto item = new QStandardItem;
        item->setText(keyword);
        _sourceModel->appendRow(item);
        _ruleFilter->sort(0);
    }
}

void QCodeEditor::removeKeyword(const QString& keyword)
{
    auto keywords = _sourceModel->findItems(keyword);
    if (keywords.size() == 1) {
        _sourceModel->removeRow(keywords.at(0)->row());
    }
}

bool QCodeEditor::keywordExists(const QString &keyword)
{
    return !_sourceModel->findItems(keyword).isEmpty();
}


void QCodeEditor::rehighlight()
{
    _highlighter->rehighlight();
}

int QCodeEditor::lineColumnWidth() const
{
    auto num = QString::number(document()->blockCount());
    auto pad = _design.lineColumnPadding();
    return fontMetrics().width(num) + pad.left() + pad.right();
}

void QCodeEditor::showTextFinder()
{
    _textFinder->show();
}

void QCodeEditor::paintEvent(QPaintEvent* event)
{
    QPlainTextEdit::paintEvent(event);
}

void QCodeEditor::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Tab) {
        return;
    }

    QPlainTextEdit::keyReleaseEvent(event);
}

void QCodeEditor::keyPressEvent(QKeyEvent* event)
{
    static QChar par(0x2029); // Qt uses paragraph separators
    static QString tab = QString(" ").repeated(4);

    // the code editor should not receive keys
    // while the auto complete menu is open.
    if (_autoComplete->popup()->isVisible()) {
        switch (event->key()) {
            case Qt::Key_Tab:
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Backtab:
                event->ignore();
                return;
            default:
                break;
        }
    }


    // adds tabs in front of the selected block(s)
    if (event->key() == Qt::Key_Tab && !textCursor().selectedText().isEmpty()) {
        // retrieves the amount of lines within the selected text
        QTextCursor cursor = textCursor();
        QString selected = cursor.selectedText();
        qint32 amountOfLines = selected.count(par) + 1;

        // does not do anything if only one line is selected
        if (amountOfLines == 1) {
            return;
        }

        int linePos, lineCopy;
        cursor.setPosition(cursor.selectionStart());
        cursor.movePosition(QTextCursor::StartOfLine);
        linePos = lineCopy = cursor.position();
        cursor.beginEditBlock();

        for (int i = 0; i < amountOfLines; ++i) {
            cursor.setPosition(linePos);
            cursor.insertText(tab);
            cursor.movePosition(QTextCursor::Down);
            cursor.movePosition(QTextCursor::StartOfLine);
            linePos = cursor.position();
        }

        cursor.movePosition(QTextCursor::Down);
        cursor.movePosition(QTextCursor::EndOfLine);
        cursor.setPosition(lineCopy, QTextCursor::KeepAnchor);
        cursor.endEditBlock();
        setTextCursor(cursor);

        return;
    }

    // removes tabs in front of selected block(s)
    if (event->key() == Qt::Key_Backtab && !textCursor().selectedText().isEmpty()) {
        // retrieves the amount of lines within the selected text
        QTextCursor cursor = textCursor();
        QString selected = cursor.selectedText();
        qint32 amountOfLines = selected.count(par) + 1;

        // does not do anything if only one line is selected
        if (amountOfLines == 1) {
            return;
        }

        int start = 0, line, diff, copy;
        cursor.setPosition(cursor.selectionStart());
        cursor.movePosition(QTextCursor::StartOfLine);
        copy = cursor.position();

        if (selected.at(0).isSpace()) {
            cursor.movePosition(QTextCursor::NextWord);
            start = cursor.position();
        }

        cursor.clearSelection();
        cursor.beginEditBlock();

        for (int i = 0; i < amountOfLines; ++i) {
            cursor.setPosition(start);
            cursor.movePosition(QTextCursor::StartOfLine);
            line = cursor.position();
            cursor.setPosition(start);

            if (start == line) {
                continue;
            }

            diff = qMin(4, start - line);
            for (int i = 0; i < diff; ++i) {
                cursor.deletePreviousChar();
            }

            cursor.movePosition(QTextCursor::Down);
            cursor.movePosition(QTextCursor::StartOfLine);
            cursor.movePosition(QTextCursor::NextWord);
            cursor.movePosition(QTextCursor::StartOfWord);
            start = cursor.position();
        }

        // Selects all the text
        cursor.movePosition(QTextCursor::Down);
        cursor.movePosition(QTextCursor::EndOfLine);
        cursor.setPosition(copy, QTextCursor::KeepAnchor);
        cursor.endEditBlock();
        setTextCursor(cursor);

        return;
    }

    // replaces a tab with four whitespaces
    if (event->key() == Qt::Key_Tab) {
        QTextCursor cursor = textCursor();
        cursor.insertText(QString(" ").repeated(4));
        setTextCursor(cursor);
        return;
    }

    QPlainTextEdit::keyPressEvent(event);

    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::WordUnderCursor);

    // close intellisense box if the current character is a
    // whitespace and the backspace key was pressed.
    if (event->key() == Qt::Key_Backspace) {
        if (cursor.selectedText().trimmed().isEmpty()) {
            _autoComplete->popup()->hide();
            return;
        }
    }

    // does absolutely nothing if a single modifier key was pressed
    if ((event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier)) &&
            event->text().isEmpty()) {
        return;
    }

    // does absolutely nothing if:
    // the word is interrupted by an invalid character
    // the word is too short to trigger a completion
    static QString invalidChars("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=");
    if (event->text().isEmpty() ||
        cursor.selectedText().length() < _completionTrigger ||
        invalidChars.contains(event->text().right(1))) {

        // hides the completion menu
        _autoComplete->popup()->hide();
        return;
    }

    QString prefix = QString("^") + cursor.selectedText();
    _ruleFilter->setFilterRegExp(QRegExp(prefix, Qt::CaseInsensitive));
    _autoComplete->popup()->setCurrentIndex(_autoComplete->completionModel()->index(0, 0));

    if (_autoComplete->popup()->model()->rowCount() == 0) {
        _autoComplete->popup()->hide();
        return;
    }

    if (!_autoComplete->popup()->isVisible()) {
        QRect rect = cursorRect();
        rect.moveTo(rect.x() + lineColumnWidth() - fontMetrics().width(prefix), rect.y()+4);
        rect.setWidth(_autoComplete->popup()->sizeHintForColumn(0) +
            _autoComplete->popup()->verticalScrollBar()->sizeHint().width());
        _autoComplete->complete(rect);
    }
}

void QCodeEditor::resizeEvent(QResizeEvent* event)
{
    QPlainTextEdit::resizeEvent(event);

    if (_design.isLineColumnVisible()) {
        QRect content = contentsRect();
        setViewportMargins(lineColumnWidth(), 0, 0, 0);
        _lineWidget->setGeometry(
            content.left(),
            content.top(),
            lineColumnWidth(),
            content.height());
    } else {
        setViewportMargins(0, 0, 0, 0);
        _lineWidget->setGeometry(0, 0, 0, 0);
    }
}

void QCodeEditor::updateLineColumn(int lineCount)
{
    Q_UNUSED(lineCount);
    setViewportMargins(lineColumnWidth(), 0, 0, 0);
}

void QCodeEditor::scrollLineColumn(QRect view, int scroll)
{
    // scrolls the line widget to the current scrollbar value
    if (_design.isLineColumnVisible()) {
        if (scroll != 0) {
            _lineWidget->scroll(0, scroll);
        } else {
            _lineWidget->update(0, view.y(), _lineWidget->width(), view.height());
        }
        setViewportMargins(lineColumnWidth(), 0, 0, 0);
    } else {
        setViewportMargins(0, 0, 0, 0);
    }
}

void QCodeEditor::completeWord(const QString& word)
{
    QTextCursor caretPos = textCursor();

    // inserts the missing characters at the current pos
    caretPos.movePosition(QTextCursor::Left);
    caretPos.movePosition(QTextCursor::StartOfWord);
    caretPos.select(QTextCursor::WordUnderCursor);
    caretPos.removeSelectedText();
    caretPos.insertText(word);

    setTextCursor(caretPos);
}

void QCodeEditor::textChanged()
{
    emit lineChanged(textCursor().block());
}
