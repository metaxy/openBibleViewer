#ifndef VERSIFICATION_ZEFANIA_H
#define VERSIFICATION_ZEFANIA_H
#include "src/core/bible/versification.h"
class Versification_Zefania: public Versification
{
     Q_OBJECT
public:
    Versification_Zefania(const QStringList &fullNames, const QStringList &shortNames, const QStringList &bookIDs);
    void setMaxChapter(const int &bookID, const int &maxChapter);
private:
    bool filter(const int &bookID, VersificationFilterFlags flags) const;
};

#endif // VERSIFICATION_ZEFANIA_H
