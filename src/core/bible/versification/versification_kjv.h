#ifndef VERSIFICATION_KJV_H
#define VERSIFICATION_KJV_H
#include "src/core/bible/versification.h"
#include <QtCore/QFlags>
#include <QtCore/QObject>
class Versification_KJV : public Versification, public QObject
{
    Q_OBJECT
public:
    Versification_KJV();
    QStringList getBookNames(VersificationFilterFlags filter) const;
    QList<QStringList> multipleBookShortNames(VersificationFilterFlags filter) const;
    QStringList bookShortNames(VersificationFilterFlags filter) const;
    QList<int> maxChapter(VersificationFilterFlags filter) const;
    QList<QList<int> > maxVerse(VersificationFilterFlags filter) const;
};

#endif // VERSIFICATION_KJV_H
