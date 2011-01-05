#include "bookmarksmanager.h"

BookmarksManager::BookmarksManager(QObject *parent) :
    QObject(parent)
{
}

void BookmarksManager::setWidget(QWidget *p)
{
    m_p = p;
}

void BookmarksManager::createDocks()
{
    m_bookmarksDockWidget = new BookmarksDockWidget(m_p);
    setAll(m_bookmarksDockWidget);
    m_bookmarksDockWidget->init();
    m_bookmarksDockWidget->hide();
}

QHash<DockWidget*, Qt::DockWidgetArea> BookmarksManager::docks()
{
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    ret.insert(m_bookmarksDockWidget,  Qt::RightDockWidgetArea);
    return ret;
}
void BookmarksManager::newBookmark(VerseSelection selection)
{
    if(m_bookmarksDockWidget->isHidden()) {
        m_bookmarksDockWidget->show();
    }
    m_bookmarksDockWidget->newBookmark(selection);
}
void BookmarksManager::save()
{
    m_bookmarksDockWidget->saveBookmarks();
}
BookmarksDockWidget* BookmarksManager::bookmarksDockWidget()
{
    return m_bookmarksDockWidget;
}
