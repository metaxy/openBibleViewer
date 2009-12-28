#ifndef QUICKJUMPDOCKWIDGET_H
#define QUICKJUMPDOCKWIDGET_H

#include <QDockWidget>
#include "dockwidget.h"
namespace Ui
{
class QuickJumpDockWidget;
}

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
