#ifndef STRONGDOCKWIDGET_H
#define STRONGDOCKWIDGET_H

#include <QtGui/QDockWidget>
#include <QtCore/QUrl>
#include "dockwidget.h"
#include "src/module/strong.h"
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
private slots:
    void loadModule(int id);
    void search();
    void get_(QUrl url);
public:
    StrongDockWidget(QWidget *parent = 0);
    ~StrongDockWidget();
    void init();
    void showStrong(QString id);
protected:
    void changeEvent(QEvent *e);
signals:
    void get(QUrl);
private:
    Ui::StrongDockWidget *ui;
    Strong m_strong;
    QStringList strongModuleTitle;
    QList<int> strongModuleID;
};

#endif // STRONGDOCKWIDGET_H
