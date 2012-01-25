#ifndef RESPONSE_H
#define RESPONSE_H
#include <QtCore/qnamespace.h>
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

   /* void setLayoutDirection(const Qt::LayoutDirection layout);

    Qt::LayoutDirection layoutDirection() const;
protected:
    Qt::LayoutDirection m_layout;*/

};

#endif // RESPONSE_H
