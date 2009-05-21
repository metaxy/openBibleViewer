#include "mdiform.h"
#include "ui_mdiform.h"
#include <QtCore/QtDebug>
#include <QtGui/QCloseEvent>

mdiForm::mdiForm(QWidget *parent) :
                QWidget(parent),
                m_ui(new Ui::mdiForm)
{
        m_ui->setupUi(this);
}
void mdiForm::changeEvent(QEvent *e)
{
        qDebug() << "mdiForm::changeEvent() = "<<e->type();
        switch (e->type())
        {
        case QEvent::LanguageChange:
                m_ui->retranslateUi(this);
                break;
        case QEvent::Close:
                qDebug() << "mdiForm::changeEvent() close";
                break;
        default:
                break;
        }
}
void mdiForm::closeEvent(QCloseEvent *event)
{
        qDebug() << "mdiForm::closeEvent()";
        emit onClose();
        event->ignore();
}

mdiForm::~mdiForm()
{
        delete m_ui;
}
