#ifndef ZEFANIASTRONG_H
#define ZEFANIASTRONG_H
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include "../core/config.h"
#include "../core/moduleconfig.h"
class zefaniaStrong
{
private:
	struct settings_s zefset;
	struct moduleConfig mConfig;
	QStringList m_id,m_title,m_trans,m_pron,m_desc;
	QDomElement format(QDomElement e);
public:
	zefaniaStrong();
	int setSettings( struct settings_s settings, struct moduleConfig mConfig );
	QString loadFile(QString fileData,QString fileName);
	bool loadDataBase(QString fileName);
	QString getStrong(QString strongID);
};

#endif // ZEFANIASTRONG_H
