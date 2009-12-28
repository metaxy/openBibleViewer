#ifndef BIBLEDISPLAY_H
#define BIBLEDISPLAY_H
#include <QtCore/QObject>
#include <QtCore/QString>
class BibleDisplay : public QObject
{
    Q_OBJECT
public:
    BibleDisplay();
    void setHtml(QString text);
    QString m_text;
signals:
    void newHtml(QString html);
};

#endif // BIBLEDISPLAY_H
