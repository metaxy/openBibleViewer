#include "marklist.h"
#include "ui_marklist.h"

MarkList::MarkList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MarkList)
{
    ui->setupUi(this);
}

MarkList::~MarkList()
{
    delete ui;
}

void MarkList::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
