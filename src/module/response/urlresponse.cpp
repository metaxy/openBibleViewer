#include "urlresponse.h"

UrlResponse::UrlResponse(const QString &url) : m_url(url)
{
}
bool UrlResponse::isValid() const
{
    return true;
}
Response::ResponseType UrlResponse::type() const
{
    return Response::UrlReponse;
}
QString UrlResponse::url() const
{
    return m_url;
}
