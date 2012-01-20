#ifndef URLRESPONSE_H
#define URLRESPONSE_H
#include "src/module/response/response.h"
#include <QtCore/QString>
class UrlResponse : public Response
{
public:
    UrlResponse(const QString &url);

    bool isValid() const;
    Response::ResponseType type() const;

    QString url() const;
private:
    QString m_url;
};

#endif // URLRESPONSE_H
