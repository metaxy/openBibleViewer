/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
#include "searchhit.h"

SearchHit::SearchHit()
{

}
SearchHit::SearchHitType SearchHit::type()
{
    return m_type;
}
void SearchHit::setType(SearchHitType type)
{
    m_type = type;
}
QVariant SearchHit::value(SearchHitKey v)
{
    return m_map.value(v);
}
void SearchHit::setValue(SearchHitKey k, QVariant value)
{
    m_map.insert(k, value);
}
