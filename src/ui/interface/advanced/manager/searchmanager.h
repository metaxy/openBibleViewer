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
#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H
#include <QtCore/QHash>
#include "windowmanager.h"
#include "src/ui/dock/advancedsearchresultdockwidget.h"

#include "src/core/basicclass.h"

#include "src/core/search/searchquery.h"
#include "src/core/search/search.h"
#include "src/core/dbghelper.h"
#include "src/ui/dialog/searchdialog.h"

class SearchManager : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit SearchManager(QObject *parent = 0);
    void setWidget(QWidget *p);
    void setWindowManager(WindowManager *windowManager);
    void createDocks();
    QHash<DockWidget *, Qt::DockWidgetArea> docks();
    AdvancedSearchResultDockWidget *advancedSearchResultDockWidget();
signals:

public slots:

    void search(SearchQuery query);
    void search();
    void showSearchDialog();
    void nextVerse();
    void previousVerse();
    void searchInText(SearchQuery query);
private:
    QWidget *m_p;
    WindowManager *m_windowManager;
    AdvancedSearchResultDockWidget *m_advancedSearchResultDockWidget;
};

#endif // SEARCHMANAGER_H
