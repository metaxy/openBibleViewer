#ifndef VERSIFICATION_CACHE_H
#define VERSIFICATION_CACHE_H
#include <QtCore/QString>
#include "src/core/verse/versification.h"
class Versification_Cache : public Versification
{
        Q_OBJECT
public:
    Versification_Cache(const QMap<int, BookV11N> &data);
    ~Versification_Cache();
};

#endif // VERSIFICATION_CACHE_H
