#include "textrangesresponse.h"

TextRangesResponse::TextRangesResponse()
{
}

bool TextRangesResponse::isValid() const
{
    return true;
}
Response::ResponseType TextRangesResponse::type() const
{
    return Response::TextRangesResponse;
}
