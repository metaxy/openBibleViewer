#ifndef BLOCKIDGEN_H
#define BLOCKIDGEN_H
#include "src/core/obvcore.h"
class BlockIDGen
{
public:
    BlockIDGen();
    quint32 counter;
    quint32 next();
    
};

#endif // BLOCKIDGEN_H
