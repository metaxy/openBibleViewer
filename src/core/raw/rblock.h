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
