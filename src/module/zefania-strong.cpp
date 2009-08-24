#include "zefania-strong.h"
#include <QtCore/QFile>
#include <QtCore/QtDebug>
#include <QtCore/QVariantList>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QCryptographicHash>
#include <QtCore/QFileInfo>
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
					desc += edetails.text() +"<br /><hr />";
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
	QString path = zefset.homePath + "cache/"+hash.result().toBase64()+".strong";
	QFileInfo info(path);
	if(info.exists())
	{
		QFile::remove(path);
	}
	QFile file(path);
	if(!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("Can not open cache file."));
		return QString();
	}
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
	QFile file(zefset.homePath + "cache/"+hash.result().toBase64()+".strong");
	qDebug() << "zefaniaStrong::loadDataBase fileName3 = " << zefset.homePath + "cache/"+hash.result().toBase64()+".strong";
	if(!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("Can not open cache file."));
		return false;
	}
	m_id.clear();
	m_title.clear();
	m_trans.clear();
	m_pron.clear();
	m_desc.clear();
	QDataStream in(&file);
	in >> m_id;
	in >> m_title;
	in >> m_trans;
	in >> m_pron;
	in >> m_desc;
	qDebug() << "zefaniaStrong::loadDataBase fileName2 = " << file.fileName() << " m_id.size() = "  << m_id.size();
	file.close();
	return true;
}
QString zefaniaStrong::getStrong(QString strongID)
{
	qDebug() << "zefaniaStrong::getStrong strongID = " << strongID;
	for(int i = 0;i < m_id.size();++i)
	{
		QString id = m_id.at(i);
		if(id.trimmed().toLower() == strongID.trimmed().toLower())
		{
			QString ret="<h3>"+m_id.at(i)+" - "+m_title.at(i)+"</h3>";
			if(m_trans.at(i) != "")
			{
				ret += " ("+m_trans.at(i)+") ";
			}
			if(m_pron.at(i) != "")
			{
				ret += " ["+m_pron.at(i)+"] ";
			}
			ret += "<br />"+m_desc.at(i);
			return ret;
		}
	}
	return QString();
}
