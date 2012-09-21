#include "blockidgen.h"

BlockIDGen::BlockIDGen() : counter(0)
{

}

rid BlockIDGen::next()
{
    return ++counter;
}

