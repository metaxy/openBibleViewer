#ifndef TEXTRANGESRESPONSE_H
#define TEXTRANGESRESPONSE_H
#include "src/module/response/response.h"
class TextRangesResponse : public Response
{
public:
    TextRangesResponse();

    bool isValid() const;
    Response::ResponseType type() const;
};

#endif // TEXTRANGESRESPONSE_H
