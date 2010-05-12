/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Graphics Dojo project on Qt Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#include "noteseditor.h"
#include "highlighter.h"
#include "src/core/dbghelper.h"
#include "src/ui/dialog/insertlinkdialog.h"
#include "ui_noteseditor.h"
#include "ui_inserthtmldialog.h"

#include <QtGui>
#include <QtWebKit>

#define FORWARD_ACTION(action1, action2) \
    connect(action1, SIGNAL(triggered()), \
            ui->webView->pageAction(action2), SLOT(trigger())); \
    connect(ui->webView->pageAction(action2), \
            SIGNAL(changed()), SLOT(adjustActions()));


NotesEditor::NotesEditor(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::NotesEditor)
        , sourceDirty(true)
        , highlighter(0)
        , ui_dialog(0)
        , insertHtmlDialog(0)
{
    DEBUG_FUNC_NAME
    ui->setupUi(this);
    ui->tabWidget->setTabText(0, tr("Normal View"));
    ui->tabWidget->setTabText(1, tr("HTML Source"));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(changeTab(int)));

    highlighter = new Highlighter(ui->plainTextEdit->document());
    m_simpleNotes = new SimpleNotes();

    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    ui->standardToolBar->insertWidget(ui->actionZoomOut, spacer);

    zoomLabel = new QLabel;
    ui->standardToolBar->insertWidget(ui->actionZoomOut, zoomLabel);

    zoomSlider = new QSlider(this);
    zoomSlider->setOrientation(Qt::Horizontal);
    zoomSlider->setMaximumWidth(150);
    zoomSlider->setRange(25, 400);
    zoomSlider->setSingleStep(25);
    zoomSlider->setPageStep(100);
    connect(zoomSlider, SIGNAL(valueChanged(int)), SLOT(changeZoom(int)));
    ui->standardToolBar->insertWidget(ui->actionZoomIn, zoomSlider);

    connect(ui->actionFileSave, SIGNAL(triggered()), SLOT(fileSave()));
    connect(ui->actionFileSaveAs, SIGNAL(triggered()), SLOT(fileSaveAs()));
    connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    connect(ui->actionInsertImage, SIGNAL(triggered()), SLOT(insertImage()));
    connect(ui->actionCreateLink, SIGNAL(triggered()), SLOT(createLink()));
    connect(ui->actionInsertHtml, SIGNAL(triggered()), SLOT(insertHtml()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), SLOT(zoomOut()));
    connect(ui->actionZoomIn, SIGNAL(triggered()), SLOT(zoomIn()));

    // these are forward to internal QWebView
    FORWARD_ACTION(ui->actionEditUndo, QWebPage::Undo);
    FORWARD_ACTION(ui->actionEditRedo, QWebPage::Redo);
    FORWARD_ACTION(ui->actionEditCut, QWebPage::Cut);
    FORWARD_ACTION(ui->actionEditCopy, QWebPage::Copy);
    FORWARD_ACTION(ui->actionEditPaste, QWebPage::Paste);
    FORWARD_ACTION(ui->actionFormatBold, QWebPage::ToggleBold);
    FORWARD_ACTION(ui->actionFormatItalic, QWebPage::ToggleItalic);
    FORWARD_ACTION(ui->actionFormatUnderline, QWebPage::ToggleUnderline);

    // Qt 4.5.0 has a bug: always returns 0 for QWebPage::SelectAll
    connect(ui->actionEditSelectAll, SIGNAL(triggered()), SLOT(editSelectAll()));

    connect(ui->actionStyleParagraph, SIGNAL(triggered()), SLOT(styleParagraph()));
    connect(ui->actionStyleHeading1, SIGNAL(triggered()), SLOT(styleHeading1()));
    connect(ui->actionStyleHeading2, SIGNAL(triggered()), SLOT(styleHeading2()));
    connect(ui->actionStyleHeading3, SIGNAL(triggered()), SLOT(styleHeading3()));
    connect(ui->actionStyleHeading4, SIGNAL(triggered()), SLOT(styleHeading4()));
    connect(ui->actionStyleHeading5, SIGNAL(triggered()), SLOT(styleHeading5()));
    connect(ui->actionStyleHeading6, SIGNAL(triggered()), SLOT(styleHeading6()));
    connect(ui->actionStylePreformatted, SIGNAL(triggered()), SLOT(stylePreformatted()));
    connect(ui->actionStyleAddress, SIGNAL(triggered()), SLOT(styleAddress()));
    connect(ui->actionFormatFontName, SIGNAL(triggered()), SLOT(formatFontName()));
    connect(ui->actionFormatFontSize, SIGNAL(triggered()), SLOT(formatFontSize()));
    connect(ui->actionFormatTextColor, SIGNAL(triggered()), SLOT(formatTextColor()));
    connect(ui->actionFormatBackgroundColor, SIGNAL(triggered()), SLOT(formatBackgroundColor()));

    // no page action exists yet for these, so use execCommand trick
    connect(ui->actionFormatStrikethrough, SIGNAL(triggered()), SLOT(formatStrikeThrough()));
    connect(ui->actionFormatAlignLeft, SIGNAL(triggered()), SLOT(formatAlignLeft()));
    connect(ui->actionFormatAlignCenter, SIGNAL(triggered()), SLOT(formatAlignCenter()));
    connect(ui->actionFormatAlignRight, SIGNAL(triggered()), SLOT(formatAlignRight()));
    connect(ui->actionFormatAlignJustify, SIGNAL(triggered()), SLOT(formatAlignJustify()));
    connect(ui->actionFormatDecreaseIndent, SIGNAL(triggered()), SLOT(formatDecreaseIndent()));
    connect(ui->actionFormatIncreaseIndent, SIGNAL(triggered()), SLOT(formatIncreaseIndent()));
    connect(ui->actionFormatNumberedList, SIGNAL(triggered()), SLOT(formatNumberedList()));
    connect(ui->actionFormatBulletedList, SIGNAL(triggered()), SLOT(formatBulletedList()));


    // necessary to sync our actions
    connect(ui->webView->page(), SIGNAL(selectionChanged()), SLOT(adjustActions()));


    connect(ui->webView->page(), SIGNAL(contentsChanged()), SLOT(adjustSource()));
    ui->webView->setFocus();

    fileNew();

    adjustActions();
    adjustSource();
    changeZoom(100);
}
void NotesEditor::init()
{
    DEBUG_FUNC_NAME
    m_simpleNotes->setModuleManager(m_moduleManager);
    m_simpleNotes->setNotes(m_notes);
    m_simpleNotes->setSettings(m_settings);
    m_simpleNotes->setBibleDisplay(m_bibleDisplay);

    //m_simpleNotes->setDataWidget(ui->textBrowser);
    m_simpleNotes->setFrameWidget(ui->webView->page()->mainFrame());
    m_simpleNotes->setViewWidget(ui->treeView);
    m_simpleNotes->setTitleWidget(ui->lineEdit_noteTitle);
    m_simpleNotes->setLinkButtonWidget(ui->pushButton_editNoteLink);
    m_simpleNotes->setLinkWidget(ui->label_noteLink);

    m_simpleNotes->init();
    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(ui->webView->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(pharseUrl(QUrl)));
    connect(this, SIGNAL(get(QUrl)), m_bibleDisplay, SIGNAL(get(QUrl)));

    /*connect(ui->toolButton_noteBold, SIGNAL(clicked()), this, SLOT(noteSetTextBold()));
    connect(ui->toolButton_noteItalic, SIGNAL(clicked()), this, SLOT(noteSetTextItalic()));
    connect(ui->toolButton_noteUnderline, SIGNAL(clicked()), this, SLOT(noteSetTextUnderline()));
    connect(ui->toolButton_noteFontColor, SIGNAL(clicked()), this, SLOT(noteSetTextColor()));
    connect(ui->toolButton_noteUndo, SIGNAL(clicked()), this, SLOT(noteUndo()));
    connect(ui->toolButton_noteRedo, SIGNAL(clicked()), this, SLOT(noteRedo()));
    connect(ui->toolButton_link, SIGNAL(clicked()), this, SLOT(addLink()));
    connect(ui->toolButton_removeLink, SIGNAL(clicked()), this, SLOT(removeLink()));
    connect(ui->textBrowser,SIGNAL(anchorClicked(QUrl)),m_bibleDisplay,SIGNAL(get(QUrl)));

    connect(ui->textBrowser, SIGNAL(undoAvailable(bool)), ui->toolButton_noteUndo, SLOT(setEnabled(bool)));
    connect(ui->textBrowser, SIGNAL(redoAvailable(bool)), ui->toolButton_noteRedo, SLOT(setEnabled(bool)));*/
    // connect(ui->textBrowser, SIGNAL(undoAvailable(bool)), m_simpleNotes, SLOT(fastSave()));
    //  connect(ui->textBrowser, SIGNAL(redoAvailable(bool)), m_simpleNotes, SLOT(fastSave()));

}

NotesEditor::~NotesEditor()
{
    delete ui;
    delete ui_dialog;
}

void NotesEditor::fileNew()
{
    ui->webView->setHtml("<p></p>");
    ui->webView->setFocus();
    ui->webView->page()->setContentEditable(true);

    // quirk in QWebView: need an initial mouse click to show the cursor
    int mx = ui->webView->width() / 2;
    int my = ui->webView->height() / 2;
    QPoint center = QPoint(mx, my);
    QMouseEvent *e1 = new QMouseEvent(QEvent::MouseButtonPress, center,
                                      Qt::LeftButton, Qt::LeftButton,
                                      Qt::NoModifier);
    QMouseEvent *e2 = new QMouseEvent(QEvent::MouseButtonRelease, center,
                                      Qt::LeftButton, Qt::LeftButton,
                                      Qt::NoModifier);
    QApplication::postEvent(ui->webView, e1);
    QApplication::postEvent(ui->webView, e2);
}
bool NotesEditor::fileSave()
{
    m_simpleNotes->saveNote();
    return true;
}

bool NotesEditor::fileSaveAs()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
                 QString(), tr("HTML-Files (*.htm *.html);;All Files (*)"));
    if (fn.isEmpty())
        return false;
    if (!(fn.endsWith(".htm", Qt::CaseInsensitive) || fn.endsWith(".html", Qt::CaseInsensitive)))
        fn += ".htm"; // default

    return fileSave();
}

void NotesEditor::insertImage()
{
    QString filters;
    filters += tr("Common Graphics (*.png *.jpg *.jpeg *.gif);;");
    filters += tr("Portable Network Graphics (PNG) (*.png);;");
    filters += tr("JPEG (*.jpg *.jpeg);;");
    filters += tr("Graphics Interchange Format (*.gif);;");
    filters += tr("All Files (*)");

    QString fn = QFileDialog::getOpenFileName(this, tr("Open image..."),
                 QString(), filters);
    if (fn.isEmpty())
        return;
    if (!QFile::exists(fn))
        return;

    QUrl url = QUrl::fromLocalFile(fn);
    execCommand("insertImage", url.toString());
}

// shamelessly copied from Qt Demo Browser
static QUrl guessUrlFromString(const QString &string)
{
    QString urlStr = string.trimmed();
    QRegExp test(QLatin1String("^[a-zA-Z]+\\:.*"));

    // Check if it looks like a qualified URL. Try parsing it and see.
    bool hasSchema = test.exactMatch(urlStr);
    if (hasSchema) {
        QUrl url(urlStr, QUrl::TolerantMode);
        if (url.isValid())
            return url;
    }

    // Might be a file.
    if (QFile::exists(urlStr))
        return QUrl::fromLocalFile(urlStr);

    // Might be a shorturl - try to detect the schema.
    if (!hasSchema) {
        int dotIndex = urlStr.indexOf(QLatin1Char('.'));
        if (dotIndex != -1) {
            QString prefix = urlStr.left(dotIndex).toLower();
            QString schema = (prefix == QLatin1String("ftp")) ? prefix : QLatin1String("http");
            QUrl url(schema + QLatin1String("://") + urlStr, QUrl::TolerantMode);
            if (url.isValid())
                return url;
        }
    }

    // Fall back to QUrl's own tolerant parser.
    return QUrl(string, QUrl::TolerantMode);
}

void NotesEditor::createLink()
{
    InsertLinkDialog *insertLinkDialog = new  InsertLinkDialog(this);
    connect(insertLinkDialog, SIGNAL(newLink(QString)), this, SLOT(createLink(QString)));
    insertLinkDialog->setSettings(m_settings);
    insertLinkDialog->setNotes(m_notes);
    insertLinkDialog->setModuleManager(m_moduleManager);
    insertLinkDialog->init();
    insertLinkDialog->setCurrent(m_moduleManager->m_bible.bibleID(), m_moduleManager->m_bible.m_biblePath,
                                 m_moduleManager->m_bible.bookID(), m_moduleManager->m_bible.chapterID() + 1, 1);
    insertLinkDialog->show();
    insertLinkDialog->exec();
}
void NotesEditor::createLink(QString link)
{
    if (link.startsWith("bible://") || link.startsWith("persistent://") || link.startsWith("note://")) {
        execCommand("createLink", link);
    } else {
        QUrl url = guessUrlFromString(link);
        if (url.isValid())
            execCommand("createLink", url.toString());
    }
}
void NotesEditor::pharseUrl(QUrl url)
{
    DEBUG_FUNC_NAME

    QString link = url.toString();
    myDebug() << "link = " << link;
    const QString note = "note://";
    if (link.startsWith(note)) {
        link = link.remove(0, note.size());
        m_simpleNotes->showNote(link, true);
    } else {
        emit get(url);
    }
}

void NotesEditor::insertHtml()
{
    if (!insertHtmlDialog) {
        insertHtmlDialog = new QDialog(this);
        if (!ui_dialog)
            ui_dialog = new Ui::InsertHtmlDialog;
        ui_dialog->setupUi(insertHtmlDialog);
        connect(ui_dialog->buttonBox, SIGNAL(accepted()),
                insertHtmlDialog, SLOT(accept()));
        connect(ui_dialog->buttonBox, SIGNAL(rejected()),
                insertHtmlDialog, SLOT(reject()));
    }

    ui_dialog->plainTextEdit->clear();
    ui_dialog->plainTextEdit->setFocus();
    Highlighter *hilite = new Highlighter(ui_dialog->plainTextEdit->document());

    if (insertHtmlDialog->exec() == QDialog::Accepted)
        execCommand("insertHTML", ui_dialog->plainTextEdit->toPlainText());

    delete hilite;
}

void NotesEditor::zoomOut()
{
    int percent = static_cast<int>(ui->webView->zoomFactor() * 100);
    if (percent > 25) {
        percent -= 25;
        percent = 25 * (int((percent + 25 - 1) / 25));
        qreal factor = static_cast<qreal>(percent) / 100;
        ui->webView->setZoomFactor(factor);
        ui->actionZoomOut->setEnabled(percent > 25);
        ui->actionZoomIn->setEnabled(true);
        zoomSlider->setValue(percent);
    }
}

void NotesEditor::zoomIn()
{
    int percent = static_cast<int>(ui->webView->zoomFactor() * 100);
    if (percent < 400) {
        percent += 25;
        percent = 25 * (int(percent / 25));
        qreal factor = static_cast<qreal>(percent) / 100;
        ui->webView->setZoomFactor(factor);
        ui->actionZoomIn->setEnabled(percent < 400);
        ui->actionZoomOut->setEnabled(true);
        zoomSlider->setValue(percent);
    }
}

void NotesEditor::editSelectAll()
{
    ui->webView->triggerPageAction(QWebPage::SelectAll);
}

void NotesEditor::execCommand(const QString &cmd)
{
    QWebFrame *frame = ui->webView->page()->mainFrame();
    QString js = QString("document.execCommand(\"%1\", false, null)").arg(cmd);
    frame->evaluateJavaScript(js);
}

void NotesEditor::execCommand(const QString &cmd, const QString &arg)
{
    QWebFrame *frame = ui->webView->page()->mainFrame();
    QString js = QString("document.execCommand(\"%1\", false, \"%2\")").arg(cmd).arg(arg);
    frame->evaluateJavaScript(js);
}

bool NotesEditor::queryCommandState(const QString &cmd)
{
    QWebFrame *frame = ui->webView->page()->mainFrame();
    QString js = QString("document.queryCommandState(\"%1\", false, null)").arg(cmd);
    QVariant result = frame->evaluateJavaScript(js);
    return result.toString().simplified().toLower() == "true";
}

void NotesEditor::styleParagraph()
{
    execCommand("formatBlock", "p");
}

void NotesEditor::styleHeading1()
{
    execCommand("formatBlock", "h1");
}

void NotesEditor::styleHeading2()
{
    execCommand("formatBlock", "h2");
}

void NotesEditor::styleHeading3()
{
    execCommand("formatBlock", "h3");
}

void NotesEditor::styleHeading4()
{
    execCommand("formatBlock", "h4");
}

void NotesEditor::styleHeading5()
{
    execCommand("formatBlock", "h5");
}

void NotesEditor::styleHeading6()
{
    execCommand("formatBlock", "h6");
}

void NotesEditor::stylePreformatted()
{
    execCommand("formatBlock", "pre");
}

void NotesEditor::styleAddress()
{
    execCommand("formatBlock", "address");
}

void NotesEditor::formatStrikeThrough()
{
    execCommand("strikeThrough");
}

void NotesEditor::formatAlignLeft()
{
    execCommand("justifyLeft");
}

void NotesEditor::formatAlignCenter()
{
    execCommand("justifyCenter");
}

void NotesEditor::formatAlignRight()
{
    execCommand("justifyRight");
}

void NotesEditor::formatAlignJustify()
{
    execCommand("justifyFull");
}

void NotesEditor::formatIncreaseIndent()
{
    execCommand("indent");
}

void NotesEditor::formatDecreaseIndent()
{
    execCommand("outdent");
}

void NotesEditor::formatNumberedList()
{
    execCommand("insertOrderedList");
}

void NotesEditor::formatBulletedList()
{
    execCommand("insertUnorderedList");
}

void NotesEditor::formatFontName()
{
    QStringList families = QFontDatabase().families();
    bool ok = false;
    QString family = QInputDialog::getItem(this, tr("Font"), tr("Select font:"),
                                           families, 0, false, &ok);

    if (ok)
        execCommand("fontName", family);
}

void NotesEditor::formatFontSize()
{
    QStringList sizes;
    sizes << "xx-small";
    sizes << "x-small";
    sizes << "small";
    sizes << "medium";
    sizes << "large";
    sizes << "x-large";
    sizes << "xx-large";

    bool ok = false;
    QString size = QInputDialog::getItem(this, tr("Font Size"), tr("Select font size:"),
                                         sizes, sizes.indexOf("medium"), false, &ok);

    if (ok)
        execCommand("fontSize", QString::number(sizes.indexOf(size)));
}

void NotesEditor::formatTextColor()
{
    QColor color = QColorDialog::getColor(Qt::black, this);
    if (color.isValid())
        execCommand("foreColor", color.name());
}

void NotesEditor::formatBackgroundColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if (color.isValid())
        execCommand("hiliteColor", color.name());
}

#define FOLLOW_ENABLE(a1, a2) a1->setEnabled(ui->webView->pageAction(a2)->isEnabled())
#define FOLLOW_CHECK(a1, a2) a1->setChecked(ui->webView->pageAction(a2)->isChecked())

void NotesEditor::adjustActions()
{
    FOLLOW_ENABLE(ui->actionEditUndo, QWebPage::Undo);
    FOLLOW_ENABLE(ui->actionEditRedo, QWebPage::Redo);
    FOLLOW_ENABLE(ui->actionEditCut, QWebPage::Cut);
    FOLLOW_ENABLE(ui->actionEditCopy, QWebPage::Copy);
    FOLLOW_ENABLE(ui->actionEditPaste, QWebPage::Paste);
    FOLLOW_CHECK(ui->actionFormatBold, QWebPage::ToggleBold);
    FOLLOW_CHECK(ui->actionFormatItalic, QWebPage::ToggleItalic);
    FOLLOW_CHECK(ui->actionFormatUnderline, QWebPage::ToggleUnderline);

    ui->actionFormatStrikethrough->setChecked(queryCommandState("strikeThrough"));
    ui->actionFormatNumberedList->setChecked(queryCommandState("insertOrderedList"));
    ui->actionFormatBulletedList->setChecked(queryCommandState("insertUnorderedList"));
}

void NotesEditor::adjustSource()
{
    sourceDirty = true;

    if (ui->tabWidget->currentIndex() == 1)
        changeTab(1);
}

void NotesEditor::changeTab(int index)
{
    if (sourceDirty && (index == 1)) {
        QString content = ui->webView->page()->mainFrame()->toHtml();
        ui->plainTextEdit->setPlainText(content);
        sourceDirty = false;
    }
}


void NotesEditor::changeZoom(int percent)
{
    ui->actionZoomOut->setEnabled(percent > 25);
    ui->actionZoomIn->setEnabled(percent < 400);
    qreal factor = static_cast<qreal>(percent) / 100;
    ui->webView->setZoomFactor(factor);

    zoomLabel->setText(tr(" Zoom: %1% ").arg(percent));
    zoomSlider->setValue(percent);
}

void NotesEditor::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    m_simpleNotes->saveNote();
}

/*bool NotesEditor::load(const QString &f)
{
    if (!QFile::exists(f))
        return false;
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    ui->webView->setContent(data, "text/html");
    ui->webView->page()->setContentEditable(true);
    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(ui->webView, SIGNAL(linkClicked(QUrl)), SLOT(openLink(QUrl)));

    setCurrentFileName(f);
    return true;
}*/

