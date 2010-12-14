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
#ifndef BIBLEMANAGER_H
#define BIBLEMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include "src/ui/dock/dockwidget.h"
#include "src/core/basicclass.h"
#include "src/ui/dock/bookdockwidget.h"
#include "src/ui/dock/moduledockwidget.h"
#include "src/ui/dock/advancedsearchresultdockwidget.h"
#include "src/ui/dock/quickjumpdockwidget.h"
#include "src/core/bible/bibleurlrange.h"
class BibleManager : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit BibleManager(QObject *parent = 0);
    void init();
    void setWidget(QWidget *p);
    void createDocks();
    QHash<DockWidget *, Qt::DockWidgetArea> docks();

    //todo:

    bool loadModuleDataByID(const int &id);
    void readBookByID(const int &id);
    void showChapter(const int &chapterID, const int &verseID);

signals:

    void updateChapters();
    void updateBooks();

public slots:
    //todo:
    void pharseUrl(const QString &url);
    void pharseUrl(const BibleUrl &url);
    /*
        void readChapter(const int &id);
        void readBook(const int &id);
    */
    //void nextVerse();
    //void previousVerse();

    void nextChapter();
    void previousChapter();
    //int copyWholeVerse();
    void reloadChapter(bool full = false);
private:
    void showRanges(Ranges ranges);
    Ranges bibleUrlRangeToRanges(BibleUrlRange r);
    QWidget *m_p;
    AdvancedSearchResultDockWidget *m_advancedSearchResultDockWidget;
    BookDockWidget *m_bookDockWidget;
    ModuleDockWidget *m_moduleDockWidget;
    QuickJumpDockWidget * m_quickJumpDockWidget;
};

#endif // BIBLEMANAGER_H
