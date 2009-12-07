#include "settings.h"
#include "dbghelper.h"
Settings::Settings()
{
}
ModuleSettings Settings::getModuleSettings(const int &bibleID)
{
    int id = moduleID[bibleID];
    if(module.size() < id) {
        return ModuleSettings();
    } else {
        return module.at(id);
    }
}
void Settings::replaceModule(const int &bibleID,ModuleSettings m)
{
    int id = moduleID[bibleID];
    module.replace(id,m);
}
int Settings::getCacheID(int bibleID)
{
    DEBUG_FUNC_NAME
    int sID = moduleID[bibleID];
    //zähle wie oft sID in moduleID noch verkommt
    int counter = 0;
    QMapIterator<int, int> i(moduleID);
    while (i.hasNext()) {
        i.next();
        if(i.key() == bibleID)
            break;
         if(i.value() == sID)
            ++counter;

    }
    myDebug() << "sid = "<< sID << " counter = " << counter;
    return counter-1;
}

void Settings::setBookCount(const int &bibleID, QMap<int, int>bookCount)
{
    ModuleSettings m = getModuleSettings(bibleID);

    QMap<QString,QVariant> newBookCount;
    QMapIterator<int, int> i(bookCount);
    while (i.hasNext()) {
         i.next();
         newBookCount[QString(i.key())] = QVariant(i.value());
     }
    m.bookCount[QString::number(getCacheID(bibleID))] = QVariant(newBookCount);
    replaceModule(bibleID,m);

}
QList<QMap<int, int> > Settings::getBookCount()
{
   /* QList<QMap<int, int> > ret;
    for(int c=0; c < module.size();++c)
    {
        QMapIterator<int, int> i(module.at(c).bookCount);
        while (i.hasNext()) {
             i.next();
         //    ret << i.value();
         }
    }*/
}

void Settings::setBookNames(const int &bibleID, QStringList bookNames)
{
    ModuleSettings m = getModuleSettings(bibleID);
    m.bookNames[QString::number(getCacheID(bibleID))] = QVariant(bookNames);
    replaceModule(bibleID,m);

}
QList<QStringList> Settings::getBookNames()
{
    QList<QStringList> ret;
    for(int c=0; c < module.size();++c)
    {
        QMapIterator<QString, QVariant> i(module.at(c).bookNames);
        while (i.hasNext()) {
             i.next();
             ret << i.value().toStringList();
         }
    }
    return ret;
}

void Settings::setBibleName(const int &bibleID, QString bibleName)
{
    ModuleSettings m = getModuleSettings(bibleID);
    m.bibleName[QString::number(getCacheID(bibleID))] = QVariant(bibleName);
    replaceModule(bibleID,m);

}
QStringList Settings::getBibleName()
{
    QStringList ret;
    for(int c=0; c < module.size();++c)
    {
        QMapIterator<QString, QVariant> i(module.at(c).bibleName);
        while (i.hasNext()) {
             i.next();
             ret << i.value().toString();
         }
    }
    return ret;
}

void Settings::setBiblePath(const int &bibleID, QString biblePath)
{
    DEBUG_FUNC_NAME
    ModuleSettings m = getModuleSettings(bibleID);
    m.biblePath[QString::number(getCacheID(bibleID))] = QVariant(biblePath);
    replaceModule(bibleID,m);
    myDebug() << m.biblePath;
}
QStringList Settings::getBiblePath()
{
    DEBUG_FUNC_NAME
    QStringList ret;
    for(int c=0; c < module.size();++c)
    {
        QMapIterator<QString, QVariant> i(module.at(c).biblePath);
        while (i.hasNext()) {
             i.next();
             ret << i.value().toString();
         }
    }
   // myDebug() << ret;
    return ret;
}
void Settings::clearNamesCache(const int &id)
{
    ModuleSettings m = module.at(id);
    m.bibleName.clear();
    m.biblePath.clear();
    m.bookCount.clear();
    m.bookNames.clear();
    module.replace(id,m);
}

