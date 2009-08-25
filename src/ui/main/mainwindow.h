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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QSettings>
#include <QList>
#include <QTreeWidgetItem>
#include <QTextBrowser>
#include <QTabWidget>
#include <QSettings>
#include <QMdiSubWindow>

#include "../../core/config.h"
#include "../../module/biblequote.h"
#include "../../module/zefania-bible.h"
#include "../../module/zefania-strong.h"
#include "../../core/tabcache.h"
#include "../../core/notes.h"
#include "../../module/bible.h"

namespace Ui
{
	class MainWindowClass;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
public slots:
	int loadModuleData(QTreeWidgetItem* );
	int readBook(QListWidgetItem * item);
	int readBook(int);
	int readChapter(QListWidgetItem * item);
	int readChapter(int);
	int zoomIn();
	int zoomOut();
	int search();
	int close();
	int printFile();
	int showSearchResults ( QString string,bool regexp,bool whole,bool casesen);
	int showSettingsDialog();
	int showAboutDialog();
	int saveSettings( struct settings_s settings );
	int reloadWindow( QMdiSubWindow * window);
	int closeWindow();
	int saveFile();
	int saveAll();
	int textBrowserContextMenu( QPoint );
	int copyWholeVerse();
	int goToSearchResult(QListWidgetItem * item);
	int loadNotes();
	int showNote(QListWidgetItem * item);
	int saveNote();
	int notesContextMenu();
	int newNote();
	int newNoteWithLink();
	int removeNote();
	int reloadNotes();
	int copyNote();
	void editNoteLink();
	void bookmarksContextMenu( void );
	int loadBookmarks();
	void newBookmark();
	void newBookmarksFolder();
	void removeBookmark();
	void editBookmark();
	void saveBookmarks();
	void bookmarksGo();
	void bookmarksGo(QTreeWidgetItem * item);
	void updateBookmark(QString pos);
	int saveSession();
	int restoreSession();
	int showChapter(int chapterID,int verseID = -1);
	int noteSetTextBold();
	int noteSetTextItalic();
	int noteSetTextUnderline();
	int noteSetTextColor();
	void searchInfo();
	void nextVerse();
	void lastVerse();
	void newMdiChild();
	void goToPos();
	int noteGo(QString pos);
	void copy();
	void selectAll();
	void myCascade();
	void myTileVertical();
	void myTileHorizontal();
	void pharseUrl(QString url);
	void pharseUrl(QUrl url);
	void strongSearch();
	void loadStrongModule(int lID);
	void onlineHelp();

signals:
	void get(QString url);
	void historySetUrl(QString url);
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	void setSettings( struct settings_s ssettings );
	void loadSettings();
	int loadModules();
	int go(int book,int chapter,int verse);
	 protected:
	void closeEvent(QCloseEvent *event);
private:
	Ui::MainWindowClass *ui;
	bible b;
	biblequote bq;
	zefaniaBible zef;
	zefaniaStrong zefStrong;
	struct settings_s set;
	QStringList encodings;
	int currentBookID,currentBibleID,fontsize,book_ccount,firsttime,currentChapterID,currentZoom,currentNoteID,currentVerseID;
	int lastActiveWindow;
	QString currentBiblePath,lastout,lastsearch,chaptersign,versesign;
	QString currentNotePos,bookmarksFileName,homeDataPath;
	QPoint currentTextCursorPos;
	QTextCursor currentTextCursor;
	QStringList bibles,biblesPath,biblesIniPath,bookPath,bookFullName,bookShortName,chapterText,bibleDirName;
	QMap <int,int> bookCount;
	QList<int> biblesTypes;
	QList<QTextBrowser*> textBrowsers;
	QTabWidget *tabWidget;
	tabCache tcache;
	QSettings *settings;
	notes *note;
	QString VERSION, BUILD;
	bool enableReload;

	int setChapters( QStringList chapterNames );
	void setBooks(QStringList bookNames);
	void setCurrentBook(int bookID);
	void setCurrentChapter(int chapterID);
	void searchInCurrentText (QString searchtext);
	void showText(QString text);
	void showBibleName(QString name);
	void readBookByID(int id);
	void loadModuleDataByID(int id);
	int verseFromCursor(QTextCursor cursor);
	void setEnableReload(bool enable);
	QString notePos2Text( QString );
	int go2Pos(QString pos);
	QMdiSubWindow *activeMdiChild();
	QList<QMdiSubWindow*> usableWindowList();
	int currentTabID();
	int tabIDof(QMdiSubWindow* window);
	int lastBookmark;
	QList<QMdiSubWindow *> internalWindows;
	void showStrong(QString strongID);
	QString loadStrong(QString strongID);
	void loadStrongs();
	int currentStrongModule;
	QString currentStrongID;
	QList<int> strongModuleID;


protected:
	bool eventFilter(QObject *obj, QEvent *ev);


};

#endif // MAINWINDOW_H
