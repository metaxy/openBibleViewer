#ifndef MODULECONFIG_H
#define MODULECONFIG_H
#include <QtCore/QString>
struct moduleConfig
{
	QString modulePath;
	QString moduleName;
	QString moduleType;
	bool isDir;
	int zefbible_textFormatting;//0 = Neue Zeile nach Vers, 1 = Unformatierter Textblock
	bool zefbible_hardCache;
	bool zefbible_softCache;
	bool zefbible_showStrong;
	bool zefbible_showStudyNote;
	bool biblequote_removeHtml;
};

#endif // MODULECONFIG_H
