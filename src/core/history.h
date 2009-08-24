#ifndef HISTORY_H
#define HISTORY_H
#include <QtCore/QString>
#include <QtCore/QStringList>
class history
{
private:
	QList<QString>  forwardItems,backwardItems;
	bool goF;
public:
	history();
	QString forward();
	QString backward();
	bool forwardAvailable();
	bool backwardAvailable();
	bool setCurrent(QString current);
};

#endif // HISTORY_H
