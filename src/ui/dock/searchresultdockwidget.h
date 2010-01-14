#ifndef SEARCHRESULTDOCKWIDGET_H
#define SEARCHRESULTDOCKWIDGET_H

#include <QtGui/QDockWidget>
#include <QtGui/QListWidgetItem>
#include "dockwidget.h"
#include "src/core/searchresult.h"
namespace Ui
{
class SearchResultDockWidget;
}

class SearchResultDockWidget : public DockWidget
{
    Q_OBJECT
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
