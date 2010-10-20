#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "src/module/simplemoduleclass.h"

class Dictionary : public SimpleModuleClass
{
public:
    Dictionary();
    int loadModuleData(const int &id);
    QString getEntry(const QString &key);
    QStringList getAllKeys();
private:
    Module *m_module;
    ZefaniaLex *m_zefaniaLex;
    BibleQuoteDict *m_bibleQuoteDict;
};

#endif // DICTIONARY_H
