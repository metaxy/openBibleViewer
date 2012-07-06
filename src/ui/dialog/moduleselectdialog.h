#ifndef MODULESELECTDIALOG_H
#define MODULESELECTDIALOG_H

#include <QDialog>
#include "src/core/basicclass.h"
#include "src/ui/moduleselectui.h"
namespace Ui {
class ModuleSelectDialog;
}

class ModuleSelectDialog : public QDialog, public BasicClass
{
    Q_OBJECT
    
public:
    explicit ModuleSelectDialog(BasicClass *basic, QWidget *parent = 0);
    ~ModuleSelectDialog();
private slots:
    void ok();
    void configure();
signals:
    void moduleSelected(const int moduleID);
private:
    ModuleSelectUI *m_moduleUI;
    Ui::ModuleSelectDialog *ui;
};

#endif // MODULESELECTDIALOG_H
