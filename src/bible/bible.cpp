#include "bible.h"
#include "../core/config.h"
#include <QtCore/QtDebug>
bible::bible()
{
}
void bible::setBibleType(int type)
{
	//check if type is valid
	bibleType = type;
	return;
}
int bible::loadBibleData(int bibleID,QString path)
{
	qDebug() << "bible::loadBibleData() bibleID = "<<bibleID << " path = "<<path << " bibleType ="<<bibleType;
	currentBibleID = bibleID;
	switch(bibleType)
	{
	case 1://biblequote
		{
			bq.setSettings(&settings);

			bq.loadBibleData(bibleID,path);
			bibleName = bq.bibleName;
			bookCount = bq.bookCount;
			bookFullName = bq.bookFullName;
			bookPath = bq.bookPath;
			chapterAdd = 1;
			currentBiblePath = bq.currentBiblePath;

			break;
		}
	case 2://zefania
		{
			zef.setSettings(&settings);

			zef.loadBibleData(bibleID,path);
			bibleName = zef.bibleName;
			bookCount = zef.bookCount;
			bookFullName = zef.bookFullName;
			bookPath = zef.bookPath;
			chapterAdd = 0;
			currentBiblePath = zef.currentBiblePath;
			break;
		}
	}
	qDebug() << "bible::loadBibleData() end";
	return 0;
}
int bible::readBook(int id)
{
	currentBookID = id;
	qDebug() << "bible::readBook() id= "<<id << " bibleType ="<<bibleType;
	switch(bibleType)
	{
	case 1://biblequote
		{
			chapterText.clear();
			chapterData.clear();

			if(id < bookPath.size())
			{
				bq.readBook(id, bookPath.at(id));
			}
			else
			{
				return 1;
				qDebug() << "bible::readBook() index out of range bookPath";
			}
			chapterText = bq.chapterText;
			chapterData = bq.chapterData;
			QStringList chapters;
			QString c = bookCount.at(id);
			int cc = c.toInt();
			if(bq.chapterZero == true)
			{
				for (int i = 0; i < cc; i++)
				{
					chapters << QString::number(i, 10);
				}
			}
			else
			{
				for (int i = 1; i <= cc; i++)
				{
					chapters << QString::number(i, 10);
				}
			}

			chapterNames = chapters;
			qDebug() << "bible::readBook() chapterText.size() = "<<chapterText.size();
			break;
		}
	case 2://zefania
		{
			chapterText.clear();
			chapterData.clear();
			zef.readBook(id);
			chapterText = zef.chapterText;
			chapterData = zef.chapterData;
			QStringList chapters;
			for (int i = 1; i <= zef.book_ccount; i++)
			{
				chapters << QString::number(i, 10);
			}
			chapterNames = chapters;

			qDebug() << "bible::readBook() chapterText.size() = "<<chapterText.size();
			break;
		}
	}
	qDebug() << "bible::readBook() end";
	return 0;
}
void bible::setSettings( struct settings_s *settings_ )
{
	settings = *settings_;
	return;
}
QString bible::readChapter( int chapterID, int verseID = -1 ,int textFormatting = 0)
{
	chapterDataList.clear();
	qDebug() << "bible::readChapter() start";
	currentChapterID = chapterID;
	QString out = "";
	switch(bibleType)
	{
	case 1://biblequote
		{
			chapter c;
			if(chapterID < chapterData.size())
			{

				c = chapterData.at(chapterID);
				qDebug() << "bible::readChapter() chapterID = " << chapterID << " chapterdata.size() = "<<chapterData.size() << " a.size() = "<<c.data.size();
				for(int i =0;i< c.data.size();i++)
				{
					if( i == verseID )
					{
						out += "<b>" + c.data.at(i)+"</b>";
					}
					else
					{
						out += c.data.at(i);
					}
					chapterDataList << c.data.at(i);
				}
				qDebug() << "bible::readChapter() out.size() = " << out.size();

			}
			else
			{
				qDebug() << "bible::readChapter() index out of range index = chapterText.size() =";
			}
			break;
		}
	case 2://zefania
		{
			qDebug() << "bible::readChapter() zefania read";
			chapter c;
			if(chapterID < chapterData.size())
			{

				c = chapterData.at(chapterID);
				qDebug() << "bible::readChapter() chapterID = " << chapterID << " chapterdata.size() = "<<chapterData.size() << " a.size() = "<<c.data.size();
				if(textFormatting == 0)
				{
					out = "<b><font size=\"+5\">"+c.bookName+" "+c.chapterName+"</font></b><br /><br />";
					for(int i = 0;i< c.data.size();i++)
					{
						QString o =  c.verseNumber.at(i)+" "+c.data.at(i) + "<br />";
						if( i == verseID)
						{
							out += "<b>" + o + "</b>";
						}
						else
						{
							out += o;
						}
						chapterDataList << o;
					}
				}
				else
				{
					out = "<b><font size=\"+5\">"+c.bookName+" "+c.chapterName+"</font></b><br /><br />";
					for(int i = 0;i< c.data.size();i++)
					{
						QString o =  c.verseNumber.at(i)+" "+c.data.at(i);
						if( i == verseID)
						{
							out += "<b>" + o + "</b>";
						}
						else
						{
							out += o;
						}
						chapterDataList << o;
					}
				}

				qDebug() << "bible::readChapter() out.size() = " << out.size();

			}
			else
			{
				qDebug() << "bible::readChapter() index out of range index = chapterText.size() =";
			}
			break;
		}
	}
	lastout = out;
	qDebug() << "bible::readChapter() end";
	return out;
}
struct stelle bible::search(QString searchstring,bool regexp,bool whole,bool casesen)
{
	lastSearchString = searchstring;
	struct stelle st;
	switch(bibleType)
	{
	case 1://biblequote
		{
			st = bq.search(searchstring,regexp,whole,casesen);
			break;
		}
	case 2://zefania
		{
			st = zef.search(searchstring,regexp,whole,casesen);
			break;
		}
	}
	st.bibleID = currentBibleID;
	return st;
}
QList<QDomElement> bible::getZefCache()
{
	return zef.cache_books_data;
}
void bible::clearZefCache()
{
	zef.cache_books_data.clear();
}
void bible::setZefCache(QList<QDomElement> cache)
{
	zef.cache_books_data = cache;
}
