#ifndef SESSION_H
#define SESSION_H
#include <QtCore/QVariant>
class Session
{
public:
    Session();
    void setData(QString key,QVariant value);
    QVariant getData(QString key, QVariant defaultValue);
    QVariant getData(QString key);
    bool isSet(QString key);
    QMapIterator<QString, QVariant> getInterator();
private:
    QMap<QString,QVariant> m_data;


};

#endif // SESSION_H
