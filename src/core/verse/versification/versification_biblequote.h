#ifndef VERSIFICATION_BIBLEQUOTE_H
#define VERSIFICATION_BIBLEQUOTE_H

#include "src/core/verse/versification.h"

class Versification_BibleQuote : public Versification
{
public:
    Versification_BibleQuote(const QStringList &fullNames, const QList<QStringList> &shortNames, const QMap<int, int> &bookCount);
private:
    bool filter(const int &bookID, VersificationFilterFlags flags) const;
};

#endif // VERSIFICATION_BIBLEQUOTE_H
