#ifndef BLOCKTOOLS_H
#define BLOCKTOOLS_H
#include "src/core/raw/bookblock.h"
#include "src/core/raw/brfragment.h"
#include "src/core/raw/captionblock.h"
#include "src/core/raw/chapterblock.h"
#include "src/core/raw/divblock.h"
#include "src/core/raw/gramblock.h"
#include "src/core/raw/noteblock.h"
#include "src/core/raw/prologblock.h"
#include "src/core/raw/remarksblock.h"
#include "src/core/raw/styleblock.h"
#include "src/core/raw/supblock.h"
#include "src/core/raw/textfragment.h"
#include "src/core/raw/verseblock.h"
#include "src/core/raw/xreffragment.h"
#include "src/core/raw/mediablock.h"
class BlockTools
{
public:
    static RBlock *create(quint32 id, quint32 parentID, RMetaData::RType type);

};

#endif // BLOCKTOOLS_H
