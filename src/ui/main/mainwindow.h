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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QList>
#include <QtCore/QSettings>
#include <QtCore/QUrl>
#include <QtCore/QTranslator>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QTextCursor>

#include "../../core/config.h"
#include "../../core/notes.h"
#include "../../core/searchquery.h"
#include "../../core/windowcache.h"

#include "../../module/bible.h"
#include "../../module/biblequote.h"
#include "../../module/zefania-bible.h"
#include "../../module/zefania-strong.h"

namespace Ui
{
class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    int loadModuleData(QTreeWidgetItem*);
    void readBook(QListWidgetItem * item);
    void readBook(const int &bookID);
    void readChapter(QListWidgetItem * item);
    void readChapter(const int &chapterID);
    void zoomIn();
    void zoomOut();
    int search();
    int printFile();
    int showSearchResults(struct searchQuery query);
    int showSettingsDialog();
    int showAboutDialog();
    int saveSettings(struct settings_s settings);
    int reloadWindow(QMdiSubWindow * window);
    int closeWindow();
    int saveFile();
    int saveAll();
    int textBrowserContextMenu(QPoint);
    int copyWholeVerse();
    int goToSearchResult(QListWidgetItem * item);
    void loadNotes();
    void showNote(QListWidgetItem * item);
    void saveNote();
    void notesContextMenu();
    void newNote();
    void newNoteWithLink();
    void removeNote();
    void reloadNotes();
    void copyNote();
    void editNoteLink();
    void bookmarksContextMenu(void);
    int loadBookmarks();
    void newBookmark();
    void newBookmarksFolder();
    void removeBookmark();
    void editBookmark();
    void saveBookmarks();
    void bookmarksGo();
    void bookmarksGo(QTreeWidgetItem * item);
    void updateBookmark(QString pos);

    void showChapter(const int &chapterID, const int &verseID = -1);
    void noteSetTextBold();
    void noteSetTextItalic();
    void noteSetTextUnderline();
    void noteSetTextColor();
    void updateNote(QString pos);
    void searchInfo();
    void nextVerse();
    void lastVerse();
    void newMdiChild();
    void goToPos();
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
    void init(const QString &homeDataPath);
    int loadModules();
    void setTranslator(QTranslator *my, QTranslator *qt);

protected:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *ev);
private:
    Ui::MainWindowClass *ui;
    bible m_bible;
    biblequote bq;
    zefaniaBible zef;
    zefaniaStrong zefStrong;
    struct settings_s set;
    notes *note;
    windowCache m_windowCache;
    QStringList encodings;
    int currentVerseID;
    QString currentNoteID;
    int lastActiveWindow;
    QString lastsearch;
    QString bookmarksFileName, homeDataPath;
    QTextCursor currentTextCursor;
    QStringList bibles, biblesPath, biblesIniPath, bookPath, bookFullName, bibleDirName;
    QMap <int, int> bookCount;
    QMap<QString, QString> currentNoteRef;
    QList<int> biblesTypes;
    QSettings *settings;
    QString VERSION, BUILD;
    bool enableReload;

    void setChapters(const QStringList &chapterNames);
    void setBooks(const QStringList &bookNames);
    void setCurrentBook(const int &bookID);
    void setCurrentChapter(const int &chapterID);
    void setEnableReload(bool enable);
    void setTitle(const QString &title);
    void setSettings(struct settings_s ssettings);

    void searchInCurrentText(QString searchtext);
    void showText(const QString &text);
    void showStrong(const QString &strongID);
    void showNote(const QString &noteID);

    void readBookByID(int id);
    void loadModuleDataByID(int id);

    void loadLanguage(const QString &language);
    void loadSettings();
    QString loadStrong(QString strongID);
    void loadStrongs();
    void loadDefaultConfig();
    void initSignals();

    void saveSession();
    void restoreSession();

    int verseFromCursor(QTextCursor cursor);
    int currentWindowID();
    int tabIDof(QMdiSubWindow* window);
    QString notePos2Text(const QString &pos);
    int internalOpenPos(const QString &pos);
    QMdiSubWindow *activeMdiChild();
    QList<QMdiSubWindow*> usableWindowList();

    QList<QMdiSubWindow *> m_internalWindows;
    int m_currentStrongModule;
    QString m_currentStrongID;
    QList<int> m_strongModuleID;
    QTranslator *myappTranslator;
    QTranslator *qtTranslator;

};

#endif // MAINWINDOW_H
