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
#include "src/api/api.h"
#include "mdiareafilter.h"
#include "windowmanager.h"
#include <QtGui/QMdiSubWindow>
#include <QtGui/QCloseEvent>
#include <QtGui/QContextMenuEvent>
namespace Ui
{
class AdvancedInterface;
}

class AdvancedInterface : public Interface
{
    Q_OBJECT
public slots:
    void closing();
    void settingsChanged(Settings oldSettings, Settings newSettings, bool modifedModuleSettings);
    void restoreSession();
private slots:


    void pharseUrl(QUrl url);
    void pharseUrl(QString url);

    void showText(const QString &text);

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

    void saveFile();
    void printFile();
    void onlineHelp();

    void copy();
    void selectAll();
    void nextVerse();
    void previousVerse();
    void nextChapter();
    void previousChapter();
    int copyWholeVerse();
    void reloadChapter(bool full = false);


    void showBookmarksDock();
    void showNotesDock();

    void showSearchDialog();
    int showAboutDialog();
    void showMarkCategories();
    void showMarkList();
    void showNotesEditor();

    void setTabView();
    void setSubWindowView();

    void showContextMenu(QContextMenuEvent*ev);
    void debugger();
    void installResizeFilter();

public:
    AdvancedInterface(QWidget *parent = 0);
    ~AdvancedInterface();
    void init();
    bool hasMenuBar();
    QMenuBar* menuBar();
    bool hasToolBar();
    QList<QToolBar*> toolBars();
    QHash<DockWidget *, Qt::DockWidgetArea> docks();

    void createDocks();
    void createToolBars();
    void createMenu();



protected:
    void changeEvent(QEvent *e);
signals:
    void get(QString);
    void historySetUrl(QString url);
    void reloadInterface();
private:
    Ui::AdvancedInterface *ui;


    bool loadModuleDataByID(int id);

    void setTitle(const QString &title);
    void setChapters(const QStringList &chapters);
    void setBooks(const QHash<int, QString> &books, QList<int> ids);
    void clearBooks();
    void clearChapters();
    void setCurrentBook(const int &bookID);
    void setCurrentChapter(const int &chapterID);

    void readBookByID(const int &id);
    VerseSelection verseSelection();

    void showChapter(const int &chapterID, const int &verseID);

    BibleDisplaySettings *m_bibleDisplaySettings;
    Api *m_api;
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
    MdiAreaFilter *m_mdiAreaFilter;

    QToolBar *m_mainBar;
    QToolBar *m_searchBar;

    QAction *m_mainBarActionSearch;
    QAction *m_mainBarActionBookmarks;
    QAction *m_mainBarActionNotes;
    QAction *m_mainBarActionNewWindow;
    QAction *m_mainBarActionZoomIn;
    QAction *m_mainBarActionZoomOut;
    QAction *m_mainBarActionModule;

    AdvancedSearchResultDockWidget *m_advancedSearchResultDockWidget;
    BookDockWidget *m_bookDockWidget;
    ModuleDockWidget *m_moduleDockWidget;
    NotesDockWidget *m_notesDockWidget;
    BookmarksDockWidget * m_bookmarksDockWidget;
    DictionaryDockWidget * m_dictionaryDockWidget;
    QuickJumpDockWidget * m_quickJumpDockWidget;

    WindowManager *m_windowManager;


};

#endif // ADVANCEDINTERFACE_H
