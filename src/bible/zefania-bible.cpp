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
zefaniaBible::zefaniaBible()
{
}
int zefaniaBible::setSettings( struct settings_s *settings )
{
	zefset = *settings;
	return 1;
}
QDomElement zefaniaBible::readBookFromCache(QString path,int bookID)
{
	if(cache_books_dataB[bookID] == true)
	{
		return cache_books_data[bookID];
	}
	QDomNode n;QDomElement e;
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(path.toLocal8Bit());
	QString fileName = zefset.homePath + "cache/"+hash.result().toBase64()+"/";

	QFile file(fileName + QString::number(bookID)+".xml");
	qDebug() <<"zefania::readBookFromCache start fileName = " << file.fileName();
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("Can not read the file"));
		qDebug() << "zefania::readBookFromCache() cant read the file";
		return n.toElement();
	}
	QDomDocument doc;
	if (!doc.setContent(&file))
	{
		QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("The file is not valid"));
		qDebug() << "zefania::readBookFromCache() the file isnt valid";
		return n.toElement();
	}
	QDomElement root = doc.documentElement();
	n = root.firstChild();
	e = n.toElement();
	//cache in ram
	cache_books_data[bookID] = e;
	cache_books_dataB[bookID] = true;

	//qDebug() << "zefania::readBookFromCache()" << doc.toString();
	return e;

}
void zefaniaBible::readBook(int id)
{
	QDomElement ncache;
	if(zefset.zefaniaBible_Cache == true)
	{
		ncache = readBookFromCache(currentBiblePath,id);
	}
	else
	{
		ncache = cache_books_data[id];
	}
	qDebug() << "zefania::readBook() id = " << id;
	chapterText.clear();
	chapterData.clear();
	currentBookID = id;

	QDomNode n = ncache.firstChild();
	QString outtext="";
	int count2 = 0;
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
			QString data ="";
			QDomNodeList a1 = e2.elementsByTagName("gr");
			QDomNodeList a2 = e2.elementsByTagName("GRAM");
			if(a1.size() != 0 || a2.size() != 0)
			{
				QDomNodeList a;
				if(a1.size() != 0)
					a = a1;
				else
					a = a2;
				//with strong tags
				for(int i=0;i<a.size();i++)
				{
					QDomElement b = a.at(i).toElement();
					data += b.text()+"<sup><a href=\"strong://"+b.attribute("str","")+"\">"+b.attribute("str","")+"</a></sup>  ";
				}
			}
			else
			{
				data = e2.text();
			}
			c.data << data;
			c.verseNumber << e2.attribute("vnumber","");
			n2 = n2.nextSibling();
		}
		c.chapterName = QString::number(count2,10);
		c.verseCount = verseCount;
		c.bookName = bookFullName.at(id);
		chapterData << c;
		chapterText << "<b><font size=\"+5\">"+bookFullName.at(id)+" "+QString::number(count2,10)+"</font></b><br /><br />"+outtext;
		n = n.nextSibling();
	}
	book_ccount = count2;
	bookCount[id] = count2;
	qDebug() << "zefania::readBook() chapterData.size() = " << chapterData.size();
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
	bookPath.clear();
	bookShortName.clear();
	bookCount.clear();
	cache_books_data.clear();
	progress.setValue(1);

	currentBiblePath = path;

	progress.setValue(5);
	QFile file(path);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("Can not read the file"));
		qDebug("zefania::loadBibleData() cant read the file");
		return;
	}
	QDomDocument doc;
	if (!doc.setContent(&file))
	{
		QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("The file is not valid"));
		qDebug("zefania::loadBibleData() the file isnt valid");
		return;
	}
	progress.setValue(10);
	QDomElement root = doc.documentElement();
	bibleName = root.attribute("biblename","");

	QDomNode n = doc.documentElement().firstChild();
	progressCounter=10;
	int i = 0;
	while(!n.isNull())
	{
		progressCounter++;
		progress.setValue(progressCounter);
		QDomElement e = n.toElement();
		if(e.attribute("bname","") != "" || e.attribute("bnumber","") != "")
		{
			//bookCount << "1500";
			cache_books_data[i] = e;
			bookFullName << e.attribute("bname",e.attribute("bsname",""));
			bookShortName << e.attribute("bsname",QObject::tr("(unknown)"));
			i++;
		}
		n = n.nextSibling();
	}
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
	if(zefset.zefaniaBible_Cache == true)
	{
		generateCacheFile(path,bookFullName,cache_books_data);
	}
	qDebug("zefania::loadBibleData() exit");
}
void zefaniaBible::generateCacheFile( QString path,QStringList bookFullName_,QMap<int,QDomElement> cache)
{
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(path.toLocal8Bit());
	QString fileName = zefset.homePath + "cache/"+hash.result().toBase64()+"/";
	QDir a;
	a.mkpath(fileName);

	qDebug() << "zefaniaBible::generateCacheFile() fileName = " << fileName;
	QFile file(fileName+"data");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QDataStream out(&file);
	out << bookFullName_;

	QMapIterator<int, QDomElement> i(cache);
	while (i.hasNext())
	{
		i.next();
		QDomDocument sdoc;
		QDomElement root = sdoc.createElement("cache");
		sdoc.appendChild(root);
		root.appendChild(i.value());
		QFile file(fileName + QString::number(i.key())+".xml");
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;
		QTextStream out(&file);
		out << "<?xml version=\"1.0\"?>"+sdoc.toString();
		file.close();
	}
	qDebug() << "zefaniaBible::generateCacheFile() written";
}
void zefaniaBible::loadCached(int id,QString path)
{
	qDebug("zefania::loadCached() start");
	bookFullName.clear();
	bookPath.clear();
	bookShortName.clear();
	bookCount.clear();
	cache_books_data.clear();

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
	qDebug() << "zefania::loadCached() bookFullName.size() = " <<  bookFullName.size();
	bibleid = id;
	currentBiblePath = path;

}
void zefaniaBible::loadBibleData(int id,QString path)
{
	qDebug("zefania::loadCached() start");
	if(zefset.zefaniaBible_Cache == true)
	{
		if(checkForCacheFiles(path))
		{
			//load from the info file all relevant data
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
	QDomDocument doc;
	if (!doc.setContent(&file))
	{
		file.close();
		return "";
	}
	QDomElement root = doc.documentElement();
	cbiblename = root.attribute("biblename","");
	bibleName = cbiblename;
	file.close();
	return bibleName;

}
struct stelle zefaniaBible::search(QString searchstring,bool regexp,bool whole,bool casesen)
{
	Q_UNUSED(whole);//todo: use it
	lastSearch = searchstring;
	QStringList outlist;
	struct stelle st2;
	if(zefset.zefaniaBible_Cache == true)//hard cache
	{
		QProgressDialog progressCache(QObject::tr( "Read Cache" ), QObject::tr( "Cancel" ), 0, bookFullName.size()-1);
		progressCache.setWindowModality(Qt::WindowModal);
		for(int i=0;i<bookFullName.size();i++)
		{
			progressCache.setValue(i);
			if(cache_books_dataB[i] != true)
			{
				cache_books_data[i] = readBookFromCache(currentBiblePath,i);
				cache_books_dataB[i] = true;
			}
				//todo:make faster and read everything evrytime from from hard cache
		}
		progressCache.close();
	}
	QProgressDialog progress(QObject::tr( "Searching"), QObject::tr("Cancel"), 0, cache_books_data.size());
	progress.setWindowModality(Qt::WindowModal);
	QMapIterator<int, QDomElement> i(cache_books_data);
	while (i.hasNext())//all books
	{
		i.next();
		progress.setValue(i.key());
		if (progress.wasCanceled())
			return st2;
		QDomElement ncache = i.value();
		QString outtext="";
		int count2 = 0;
		QDomNode n = ncache.firstChild();
		while(!n.isNull())//all chapters
		{
			count2++;outtext="";
			int count3=0;
			QDomNode n2 = n.firstChild();
			while(!n2.isNull())//all verses
			{
				count3++;
				QDomElement e2 = n2.toElement();
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
				if(whole == true)
				{

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
	return st2;
}
