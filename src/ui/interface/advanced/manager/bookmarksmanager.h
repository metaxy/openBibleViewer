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
#ifndef BookmarksMANAGER_H
#define BookmarksMANAGER_H
#include "src/core/basicclass.h"
#include "src/ui/dock/bookmarksdockwidget.h"
#include <QObject>
class BookmarksManager : public QObject, public BasicClass
{
    Q_OBJECT
public:
    BookmarksManager(QObject *parent = 0);
    void setWidget(QWidget *p);
    void createDocks();
    QHash<DockWidget *, Qt::DockWidgetArea> docks();
    BookmarksDockWidget *bookmarksDockWidget();

    void newBookmark(VerseSelection selection);
    void save();
private:
    QWidget *m_p;
    BookmarksDockWidget *m_bookmarksDockWidget;
};

#endif // BookmarksMANAGER_H
