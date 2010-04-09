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
#include <QtGui/QMessageBox>
NotesDockWidget::NotesDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::NotesDockWidget)
{
    ui->setupUi(this);
    m_simpleNotes = new SimpleNotes();
}

NotesDockWidget::~NotesDockWidget()
{
    delete ui;
}
void NotesDockWidget::init()
{
    DEBUG_FUNC_NAME

    m_simpleNotes->setModuleManager(m_moduleManager);
    m_simpleNotes->setNotes(m_notes);
    m_simpleNotes->setSettings(m_settings);
    m_simpleNotes->setBibleDisplay(m_bibleDisplay);

    m_simpleNotes->setDataWidget(ui->textBrowser);
    m_simpleNotes->setViewWidget(ui->treeView);
    m_simpleNotes->setTitleWidget(ui->lineEdit_noteTitle);
    m_simpleNotes->setLinkButtonWidget(ui->pushButton_editNoteLink);
    m_simpleNotes->setLinkWidget(ui->label_noteLink);
    m_simpleNotes->init();

    connect(ui->pushButton_note_save, SIGNAL(clicked()), this, SLOT(saveNote()));
    connect(ui->toolButton_noteBold, SIGNAL(clicked()), this, SLOT(noteSetTextBold()));
    connect(ui->toolButton_noteItalic, SIGNAL(clicked()), this, SLOT(noteSetTextItalic()));
    connect(ui->toolButton_noteUnderline, SIGNAL(clicked()), this, SLOT(noteSetTextUnderline()));
    connect(ui->toolButton_noteColor, SIGNAL(clicked()), this, SLOT(noteSetTextColor()));
    connect(ui->toolButton_noteUndo, SIGNAL(clicked()), this, SLOT(noteUndo()));
    connect(ui->toolButton_noteRedo, SIGNAL(clicked()), this, SLOT(noteRedo()));

    connect(ui->textBrowser, SIGNAL(undoAvailable(bool)), ui->toolButton_noteUndo, SLOT(setEnabled(bool)));
    connect(ui->textBrowser, SIGNAL(redoAvailable(bool)), ui->toolButton_noteRedo, SLOT(setEnabled(bool)));

    // connect(ui->textBrowser, SIGNAL(undoAvailable(bool)), m_simpleNotes, SLOT(fastSave()));
    //    connect(ui->textBrowser, SIGNAL(redoAvailable(bool)), m_simpleNotes, SLOT(fastSave()));
    m_moduleManager->m_bible.setNotes(m_notes);//todo: fix this bug
}

void NotesDockWidget::showNote(const QString &noteID)
{
    m_simpleNotes->showNote(noteID, true);

}

void NotesDockWidget::saveNote(void)
{
    m_simpleNotes->saveNote();
}
void NotesDockWidget::newNote(void)
{
    m_simpleNotes->newNote();
}
void NotesDockWidget::newNoteWithLink(VerseSelection selection)
{
    m_simpleNotes->newNoteWithLink(selection);
    emit reloadChapter();
}
void NotesDockWidget::noteSetTextBold(void)
{
    if (ui->textBrowser->fontWeight() == QFont::Bold) {
        ui->textBrowser->setFontWeight(QFont::Normal);
    } else {
        ui->textBrowser->setFontWeight(QFont::Bold);
    }
}
void NotesDockWidget::noteSetTextItalic(void)
{
    if (ui->textBrowser->fontItalic()) {
        ui->textBrowser->setFontItalic(false);
    } else {
        ui->textBrowser->setFontItalic(true);
    }
}
void NotesDockWidget::noteSetTextUnderline(void)
{
    if (ui->textBrowser->fontUnderline()) {
        ui->textBrowser->setFontUnderline(false);
    } else {
        ui->textBrowser->setFontUnderline(true);
    }
}
void NotesDockWidget::noteSetTextColor(void)
{
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid()) {
        ui->textBrowser->setTextColor(color);
    }
}
void NotesDockWidget::noteUndo()
{
    ui->textBrowser->undo();
}
void NotesDockWidget::noteRedo()
{
    ui->textBrowser->redo();
}

void NotesDockWidget::newMark(VerseSelection selection, QColor color)
{
    //DEBUG_FUNC_NAME
    if (selection.shortestStringInEndVerse == "" || selection.shortestStringInStartVerse == "") {
        myDebug() << "cannot create mark";
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Cannot create mark."));
        return;
    }
    QString link;
    UrlConverter urlConverter(UrlConverter::None, UrlConverter::PersistentUrl, "");
    urlConverter.m_biblesIniPath = m_moduleManager->m_bible.biblesIniPath;
    urlConverter.m_bibleID = QString::number(m_moduleManager->m_bible.bibleID());
    urlConverter.m_bookID = m_moduleManager->m_bible.bookID();
    urlConverter.m_chapterID = m_moduleManager->m_bible.chapterID() - m_moduleManager->m_bible.chapterAdd();
    urlConverter.m_verseID = selection.startVerse;
    urlConverter.m_bookName = m_moduleManager->m_bible.bookFullName.at(m_moduleManager->m_bible.bookID());
    link = urlConverter.convert();

    m_notes->saveNotes();
    //reloadNotes();
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
