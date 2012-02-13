/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
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
        StringReponse = 3,
        HtmlResponse
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
