#include "strongurl.h"
const QString scheme_strong = "strong:/";
StrongUrl::StrongUrl()
{
}

void StrongUrl::addStrongNumber(const int number)
{
    m_numbers.append(number);
}

void StrongUrl::setStrongNumbers(const QList<int> &numbers)
{
    m_numbers = numbers;
}

void StrongUrl::setPrefix(StrongPrefix prefix)
{
    m_prefix = prefix;
}
//todo: move out to an extra class
bool StrongUrl::fromText(QString text)
{
    m_numbers.clear();
    text.trimmed();
    if(text.at(0).toLower() == 'g') {
        m_prefix = StrongUrl::G;
    } else if(text.at(0).toLower() == 'h') {
        m_prefix = StrongUrl::H;
    } else {
        return false;
    }
    text.remove(0, 1);
    if(text.contains(" ")) {
        const QStringList parts = text.split(" ");
        foreach(const QString & part, parts) {
            m_numbers.append(part.toInt());
        }
    } else {
        m_numbers.append(text.toInt());
    }
    return true;
}

bool StrongUrl::fromString(QString strong)
{
    //exp:
    //strong:/G1
    //strong:/H10
    //strong:/H10 20
    m_numbers.clear();
    if(!strong.startsWith(scheme_strong))
        return false;
    strong.remove(0, scheme_strong.size());
    strong.trimmed();
    if(strong.at(0).toLower() == 'g') {
        m_prefix = StrongUrl::G;
    } else if(strong.at(0).toLower() == 'h') {
        m_prefix = StrongUrl::H;
    } else {
        return false;
    }
    strong.remove(0, 1);
    if(strong.contains(" ")) {
        const QStringList parts = strong.split(" ");
        foreach(const QString & part, parts) {
            m_numbers.append(part.toInt());
        }
    } else {
        m_numbers.append(strong.toInt());
    }
    return true;
}

QString StrongUrl::toString()
{
    QString ret;
    ret = scheme_strong;
    if(m_prefix == StrongUrl::G) {
        ret += "G";
    } else {
        ret += "H";
    }
    bool beginn = true;
    foreach(const int & i, m_numbers) {
        if(!beginn) {
            ret += " ";
        } else {
            beginn = false;
        }
        ret += QString::number(i);
    }
    return ret;
}
QList<int> StrongUrl::numbers() const
{
    return m_numbers;
}
StrongUrl::StrongPrefix StrongUrl::prefix() const
{
    return m_prefix;
}
