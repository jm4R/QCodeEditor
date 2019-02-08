#include <KGL/Widgets/QCodeEditorTextFinder.hpp>

namespace kgl {

    ///
    ///  @fn        Constructor
    ///  @author    Marek Bula
    ///
QCodeEditorTextFinder::QCodeEditorTextFinder(QWidget* parent, QCodeEditor *codeEditor)
        : QWidget(parent),
          m_Editor(codeEditor)  {

        setMinimumWidth(400);

        m_ReplaceButton = new QPushButton("Replace", this);
        m_ReplaceAllButton = new QPushButton("Replace all", this);
        m_FindLabel = new QLabel("Find:", this);
        m_TextToFind = new QLineEdit(this);
        m_ReplaceLabel = new QLabel("Replace with:", this);
        m_TextToReplace = new QLineEdit(this);
        m_InfoLabel = new QLabel("0/0", this);
        m_FindNextButton = new QPushButton("Find next", this);
        m_FindPrevButton = new QPushButton("Find prev", this);

        m_DialogLayout = new QGridLayout(this);
        m_DialogLayout->addWidget(m_FindLabel,0,0);
        m_DialogLayout->addWidget(m_TextToFind,0,1);
        m_DialogLayout->addWidget(m_TextToReplace,1,1);
        m_DialogLayout->addWidget(m_FindPrevButton,0,2);
        m_DialogLayout->addWidget(m_FindNextButton,0,3);
        m_DialogLayout->addWidget(m_InfoLabel,0,4);
        m_DialogLayout->addWidget(m_ReplaceLabel,1,0);
        m_DialogLayout->addWidget(m_ReplaceButton,1,2);
        m_DialogLayout->addWidget(m_ReplaceAllButton,1,3);

        connect(m_TextToFind,SIGNAL(textChanged(const QString &)),this, SLOT(onTextToFindChanged(const QString &)));
        connect(m_FindNextButton,SIGNAL(pressed()),this,SLOT(findNext()));
        connect(m_FindPrevButton,SIGNAL(pressed()),this,SLOT(findPrev()));
        connect(m_TextToReplace,SIGNAL(textChanged(const QString &)),this, SLOT(onTextToReplaceChanged(const QString &)));
        connect(m_ReplaceButton,SIGNAL(pressed()),this,SLOT(replaceText()));
        connect(m_ReplaceAllButton,SIGNAL(pressed()),this,SLOT(replaceTextAll()));

        m_ReplaceButton->setEnabled(false);
        m_ReplaceAllButton->setEnabled(false);
        m_CurrentSelectionIdx = 0;
        m_CurrentSelectionRemoved = false;
        m_FoundCount = 0;

        setReplaceAsFocusProxy(false);
    }

    ///
    ///  @fn        Destructor
    ///  @author    Marek Bula
    ///
    QCodeEditorTextFinder::~QCodeEditorTextFinder() {
    }

    ///
    ///  @fn        makeDialog
    ///  @author    Mariusz Jaskolka
    ///
    QDialog *QCodeEditorTextFinder::makeDialog(QCodeEditor *codeEditor)
    {
        QDialog *dialog = new QDialog(codeEditor);
        dialog->setLayout(new QVBoxLayout());
        QCodeEditorTextFinder *finder = new QCodeEditorTextFinder(dialog, codeEditor);
        dialog->layout()->addWidget(finder);
        connect(dialog, SIGNAL(rejected()), finder, SLOT(clearSelections()));
        return dialog;
    }

    ///
    ///  @fn        makeDialog
    ///  @author    Mariusz Jaskolka
    ///
    void QCodeEditorTextFinder::setReplaceAsFocusProxy(bool value)
    {
        QWidget* focusProxyWidget = value ? m_TextToReplace : m_TextToFind;
        setFocusProxy(focusProxyWidget);
    }

    ///
    ///  @fn        onTextToFindChanged
    ///  @author    Marek Bula
    ///
    void QCodeEditorTextFinder::onTextToFindChanged(const QString& text) {
        clearSelections();
        findText(text);
        updateInfoLabel();
    }

    ///
    ///  @fn        findText
    ///  @author    Marek Bula
    ///
    void QCodeEditorTextFinder::findText(QString text) {
        QColor highlightColor = Qt::gray;

        int orgPos = m_Editor->textCursor().position();
        int pos = orgPos;

        bool wraped = false;
        bool end = false;

        m_FoundCount = 0;

        QString code = m_Editor->toPlainText();
        while (pos >=0 && !text.isEmpty() && !end) {
            pos = code.indexOf(text,pos); //-1 if not found
            if (wraped == true && (pos >= orgPos || pos < 0)){ //end of search
                end = true;
            } else if (pos >= 0) {                //if found
                highlightText(pos + text.count(),pos, highlightColor);
                pos = pos + text.count();
                m_FoundCount++;
            } else {  //end of sheet, set pos to beginning and search till orgPos
                wraped = true;
                pos = 0;
            }
        }

        if (m_FoundCount >0) {
            m_CurrentSelectionIdx = 0;
            updateCurrentSelection();
        }
    }

    ///
    ///  @fn        onTextToReplaceChanged
    ///  @author    Marek Bula
    ///
    void QCodeEditorTextFinder::onTextToReplaceChanged(const QString& text) {
        if (!text.isEmpty()) {
            m_ReplaceButton->setEnabled(true);
            m_ReplaceAllButton->setEnabled(true);
        } else {
            m_ReplaceButton->setEnabled(false);
            m_ReplaceAllButton->setEnabled(false);
        }

    }

    ///
    ///  @fn        replaceText
    ///  @author    Marek Bula
    ///
    void QCodeEditorTextFinder::replaceText(){
        if (m_TextToReplace->text() != m_TextToFind->text() && !m_Editor->textCursor().selectedText().isEmpty()) {
            m_Editor->textCursor().insertText(m_TextToReplace->text());
            m_Selections.removeAt(m_CurrentSelectionIdx);
            m_CurrentSelectionRemoved = true;
            m_FoundCount--;
            updateInfoLabel();
        }
    }

    ///
    ///  @fn        replaceTextAll
    ///  @author    Marek Bula
    ///
    void QCodeEditorTextFinder::replaceTextAll() {
        int val = m_Editor->verticalScrollBar()->value();
        if (m_TextToReplace->text() != m_TextToFind->text()) {
            for (int i = 0; i < m_FoundCount; i++) {
                m_Editor->textCursor().insertText(m_TextToReplace->text());
                findNext();
            }

            clearSelections();
            findText(m_TextToFind->text());
            updateInfoLabel();
        }
        m_Editor->verticalScrollBar()->setValue(val);
    }

    ///
    ///  @fn        highlightText
    ///  @author    Marek Bula
    ///
    void QCodeEditorTextFinder::highlightText(int begin, int end, QColor color) {
        QTextCursor cursor = m_Editor->textCursor();
        cursor.setPosition(begin);
        cursor.setPosition(end,QTextCursor::KeepAnchor);

        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(color);
        selection.cursor = cursor;

        m_Selections.append(selection);
    }

    ///
    ///  @fn        clearSelections
    ///  @author    Marek Bula
    ///
    void QCodeEditorTextFinder::clearSelections() {
        QTextCursor cursor = m_Editor->textCursor();
        cursor.clearSelection();
        m_Editor->setTextCursor(cursor);

        m_Selections.clear();
        m_Editor->setExtraSelections(m_Selections);
    }

    ///
    ///  @fn        updateCurrentSelection
    ///  @author    Marek Bula
    ///
    void QCodeEditorTextFinder::updateCurrentSelection() {
        if (!m_Selections.isEmpty()) {
            m_Editor->setTextCursor(m_Selections[m_CurrentSelectionIdx].cursor);
        }
        m_Editor->setExtraSelections(m_Selections);
    }

    ///
    ///  @fn        findNext
    ///  @author    Marek Bula
    ///
    void QCodeEditorTextFinder::findNext() {
        if (m_FoundCount > 0 && !m_CurrentSelectionRemoved) {
            m_CurrentSelectionIdx++;
            m_CurrentSelectionIdx = m_CurrentSelectionIdx % m_FoundCount;
        } else if(m_FoundCount > 0 && m_CurrentSelectionRemoved) {
            m_CurrentSelectionIdx = (m_CurrentSelectionIdx+m_FoundCount) % m_FoundCount;
            m_CurrentSelectionRemoved = false;
        }
        updateCurrentSelection();
        updateInfoLabel();
    }

    ///
    ///  @fn        findPrev
    ///  @author    Marek Bula
    ///
    void QCodeEditorTextFinder::findPrev() {
        if (m_FoundCount > 0 && !m_CurrentSelectionRemoved) {
            m_CurrentSelectionIdx--;
            m_CurrentSelectionIdx = (m_CurrentSelectionIdx+m_FoundCount) % m_FoundCount;
        } else if(m_FoundCount > 0 && m_CurrentSelectionRemoved) {
            m_CurrentSelectionIdx--;
            m_CurrentSelectionIdx = (m_CurrentSelectionIdx+m_FoundCount) % m_FoundCount;
            m_CurrentSelectionRemoved = false;
        }
        updateCurrentSelection();
        updateInfoLabel();
    }

    ///
    ///  @fn        updateInfoLabel
    ///  @author    Marek Bula
    ///
    void QCodeEditorTextFinder::updateInfoLabel() {
        if (m_FoundCount > 0){
            m_InfoLabel->setText((QString::number(m_CurrentSelectionIdx+1)) + "/" + (QString::number(m_FoundCount)));
        } else {
            m_InfoLabel->setText("0/0");
        }
    }

}
