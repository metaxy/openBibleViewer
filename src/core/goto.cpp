#include "goto.h"
#include <QtCore/QStringList>
#include <QtCore/QRegExp>
#include <QtCore/QtDebug>
goTo::goTo(int currentBibleID_,QStringList bookFullName_,int chapterAdd_)
{
	currentBibleID = currentBibleID_;
	bookFullName = bookFullName_;
	chapterAdd = chapterAdd_;
	qDebug() << "goTo::goTo() chapterAdd = " << chapterAdd;
}
QString goTo::getUrl(const QString& text)
{
	qDebug() << "goTo::getUrl() text = " << text;
	if(text.size() < 1)
		return QString();
	QStringList reg;
	QRegExp foundRegExp;
	//reg << "![A-Za-z0-9\.\s]{2,}!is" << "![A-Za-z0-9\.\s]{2,} [0-9]{1,}!is" << "![A-Za-z0-9\.\s]{2,} [0-9]{1,},[\s]{0,1}[0-9]{1,}!is";
	reg << "(.*)" << "(.*)(\\s+)(\\d+)"  << "(.*)(\\s+)(\\d+),(\\d+)";
	int found = -1;
	for(int i = reg.size()-1; i >= 0;--i)
	{
		QRegExp r(reg.at(i));
		int pos = r.indexIn(text);
		qDebug() << "pos = " << pos;
		if(pos > -1)
		{
			found = i;
			foundRegExp = r;
			break;
		}
	}
	qDebug() << "goTo::getUrl() found = " << found;
	if(found == 0)//bsp: Hiob
	{
		int bookID = bookNameToBookID(foundRegExp.cap(1));
		return "bible://"+QString::number(currentBibleID)+"/"+QString::number(bookID)+","+QString::number(0)+","+QString::number(0);

	}
	else if(found == 1)//Hiob 4
	{
		int bookID =  bookNameToBookID(foundRegExp.cap(1));
		int chapterID = foundRegExp.cap(3).toInt() - 1;
		return "bible://"+QString::number(currentBibleID)+"/"+QString::number(bookID)+","+QString::number(chapterID)+","+QString::number(0);
	}
	else if(found == 2)//Hiob 4,9
	{
		int bookID =  bookNameToBookID(foundRegExp.cap(1));
		int chapterID = foundRegExp.cap(3).toInt() - 1;
		int verseID = foundRegExp.cap(4).toInt() - 1;
		return "bible://"+QString::number(currentBibleID)+"/"+QString::number(bookID)+","+QString::number(chapterID)+","+QString::number(verseID);
	}
	return QString();
}
int goTo::bookNameToBookID(const QString& name)
{
	int min=-1,bookID=-1;
	for(int i = 0;i < bookFullName.size();i++)
	{
		if(name == bookFullName.at(i))
		 {
			 bookID = i;
			 break;
		 }
	}
	if(bookID == -1)
	{
		for(int i = 0;i < bookFullName.size();i++)
		{
			if(bookFullName.at(i).startsWith(name,Qt::CaseInsensitive))
			{
				bookID = i;
				break;
			}
		}
	}
	if(bookID == -1)
	{
		for(int i = 0;i < bookFullName.size();i++)
		{
			if(bookFullName.at(i).contains(name,Qt::CaseInsensitive))
			{
				bookID = i;
				break;
			}
		}
	}
	if(bookID == -1)
	{
		for(int i = 0;i < bookFullName.size();i++)
		{
			int lev = levenshteinDistance(name,bookFullName.at(i));
			if(lev < min || min < 0)
			{
				bookID = i;
				min = lev;
			}
		}
	}
	return bookID;
}
int goTo::levenshteinDistance(const QString& s, const QString& t)
{
	QByteArray m_d;
	int m = s.length();
	int n = t.length();
	int dWidth = m + 1 ;

	// make sure the matrix is big enough

	if (m_d.size() < (m + 1) * (n + 1))
	{
		m_d.resize((m + 1) * (n + 1));
	}

	int i;

	int j;

	// init 0..m, 0..n as starting values - distance to ""

	for (i = 0; i <= m; i++)
	{
		m_d[i + 0*dWidth] = i;
	}
	for (j = 0; j <= n; j++)
	{
		m_d[0 + j*dWidth] = j;
	}
	int cost;
	for (i = 1; i <= m; i++)
	{
		for (j = 1; j <= n; j++)
		{
			if (s[i-1] == t[j-1])
			{
				// if current char is equal, no cost for substitution
				cost = 0;
			}
			else
			{
				cost = 1;
			}

			m_d[i + j*dWidth] = qMin(qMin(   // min of three possibilities

					m_d[i-1 + (j)*dWidth] + 1,       // deletion
					m_d[i   + (j-1)*dWidth] + 1),     // insertion
						 m_d[i-1 + (j-1)*dWidth] + cost);   // substitution
		}
	}

	return m_d[m + n*dWidth];
}
