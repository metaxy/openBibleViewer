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
#include <QtCore/QString>
#include <QtCore/QtDebug>
#include <QtCore/QFSFileEngine>
#include <QtCore/QTextStream>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtGui/QAction>
#include <QtGui/QPrintDialog>
#include <QtGui/QPrinter>
#include <QtGui/QProgressDialog>
#include <QtGui/QFileDialog>
#include <QtGui/QClipboard>
#include <QtGui/QMessageBox>
#include <QtGui/QComboBox>
#include <QtGui/QDesktopServices>
#include <QtGui/QKeyEvent>


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../searchdialog.h"
#include "../settingsdialog.h"
#include "../aboutdialog.h"
#include "../../core/stelle.h"
#include "../../core/chapter.h"
#include "../../core/moduleconfig.h"
#include "../../core/goto.h"

//spilt MainWindow in some files
#include "mainnotes.cpp"
#include "mainbookmarks.cpp"
#include "mainsearch.cpp"
#include "mainstrong.cpp"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
	ui->setupUi(this);
	VERSION  = "0.2";
	BUILD =  "2009-9-14";

#ifdef _PORTABLE_VERSION
	homeDataPath = QApplication::applicationDirPath()+"/";
	settings = new QSettings( homeDataPath+"openBibleViewer.ini",QSettings::IniFormat);
#else
	homeDataPath = QApplication::applicationDirPath()+"/";

#ifdef Q_WS_MAC
	homeDataPath = QFSFileEngine::homePath() +"/.openbible/";
	settings = new QSettings(homeDataPath+"openBibleViewer.ini",QSettings::IniFormat);
#endif
#ifdef Q_WS_X11
	homeDataPath = QFSFileEngine::homePath() +"/.openbible/";
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
	set.zoomstep = 1;
	set.removeHtml = true;
	set.version = VERSION;
	set.build = BUILD;
	set.autoLayout = 1;
	set.onClickBookmarkGo = true;
	set.textFormatting = 0;
	set.homePath = homeDataPath;
	set.zefaniaBible_hardCache = true;
	set.zefaniaBible_softCache = true;
	QStringList bookNames;
	bookNames << "Genesis";
	bookNames << "Exodus";
	bookNames << "Leviticus";
	bookNames << "Numbers";
	bookNames << "Deuteronomy";
	bookNames << "Joshua";
	bookNames << "Judges";
	bookNames << "Ruth";
	bookNames << "1 Samuel";
	bookNames << "2 Samuel";
	bookNames << "1 Kings";
	bookNames << "2 Kings";
	bookNames << "1 Chronicles";
	bookNames << "2 Chronicles";
	bookNames << "Ezrav";
	bookNames << "Nehemiah";
	bookNames << "Esther";
	bookNames << "Job";
	bookNames << "Psalm";
	bookNames << "Proverbs";
	bookNames << "Ecclesiastes";
	bookNames << "Song of Solomon";
	bookNames << "Isaiah";
	bookNames << "Jeremiah";
	bookNames << "Lamentations";
	bookNames << "Ezekiel";
	bookNames << "Daniel";
	bookNames << "Hosea";
	bookNames << "Joel";
	bookNames << "Amos";
	bookNames << "Obadiah";
	bookNames << "Jonah";
	bookNames << "Micah";
	bookNames << "Nahum";
	bookNames << "Habakkuk";
	bookNames << "Zephaniah";
	bookNames << "Haggai";
	bookNames << "Zechariah";
	bookNames << "Malachi";
	bookNames << "Matthew";
	bookNames << "Mark";
	bookNames << "Luke";
	bookNames << "John";
	bookNames << "Acts";
	bookNames << "Romans";
	bookNames << "1 Corinthians";
	bookNames << "2 Corinthians";
	bookNames << "Galatians";
	bookNames << "Ephesians";
	bookNames << "Philippians";
	bookNames << "Colossians";
	bookNames << "1 Thessalonians";
	bookNames << "2 Thessalonians";
	bookNames << "1 Timothy";
	bookNames << "2 Timothy";
	bookNames << "Titus";
	bookNames << "Philemon";
	bookNames << "Hebrews";
	bookNames << "James";
	bookNames << "1 Peter";
	bookNames << "2 Peter";
	bookNames << "1 John";
	bookNames << "2 John";
	bookNames << "3 John";
	bookNames << "Jude";
	bookNames << "Revelation";
	set.bookNames  = bookNames;



	loadSettings();
	newMdiChild();
	loadStrongs();

	/*if(set.module.size() == 0)
	{
		QString appPath = QApplication::applicationDirPath();
		if(appPath.endsWith("/"))
		{
			appPath.remove(appPath.size()-1,10);
		}
		moduleConfig m;
		m.biblequote_removeHtml = true;
		m.moduleName = appPath;
		m.modulePath = appPath;
		m.moduleType = "-1";
		m.zefbible_hardCache = true;
		m.zefbible_showStrong = true;
		m.zefbible_showStudyNote = true;
		m.zefbible_softCache = true;
		m.isDir = true;
		set.module << m;
	}*/
	connect( this, SIGNAL( get ( QString )), this, SLOT( pharseUrl( QString )));
	connect( ui->mdiArea, SIGNAL( subWindowActivated ( QMdiSubWindow * )), this, SLOT(reloadWindow( QMdiSubWindow * )));
	connect( ui->treeWidget_bibles, SIGNAL(itemClicked( QTreeWidgetItem *, int)), this, SLOT( loadModuleData( QTreeWidgetItem* ) ) );
	connect( ui->listWidget_books, SIGNAL( itemActivated(QListWidgetItem *) ), this, SLOT( readBook(QListWidgetItem * ) ) );
	connect( ui->listWidget_chapters, SIGNAL( itemActivated(QListWidgetItem *) ), this, SLOT( readChapter(QListWidgetItem * ) ) );
	connect( ui->listWidget_search, SIGNAL( itemActivated(QListWidgetItem *) ), this, SLOT( goToSearchResult(QListWidgetItem * ) ) );
	connect( ui->listWidget_notes, SIGNAL( itemActivated(QListWidgetItem *) ), this, SLOT( showNote(QListWidgetItem * ) ) );

	//menu
	connect( ui->actionPrint, SIGNAL(triggered()), this, SLOT(printFile()));
	connect( ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
	connect( ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect( ui->actionSearch, SIGNAL(triggered()), this, SLOT(search()));
	connect( ui->actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
	connect( ui->actionSelectAll, SIGNAL(triggered()), this, SLOT(selectAll()));
	connect( ui->actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
	connect( ui->actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));
	connect( ui->actionNotes, SIGNAL(triggered()), this, SLOT(loadNotes()));
	connect( ui->actionBookmarks, SIGNAL(triggered()), this, SLOT(loadBookmarks()));
	connect( ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
	connect( ui->actionOnlineHelp, SIGNAL(triggered()), this, SLOT(onlineHelp()));
	connect( ui->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
	connect( ui->actionNextVerse, SIGNAL(triggered()), this, SLOT(nextVerse()));
	connect( ui->actionLastVerse, SIGNAL(triggered()), this, SLOT(lastVerse()));
	connect( ui->actionNewWindow, SIGNAL(triggered()), this, SLOT(newMdiChild()));

	connect( ui->actionVerticalTile, SIGNAL(triggered()), this, SLOT(myTileVertical()));
	connect( ui->actionHorizontalTile, SIGNAL(triggered()), this, SLOT(myTileHorizontal()));
	connect( ui->actionCascade, SIGNAL(triggered()), this, SLOT(myCascade()));

	connect( ui->tactionSearch, SIGNAL(triggered()), this, SLOT(search()));
	connect( ui->tactionNewWindow, SIGNAL(triggered()), this, SLOT(newMdiChild()));
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
	connect( ui->pushButton_goTo, SIGNAL( clicked() ), this, SLOT( goToPos( ) ) );
	connect( ui->pushButton_editNoteLink, SIGNAL( clicked() ), this, SLOT( editNoteLink( ) ) );
	connect( ui->pushButton_strongSearch, SIGNAL( clicked() ), this, SLOT( strongSearch( ) ) );
	//pushButton_editNoteLink
	//buttons end
	connect( ui->listWidget_notes, SIGNAL(customContextMenuRequested( QPoint )), this, SLOT(notesContextMenu()));
	connect( ui->treeWidget_bookmarks, SIGNAL(customContextMenuRequested( QPoint )), this, SLOT(bookmarksContextMenu()));
	connect( ui->treeWidget_bookmarks, SIGNAL(itemActivated(QTreeWidgetItem *,int)), this, SLOT(bookmarksGo(QTreeWidgetItem *) ));
	connect( ui->label_noteLink, SIGNAL( linkActivated( QString ) ), this, SLOT( noteGo( QString ) ) );
	connect( ui->comboBox_strong, SIGNAL(currentIndexChanged(int)), this, SLOT( loadStrongModule( int ) ) );
	connect( ui->textBrowser_strong, SIGNAL(anchorClicked(QUrl)), this, SLOT(pharseUrl( QUrl )));
	ui->dockWidget_search->hide();
	ui->dockWidget_go->hide();
	ui->dockWidget_notes->hide();
	ui->dockWidget_bookmarks->hide();

	ui->lineEdit_goTo->installEventFilter(this);

	loadNotes();
	loadBookmarks();

	restoreSession();
}

int MainWindow::loadModuleData(QTreeWidgetItem *fitem)
{
	QString sid(fitem->text(1));
	bool ok;
	int id = sid.toInt(&ok);
	showText("");
	emit get("bible://"+QString::number(id)+"/0,0,0");
	qDebug() << "MainWindow::loadModuleData end id = " << id;
	return 0;
}
void MainWindow::loadModuleDataByID(int id)
{
	qDebug() << "MainWindow::loadModuleDataByID() id = " << id;
	currentBibleID = id;
	if(biblesTypes.size() < currentBibleID)//keine solche bibel vorhanden
		return;
	qDebug() << "MainWindow::loadModuleDataByID() biblesTypes[currentBibleID] = " << biblesTypes[currentBibleID];
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
	qDebug() << "MainWindow::zoomIn()";
	if (activeMdiChild())
	{
		qDebug() << "MainWindow::zoomIn() zoom = " << set.zoomstep;
		QTextBrowser *t = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
		t->zoomIn(set.zoomstep);
	}
	return 0;
}
int MainWindow::zoomOut()
{
	qDebug() << "MainWindow::zoomOut()";
	if (activeMdiChild())
	{
		qDebug() << "MainWindow::zoomOut() zoom = " << set.zoomstep;
		QTextBrowser *t = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
		t->zoomOut(set.zoomstep);
	}
	return 0;
}

int MainWindow::readBook(QListWidgetItem * item)
{
	int id = ui->listWidget_books->row(item);
	qDebug() << "MainWindow::readBook(QListWidgetItem) id = " << id;
	readBook(id);
	return 0;
}
int MainWindow::readBook(int id)
{
	qDebug() << "MainWindow::readBook(int) id = " << id;
	emit get("bible://current/"+QString::number(id)+",0,0");
	return 0;
}
void MainWindow::readBookByID(int id)
{
	qDebug() << "MainWindow::readBookByID() id = " << id;
	if( id < 0)
	{
		QMessageBox::critical(0,tr("Error"),tr("This Book is not available."));
		qDebug() << "MainWindow::readBookByID() invalid bookID";
		return;
	}
	if(id >= b.bookFullName.size())
	{
		QMessageBox::critical(0,tr("Error"),tr("This Book is not available."));
		qDebug() << "MainWindow::readBookByID() invalid bookID";
		return;
	}
	if(b.readBook(id) != 0)
	{
		QMessageBox::critical(0,tr("Error"),tr("Can not read the file."));
		//error while reading
		return;
	}
	currentBookID = id;
	int icout = b.bookCount[id];
	tcache.setCurrentTabId(currentTabID());
	tcache.setCurrentBook(id,icout);

	setChapters(b.chapterNames);
	showChapter(0+b.chapterAdd);
	if (activeMdiChild())
	{
		QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
		textBrowser->setSearchPaths(QStringList(b.currentBiblePath));
	}

}
int MainWindow::showChapter(int chapterid,int verseID)
{
	qDebug() << "MainWindow::showChapter() chapterid = " << chapterid << " chapterAdd = " << b.chapterAdd;
	b.currentChapterID = chapterid;
	currentVerseID = verseID;
	tcache.setBible(b);
	showText(b.readChapter(b.currentChapterID,verseID));
	setCurrentChapter(b.currentChapterID-b.chapterAdd);
	return 0;
}
int MainWindow::textBrowserContextMenu( QPoint pos )
{
	if(!activeMdiChild())
		return 1;
	QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
	qDebug() << "MainWindow::textBrowserContextMenu() start";
	QMenu *contextMenu = new QMenu(this);
	QAction *actionCopy = new QAction(this);
	actionCopy->setText(tr("Copy"));
	connect( actionCopy, SIGNAL(triggered()), textBrowser, SLOT(copy()));
	QAction *actionCopyWholeVerse = new QAction(this);
	QTextCursor cursor = textBrowser->cursorForPosition(pos);	
	QTextCursor cursor2 = textBrowser->textCursor();
	if(cursor2.hasSelection())
	{
		cursor = cursor2;
	}
	currentTextCursor = cursor;
	if(cursor.hasSelection())
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
		if(biblesTypes.at(currentBibleID) == 2 && set.module.at(set.moduleID[currentBibleID]).zefbible_textFormatting == 0)
		{
			startverse = startline -1;
			endverse = endline -1;
			if(startverse < 0)
				startverse = 0;
		}
		else
		{
			qDebug() << "MainWindow::textBrowserContextMenu() startline = " << startline << ", endline = " << endline;
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
		}
		QString addText;
		if(startverse != endverse)
			addText = " "+QString::number(startverse,10)+" - "+QString::number(endverse,10);
		else
			addText = " "+QString::number(startverse,10);
		qDebug() << "MainWindow::textBrowserContextMenu() startverse = " << startverse << ", endverse = " << endverse;
		if(startverse < 0 || endverse <= 0)
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
		if(biblesTypes.at(currentBibleID) == 2 && set.module.at(set.moduleID[currentBibleID]).zefbible_textFormatting == 0)
		{
			startverse = startline -1;
			endverse = endline -1;
		}
		else
		{
			qDebug() << "MainWindow::textBrowserContextMenu() startline = " << startline << ", endline = " << endline;
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
		}
		if(startverse < 0 || endverse <= 0)
			return 1;
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
	emit get("bible://current/"+QString::number(b.currentBookID)+","+QString::number(id)+",0");
	return 0;
}
int MainWindow::readChapter(int id)
{
	emit get("bible://current/"+QString::number(b.currentBookID)+","+QString::number(id)+",0");
	return 0;
}
int MainWindow::loadModules()
{
	int rcount=0;//Counter fo the Bible ID
	ui->treeWidget_bibles->clear();
	bibles.clear();
	biblesIniPath.clear();
	biblesPath.clear();
	biblesTypes.clear();
	bibleDirName.clear();
	QList<QTreeWidgetItem *> items;
	QProgressDialog progress(tr( "Loading Module" ), tr( "Cancel" ), 0, set.module.size());
	progress.setWindowModality(Qt::WindowModal);

	for (int i = 0; i < set.module.size(); ++i)//Alle Ordner auslesen
	{
		if (progress.wasCanceled())
				break;
		if(set.module.at(i).isDir == true)
		{
			QTreeWidgetItem *top = new QTreeWidgetItem(ui->treeWidget_bibles);
			QStyle *style = ui->treeWidget_bibles->style();
			QIcon folderIcon;
			folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
			folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon),QIcon::Normal, QIcon::On);
			top->setIcon(0, folderIcon);

			top->setText(0, set.module.at(i).moduleName);
			items.append(top);
			QString rpath = set.module.at(i).modulePath+"/";
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
					file.setFileName(rpath+dirname+"/"+"BIBLEQT.INI");
					if(file.exists())
					{
						rfile = file.fileName();
						bibletype = 1;
					}
					file.setFileName(rpath+dirname+"/"+"BIBLEQT.ini");
					if(bibletype == 0 && file.exists())
					{
						rfile = file.fileName();
						bibletype = 1;
					}
					file.setFileName(rpath+dirname+"/"+"bibleqt.ini");
					if(bibletype == 0 && file.exists())
					{
						rfile = file.fileName();
						bibletype = 1;
					}
					file.setFileName(rpath+dirname+"/"+dirname+".xml");
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
								bname = b.bq.readInfo(file);
								if(bname.size() > 0)
								{
									biblesTypes << 1;
									bibles << bname;
									biblesIniPath << file.fileName();
									biblesPath << rpath+dirname+"/";
									bibleDirName << dirname;
									QTreeWidgetItem *ibible = new QTreeWidgetItem();
									ibible->setText(0, bname);

									QString srcount;
									srcount.setNum(rcount,10);
									ibible->setText(1, srcount);

									QIcon bibleIcon;
									bibleIcon.addPixmap(QPixmap(":/icons/16x16/text-x-generic.png"), QIcon::Normal, QIcon::Off);
									ibible->setIcon(0,bibleIcon);
									top->addChild(ibible);
									rcount++;
								}
								break;
							}
						case 2:
							{
								//ZenfaniaXML-Bible
								bname = b.zef.readInfo(file);
								if(bname.size() > 0)
								{
									biblesTypes << 2;
									bibles << bname;
									biblesIniPath << file.fileName();
									biblesPath << rpath+dirname+"/";
									bibleDirName << dirname;
									QTreeWidgetItem *bibleItem = new QTreeWidgetItem();
									bibleItem->setText(0, bname);
									QString srcount;
									srcount.setNum(rcount,10);
									bibleItem->setText(1, srcount);
									QIcon bibleIcon;
									bibleIcon.addPixmap(QPixmap(":/icons/16x16/text-xml.png"), QIcon::Normal, QIcon::Off);
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
		}
		else
		{

			QFile file;
			QString rfile;
			QString dirname = set.module.at(i).modulePath;
			int lPos = dirname.lastIndexOf("/");
			dirname = dirname.remove(lPos,dirname.size())+"/";
			qDebug() << "MainWindow::loadModules() dirname:"<<dirname;
			int bibletype = set.module.at(i).moduleType.toInt();
			file.setFileName(set.module.at(i).modulePath);
			if (bibletype != 0 && file.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				switch(bibletype)
				{
				case 1://BibleQuote
					{
						biblesTypes << 1;//Insert the bibleID
						bibles << set.module.at(i).moduleName; // Insert the title
						biblesIniPath << file.fileName();
						biblesPath << set.module.at(i).modulePath;
						bibleDirName << dirname;
						QTreeWidgetItem *bibleItem = new QTreeWidgetItem(ui->treeWidget_bibles);
						bibleItem->setText(0, set.module.at(i).moduleName);
						QString srcount;
						srcount.setNum(rcount,10);
						bibleItem->setText(1, srcount);
						set.moduleID[rcount] = i;

						QIcon bibleIcon;
						bibleIcon.addPixmap(QPixmap(":/icons/16x16/text-x-generic.png"), QIcon::Normal, QIcon::Off);
						bibleItem->setIcon(0,bibleIcon);
						items.append(bibleItem);
						rcount++;
						break;
					}
				case 2://ZenfaniaXML
					{
						biblesTypes << 2;
						bibles << set.module.at(i).moduleName;
						biblesIniPath << file.fileName();
						biblesPath << set.module.at(i).modulePath;
						bibleDirName << dirname;
						QTreeWidgetItem *bibleItem = new QTreeWidgetItem(ui->treeWidget_bibles);
						bibleItem->setText(0, set.module.at(i).moduleName);
						QString srcount;
						srcount.setNum(rcount,10);
						bibleItem->setText(1, srcount);
						set.moduleID[rcount] = i;

						QIcon bibleIcon;
						bibleIcon.addPixmap(QPixmap(":/icons/16x16/text-xml.png"), QIcon::Normal, QIcon::Off);
						bibleItem->setIcon(0,bibleIcon);
						items.append(bibleItem);
						rcount++;
						break;
					}
				}
			}
		}
	}
	ui->treeWidget_bibles->insertTopLevelItems(0, items);
	ui->treeWidget_bibles->sortByColumn (0,Qt::AscendingOrder);//sort
	//qDebug() << "MainWindow::loadModules() exit";
	return 0;
}
void MainWindow::setSettings( struct settings_s ssettings )
{
	set = ssettings;
	b.setSettings(set);
	zefStrong.setSettings(set,moduleConfig());
	return;
}
void MainWindow::loadSettings( )
{
	set.encoding = settings->value("general/encoding",set.encoding).toString();
	set.zoomstep = settings->value("general/zoomstep",set.zoomstep).toInt();
#ifdef Q_WS_WIN
	set.language = settings->value("general/language","en").toString();
#else
	set.language = settings->value("general/language",QLocale::system().name()).toString();
#endif
	set.autoLayout = settings->value("window/layout",set.autoLayout).toInt();
	set.onClickBookmarkGo = settings->value("window/onClickBookmarkGo",set.onClickBookmarkGo).toBool();

	set.textFormatting = settings->value("bible/textFormatting",set.textFormatting).toInt();

	int size = settings->beginReadArray("module");
	for (int i = 0; i < size; ++i)
	{
		settings->setArrayIndex(i);
		moduleConfig m;
		m.moduleName = settings->value("name").toString();
		m.modulePath = settings->value("path").toString();
		m.moduleType = settings->value("type").toString();
		m.biblequote_removeHtml = settings->value("removeHtml").toInt();
		m.zefbible_textFormatting = settings->value("textFormatting").toInt();
		m.zefbible_hardCache = settings->value("hardCache").toBool();
		m.zefbible_softCache = settings->value("softCache").toBool();
		m.zefbible_showStrong = settings->value("showStrong").toBool();
		m.zefbible_showStudyNote = settings->value("showStudyNote").toBool();
		m.isDir = settings->value("isDir").toBool();
		set.module.append(m);
	}
	settings->endArray();

	zefStrong.setSettings(set,moduleConfig());
	b.setSettings(set);
	return;
}


int MainWindow::saveSettings( struct settings_s ssettings )
{
	bool reloadBibles=false;
	if(set.encoding != ssettings.encoding)
	{
		reloadBibles = true;
	}
	for(int i = 0;i < ssettings.module.size();++i)
	{
		if(set.module.size() < i || set.module.empty())
		{
			reloadBibles = true;
			break;
		}
		else
		{
			moduleConfig m1,m2;
			m1 = ssettings.module.at(i);
			m2 = set.module.at(i);
			if(memcmp(&m1,&m2,sizeof(struct moduleConfig)))
			{
				reloadBibles = true;
				break;
			}
		}
	}
	if(set.language != ssettings.language /* || set.theme != ssettings->theme*/)
	{
		QTranslator myappTranslator;
		qDebug() << "MainWindow::saveSettings() lang = " << ssettings.language;
		myappTranslator.load("obv_" + ssettings.language);
		QApplication::installTranslator(&myappTranslator);

		ui->retranslateUi(this);
	}
	//qDebug("MainWindow::saveSettings( struct settings_s  ) started");
	setSettings(ssettings);
	settings->setValue("general/encoding",set.encoding);
	settings->setValue("general/zoomstep",set.zoomstep);
	settings->setValue("general/language",set.language);
	settings->setValue("window/layout",set.autoLayout);
	settings->setValue("window/onClickBookmarkGo",set.onClickBookmarkGo);
	settings->setValue("bible/textFormatting",set.textFormatting);

	settings->beginWriteArray("module");
	for (int i = 0; i < set.module.size(); ++i)
	{
		settings->setArrayIndex(i);
		settings->setValue("name", set.module.at(i).moduleName);
		settings->setValue("path", set.module.at(i).modulePath);
		settings->setValue("type", set.module.at(i).moduleType);
		settings->setValue("textFormatting", set.module.at(i).zefbible_textFormatting);
		settings->setValue("removeHtml", set.module.at(i).biblequote_removeHtml);
		settings->setValue("hardCache", set.module.at(i).zefbible_hardCache);
		settings->setValue("softCache", set.module.at(i).zefbible_softCache);
		settings->setValue("showStrong", set.module.at(i).zefbible_showStrong);
		settings->setValue("showStudyNote", set.module.at(i).zefbible_showStudyNote);
		settings->setValue("isDir", set.module.at(i).isDir);
	}
	settings->endArray();

	if(reloadBibles == true)
	{
		qDebug() << " MainWindow::saveSettings() reload bibles";
		loadModules();
		loadStrongs();
	}
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
	//dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setNameFilter(tr("Html (*.html *.htm);;PDF (*.pdf);;Plain (*.txt)"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);

	QString fileName = QFileDialog::getSaveFileName();
	qDebug() << "MainWindow::saveFile() fileName = "<<fileName;
	if (activeMdiChild())
	{
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
	connect( &setDialog, SIGNAL( save(struct settings_s ) ), this, SLOT( saveSettings(struct settings_s ) ) );
	connect( &setDialog, SIGNAL( save(struct settings_s ) ), &setDialog, SLOT( close() ) );
	setDialog.setSettings(set);
	setDialog.setWindowTitle(tr("Configuration"));
	setDialog.show();
	return setDialog.exec();
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
		QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
		textBrowser->copy();
	}
}
void MainWindow::selectAll()
{
	if (activeMdiChild())
	{
		QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
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
	//qDebug() << "MainWindow::go2Pos() pos = " << pos;
	QStringList list = pos.split(";");
	if(list.size() < 4)//invalid pos
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
	//qDebug() << "MainWindow::go2Pos() bibleID = " << bibleID << " , bookID = " << bookID << " , chapterID = " << chapterID << ", verseID = " << verseID;
	emit get("bible://"+QString::number(bibleID)+"/"+QString::number(bookID)+","+QString::number(chapterID-1)+","+QString::number(verseID-1));
	return 0;
}
void MainWindow::goToPos()
{
	QString text = ui->lineEdit_goTo->text();
	//qDebug() << "MainWindow::goTo() text = " << text;
	goTo go(currentBibleID,b.bookFullName,b.chapterAdd);
	QString url = go.getUrl(text);
	emit get(url);
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
		int res = pos;
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
	return startverse+1;//wegen titel
}
int MainWindow::setChapters(QStringList list)
{
	ui->listWidget_chapters->clear();
	ui->listWidget_chapters->insertItems(0,list);
	if (activeMdiChild())
	{
		//qDebug() << "MainWindow::setChapters() hast active Child";
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
		//qDebug() << "MainWindow::setBooks() hast active Child";
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
	//qDebug() << "MainWindow::setCurrentChapter() chapterID = " << chapterID;
	ui->listWidget_chapters->setItemSelected(ui->listWidget_chapters->item(chapterID),true);
	if (activeMdiChild())
	{
		QComboBox *comboBox_chapters = activeMdiChild()->widget()->findChild<QComboBox *>("comboBox_chapters");
		comboBox_chapters->setCurrentIndex(chapterID);
	}
}
void MainWindow::showText(QString text)
{
	//qDebug() << "MainWindow::showText() text.size() = " << text.size();
	if (activeMdiChild())
	{
		QTextBrowser *textBrowser = activeMdiChild()->widget()->findChild<QTextBrowser *>("textBrowser");
		textBrowser->setHtml(text);
		if(currentVerseID > 1)
			textBrowser->scrollToAnchor("currentVerse");
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
void MainWindow::pharseUrl(QUrl url)
{
	pharseUrl(url.toString());
}
void MainWindow::pharseUrl(QString url)
{
	QString url_backup = url;
	setEnableReload(false);
	qDebug() << "MainWindow::pharseUrl() url = " << url;
	QString bible = "bible://";
	QString strong = "strong://";
	QString http = "http://";
	QString bq = "go";
	if(url.startsWith(bible))
	{
		url = url.remove(0,bible.size());
		QStringList a = url.split("/");
		if(a.size() == 2)
		{
			QStringList c = a.at(1).split(",");
			if(c.size() >= 3)
			{
				int bibleID;
				if(a.at(0) == "current")
				{
					bibleID = b.currentBibleID;
				}
				else
				{
					bibleID = a.at(0).toInt();
				}
				int bookID = c.at(0).toInt();
				int chapterID = c.at(1).toInt();
				int verseID = c.at(2).toInt();
				if(bibleID != b.currentBibleID)
				{
					loadModuleDataByID(bibleID);
					readBookByID(bookID);
					setCurrentBook(bookID);
					showChapter(chapterID+b.chapterAdd,verseID);
					setCurrentChapter(chapterID);
					//load bible
				}
				else if(bookID != b.currentBookID)
				{
					readBookByID(bookID);
					setCurrentBook(bookID);
					showChapter(chapterID+b.chapterAdd,verseID);
					setCurrentChapter(chapterID);
					//load book
				}
				else if(chapterID != b.currentChapterID)
				{
					showChapter(chapterID+b.chapterAdd,verseID);
					setCurrentChapter(chapterID);
					//load chapter
				}
				else
				{
					showChapter(chapterID+b.chapterAdd,verseID);
					setCurrentChapter(chapterID);
				}
				if(c.size() == 4 && c.at(3) == "searchInCurrentText=true")
				{
					searchInCurrentText(lastsearch);
				}
				emit historySetUrl(url_backup);
			}
			else
			{
				qDebug() << "MainWindow::pharseUrl() invalid URL";
			}
		}
		else
		{
			qDebug() << "MainWindow::pharseUrl() invalid URL";
		}
		//bible://bibleID/bookID,chapterID,verseID
	}
	else if(url.startsWith(strong))
	{
		url = url.remove(0,strong.size());
		showStrong(url);
		//strong://strongID
	}
	else if(url.startsWith(http))
	{
		QDesktopServices::openUrl(url);
		//its a web link
	}
	else if(url.startsWith(bq))
	{
		//its a biblequote internal link but i dont have the specifications!!!
		QStringList internal = url.split(" ");
		QString bibleID = internal.at(1);//todo: use it
		int bookID = internal.at(2).toInt() - 1;
		int chapterID = internal.at(3).toInt() -1;
		int verseID = internal.at(4).toInt();
	//	qDebug() << "MainWindow::pharseUrl() internal = " << internal << " internalChapter = " <<internal.at(3).toInt() << " chapterID" << chapterID << " chapterAdd = "<< b.chapterAdd;
		/*if(bibleID != b.currentBibleID)
		{
			loadModuleDataByID(bibleID);
			readBookByID(bookID);
			setCurrentBook(bookID);
			showChapter(chapterID+b.chapterAdd,verseID);
			setCurrentChapter(chapterID);
			//load bible
		}
		else */if(bookID != b.currentBookID)
		{
			readBookByID(bookID);
			setCurrentBook(bookID);
			showChapter(chapterID+b.chapterAdd,verseID);
			setCurrentChapter(chapterID);
			//load book
		}
		else if(chapterID != b.currentChapterID)
		{
			showChapter(chapterID+b.chapterAdd,verseID);
			setCurrentChapter(chapterID);
			//load chapter
		}
		else
		{
			showChapter(chapterID+b.chapterAdd,verseID);
			setCurrentChapter(chapterID);
		}
		emit historySetUrl(url_backup);

	}
	else
	{
		qDebug() << "MainWindow::pharseUrl() invalid URL";
	}
	setEnableReload(true);
	return;
}
void MainWindow::onlineHelp()
{
	QDesktopServices::openUrl(QString("http://openbv.uucyc.name/faq.html"));
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == ui->lineEdit_goTo)
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
			if(keyEvent->key() == 16777220)
			{
				goToPos();
				return true;
			}
			else
			{
				return QMainWindow::eventFilter(obj, event);
			}

		}
		else
		{
			return QMainWindow::eventFilter(obj, event);;
		}
	}
	else if (obj == ui->lineEdit_strong)
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
			if(keyEvent->key() == 16777220)
			{
				strongSearch();
				return true;
			}
			else
			{
				return QMainWindow::eventFilter(obj, event);
			}

		}
		else
		{
			return QMainWindow::eventFilter(obj, event);;
		}
	}
	else
	{
		return QMainWindow::eventFilter(obj, event);
	}
}

MainWindow::~MainWindow()
{
	delete ui;
	delete settings;
	delete note;
}
