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
#include "rblock.h"
#include "rmetadata.h"

RBlock::RBlock(quint32 rid, const RMetaData &data) : m_rid(rid), m_data(data)
{
}

/*RBlock::RBlock(const RBlock& other) : children(other.children), m_rid(other.rid()), m_data(other.metaData())
{
}*/

/*RBlock& RBlock::operator=(const RBlock& other)
{
    return *this;
}*/

bool RBlock::operator==(const RBlock& other) const
{
    return this->rid() == other.rid();
}

quint32 RBlock::rid() const
{
    return m_rid;
}
RMetaData RBlock::metaData() const
{
    return m_data;
}

void RBlock::add(const RBlock& rblock)
{
    children.insert(rblock.rid(), rblock);
}
