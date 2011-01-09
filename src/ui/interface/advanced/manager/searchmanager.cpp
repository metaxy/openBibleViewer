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
#include "searchmanager.h"

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
    //DEBUG_FUNC_NAME
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

void SearchManager::searchInText(SearchQuery query)
{
    DEBUG_FUNC_NAME
    //todo: refractor
    /*if(query.queryType == SearchQuery::Simple) {
        QString s = query.searchText;
        //todo: hacky
        s.remove('*');
        s.remove('?');
        getView()->findText(s, QWebPage::HighlightAllOccurrences);
    }*/
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
