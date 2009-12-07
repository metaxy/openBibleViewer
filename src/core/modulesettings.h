#ifndef MODULESETTINGS_H
#define MODULESETTINGS_H
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QStringList>
class ModuleSettings
{
public:
    ModuleSettings();
    QString modulePath;
    QString moduleName;
    QString moduleType;
    QString encoding;
    bool isDir;
    int zefbible_textFormatting;//0 = Neue Zeile nach Vers, 1 = Unformatierter Textblock
    bool zefbible_hardCache;
    bool zefbible_softCache;
    bool zefbible_showStrong;
    bool zefbible_showStudyNote;
    bool biblequote_removeHtml;
    int uModuleCount;

    QMap <QString, QVariant> bookCount;//QMap<int,int>
    QMap<QString, QVariant> bookNames;//QStringList
    QMap<QString,QVariant> biblePath;//QString
    QMap<QString,QVariant> bibleName;//QString


};

#endif // MODULESETTINGS_H
