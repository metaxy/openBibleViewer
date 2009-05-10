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
#include "zefania.h"
#include <QFile>
#include <QtXml>
#include <QDomAttr>
#include <QDomElement>
#include <QProgressDialog>
#include <QMessageBox>
zefania::zefania()
{
}
int zefania::setSettings( struct settings_s *settings )
{
	zefset = *settings;
	return 1;
}
void zefania::readBook(int id)
{
	qDebug() << "zefania::readBook() id = "<<id;
	chapterText.clear();
	chapterData.clear();
	currentBookID = id;
	QDomElement ncache = cache_books_data[id];
	QString outtext="";
	int count2 = 0;
	QDomNode n = ncache.firstChild();

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
			c.data << e2.text();
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
void zefania::loadBibleData(int id,QString path)
{
	qDebug("zefania::loadBibleData() start");
	QProgressDialog progress("Loading Bible", "Cancel", 0, 76);
	int i=0;//für QProgressDialog
	progress.setWindowModality(Qt::WindowModal);
	bibleid = id;
	//data clear
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
		QMessageBox::critical(0,QObject::tr("Erorr"),QObject::tr("Can not read the file"));
                qDebug("zefania::loadBibleData() cant read the file");
		return;
	}
	QDomDocument doc;
	if (!doc.setContent(&file))
	{
		QMessageBox::critical(0,QObject::tr("Erorr"),QObject::tr("The file is not valid"));
                qDebug("zefania::loadBibleData() the file isnt valid");
		return;
	}
	progress.setValue(10);
	QDomElement root = doc.documentElement();
	bibleName = root.attribute("biblename","");

	QDomNode n = doc.documentElement().firstChild();
	i=10;
	while(!n.isNull())
	{
		i++;
		progress.setValue(i);
		QDomElement e = n.toElement();
                if(e.attribute("bname","") != "" || e.attribute("bnumber","") != "")
                {
			bookCount << "150";
			cache_books_data << e.toElement();
			bookFullName << e.attribute("bname",e.attribute("bnumber",""));
			bookShortName << e.attribute("bsname",QObject::tr("(unknown)"));
                }
		n = n.nextSibling();
	}
	file.close();
	qDebug("zefania::loadBibleData() exit");
}
QString zefania::readInfo(QFile &file)
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
struct stelle zefania::search(QString searchstring,bool regexp,bool whole,bool casesen)
{
	Q_UNUSED(whole);//todo: use it
	lastSearch = searchstring;
        QProgressDialog progress(QObject::tr( "Searching"), QObject::tr("Cancel"), 0, cache_books_data.size());
	progress.setWindowModality(Qt::WindowModal);
	QStringList outlist;
	struct stelle st2;
	for(int id=0;id<cache_books_data.size();id++)
	{
                progress.setValue(id);
		if (progress.wasCanceled())
			 return st2;
		QDomElement ncache = cache_books_data[id];
		QString outtext="";
		int count2 = 0;
		QDomNode n = ncache.firstChild();
		while(!n.isNull())
		{
			count2++;outtext="";
			int count3=0;
			QDomNode n2 = n.firstChild();
			while(!n2.isNull())//alle verse
			{
				count3++;
				QDomElement e2 = n2.toElement(); // try to convert the node to an element.
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
					st2.book << id;
					st2.chapter << count2;
					st2.verse << count3;
				}
				n2 = n2.nextSibling();
			}
			n = n.nextSibling();
		}
	}
	progress.close();
	return st2;
}

