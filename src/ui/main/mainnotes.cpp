/***************************************************************************
openBibleViewer - Free Bibel Viewer
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
#include "mainwindow.h"
#include "../../core/stelle.h"
#include "../../core/notes.h"
#include "../poschoser.h"
#include "ui_mainwindow.h"

#include <QtCore/QString>
#include <QtGui/QTreeView>
#include <QtGui/QClipboard>
#include <QtGui/QColorDialog>

int MainWindow::loadNotes(void)
{

    if (ui->dockWidget_notes->isVisible()) {
        ui->dockWidget_notes->hide();
    } else {
        ui->dockWidget_notes->show();
    }
    note = new notes(homeDataPath + "notes.xml");
    note->loadNotes();
    note->readNotes();
    ui->listWidget_notes->clear();
    ui->listWidget_notes->insertItems(0, note->notesTitel);
    currentNoteID = -2;
    return 0;
}
int MainWindow::showNote(QListWidgetItem *item)
{
    int id;
    //qDebug() << "MainWindow::showNote() currentNoteID = " << currentNoteID;
    id = currentNoteID;
    QStringList myNotesData = note->notesData;
    QStringList myNotesTitel = note->notesTitel;
    QStringList myNotesPos = note->notesPos;
    note->notesData.clear();
    note->notesTitel.clear();
    note->notesPos.clear();
    for (int i = 0; i < myNotesTitel.size(); i++) {
        if (i != id) {
            note->notesData << myNotesData.at(i);
            note->notesTitel << myNotesTitel.at(i);
            note->notesPos << myNotesPos.at(i);
        } else {
            note->notesData << ui->textEdit_note->toHtml();
            note->notesTitel << ui->lineEdit_note_titel->text();
            note->notesPos << currentNotePos;
        }
    }
    note->saveNotes();
    statusBar()->showMessage(tr("Note saved"), 5000);

    id = ui->listWidget_notes->row(item);
    currentNoteID = id;
    if (id < note->notesData.size() && id >= 0) {
        //qDebug() << "MainWindow::showNote() id = " << id;
        ui->lineEdit_note_titel->setText(note->notesTitel.at(id));
        ui->textEdit_note->setHtml(note->notesData.at(id));
        currentNotePos = note->notesPos.at(id);
        ui->label_noteLink->setText(notePos2Text(currentNotePos));
    }
    return 0;
}
int MainWindow::copyNote(void)
{
    int id = ui->listWidget_notes->currentRow();
    if (id < note->notesData.size() && id >= 0) {
        QClipboard *clipboard = QApplication::clipboard();
        QTextDocument doc;
        doc.setHtml(note->notesData.at(id));
        clipboard->setText(doc.toPlainText());
    } else {
        //qDebug() << "MainWindow::copyNote() no note";
    }
    return 0;
}
int MainWindow::saveNote(void)
{
    int id;
    //qDebug() << "MainWindow::saveNote() currentNoteID = " << currentNoteID;
    //id = ui->listWidget_notes->currentRow();
    id = currentNoteID;
    QStringList myNotesData = note->notesData;
    QStringList myNotesTitel = note->notesTitel;
    QStringList myNotesPos = note->notesPos;
    note->notesData.clear();
    note->notesTitel.clear();
    note->notesPos.clear();
    for (int i = 0; i < myNotesTitel.size(); i++) {
        if (i != id) {
            note->notesData << myNotesData.at(i);
            note->notesTitel << myNotesTitel.at(i);
            note->notesPos << myNotesPos.at(i);
        } else {
            note->notesData << ui->textEdit_note->toHtml();
            note->notesTitel << ui->lineEdit_note_titel->text();
            note->notesPos << currentNotePos;
        }
    }
    note->saveNotes();
    statusBar()->showMessage(tr("Note saved"), 5000);
    reloadNotes();
    return 0;
}
int MainWindow::newNote(void)
{
    saveNote();
    ui->lineEdit_note_titel->setText(tr("(unnamed)"));
    ui->textEdit_note->setHtml("");
    note->notesData << "";
    note->notesTitel << tr("(unnamed)");
    note->notesPos << "";
    reloadNotes();
    // ui->listWidget_notes->setCurrentRow(ui->listWidget_notes->count()-1);
    ui->listWidget_notes->setCurrentRow(note->notesData.size() - 1);
    currentNoteID = note->notesData.size() - 1;
    return 0;
}
int MainWindow::newNoteWithLink()
{
    QTextCursor cursor = currentTextCursor;
    int startverse = verseFromCursor(cursor);
    QString pos;
    pos = bibleDirName[currentBibleID] + ";" + QString::number(b.currentBookID, 10) + ";" + QString::number(b.currentChapterID + 1 - b.chapterAdd, 10) + ";" + QString::number(startverse, 10) + ";" + b.bookFullName.at(b.currentBookID);

    saveNote();
    ui->lineEdit_note_titel->setText(tr("(unnamed)"));
    ui->textEdit_note->setHtml("");
    note->notesData << "";
    note->notesTitel << tr("(unnamed)");
    note->notesPos << pos;
    reloadNotes();
    ui->listWidget_notes->setCurrentRow(note->notesData.size() - 1);
    currentNoteID = note->notesData.size() - 1;
    //qDebug() << "MainWindow::newNoteWithLink() pos = " << pos;
    return 0;
}
int MainWindow::removeNote(void)
{
    int id = currentNoteID;
    //  qDebug() << "MainWindow::removeNote() id = "<< id << "notesPos = "<<note->notesPos;
    if (id < 0)
        return 1;

    //  qDebug() << "MainWindow::removeNote() notesTitel = "<<note->notesTitel;

    note->notesData.removeAt(id);
    note->notesTitel.removeAt(id);
    note->notesPos.removeAt(id);

    ui->lineEdit_note_titel->setText(tr(""));
    ui->textEdit_note->setHtml("");
    if (id == 0) {
        currentNoteID = 1;
    } else {
        currentNoteID = id - 1;
    }
    reloadNotes();
    // saveNote();

    id = ui->listWidget_notes->currentRow();
    qDebug() << "MainWindow::removeNote() 2 id = " << id << "notesPos = " << note->notesPos;
    qDebug() << "MainWindow::removeNote() 2 notesTitel = " << note->notesTitel;
    return 0;
}
int MainWindow::reloadNotes(void)
{
    qDebug() << "MainWindow::reloadNotes";
    ui->listWidget_notes->clear();
    ui->listWidget_notes->insertItems(0, note->notesTitel);
    ui->listWidget_notes->setCurrentRow(currentNoteID);
    return 0;
}
int MainWindow::notesContextMenu(void)
{
    //qDebug("MainWindow::notesContextMenu() start");
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
    return 0;
}
int MainWindow::noteSetTextBold(void)
{
    if (ui->textEdit_note->fontWeight() == QFont::Bold) {
        ui->textEdit_note->setFontWeight(QFont::Normal);
    } else {
        ui->textEdit_note->setFontWeight(QFont::Bold);
    }
    return 0;
}
int MainWindow::noteSetTextItalic(void)
{
    if (ui->textEdit_note->fontItalic()) {
        ui->textEdit_note->setFontItalic(false);
    } else {
        ui->textEdit_note->setFontItalic(true);
    }
    return 0;
}
int MainWindow::noteSetTextUnderline(void)
{
    if (ui->textEdit_note->fontUnderline()) {
        ui->textEdit_note->setFontUnderline(false);
    } else {
        ui->textEdit_note->setFontUnderline(true);
    }
    return 0;
}
int MainWindow::noteSetTextColor(void)
{
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid()) {
        ui->textEdit_note->setTextColor(color);
    }
    return 0;
}
int MainWindow::noteGo(QString pos)
{
    if ( internalOpenPos(pos) == 1) {
        qDebug("MainWindow::noteGo( void ) invalid note");
    }
    return 0;
}

QString MainWindow::notePos2Text(QString pos)
{
    //qDebug("MainWindow::notePos2Text start pos=%s",pos.toStdString().c_str());
    QString string = "";
    QStringList list = pos.split(";");
    if (list.size() < 5) {
        //qDebug("MainWindow::notePos2Text( ) invalid pos");
        return "";
    }
    QString dirname = list.at(0);
    QString schapterID = list.at(2);
    QString sverseID = list.at(3);
    QString bookName = list.at(4);
    int bibleID = 0;
    int chapterID = schapterID.toInt();
    int verseID = sverseID.toInt();
    //get bibleID
    //qDebug("MainWindow::notePos2Text() get bibleID");
    for (int i = 0; i < bibleDirName.size(); i++) {
        if (bibleDirName.at(i) == dirname) {
            bibleID = i;
            break;
        }
    }
    //load bible id

    //qDebug("MainWindow::notePos2Text() generate string with bibleID= %i and bookID = %i ",bibleID,bookID);
    string =  bookName + " " + QString::number(chapterID, 10) + "," + QString::number(verseID, 10);
    //qDebug("MainWindow::notePos2Text end");
    return  "<a href=\"" + pos + "\" > " + string + "</a>";
}
void MainWindow::editNoteLink()
{
    /*int id = ui->listWidget_notes->currentRow();
    QString pos = note->notesPos.at(id);

    QStringList list = pos.split(";");
    if(list.size() < 4)
    {
        return ;
    }
    QString dirname = list.at(0);
    QString sbookID = list.at(1);
    QString schapterID = list.at(2);
    QString sverseID = list.at(3);
    int bibleID = 0;
    int bookID = sbookID.toInt();
    int chapterID = schapterID.toInt();
    int verseID = sverseID.toInt();
    //get bibleID
    for(int i=0;i<bibleDirName.size();i++)
    {
        if(bibleDirName.at(i) == dirname)
        {
            bibleID = i;
            break;
        }
    }
    posChoser *pChoser = new posChoser(this);
    //connect( pChoser, SIGNAL( searched( QString,bool,bool,bool ) ), this, SLOT( showSearchResults( QString,bool,bool,bool ) ) );
    qDebug() << "MainWindow::editNoteLink() b.chapterData.size() = " << b.chapterData.size(),
    pChoser->setData(bibles,b.bookFullName);
    pChoser->setCurrent(bibleID,bookID,chapterID,verseID);
    pChoser->show();
    pChoser->exec();*/
}
