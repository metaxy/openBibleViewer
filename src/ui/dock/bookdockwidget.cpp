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
#include "src/core/bible/bibleurl.h"
BookDockWidget::BookDockWidget(QWidget *parent) :
    DockWidget(parent),
    ui(new Ui::BookDockWidget)
{
    ui->setupUi(this);
    connect(ui->listView_books, SIGNAL(activated(QModelIndex)), this, SLOT(readBook(QModelIndex)));


    m_bookModel = new QStandardItemModel(this);
    m_chapterModel = new QStandardItemModel(this);

    m_bookSelection =  new QItemSelectionModel(m_bookModel);
    m_chapterSelection = new QItemSelectionModel(m_chapterModel);
    connect(m_chapterSelection, SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(readChapter()));

    ui->listView_books->setModel(m_bookModel);
    ui->listView_chapters->setModel(m_chapterModel);

    ui->listView_books->setSelectionModel(m_bookSelection);
    ui->listView_chapters->setSelectionModel(m_chapterSelection);
}
void BookDockWidget::init()
{
    connect(m_actions, SIGNAL(_setCurrentBook(QSet<int>)), this, SLOT(setCurrentBook(QSet<int>)));
    connect(m_actions, SIGNAL(_setCurrentChapter(QSet<int>)), this, SLOT(setCurrentChapter(QSet<int>)));

    connect(m_actions, SIGNAL(_clearBooks()), this, SLOT(clearBooks()));
    connect(m_actions, SIGNAL(_clearChapters()), this, SLOT(clearChapters()));

    connect(m_actions, SIGNAL(_updateBooks(Versification *)), this, SLOT(setBooks(Versification *)));
    connect(m_actions, SIGNAL(_updateChapters(QStringList)), this, SLOT(setChapters(QStringList)));
}
BookDockWidget::~BookDockWidget()
{
    delete ui;
}
void BookDockWidget::readBook(QModelIndex index)
{
    BibleUrl url;
    BibleUrlRange range;
    range.setBible(BibleUrlRange::LoadCurrentBible);
    range.setBook(index.data(Qt::UserRole + 1).toInt());
    range.setChapter(BibleUrlRange::LoadFirstChapter);
    range.setWholeChapter();
    url.addRange(range);
    m_actions->get(url);

}
void BookDockWidget::readChapter()
{
    BibleUrl url;
    foreach(const QModelIndex & i, m_chapterSelection->selectedIndexes()) {
        BibleUrlRange range;
        range.setBible(BibleUrlRange::LoadCurrentBible);
        range.setBook(BibleUrlRange::LoadCurrentBook);
        range.setChapter(i.data(Qt::UserRole + 1).toInt());
        range.setWholeChapter();
        url.addRange(range);
    }
    m_actions->get(url);
}
void BookDockWidget::setChapters(const QStringList &chapters)
{
    m_chapterModel->clear();
    for(int i = 0; i < chapters.size(); ++i) {
        QStandardItem *top = new QStandardItem;
        top->setText(chapters.at(i));
        top->setData(i, Qt::UserRole + 1);
        m_chapterModel->appendRow(top);
    }
}
void BookDockWidget::setBooks(Versification *v11n)
{
    m_bookModel->clear();
    QHashIterator<int, QString> i(v11n->bookNames());
    int count = 0;
    while(i.hasNext()) {
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

    const QModelIndexList list = m_bookModel->match(m_bookModel->index(0, 0), Qt::UserRole + 1, bookID, 1, Qt::MatchExactly);
    if(list.size() == 1) {
        m_bookSelection->clearSelection();
        m_bookSelection->setCurrentIndex(list.at(0), QItemSelectionModel::Select);
        ui->listView_books->scrollTo(list.at(0), QAbstractItemView::EnsureVisible);
    }

}
void BookDockWidget::setCurrentChapter(const int &chapterID)
{
    disconnect(m_chapterSelection, SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(readChapter()));
    const QModelIndexList list = m_chapterModel->match(m_chapterModel->index(0, 0), Qt::UserRole + 1, chapterID, 1, Qt::MatchExactly);
    if(list.size() == 1) {
        m_chapterSelection->clearSelection();
        m_chapterSelection->setCurrentIndex(list.at(0), QItemSelectionModel::Select);
        ui->listView_chapters->scrollTo(list.at(0), QAbstractItemView::EnsureVisible);
    }
    connect(m_chapterSelection, SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(readChapter()));

}

void BookDockWidget::setCurrentBook(const QSet<int> &bookID)
{
    //DEBUG_FUNC_NAME
    QModelIndexList sel;
    foreach(int b, bookID) {
        const QModelIndexList list = m_bookModel->match(m_bookModel->index(0, 0), Qt::UserRole + 1, b, 1, Qt::MatchExactly);
        if(list.size() == 1) {
            sel << list.at(0);
        }
    }
    if(!sel.isEmpty())  {
        m_bookSelection->clearSelection();
        m_bookSelection->setCurrentIndex(sel.at(0), QItemSelectionModel::Select);
        foreach(const QModelIndex & i, sel) {
            m_bookSelection->select(i, QItemSelectionModel::Select);
        }
        ui->listView_books->scrollTo(sel.at(0), QAbstractItemView::EnsureVisible);
    }

}
void BookDockWidget::setCurrentChapter(const QSet<int> &chapterID)
{
    //DEBUG_FUNC_NAME
    disconnect(m_chapterSelection, SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(readChapter()));

    QModelIndexList sel;
    foreach(int c, chapterID) {
        const QModelIndexList list = m_chapterModel->match(m_chapterModel->index(0, 0), Qt::UserRole + 1, c, 1, Qt::MatchExactly);
        if(list.size() == 1) {
            sel << list.at(0);
        }
    }
    if(!sel.isEmpty())  {
        m_chapterSelection->clearSelection();
        m_chapterSelection->setCurrentIndex(sel.at(0), QItemSelectionModel::Select);
        foreach(const QModelIndex & i, sel) {
            m_chapterSelection->select(i, QItemSelectionModel::Select);
        }
        ui->listView_chapters->scrollTo(sel.at(0), QAbstractItemView::EnsureVisible);
    }
    connect(m_chapterSelection, SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(readChapter()));

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
