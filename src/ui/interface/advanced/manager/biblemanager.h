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
#ifndef BIBLEMANAGER_H
#define BIBLEMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include "src/ui/dock/dockwidget.h"
#include "src/core/basicclass.h"
#include "src/ui/dock/bookdockwidget.h"
#include "src/ui/dock/moduledockwidget.h"

#include "src/ui/dock/quickjumpdockwidget.h"
#include "src/core/verse/verseurlrange.h"
#include "src/core/dbghelper.h"

#include "src/ui/interface/advanced/manager/windowmanager.h"

class BibleManager : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit BibleManager(QObject *parent = 0);
    void init();
    void setWidget(QWidget *p);
    void createDocks();
    QHash<DockWidget *, Qt::DockWidgetArea> docks();

    BookDockWidget *bookDockWidget();
    ModuleDockWidget *moduleDockWidget();
    QuickJumpDockWidget * quickJumpDockWidget();

    void setWindowManager(WindowManager *windowManager);

signals:

    void updateChapters();
    void updateBooks();

public slots:
    void pharseUrl(const QString &url);
    void pharseUrl(const VerseUrl &url);

    void nextChapter();
    void previousChapter();
    void loadBibleList(bool hadBible);

    void reShowCurrentRange();
    void reloadBible();

    void setCurrentVerseTableID(const int verseTableID);
private:

    QWidget *m_p;

    BookDockWidget *m_bookDockWidget;
    ModuleDockWidget *m_moduleDockWidget;
    QuickJumpDockWidget * m_quickJumpDockWidget;

    WindowManager *m_windowManager;
};

#endif // BIBLEMANAGER_H
