#include "moduledialog.h"
#include "ui_moduledialog.h"

moduleDialog::moduleDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::moduleDialog)
{
    m_ui->setupUi(this);
}

moduleDialog::~moduleDialog()
{
    delete m_ui;
}

void moduleDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
