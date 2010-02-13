#include "searchresultdockwidget.h"
#include "src/core/searchhit.h"
#include "ui_searchresultdockwidget.h"
#include <QtGui/QMessageBox>
#include "src/ui/dialog/searchinfodialog.h"
#include "src/core/dbghelper.h"
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
    QList<SearchHit> hits = searchResult.hits();
    for (int i = 0; i < hits.size(); ++i) {
        SearchHit hit = hits.at(i);
        QString bookn = m_moduleManager->m_bible.bookFullName.at(hit.bookID());
        outlist << bookn + " " + QString::number(hit.chapterID()) + " , " + QString::number(hit.verseID());
    }
    ui->listWidget_search->clear();
    ui->listWidget_search->insertItems(0, outlist);
    ui->pushButton_searchInfo->setDisabled(false);
}
void SearchResultDockWidget::goToSearchResult(QListWidgetItem * item)
{
    int id = ui->listWidget_search->row(item);
    if (m_moduleManager->m_moduleList.size() < m_moduleManager->m_bible.bibleID())
        return;
    if (id < m_searchResult.hits().size() && id > 0) {
        SearchHit hit = m_searchResult.hits().at(id);
        emit get("bible://" + QString::number(hit.bibleID()) + "/" + QString::number(hit.bookID()) + "," + QString::number(hit.chapterID() - 1) + "," + QString::number(hit.verseID() - 1) + ",searchInCurrentText=true");
    }
}
void SearchResultDockWidget::searchInfo()
{
    SearchResult result;
    QStringList bookNames;
    QString searchString;
    if (m_moduleManager->m_moduleList.size() < m_moduleManager->m_bible.bibleID()) {
        QMessageBox::information(0, tr("Error"), tr("No search information available."));
        return;
    }
    QStringList textList;

    bookNames = m_moduleManager->m_bible.bookFullName;
    result = m_moduleManager->m_bible.lastSearchResult;
    for (int i = 0; i < result.hits().size(); ++i) {
        SearchHit hit = result.hits().at(i);
        QString bookn = m_moduleManager->m_bible.bookFullName.at(hit.bookID());
        textList << hit.text() + "\n - <i>" + bookn + " " + QString::number(hit.chapterID()) + " , " + QString::number(hit.verseID()) + "</i>";
    }
    searchString = m_moduleManager->m_bible.lastSearchQuery.searchText;

    SearchInfoDialog sDialog;
    sDialog.show();

    sDialog.setInfo(result, bookNames, searchString, textList);
    sDialog.exec();

}
void SearchResultDockWidget::nextVerse()
{
    if (ui->listWidget_search->count() != 0) {
        int currentID = ui->listWidget_search->currentRow();

        int nextID = currentID + 1;
        if (nextID < ui->listWidget_search->count()) {
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
        myDebug() << "no search Results available";
    }
    return;
}
void SearchResultDockWidget::previousVerse()
{
    if (ui->listWidget_search->count() != 0) {
        int currentID = ui->listWidget_search->currentRow();

        int nextID = currentID - 1;
        if (nextID < ui->listWidget_search->count() && nextID >= 0) {
            ui->listWidget_search->setCurrentRow(nextID);
            goToSearchResult(ui->listWidget_search->currentItem());
        }
    } else {
        myDebug() << "no search Results available";
    }
    return;
}
void SearchResultDockWidget::changeEvent(QEvent *e)
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
