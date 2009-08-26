#include "zefania-strong.h"
#include <QtCore/QFile>
#include <QtCore/QtDebug>
#include <QtCore/QVariantList>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QCryptographicHash>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressDialog>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
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
	QDomDocument ddd;
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
					KoXmlNode descNode = details.firstChild();
					while(!descNode.isNull())
					{
						if(descNode.nodeType() == 2)
						{
							desc += descNode.toText().data();
						}
						else if(descNode.nodeType() == 1)
						{
							KoXmlElement descElement  = descNode.toElement();
							if(descElement.tagName() == "reflink")
							{
								QString mscope = descElement.attribute("mscope",";;;");
								QStringList list = mscope.split(";");
								int bookID = list.at(0).toInt() - 1;
								int chapterID = list.at(1).toInt() - 1;
								int verseID = list.at(2).toInt() - 1;
								QString url = "bible://current/"+QString::number(bookID)+","+QString::number(chapterID)+","+QString::number(verseID)+",";
								QString name;

								if(bookID < zefset.bookNames.size())
								{
									name = zefset.bookNames.at(bookID)+" "+list.at(1)+","+list.at(2);
								}
								else
								{
									name = list.at(0)+" "+list.at(1)+","+list.at(2);
								}
								desc += " <a href=\""+url+"\">"+name+"</a> ";
							}
						}
						descNode = descNode.nextSibling();
					}
					desc += "<hr />";
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
	doc.clear();
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(fileName.toLocal8Bit());
	QString path = zefset.homePath + "cache/"+hash.result().toBase64()+".strong";
	QFileInfo info(path);
	if(info.exists())
	{
		QFile::remove(path);
	}
	QDir dir;
	dir.mkpath(zefset.homePath + "cache/");
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
QDomElement zefaniaStrong::format(QDomElement e)
{
	//qDebug() << "zefaniaStrong::format name = " << e.tagName();
	QDomNode n = e.firstChild();
	QDomDocument doc;
	while(!n.isNull())//alle verse
	{
		if(n.nodeName().toLower() == "title" )
		{
			QDomNode node = n;
			QDomText t = node.firstChild().toText();

			t.setData("<font size=\"+1\">"+t.data()+"</font>");
			node.replaceChild(t,node.firstChild());
			e.replaceChild(node, n);
		}
		else if(n.nodeName().toLower() == "reflink" )
		{
			QDomNode node = n;

			QDomElement b = n.toElement();
			QString mscope = b.attribute("mscope","");
			QStringList list = mscope.split(";");
			QString url = "bible://current/"/*+list.at(0)+","+list.at(1)+","+list.at(2)+","*/;
			QDomNode newE = doc.createTextNode("<a href=\""+url+"\">Link</a>");
			e.replaceChild(newE,n);
		}
		/*else if((n.nodeName().toLower() == "gram" || n.nodeName().toLower() == "gr") && n.toElement().attribute("str","") != "" && mConfig.zefbible_showStrong == true)
		{
			QDomNode node = n;
			QDomText t = n.firstChild().toText();
			QDomElement b = n.toElement();
			QString add;
			if(currentBookID < 39)
				add = "H";
			else
				add = "G";

			t.setData(t.data()+"<sup><a href=\"strong://"+add+b.attribute("str","")+"\">"+add+b.attribute("str","")+"</a></sup>  ");
			node.replaceChild(t,node.firstChild());
			e.replaceChild(node, n);
		}*/
		if(n.childNodes().count() > 0)
			e.replaceChild(format(n.toElement()),n);
		n = n.nextSibling();
	}
	return e;

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

