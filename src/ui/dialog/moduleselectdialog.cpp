#include "moduleselectdialog.h"
#include "ui_moduleselectdialog.h"

ModuleSelectDialog::ModuleSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModuleSelectDialog)
{
    ui->setupUi(this);
}

ModuleSelectDialog::~ModuleSelectDialog()
{
    delete ui;
}
