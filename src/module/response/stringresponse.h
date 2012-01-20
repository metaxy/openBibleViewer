#ifndef STRINGRESPONSE_H
#define STRINGRESPONSE_H
#include "src/module/response/response.h"
#include <QtCore/QString>
class StringResponse : public Response
{
public:
    StringResponse(const QString &data);

    bool isValid() const;
    Response::ResponseType type() const;

    virtual QString data() const;
private:
    QString m_data;
};

#endif // STRINGRESPONSE_H
