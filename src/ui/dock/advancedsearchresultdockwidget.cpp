/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "advancedsearchresultdockwidget.h"
#include "ui_advancedsearchresultdockwidget.h"
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QMessageBox>
#include "src/core/verse/verseurl.h"
#include "src/ui/dialog/searchinfodialog.h"
#include "src/core/dbghelper.h"
AdvancedSearchResultDockWidget::AdvancedSearchResultDockWidget(QWidget *parent) :
    DockWidget(parent),
    ui(new Ui::AdvancedSearchResultDockWidget)
{
    ui->setupUi(this);
    connect(ui->pushButton_searchInfo, SIGNAL(clicked()), this, SLOT(searchInfo()));
}

AdvancedSearchResultDockWidget::~AdvancedSearchResultDockWidget()
{
    delete ui;
}
void AdvancedSearchResultDockWidget::init()
{
    m_itemModel = new QStandardItemModel(this);

    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_itemModel);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_selectionModel = new QItemSelectionModel(m_proxyModel);

    //ui->treeView->setSortingEnabled(true);
    ui->treeView->setModel(m_proxyModel);
    ui->treeView->setSelectionModel(m_selectionModel);
    connect(ui->treeView, SIGNAL(activated(QModelIndex)), this, SLOT(goToSearchResult(QModelIndex)));

}
void AdvancedSearchResultDockWidget::setSearchResult(SearchResult searchResult)
{
    DEBUG_FUNC_NAME

    m_itemModel->clear();
    m_searchResult = searchResult;
    ui->label_searchInfo->setText(tr("Search: %1").arg(searchResult.searchQuery.searchText));
    QStandardItem *parentItem = m_itemModel->invisibleRootItem();

    QList<SearchHit> hits = m_searchResult.hits();
    QMap<int, QStandardItem *> m_bookItems;
    foreach(const SearchHit & hit, hits) {
        if(hit.type() != SearchHit::BibleHit)
            continue;
        const int book = hit.value(SearchHit::BookID).toInt();
        if(m_bookItems.contains(book))
            continue;
        QStandardItem *bookItem = new QStandardItem(m_moduleManager->verseModule()->versification()->bookName(book));
        bookItem->setData("book", Qt::UserRole + 2);
        parentItem->appendRow(bookItem);
        m_bookItems.insert(book, bookItem);
    }
    QStandardItem *notesItem = 0;
    for(int i = 0; i < hits.size(); ++i) {
        SearchHit hit = hits.at(i);
        if(hit.type() == SearchHit::BibleHit) {
            QStandardItem *hitItem = new QStandardItem(m_moduleManager->verseModule()->versification()->bookName(hit.value(SearchHit::BookID).toInt()) + " " +
                    QString::number(hit.value(SearchHit::ChapterID).toInt() + 1) + ":" +
                    QString::number(hit.value(SearchHit::VerseID).toInt() + 1));
            hitItem->setData(i, Qt::UserRole + 1);
            hitItem->setData("bibleHit", Qt::UserRole + 2);
            m_bookItems.value(hit.value(SearchHit::BookID).toInt())->appendRow(hitItem);
        } else if(hit.type() == SearchHit::NoteHit) {
            if(!notesItem) {
                notesItem = new QStandardItem(tr("Notes"));
                parentItem->appendRow(notesItem);
            }
            QStandardItem *hitItem = new QStandardItem(m_notes->getTitle(hit.value(SearchHit::NoteID).toString()));
            hitItem->setData(i, Qt::UserRole + 1);
            hitItem->setData("noteHit", Qt::UserRole + 2);
            notesItem->appendRow(hitItem);
        }
    }

    ui->pushButton_searchInfo->setDisabled(false);

}
void AdvancedSearchResultDockWidget::goToSearchResult(QModelIndex index)
{
    DEBUG_FUNC_NAME
    const int id = index.data(Qt::UserRole + 1).toInt();
    if(id < m_searchResult.hits().size() && id >= 0) {
        SearchHit hit = m_searchResult.hits().at(id);
        if(hit.type() == SearchHit::BibleHit) {
            if(!m_moduleManager->contains(hit.value(SearchHit::BibleID).toInt()))
                return;

            VerseUrl url;
            VerseUrlRange range;
            range.setModule(hit.value(SearchHit::BibleID).toInt());
            range.setBook(hit.value(SearchHit::BookID).toInt());
            range.setChapter(hit.value(SearchHit::ChapterID).toInt());
            range.setActiveVerse(hit.value(SearchHit::VerseID).toInt());
            range.setWholeChapter();
            url.addRange(range);
            url.setParam("searchInCurrentText", "true");
            m_actions->get(url);
        } else if(hit.type() == SearchHit::NoteHit) {
            m_actions->get("note://" + hit.value(SearchHit::NoteID).toString());
        }
    }
}
void AdvancedSearchResultDockWidget::searchInfo()
{
    if(!m_moduleManager->contains(m_moduleManager->verseModule()->moduleID())) {
        QMessageBox::information(0, tr("Error"), tr("No search information available."));
        return;
    }

    SearchResult result = m_searchResult;
    QList<SearchHit> list = result.hits(SearchHit::BibleHit);

    QStringList textList;
    for(int i = 0; i < list.size(); ++i) {
        SearchHit hit = list.at(i);
        if(hit.type() == SearchHit::BibleHit) {
            const QString bookn = m_moduleManager->verseModule()->versification()->bookName(hit.value(SearchHit::BookID).toInt()); //todo: maybe the bible isn't loaded and you need another bookNames
            //or we call it a feature
            textList << hit.value(SearchHit::VerseText).toString() + "\n - <i>" + bookn
                     + " " + QString::number(hit.value(SearchHit::ChapterID).toInt() + 1)
                     + " , " + QString::number(hit.value(SearchHit::VerseID).toInt() + 1) + "</i>";
        }
    }

    SearchInfoDialog sDialog;
    sDialog.show();

    sDialog.setInfo(result, m_moduleManager->verseModule()->versification(), m_searchResult.searchQuery.searchText, textList);
    sDialog.exec();
}

void AdvancedSearchResultDockWidget::nextVerse()
{
    if(m_itemModel->rowCount() != 0) {
        const QModelIndex index = m_selectionModel->currentIndex();
        const QModelIndex parent = index.parent();
        QModelIndex neededIndex;
        bool found = false;
        int rowID = index.row();
        while(!found) {
            if(rowID >= m_proxyModel->rowCount(parent)) {
                break;
            }
            rowID++;
            const QModelIndex n = m_proxyModel->index(rowID, 0, parent);
            if(n.data(Qt::UserRole + 2) == "bibleHit") {
                found = true;
                neededIndex = n;
                break;
            }
        }
        if(!found) {
            int rowID = parent.row();
            const QModelIndex parent2 = parent.parent();
            while(!found) {
                if(rowID >= m_proxyModel->rowCount(parent2)) {
                    break;
                }
                rowID++;
                const QModelIndex n = m_proxyModel->index(rowID, 0, parent2);
                if(m_proxyModel->hasChildren(n)) {
                    const QModelIndex n2 = m_proxyModel->index(0, 0, n); //the first
                    if(n2.data(Qt::UserRole + 2) == "bibleHit") {
                        found = true;
                        neededIndex = n2;
                        break;
                    }
                }
            }

        }

        if(found) {
            goToSearchResult(neededIndex);
            m_selectionModel->clearSelection();
            m_selectionModel->setCurrentIndex(neededIndex, QItemSelectionModel::Select);
        }

    } else {
        myWarning() << "no search Results available";
    }
    return;
}


void AdvancedSearchResultDockWidget::previousVerse()
{
    if(m_itemModel->rowCount() != 0) {
        const QModelIndex index = m_selectionModel->currentIndex();
        const QModelIndex parent = index.parent();
        QModelIndex neededIndex;
        bool found = false;
        int rowID = index.row();
        while(!found) {
            if(rowID == 0) {
                break;
            }
            rowID--;
            const QModelIndex n = m_proxyModel->index(rowID, 0, parent);
            //myDebug() << rowID << n.data();
            if(n.data(Qt::UserRole + 2) == "bibleHit") {
                found = true;
                neededIndex = n;
                break;
            }
        }
        if(!found) {
            int rowID = parent.row();
            const QModelIndex parent2 = parent.parent();
            while(!found) {
                if(rowID == 0) {
                    break;
                }
                rowID--;
                const QModelIndex n = m_proxyModel->index(rowID, 0, parent2);
                if(m_proxyModel->hasChildren(n)) {
                    //myDebug() << m_proxyModel->rowCount(n) - 1;
                    const QModelIndex n2 = m_proxyModel->index(m_proxyModel->rowCount(n) - 1, 0, n); //the last
                    if(n2.data(Qt::UserRole + 2) == "bibleHit") {
                        found = true;
                        neededIndex = n2;
                        break;
                    }
                }
            }
        }

        if(found) {
            goToSearchResult(neededIndex);
            m_selectionModel->clearSelection();
            m_selectionModel->setCurrentIndex(neededIndex, QItemSelectionModel::Select);
        }

    } else {
        myWarning() << "no search Results available";
    }
}
void AdvancedSearchResultDockWidget::changeEvent(QEvent *e)
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
