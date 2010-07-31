/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
#include "src/core/core.h"
BookDockWidget::BookDockWidget(QWidget *parent) :
    DockWidget(parent),
    ui(new Ui::BookDockWidget)
{
    ui->setupUi(this);
    connect(ui->listView_books,SIGNAL(activated(QModelIndex)),this,SLOT(readBook(QModelIndex)));
    connect(ui->listView_chapters,SIGNAL(activated(QModelIndex)),this,SLOT(readChapter(QModelIndex)));

    m_bookModel = new QStandardItemModel();
    m_chapterModel = new QStandardItemModel();

    m_bookSelection =  new QItemSelectionModel(m_bookModel);
    m_chapterSelection = new QItemSelectionModel(m_chapterModel);

    ui->listView_books->setModel(m_bookModel);
    ui->listView_chapters->setModel(m_chapterModel);

    ui->listView_books->setSelectionModel(m_bookSelection);
    ui->listView_chapters->setSelectionModel(m_chapterSelection);
}

BookDockWidget::~BookDockWidget()
{
    delete ui;
    delete m_bookModel;
    delete m_chapterModel;
}
void BookDockWidget::readBook(QModelIndex index)
{
    emit get("bible://current/" +  index.data(Qt::UserRole + 1).toString() + ",0,0");
}
void BookDockWidget::readChapter(QModelIndex index)
{
    emit get("bible://current/" + QString::number(m_moduleManager->bible()->bookID()) + "," + index.data(Qt::UserRole + 1).toString() + ",0");
}
void BookDockWidget::setChapters(const QStringList &chapters)
{
    m_chapterModel->clear();
    for(int i = 0; i < chapters.size(); ++i) {
        myDebug() << i << chapters.at(i);
        QStandardItem *top = new QStandardItem;
        top->setText(chapters.at(i));
        top->setData(i,Qt::UserRole +1);
        m_chapterModel->appendRow(top);
    }
}
void BookDockWidget::setBooks(const QHash<int, QString> &books)
{
    m_bookModel->clear();
    QHashIterator<int, QString> i(books);
    int count = 0;
    while (i.hasNext()) {
        i.next();
        QStandardItem *top = new QStandardItem;
        top->setText(i.value());
        top->setData(i.key(), Qt::UserRole + 1);
        m_bookModel->appendRow(top);
        count++;
    }
}
void BookDockWidget::clearBooks()
{
    m_bookModel->clear();
}
void BookDockWidget::clearChapters()
{
    m_chapterModel->clear();
}

void BookDockWidget::setCurrentBook(const int &bookID)
{
    QModelIndexList list = m_bookModel->match(m_bookModel->index(0,0), Qt::UserRole + 1, bookID);
    if(list.size() == 1) {
        m_bookSelection->clearSelection();
        m_bookSelection->setCurrentIndex(list.at(0), QItemSelectionModel::Select);
    }

}
void BookDockWidget::setCurrentChapter(const int &chapterID)
{
    DEBUG_FUNC_NAME
    myDebug() << chapterID;
    QModelIndexList list = m_bookModel->match(m_chapterModel->index(0,0), Qt::UserRole + 1, chapterID);
    if(list.size() == 1) {
        m_chapterSelection->clearSelection();
        m_chapterSelection->setCurrentIndex(list.at(0), QItemSelectionModel::Select);
    }
}
void BookDockWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
