#ifndef MODULEDOCKWIDGET_H
#define MODULEDOCKWIDGET_H

#include <QDockWidget>
#include "dockwidget.h"
namespace Ui
{
class ModuleDockWidget;
}
/*!
 ModuleDockWidget represents a dock widget from which you can load modules

*/
class ModuleDockWidget : public DockWidget
{
    Q_OBJECT
private slots:
    void loadModuleData(QTreeWidgetItem *fitem);
public:
    ModuleDockWidget(QWidget *parent = 0);
    ~ModuleDockWidget();
    void init();
protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void get(QString);
private:
    Ui::ModuleDockWidget *ui;
};

#endif // MODULEDOCKWIDGET_H
