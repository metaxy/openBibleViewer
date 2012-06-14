#include "moduleselectdialog.h"
#include "ui_moduleselectdialog.h"

ModuleSelectDialog::ModuleSelectDialog(BasicClass *basic, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModuleSelectDialog)
{
    ui->setupUi(this);
    copy(basic);

    ui->treeView->clearSelection();
    m_moduleUI = new ModuleSelectUI(this, this);

   // connect(ui->lineEdit_filter, SIGNAL(textChanged(QString)), this, SLOT(filter(QString)));

 //   connect(ui->treeView_module, SIGNAL(activated(QModelIndex)), this, SLOT(loadModuleData(QModelIndex)));
    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));

    connect(m_moduleUI, SIGNAL(configure()), this, SLOT(configure()));
  //  connect(m_moduleUI, SIGNAL(open()), this, SLOT(open()));
  //  connect(m_moduleUI, SIGNAL(openInNewTab()), this, SLOT(openInNewTab()));


    ui->treeView->setSortingEnabled(true);
    ui->treeView->setModel(m_moduleUI->model());
    ui->treeView->setSelectionModel(m_moduleUI->selectionModel());

    m_moduleUI->model()->sort(0);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(close()));

}

ModuleSelectDialog::~ModuleSelectDialog()
{
    delete ui;
}
void ModuleSelectDialog::ok()
{

    emit moduleSelected(1);
}
