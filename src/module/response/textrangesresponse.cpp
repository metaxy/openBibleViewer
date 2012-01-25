#include "textrangesresponse.h"

TextRangesResponse::TextRangesResponse(const TextRanges &ranges) : m_textRanges(ranges)
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
TextRanges TextRangesResponse::ranges() const
{
    return m_textRanges;
}
