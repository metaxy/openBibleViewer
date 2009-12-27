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
#include "../searchdialog.h"
#include "../searchinfodialog.h"
#include "../../core/dbghelper.h"
#include "../../core/searchquery.h"
#include "../../core/searchresult.h"
#include "../../core/searchhit.h"
#include "ui_mainwindow.h"

#include <QtCore/QString>
#include <QtCore/QtDebug>
#include <QtCore/QTextStream>
#include <QtGui/QAction>
#include <QtGui/QTreeView>
#include <QtGui/QProgressDialog>
#include <QtGui/QTextBrowser>
#include <QtGui/QClipboard>
#include <QtGui/QMessageBox>
#include <QtGui/QTextCursor>


int MainWindow::search(void)
{
    DEBUG_FUNC_NAME
    SearchDialog *sDialog = new SearchDialog(this);
    connect(sDialog, SIGNAL(searched(SearchQuery)), this, SLOT(showSearchResults(SearchQuery)));
    if (activeMdiChild()) {
        QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
        if (textBrowser->textCursor().hasSelection() == true)//etwas ist markiert
            sDialog->setText(textBrowser->textCursor().selectedText());
    }
    sDialog->show();
    return 0;
}
int MainWindow::showSearchResults(SearchQuery query)
{
    DEBUG_FUNC_NAME
    ui->dockWidget_search->show();
    lastsearch = query.searchText;
    searchInCurrentText(query.searchText);
    ui->label_search->setText(tr("Search: %1").arg(query.searchText));
    QStringList outlist;
    if (biblesTypes.size() < m_bible.bibleID())
        return 1;
    SearchResult result;
    result = m_bible.search(query);
    m_bible.lastSearchResult = result;
    m_windowCache.setBible(m_bible);
    QList<SearchHit> hits = result.hits();
    for (int i = 0; i < hits.size(); ++i) {
        SearchHit hit = hits.at(i);
        QString bookn = m_bible.bookFullName.at(hit.bookID());
        outlist << bookn + " " + QString::number(hit.chapterID()) + " , " + QString::number(hit.verseID());
    }
    ui->listWidget_search->clear();
    ui->listWidget_search->insertItems(0, outlist);
    if (outlist.size() > 0) {
        ui->actionNextVerse->setEnabled(true);
        ui->actionLastVerse->setEnabled(true);
    }
    return 0;
}
int MainWindow::goToSearchResult(QListWidgetItem * item)
{
    int id = ui->listWidget_search->row(item);
    if (biblesTypes.size() < m_bible.bibleID())
        return 1;
    if (id < m_bible.lastSearchResult.hits().size() /*&& id < m_bible.st.chapter.size()*/) {
        SearchHit hit = m_bible.lastSearchResult.hits().at(id);
        emit get("bible://" + QString::number(hit.bibleID()) + "/" + QString::number(hit.bookID()) + "," + QString::number(hit.chapterID() - 1) + "," + QString::number(hit.verseID() - 1) + ",searchInCurrentText=true");
    }
    return 0;
}

void MainWindow::searchInfo()
{
    SearchResult result;
    QStringList bookNames;
    QString searchString;
    if (biblesTypes.size() < m_bible.bibleID()) {
        QMessageBox::information(0, tr("Error"), tr("No search information available."));
        return;
    }
    QStringList textList;

    bookNames = m_bible.bookFullName;
    result = m_bible.lastSearchResult;
    for (int i = 0; i < result.hits().size(); ++i) {
        SearchHit hit = result.hits().at(i);
        QString bookn = m_bible.bookFullName.at(hit.bookID());
        textList << hit.text() + "\n - <i>" + bookn + " " + QString::number(hit.chapterID()) + " , " + QString::number(hit.verseID()) + "</i>";
    }
    searchString = m_bible.lastSearchQuery.searchText;

    SearchInfoDialog sDialog;
    sDialog.show();

    sDialog.setInfo(result, bookNames, searchString, textList);
    sDialog.exec();

}
void MainWindow::searchInCurrentText(QString searchtext)
{
    qDebug() << "MainWindow::searchInCurrentText() searchtext = " << searchtext;
    /*QString text;
    if(biblesTypes.size() < currentBibleID)
        return;

    text = m_bible.lastout;
    int count = 0;
    int lastindex = text.indexOf(searchtext,0,Qt::CaseInsensitive);
    QString ftext="<b><font color=\"#00C000\">",ntext="</font></b>";
    while( lastindex != -1)
    {
        count++,
        //qDebug("MainWindow::showSearchResults( QString ) lastindex= %i",lastindex);
        text.insert(lastindex+searchtext.size(),ntext);
        text.insert(lastindex,ftext);
        lastindex = text.indexOf(searchtext,lastindex+searchtext.size()+ftext.size()+ntext.size(),Qt::CaseInsensitive);
    }

    showText(text);*/
    /*if (activeMdiChild())
    {
        QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
        //m_bible.st.
        textBrowser->find(searchtext);
    }*/
    //  qDebug() << "MainWindow::searchInCurrentText() count = " << count << " ( end )";
    return ;
}
void MainWindow::nextVerse()
{
    if (ui->listWidget_search->count() != 0) {
        int currentID = ui->listWidget_search->currentRow();

        int nextID = currentID + 1;
        if (nextID < ui->listWidget_search->count()) {
            ui->listWidget_search->setCurrentRow(nextID);
            goToSearchResult(ui->listWidget_search->currentItem());
        } else {
            //wieder von vorne
            //todo: show info
            nextID = 0;
            ui->listWidget_search->setCurrentRow(nextID);
            goToSearchResult(ui->listWidget_search->currentItem());
        }
    } else {
        myDebug() << "no search Results available";
    }
    return;
}
void MainWindow::lastVerse()
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
