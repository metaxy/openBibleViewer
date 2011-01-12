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
#ifndef ADVANCEDSEARCHRESULTDOCKWIDGET_H
#define ADVANCEDSEARCHRESULTDOCKWIDGET_H

#include <QtGui/QDockWidget>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QItemSelectionModel>
#include "dockwidget.h"
#include "src/core/search/searchresult.h"
namespace Ui
{
class AdvancedSearchResultDockWidget;
}

class AdvancedSearchResultDockWidget : public DockWidget
{
    Q_OBJECT
public:
    explicit AdvancedSearchResultDockWidget(QWidget *parent = 0);
    ~AdvancedSearchResultDockWidget();
    void init();
    /**
      * Set the SearchResults. E.g from the SearchManager.
      */
    void setSearchResult(SearchResult searchResult);
public slots:
    /**
      * Activates the next search result.
      */
    void nextVerse();
    /**
      * Activates the previous search result.
      **/
    void previousVerse();
private slots:
    void goToSearchResult(QModelIndex index);
    /**
      * Shows the SearchInfoDialog.
      */
    void searchInfo();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::AdvancedSearchResultDockWidget *ui;
    SearchResult m_searchResult;
    QSortFilterProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;
    QStandardItemModel *m_itemModel;
    QModelIndex findNext(QModelIndex start, const int &rowID, const int &level, int currentLevel);
};

#endif // ADVANCEDSEARCHRESULTDOCKWIDGET_H
