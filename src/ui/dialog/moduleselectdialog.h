#ifndef MODULESELECTDIALOG_H
#define MODULESELECTDIALOG_H

#include <QDialog>

namespace Ui {
class ModuleSelectDialog;
}

class ModuleSelectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ModuleSelectDialog(QWidget *parent = 0);
    ~ModuleSelectDialog();
    
private:
    Ui::ModuleSelectDialog *ui;
};

#endif // MODULESELECTDIALOG_H
