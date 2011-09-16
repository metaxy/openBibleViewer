#ifndef WEBDICTIONARY_H
#define WEBDICTIONARY_H
#include "src/module/dictionary/dictionarymodule.h"

class WebDictionary : public DictionaryModule
{
public:
    WebDictionary();


    QString getEntry(const QString &entry);
    QStringList getAllKeys();
private:
    bool m_loaded;
};

#endif // WEBDICTIONARY_H
