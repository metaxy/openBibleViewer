#ifndef STRONG_H
#define STRONG_H
#include "src/module/simplemoduleclass.h"
#include "src/module/zefania-strong.h"
class Strong : public SimpleModuleClass
{
public:
    Strong();
    QString loadStrong(QString strongID);
    void loadStrongModule(const int &id, QString path);
    int m_strongModuleID;
    QString m_strongString;
private:
    ZefaniaStrong m_zefStrong;
};

#endif // STRONG_H
