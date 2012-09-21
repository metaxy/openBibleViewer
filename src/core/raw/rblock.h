#ifndef RBLOCK_H
#define RBLOCK_H
#include <QFlags>
#include <QMap>
#include "src/core/obvcore.h"
class RMetaData;

class RBlock
{

public:
    
    RBlock(qint32 id, const RMetaData &data);
    RBlock(const RBlock& other);
    virtual RBlock& operator=(const RBlock& other);
    virtual bool operator==(const RBlock& other) const;
    void add(const RBlock &rblock);
    
    QMap<qint32, RBlock> children;
    qint32 rid() const;
    void setRID(qint32 rid);
    RMetaData metaData() const;
    
    
private:
    qint32 m_rid;
    RMetaData m_data;
};

#endif // RBLOCK_H
