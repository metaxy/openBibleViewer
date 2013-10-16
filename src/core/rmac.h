#ifndef RMAC_H
#define RMAC_H
#include <QString>
#include <QHash>
/**
 * @brief The RMAC class
 * We need it for fast parsing in indexing bibles and searching.
 */
class RMAC
{
public:

    enum Content {

    };
    enum Selector {
        PartOfSpeech,
        Case,
        Number,
        Gender,
        Degree,
        Tense,
        Voice,
        Mood,
        Person
    };

    RMAC();

    RMACItem parseString(const QString &item);

    //QString sSelector(RMAC::Selector) const;


};

class RMACItem
{
public:
    QString id;
    QHash<RMAC::Content, RMAC::Selector> data;
};

#endif // RMAC_H
