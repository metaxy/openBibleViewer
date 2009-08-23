#include "zefania-strong.h"
#include <QtCore/QFile>
#include <QtCore/QtDebug>
#include <QtCore/QVariantList>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QCryptographicHash>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressDialog>
/*#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlTableModel>*/
#include "../core/KoXmlReader.h"
zefaniaStrong::zefaniaStrong()
{
}
int zefaniaStrong::setSettings( struct settings_s settings , struct moduleConfig mConfig_)
{
	zefset = settings;
	mConfig = mConfig_;
	return 1;
}
QString zefaniaStrong::loadFile(QString fileData,QString fileName)
{
	KoXmlDocument doc;
	if (!doc.setContent(fileData))
	{
		QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("The file is not valid"));
		qDebug("zefaniaStrong::loadFile() the file isnt valid");
		return QString();
	}
	QString fileTitle="";
	QStringList l_id,l_title,l_trans,l_pron,l_desc;
	KoXmlNode item = doc.documentElement().firstChild();
	for(int c = 0;!item.isNull();)
	{
		QString id="",title="",trans="",pron="",desc="";
		KoXmlElement e = item.toElement();
		if(e.tagName() == "INFORMATION")
		{
			KoXmlNode titel = item.namedItem("title");
			fileTitle = titel.toElement().text();
		}
		else if(e.tagName() == "item")
		{
			id = e.attribute("id");
			KoXmlNode details = item.firstChild();
			while(!details.isNull())
			{
				KoXmlElement edetails = details.toElement();
				if(edetails.tagName() == "title")
					title = edetails.text();
				else if(edetails.tagName() == "transliteration")
					trans =  edetails.text();
				else if(edetails.tagName() == "pronunciation")
				{
					KoXmlNode em = details.firstChild();
					while(!em.isNull())
					{
						if(em.toElement().tagName() == "em")
							pron = "<em>" + em.toElement().text()+ "</em>";
						em = em.nextSibling();
					}
				}
				else if(edetails.tagName() == "description")
				{
					desc = edetails.text();
				}

				details = details.nextSibling();
			}
			l_id << id;
			l_title << title;
			l_trans << trans;
			l_pron << pron;
			l_desc << desc;
		}
		item = item.nextSibling();
		c++;
	}
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(fileName.toLocal8Bit());
	//todo:check if file exists
	QFile file(zefset.homePath + "cache/"+hash.result().toBase64()+".strong");
	file.open(QIODevice::WriteOnly);
	QDataStream out(&file);
	out << l_id;
	out << l_title;
	out << l_trans;
	out << l_pron;
	out << l_desc;
	file.close();
	return fileTitle;
}
bool zefaniaStrong::loadDataBase(QString fileName)
{
	qDebug() << "zefaniaStrong::loadDataBase fileName = " << fileName;
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(fileName.toLocal8Bit());
	//todo:check if file exists
	QFile file(zefset.homePath + "cache/"+hash.result().toBase64()+".strong");
	qDebug() << "zefaniaStrong::loadDataBase fileName3 = " << zefset.homePath + "cache/"+hash.result().toBase64()+".strong";
	if(!file.open(QIODevice::ReadOnly))
		return false;
	QDataStream in(&file);
	in >> m_id;
	in >> m_title;
	in >> m_trans;
	in >> m_pron;
	in >> m_desc;
	qDebug() << "zefaniaStrong::loadDataBase fileName2 = " << file.fileName() << " m_id.size() = "  << m_id.size();
	file.close();
	return true;
	//open database
}
QString zefaniaStrong::getStrong(QString strongID)
{
	qDebug() << "zefaniaStrong::getStrong strongID = " << strongID;
	for(int i = 0;i < m_id.size();++i)
	{
		if(m_id.at(i) == strongID)
		{
			return m_title.at(i);//todo: das ist nicht genug
		}
	}
	return QString();
}
