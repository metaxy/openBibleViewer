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
#include "searchresultdockwidget.h"
#include <QtGui/QMessageBox>
#include <QtCore/QPointer>
#include "ui_searchresultdockwidget.h"
#include "src/core/search/searchhit.h"
#include "src/ui/dialog/searchinfodialog.h"
#include "src/core/dbghelper.h"
#include "src/core/moduletools.h"
#include "src/core/link/verseurl.h"
SearchResultDockWidget::SearchResultDockWidget(QWidget *parent) :
    DockWidget(parent),
    ui(new Ui::SearchResultDockWidget)
{
    ui->setupUi(this);
    connect(ui->listWidget_search, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(goToSearchResult(QListWidgetItem *)));
    connect(ui->pushButton_searchInfo, SIGNAL(clicked()), this, SLOT(searchInfo()));
    ui->pushButton_searchInfo->setDisabled(true);
    m_searchResult = NULL;

}

SearchResultDockWidget::~SearchResultDockWidget()
{
    delete ui;
}
void SearchResultDockWidget::setSearchResult(SearchResult *searchResult)
{
    m_searchResult = searchResult;

    ui->label_search->setText(tr("Search: %1").arg(searchResult->searchQuery.searchText));
    QStringList outlist;
    const QList<SearchHit> hits = searchResult->hits(SearchHit::BibleHit);
    foreach(const SearchHit & hit, hits) {
        QSharedPointer<Versification> v11n = m_settings->getV11N(hit.value(SearchHit::ModuleID).toInt());
        const QString bookn = v11n->bookName(hit.value(SearchHit::BookID).toInt());
        outlist << bookn + " " + QString::number(hit.value(SearchHit::ChapterID).toInt() + 1) + " , " +
                QString::number(hit.value(SearchHit::VerseID).toInt() + 1);
    }
    ui->listWidget_search->clear();
    ui->listWidget_search->insertItems(0, outlist);
    ui->pushButton_searchInfo->setDisabled(false);
}
void SearchResultDockWidget::goToSearchResult(QListWidgetItem * item)
{

    int id = ui->listWidget_search->row(item);

    if(id < m_searchResult->hits().size() && id >= 0) {
        SearchHit hit = m_searchResult->hits().at(id);
        if(!m_moduleManager->contains(hit.value(SearchHit::ModuleID).toInt()))
            return;

        VerseUrlRange range;
        range.setModule(hit.value(SearchHit::ModuleID).toInt());
        range.setBook(hit.value(SearchHit::BookID).toInt());
        range.setChapter(hit.value(SearchHit::ChapterID).toInt());
        range.setStartVerse(hit.value(SearchHit::VerseID).toInt());

        VerseUrl url(range);
        url.setParam("searchInCurrentText", "true");
        m_actions->get(url);
    }
}
void SearchResultDockWidget::searchInfo()
{
    SearchResult *result = m_searchResult;
    if(result == NULL) {
        QMessageBox::information(0, tr("Error"), tr("No search information available."));
        return;
    }

    const QList<SearchHit> list = result->hits(SearchHit::BibleHit);
    QSharedPointer<Versification> v11n_t;
    QStringList textList;
    foreach(const SearchHit & hit, list) {
        if(hit.type() == SearchHit::BibleHit) {
            QSharedPointer<Versification> v11n = m_settings->getV11N(hit.value(SearchHit::ModuleID).toInt());
            v11n_t = v11n;
            const QString bookn = v11n->bookName(hit.value(SearchHit::BookID).toInt());
            textList << hit.value(SearchHit::VerseText).toString() + "\n - <i>" + bookn
                     + " " + QString::number(hit.value(SearchHit::ChapterID).toInt() + 1)
                     + " , " + QString::number(hit.value(SearchHit::VerseID).toInt() + 1) + "</i>";
        }
    }

    QPointer<SearchInfoDialog> sDialog = new SearchInfoDialog(this);

    sDialog->setInfo(result, v11n_t, m_searchResult->searchQuery.searchText, textList);
    sDialog->exec();
    delete sDialog;

}
void SearchResultDockWidget::nextVerse()
{
    if(ui->listWidget_search->count() != 0) {
        int currentID = ui->listWidget_search->currentRow();

        int nextID = currentID + 1;
        if(nextID < ui->listWidget_search->count()) {
            ui->listWidget_search->setCurrentRow(nextID);
            goToSearchResult(ui->listWidget_search->currentItem());
        } else {
            //wieder von vorne
            QMessageBox::information(this, tr("No more Results."), tr("Show the first result again?"));
            nextID = 0;
            ui->listWidget_search->setCurrentRow(nextID);
            goToSearchResult(ui->listWidget_search->currentItem());
        }
    } else {
        myWarning() << "no search Results available";
    }
    return;
}
void SearchResultDockWidget::previousVerse()
{
    if(ui->listWidget_search->count() != 0) {
        int currentID = ui->listWidget_search->currentRow();

        int nextID = currentID - 1;
        if(nextID < ui->listWidget_search->count() && nextID >= 0) {
            ui->listWidget_search->setCurrentRow(nextID);
            goToSearchResult(ui->listWidget_search->currentItem());
        }
    } else {
        myWarning() << "no search results available";
    }
    return;
}
void SearchResultDockWidget::changeEvent(QEvent *e)
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
