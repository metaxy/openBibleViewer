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
#include "../../core/stelle.h"
#include "../../core/dbghelper.h"
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
    if (!activeMdiChild())
        return 1;
    QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
    searchDialog *sDialog = new searchDialog(this);
    connect(sDialog, SIGNAL(searched(QString, bool, bool, bool)), this, SLOT(showSearchResults(QString, bool, bool, bool)));
    if (textBrowser->textCursor().hasSelection() == true)//etwas ist markiert
        sDialog->setText(textBrowser->textCursor().selectedText());
    sDialog->show();
    return 0;
}
int MainWindow::showSearchResults(QString searchtext, bool regexp, bool whole, bool casesen)
{
    DEBUG_FUNC_NAME
    ui->dockWidget_search->show();
    //im zurzeit angezeigten text suchen
    lastsearch = searchtext;
    searchInCurrentText(lastsearch);
    ui->label_search->setText(tr("Searchstring: %1").arg(searchtext));
    QStringList outlist;
    if (biblesTypes.size() < m_bible.currentBibleID)
        return 1;
    struct stelle st;
    st = m_bible.search(searchtext, regexp, whole, casesen);
    m_bible.st = st;

    m_windowCache.setBible(m_bible);
    myDebug() << "results = " << st.book.size();
    for (int i = 0; i < st.book.size(); i++) {
        QString bookn = m_bible.bookFullName.at(st.book.at(i));
        outlist << bookn + " " + QString::number(st.chapter.at(i), 10) + " , " + QString::number(st.verse.at(i), 10);
    }
    ui->listWidget_search->clear();
    ui->listWidget_search->insertItems(0, outlist);
    ui->actionNextVerse->setEnabled(true);
    ui->actionLastVerse->setEnabled(true);
    return 0;
}
int MainWindow::goToSearchResult(QListWidgetItem * item)
{
    int id = ui->listWidget_search->row(item);
    if (biblesTypes.size() < m_bible.currentBibleID)
        return 1;
    if (id < m_bible.st.book.size() && id < m_bible.st.chapter.size()) {
        emit get("bible://" + QString::number(m_bible.st.bibleID) + "/" + QString::number(m_bible.st.book.at(id)) + "," + QString::number(m_bible.st.chapter.at(id) - 1) + "," + QString::number(m_bible.st.verse.at(id) - 1) + ",searchInCurrentText=true");
    }
    return 0;
}

void MainWindow::searchInfo()
{
    struct stelle st;
    QStringList bookNames;
    QString searchString;
    if (biblesTypes.size() < m_bible.currentBibleID) {
        QMessageBox::information(0, tr("Error"), tr("No search information available."));
        return;
    }
    QStringList textList;

    myDebug() << "textList.size() = " << textList.size();
    bookNames = m_bible.bookFullName;
    st = m_bible.st;
    for (int i = 0; i < st.book.size(); ++i) {
        QString bookn = m_bible.bookFullName.at(st.book.at(i));
        textList << st.text.at(i) + "\n - <i>" + bookn + " " + QString::number(st.chapter.at(i), 10) + " , " + QString::number(st.verse.at(i), 10) + "</i>";
    }
    searchString = m_bible.lastSearchString;

    searchInfoDialog sDialog;
    sDialog.show();

    sDialog.setInfo(st, bookNames, searchString, textList);
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
