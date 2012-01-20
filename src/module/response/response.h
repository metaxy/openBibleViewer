#ifndef RESPONSE_H
#define RESPONSE_H

class Response
{
public:
    enum ResponseType {
        UnkownResponse = 0,
        TextRangesResponse = 1,
        UrlReponse = 2,
        StringReponse = 3
    };

    Response();
    virtual bool isValid() const;
    virtual Response::ResponseType type() const;
};

#endif // RESPONSE_H
