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

#include <QApplication>
#include <QMainWindow>
#include <QGridLayout>

#include <QCodeEditor/QCodeEditor.hpp>
#include <QCodeEditor/QCodeEditorDesign.hpp>
#include <QCodeEditor/QSyntaxRule.hpp>


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QCodeEditor* codeEditor = new QCodeEditor();
    codeEditor->setDesign(QCodeEditorDesign(":/design.xml"));
    codeEditor->setRules(QSyntaxRules::loadFromFile(":/rules.xml", codeEditor->design()));

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->addWidget(codeEditor);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    QMainWindow window;
    window.setCentralWidget(new QWidget);
    window.centralWidget()->setLayout(gridLayout);
    window.show();

    return app.exec();
}
