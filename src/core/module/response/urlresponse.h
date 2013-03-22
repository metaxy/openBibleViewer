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
#ifndef URLRESPONSE_H
#define URLRESPONSE_H
#include "src/core/module/response/response.h"
#include <QtCore/QString>
#include "src/core/blockrules.h"
class UrlResponse : public Response
{
public:
    UrlResponse(const QString &url);
    UrlResponse(const QString &url, const BlockRules &rules);
    bool isValid() const;
    Response::ResponseType type() const;

    QString url() const;

    BlockRules blockRules();
private:
    QString m_url;
    BlockRules m_blockRules;
};

#endif // URLRESPONSE_H
