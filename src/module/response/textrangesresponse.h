#ifndef TEXTRANGESRESPONSE_H
#define TEXTRANGESRESPONSE_H
#include "src/module/response/response.h"
#include "src/core/verse/textranges.h"
class TextRangesResponse : public Response
{
public:
    TextRangesResponse(const TextRanges &ranges);

    bool isValid() const;
    Response::ResponseType type() const;

    TextRanges ranges() const;
private:
    TextRanges m_textRanges;
};

#endif // TEXTRANGESRESPONSE_H
