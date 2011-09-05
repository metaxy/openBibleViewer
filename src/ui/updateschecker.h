#ifndef UPDATESCHECKER_H
#define UPDATESCHECKER_H

#include <QtCore/QObject>
#include "src/core/basicclass.h"
#include <QtNetwork/QNetworkReply>


class UpdatesChecker : public QObject, BasicClass
{
    Q_OBJECT
public:
    UpdatesChecker();

public slots:
    void checkForUpdates();

private slots:
    void updatesAvailable();

    void replyFinished(QNetworkReply* reply);

private:
    bool isImportantUpdate;
};

#endif // UPDATESCHECKER_H
