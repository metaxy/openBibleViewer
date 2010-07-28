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
#ifndef ADVANCEDINTERFACE_H
#define ADVANCEDINTERFACE_H

#include <QWidget>
#include "src/ui/interface/interface.h"
#include "src/ui/dock/bookdockwidget.h"
#include "src/ui/dock/moduledockwidget.h"
#include "src/ui/dock/searchresultdockwidget.h"
#include "src/ui/dock/advancedsearchresultdockwidget.h"
#include "src/ui/dock/notesdockwidget.h"
#include "src/ui/dock/bookmarksdockwidget.h"
#include "src/ui/dock/dictionarydockwidget.h"
#include "src/ui/dock/quickjumpdockwidget.h"
#include "src/core/windowcache.h"
#include "src/api/bibleapi.h"
#include <QtGui/QMdiSubWindow>
#include <QtGui/QCloseEvent>
#include "mdiform.h"
#include <QContextMenuEvent>
namespace Ui
{
class AdvancedInterface;
}

class AdvancedInterface : public Interface
{
    Q_OBJECT
public slots:
    void closing();
    void settingsChanged(Settings oldSettings, Settings newSettings);
    void restoreSession();
private slots:
    void myCascade();
    void myTileVertical();
    void myTileHorizontal();
    void myTile();

    void newSubWindow(bool doAutoLayout = true);
    void closeSubWindow();
    int closingWindow();
    int reloadWindow(QMdiSubWindow * window);

    void pharseUrl(QUrl url);
    void pharseUrl(QString url);

    void showText(const QString &text);
    void zoomIn();
    void zoomOut();
    void readChapter(const int &id);
    void readBook(const int &id);


    void newColorMark();
    void newCustomColorMark();
    void newBoldMark();
    void newItalicMark();
    void newUnderlineMark();

    void removeMark();
    void newBookmark();
    void newNoteWithLink();

    void search(SearchQuery query);
    void search();

    int saveFile();
    int printFile();
    void onlineHelp();

    void copy();
    void selectAll();
    void nextVerse();
    void previousVerse();
    void nextChapter();
    void previousChapter();
    int copyWholeVerse();
    void reloadChapter(bool full = false);
    void reloadActive();

    void showBookmarksDock();
    void showNotesDock();

    void showSearchDialog();
    int showAboutDialog();
    void showMarkCategories();
    void showMarkList();
    void showNotesEditor();

    void setTabView();
    void setSubWindowView();

    void attachApi();
    void showContextMenu(QContextMenuEvent*ev);
    void debugger();



public:
    AdvancedInterface(QWidget *parent = 0);
    ~AdvancedInterface();
    void init();
    bool hasMenuBar();
    QMenuBar* menuBar();
    bool hasToolBar();
    QList<QToolBar*> toolBars();
    void setBookDockWidget(BookDockWidget *bookDockWidget);
    void setModuleDockWidget(ModuleDockWidget *moduleDockWidget);
    //void setSearchResultDockWidget(SearchResultDockWidget *searchResultDockWidget);
    void setAdvancedSearchResultDockWidget(AdvancedSearchResultDockWidget *advancedSearchResultDockWidget);
    void setNotesDockWidget(NotesDockWidget *notesDockWidget);
    void setBookmarksDockWidget(BookmarksDockWidget *boockmarksDockWidget);
    void setDictionaryDockWidget(DictionaryDockWidget *dictionaryDockWidget);
    void setQuickJumpDockWidget(QuickJumpDockWidget *quickJumpDockWidget);



protected:
    void changeEvent(QEvent *e);
signals:
    void get(QString);
    void historySetUrl(QString url);
    void reloadInterface();
private:
    Ui::AdvancedInterface *ui;
    bool m_enableReload;

    QMdiSubWindow *activeMdiChild();
    QList<QMdiSubWindow*> usableWindowList();
    QList<QMdiSubWindow *> m_internalWindows;

    int currentWindowID();
    int tabIDof(QMdiSubWindow* window);
    WindowCache m_windowCache;
    int m_lastActiveWindow;
    QWebView* getView();
    void setEnableReload(bool enable);
    void loadModuleDataByID(int id);

    void setTitle(const QString &title);
    void setChapters(const QStringList &chapters);
    void setBooks(const QStringList &books);
    void setCurrentBook(const int &bookID);
    void setCurrentChapter(const int &chapterID);

    void readBookByID(int id);
    VerseSelection verseSelection();

    void showChapter(const int &chapterID, const int &verseID);
    void autoLayout();
    QTextCursor m_textCursor;
    BibleDisplaySettings *m_bibleDisplaySettings;

    //Menu
    QAction *m_actionTabView;
    QAction *m_actionSubWindowView;
    void searchInText(SearchQuery query);

    BibleApi *m_bibleApi;
    QAction *m_actionCopy;
    QAction *m_actionSelect;
    QMenu *m_menuMark;
    QAction *m_actionRemoveMark;
    QAction *m_actionBookmark;
    QAction *m_actionNote;
    void createDefaultMenu();

};

#endif // ADVANCEDINTERFACE_H
