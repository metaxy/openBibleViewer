#ifndef BOOKBLOCK_H
#define BOOKBLOCK_H
#include "rblock.h"

class BookBlock : public RBlock
{
    
public:
    BookBlock(quint32 id, const RMetaData &data);
};

#endif // BOOKBLOCK_H
