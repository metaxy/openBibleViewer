#include "quickjumpdockwidget.h"
#include "ui_quickjumpdockwidget.h"

QuickJumpDockWidget::QuickJumpDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::QuickJumpDockWidget)
{
    ui->setupUi(this);
}

QuickJumpDockWidget::~QuickJumpDockWidget()
{
    delete ui;
}

void QuickJumpDockWidget::changeEvent(QEvent *e)
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
