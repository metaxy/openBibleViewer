#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "src/module/simplemoduleclass.h"

class Dictionary : public SimpleModuleClass
{
public:
    Dictionary();
    int loadModuleData(const int &id);
    QString getEntry(const QString &string);
private:
    Module *m_module;
    ZefaniaStrong *m_zefaniaStrong;
};

#endif // DICTIONARY_H
