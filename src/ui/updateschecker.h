#ifndef UPDATESCHECKER_H
#define UPDATESCHECKER_H

#include <QtCore/QObject>
#include "src/core/basicclass.h"
#include <QtNetwork/QNetworkReply>


class UpdatesChecker : public QObject, public BasicClass
{
    Q_OBJECT
public:
    UpdatesChecker(QObject *parent = 0);
    void checkForUpdates();

public slots:

    void replyFinished(QNetworkReply* reply);

private slots:
    void updatesAvailable();



private:
    bool isImportantUpdate;
};

#endif // UPDATESCHECKER_H
