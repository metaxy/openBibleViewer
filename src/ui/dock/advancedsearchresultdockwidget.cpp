#include "advancedsearchresultdockwidget.h"
#include "ui_advancedsearchresultdockwidget.h"
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QMessageBox>
#include "src/core/bibleurl.h"
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
    m_itemModel->clear();
    m_searchResult = searchResult;
    ui->label_searchInfo->setText(tr("Search: %1").arg(searchResult.searchQuery.searchText));
    QStandardItem *parentItem = m_itemModel->invisibleRootItem();

    QList<SearchHit> hits = m_searchResult.hits();
    QMap<int, QStandardItem *> m_bookItems;
    foreach(SearchHit hit, hits) {
        if(hit.type() != SearchHit::BibleHit)
            continue;
        const int book = hit.value(SearchHit::BookID).toInt();
        if(m_bookItems.contains(book))
            continue;
        QStandardItem *bookItem = new QStandardItem(m_moduleManager->bible()->bookName(book));
        bookItem->setData("book",Qt::UserRole + 2);
        parentItem->appendRow(bookItem);
        m_bookItems.insert(book, bookItem);
    }
    QStandardItem *notesItem = 0;
    for(int i = 0; i < hits.size(); ++i) {
        SearchHit hit = hits.at(i);
        if(hit.type() == SearchHit::BibleHit) {
            QStandardItem *hitItem = new QStandardItem(m_moduleManager->bible()->bookName(hit.value(SearchHit::BookID).toInt()) + " " +
                                                       QString::number(hit.value(SearchHit::ChapterID).toInt() + 1) + ":" +
                                                       QString::number(hit.value(SearchHit::VerseID).toInt() + 1));
            hitItem->setData(i, Qt::UserRole + 1);
            hitItem->setData("bibleHit",Qt::UserRole + 2);
            m_bookItems.value(hit.value(SearchHit::BookID).toInt())->appendRow(hitItem);
        } else if(hit.type() == SearchHit::NoteHit) {
            if(!notesItem) {
                notesItem = new QStandardItem(tr("Notes"));
                parentItem->appendRow(notesItem);
            }
            QStandardItem *hitItem = new QStandardItem(m_notes->getTitle(hit.value(SearchHit::NoteID).toString()));
            hitItem->setData(i, Qt::UserRole + 1);
             hitItem->setData("noteHit",Qt::UserRole + 2);
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
            BibleUrl url;
            url.setBibleID(hit.value(SearchHit::BibleID).toInt());
            url.setBookID(hit.value(SearchHit::BookID).toInt());
            url.setChapterID(hit.value(SearchHit::ChapterID).toInt());
            url.setVerseID(hit.value(SearchHit::VerseID).toInt());
            url.setParam("searchInCurrentText","true");
            emit get(url.toString());
        } else if(hit.type() == SearchHit::NoteHit) {
            emit get("note://" + hit.value(SearchHit::NoteID).toString());
        }
    }
}
void AdvancedSearchResultDockWidget::searchInfo()
{
    if(!m_moduleManager->contains(m_moduleManager->bible()->moduleID())) {
        QMessageBox::information(0, tr("Error"), tr("No search information available."));
        return;
    }

    const QStringList bookNames = m_moduleManager->bible()->bookNames().values();//todo: check
    SearchResult result = m_searchResult;
    QList<SearchHit> list = result.hits(SearchHit::BibleHit);

    QStringList textList;
    for(int i = 0; i < list.size(); ++i) {
        SearchHit hit = list.at(i);
        if(hit.type() == SearchHit::BibleHit) {
            const QString bookn = m_moduleManager->bible()->bookName(hit.value(SearchHit::BookID).toInt()); //todo: maybe the bible isn't loaded and you need another bookNames
            textList << hit.value(SearchHit::VerseText).toString() + "\n - <i>" + bookn
                    + " " + QString::number(hit.value(SearchHit::ChapterID).toInt() + 1)
                    + " , " + QString::number(hit.value(SearchHit::VerseID).toInt() + 1) + "</i>";
        }
    }

    SearchInfoDialog sDialog;
    sDialog.show();

    sDialog.setInfo(result, bookNames, m_searchResult.searchQuery.searchText, textList);
    sDialog.exec();
}

void AdvancedSearchResultDockWidget::nextVerse()
{
    DEBUG_FUNC_NAME
    if(m_itemModel->rowCount() != 0) {
        const QModelIndex index = m_selectionModel->currentIndex();
        QModelIndex parent = index.parent();
        forever {
            if(parent.parent() != QModelIndex()) {
                parent = parent.parent();
            } else {
                break;
            }
        }
        int rowID = index.row();
        myDebug() << rowID << index << index.data(Qt::UserRole + 2);
        //todo: folder
        forever {
            if(rowID >= m_proxyModel->rowCount())
                break;
            rowID++;
            const QModelIndex n = m_proxyModel->index(rowID,0,parent);
            if(n.data(Qt::UserRole +2) == "bibleHit") {
                goToSearchResult(n);
                m_selectionModel->clearSelection();
                m_selectionModel->setCurrentIndex(n,QItemSelectionModel::Select);
                break;
            }

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
        QModelIndex parent = index.parent();
        forever {
            if(parent.parent() != QModelIndex()) {
                parent = parent.parent();
            } else {
                break;
            }
        }
        int rowID = index.row();
        myDebug() << rowID << index << index.data(Qt::UserRole +2);
        forever {
            if(rowID == 0)
                break;
            rowID--;
            const QModelIndex n = m_proxyModel->index(rowID,0,parent);
            if(n.data(Qt::UserRole + 2) == "bibleHit") {
                goToSearchResult(n);
                m_selectionModel->clearSelection();
                m_selectionModel->setCurrentIndex(n,QItemSelectionModel::Select);
                break;
            }

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
