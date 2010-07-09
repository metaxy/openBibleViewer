#ifndef BIBLEAPI_H
#define BIBLEAPI_H
#include "src/core/basicclass.h"
#include <QObject>
#include <QtWebKit/QWebFrame>
class BibleApi : public QObject, public BasicClass
{
    Q_OBJECT
public slots:
    void activateBible(int moduleID);

public:
    BibleApi();
    void setFrame(QWebFrame *frame);
private:
    QWebFrame *m_frame;
};

#endif // BIBLEAPI_H
