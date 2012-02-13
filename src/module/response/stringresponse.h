/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#ifndef STRINGRESPONSE_H
#define STRINGRESPONSE_H
#include "src/module/response/response.h"
#include <QtCore/QString>
class StringResponse : public Response
{
public:
    StringResponse(const QString &data);

    bool isValid() const;
    virtual Response::ResponseType type() const;

    QString data() const;
    void append(const QString &t);
    void prepend(const QString &t);
protected:
    QString m_data;
};

#endif // STRINGRESPONSE_H
