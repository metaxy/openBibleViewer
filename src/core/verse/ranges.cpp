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
#include "ranges.h"

Ranges::Ranges()
{
}
void Ranges::addRange(Range range)
{
    m_ranges << range;
}
void Ranges::addRanges(Ranges ranges)
{
    m_ranges << ranges.getList();
}
QList<Range> Ranges::getList() const
{
    return m_ranges;
}
