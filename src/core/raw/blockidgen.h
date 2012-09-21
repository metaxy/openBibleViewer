#ifndef BLOCKIDGEN_H
#define BLOCKIDGEN_H
#include <qglobal.h>
#include "src/core/obvcore.h"
class BlockIDGen
{
    BlockIDGen();
    rid counter;
    rid next();
    
};

#endif // BLOCKIDGEN_H
