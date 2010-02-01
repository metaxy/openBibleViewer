#ifndef STRONG_H
#define STRONG_H
#include "src/module/simplemoduleclass.h"
#include "src/module/zefania-strong.h"
class Strong : public SimpleModuleClass
{
public:
    Strong();
    QStringList loadStrongs();
    QString loadStrong(QString strongID);
    void showStrong(const QString &strongID);
    void loadStrongModule(int lID);
    int m_currentStrongModule;
       QString m_currentStrongID;
       QList<int> m_strongModuleID;
private:
    ZefaniaStrong m_zefStrong;
};

#endif // STRONG_H
