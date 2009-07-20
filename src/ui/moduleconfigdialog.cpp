#include "moduleconfigdialog.h"
#include "ui_moduleconfigdialog.h"
#include <QtGui/QFileDialog>

moduleConfigDialog::moduleConfigDialog(QWidget *parent) :
		QDialog(parent),
		m_ui(new Ui::moduleConfigDialog)
{
	m_ui->setupUi(this);
	connect( m_ui->pushButton_ok, SIGNAL( clicked() ), this, SLOT( bsave( ) ) );
	connect( m_ui->pushButton_file, SIGNAL( clicked() ), this, SLOT( fileSelect( ) ) );
	connect( m_ui->comboBox_type, SIGNAL( currentIndexChanged(int) ), this, SLOT(moduleTypeChanged(int) ));
}

moduleConfigDialog::~moduleConfigDialog()
{
	delete m_ui;
}
void moduleConfigDialog::setModule(struct moduleConfig config)
{
	c = config;
	m_ui->lineEdit_name->setText(config.moduleName);
	m_ui->lineEdit_path->setText(config.modulePath);
	m_ui->comboBox_type->setCurrentIndex(config.moduleType.toInt());
	m_ui->comboBox_textFromatting->setCurrentIndex(config.zefbible_textFormatting);
	if(config.biblequote_removeHtml == true)
	{
		m_ui->checkBox_removeHtml->setChecked(true);
	}
	else
	{
		m_ui->checkBox_removeHtml->setChecked(false);
	}
}
void moduleConfigDialog::bsave()
{	c.moduleName = m_ui->lineEdit_name->text();
	c.modulePath = m_ui->lineEdit_path->text();
	c.moduleType = QString::number(m_ui->comboBox_type->currentIndex());
	c.zefbible_textFormatting = m_ui->comboBox_textFromatting->currentIndex();
	c.biblequote_removeHtml = m_ui->checkBox_removeHtml->isChecked();
	emit save(c);
}
void moduleConfigDialog::moduleTypeChanged(int id)
{
	//qDebug() << "moduleConfigDialog::moduleTypeChanged id1 = " << id;
	m_ui->groupBox_bq->setVisible(false);
	m_ui->groupBox_zefBible->setVisible(false);
	id = m_ui->comboBox_type->currentIndex();
	//qDebug() << "moduleConfigDialog::moduleTypeChanged id2 = " << id;
	switch( id )
	{
	case 1:
		m_ui->groupBox_bq->setVisible(true);
		break;
	case 2:
		m_ui->groupBox_zefBible->setVisible(true);
		break;

	}
}
void  moduleConfigDialog::fileSelect()
{
	QString fileName = QFileDialog::getOpenFileName(this,tr("Open Bible"), c.modulePath, tr("Bibles (*.ini *.xml *.*)"));
	if(fileName != "")
	{
		m_ui->lineEdit_path->setText(fileName);
	}
	return;
}
void moduleConfigDialog::changeEvent(QEvent *e)
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
