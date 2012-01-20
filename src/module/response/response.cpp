#include "response.h"

Response::Response()
{
}
bool Response::isValid() const
{
    return false;
}
Response::ResponseType Response::type() const
{
    return Response::UnkownResponse;
}
