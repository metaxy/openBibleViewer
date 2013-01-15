/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#include "searchmanager.h"
#include <QtCore/QPointer>
#include <CLucene.h>
SearchManager::SearchManager(QObject *parent) :
    QObject(parent)
{
}

void SearchManager::init()
{
    connect(m_actions, SIGNAL(_searchInText()), this, SLOT(searchInText()));
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
    QPointer<SearchDialog> sDialog = new SearchDialog(m_p, this);
    connect(sDialog, SIGNAL(searched(SearchQuery)), this, SLOT(search(SearchQuery)));
    if(m_windowManager->activeForm()) {
        const QString text = m_windowManager->activeForm()->selectedText();
        if(!text.isEmpty()) {
            sDialog->setText(text);
        }
    }
    sDialog->show();
   // delete sDialog;
}

void SearchManager::search()
{
    //DEBUG_FUNC_NAME
    SearchQuery query;
    query.searchText = ((QLineEdit *) sender())->text();
    query.searchInNotes = true;
    query.queryType = SearchQuery::Simple;
    search(query);
}

void SearchManager::search(SearchQuery query)
{
    DEBUG_FUNC_NAME;

    m_advancedSearchResultDockWidget->show();
    Search s;
    setAll(&s);
    myDebug() << m_windowManager->activeForm()->type();

    if(m_windowManager->activeForm())
        s.addModule(m_windowManager->activeForm()->searchableModule());
    SearchResult *res = NULL;

    try {
         res = s.search(query);
    }
    catch(CLuceneError &err) {
        myWarning() << "search failed: clucene error = " << err.what();
    }
    catch(...) {
        myWarning() << "search failed";
    }

    m_advancedSearchResultDockWidget->setSearchResult(res);
}

void SearchManager::searchInText(SearchResult *res)
{
    DEBUG_FUNC_NAME;
    m_actions->searchInText(res);
}

void SearchManager::searchInText()
{
    DEBUG_FUNC_NAME
    m_actions->searchInText(m_advancedSearchResultDockWidget->currentResult());
}

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
