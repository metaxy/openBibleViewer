#include "htmlresponse.h"

HtmlResponse::HtmlResponse(const QString &data) : StringResponse(data)
{
}
Response::ResponseType HtmlResponse::type() const
{
    return Response::HtmlResponse;
}
