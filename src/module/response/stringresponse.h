#ifndef STRINGRESPONSE_H
#define STRINGRESPONSE_H
#include "src/module/response/response.h"
#include <QtCore/QString>
class StringResponse : public Response
{
public:
    StringResponse(const QString &data);

    bool isValid() const;
    virtual Response::ResponseType type() const;

    QString data() const;
    void append(const QString &t);
    void prepend(const QString &t);
protected:
    QString m_data;
};

#endif // STRINGRESPONSE_H
