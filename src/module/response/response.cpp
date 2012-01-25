#include "response.h"

Response::Response()
{
    //m_layout = Qt::LayoutDirectionAuto;
}
bool Response::isValid() const
{
    return false;
}
Response::ResponseType Response::type() const
{
    return Response::UnkownResponse;
}
/*void Response::setLayoutDirection(const Qt::LayoutDirection layout)
{
    m_layout = layout;
}

Qt::LayoutDirection Response::layoutDirection() const
{
    return m_layout;
}
*/
