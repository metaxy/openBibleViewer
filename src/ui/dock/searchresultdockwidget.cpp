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
#include <QtGui/QMessageBox>
#include "searchresultdockwidget.h"
#include "ui_searchresultdockwidget.h"
#include "src/core/searchhit.h"
#include "src/ui/dialog/searchinfodialog.h"
#include "src/core/dbghelper.h"
#include "src/core/core.h"
SearchResultDockWidget::SearchResultDockWidget(QWidget *parent) :
    DockWidget(parent),
    ui(new Ui::SearchResultDockWidget)
{
    ui->setupUi(this);
    connect(ui->listWidget_search, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(goToSearchResult(QListWidgetItem *)));
    connect(ui->pushButton_searchInfo, SIGNAL(clicked()), this, SLOT(searchInfo()));
    ui->pushButton_searchInfo->setDisabled(true);
}

SearchResultDockWidget::~SearchResultDockWidget()
{
    delete ui;
}
void SearchResultDockWidget::setSearchResult(SearchResult searchResult)
{
    m_searchResult = searchResult;

    ui->label_search->setText(tr("Search: %1").arg(searchResult.searchQuery.searchText));
    QStringList outlist;
    QList<SearchHit> hits = searchResult.hits(SearchHit::BibleHit);
    for(int i = 0; i < hits.size(); ++i) {
        SearchHit hit = hits.at(i);
        QString bookn = m_moduleManager->bible()->bookName(hit.value(SearchHit::BookID).toInt());
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

    if(id < m_searchResult.hits().size() && id >= 0) {
        SearchHit hit = m_searchResult.hits().at(id);
        if(!m_moduleManager->contains(hit.value(SearchHit::BibleID).toInt()))
            return;
        emit get("bible://" + hit.value(SearchHit::BibleID).toString() + "/" + hit.value(SearchHit::BookID).toString() + "," + hit.value(SearchHit::ChapterID).toString() + "," + hit.value(SearchHit::VerseID).toString() + ",searchInCurrentText=true");
    }
}
void SearchResultDockWidget::searchInfo()
{
    if(!m_moduleManager->contains(m_moduleManager->bible()->moduleID())) {
        QMessageBox::information(0, tr("Error"), tr("No search information available."));
        return;
    }

    const QStringList bookNames = m_moduleManager->bible()->bookFullNames();//todo: check
    SearchResult result = m_searchResult;
    QList<SearchHit> list = result.hits(SearchHit::BibleHit);

    QStringList textList;
    for(int i = 0; i < list.size(); ++i) {
        SearchHit hit = list.at(i);
        QString bookn = m_moduleManager->bible()->bookName(hit.value(SearchHit::BookID).toInt()); //todo: maybe the bible isn't loaded and you need another bookNames
        textList << hit.value(SearchHit::VerseText).toString() + "\n - <i>" + bookn + " " + QString::number(hit.value(SearchHit::ChapterID).toInt() + 1) + " , " + QString::number(hit.value(SearchHit::VerseID).toInt() + 1) + "</i>";
    }

    SearchInfoDialog sDialog;
    sDialog.show();

    sDialog.setInfo(result, bookNames, m_searchResult.searchQuery.searchText, textList);
    sDialog.exec();

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
        myWarning() << "no search Results available";
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
