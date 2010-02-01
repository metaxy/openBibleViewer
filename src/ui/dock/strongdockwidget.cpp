#include "strongdockwidget.h"
#include "ui_strongdockwidget.h"

StrongDockWidget::StrongDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::StrongDockWidget)
{
    ui->setupUi(this);
    ui->comboBox_strong->clear();
    ui->comboBox_strong->insertItems(0, m_moduleManager->m_strong.loadStrongs());

}

StrongDockWidget::~StrongDockWidget()
{
    delete ui;
}
/*StrongDockWidget::search()
{
}
StrongDockWidget::search(QString id)
{

}
*/
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
