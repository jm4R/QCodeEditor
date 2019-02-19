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

#include <QList>
#include <QRegularExpression>
#include <QTextCharFormat>

#include <QCodeEditor/QCodeEditor.hpp>
#include <QCodeEditor/QCodeEditorHighlighter.hpp>
#include <QCodeEditor/QSyntaxRule.hpp>

QCodeEditorHighlighter::QCodeEditorHighlighter(QCodeEditor* parent)
    : QSyntaxHighlighter(parent->document())
    , _rules(&parent->rules())
    , _design(&parent->design())
    , _parent(parent)
{
}

void QCodeEditorHighlighter::updateFormats()
{
    _formats.clear();

    for (const QSyntaxRule &rule : _parent->rules()) {
        QTextCharFormat format;
        if (rule.useFont()) {
            format.setFont(rule.font());
        } else {
            format.setFont(_parent->font());
        } if (rule.foreColor().alpha() == 0) {
            format.setForeground(QBrush(_parent->design().editorTextColor()));
        } else {
            format.setForeground(rule.foreColor());
        }

        format.setBackground(QBrush(rule.backColor()));
        _formats.push_back(format);
    }
}

void QCodeEditorHighlighter::highlight(int start, int length, const QTextCharFormat &format)
{
    setFormat(start, length, format);
}

void QCodeEditorHighlighter::highlightBlock(const QString &text)
{
    if (currentBlockUserData() != nullptr) {
        auto d = static_cast<QCodeEditorBlockData*>(currentBlockUserData());
        QRegularExpression regex(d->re);
        if (!regex.match(text).hasMatch()) {
            onRemove(d);
        }
    }

    int ruleIndex = 0;
    setCurrentBlockState(-1);

    for (const auto& rule : *_rules) {
        const auto &format = _formats.at(ruleIndex);
        QRegularExpression regex(rule.regex());
        QRegularExpressionMatch match;

        if (rule.isGlobal()) {
            // searches for more than one match
            QRegularExpressionMatchIterator iter = regex.globalMatch(text);
            if (iter.hasNext() && !rule.closingRegex().isEmpty() && currentBlockState() == -1) {
                setCurrentBlockState(ruleIndex);
            }

            // iterates through all other matches and highlights them
            while (iter.hasNext()) {
                match = iter.next();
                setFormat(match.capturedStart(), match.capturedLength(), format);
                if (!rule.id().isEmpty()) {
                    if (currentBlockUserData() == nullptr) {
                        setCurrentBlockUserData(new QCodeEditorBlockData(rule.regex()));
                        emit onMatch(rule, match.captured(), currentBlock());
                    } else {
                        auto d = static_cast<QCodeEditorBlockData*>(currentBlockUserData());
                        if (d->re == rule.regex()) {
                            emit onRemove(d);
                            setCurrentBlockUserData(new QCodeEditorBlockData(rule.regex()));
                            emit onMatch(rule, match.captured(), currentBlock());
                        }
                    }
                }
            }
        } else {
            match = regex.match(text);
            if (match.hasMatch()) {
                if (!rule.closingRegex().isEmpty() && currentBlockState() == -1) {
                    // before setting the multiline trigger, checks if the
                    // closing sequence is on the same line.
                    QRegularExpression mulEnd(rule.closingRegex());
                    if (!mulEnd.match(text).hasMatch())
                        setCurrentBlockState(ruleIndex);
                } if (!rule.id().isEmpty()) {
                    if (currentBlockUserData() == nullptr) {
                        setCurrentBlockUserData(new QCodeEditorBlockData(rule.regex()));
                        emit onMatch(rule, match.captured(), currentBlock());
                    } else {
                        auto d = static_cast<QCodeEditorBlockData *>(currentBlockUserData());
                        if (d->re == rule.regex()) {
                            emit onRemove(d);
                            setCurrentBlockUserData(new QCodeEditorBlockData(rule.regex()));
                            emit onMatch(rule, match.captured(), currentBlock());
                        }
                    }
                }
                setFormat(match.capturedStart(), match.capturedLength(), format);
            }
        }
        ++ruleIndex;
    }

    // if there was a multi-line-rule match in the previous block, we need to
    // check against its closing regex.
    if (previousBlockState() != -1) {
        const auto& rule = _rules->at(previousBlockState());
        const auto format = _formats.at(previousBlockState());
        QRegularExpression regex(rule.closingRegex());
        QRegularExpressionMatch match = regex.match(text);

        // If now has a match, ends the multi-line regex
        if (match.hasMatch()) {
            setFormat(match.capturedStart(), match.capturedLength(), format);
            setCurrentBlockState(-1);

            if (!rule.id().isEmpty()) {
                if (currentBlockUserData() == nullptr) {
                    setCurrentBlockUserData(new QCodeEditorBlockData(rule.regex()));
                    emit onMatch(rule, match.captured(), currentBlock());
                } else {
                    auto d = static_cast<QCodeEditorBlockData*>(currentBlockUserData());
                    if (d->re == rule.regex()) {
                        // Already containing a match that has the exact regex:
                        // Remove it first, then add it again
                        emit onRemove(d);
                        setCurrentBlockUserData(new QCodeEditorBlockData(rule.regex()));
                        emit onMatch(rule, match.captured(), currentBlock());
                    }
                }
            }
        } else {
            // Has no match, highlights the entire line and
            // forwards the previous state to the next line.
            setFormat(0, text.length(), format);
            setCurrentBlockState(previousBlockState());
        }
    }


    // Provides custom highlighting logic
    emit onHighlight(this);
}

QList<QUuid> uniqueIds;

QCodeEditorBlockData::QCodeEditorBlockData(QString r)
{
    do {
        id = QUuid::createUuid();
    } while (uniqueIds.contains(id));

    uniqueIds.push_back(id);
    re = r;
}

QCodeEditorBlockData::~QCodeEditorBlockData()
{
    uniqueIds.removeOne(id);
}
