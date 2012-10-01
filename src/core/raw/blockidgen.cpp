#include "blockidgen.h"

BlockIDGen::BlockIDGen() : counter(0)
{

}

quint32 BlockIDGen::next()
{
    return ++counter;
}

