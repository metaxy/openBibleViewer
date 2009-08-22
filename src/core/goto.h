#ifndef GOTO_H
#define GOTO_H

#include <QtCore/QString>
#include <QtCore/QStringList>
class goTo
{
public:
	goTo(int currentBibleID,QStringList bookFullName,int chapterAdd);
	QString getUrl(const QString& text);
private:
	int bookNameToBookID(const QString& name);
	int levenshteinDistance(const QString& s, const QString& t);
	int currentBibleID,chapterAdd;
	QStringList bookFullName;
};

#endif // GOTO_H
