#include "advancedinterface.h"
#include "ui_advancedinterface.h"

AdvancedInterface::AdvancedInterface(QWidget *parent) :
        Interface(parent),
        ui(new Ui::AdvancedInterface)
{
    ui->setupUi(this);
}
void AdvancedInterface::init()
{
}
AdvancedInterface::~AdvancedInterface()
{
    delete ui;
}

void AdvancedInterface::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
