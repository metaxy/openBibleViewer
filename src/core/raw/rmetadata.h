#ifndef RMETADATA_H
#define RMETADATA_H
#include "src/core/obvcore.h"

class RMetaData
{
public:
    enum RType {
        Block,
        ElementaryBlock,
        BookBlock,
        ChapterBlock,
        VerseBlock,
        StyleFragment,
        TextFragment
    };

    RMetaData(quint32 parent, RMetaData::RType t);
    

    
    RMetaData::RType type;
    quint32 parentRID;
    
};

#endif // RMETADATA_H
