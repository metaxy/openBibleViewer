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
#ifndef BOOKMARKSDOCKWIDGET_H
#define BOOKMARKSDOCKWIDGET_H

#include <QDockWidget>
#include "dockwidget.h"
#include "src/core/verseselection.h"
#include <QtGui/QTreeWidgetItem>
namespace Ui
{
class BookmarksDockWidget;
}
/**
 * BookmarksDockWidget represents a dock widget, where you can view and edit your bookmarks.
 */
class BookmarksDockWidget : public DockWidget
{
    Q_OBJECT
public slots:
    void newBookmark(VerseSelection selection);
    void saveBookmarks();
private slots:
    void bookmarksContextMenu();
    void newBookmarksFolder();
    void removeBookmark();
    void editBookmark();

    void bookmarksGo();
    void bookmarksGo(QTreeWidgetItem * item);
    void updateBookmarkLink(VerseUrl url);
public:
    BookmarksDockWidget(QWidget *parent = 0);
    ~BookmarksDockWidget();
    int init();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::BookmarksDockWidget *ui;
    int internalOpenPos(const QString &pos);
    QString m_bookmarksFileName;
};

#endif // BOOKMARKSDOCKWIDGET_H
