#ifndef VERSEBLOCK_H
#define VERSEBLOCK_H

#include "src/core/raw/rblock.h"


class VerseBlock : public RBlock
{
public:
    VerseBlock(quint32 id, const RMetaData& data);
};

#endif // VERSEBLOCK_H
