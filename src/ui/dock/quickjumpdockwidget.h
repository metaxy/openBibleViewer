#ifndef QUICKJUMPDOCKWIDGET_H
#define QUICKJUMPDOCKWIDGET_H

#include <QDockWidget>
#include "dockwidget.h"
namespace Ui
{
class QuickJumpDockWidget;
}
/*!
 ModuleDockWidget represents a dock widget from which you can jump fast to certain position in the module

*/
class QuickJumpDockWidget : public DockWidget
{
    Q_OBJECT
public:
    QuickJumpDockWidget(QWidget *parent = 0);
    ~QuickJumpDockWidget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QuickJumpDockWidget *ui;
};

#endif // QUICKJUMPDOCKWIDGET_H
