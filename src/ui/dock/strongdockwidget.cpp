#include "strongdockwidget.h"
#include "ui_strongdockwidget.h"

StrongDockWidget::StrongDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::StrongDockWidget)
{
    ui->setupUi(this);
}

StrongDockWidget::~StrongDockWidget()
{
    delete ui;
}

void StrongDockWidget::changeEvent(QEvent *e)
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
