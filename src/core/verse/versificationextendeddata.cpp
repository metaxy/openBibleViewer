#include "versificationextendeddata.h"

VersificationExtendedData::VersificationExtendedData()
{
    m_hasChapterZero = false;
}
void VersificationExtendedData::setHasChapterZeor(bool chapterZero)
{
    m_hasChapterZero = chapterZero;
}

bool VersificationExtendedData::hasChapterZero()
{
    return m_hasChapterZero;
}
