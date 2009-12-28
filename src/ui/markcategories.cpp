#include "markcategories.h"
#include "ui_markcategories.h"

MarkCategories::MarkCategories(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::MarkCategories)
{
    ui->setupUi(this);
}

MarkCategories::~MarkCategories()
{
    delete ui;
}

void MarkCategories::changeEvent(QEvent *e)
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
