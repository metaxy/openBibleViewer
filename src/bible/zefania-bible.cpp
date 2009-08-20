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
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtGui/QMessageBox>
#include <QtGui/QProgressDialog>
#include <QtXml/QDomAttr>
#include <QtXml/QDomElement>
#include <QtXml/QtXml>
#include <QCryptographicHash>
#include "zefania-bible.h"
#include "../core/KoXmlWriter.h"
zefaniaBible::zefaniaBible()
{
}
int zefaniaBible::setSettings( struct settings_s settings , struct moduleConfig mConfig_)
{
	zefset = settings;
	mConfig = mConfig_;
	if(mConfig.zefbible_hardCache == false && mConfig.zefbible_softCache == false)
		QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("Please aktivate Caching.(Hard or Soft Cache)"));
	return 1;
}
QDomNode zefaniaBible::readBookFromCache(QString path,int bookID)
{
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(path.toLocal8Bit());
	QString fileName = zefset.homePath + "cache/"+hash.result().toBase64()+"/";
	QDomElement e;
	QFile file(fileName + QString::number(bookID)+".xml");
	qDebug() <<"zefania::readBookFromCache start fileName = " << file.fileName();
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("Can not read the file."));
		qDebug() << "zefania::readBookFromCache() cant read the file";
		return e;
	}
	QDomDocument doc;
	if (!doc.setContent(&file))
	{
		QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("The file is not valid."));
		qDebug() << "zefania::readBookFromCache() the file isnt valid";
		return e;
	}
	QDomElement root = doc.documentElement();
	//cache in ram
	//e = root.firstChild().toElement();
/*	if(softCacheAvi[bookID] == false && mConfig.zefbible_softCache == true)
	{
		softCache[bookID] = e;
		softCacheAvi[bookID] = true;
	}*/

	//qDebug() << "zefania::readBookFromCache()" << doc.toString();
	return root.firstChild();

}
QMap<int,QList<chapter> > zefaniaBible::softCache()
{
	if(mConfig.zefbible_softCache == true)
	{
		return softCacheData;
	}
	return QMap<int,QList<chapter> >();
}
QList<chapter> zefaniaBible::softCache(int bookID)
{	if(mConfig.zefbible_softCache == true)
	{
		return softCacheData[bookID];
	}
	return QList<chapter>();
}
bool zefaniaBible::setSoftCache(QMap< int,QList<chapter> > cache)
{
	if(mConfig.zefbible_softCache == true)
	{
		softCacheData.clear();
		softCacheAvi.clear();
		softCacheData = cache;
		QMapIterator< int,QList<chapter> > i(cache);
		while (i.hasNext())
		{
			softCacheAvi[i.key()] = true;
			i.next();
		}
	}
	return true;
}
bool zefaniaBible::setSoftCache(int bookID,QList<chapter> c)
{
	if(mConfig.zefbible_softCache == true)
	{
		softCacheData[bookID] = c;
		softCacheAvi[bookID] = true;
	}
	return true;
}
bool zefaniaBible::clearSoftCache()
{
	softCacheData.clear();
	softCacheAvi.clear();
	return true;
}
void zefaniaBible::readBook(int id)
{
	qDebug() << "zefania::readBook() id = " << id;

	QDomNode ncache;
	if(mConfig.zefbible_hardCache == true)
	{
		ncache = readBookFromCache(currentBiblePath,id);
	}
	else
	{
		chapterData = softCache(id);
		return;
	}
	qDebug() << "1";
	chapterText.clear();
	chapterData.clear();
	currentBookID = id;
	qDebug() << "1.5";
	if(!ncache.hasChildNodes())
		return;
	qDebug() << "2";
	QDomNode n = ncache.firstChild();
	QString outtext="";
	int count2 = 0;
	qDebug() << "3";
	while(!n.isNull())
	{
		chapter c;
		count2++;outtext="";
		QDomNode n2 = n.firstChild();
		int verseCount = 0;
		while(!n2.isNull())//alle verse
		{
			verseCount++;
			QDomElement e2 = n2.toElement();

			e2 = format(e2);
			c.data <<  e2.text();
			c.verseNumber << e2.attribute("vnumber","");
			n2 = n2.nextSibling();
		}
		c.chapterName = QString::number(count2,10);
		c.verseCount = verseCount;
		c.bookName = bookFullName.at(id);
		chapterData << c;
		chapterText << "<b><font size=\"+5\">"+c.bookName+" "+QString::number(count2)+"</font></b><br /><br />"+outtext;
		n = n.nextSibling();
	}
	book_ccount = count2;
	bookCount[id] = count2;
	setSoftCache(currentBookID,chapterData);
	qDebug() << "zefania::readBook() chapterData.size() = " << chapterData.size();
}
QDomElement zefaniaBible::format(QDomElement e)
{
	QDomNode n = e.firstChild();
	while(!n.isNull())//alle verse
	{
		if(n.nodeName().toLower() == "note")
		{
			QDomNode node = n;
			QDomText t = node.firstChild().toText();
			t.setData("[<font size=\"-2\" <i>"+t.data()+"</i> </font>]");
			node.replaceChild(t,node.firstChild());
			e.replaceChild(node, n);
		}
		else if((n.nodeName().toLower() == "gram" || n.nodeName().toLower() == "gr") && n.toElement().attribute("str","") != "")
		{
			QDomNode node = n;
			QDomText t = n.firstChild().toText();
			QDomElement b = n.toElement();
			t.setData(t.data()+"<sup><a href=\"strong://"+b.attribute("str","")+"\">"+b.attribute("str","")+"</a></sup>  ");
			node.replaceChild(t,node.firstChild());
			e.replaceChild(node, n);
		}
		if(n.childNodes().count() > 0)
			e.replaceChild(format(n.toElement()),n);
		n = n.nextSibling();
	}
	return e;

}
bool zefaniaBible::checkForCacheFiles( QString path)
{
	qDebug() << "zefaniaBible::checkForCacheFiles() path = " << path;
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(path.toLocal8Bit());
	QString fileName = zefset.homePath + "cache/"+hash.result().toBase64()+"/";
	QFile file(fileName+"data");
	if (file.exists())
		return true;
	return false;
}
void zefaniaBible::loadNoCached( int id,QString path)
{
	qDebug("zefania::loadNoCached() start");
	QProgressDialog progress(QObject::tr("Loading Bible"), QObject::tr("Cancel"), 0, 76);
	int progressCounter=0;
	progress.setWindowModality(Qt::WindowModal);
	bibleid = id;
	bookFullName.clear();
	bookShortName.clear();
	bookCount.clear();
	clearSoftCache();
	progress.setValue(1);

	currentBiblePath = path;
	#ifdef KOXML_USE_QDOM
		QMap<int,KoXmlElement> myCache;
	#else
		//hard cache: genrate fileName
		QCryptographicHash hash(QCryptographicHash::Md5);
		hash.addData(path.toLocal8Bit());
		QString fileName = zefset.homePath + "cache/"+hash.result().toBase64()+"/";
		QDir dir;
		dir.mkpath(fileName);
	#endif
	//
	progress.setValue(2);
	QFile file(path);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("Can not read the file"));
		qDebug("zefania::loadBibleData() cant read the file");
		return;
	}
	progress.setValue(3);
	KoXmlDocument doc;
	#ifdef KOXML_USE_QDOM
		if (!doc.setContent(&file))
		{
			QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("The file is not valid"));
			qDebug("zefania::loadBibleData() the file isnt valid");
			return;
		}
	#else
		QStringList fileList;
		QString data;

		QTextCodec *codec = QTextCodec::codecForName("UTF-8");
		QTextDecoder *decoder = codec->makeDecoder();
		while (!file.atEnd())
		{
			QByteArray byteline = file.readLine();
			QString l = decoder->toUnicode(byteline);
			data +=l;
			fileList << l;

		}
		progress.setValue(5);
		if (!doc.setContent(data))
		{
			QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("The file is not valid"));
			qDebug("zefania::loadBibleData() the file isnt valid");
			return;
		}
		data.clear();
	#endif


	progress.setValue(10);
	KoXmlElement root = doc.documentElement();
	bibleName = root.attribute("biblename","");

	KoXmlNode n = doc.documentElement().firstChild();
	progressCounter=10;
	int c = 0;

	while(!n.isNull())
	{
		progressCounter++;
		progress.setValue(progressCounter);
		KoXmlElement e = n.toElement();
		int currentPos = 0;
		if(e.attribute("bname","") != "" || e.attribute("bnumber","") != "")
		{
			//bookCount << "1500";

			#ifdef KOXML_USE_QDOM
				myCache[c] = e;
			#else
				int start=0,end=0;
				for(int i=currentPos;i<fileList.size();++i)
				{
					QString line = fileList.at(i);
					if(line.contains("<BIBLEBOOK",Qt::CaseInsensitive))
					{
						currentPos = i;
						start = i;
						break;
					}

				}
				for(int i=currentPos;i<fileList.size();++i)
				{
					QString line = fileList.at(i);
					if(line.contains("</BIBLEBOOK",Qt::CaseInsensitive))
					{
						currentPos = i;
						end = i;
						break;
					}

				}
				QString data ="<?xml version=\"1.0\"?>\n<cache>\n";
				for(int i = start;i<=end;++i)
				{
					data+= fileList.at(i);
				}
				data += "</cache>";
				QFile file(fileName +"/"+ QString::number(c)+".xml");
				if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
					return;
				QTextStream out(&file);
				out << data;
				file.close();
			#endif
			bookFullName << e.attribute("bname",e.attribute("bsname",""));
			bookShortName << e.attribute("bsname",QObject::tr("(unknown)"));
			c++;
		}
		n = n.nextSibling();
	}
	fileList.clear();
	bool hasAny = false;
	for(int i=0;i<bookFullName.size();i++)
	{
		if(bookFullName.at(i) != "")
		{
			hasAny = true;
			break;
		}
	}
	if(!hasAny)
	{
		if(bookFullName.size() == 66)
		{
			bookFullName.clear();
			//load default booknames
			bookFullName <<  "Genesis";
			bookFullName << "Exodus";
			bookFullName << "Leviticus";
			bookFullName << "Numbers";
			bookFullName << "Deuteronomy";
			bookFullName << "Joshua";
			bookFullName << "Judges";
			bookFullName << "Ruth";
			bookFullName << "1 Samuel";
			bookFullName << "2 Samuel";
			bookFullName << "1 Kings";
			bookFullName << "2 Kings";
			bookFullName << "1 Chronicles";
			bookFullName << "2 Chronicles";
			bookFullName << "Ezrav";
			bookFullName << "Nehemiah";
			bookFullName << "Esther";
			bookFullName << "Job";
			bookFullName << "Psalm";
			bookFullName << "Proverbs";
			bookFullName << "Ecclesiastes";
			bookFullName << "Song of Solomon";
			bookFullName << "Isaiah";
			bookFullName << "Jeremiah";
			bookFullName << "Lamentations";
			bookFullName << "Ezekiel";
			bookFullName << "Daniel";
			bookFullName << "Hosea";
			bookFullName << "Joel";
			bookFullName << "Amos";
			bookFullName << "Obadiah";
			bookFullName << "Jonah";
			bookFullName << "Micah";
			bookFullName << "Nahum";
			bookFullName << "Habakkuk";
			bookFullName << "Zephaniah";
			bookFullName << "Haggai";
			bookFullName << "Zechariah";
			bookFullName << "Malachi";
			bookFullName << "Matthew";
			bookFullName << "Mark";
			bookFullName << "Luke";
			bookFullName << "John";
			bookFullName << "Acts";
			bookFullName << "Romans";
			bookFullName << "1 Corinthians";
			bookFullName << "2 Corinthians";
			bookFullName << "Galatians";
			bookFullName << "Ephesians";
			bookFullName << "Philippians";
			bookFullName << "Colossians";
			bookFullName << "1 Thessalonians";
			bookFullName << "2 Thessalonians";
			bookFullName << "1 Timothy";
			bookFullName << "2 Timothy";
			bookFullName << "Titus";
			bookFullName << "Philemon";
			bookFullName << "Hebrews";
			bookFullName << "James";
			bookFullName << "1 Peter";
			bookFullName << "2 Peter";
			bookFullName << "1 John";
			bookFullName << "2 John";
			bookFullName << "3 John";
			bookFullName << "Jude";
			bookFullName << "Revelation";

		}
	}
	progress.hide();
	file.close();
	if(mConfig.zefbible_hardCache == true)
	{

		//hard cache: write data
		#ifdef KOXML_USE_QDOM
			generateCacheFile(path,bookFullName,myCache);
		#else
			QFile file(fileName+"data");
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
				return;
			QDataStream out(&file);
			out << bookFullName;
			file.close();
		#endif
	}
	qDebug("zefania::loadNoCached() end");
}
void zefaniaBible::generateCacheFile( QString path,QStringList bookFullName_,QMap<int,KoXmlElement> cache)
{
	#ifdef KOXML_USE_QDOM
	QProgressDialog progressCache(QObject::tr( "Generate Cache" ), QObject::tr( "Cancel" ), 0, cache.size()+1);
	progressCache.setWindowModality(Qt::WindowModal);
	progressCache.setValue(1);
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(path.toLocal8Bit());
	QString fileName = zefset.homePath + "cache/"+hash.result().toBase64();
	QDir dir(path);
	dir.mkpath(fileName+"/");

	QFile file(fileName+"/data");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;
	QDataStream out(&file);
	out << bookFullName_;

	QMapIterator<int, KoXmlElement> i(cache);
	int counter=1;
	for(int counter=1;i.hasNext();++counter)
	{
		i.next();
		QDomDocument sdoc("");
		QDomElement root = sdoc.createElement("cache");
		sdoc.appendChild(root);
		root.appendChild(i.value());

		QFile file(fileName +"/"+ QString::number(i.key())+".xml");
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;

		QTextStream out(&file);
		out << "<?xml version=\"1.0\"?>"+sdoc.toString();

		file.close();
		progressCache.setValue(counter);
	}
	progressCache.close();
#else
	Q_UNUSED(path);
	Q_UNUSED(bookFullName_);
	Q_UNUSED(cache);

	#endif
}
void zefaniaBible::loadCached(int id,QString path)
{
	bookFullName.clear();
	bookShortName.clear();
	bookCount.clear();
	clearSoftCache();

	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(path.toLocal8Bit());
	QString fileName = zefset.homePath + "cache/" + hash.result().toBase64()+"/";
	QFile file(fileName+"data");
	if (!file.open(QIODevice::ReadOnly))
		return;
	QDataStream in(&file);
	QStringList str;
	in >> str;
	bookFullName = str;
	bibleid = id;
	currentBiblePath = path;

}
void zefaniaBible::loadBibleData(int id,QString path)
{
	qDebug("zefania::loadCached() start");
	if(mConfig.zefbible_hardCache == true)
	{
		if(checkForCacheFiles(path))
		{
			loadCached(id,path);
		}
		else
		{
			loadNoCached(id,path);
		}

	}
	else
	{
		loadNoCached(id,path);
	}

}
QString zefaniaBible::readInfo(QFile &file)
{
	biblepath = file.fileName();
	QString cbiblename;
	KoXmlDocument doc;
	if (!doc.setContent(&file))
	{
		file.close();
		return "";
	}
	KoXmlElement root = doc.documentElement();
	cbiblename = root.attribute("biblename","");
	bibleName = cbiblename;
	file.close();
	return bibleName;

}
QString zefaniaBible::readInfo(QString content)
{
	biblepath = "";
	QString cbiblename;
	KoXmlDocument doc;
	if (!doc.setContent(content))
	{
		return "";
	}
	KoXmlElement root = doc.documentElement();
	cbiblename = root.attribute("biblename","");
	bibleName = cbiblename;
	return bibleName;
}
struct stelle zefaniaBible::search(QString searchstring,bool regexp,bool whole,bool casesen)
{
	/*if(whole == true)
	{
		searchstring = " "+searchstring+" ";
	}
	lastSearch = searchstring;
	QStringList outlist;
	struct stelle st2;
	if(mConfig.zefbible_hardCache== true)//hard cache
	{
		QProgressDialog progressCache(QObject::tr( "Read Cache" ), QObject::tr( "Cancel" ), 0, bookFullName.size()-1);
		progressCache.setWindowModality(Qt::WindowModal);
		for(int i=0;i<bookFullName.size();i++)
		{
			progressCache.setValue(i);
			if(softCacheAvi[i] == false)
			{
				softCache[i] = readBookFromCache(currentBiblePath,i);
				softCacheAvi[i] = true;
			}
				//todo:make faster and dont read everything everytime from the hard cache
		}
		progressCache.close();
	}
	QProgressDialog progress(QObject::tr( "Searching"), QObject::tr("Cancel"), 0, softCache.size());
	progress.setWindowModality(Qt::WindowModal);
	QMapIterator<int, KoXmlElement> i(softCache);
	while (i.hasNext())//all books
	{
		i.next();
		progress.setValue(i.key());
		if (progress.wasCanceled())
			return st2;
		KoXmlElement ncache = i.value();
		QString outtext="";
		int count2 = 0;
		KoXmlNode n = ncache.firstChild();
		while(!n.isNull())//all chapters
		{
			count2++;outtext="";
			int count3=0;
			KoXmlNode n2 = n.firstChild();
			while(!n2.isNull())//all verses
			{
				count3++;
				KoXmlElement e2 = n2.toElement();
				bool b;
				QString etext = e2.text();
				if(regexp == true)
				{
					b = etext.contains(QRegExp(searchstring));
				}
				else
				{
					if(casesen == true)
					{
						b = etext.contains(searchstring,Qt::CaseSensitive);
					}
					else
					{
						b = etext.contains(searchstring,Qt::CaseInsensitive);
					}
				}

				if(b)
				{
					st2.book << i.key();
					st2.chapter << count2;
					st2.verse << count3;
					st2.text << etext;
				}
				n2 = n2.nextSibling();
			}
			n = n.nextSibling();
		}
	}
	progress.close();
	return st2;*/
}
