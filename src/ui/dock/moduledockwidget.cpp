#include "moduledockwidget.h"
#include "ui_moduledockwidget.h"
#include "src/core/dbghelper.h"
ModuleDockWidget::ModuleDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::ModuleDockWidget)
{
    ui->setupUi(this);
    connect(ui->treeWidget_bibles, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(loadModuleData(QTreeWidgetItem*)));
}
void ModuleDockWidget::init()
{
    ui->treeWidget_bibles->clear();
    ui->treeWidget_bibles->insertTopLevelItems(0, m_moduleManager->m_items);
    ui->treeWidget_bibles->sortByColumn(0, Qt::AscendingOrder);//sort
}
void ModuleDockWidget::loadModuleData(QTreeWidgetItem *fitem)
{
    DEBUG_FUNC_NAME
    m_bibleDisplay->setHtml("");
    emit get("bible://" + fitem->text(1) + "/0,0,0");
}
ModuleDockWidget::~ModuleDockWidget()
{
    delete ui;
}

void ModuleDockWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
