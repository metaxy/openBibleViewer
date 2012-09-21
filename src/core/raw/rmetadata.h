#ifndef RMETADATA_H
#define RMETADATA_H
#include "src/core/obvcore.h"
class RMetaData
{
public:
    RMetaData(rid parent, RMetaData::RType t);
    
    enum RType {
        Block,
        ElementaryBlock,
        BookBlock,
        ChapterBlock,
        VerseBlock,
        StyleFragment,
        TextFragment
    };
    
    RMetaData::RType type;
    rid parentRID;
    
};

#endif // RMETADATA_H
