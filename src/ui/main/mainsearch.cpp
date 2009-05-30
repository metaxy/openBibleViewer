/***************************************************************************
openBibleViewer - Free Bibel Viewer
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


int MainWindow::search( void )
{
	qDebug("MainWindow::search( void ) start");


	if (!activeMdiChild())
		return 1;
	QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
	searchDialog *sDialog = new searchDialog(this);
	connect( sDialog, SIGNAL( searched( QString,bool,bool,bool ) ), this, SLOT( showSearchResults( QString,bool,bool,bool ) ) );
	if(textBrowser->textCursor().hasSelection() == true)//etwas ist markiert
		sDialog->setText(textBrowser->textCursor().selectedText());
	sDialog->show();
	qDebug("MainWindow::search( void ) end");
	return 0;
}
int MainWindow::showSearchResults( QString searchtext,bool regexp,bool whole,bool casesen)
{
	qDebug() << " MainWindow::showSearchResults start";
	ui->dockWidget_search->show();
	//im zurzeit angezeigten text suchen
	lastsearch = searchtext;
	searchInCurrentText(lastsearch);
	ui->label_search->setText(tr("Searchstring: %1").arg(searchtext));
	QStringList outlist;
	if(biblesTypes.size() < currentBibleID)
		return 1;
	struct stelle st;
	st = b.search(searchtext,regexp,whole,casesen);
	b.st = st;

	tcache.setBible(b);
	qDebug() << " MainWindow::showSearchResults results = " << st.book.size();
	for(int i=0;i<st.book.size();i++)
	{
		QString bookn = b.bookFullName.at(st.book.at(i));
		outlist << bookn+" "+QString::number(st.chapter.at(i),10)+" , "+QString::number(st.verse.at(i),10);
	}
	ui->listWidget_search->clear();
	ui->listWidget_search->insertItems(0,outlist);
	qDebug() << "MainWindow::showSearchResults end";
	return 0;
}
int MainWindow::goToSearchResult(QListWidgetItem * item)
{
	qDebug("MainWindow::goToSearchResult() currentBibleID= %i , searchedBibleID = %i",currentBibleID,b.st.bibleID);
	int id = ui->listWidget_search->row(item);
	if(biblesTypes.size() < currentBibleID)
		return 1;
	if(id < b.st.book.size() && id < b.st.chapter.size())
	{
		if(b.st.bibleID == currentBibleID)
		{
			qDebug() << "MainWindow::goToSearchResult() same bible";
			go(b.st.book.at(id),b.st.chapter.at(id),b.st.verse.at(id));
		}
		else
		{
			loadBibleDataByID(b.st.bibleID);
			go(b.st.book.at(id),b.st.chapter.at(id),b.st.verse.at(id));
		}

	}
	qDebug() << "MainWindow::goToSearchResult() end";
	return 0;
}
int MainWindow::go(int bookID,int chapterID,int verseID)
{
	qDebug() <<"MainWindow::go() book = " <<bookID << " chapter = "<<chapterID;

	setBooks(b.bookFullName);
	showBibleName(b.bibleName);
	setChapters(b.chapterNames);
	readBook(bookID);
	chapterText = b.chapterText;
	showChapter(chapterID -1 + b.chapterAdd,verseID-1);
	searchInCurrentText(lastsearch);
	qDebug() << "MainWindow::go() end";
	return 0;
}
void MainWindow::searchInfo()
{
	struct stelle st;
	QStringList bookNames;
	QString searchString;
	if(biblesTypes.size() < currentBibleID)
	{
		QMessageBox::information(0,QObject::tr("Error"),QObject::tr("No search information aviable"));
		return;
	}

	bookNames = b.bookFullName;
	st = b.st;
	searchString = b.lastSearchString;

	searchInfoDialog sDialog;
	sDialog.show();

	sDialog.setInfo(st,bookNames,searchString);
	sDialog.exec();

}
void MainWindow::searchInCurrentText(QString searchtext)
{
	qDebug() << "MainWindow::searchInCurrentText() searchtext = " << searchtext;
	QString text;
	if(biblesTypes.size() < currentBibleID)
		return;

	text = b.lastout;
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

	showText(text);
	qDebug() << "MainWindow::searchInCurrentText() count = " << count << " ( end )";
	return ;
}
void MainWindow::nextVerse()
{
	if( ui->listWidget_search->count() != 0)
	{
		int currentID = ui->listWidget_search->currentRow();

		int nextID = currentID+1;
		if( nextID < ui->listWidget_search->count())
		{
			ui->listWidget_search->setCurrentRow(nextID);
			goToSearchResult(ui->listWidget_search->currentItem());
		}
		else
		{
			//wieder von vorne
			nextID = 0;
			ui->listWidget_search->setCurrentRow(nextID);
			goToSearchResult(ui->listWidget_search->currentItem());
		}
	}
	else
	{
		qDebug() << "MainWindow::nextVerse() no search Results available";
	}
	return;
}
void MainWindow::lastVerse()
{
	if( ui->listWidget_search->count() != 0)
	{
		int currentID = ui->listWidget_search->currentRow();

		int nextID = currentID-1;
		if( nextID < ui->listWidget_search->count() && nextID >= 0)
		{
			ui->listWidget_search->setCurrentRow(nextID);
			goToSearchResult(ui->listWidget_search->currentItem());
		}
	}
	else
	{
		qDebug() << "MainWindow::nextVerse() no search Results available";
	}
	return;
}
