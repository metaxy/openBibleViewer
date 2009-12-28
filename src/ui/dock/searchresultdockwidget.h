#ifndef SEARCHRESULTDOCKWIDGET_H
#define SEARCHRESULTDOCKWIDGET_H

#include <QDockWidget>
#include "dockwidget.h"
namespace Ui
{
class SearchResultDockWidget;
}

class SearchResultDockWidget : public DockWidget
{
    Q_OBJECT
public:
    SearchResultDockWidget(QWidget *parent = 0);
    ~SearchResultDockWidget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SearchResultDockWidget *ui;
};

#endif // SEARCHRESULTDOCKWIDGET_H
