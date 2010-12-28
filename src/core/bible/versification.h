#ifndef VERSIFICATION_H
#define VERSIFICATION_H
#include <QtCore/QStringList>
#include <QtCore/QFlags>
class Versification: public QObject
{
    Q_OBJECT
public:
    Versification();
    virtual ~Versification();
    enum VersificationFilter {
        ReturnAll = 0x0,
        ReturnOT = 0x1,
        ReturnNT = 0x2
    };
    Q_DECLARE_FLAGS(VersificationFilterFlags, VersificationFilter)

    virtual QStringList getBookNames(VersificationFilterFlags filter) const = 0;
    virtual QList<QStringList> multipleBookShortNames(VersificationFilterFlags filter) const = 0;
    virtual QStringList bookShortNames(VersificationFilterFlags filter) const = 0;
    virtual QList<int> maxChapter(VersificationFilterFlags filter) const = 0;
    virtual QList< QList<int> > maxVerse(VersificationFilterFlags filter) const = 0;

protected:
    QStringList m_bookNames;
    QList<QStringList> m_bookShortNames;
    QList<int> m_maxChapter;
    QList<QList<int> > m_maxVerse;

};
Q_DECLARE_OPERATORS_FOR_FLAGS(Versification::VersificationFilterFlags)
#endif // VERSIFICATION_H
