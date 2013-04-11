#ifndef SEARCHAPI_H
#define SEARCHAPI_H
#include <QObject>
#include "src/core/basicclass.h"
#include <QtWebKit/QWebFrame>
#include "src/core/search/search.h"

class SearchApi : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit SearchApi(QObject *parent = 0);
    QString name() const;
signals:
    
public slots:
    QVariantList search(const int moduleID, const QString &query);
};

#endif // SEARCHAPI_H
