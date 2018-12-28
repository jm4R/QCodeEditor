#include <KGL/Widgets/QCodeEditorTextFinder.hpp>
#include <KGL/Design/QCodeEditorSheets.hpp>

#include <QScrollBar>

namespace kgl {

    ///
    ///  @fn        Constructor
    ///  @author    Marek Bula
    ///
    QCodeEditorTextFinder::QCodeEditorTextFinder(QCodeEditor *parent)
        : QWidget(parent),
          m_Parent(parent)  {

        QWidget::setVisible(false);

        m_FindDialog = new QDialog(parent);
        m_FindDialog->setWindowTitle("Find and replace");

        m_SpacerWidget = new QWidget(parent);
        parent->addScrollBarWidget(m_SpacerWidget, Qt::AlignBottom);

        m_ReplaceButton = new QPushButton("Replace");
        m_ReplaceAllButton = new QPushButton("Replace all");
        m_FindLabel = new QLabel("Find:");
        m_TextToFind = new QLineEdit();
        m_ReplaceLabel = new QLabel("Replace with:");
        m_TextToReplace = new QLineEdit();
        m_InfoLabel = new QLabel("1/3 ");
        m_FindNextButton = new QPushButton("Find next");
        m_FindPrevButton = new QPushButton("Find prev");

        m_DialogLayout = new QGridLayout;
        m_DialogLayout->addWidget(m_FindLabel,0,0);
        m_DialogLayout->addWidget(m_TextToFind,0,1);
        m_DialogLayout->addWidget(m_TextToReplace,1,1);
        m_DialogLayout->addWidget(m_FindPrevButton,0,2);
        m_DialogLayout->addWidget(m_FindNextButton,0,3);
        m_DialogLayout->addWidget(m_InfoLabel,0,4);
        m_DialogLayout->addWidget(m_ReplaceLabel,1,0);
        m_DialogLayout->addWidget(m_ReplaceButton,1,2);
        m_DialogLayout->addWidget(m_ReplaceAllButton,1,3);
        m_FindDialog->setLayout(m_DialogLayout);

        connect(m_TextToFind,SIGNAL(textChanged(const QString &)),this, SLOT(onTextToFindChanged(const QString &)));
        connect(m_FindNextButton,SIGNAL(pressed()),this,SLOT(findNext()));
        connect(m_FindPrevButton,SIGNAL(pressed()),this,SLOT(findPrev()));
        connect(m_FindDialog, SIGNAL(rejected()),this,SLOT(clearSelections()));
        connect(m_TextToReplace,SIGNAL(textChanged(const QString &)),this, SLOT(onTextToReplaceChanged(const QString &)));
        connect(m_ReplaceButton,SIGNAL(pressed()),this,SLOT(replaceText()));
        connect(m_ReplaceAllButton,SIGNAL(pressed()),this,SLOT(replaceTextAll()));

        m_ReplaceButton->setEnabled(false);
        m_ReplaceAllButton->setEnabled(false);
        m_CurrentSelectionIdx = 0;
        m_CurrentSelectionRemoved = false;
        m_FoundCount = 0;

        updateView();

        setEmbeded(true);
    }

    ///
    ///  @fn        Destructor
    ///  @author    Marek Bula
    ///
    QCodeEditorTextFinder::~QCodeEditorTextFinder() {
    }

    ///
    ///  @fn        setEmbeded
    ///  @author    Mariusz Jaskolka
    ///
    void QCodeEditorTextFinder::setEmbeded(bool value) {
        if(m_IsEmbeded == value)
            return;
        m_IsEmbeded = value;
        bool hidden = isHidden() && m_FindDialog->isHidden();

        if (value) {
            setLayout(m_FindDialog->layout());
        } else {
            m_FindDialog->setLayout(layout());
        }

        updateView();

        if (!hidden)
            show();
    }

    ///
    ///  @fn        isEmbeded
    ///  @author    Mariusz Jaskolka
    ///
    bool QCodeEditorTextFinder::isEmbeded()
    {
        return m_IsEmbeded;
    }

    ///
    ///  @fn        updateDesign
    ///  @author    Mariusz Jaskolka
    ///
    void QCodeEditorTextFinder::updateDesign()
    {
        QString sheet = QCodeEditorTextFinderSheets::sheet(m_Parent->design());
        //---
        setAutoFillBackground(true);
        m_FindDialog->resize(m_Parent->design().textFinderSize());
        //setAutoFillBackground(true);
        m_FindNextButton->setFlat(true);
        m_FindPrevButton->setFlat(true);
        m_ReplaceButton->setFlat(true);
        m_ReplaceAllButton->setFlat(true);
        QPalette palette;
        palette.setColor(QPalette::Base, m_Parent->design().textFinderBackColor());
        palette.setColor(QPalette::Window, m_Parent->design().textFinderBackColor());
        palette.setColor(QPalette::Button, m_Parent->design().textFinderBackColor());
        palette.setColor(QPalette::Text, m_Parent->design().editorTextColor());
        palette.setColor(QPalette::WindowText, m_Parent->design().editorTextColor());
        palette.setColor(QPalette::ButtonText, m_Parent->design().editorTextColor());
        palette.setColor(QPalette::BrightText, m_Parent->design().editorTextColor());
        //setPalette(palette);
        setStyleSheet(sheet);

        //m_FindDialog->setPalette(palette);
        //m_SpacerWidget->setPalette(palette);
        m_FindDialog->setStyleSheet(sheet);
        m_SpacerWidget->setStyleSheet(sheet);
        QWidget* currentWidget = m_IsEmbeded ? this : static_cast<QWidget*>(m_FindDialog);
        for (QWidget* w : currentWidget->findChildren<QWidget*>())
        {
            w->setStyleSheet(sheet);
            //w->setPalette(palette);
        }
        updateView();
    }

    ///
    ///  @fn        sizeHint
    ///  @author    Mariusz Jaskolka
    ///
    QSize QCodeEditorTextFinder::sizeHint() const {
        const QSize size = m_Parent->design().textFinderSize();
        if(!m_IsEmbeded)
            return size;
        else
            return QSize(m_Parent->contentsRect().width(), size.height());
    }

    void QCodeEditorTextFinder::setVisible(bool value)
    {
        if (m_IsEmbeded)
            QWidget::setVisible(value);
        else
            m_FindDialog->setVisible(value);

        updateView();
        m_TextToFind->setFocus();
        onTextToFindChanged(m_TextToFind->text());
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

        int orgPos = m_Parent->textCursor().position();
        int pos = orgPos;

        bool wraped = false;
        bool end = false;

        m_FoundCount = 0;

        QString code = m_Parent->toPlainText();
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
        if (m_TextToReplace->text() != m_TextToFind->text() && !m_Parent->textCursor().selectedText().isEmpty()) {
            m_Parent->textCursor().insertText(m_TextToReplace->text());
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
        int val = m_Parent->verticalScrollBar()->value();
        if (m_TextToReplace->text() != m_TextToFind->text()) {
            for (int i = 0; i < m_FoundCount; i++) {
                m_Parent->textCursor().insertText(m_TextToReplace->text());
                findNext();
            }

            clearSelections();
            findText(m_TextToFind->text());
            updateInfoLabel();
        }
        m_Parent->verticalScrollBar()->setValue(val);
    }

    ///
    ///  @fn        highlightText
    ///  @author    Marek Bula
    ///
    void QCodeEditorTextFinder::highlightText(int begin, int end, QColor color) {
        QTextCursor cursor = m_Parent->textCursor();
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
        QTextCursor cursor = m_Parent->textCursor();
        cursor.clearSelection();
        m_Parent->setTextCursor(cursor);

        m_Selections.clear();
        m_Parent->setExtraSelections(m_Selections);
    }

    ///
    ///  @fn        updateCurrentSelection
    ///  @author    Marek Bula
    ///
    void QCodeEditorTextFinder::updateCurrentSelection() {
        if (!m_Selections.isEmpty()) {
            m_Parent->setTextCursor(m_Selections[m_CurrentSelectionIdx].cursor);
        }
        m_Parent->setExtraSelections(m_Selections);
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

    void QCodeEditorTextFinder::updateView()
    {
        bool spacerVisible = m_IsEmbeded && isVisible();
        int h = spacerVisible * m_Parent->design().textFinderSize().height();
        m_SpacerWidget->setFixedHeight(h);
        m_SpacerWidget->setVisible(spacerVisible);
    }

}
