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

#include <QPainter>
#include <QTextBlock>

#include <QCodeEditor/QCodeEditor.hpp>
#include <QCodeEditor/QCodeEditorLineWidget.hpp>

QCodeEditorLineWidget::QCodeEditorLineWidget(QCodeEditor* parent) : QWidget(parent)
{
}

QSize QCodeEditorLineWidget::sizeHint() const
{
    auto parent = static_cast<QCodeEditor*>(parentWidget());
    if (parent->design().isLineColumnVisible()) {
        return QSize(parent->lineColumnWidth(), 0);
    } else {
        return QSize(0, 0); // "hide" line column
    }
}

void QCodeEditorLineWidget::paintEvent(QPaintEvent* event)
{
    auto parent = static_cast<QCodeEditor*>(parentWidget());
    if (!parent->design().isLineColumnVisible()) {
        return;
    }

    const auto& design = parent->design();
    const auto& content = event->rect();

    QPainter painter(this);
    painter.fillRect(content, design.lineColumnBackColor());
    painter.setPen(design.lineColumnSeparatorColor());
    painter.drawLine(content.width()-1, 0, content.width()-1, height()-1);
    painter.setPen(design.lineColumnTextColor());

    auto firstLine = parent->firstVisibleBlock();
    qint32 bnActive = parent->textCursor().blockNumber() + ((design.startsWithOne()) ? 1 : 0);
    qint32 blockNumber = firstLine.blockNumber() + ((design.startsWithOne()) ? 1 : 0);
    qint32 blockTop = static_cast<int>(parent->blockBoundingGeometry(firstLine).translated(parent->contentOffset()).top());
    qint32 blockBottom = static_cast<int>(parent->blockBoundingRect(firstLine).height()) + blockTop;

    // paints all visible line numbers
    while (firstLine.isValid() && blockTop <= content.bottom()) {
        if (firstLine.isVisible() && blockBottom >= content.top()) {
            if (blockNumber == bnActive) {
                painter.setPen(design.activeLineColor());
                painter.drawText(0, blockTop, width(), fontMetrics().height(), Qt::AlignCenter, QString::number(blockNumber));
                painter.setPen(design.lineColumnTextColor());
            } else {
                painter.drawText(0, blockTop, width(), fontMetrics().height(), Qt::AlignCenter, QString::number(blockNumber));
            }
        }

        // jumps to the next line
        blockNumber++;
        firstLine = firstLine.next();
        blockTop = blockBottom;
        blockBottom += static_cast<int>(parent->blockBoundingRect(firstLine).height());
    }
}
