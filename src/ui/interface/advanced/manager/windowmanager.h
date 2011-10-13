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
#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>
#include <QtGui/QMdiArea>
#include <QtWebKit/QWebView>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QVBoxLayout>

#include "src/core/basicclass.h"
#include "src/core/link/urlconverter2.h"
#include "src/core/dbghelper.h"
#include "src/api/api.h"
#include "src/ui/interface/advanced/mdiareafilter.h"
#include "src/ui/interface/advanced/form/bibleform.h"
#include "src/ui/interface/advanced/form/webform.h"
#include "bookmarksmanager.h"
#include "notesmanager.h"
#include "src/ui/interface/advanced/form/form.h"

class WindowManager : public QObject , public BasicClass
{
    Q_OBJECT
public:
    explicit WindowManager(QObject *parent = 0);
    ~WindowManager();
    void setMdiArea(QMdiArea *area);
    void setApi(Api *api);
    void setNotesManager(NotesManager *notesManager);
    void setBookmarksManager(BookmarksManager *bookmarksManager);
    void init();
    Form *activeForm();
    QMdiSubWindow *activeSubWindow();

    QList<QMdiSubWindow*> usableWindowList() const;

public slots:
    void cascade();
    void tileVertical();
    void tileHorizontal();
    void tile();
    QMdiSubWindow * newSubWindow(bool doAutoLayout = true, bool forceMax = false, Form::FormType type = Form::BibleForm);
    QMdiSubWindow * newBibleSubWindow(bool doAutoLayout = true, bool forceMax = false);
    QMdiSubWindow * newWebSubWindow(bool doAutoLayout = true, bool forceMax = false);
    QMdiSubWindow * newDictionarySubWindow(bool doAutoLayout = true, bool forceMax = false);

    Form* getForm(QMdiSubWindow *w);

    void closeSubWindow();
    int closingWindow();
    int reloadWindow(QMdiSubWindow * window);
    void mdiAreaResized();
    void zoomIn();
    void zoomOut();
    void reloadActive();

    void disable();
    void enable();

    void setTabbedView();
    void setSubWindowView();

    void save();
    void restore();
    void setTitle(const QString &title);

    void reloadChapter(bool full);
    void newSubWindowIfEmpty();

    QMdiSubWindow* needBibleWindow();
    QMdiSubWindow* needDictionaryWindow();
    QMdiSubWindow* needWebWindow();
    QMdiSubWindow* needWindow(Form::FormType type = Form::BibleForm);

    QMdiSubWindow* hasDictWindow(OBVCore::DefaultModule d);
    QMdiSubWindow* hasDictWindow(const int moduleID);



private:
    int m_nameCounter;
    QMdiArea *m_area;//not in our control
    Api *m_api;//not in out control
    MdiAreaFilter *m_mdiAreaFilter;
    bool m_enableReload;
    void autoLayout();

    /*int currentWindowName();*/
    int m_lastActiveWindow;
    void setEnableReload(bool enable);

    NotesManager *m_notesManager;
    BookmarksManager *m_bookmarksManager;
    int *m_currentWindowID;

    void installResizeFilter();


};

#endif // WINDOWMANAGER_H
