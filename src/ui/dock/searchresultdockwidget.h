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
#ifndef SEARCHRESULTDOCKWIDGET_H
#define SEARCHRESULTDOCKWIDGET_H

#include <QtGui/QDockWidget>
#include <QtGui/QListWidgetItem>
#include "dockwidget.h"
#include "src/core/search/searchresult.h"
#include "src/core/basicclass.h"
namespace Ui
{
class SearchResultDockWidget;
}
/*!
 SearchResultDockWidget represents a dock widget to show the search results

*/
class SearchResultDockWidget : public DockWidget
{
    Q_OBJECT
public slots:
    void nextVerse();
    void previousVerse();
private slots:
    void goToSearchResult(QListWidgetItem * item);
    void searchInfo();
public:
    SearchResultDockWidget(QWidget *parent = 0);
    ~SearchResultDockWidget();
    void setSearchResult(SearchResult searchResult);


protected:
    void changeEvent(QEvent *e);
signals:
    void get(QString);
private:
    Ui::SearchResultDockWidget *ui;
    SearchResult m_searchResult;
};

#endif // SEARCHRESULTDOCKWIDGET_H
