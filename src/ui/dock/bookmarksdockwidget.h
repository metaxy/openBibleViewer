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
/*!
 BookmarksDockWidget represents a dock widget, where you can view and edit your bookmarks.

*/
class BookmarksDockWidget : public DockWidget
{
    Q_OBJECT
public slots:
    void newBookmark(VerseSelection selection);
    void saveBookmarks();
private slots:
    void bookmarksContextMenu(void);
    void newBookmarksFolder();
    void removeBookmark();
    void editBookmark();

    void bookmarksGo();
    void bookmarksGo(QTreeWidgetItem * item);
    void updateBookmark(QString pos);
public:
    BookmarksDockWidget(QWidget *parent = 0);
    ~BookmarksDockWidget();
    int init();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::BookmarksDockWidget *ui;
    int internalOpenPos(const QString &pos);
    QString bookmarksFileName;
signals:
    void get(QString url);
};

#endif // BOOKMARKSDOCKWIDGET_H
