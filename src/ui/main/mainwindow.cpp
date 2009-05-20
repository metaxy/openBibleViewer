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
#include <QDir>
#include <QTextCodec>
#include <QAction>
#include <QString>
#include <QPrintDialog>
#include <QPrinter>
#include <QTreeView>
#include <QProgressDialog>
#include <QFileDialog>
#include <QTextStream>
#include <QClipboard>
#include <QMessageBox>
#include <QtDebug>
#include <QFSFileEngine>
#include <QComboBox>
#include <QTranslator>
#include <QLocale>

#include "mainwindow.h"
#include "../searchdialog.h"
#include "../settingsdialog.h"
#include "../aboutdialog.h"
#include "../../kernel/stelle.h"
#include "../../kernel/chapter.h"
#include "ui_mainwindow.h"
//spilt MainWindow in some files
#include "mainnotes.cpp"
#include "mainbookmarks.cpp"
#include "mainsearch.cpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
	ui->setupUi(this);
        ui->mdiArea->installEventFilter(this);
        VERSION  = "0.2b1";
        BUILD =  "2009-5-10";

	#ifdef _PORTABLE_VERSION
		homeDataPath = QApplication::applicationDirPath()+"/";
		settings = new QSettings( homeDataPath+"openBibleViewer.ini",QSettings::IniFormat);
	#else
		homeDataPath = QApplication::applicationDirPath()+"/";
		#ifdef Q_WS_MAC
			 homeDataPath = QFSFileEngine::homePath() +"/.config/openbible/";
			 settings = new QSettings(homeDataPath+"openBibleViewer.ini",QSettings::IniFormat);
		#endif
		#ifdef Q_WS_X11
			 homeDataPath = QFSFileEngine::homePath() +"/.config/openbible/";
			 settings = new QSettings(homeDataPath+"openBibleViewer.ini",QSettings::IniFormat);
		#endif
		#ifdef Q_WS_WIN
			 homeDataPath = "%APPDATA%/openbible/";
			 settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
			"openBible", "openBibleViewer");
		#endif
	#endif
        qDebug() << "MainWindow::MainWindow() settingsPath = " << homeDataPath;



	set.encoding = "Windows-1251";
	set.dict = "/";
	set.zoomstep = 1;
	set.removeHtml = false;
	set.version = VERSION;
	set.build = BUILD;
	set.zoomstep = 0.2;
        set.autoLayout = 1;
        set.onClickBookmarkGo = true;

	QString appPath = QApplication::applicationDirPath();
	if(appPath.endsWith(set.dict))
	{
		appPath.remove(appPath.size()-1,10);
	}
	set.path << appPath;

	loadSettings();
	newMdiChild();

        connect( ui->mdiArea, SIGNAL( subWindowActivated ( QMdiSubWindow * )), this, SLOT(reloadWindow( QMdiSubWindow * )));
	connect( ui->treeWidget_bibles, SIGNAL(itemClicked( QTreeWidgetItem *, int)), this, SLOT( loadBibleData( QTreeWidgetItem* ) ) );
	connect( ui->listWidget_books, SIGNAL( itemClicked(QListWidgetItem *) ), this, SLOT( readBook(QListWidgetItem * ) ) );
	connect( ui->listWidget_chapters, SIGNAL( itemClicked(QListWidgetItem *) ), this, SLOT( readChapter(QListWidgetItem * ) ) );
	connect( ui->listWidget_search, SIGNAL( itemClicked(QListWidgetItem *) ), this, SLOT( goToSearchResult(QListWidgetItem * ) ) );
	connect( ui->listWidget_notes, SIGNAL( itemClicked(QListWidgetItem *) ), this, SLOT( showNote(QListWidgetItem * ) ) );

	//menu
	connect( ui->actionPrint, SIGNAL(triggered()), this, SLOT(printFile()));
	connect( ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
	connect( ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect( ui->actionSearch, SIGNAL(triggered()), this, SLOT(search()));
	connect( ui->actionNewTab, SIGNAL(triggered()), this, SLOT(myTileHorizontal()));
        connect( ui->actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
        connect( ui->actionSelectAll, SIGNAL(triggered()), this, SLOT(selectAll()));
	connect( ui->actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
	connect( ui->actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));
	connect( ui->actionNotes, SIGNAL(triggered()), this, SLOT(loadNotes()));
	connect( ui->actionBookmarks, SIGNAL(triggered()), this, SLOT(loadBookmarks()));
	connect( ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
	connect( ui->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
	connect( ui->actionNextVerse, SIGNAL(triggered()), this, SLOT(nextVerse()));
	connect( ui->actionLastVerse, SIGNAL(triggered()), this, SLOT(lastVerse()));

	connect( ui->actionVerticalTile, SIGNAL(triggered()), this, SLOT(myTileVertical()));
	connect( ui->actionHorizontalTile, SIGNAL(triggered()), this, SLOT(myTileHorizontal()));
	connect( ui->actionCascade, SIGNAL(triggered()), this, SLOT(myCascade()));

	connect( ui->tactionSearch, SIGNAL(triggered()), this, SLOT(search()));
	connect( ui->tactionNewWindow, SIGNAL(triggered()), this, SLOT(newMdiChild()));
	//connect( ui->tactionCloseTab, SIGNAL(triggered()), this, SLOT(closeTab()));
	connect( ui->tactionSave, SIGNAL(triggered()), this, SLOT(saveAll()));
	connect( ui->tactionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
	connect( ui->tactionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));
	connect( ui->tactionNotes, SIGNAL(triggered()), this, SLOT(loadNotes()));
	connect( ui->tactionBookmarks, SIGNAL(triggered()), this, SLOT(loadBookmarks()));

	//menu end
	//buttons
	connect( ui->pushButton_note_save, SIGNAL( clicked() ), this, SLOT( saveNote( ) ) );
	connect( ui->pushButton_addNote, SIGNAL( clicked() ), this, SLOT( newNote( ) ) );
	connect( ui->pushButton_removeNote, SIGNAL( clicked() ), this, SLOT( removeNote( ) ) );
	connect( ui->pushButton_noteBold, SIGNAL( clicked() ), this, SLOT( noteSetTextBold( ) ) );
	connect( ui->pushButton_noteItalic, SIGNAL( clicked() ), this, SLOT( noteSetTextItalic( ) ) );
	connect( ui->pushButton_noteUnderline, SIGNAL( clicked() ), this, SLOT( noteSetTextUnderline( ) ) );
	connect( ui->pushButton_noteColor, SIGNAL( clicked() ), this, SLOT( noteSetTextColor( ) ) );
	connect( ui->pushButton_bookmarks_save, SIGNAL( clicked() ), this, SLOT( saveBookmarks( ) ) );
	connect( ui->pushButton_searchInfo, SIGNAL( clicked() ), this, SLOT( searchInfo( ) ) );
	connect( ui->pushButton_goTo, SIGNAL( clicked() ), this, SLOT( goTo( ) ) );
	connect( ui->pushButton_editNoteLink, SIGNAL( clicked() ), this, SLOT( editNoteLink( ) ) );
	//pushButton_editNoteLink
	//buttons end
	connect( ui->listWidget_notes, SIGNAL(customContextMenuRequested( QPoint )), this, SLOT(notesContextMenu()));
	connect( ui->treeWidget_bookmarks, SIGNAL(customContextMenuRequested( QPoint )), this, SLOT(bookmarksContextMenu()));
	connect( ui->treeWidget_bookmarks, SIGNAL(itemActivated(QTreeWidgetItem *,int)), this, SLOT(bookmarksGo(QTreeWidgetItem *) ));
	connect( ui->label_noteLink, SIGNAL( linkActivated( QString ) ), this, SLOT( noteGo( QString ) ) );

	ui->dockWidget_search->hide();
	ui->dockWidget_go->hide();
	ui->dockWidget_notes->hide();
	ui->dockWidget_bookmarks->hide();

	loadNotes();
	loadBookmarks();
	restoreSession();
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
        if (obj == ui->mdiArea)
        {
           //     qDebug() << "MainWindow::eventFilter() mdi event->type() = " << event->type();

                if (event->type() == QEvent::Close)
                {
                        qDebug() << "MainWindow::eventFilter()";
                        return true;
                }
                return false;
        }
        else if (obj == ui->mdiArea->currentSubWindow())
        {
              //  qDebug() << "MainWindow::eventFilter() window event->type() = " << event->type();

                if (event->type() == QEvent::Close)
                {
                        qDebug() << "MainWindow::eventFilter()";
                        return true;
                }
                return false;
        }
        else
        {
                // pass the event on to the parent class
                return QMainWindow::eventFilter(obj, event);
        }
 }
int MainWindow::loadBibleData(QTreeWidgetItem *fitem)
{
        setEnableReload(false);
	QString sid(fitem->text(1));
	bool ok;
	int id = sid.toInt(&ok,10);
	if(ok == true)
	{
		showText("");
		loadBibleDataByID(id);
                readBook(0);//das erste buch laden
	}
        qDebug() << "MainWindow::loadBibleData end id = " << id;
        setEnableReload(true);
	return 0;
}
void MainWindow::loadBibleDataByID(int id)
{
        qDebug() << "MainWindow::loadBibleDataByID() id = " << id;
	currentBibleID = id;
	if(biblesTypes.size() < currentBibleID)
		return;
        qDebug() << "MainWindow::loadBibleDataByID() biblesTypes[currentBibleID] = " << biblesTypes[currentBibleID];
	b.setBibleType(biblesTypes.at(currentBibleID));

	b.loadBibleData(id,biblesIniPath[currentBibleID]);
	showBibleName(b.bibleName);
	setBooks(b.bookFullName);
	tcache.setCurrentTabId(currentTabID());
	tcache.setBible(b);
	setChapters(b.chapterNames);
	setBooks(b.bookFullName);
	setCurrentChapter(b.currentChapterID);
	currentBibleID = b.currentBibleID;
}
int MainWindow::zoomIn()
{
	if (activeMdiChild())
	{
                QTextBrowser *t = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
		t->zoomIn(set.zoomstep);
	}
	return 0;
}
int MainWindow::zoomOut()
{
	if (activeMdiChild())
	{
                QTextBrowser *t = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
		t->zoomOut(set.zoomstep);
	}
	return 0;
}

int MainWindow::readBook(QListWidgetItem * item)
{
	int id = ui->listWidget_books->row(item);
        readBook(id);
	return 0;
}
int MainWindow::readBook(int id)
{
	readBookByID(id);
        setCurrentBook(id);//Load the book
	showChapter(0+b.chapterAdd);
        setCurrentChapter(0);//show the first chapter
	return 0;
}
void MainWindow::readBookByID(int id)
{
	qDebug() << "MainWindow::readBookByID() id = " << id;
	if( id < 0)
	{
		qDebug() << "MainWindow::readBookByID() invalid bookID";
		return;
	}
	currentBookID = id;
        if(b.readBook(currentBookID) != 0)
        {
                //error while reading
                return;
        }

        int icout = b.bookCount.at(id).toInt();
	tcache.setCurrentTabId(currentTabID());
	tcache.setCurrentBook(id,icout);

	setChapters(b.chapterNames);
	showChapter(0+b.chapterAdd);
        if (activeMdiChild())
        {
                QTextBrowser *textBrowser =   activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
                textBrowser->setSearchPaths(QStringList(b.currentBiblePath));
        }

}
int MainWindow::showChapter(int chapterid,int verseID)
{
        qDebug() << "MainWindow::showChapter() chapterid = " << chapterid << " chapterAdd = " << b.chapterAdd;
	b.currentChapterID = chapterid;
        tcache.setBible(b);
        showText(b.readChapter(b.currentChapterID,verseID));
	setCurrentChapter(b.currentChapterID-b.chapterAdd);
	currentVerseID = 0;
	return 0;
}
int MainWindow::textBrowserContextMenu( QPoint pos )
{
	Q_UNUSED(pos);
	if(!activeMdiChild())
		return 1;
	QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
	qDebug() << "MainWindow::textBrowserContextMenu() start";
	QMenu *contextMenu = new QMenu(this);
	QAction *actionCopy = new QAction(this);
	actionCopy->setText(tr("Copy"));
	connect( actionCopy, SIGNAL(triggered()), textBrowser, SLOT(copy()));
	QAction *actionCopyWholeVerse = new QAction(this);
	QTextCursor cursor = textBrowser->textCursor();
	currentTextCursor = cursor;

	if(cursor.hasSelection() == true)
	{
		int start = cursor.selectionStart(),end = cursor.selectionEnd();
		int startline=0,startverse=0,endline=0,endverse=0;
		int res=start,res2=end;
		QString text = textBrowser->toPlainText();
		QStringList lines = text.split("\n"),verses;
		bool started=false;
		if( b.chapterData.size() < b.currentChapterID)
			return 1;
		verses = b.chapterData.at(b.currentChapterID).data;
		for(int i=0;i<lines.size();++i)
		{
			res -= lines.at(i).size() + 1;
			res2 -= lines.at(i).size() + 1;
			if(started == false && res < 0)
			{
				if((-res) > cursor.selectedText().size())
				{
					startline = i;
					endline = i;
					break;
				}
				startline = i;
				started = true;
			}
			if(res2 < 0)
			{
				endline = i;
				break;
			}
		}
		started = false;
		for(int i=0;i<verses.size();++i)
		{
			QTextDocument doc;
			doc.setHtml(verses.at(i));
			QString t = doc.toPlainText();
			if(started == false && t.contains(lines.at(startline)))
			{
				startverse = i;
				started = true;
			}
			if(t.contains(lines.at(endline)))
			{
				endverse = i;
				break;
			}
		}
		QString addText;
		if(startverse != endverse)
			addText = " "+QString::number(startverse,10)+" - "+QString::number(endverse,10);
		else
			addText = " "+QString::number(startverse,10);

		if(startverse == 0 && endverse == 0)
		{
			actionCopyWholeVerse->setText(tr("Copy Verse"));
			actionCopyWholeVerse->setEnabled(false);
		}
		else
		{
			actionCopyWholeVerse->setText(tr("Copy Verse")+addText);
			actionCopyWholeVerse->setEnabled(true);
			connect( actionCopyWholeVerse, SIGNAL(triggered()), this , SLOT(copyWholeVerse()));
		}
	}
	else
	{
		qDebug() << "MainWindow::textBrowserContextMenu() no selection";
		actionCopyWholeVerse->setText(tr("Copy Verse"));
		actionCopyWholeVerse->setEnabled(false);
	}
	QAction *actionSelect = new QAction(this);
	actionSelect->setText(tr("Select All"));
	connect( actionSelect, SIGNAL(triggered()), textBrowser , SLOT(selectAll ()));

	QAction *actionBookmark = new QAction(this);
	actionBookmark->setText(tr("Add Bookmark"));
	connect( actionBookmark, SIGNAL(triggered()), this , SLOT(newBookmark()));
	
	QAction *actionNote = new QAction(this);
	actionNote->setText(tr("Add Note"));
	connect( actionNote, SIGNAL(triggered()), this , SLOT(newNoteWithLink()));

	contextMenu->addAction(actionCopy);
	contextMenu->addAction(actionCopyWholeVerse);
	contextMenu->addAction(actionSelect);
	contextMenu->addAction(actionBookmark);
	contextMenu->addAction(actionNote);
	contextMenu->exec(QCursor::pos());
	return 0;
}

int MainWindow::copyWholeVerse( void )
{
	if(!activeMdiChild())
		return 1;
	QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
	QTextCursor cursor = textBrowser->textCursor();
	currentTextCursor = cursor;
	if(cursor.hasSelection() == true)
	{
		int start = cursor.selectionStart(),end = cursor.selectionEnd();
		int startline=0,startverse=0,endline=0,endverse=0;
		int res=start,res2=end;
		QString text = textBrowser->toPlainText();
		QStringList lines = text.split("\n"),verses;
		bool started=false;
		if( b.chapterData.size() < b.currentChapterID)
			return 1;
		verses = b.chapterData.at(b.currentChapterID).data;
		for(int i=0;i<lines.size();++i)
		{
			res -= lines.at(i).size() + 1;
			res2 -= lines.at(i).size() + 1;
			if(started == false && res < 0)
			{
				if((-res) > cursor.selectedText().size())
				{
					startline = i;
					endline = i;
					break;
				}
				startline = i;
				started = true;
			}
			if(res2 < 0)
			{
				endline = i;
				break;
			}
		}
		started = false;
		for(int i=0;i<verses.size();++i)
		{
			QTextDocument doc;
			doc.setHtml(verses.at(i));
			QString t = doc.toPlainText();
			if(started == false && t.contains(lines.at(startline)))
			{
				startverse = i;
				started = true;
			}
			if(t.contains(lines.at(endline)))
			{
				endverse = i;
				break;
			}
		}
		QString stext;
		for(int i = startverse; i <= endverse;++i)
		{
			stext += verses.at(i);
		}
		QTextDocument doc2;
		doc2.setHtml(stext);
		QString sverse = "";
		if(startverse == endverse)
		{
			sverse = ","+QString::number(startverse);
		}
		else
		{
			sverse = " "+QString::number(startverse) +"-" +QString::number(endverse);
		}
		QString newText = bookFullName[currentBookID] + " "+QString::number(b.currentChapterID,10)+sverse+"\n"+doc2.toPlainText();
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(newText);


	}
	else
	{
		qDebug() << "MainwWindow::copyWholeVers ( void ) nothing is selected";
	}

	return 0;
}
int MainWindow::readChapter(QListWidgetItem * item)
{
	int id = ui->listWidget_chapters->row(item);
	showChapter(id+b.chapterAdd);
	return 0;
}
int MainWindow::readChapter(int id)
{
	showChapter(id+b.chapterAdd);
	return 0;
}
int MainWindow::loadBibles()
{
	int rcount=0;
	ui->treeWidget_bibles->clear();
	bibles.clear();
	biblesIniPath.clear();
	biblesPath.clear();
        biblesTypes.clear();
        bibleDirName.clear();
	QList<QTreeWidgetItem *> items;
	QStringList lpath = set.path;
	QProgressDialog progress(tr( "Loading Module" ), tr( "Cancel" ), 0, lpath.size());
	progress.setWindowModality(Qt::WindowModal);
	for (int path_count = 0; path_count < lpath.size(); ++path_count)
	{
		if (progress.wasCanceled())
			break;
		QTreeWidgetItem *top = new QTreeWidgetItem(ui->treeWidget_bibles);
		QStyle *style = ui->treeWidget_bibles->style();
		QIcon folderIcon;
		folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
		folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon),QIcon::Normal, QIcon::On);
		top->setIcon(0, folderIcon);

		QStringList ldictname = (lpath[path_count]+set.dict).split(set.dict);
		QString dictname;
		if(ldictname.size() > 0)
		{
			dictname = ldictname[ldictname.size()-2];
		}
		else
		{
			QString spath_count;
			spath_count.setNum(path_count,10);
			dictname = "("+spath_count+")";
		}
		top->setText(0, dictname);
		items.append(top);
		QString rpath = lpath.at(path_count)+set.dict;
		int bibletype;
		QDir dir(rpath);
		dir.setFilter(QDir::Dirs);
		QFileInfoList list = dir.entryInfoList();
		for (int i = 0; i < list.size(); ++i)//Alle Ordner auslesen
		{
			QFileInfo fileInfo = list.at(i);
			QString dirname = fileInfo.fileName();
			if(dirname != "." && dirname != "..")
			{
				QFile file;
				QString rfile;
				bibletype = 0;
				file.setFileName(rpath+dirname+set.dict+"BIBLEQT.INI");
				if(file.exists())
				{
					rfile = file.fileName();
					bibletype = 1;
				}
				file.setFileName(rpath+dirname+set.dict+"BIBLEQT.ini");
				if(bibletype == 0 && file.exists())
				{
					rfile = file.fileName();
					bibletype = 1;
				}
				file.setFileName(rpath+dirname+set.dict+"bibleqt.ini");
				if(bibletype == 0 && file.exists())
				{
					rfile = file.fileName();
					bibletype = 1;
				}
				file.setFileName(rpath+dirname+set.dict+dirname+".xml");
				if(bibletype == 0 && file.exists())
				{
					rfile = file.fileName();
					bibletype = 2;
				}
				file.setFileName(rfile);
				if (bibletype != 0 && file.open(QIODevice::ReadOnly | QIODevice::Text))
				{
					QString bname;
					switch(bibletype)
					{
						case 1:
						{
							//BibleQuote
							bname = bq.readInfo(file);
							if(bname.size() > 0)
							{
								biblesTypes << 1;
								bibles << bname;
								biblesIniPath << file.fileName();
								biblesPath << rpath+dirname+set.dict;
								bibleDirName << dirname;
								QTreeWidgetItem *ibible = new QTreeWidgetItem();
								ibible->setText(0, bname);

								QString srcount;
								srcount.setNum(rcount,10);
								ibible->setText(1, srcount);

								QIcon bibleIcon;
								bibleIcon.addPixmap(QPixmap(QString::fromUtf8(":/icons/16x16/text-x-generic.png")), QIcon::Normal, QIcon::Off);
								ibible->setIcon(0,bibleIcon);
								top->addChild(ibible);
								rcount++;
							}
							break;
						}
						case 2:
						{
							//ZenfaniaXML
							bname = zef.readInfo(file);
							if(bname.size() > 0)
							{
								biblesTypes << 2;
								bibles << bname;
								biblesIniPath << file.fileName();
								biblesPath << rpath+dirname+set.dict;
								bibleDirName << dirname;
								QTreeWidgetItem *bibleItem = new QTreeWidgetItem();
								bibleItem->setText(0, bname);
								QString srcount;
								srcount.setNum(rcount,10);
								bibleItem->setText(1, srcount);
								QIcon bibleIcon;
								bibleIcon.addPixmap(QPixmap(QString::fromUtf8(":/icons/16x16/text-xml.png")), QIcon::Normal, QIcon::Off);
								bibleItem->setIcon(0,bibleIcon);
								top->addChild(bibleItem);
								rcount++;
							}
							break;
						}
					}

				}
			}
		}
		progress.setValue(path_count);
	}
	//************************************************************************************************************************************//
	//einzelne Module laden

	//qDebug("MainWindow::loadBibles() reads the folder");
	for (int i = 0; i < set.modulePath.size(); ++i)//Alle Ordner auslesen
	{
		QFile file;
		QString rfile;
		QString dirname = set.modulePath.at(i);
		int lPos = dirname.lastIndexOf(set.dict);
		dirname = dirname.remove(lPos,dirname.size())+"/";
		qDebug() << "MainWindow::loadBibles() dirname:"<<dirname;
		int bibletype = set.moduleType.at(i).toInt();
		file.setFileName(set.modulePath.at(i));
		if (bibletype != 0 && file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QString bname;
			switch(bibletype)
			{
				case 1://BibleQuote
				{

					bname = bq.readInfo(file);
					biblesTypes << 1;
					bibles << set.moduleName.at(i);
					biblesIniPath << file.fileName();
					biblesPath << set.modulePath.at(i);//unwichtig
					bibleDirName << dirname;
					QTreeWidgetItem *bibleItem = new QTreeWidgetItem(ui->treeWidget_bibles);
					bibleItem->setText(0, bname);
					QString srcount;
					srcount.setNum(rcount,10);
					bibleItem->setText(1, srcount);
					QIcon bibleIcon;
					bibleIcon.addPixmap(QPixmap(QString::fromUtf8(":/icons/16x16/text-x-generic.png")), QIcon::Normal, QIcon::Off);
					bibleItem->setIcon(0,bibleIcon);

					items.append(bibleItem);
					rcount++;
					break;
				}
				case 2://ZenfaniaXML
				{

					bname = zef.readInfo(file);
					biblesTypes << 2;
					bibles << set.moduleName.at(i);
					biblesIniPath << file.fileName();
					biblesPath << set.modulePath.at(i);//unwichtig
					bibleDirName << dirname;
					QTreeWidgetItem *bibleItem = new QTreeWidgetItem(ui->treeWidget_bibles);
					bibleItem->setText(0, bname);
					QString srcount;
					srcount.setNum(rcount,10);

					bibleItem->setText(1, srcount);
					QIcon bibleIcon;
					bibleIcon.addPixmap(QPixmap(QString::fromUtf8(":/icons/16x16/text-xml.png")), QIcon::Normal, QIcon::Off);
					bibleItem->setIcon(0,bibleIcon);
					items.append(bibleItem);
					rcount++;
					break;
				}
			}
		}
	}
	ui->treeWidget_bibles->insertTopLevelItems(0, items);
	ui->treeWidget_bibles->sortByColumn (0,Qt::AscendingOrder);//sort
	qDebug() << "MainWindow::loadBibles() exit";
	return 0;
}
void MainWindow::setSettings( struct settings_s *ssettings )
{
	set = *ssettings;
	b.setSettings(&set);
	bq.setSettings(&set);
	zef.setSettings(&set);
	return;
}
void MainWindow::loadSettings( )
{
	set.dict = settings->value("general/dict",set.dict).toString();
	set.encoding = settings->value("general/encoding",set.encoding).toString();
	set.zoomstep = settings->value("general/zoomstep",set.zoomstep).toInt();
	set.path = settings->value("general/path",set.path).toStringList();
	set.language = settings->value("general/language",QLocale::system().name()).toString();
        set.autoLayout = settings->value("window/layout",set.autoLayout).toInt();
        set.onClickBookmarkGo = settings->value("window/onClickBookmarkGo",set.onClickBookmarkGo).toBool();

        set.textFormatting = settings->value("bible/textFormatting",set.textFormatting).toInt();

	set.moduleName = settings->value("module/name",set.moduleName).toStringList();
	set.modulePath = settings->value("module/path",set.moduleName).toStringList();
	set.moduleType = settings->value("module/type",set.moduleName).toStringList();

	bq.setSettings(&set);
	zef.setSettings(&set);
	b.setSettings(&set);
	return;
}


int MainWindow::saveSettings( struct settings_s *ssettings )
{
	bool reloadBibles=false;
	if(set.dict != ssettings->dict || set.path != ssettings->path || set.encoding != ssettings->encoding || set.modulePath != ssettings->modulePath)
	{
		reloadBibles = true;
	}
	if(set.language != ssettings->language /* || set.theme != ssettings->theme*/)
	{
                QTranslator myappTranslator;
                qDebug() << "MainWindow::saveSettings() lang = " << ssettings->language;
                myappTranslator.load("obv_" + ssettings->language);
                QApplication::installTranslator(&myappTranslator);

                ui->retranslateUi(this);

                /* QMessageBox msgBox;
		msgBox.setText(tr("You need to restart the Application to apply the changes"));
                msgBox.setInformativeText(tr("Restart now?"));
		msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Ok);
		int ret = msgBox.exec();
		switch (ret)
		{

			case QMessageBox::Ok:
				qDebug() << "MainWindow::saveSettings() is going to restart";
				//restart
				break;
			case QMessageBox::Cancel:
				break;
			default:
				break;
                 }*/
	}
	//qDebug("MainWindow::saveSettings( struct settings_s * ) started");
	setSettings(ssettings);
	settings->setValue("general/dict",set.dict);
	settings->setValue("general/encoding",set.encoding);
	settings->setValue("general/zoomstep",set.zoomstep);
	settings->setValue("general/path",set.path);
	settings->setValue("general/language",set.language);
	settings->setValue("module/name",set.moduleName);
	settings->setValue("module/path",set.modulePath);
	settings->setValue("module/type",set.moduleType);
        settings->setValue("window/layout",set.autoLayout);
        settings->setValue("window/onClickBookmarkGo",set.onClickBookmarkGo);
        settings->setValue("bible/textFormatting",set.textFormatting);
	if(reloadBibles == true)
	{
		loadBibles();
	}
	//qDebug("MainWindow::saveSettings( struct settings_s * ) exit");
	return 0;
}
int MainWindow::saveSession ( void )
{
	settings->setValue("session/geometry", saveGeometry());
	settings->setValue("session/state",saveState());
	return 0;
}
int MainWindow::restoreSession( void )
{
	QByteArray geometry = settings->value("session/geometry").toByteArray();
	QByteArray state = settings->value("session/state").toByteArray();
	if(geometry.size() != 0)
	{
		restoreGeometry(geometry);
	}
	if(state.size() != 0)
	{
		restoreState(state);
	}

	return 0;
}

int MainWindow::printFile( void )
{
	QPrinter printer;
	QPrintDialog *dialog = new QPrintDialog(&printer, this);
	dialog->setWindowTitle(tr("Print"));
	if (dialog->exec() != QDialog::Accepted)
		return 1;
	qDebug("MainWindow::printFile() prints");
	if (activeMdiChild())
	{
		QTextBrowser *t = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
		t->print(&printer);
	}
	return 0;
}
int MainWindow::saveFile( void )
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	QString fileName = QFileDialog::getSaveFileName();
	qDebug() << "MainWindow::saveFile() fileName = "<<fileName;
	if (activeMdiChild())
	{
		qDebug() << "MainWindow::saveFile() hast active child";
		QTextBrowser *t = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
		if(fileName.endsWith(".html") || fileName.endsWith(".htm"))
		{
			QFile file(fileName);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
				return 0;
			QTextStream out(&file);
			out << t->toHtml();
			file.close();
		}
		else if(fileName.endsWith(".pdf"))
		{
			QPrinter printer;
			printer.setOutputFormat(QPrinter::PdfFormat);
			printer.setOutputFileName(fileName);
			t->print(&printer);

		}
		else
		{
			QFile file(fileName);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
				return 0;
			QTextStream out(&file);
			out << t->toPlainText();
			file.close();
		}
	}

	return 0;
}
int MainWindow::showSettingsDialog( void )
{
	settingsDialog setDialog;
	connect( &setDialog, SIGNAL( save(struct settings_s *) ), this, SLOT( saveSettings(struct settings_s *) ) );
	connect( &setDialog, SIGNAL( save(struct settings_s *) ), &setDialog, SLOT( close() ) );
	setDialog.setSettings(&set);
	setDialog.setWindowTitle(tr("Configuration"));
	setDialog.show();
	setDialog.exec();
	return 0;
}

int MainWindow::close( void )
{
        close();
	return 0;
}

void MainWindow::copy()
{
        if (activeMdiChild())
        {
                QTextBrowser *textBrowser =   activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
                textBrowser->copy();
        }
}
void MainWindow::selectAll()
{
        if (activeMdiChild())
        {
                QTextBrowser *textBrowser =   activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
                textBrowser->selectAll();
        }
}
int MainWindow::saveAll()
{
	saveBookmarks();
	saveNote();
	saveSession();
	return 0;
}
int MainWindow::showAboutDialog( void )
{
        aboutDialog aDialog;

        aDialog.setWindowTitle(tr("About openBibleViewer"));
	aDialog.show();
	aDialog.setText("openBibleViewer <br> version: "+VERSION+" build: "+BUILD+"<br> <a href=\"http://openbv.uucyc.name/\"> Official Website</a> | <a href=\"http://openbv.uucyc.name/bug/\">Bug report</a>");
        return aDialog.exec();
}
int MainWindow::go2Pos(QString pos)
{
        qDebug() << "MainWindow::go2Pos() pos = " << pos;
	QStringList list = pos.split(";");
	if(list.size() < 4)
	{
		return 1;
	}
	QString dirname = list.at(0);
	QString sbookID = list.at(1);
	QString schapterID = list.at(2);
	QString sverseID = list.at(3);
	int bibleID=0;
	int bookID = sbookID.toInt();
	int chapterID = schapterID.toInt();
	int verseID = sverseID.toInt();
	//get bibleID
	for(int i=0;i<bibleDirName.size();i++)
	{
		if(bibleDirName.at(i) == dirname)
		{
			bibleID = i;
			break;
		}
	}
        qDebug() << "MainWindow::go2Pos() bibleID = " << bibleID << " , bookID = " << bookID << " , chapterID = " << chapterID << ", verseID = " << verseID;
        if(biblesTypes.size() < bibleID)
                return 1;
        qDebug() << "MainWindow::go2Pos() biblesTypes[currentBibleID] = " << biblesTypes.at(bibleID);
        b.setBibleType(biblesTypes.at(bibleID));
        if(bibleID != currentBibleID)
                b.loadBibleData(bibleID,biblesIniPath[bibleID]);

        showBibleName(b.bibleName);
        setBooks(b.bookFullName);
        tcache.setCurrentTabId(currentTabID());
        tcache.setBible(b);
        setChapters(b.chapterNames);
        setBooks(b.bookFullName);

        currentBibleID = b.currentBibleID;

        if(bookID >= 0 && bookID < b.bookFullName.size())
        {
                readBook(bookID);
                setCurrentBook(bookID);
        }
        showChapter(chapterID-1+b.chapterAdd,verseID-1);
        setCurrentChapter(chapterID-1+b.chapterAdd);


	return 0;
}
void MainWindow::goTo()
{
	QString text = ui->lineEdit_goTo->text();
	if(text.size() < 1)
		return;
        /*if(text.contains(QRegExp("![A-Za-z0-9\.\s]{2,}!is")))
        {
                //Hiob
        }
        else if(text.contains(QRegExp("![A-Za-z0-9\.\s]{2,} [0-9]{1,}!is")))
        {

                //Hiob 4
        }
        else if(text.contains(QRegExp("![A-Za-z0-9\.\s]{2,} [0-9]{1,},[\s]{0,1}[0-9]{1,}!is")))
        {
                //Hiob 4,9
        }*/
	//todo: implement goTo
	return;
}
int MainWindow::verseFromCursor(QTextCursor cursor)
{
	qDebug() << "MainWindow::verseFromCursor()";
	if(!activeMdiChild())
		return 1;
	QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");

	int startline=0,startverse=0;
	if(cursor.position() != 0)
	{
		int pos = cursor.position();
		qDebug() << "MainWindow::verseFromCursor() pos = " << pos;
		QString text = textBrowser->toPlainText();
		QStringList lines = text.split("\n");
		int res=pos;
		QString htmltext;
		if(biblesTypes.size() < currentBibleID)
			return 1;
		QStringList verses = b.chapterDataList;
		qDebug() << "MainWindow::verseFromCursor() lines.size() = " << lines.size();
		for(int i=0;i < lines.size();++i)
		{
			QString l = lines.at(i);
			res -= l.size()+1;
			if(res < 0)
			{
				startline = i;
				break;
			}
		}
                qDebug() << "MainWindow::verseFromCursor() verses.size() = " << verses.size() << ", startline = " << startline;
		qDebug() << "MainWindow::verseFromCursor() lines.at(startline) = " << lines.at(startline);
		for(int i=0;i < verses.size();++i)
		{
			QTextDocument doc;
			doc.setHtml(verses.at(i));
			QString t = doc.toPlainText();
			qDebug() << "MainWindow::verseFromCursor() i = " << i << " t = " << t;
			if(t.contains(lines.at(startline)) /*|| lines.at(startline).contains(t)*/)
			{
				startverse = i;
				break;
			}
		}
	}
	qDebug() << "MainWindow::verseFromCursor() startverse = " << startverse;
	return startverse;
}
int MainWindow::setChapters(QStringList list)
{
	ui->listWidget_chapters->clear();
	ui->listWidget_chapters->insertItems(0,list);
	if (activeMdiChild())
	{
		qDebug() << "MainWindow::setChapters() hast active Child";
		QComboBox *comboBox_chapters = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_chapters");
		comboBox_chapters->clear();
		comboBox_chapters->insertItems(0,list);
	}
	return 0;
}
void MainWindow::setBooks(QStringList books)
{

	ui->listWidget_books->clear();
	ui->listWidget_books->insertItems(0,books);
	if (activeMdiChild())
	{
		qDebug() << "MainWindow::setBooks() hast active Child";
		QComboBox *comboBox_books = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_books");
		comboBox_books->clear();
		comboBox_books->insertItems(0,books);
	}
	return;
}
void MainWindow::setCurrentBook(int bookID)
{
	ui->listWidget_books->setItemSelected(ui->listWidget_books->item(bookID),true);
	if (activeMdiChild())
	{
		QComboBox *comboBox_books = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_books");
		comboBox_books->setCurrentIndex(bookID);
	}
}
void MainWindow::setCurrentChapter(int chapterID)
{
        qDebug() << "MainWindow::setCurrentChapter() chapterID = " << chapterID;
        ui->listWidget_chapters->setItemSelected(ui->listWidget_chapters->item(chapterID),true);
	if (activeMdiChild())
	{
		QComboBox *comboBox_chapters = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_chapters");
		comboBox_chapters->setCurrentIndex(chapterID);
	}
}
void MainWindow::showText(QString text)
{
	qDebug() << "MainWindow::showText() text.size() = " << text.size();
	if (activeMdiChild())
	{
		QTextBrowser *textBrowser =   activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
		textBrowser->setHtml(text);
	}
}
void MainWindow::setEnableReload(bool enable)
{
        enableReload = enable;
}
void MainWindow::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event);
        saveNote();
        saveBookmarks();
	saveSession();
}
void MainWindow::showBibleName(QString name)
{
	if (activeMdiChild())
	{
		activeMdiChild()->setWindowTitle(name);
	}
}
MainWindow::~MainWindow()
{
	delete ui;
}
