#ifndef STRONGDOCKWIDGET_H
#define STRONGDOCKWIDGET_H

#include <QDockWidget>
#include "dockwidget.h"
namespace Ui
{
class StrongDockWidget;
}
/*!
 StrongDockWidget represents a dock widget to show a strong.

*/
class StrongDockWidget : public DockWidget
{
    Q_OBJECT
public:
    StrongDockWidget(QWidget *parent = 0);
    ~StrongDockWidget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::StrongDockWidget *ui;
};

#endif // STRONGDOCKWIDGET_H
