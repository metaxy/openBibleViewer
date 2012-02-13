/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
#include "src/core/dbghelper.h"
#include "src/ui/dialog/insertlinkdialog.h"
#include "src/module/moduletools.h"

#include "ui_noteseditor.h"
#include "noteseditor.h"
#include <QtGui/QFileDialog>
#include <QtGui/QFontDatabase>
#include <QtGui/QInputDialog>
#include <QtGui/QColorDialog>
#include "ui_inserthtmldialog.h"
#include "highlighter.h"
#define FORWARD_ACTION(action1, action2) \
    connect(action1, SIGNAL(triggered()), \
            ui->webView->pageAction(action2), SLOT(trigger())); \
    connect(ui->webView->pageAction(action2), \
            SIGNAL(changed()), SLOT(adjustActions()));


NotesEditor::NotesEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NotesEditor)
    , m_sourceDirty(true)
    , m_highlighter(0)
    , ui_dialog(0)
    , m_insertHtmlDialog(0)
{
    ui->setupUi(this);
    ui->tabWidget->setTabText(0, tr("Normal View"));
    ui->tabWidget->setTabText(1, tr("HTML Source"));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(changeTab(int)));

    m_highlighter = new Highlighter(ui->plainTextEdit->document());
    m_simpleNotes = new SimpleNotes();

    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    ui->standardToolBar->insertWidget(ui->actionZoomOut, spacer);

    m_zoomLabel = new QLabel(this);
    ui->standardToolBar->insertWidget(ui->actionZoomOut, m_zoomLabel);

    m_zoomSlider = new QSlider(this);
    m_zoomSlider->setOrientation(Qt::Horizontal);
    m_zoomSlider->setMaximumWidth(150);
    m_zoomSlider->setRange(25, 500);
    m_zoomSlider->setSingleStep(25);
    m_zoomSlider->setPageStep(100);
    connect(m_zoomSlider, SIGNAL(valueChanged(int)), SLOT(changeZoom(int)));
    ui->standardToolBar->insertWidget(ui->actionZoomIn, m_zoomSlider);

    connect(ui->actionFileSave, SIGNAL(triggered()), SLOT(fileSave()));
    connect(ui->actionNewFile, SIGNAL(triggered()), m_simpleNotes, SLOT(newTextNote()));
    connect(ui->actionFileSaveAs, SIGNAL(triggered()), SLOT(fileSaveAs()));
    connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    connect(ui->actionInsertImage, SIGNAL(triggered()), SLOT(insertImage()));
    connect(ui->actionCreateLink, SIGNAL(triggered()), SLOT(createLink()));
    connect(ui->actionInsertHtml, SIGNAL(triggered()), SLOT(insertHtml()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), SLOT(zoomOut()));
    connect(ui->actionZoomIn, SIGNAL(triggered()), SLOT(zoomIn()));


    FORWARD_ACTION(ui->actionEditUndo, QWebPage::Undo);
    FORWARD_ACTION(ui->actionEditRedo, QWebPage::Redo);
    FORWARD_ACTION(ui->actionEditCut, QWebPage::Cut);
    FORWARD_ACTION(ui->actionEditCopy, QWebPage::Copy);
    FORWARD_ACTION(ui->actionEditPaste, QWebPage::Paste);
    FORWARD_ACTION(ui->actionFormatBold, QWebPage::ToggleBold);
    FORWARD_ACTION(ui->actionFormatItalic, QWebPage::ToggleItalic);
    FORWARD_ACTION(ui->actionFormatUnderline, QWebPage::ToggleUnderline);


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


    connect(ui->actionFormatStrikethrough, SIGNAL(triggered()), SLOT(formatStrikeThrough()));
    connect(ui->actionFormatAlignLeft, SIGNAL(triggered()), SLOT(formatAlignLeft()));
    connect(ui->actionFormatAlignCenter, SIGNAL(triggered()), SLOT(formatAlignCenter()));
    connect(ui->actionFormatAlignRight, SIGNAL(triggered()), SLOT(formatAlignRight()));
    connect(ui->actionFormatAlignJustify, SIGNAL(triggered()), SLOT(formatAlignJustify()));
    connect(ui->actionFormatDecreaseIndent, SIGNAL(triggered()), SLOT(formatDecreaseIndent()));
    connect(ui->actionFormatIncreaseIndent, SIGNAL(triggered()), SLOT(formatIncreaseIndent()));
    connect(ui->actionFormatNumberedList, SIGNAL(triggered()), SLOT(formatNumberedList()));
    connect(ui->actionFormatBulletedList, SIGNAL(triggered()), SLOT(formatBulletedList()));

    connect(ui->webView->page(), SIGNAL(selectionChanged()), SLOT(adjustActions()));

    connect(ui->webView->page(), SIGNAL(contentsChanged()), SLOT(adjustSource()));
    connect(m_simpleNotes, SIGNAL(contentChanged()), SLOT(adjustSource()));
    ui->webView->setFocus();

    fileNew();

    adjustActions();
    adjustSource();
    changeZoom(100);
}
void NotesEditor::init()
{
    setAll(m_simpleNotes);
    m_simpleNotes->setFrameWidget(ui->webView->page()->mainFrame());
    m_simpleNotes->setViewWidget(ui->treeView);
    m_simpleNotes->setTitleWidget(ui->lineEdit_noteTitle);
    m_simpleNotes->setLinkButtonWidget(ui->pushButton_editNoteLink);
    m_simpleNotes->setLinkWidget(ui->label_noteLink);

    m_simpleNotes->init();
    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(ui->webView->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(pharseUrl(QUrl)));
    //connect(ui->plainTextEdit, SIGNAL(textChanged()), this, SLOT(adjustHtml()));
}

NotesEditor::~NotesEditor()
{
    delete m_simpleNotes;
    delete m_highlighter;

    delete ui;
    delete ui_dialog;
}
void NotesEditor::adjustSource()
{
    DEBUG_FUNC_NAME
    myDebug() << "current index = " << ui->tabWidget->currentIndex();
    m_sourceDirty = true;

    if(ui->tabWidget->currentIndex() == 1)
        changeTab(1);
}
void NotesEditor::adjustHtml()
{
    DEBUG_FUNC_NAME;
    ui->webView->setHtml(ui->plainTextEdit->toPlainText());
}

void NotesEditor::changeTab(int index)
{
    DEBUG_FUNC_NAME;
    myDebug() << "dirty = " << m_sourceDirty << " index = " << index;
    if(m_sourceDirty && (index == 1)) {
        QString content = ui->webView->page()->mainFrame()->toHtml();
        ui->plainTextEdit->setPlainText(content);
        m_sourceDirty = false;
    } else if(index == 0) {
        adjustHtml();
    }
}

void NotesEditor::fileNew()
{
    ui->tabWidget->setCurrentIndex(0);
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
    if(ui->tabWidget->currentIndex() == 1)
        adjustHtml();
    m_simpleNotes->saveNote();
    return true;
}

bool NotesEditor::fileSaveAs()
{
    if(ui->tabWidget->currentIndex() == 1)
        adjustHtml();

    QFileDialog dialog(this);

    dialog.setAcceptMode(QFileDialog::AcceptSave);

    QString fn = dialog.getSaveFileName(this, tr("Save as..."),
                                        QString(), tr("HTML-Files (*.html *.htm);;Text-Files (*.txt);;All Files (*)"));
    if(fn.isEmpty())
        return false;
    QFile data(fn);
    if(data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        if(fn.endsWith(".html", Qt::CaseInsensitive) || fn.endsWith(".htm", Qt::CaseInsensitive)) {
            out <<  ui->webView->page()->mainFrame()->toHtml();
        } else {
            out <<  ui->webView->page()->mainFrame()->toPlainText();
        }
        data.close();
    }

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
    if(fn.isEmpty())
        return;
    if(!QFile::exists(fn))
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
    if(hasSchema) {
        QUrl url(urlStr, QUrl::TolerantMode);
        if(url.isValid())
            return url;
    }

    // Might be a file.
    if(QFile::exists(urlStr))
        return QUrl::fromLocalFile(urlStr);

    // Might be a shorturl - try to detect the schema.
    if(!hasSchema) {
        int dotIndex = urlStr.indexOf(QLatin1Char('.'));
        if(dotIndex != -1) {
            QString prefix = urlStr.left(dotIndex).toLower();
            QString schema = (prefix == QLatin1String("ftp")) ? prefix : QLatin1String("http");
            QUrl url(schema + QLatin1String("://") + urlStr, QUrl::TolerantMode);
            if(url.isValid())
                return url;
        }
    }

    // Fall back to QUrl's own tolerant parser.
    return QUrl(string, QUrl::TolerantMode);
}

void NotesEditor::createLink()
{

    InsertLinkDialog *insertLinkDialog = new InsertLinkDialog(this);
    connect(insertLinkDialog, SIGNAL(newLink(QString)), this, SLOT(createLink(QString)));
    setAll(insertLinkDialog);

    insertLinkDialog->init();
    //todo: bibleform: How can we get the current active window, and its url
    //insertLinkDialog->setCurrent(m_moduleManager->verseModule()->lastTextRanges()->source().source());
    insertLinkDialog->show();
    insertLinkDialog->exec();
}
void NotesEditor::createLink(QString link)
{
    if(link.startsWith("verse:") || link.startsWith("note:")) {
        execCommand("createLink", link);
    } else {
        QUrl url = guessUrlFromString(link);
        if(url.isValid())
            execCommand("createLink", url.toString());
    }
}
void NotesEditor::pharseUrl(QUrl url)
{
    DEBUG_FUNC_NAME;
    QString link = url.toString();
    const QString note = "note://";
    if(link.startsWith(note)) {
        link = link.remove(0, note.size());
        m_simpleNotes->showNote(link, true);
    } else {
        m_actions->get(url);
    }
}

void NotesEditor::insertHtml()
{
    if(!m_insertHtmlDialog) {
        m_insertHtmlDialog = new QDialog(this);
        if(!ui_dialog)
            ui_dialog = new Ui::InsertHtmlDialog;
        ui_dialog->setupUi(m_insertHtmlDialog);
        connect(ui_dialog->buttonBox, SIGNAL(accepted()),
                m_insertHtmlDialog, SLOT(accept()));
        connect(ui_dialog->buttonBox, SIGNAL(rejected()),
                m_insertHtmlDialog, SLOT(reject()));
    }

    ui_dialog->plainTextEdit->clear();
    ui_dialog->plainTextEdit->setFocus();
    Highlighter *hilite = new Highlighter(ui_dialog->plainTextEdit->document());

    if(m_insertHtmlDialog->exec() == QDialog::Accepted)
        execCommand("insertHTML", ui_dialog->plainTextEdit->toPlainText());

    delete hilite;
}

void NotesEditor::zoomOut()
{
    int percent = static_cast<int>(ui->webView->zoomFactor() * 100);
    if(percent > 25) {
        percent -= 25;
        percent = 25 * (int((percent + 25 - 1) / 25));
        qreal factor = static_cast<qreal>(percent) / 100;
        ui->webView->setZoomFactor(factor);
        ui->actionZoomOut->setEnabled(percent > 25);
        ui->actionZoomIn->setEnabled(true);
        m_zoomSlider->setValue(percent);
    }
}

void NotesEditor::zoomIn()
{
    int percent = static_cast<int>(ui->webView->zoomFactor() * 100);
    if(percent < 400) {
        percent += 25;
        percent = 25 * (int(percent / 25));
        qreal factor = static_cast<qreal>(percent) / 100;
        ui->webView->setZoomFactor(factor);
        ui->actionZoomIn->setEnabled(percent < 400);
        ui->actionZoomOut->setEnabled(true);
        m_zoomSlider->setValue(percent);
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

    if(ok)
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

    if(ok)
        execCommand("fontSize", QString::number(sizes.indexOf(size)));
}

void NotesEditor::formatTextColor()
{
    QColor color = QColorDialog::getColor(Qt::black, this);
    if(color.isValid())
        execCommand("foreColor", color.name());
}

void NotesEditor::formatBackgroundColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if(color.isValid())
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



void NotesEditor::changeZoom(int percent)
{
    ui->actionZoomOut->setEnabled(percent > 25);
    ui->actionZoomIn->setEnabled(percent < 500);
    qreal factor = static_cast<qreal>(percent) / 100;
    ui->webView->setZoomFactor(factor);

    m_zoomLabel->setText(tr(" Zoom: %1% ").arg(percent));
    m_zoomSlider->setValue(percent);
}

void NotesEditor::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    m_simpleNotes->saveNote();
}


