#ifndef HTMLRESPONSE_H
#define HTMLRESPONSE_H
#include "stringresponse.h"

class HtmlResponse : public StringResponse
{
public:
    HtmlResponse(const QString &data);
    Response::ResponseType type() const;
};

#endif // HTMLRESPONSE_H
