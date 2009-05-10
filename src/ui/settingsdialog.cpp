/***************************************************************************
openBibleViewer - Free Bibel Viewer
Copyright (C) 2009 Paul Walger
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "../kernel/config.h"
#include "../bible/zefania.h"
#include "../bible/biblequote.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QtDebug>
#include <QProgressDialog>
#include <QStringList>


settingsDialog::settingsDialog(QWidget *parent) : QDialog(parent), m_ui(new Ui::settingsDialog)
{
	m_ui->setupUi(this);
	connect( m_ui->pushButton_remove_path, SIGNAL( clicked() ), this, SLOT( removePath( ) ) );
	connect( m_ui->pushButton_add_path, SIGNAL( clicked() ), this, SLOT( addPath( ) ) );

	connect( m_ui->pushButton_add_module, SIGNAL( clicked() ), this, SLOT( addModule( ) ) );
	connect( m_ui->pushButton_remove_module, SIGNAL( clicked() ), this, SLOT( removeModule( ) ) );
	connect( m_ui->pushButton_save, SIGNAL( clicked() ), this, SLOT( bsave( ) ) );
	connect( m_ui->pushButton_reset, SIGNAL( clicked() ), this, SLOT( reset( ) ) );
}

settingsDialog::~settingsDialog()
{
    delete m_ui;
}
void settingsDialog::reset()
{
	set = backupSet;
	setSettings(&set);
}
int settingsDialog::setSettings(settings_s *settings)
{
	set = *settings;
	backupSet = set;
	encodings.clear();
	langCode.clear();
	//Allgemeine
	////Encoding
	encodings << "Apple Roman" << "Big5" << "Big5-HKSCS" << "EUC-JP" << "EUC-KR" << "GB18030-0" << "IBM 850"
			<< "IBM 866" << "IBM 874" << "ISO 2022-JP" << "ISO 8859-1" << "ISO 8859-2" << "ISO 8859-3" << "ISO 8859-4"
			<< "ISO 8859-5" << "ISO 8859-6" << "ISO 8859-7" << "ISO 8859-8" << "ISO 8859-9" << "ISO 8859-10"
			<< "ISO 8859-13" << "ISO 8859-14" << "ISO 8859-15" << "ISO 8859-16" << "Iscii-Bng" << "Dev" << "Gjr"
			<< "Knd" << "Mlm" << "Ori" << "Pnj" << "Tlg" << "Tml" << "JIS X 0201" << "JIS X 0208" << "KOI8-R"
			<< "KOI8-U" << "MuleLao-1" << "ROMAN8" << "Shift-JIS" << "TIS-620" << "TSCII" << "UTF-8" << "UTF-16"
			<< "UTF-16BE" << "UTF-16LE" << "UTF-32" << "UTF-32BE" << "UTF-32LE" << "Windows-1250" << "Windows-1251" << "Windows-1252"
			<< "Windows-1253" << "Windows-1254" << "Windows-1255" << "Windows-1256" << "Windows-1257" << "Windows-1258" << "WINSAMI2";
	m_ui->comboBox_encoding->clear();
	m_ui->comboBox_encoding->insertItems(0,encodings);
	m_ui->comboBox_encoding->setCurrentIndex(encodings.lastIndexOf(set.encoding));
	////Ordner
	m_ui->listWidget_path->clear();
	m_ui->listWidget_path->insertItems(0,set.path);
	//Module
	m_ui->treeWidget_module->clear();
	QList<QTreeWidgetItem *> items;
	for(int i=0;i< set.modulePath.size();i++)
	{
		QTreeWidgetItem *ibible = new QTreeWidgetItem(m_ui->treeWidget_module);
		ibible->setText(0, set.moduleName.at(i) );
		ibible->setText(1, set.modulePath.at(i) );
		QString moduleType;
		switch(set.moduleType.at(i).toInt())
		{
			case 1:
				moduleType = "Bible Quote";
				break;
			case 2:
				moduleType = "Zefania XML";
				break;

		}
		ibible->setText(2,moduleType);
		items << ibible;
	}
	m_ui->treeWidget_module->insertTopLevelItems(0, items);
	QStringList langs;
	langs << "German" << "English" << "Russian";
	langCode << "de" << "en" << "ru";
	qDebug() << "settingsDialog::setSettings() langCode.lastIndexOf(set.language) = " << langCode.lastIndexOf(set.language) << " set.language = " <<set.language;
	m_ui->comboBox_language->clear();
	m_ui->comboBox_language->insertItems(0,langs);
	m_ui->comboBox_language->setCurrentIndex(langCode.lastIndexOf(set.language));


	return 0;
	//Path

}
void settingsDialog::addPath( void )
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	if(dialog.exec())
	{
		QStringList fileName = dialog.selectedFiles();
		if(fileName.size() > 0)
		{
			QStringList fileName2;
			for(int i = 0;i<fileName.size();i++)
			{
				QString f = fileName.at(i);
				if(f.endsWith(set.dict))
				{
					f.remove(f.size()-1,10);
				}
				fileName2 << f;
			}
			set.path << fileName2;
			m_ui->listWidget_path->clear();
			m_ui->listWidget_path->insertItems(0,set.path);
		}
	}
	return;
}
void settingsDialog::removePath()
{
	int row = m_ui->listWidget_path->currentRow();

	//remove from listWidget
	QListWidgetItem * token = m_ui->listWidget_path->takeItem(row);
	delete token;
	//remove from set
	set.path.removeAt(row);
	return;
}
void settingsDialog::addModule( void )
{
	QFileDialog dialog(this);
	QList<QTreeWidgetItem *> items;

	dialog.setFileMode(QFileDialog::ExistingFiles);
	if(dialog.exec())
	{
		QStringList fileName = dialog.selectedFiles();
		if(fileName.size() > 0)
		{
                        QProgressDialog progress(QObject::tr( "Searching"), QObject::tr("Cancel"), 0, fileName.size());
                        progress.setWindowModality(Qt::WindowModal);
			for(int i=0;i< fileName.size();i++)
			{
                                progress.setValue(i);
                                if (progress.wasCanceled())
                                        return;
				QString f = fileName.at(i);
				QString fileData;
				QString bibleName;
                                int imoduleType = 0;
				//open file
				QFile file(f);
				if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
					return;
				QTextStream in(&file);
				while (!in.atEnd())
				{
					QString line = in.readLine();
					fileData+=line;
				}
				if(fileData.contains("BookQty",Qt::CaseInsensitive))
				{
					imoduleType = 1;//BibleQuote
				}
				else if(fileData.contains("XMLBIBLE",Qt::CaseInsensitive))
				{
					imoduleType = 2;//Zefania
				}
                                //QFile iFile(f);
				biblequote bq;
				zefania zef;
				QString moduleType;
				switch(imoduleType)
				{
					case 1:
                                                bibleName = bq.readInfo(file);
						moduleType = "Bible Quote";
						break;
					case 2:
                                                bibleName = zef.readInfo(file);
						moduleType = "Zefania XML";
						break;

				}
				qDebug() << "settingsDialog::addModule() new Module file"<< f << " moduleName" << bibleName << " moduleType" << moduleType;
                                if (progress.wasCanceled())
                                        return;
				set.modulePath << f;
				set.moduleName << bibleName;
				set.moduleType << QString::number(imoduleType);
				QTreeWidgetItem *ibible = new QTreeWidgetItem(m_ui->treeWidget_module);
                                ibible->setText(0, set.moduleName.at(i) );
                                ibible->setText(1, set.modulePath.at(i) );
				ibible->setText(2, moduleType);
				items << ibible;
			}
                        progress.close();
			m_ui->treeWidget_module->insertTopLevelItems(0, items);
		}

	}
	return;
}
void settingsDialog::removeModule()
{
	int row = m_ui->treeWidget_module->indexOfTopLevelItem(m_ui->treeWidget_module->currentItem());

	//remove from listWidget
	QTreeWidgetItem * token = m_ui->treeWidget_module->currentItem();
	delete token;
	//remove from settings
	set.modulePath.removeAt(row);
	set.moduleName.removeAt(row);
	set.moduleType.removeAt(row);
	return;
}
int settingsDialog::bsave( void )
{

	set.encoding = encodings.at(m_ui->comboBox_encoding->currentIndex());
	set.language = langCode.at(m_ui->comboBox_language->currentIndex());
	qDebug("settingsDialog::bsave() set.encoding = %s",set.encoding.toStdString().c_str());
	emit save(&set);
	return 0;
}
void settingsDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
