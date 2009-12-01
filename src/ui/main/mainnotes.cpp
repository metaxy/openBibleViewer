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
#include "mainwindow.h"
#include "../../core/stelle.h"
#include "../../core/notes.h"
#include "../../core/dbghelper.h"
#include "../poschoser.h"
#include "ui_mainwindow.h"

#include <QtCore/QString>
#include <QtGui/QTreeView>
#include <QtGui/QClipboard>
#include <QtGui/QColorDialog>

void MainWindow::loadNotes(void)
{
    // myDebug();
    DEBUG_FUNC_NAME
    if (ui->dockWidget_notes->isVisible()) {
        ui->dockWidget_notes->hide();
    } else {
        ui->dockWidget_notes->show();
    }
    note = new notes(homeDataPath + "notes.xml");
    note->loadNotes();
    note->readNotes();
    ui->listWidget_notes->clear();
    QStringList id = note->getIDList();
    QStringList titles;
    for (int i = 0; i < id.size(); i++) {
        if (note->getType(id.at(i)) == "text")
            titles << note->getTitle(id.at(i));
    }
    ui->listWidget_notes->insertItems(0, titles);
    currentNoteID = "";
}
void MainWindow::showNote(QListWidgetItem *item)
{
    int id = ui->listWidget_notes->row(item);
    if (id >= 0 && id < note->getIDList().size()) {
        showNote(note->getIDList().at(id));
    }
}
void MainWindow::showNote(const QString &noteID)
{
    DEBUG_FUNC_NAME
    note->setData(currentNoteID, ui->textEdit_note->toHtml());
    note->setTitle(currentNoteID, ui->lineEdit_note_titel->text());
    note->setRef(currentNoteID, currentNoteRef);
    note->saveNotes();
    if (!note->getIDList().contains(noteID)) {
        myDebug() << "invalid noteID = " << noteID;
        return;
    }
    currentNoteID = noteID;
    myDebug() << "id = " << noteID << " currentNoteID = " << currentNoteID;
    ui->lineEdit_note_titel->setText(note->getTitle(currentNoteID));
    ui->textEdit_note->setHtml(note->getData(currentNoteID));
    currentNoteRef = note->getRef(currentNoteID);
    myDebug() << "link = " << note->getRef(currentNoteID, "link");
    if (!note->getRef(currentNoteID, "link").isEmpty())
        ui->label_noteLink->setText(notePos2Text(note->getRef(currentNoteID, "link")));
    else
        ui->label_noteLink->setText("");

}

void MainWindow::copyNote(void)
{
    DEBUG_FUNC_NAME

    int id = ui->listWidget_notes->currentRow();
    if (id < note->getIDList().size() && id >= 0) {
        QClipboard *clipboard = QApplication::clipboard();
        QTextDocument doc;
        doc.setHtml(note->getData(note->getIDList().at(id)));
        clipboard->setText(doc.toPlainText());
    } else {
        myDebug() << "no note";
    }
}
void MainWindow::saveNote(void)
{
    DEBUG_FUNC_NAME
    myDebug() << "currentNoteID = " << currentNoteID;
    note->setData(currentNoteID, ui->textEdit_note->toHtml());
    note->setTitle(currentNoteID, ui->lineEdit_note_titel->text());
    note->setRef(currentNoteID, currentNoteRef);
    note->saveNotes();
    reloadNotes();
}
void MainWindow::newNote(void)
{
    DEBUG_FUNC_NAME
    saveNote();
    reloadNotes();
    QString newID = note->generateNewID();
    note->setData(newID, "");
    note->setTitle(newID, tr("(unnamed)"));
    note->setType(newID, "text");
    QMap<QString, QString> ref;
    currentNoteRef = ref;
    //m["link"] = "";
    note->setRef(newID, ref);
    note->insertID(newID);
    currentNoteID = newID;
    reloadNotes();
    ui->listWidget_notes->setCurrentRow(note->getIDList().size() - 1);
    //showNote(ui->listWidget_notes->currentItem());

    ui->label_noteLink->setText("");
    ui->lineEdit_note_titel->setText(tr("(unnamed)"));
    ui->textEdit_note->setHtml("");
    if (!note->getRef(currentNoteID, "link").isEmpty())
        ui->label_noteLink->setText(notePos2Text(note->getRef(currentNoteID, "link")));
    else
        ui->label_noteLink->setText("");

    myDebug() << " newID = " << newID << " currentNoteID = " << currentNoteID;
}
void MainWindow::newNoteWithLink()
{
    DEBUG_FUNC_NAME
    QTextCursor cursor = currentTextCursor;
    int startverse = verseFromCursor(cursor);
    QString link;
    link = bibleDirName[m_bible.currentBibleID] + ";" + QString::number(m_bible.currentBookID, 10) + ";" + QString::number(m_bible.currentChapterID + 1 - m_bible.chapterAdd, 10) + ";" + QString::number(startverse, 10) + ";" + m_bible.bookFullName.at(m_bible.currentBookID);

    saveNote();
    reloadNotes();
    QString newID = note->generateNewID();
    note->setData(newID, "");
    note->setTitle(newID, tr("(unnamed)"));
    note->setType(newID, "text");
    QMap<QString, QString> ref;
    ref["link"] = link;
    currentNoteRef = ref;
    note->setRef(newID, ref);
    note->insertID(newID);
    currentNoteID = newID;
    reloadNotes();
    ui->listWidget_notes->setCurrentRow(note->getIDList().size() - 1);
    //showNote(ui->listWidget_notes->currentItem());
    ui->label_noteLink->setText("");
    ui->lineEdit_note_titel->setText(tr("(unnamed)"));
    ui->textEdit_note->setHtml("");
    if (!note->getRef(currentNoteID, "link").isEmpty())
        ui->label_noteLink->setText(notePos2Text(note->getRef(currentNoteID, "link")));
    else
        ui->label_noteLink->setText("");
}
void MainWindow::removeNote(void)
{
    DEBUG_FUNC_NAME
    note->removeNote(currentNoteID);
    ui->lineEdit_note_titel->setText(tr(""));
    ui->textEdit_note->setHtml("");
    ui->label_noteLink->setText("");
    reloadNotes();
}
void MainWindow::reloadNotes(void)
{
    DEBUG_FUNC_NAME
    ui->listWidget_notes->clear();
    QStringList id = note->getIDList();
    QStringList titles;
    myDebug() << " idList = " << id;
    for (int i = 0; i < id.size(); i++) {
        if (note->getType(id.at(i)) == "text") {
            myDebug() << " id.at(i) = " << id.at(i) << " title = " << note->getTitle(id.at(i));
            titles << note->getTitle(id.at(i));
        }
    }
    ui->listWidget_notes->insertItems(0, titles);
    ui->listWidget_notes->setCurrentRow(id.lastIndexOf(currentNoteID));
}
void MainWindow::notesContextMenu(void)
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
void MainWindow::noteSetTextBold(void)
{
    if (ui->textEdit_note->fontWeight() == QFont::Bold) {
        ui->textEdit_note->setFontWeight(QFont::Normal);
    } else {
        ui->textEdit_note->setFontWeight(QFont::Bold);
    }
}
void MainWindow::noteSetTextItalic(void)
{
    if (ui->textEdit_note->fontItalic()) {
        ui->textEdit_note->setFontItalic(false);
    } else {
        ui->textEdit_note->setFontItalic(true);
    }
}
void MainWindow::noteSetTextUnderline(void)
{
    if (ui->textEdit_note->fontUnderline()) {
        ui->textEdit_note->setFontUnderline(false);
    } else {
        ui->textEdit_note->setFontUnderline(true);
    }
}
void MainWindow::noteSetTextColor(void)
{
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid()) {
        ui->textEdit_note->setTextColor(color);
    }
}

QString MainWindow::notePos2Text(const QString &pos)
{
    DEBUG_FUNC_NAME

    myDebug() << "start pos = " << pos;
    QString string = "";
    QStringList list = pos.split(";");
    if (list.size() < 5) {
        myDebug() << "invalid pos";
        return "";
    }
    QString dirname = list.at(0);
    QString sbookID = list.at(1);
    QString schapterID = list.at(2);
    QString sverseID = list.at(3);
    QString bookName = list.at(4);
    int bibleID = 0;
    int chapterID = schapterID.toInt();
    int verseID = sverseID.toInt();
    int bookID = sbookID.toInt();
    //get bibleID
    myDebug() << "get bibleID";
    for (int i = 0; i < bibleDirName.size(); i++) {
        if (bibleDirName.at(i) == dirname) {
            bibleID = i;
            break;
        }
    }
    //load bible id

    myDebug() << " generate string with bibleID= " << bibleID ;
    QString link =  "bible://" + QString::number(bibleID) + "/" + QString::number(bookID) + "," + QString::number(chapterID - 1) + "," + QString::number(verseID - 1);
    string =  bookName + " " + QString::number(chapterID, 10) + "," + QString::number(verseID, 10);
    return  "<a href=\"" + link + "\" > " + string + "</a>";
}
void MainWindow::editNoteLink()
{
    DEBUG_FUNC_NAME
    /*int id = ui->listWidget_notes->currentRow();
    QString pos = note->notesPos.at(id);

    QStringList list = pos.split(";");
    if (list.size() < 5) {
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
    for (int i = 0; i < bibleDirName.size(); i++) {
        if (bibleDirName.at(i) == dirname) {
            bibleID = i;
            break;
        }
    }
    posChoser *pChoser = new posChoser(this);
    //connect( pChoser, SIGNAL( searched( QString,bool,bool,bool ) ), this, SLOT( showSearchResults( QString,bool,bool,bool ) ) );
    connect(pChoser, SIGNAL(updated(QString)), this, SLOT(updateNote(QString)));
    qDebug() << "MainWindow::editNoteLink() m_bible.chapterData.size() = " << m_bible.chapterData.size(),
    pChoser->setData(bibles, m_bible.bookFullName);
    pChoser->setCurrent(bibleID, dirname, bookID, chapterID, verseID);
    pChoser->show();
    pChoser->exec();*/
}
void MainWindow::updateNote(QString link)
{
    DEBUG_FUNC_NAME
    //  qDebug() << "MainWindow::updateBookmark() pos = " << pos;
    currentNoteRef["link"] = link;
    showNote(ui->listWidget_notes->currentItem());
    return;
}
