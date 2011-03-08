#ifndef STRONGURL_H
#define STRONGURL_H
#include "src/core/url.h"
#include <QtCore/QStringList>
class StrongUrl : public Url
{
public:
    StrongUrl();
    enum StrongPrefix {
        H,
        G
    };

    void addStrongNumber(const int number);
    void setStrongNumbers(const QList<int> &numbers);
    void setPrefix(StrongPrefix prefix);
    bool fromText(QString text);
    bool fromString(QString strong);
    QString toString();

    QList<int> numbers() const;
    StrongPrefix prefix() const;
private:
    StrongPrefix m_prefix;
    QList<int> m_numbers;

};

#endif // STRONGURL_H
