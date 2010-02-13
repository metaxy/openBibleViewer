#ifndef MODULEDOCKWIDGET_H
#define MODULEDOCKWIDGET_H

#include <QDockWidget>
#include <QtGui/QTreeWidgetItem>
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
    void loadedModule(int id);
protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void get(QString);
private:
    Ui::ModuleDockWidget *ui;
    bool m_dontLoad;
    int m_moduleID;
    void iterateTreeWidget(QTreeWidgetItem *parent = 0);
};

#endif // MODULEDOCKWIDGET_H
