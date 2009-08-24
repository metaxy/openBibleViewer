#include "mdiform.h"
#include "ui_mdiform.h"
#include <QtCore/QtDebug>


mdiForm::mdiForm(QWidget *parent) :
		QWidget(parent),
		m_ui(new Ui::mdiForm)
{
	m_ui->setupUi(this);
	m_ui->textBrowser->setOpenLinks(false);
	m_ui->textBrowser->setOpenExternalLinks(false);
	connect( m_ui->pushButton_backward, SIGNAL(clicked()), this, SLOT(backward()));
	connect( m_ui->pushButton_forward, SIGNAL(clicked()), this, SLOT(forward()));
	setButtons();

}
/*void mdiForm::changeEvent(QEvent *e)
{
	qDebug() << "mdiForm::changeEvent() = " << e->type();
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
}*/
void mdiForm::historyGetUrl(QString url)
{
	qDebug() <<" mdiForm::historyGetUrl() url = " << url;
	browserHistory.setCurrent(url);
	setButtons();
}
void mdiForm::backward()
{
	qDebug() <<" mdiForm::backward()";
	emit historyGo(browserHistory.backward());
	setButtons();
}
void mdiForm::forward()
{
	qDebug() <<" mdiForm::forward()";
	emit historyGo(browserHistory.forward());
	setButtons();
}
void mdiForm::setButtons()
{
	if(browserHistory.backwardAvailable())
	{
		m_ui->pushButton_backward->setDisabled(false);
	}
	else
	{
		m_ui->pushButton_backward->setDisabled(true);
	}
	if(browserHistory.forwardAvailable())
	{
		m_ui->pushButton_forward->setDisabled(false);
	}
	else
	{
		m_ui->pushButton_forward->setDisabled(true);
	}
}
mdiForm::~mdiForm()
{
	delete m_ui;
}
