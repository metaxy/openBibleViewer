#ifndef CHAPTERBLOCK_H
#define CHAPTERBLOCK_H
#include "rblock.h"

class ChapterBlock : public RBlock
{
public:
    ChapterBlock(quint32 id, const RMetaData &data);
};

#endif // CHAPTERBLOCK_H
