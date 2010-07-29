#ifndef BIBLEQUOTEDICT_H
#define BIBLEQUOTEDICT_H
#include <QtCore/QString>
#include <QtCore/QFile>
#include "src/core/settings.h"

class BibleQuoteDict
{
public:
    BibleQuoteDict();
    void setSettings(Settings *settings);
    QString readInfo(QFile &file);
    void buildIndex();
    bool hasIndex();
    QString getEntry(const QString &id);
    void setID(const int &id, const QString &path);
private:
    Settings *m_settings;
    QString m_modulePath;
    int m_moduleID;
};

#endif // BIBLEQUOTEDICT_H
