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
