#ifndef MODULECONFIG_H
#define MODULECONFIG_H
#include <QtCore/QString>
struct moduleConfig
{
	QString modulePath;
	QString moduleName;
	QString moduleType;
	int zefbible_textFormatting;//0 = Neue Zeile nach Vers, 1 = Unformatierter Textblock
	bool biblequote_removeHtml;
};

#endif // MODULECONFIG_H
