/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSizePolicy>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopServices>
#include <QtGui/QScrollBar>
#include <QtGui/QFileDialog>
#include <QtGui/QKeySequence>
#include <QtGui/QLineEdit>
#include <QtGui/QCloseEvent>

#include <QtCore/QScopedPointer>
#include <QtCore/QTimer>

#include "src/api/api.h"

#include "src/core/obvcore.h"
#include "src/core/dbghelper.h"

#include "src/ui/interface/interface.h"
#include "src/ui/dock/dictionarydockwidget.h"

#include "src/ui/interface/advanced/manager/windowmanager.h"
#include "src/ui/interface/advanced/manager/biblemanager.h"
#include "src/ui/interface/advanced/manager/notesmanager.h"
#include "src/ui/interface/advanced/manager/searchmanager.h"
#include "src/ui/interface/advanced/manager/bookmarksmanager.h"

#include "mdiareafilter.h"

#include "src/ui/dialog/aboutdialog.h"
#include "src/ui/noteseditor.h"
#include "src/ui/marklist.h"

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


    void saveFile();
    void printFile();
    void printPreview();
    void onlineHelp();

    void copy();
    void selectAll();


    void showBookmarksDock();
    void showNotesDock();

    int showAboutDialog();
    void showMarkCategories();
    void showMarkList();
    void showNotesEditor();

    void setTabbedView();
    void setSubWindowView();
    void setTitle(const QString &title);


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
    void historySetUrl(QString url);
    void reloadInterface();
private:
    Ui::AdvancedInterface *ui;

    ModuleDisplaySettings *m_moduledisplaysettings;
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

    void toolBarSetText();


    DictionaryDockWidget * m_dictionaryDockWidget;

    WindowManager *m_windowManager;
    BibleManager *m_bibleManager;
    NotesManager *m_notesManager;
    SearchManager *m_searchManager;
    BookmarksManager *m_bookmarksManager;
};

#endif // ADVANCEDINTERFACE_H
