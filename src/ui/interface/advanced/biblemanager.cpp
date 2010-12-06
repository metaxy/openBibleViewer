#include "biblemanager.h"
#include "src/core/dbghelper.h"
BibleManager::BibleManager(QObject *parent) :
    QObject(parent)
{
}
void BibleManager::init()
{

}
void BibleManager::createDocks()
{
    m_moduleDockWidget = new ModuleDockWidget(this->parentWidget());
    setAll(m_moduleDockWidget);
    m_moduleDockWidget->init();
    connect(m_moduleDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    m_bookDockWidget = new BookDockWidget(this->parentWidget());
    setAll(m_bookDockWidget);
    m_bookDockWidget->hide();
    connect(m_bookDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    m_advancedSearchResultDockWidget = new AdvancedSearchResultDockWidget(this->parentWidget());
    setAll(m_advancedSearchResultDockWidget);
    m_advancedSearchResultDockWidget->init();
    m_advancedSearchResultDockWidget->hide();
    connect(m_advancedSearchResultDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));

    m_quickJumpDockWidget = new QuickJumpDockWidget(this->parent()->parentWidget());
    setAll(m_quickJumpDockWidget);
    m_quickJumpDockWidget->init();
    m_quickJumpDockWidget->hide();
    connect(m_quickJumpDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));
}

QHash<DockWidget*, Qt::DockWidgetArea> BibleManager::docks()
{
    DEBUG_FUNC_NAME
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    ret.insert(m_advancedSearchResultDockWidget, Qt::LeftDockWidgetArea);
    ret.insert(m_bookDockWidget, Qt::LeftDockWidgetArea);
    ret.insert(m_moduleDockWidget, Qt::LeftDockWidgetArea);
    ret.insert(m_quickJumpDockWidget, Qt::RightDockWidgetArea);
    return ret;

}
void BibleManager::pharseUrl(const QString &url)
{
}
