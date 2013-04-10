/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QMessageBox>
#include <QDesktopServices>
#include <QScrollBar>
#include <QFileDialog>
#include <QKeySequence>
#include <QLineEdit>
#include <QCloseEvent>

#include <QScopedPointer>
#include <QTimer>

#include "src/core/moduletools.h"
#include "src/core/dbghelper.h"

#include "src/ui/interface/interface.h"
#include "src/ui/dock/dictionarydockwidget.h"
#include "src/ui/historylineedit.h"

#include "src/ui/interface/advanced/manager/windowmanager.h"
#include "src/ui/interface/advanced/manager/versemodulemanager.h"
#include "src/ui/interface/advanced/manager/notesmanager.h"
#include "src/ui/interface/advanced/manager/searchmanager.h"
#include "src/ui/interface/advanced/manager/bookmarksmanager.h"
#include "src/ui/interface/advanced/manager/dictionarymanager.h"
#include "src/ui/interface/advanced/manager/webpagemanager.h"

#include "src/ui/dialog/aboutdialog.h"
#include "src/ui/noteseditor.h"
#include "src/ui/marklist.h"

#include <QPointer>
class Api;
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
    void parseUrl(QString url, const Actions::OpenLinkModifiers mod);

    void saveFile();
    void printFile();
    void printPreview();
    void onlineHelp();

    void copy();
    void selectAll();

    void showAboutDialog();
    void showMarkCategories();
    void showMarkList();
    void showNotesEditor();

    void setTabbedView();
    void setSubWindowView();
    void setTitle(const QString &title);

    void quick();
    void uncheck(bool b);

    void fullscreen(bool enabled);
    void focusQuickOpen();

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

    QString name() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AdvancedInterface *ui;

    Api *m_api;
    //Menu
    QAction *m_actionTabbedView;
    QAction *m_actionSubWindowView;
    void searchInText(SearchQuery query);

    QToolBar *m_mainBar;
    QToolBar *m_searchBar;

    QAction *m_mainBarActionSearch;
    QAction *m_mainBarActionBookmarks;
    QAction *m_mainBarActionNotes;
    QAction *m_mainBarActionNewWindow;
    QAction *m_mainBarActionZoomIn;
    QAction *m_mainBarActionZoomOut;
    QAction *m_mainBarActionModule;

    QAction *m_actionCascade;
    QAction *m_actionTile;
    QAction *m_actionTileVertical;
    QAction *m_actionTileHorizontal;

    void toolBarSetText();


    WindowManager *m_windowManager;
    VerseModuleManager *m_bibleManager;
    NotesManager *m_notesManager;
    SearchManager *m_searchManager;
    BookmarksManager *m_bookmarksManager;
    DictionaryManager *m_dictionaryManager;
    WebPageManager *m_webPageManager;

    void quickSearch(const QString &text);
    void quick(QString text);
    HistoryLineEdit *m_quickLine;

};

#endif // ADVANCEDINTERFACE_H
