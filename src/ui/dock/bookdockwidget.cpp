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
#include "bookdockwidget.h"
#include "ui_bookdockwidget.h"
#include "src/core/dbghelper.h"
BookDockWidget::BookDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::BookDockWidget)
{
    ui->setupUi(this);
    connect(ui->listWidget_books, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(readBook(QListWidgetItem *)));
    connect(ui->listWidget_chapters, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(readChapter(QListWidgetItem *)));
}

BookDockWidget::~BookDockWidget()
{
    delete ui;
}
void BookDockWidget::readBook(QListWidgetItem * item)
{
    //DEBUG_FUNC_NAME
    emit get("bible://current/" + QString::number(ui->listWidget_books->row(item)) + ",0,0");
}
void BookDockWidget::readChapter(QListWidgetItem * item)
{
    //DEBUG_FUNC_NAME
    int id = ui->listWidget_chapters->row(item);
    emit get("bible://current/" + QString::number(m_moduleManager->bible()->bookID()) + "," + QString::number(id) + ",0");
}
void BookDockWidget::setChapters(const QStringList &chapters)
{
    ui->listWidget_chapters->clear();
    ui->listWidget_chapters->insertItems(0, chapters);
}
void BookDockWidget::setBooks(const QStringList &books)
{
    //DEBUG_FUNC_NAME
    ui->listWidget_books->clear();
    ui->listWidget_books->insertItems(0, books);
}
void BookDockWidget::setCurrentBook(const int &bookID)
{
    ui->listWidget_books->setItemSelected(ui->listWidget_books->item(bookID), true);
    ui->listWidget_books->scrollToItem(ui->listWidget_books->item(bookID));
}
void BookDockWidget::setCurrentChapter(const int &chapterID)
{
    myDebug() << "chapterID = " << chapterID;
    ui->listWidget_chapters->setItemSelected(ui->listWidget_chapters->item(chapterID), true);
    ui->listWidget_chapters->scrollToItem(ui->listWidget_chapters->item(chapterID));
}
void BookDockWidget::changeEvent(QEvent *e)
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
