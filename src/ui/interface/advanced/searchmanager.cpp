#include "searchmanager.h"
#include "src/core/search/searchquery.h"
#include "src/core/search/search.h"
#include "src/core/dbghelper.h"
#include "src/ui/dialog/searchdialog.h"
SearchManager::SearchManager(QObject *parent) :
    QObject(parent)
{
}
void SearchManager::createDocks()
{
    m_advancedSearchResultDockWidget = new AdvancedSearchResultDockWidget(m_p);
    setAll(m_advancedSearchResultDockWidget);
    m_advancedSearchResultDockWidget->init();
    m_advancedSearchResultDockWidget->hide();
}

QHash<DockWidget*, Qt::DockWidgetArea> SearchManager::docks()
{
    DEBUG_FUNC_NAME
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    ret.insert(m_advancedSearchResultDockWidget, Qt::LeftDockWidgetArea);
    return ret;

}
void SearchManager::setWindowManager(WindowManager *windowManager)
{
    m_windowManager = windowManager;
}
void SearchManager::setWidget(QWidget *p)
{
    m_p = p;
}
void SearchManager::showSearchDialog()
{
    //todo: memory leak?
    SearchDialog *sDialog = new SearchDialog(m_p);
    connect(sDialog, SIGNAL(searched(SearchQuery)), this, SLOT(search(SearchQuery)));
    if(m_windowManager->activeForm()) {
        const QString text = m_windowManager->activeForm()->selectedText();
        if(!text.isEmpty()) {
            sDialog->setText(text);
        }
    }
    sDialog->show();
    sDialog->exec();
}
void SearchManager::search()
{
    DEBUG_FUNC_NAME
    SearchQuery query;
    query.searchText = ((QLineEdit *) sender())->text();
    query.searchInNotes = true;
    query.queryType = SearchQuery::Simple;
    search(query);
}

void SearchManager::search(SearchQuery query)
{
    DEBUG_FUNC_NAME
    m_advancedSearchResultDockWidget->show();
    Search s;
    setAll(&s);
    SearchResult *res = s.search(query);
    m_advancedSearchResultDockWidget->setSearchResult(*res);
}

/*void SearchManager::searchInText(SearchQuery query)
{
    DEBUG_FUNC_NAME
    //todo: refractor
     if(query.queryType == SearchQuery::Simple) {
         QString s = query.searchText;
         //todo: hacky
         s.remove('*');
         s.remove('?');
         getView()->findText(s, QWebPage::HighlightAllOccurrences);
     }
}*/

void SearchManager::nextVerse()
{
    m_advancedSearchResultDockWidget->nextVerse();
}

void SearchManager::previousVerse()
{
    m_advancedSearchResultDockWidget->previousVerse();
}
AdvancedSearchResultDockWidget *SearchManager::advancedSearchResultDockWidget()
{
    return m_advancedSearchResultDockWidget;
}
