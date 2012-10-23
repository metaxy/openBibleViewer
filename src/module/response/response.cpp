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
#include "response.h"

Response::Response()
{
    //m_layout = Qt::LayoutDirectionAuto;
}
Response::~Response()
{

}

bool Response::isValid() const
{
    return false;
}
Response::ResponseType Response::type() const
{
    return Response::UnkownResponse;
}
/*void Response::setLayoutDirection(const Qt::LayoutDirection layout)
{
    m_layout = layout;
}

Qt::LayoutDirection Response::layoutDirection() const
{
    return m_layout;
}
*/
