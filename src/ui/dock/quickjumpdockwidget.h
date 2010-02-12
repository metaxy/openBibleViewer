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
private slots:
    void goToPos();
public:
    QuickJumpDockWidget(QWidget *parent = 0);
    ~QuickJumpDockWidget();

protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void get(QString url);
private:
    Ui::QuickJumpDockWidget *ui;
};

#endif // QUICKJUMPDOCKWIDGET_H
