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
#ifndef RBLOCK_H
#define RBLOCK_H
#include <QFlags>
#include <QMap>
#include "src/core/raw/rmetadata.h"

class RBlock
{

public:
    
    RBlock(quint32 id, const RMetaData &data);
    RBlock(const RBlock& other);
    virtual RBlock& operator=(const RBlock& other);
    virtual bool operator==(const RBlock& other) const;
    void add(const RBlock &rblock);
    
    QMap<quint32, RBlock> children;
    quint32 rid() const;
    void setRID(quint32 rid);
    RMetaData metaData() const;
    
    
private:
    quint32 m_rid;
    RMetaData m_data;
};

#endif // RBLOCK_H
