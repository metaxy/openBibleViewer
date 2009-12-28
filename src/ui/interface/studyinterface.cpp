#include "studyinterface.h"
#include "ui_studyinterface.h"

StudyInterface::StudyInterface(QWidget *parent) :
        Interface(parent),
        ui(new Ui::StudyInterface)
{
    ui->setupUi(this);
}
void StudyInterface::init()
{
}

StudyInterface::~StudyInterface()
{
    delete ui;
}

void StudyInterface::changeEvent(QEvent *e)
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
