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
#include "urlresponse.h"

UrlResponse::UrlResponse(const QString &url) : m_url(url)
{
}
UrlResponse::UrlResponse(const QString &url,const BlockRules &rules) : m_url(url), m_blockRules(rules)
{

}
bool UrlResponse::isValid() const
{
    return true;
}
Response::ResponseType UrlResponse::type() const
{
    return Response::UrlReponse;
}
QString UrlResponse::url() const
{
    return m_url;
}
BlockRules UrlResponse::blockRules()
{
    return m_blockRules;
}
