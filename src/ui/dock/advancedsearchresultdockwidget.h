#ifndef ADVANCEDSEARCHRESULTDOCKWIDGET_H
#define ADVANCEDSEARCHRESULTDOCKWIDGET_H

#include <QtGui/QDockWidget>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QItemSelectionModel>
#include "dockwidget.h"
#include "src/core/searchresult.h"

namespace Ui {
    class AdvancedSearchResultDockWidget;
}

class AdvancedSearchResultDockWidget : public DockWidget {
    Q_OBJECT
public:
    explicit AdvancedSearchResultDockWidget(QWidget *parent = 0);
    ~AdvancedSearchResultDockWidget();
    void init();
    void setSearchResult(SearchResult searchResult);
private slots:
    void goToSearchResult(QModelIndex index);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::AdvancedSearchResultDockWidget *ui;
    SearchResult m_searchResult;
    QSortFilterProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;
    QStandardItemModel *m_itemModel;
};

#endif // ADVANCEDSEARCHRESULTDOCKWIDGET_H
