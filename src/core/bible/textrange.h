#ifndef TEXTRANGE_H
#define TEXTRANGE_H
#include <QtCore/QString>
#include <QtCore/QList>
#include "src/core/bible/verse.h"
class TextRange
{
public:
    TextRange();
    QString join(const QString &seperator);
    void addVerse(const Verse &verse);
    void addVerse(const QList<Verse> &verse);
};

#endif // TEXTRANGE_H
