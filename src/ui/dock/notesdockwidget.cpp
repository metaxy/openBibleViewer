/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#include "notesdockwidget.h"
#include "ui_notesdockwidget.h"
#include "src/core/dbghelper.h"
#include "src/ui/dialog/biblepassagedialog.h"
#include <QtGui/QClipboard>
#include <QtGui/QMenu>
#include <QtGui/QColorDialog>
NotesDockWidget::NotesDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::NotesDockWidget)
{
    ui->setupUi(this);
}

NotesDockWidget::~NotesDockWidget()
{
    delete ui;
}
void NotesDockWidget::init()
{
    DEBUG_FUNC_NAME
    connect(ui->pushButton_editNoteLink, SIGNAL(clicked()), this, SLOT(editNoteLink()));
    connect(ui->listWidget_notes, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(notesContextMenu()));

    connect(ui->pushButton_note_save, SIGNAL(clicked()), this, SLOT(saveNote()));
    connect(ui->toolButton_noteBold, SIGNAL(clicked()), this, SLOT(noteSetTextBold()));
    connect(ui->toolButton_noteItalic, SIGNAL(clicked()), this, SLOT(noteSetTextItalic()));
    connect(ui->toolButton_noteUnderline, SIGNAL(clicked()), this, SLOT(noteSetTextUnderline()));
    connect(ui->toolButton_noteColor, SIGNAL(clicked()), this, SLOT(noteSetTextColor()));
    connect(ui->toolButton_noteUndo, SIGNAL(clicked()), this, SLOT(noteUndo()));
    connect(ui->toolButton_noteRedo, SIGNAL(clicked()), this, SLOT(noteRedo()));

    connect(ui->textEdit_note, SIGNAL(undoAvailable(bool)), ui->toolButton_noteUndo, SLOT(setEnabled(bool)));
    connect(ui->textEdit_note, SIGNAL(redoAvailable(bool)), ui->toolButton_noteRedo, SLOT(setEnabled(bool)));
    connect(ui->listWidget_notes, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(showNote(QListWidgetItem *)));
    if (!m_notes->isLoaded()) {
        //it has to be loaded first !!!!
        myDebug() << "new Notes";
        m_notes->init(m_settings->homePath + "notes.xml");
        m_notes->loadNotes();
        m_notes->readNotes();
    }
    m_notes->loadingNewInstance();
    connect(m_notes, SIGNAL(saveAll()), this, SLOT(saveNote()));

    ui->listWidget_notes->clear();
    m_textNotesID.clear();
    QStringList id = m_notes->getIDList();
    QStringList titles;
    for (int i = 0; i < id.size(); i++) {
        if (m_notes->getType(id.at(i)) == "text") {
            titles << m_notes->getTitle(id.at(i));
            m_textNotesID << id.at(i);
        }
    }
    ui->listWidget_notes->insertItems(0, titles);
    m_noteID = "";
    m_moduleManager->m_bible.setNotes(m_notes);
}
void NotesDockWidget::showNote(QListWidgetItem *item)
{
    int id = ui->listWidget_notes->row(item);
    if (id >= 0 && id < m_textNotesID.size()) {
        showNote(m_textNotesID.at(id));
    }
}
void NotesDockWidget::showNote(const QString &noteID)
{
    //DEBUG_FUNC_NAME
    //save current notes
    int row = m_textNotesID.indexOf(noteID);
    ui->listWidget_notes->setCurrentRow(row);
    m_notes->setData(m_noteID, ui->textEdit_note->toHtml());
    m_notes->setTitle(m_noteID, ui->lineEdit_note_titel->text());
    m_notes->setRef(m_noteID, currentNoteRef);
    m_notes->saveNotes();//save on hard disk
    if (!m_textNotesID.contains(noteID)) {
        myDebug() << "invalid noteID = " << noteID;
        return;
    }
    //load new notes
    m_noteID = noteID;
    myDebug() << "id = " << noteID << " m_noteID = " << m_noteID;
    ui->lineEdit_note_titel->setText(m_notes->getTitle(m_noteID));
    ui->textEdit_note->setHtml(m_notes->getData(m_noteID));
    currentNoteRef = m_notes->getRef(m_noteID);
    myDebug() << "link = " << m_notes->getRef(m_noteID, "link");
    if (!m_notes->getRef(m_noteID, "link").isEmpty()) {
        ui->label_noteLink->setText(m_moduleManager->notePos2Text(m_notes->getRef(m_noteID, "link")));
        ui->pushButton_editNoteLink->setEnabled(true);
    } else {
        ui->label_noteLink->setText("");
        ui->pushButton_editNoteLink->setEnabled(false);
    }

}

void NotesDockWidget::copyNote(void)
{
    //DEBUG_FUNC_NAME

    int id = ui->listWidget_notes->currentRow();
    if (id < m_textNotesID.size() && id >= 0) {
        QClipboard *clipboard = QApplication::clipboard();
        QTextDocument doc;
        doc.setHtml(m_notes->getData(m_textNotesID.at(id)));
        clipboard->setText(doc.toPlainText());
    } else {
        myDebug() << "no note";
    }
}
void NotesDockWidget::saveNote(void)
{
    //DEBUG_FUNC_NAME
    myDebug() << "m_noteID = " << m_noteID;
    m_notes->setData(m_noteID, ui->textEdit_note->toHtml());
    m_notes->setTitle(m_noteID, ui->lineEdit_note_titel->text());
    m_notes->setRef(m_noteID, currentNoteRef);
    m_notes->saveNotes();
    reloadNotes();
}
void NotesDockWidget::newNote(void)
{
    //DEBUG_FUNC_NAME

    saveNote();
    reloadNotes();
    QString newID = m_notes->generateNewID();
    m_notes->setData(newID, "");
    m_notes->setTitle(newID, tr("(unnamed)"));
    m_notes->setType(newID, "text");
    QMap<QString, QString> ref;
    currentNoteRef = ref;
    //m["link"] = "";
    m_notes->setRef(newID, ref);
    m_notes->insertID(newID);
    m_noteID = newID;
    reloadNotes();
    ui->listWidget_notes->setCurrentRow(m_textNotesID.size() - 1);
    //showNote(ui->listWidget_notes->currentItem());

    ui->label_noteLink->setText("");
    ui->lineEdit_note_titel->setText(tr("(unnamed)"));
    ui->textEdit_note->setHtml("");

    ui->label_noteLink->setText("");
    ui->pushButton_editNoteLink->setEnabled(false);

    myDebug() << " newID = " << newID << " m_noteID = " << m_noteID;
}
void NotesDockWidget::newNoteWithLink(VerseSelection selection)
{
    DEBUG_FUNC_NAME

    QString link;
    UrlConverter urlConverter(UrlConverter::None, UrlConverter::PersistentUrl, "");
    urlConverter.m_biblesIniPath = m_moduleManager->m_bible.biblesIniPath;
    urlConverter.m_bibleID =  QString::number(m_moduleManager->m_bible.bibleID());
    urlConverter.m_bookID = m_moduleManager->m_bible.bookID();
    urlConverter.m_chapterID = m_moduleManager->m_bible.chapterID() - m_moduleManager->m_bible.chapterAdd();
    urlConverter.m_verseID = selection.startVerse - 1;
    urlConverter.m_bookName = m_moduleManager->m_bible.bookFullName.at(m_moduleManager->m_bible.bookID());
    link = urlConverter.convert();
    //link = biblesIniPath.at(m_moduleManager->m_bible.bibleID()) + ";" + QString::number(m_moduleManager->m_bible.bookID(), 10) + ";" + QString::number(m_moduleManager->m_bible.chapterID() + 1 - m_moduleManager->m_bible.chapterAdd(), 10) + ";" + QString::number(startverse, 10) + ";" + ;

    saveNote();
    reloadNotes();
    QString newID = m_notes->generateNewID();
    m_notes->setData(newID, "");
    m_notes->setTitle(newID, tr("(unnamed)"));
    m_notes->setType(newID, "text");
    QMap<QString, QString> ref;
    ref["link"] = link;
    currentNoteRef = ref;
    m_notes->setRef(newID, ref);
    m_notes->insertID(newID);
    m_noteID = newID;
    reloadNotes();
    ui->listWidget_notes->setCurrentRow(m_textNotesID.size() - 1);
    ui->label_noteLink->setText("");
    ui->lineEdit_note_titel->setText(tr("(unnamed)"));
    ui->textEdit_note->setHtml("");
    if (!m_notes->getRef(m_noteID, "link").isEmpty()) {
        ui->label_noteLink->setText(m_moduleManager->notePos2Text(m_notes->getRef(m_noteID, "link")));
        ui->pushButton_editNoteLink->setEnabled(true);
    } else {
        ui->label_noteLink->setText("");
        ui->pushButton_editNoteLink->setEnabled(false);
    }
    emit reloadChapter();
}
void NotesDockWidget::removeNote(void)
{
    //DEBUG_FUNC_NAME
    m_notes->removeNote(m_noteID);
    ui->lineEdit_note_titel->setText(tr(""));
    ui->textEdit_note->setHtml("");
    ui->label_noteLink->setText("");
    reloadNotes();
}
void NotesDockWidget::reloadNotes(void)
{
    //DEBUG_FUNC_NAME
    ui->listWidget_notes->clear();
    m_textNotesID.clear();
    QStringList id = m_notes->getIDList();
    QStringList titles;
    for (int i = 0; i < id.size(); i++) {
        if (m_notes->getType(id.at(i)) == "text") {
            titles << m_notes->getTitle(id.at(i));
            m_textNotesID << id.at(i);
        }
    }
    ui->listWidget_notes->insertItems(0, titles);
    ui->listWidget_notes->setCurrentRow(id.lastIndexOf(m_noteID));
}
void NotesDockWidget::notesContextMenu(void)
{
    QMenu *contextMenu = new QMenu(this);
    contextMenu->setObjectName("contextMenu");

    QAction *actionCopy = new QAction(this);
    actionCopy->setObjectName("actionCopy");
    actionCopy->setText(tr("Copy"));
    QIcon iconCopy;
    iconCopy.addPixmap(QPixmap(":/icons/16x16/edit-copy.png"), QIcon::Normal, QIcon::Off);
    actionCopy->setIcon(iconCopy);
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copyNote()));

    QAction *actionNew = new QAction(this);
    actionNew->setObjectName(QString::fromUtf8("actionNew"));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(newNote()));
    QIcon iconNew;
    iconNew.addPixmap(QPixmap(":/icons/16x16/document-new.png"), QIcon::Normal, QIcon::Off);
    actionNew->setIcon(iconNew);
    actionNew->setText(tr("New"));

    QAction *actionRemove = new QAction(this);
    actionRemove->setObjectName("actionRemove");
    actionRemove->setText(tr("Delete"));
    QIcon iconRemove;
    iconRemove.addPixmap(QPixmap(":/icons/16x16/edit-delete.png"), QIcon::Normal, QIcon::Off);
    actionRemove->setIcon(iconRemove);
    connect(actionRemove, SIGNAL(triggered()), this, SLOT(removeNote()));

    contextMenu->addAction(actionCopy);
    contextMenu->addAction(actionNew);
    contextMenu->addAction(actionRemove);
    contextMenu->exec(QCursor::pos());
}
void NotesDockWidget::noteSetTextBold(void)
{
    if (ui->textEdit_note->fontWeight() == QFont::Bold) {
        ui->textEdit_note->setFontWeight(QFont::Normal);
    } else {
        ui->textEdit_note->setFontWeight(QFont::Bold);
    }
}
void NotesDockWidget::noteSetTextItalic(void)
{
    if (ui->textEdit_note->fontItalic()) {
        ui->textEdit_note->setFontItalic(false);
    } else {
        ui->textEdit_note->setFontItalic(true);
    }
}
void NotesDockWidget::noteSetTextUnderline(void)
{
    if (ui->textEdit_note->fontUnderline()) {
        ui->textEdit_note->setFontUnderline(false);
    } else {
        ui->textEdit_note->setFontUnderline(true);
    }
}
void NotesDockWidget::noteSetTextColor(void)
{
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid()) {
        ui->textEdit_note->setTextColor(color);
    }
}
void NotesDockWidget::noteUndo()
{
    ui->textEdit_note->undo();
}
void NotesDockWidget::noteRedo()
{
    ui->textEdit_note->redo();
}


void NotesDockWidget::editNoteLink()
{
    //DEBUG_FUNC_NAME
    QString link = currentNoteRef["link"];
    myDebug() << "link = " << link;

    UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::None, link);
    urlConverter.m_biblesIniPath = m_moduleManager->m_bible.biblesIniPath;
    urlConverter.pharse();

    BiblePassageDialog *passageDialog = new  BiblePassageDialog(this);
    connect(passageDialog, SIGNAL(updated(QString)), this, SLOT(updateNote(QString)));
    passageDialog->setSettings(m_settings);
    passageDialog->setCurrent(urlConverter.m_bibleID.toInt(), urlConverter.m_path, urlConverter.m_bookID, urlConverter.m_chapterID + 1, urlConverter.m_verseID + 1);
    passageDialog->show();
    passageDialog->exec();

}
void NotesDockWidget::updateNote(QString link)
{
    //DEBUG_FUNC_NAME
    myDebug() << "link = " << link;
    currentNoteRef["link"] = link;
    showNote(ui->listWidget_notes->currentItem());
    return;
}

void NotesDockWidget::newMark(VerseSelection selection, QColor color)
{
    //DEBUG_FUNC_NAME
    QString link;
    UrlConverter urlConverter(UrlConverter::None, UrlConverter::PersistentUrl, "");
    urlConverter.m_biblesIniPath = m_moduleManager->m_bible.biblesIniPath;
    urlConverter.m_bibleID = QString::number(m_moduleManager->m_bible.bibleID());
    urlConverter.m_bookID = m_moduleManager->m_bible.bookID();
    urlConverter.m_chapterID = m_moduleManager->m_bible.chapterID() - m_moduleManager->m_bible.chapterAdd();
    urlConverter.m_verseID = selection.startVerse;
    urlConverter.m_bookName = m_moduleManager->m_bible.bookFullName.at(m_moduleManager->m_bible.bookID());
    link = urlConverter.convert();

    saveNote();
    reloadNotes();
    QString newID = m_notes->generateNewID();
    m_notes->setData(newID, "");
    m_notes->setTitle(newID, tr("(unnamed)"));
    m_notes->setType(newID, "mark");
    QMap<QString, QString> ref;
    ref["link"] = link;
    ref["start"] = QString::number(selection.startVerse - 1);
    ref["end"] = QString::number(selection.endVerse - 1);
    ref["startPos"] = QString::number(selection.posInStartVerse);
    ref["endPos"] = QString::number(selection.posInEndVerse);
    ref["startString"] = selection.shortestStringInStartVerse;
    ref["endString"] = selection.shortestStringInEndVerse;
    ref["color"] = color.name();
    m_notes->setRef(newID, ref);
    m_notes->insertID(newID);

    emit reloadChapter();

}
void NotesDockWidget::removeMark(VerseSelection selection)
{
    //DEBUG_FUNC_NAME
    QStringList id = m_notes->getIDList();
    for (int i = 0; i < id.size(); i++) {
        if (m_notes->getType(id.at(i)) == "mark") {
            QString noteID = id.at(i);
            QString link = m_notes->getRef(noteID, "link");
            UrlConverter urlConverter(UrlConverter::PersistentUrl, UrlConverter::None, link);
            urlConverter.m_biblesIniPath = m_moduleManager->m_bible.biblesIniPath;
            urlConverter.pharse();

            if (urlConverter.m_bibleID.toInt() == m_moduleManager->m_bible.bibleID() && urlConverter.m_bookID == m_moduleManager->m_bible.bookID() && urlConverter.m_chapterID == m_moduleManager->m_bible.chapterID()) {
                int start = selection.startVerse - 1;
                int end;
                if (selection.endVerse != -1) {
                    end = selection.endVerse;
                } else {
                    end = start;
                }
                if (m_notes->getRef(noteID, "start").toInt() <= start && end <= m_notes->getRef(noteID, "end").toInt()) {
                    //todo: work with positions in text
                    m_notes->removeNote(noteID);
                    emit reloadChapter();
                }
            }
        }
    }
}
void NotesDockWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
