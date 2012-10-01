#include "rblock.h"
#include "rmetadata.h"

RBlock::RBlock(quint32 rid, const RMetaData &data) : m_rid(rid), m_data(data)
{
}

RBlock::RBlock(const RBlock& other) : children(other.children), m_rid(other.rid()), m_data(other.metaData())
{
}

RBlock& RBlock::operator=(const RBlock& other)
{
    return *this;
}

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
