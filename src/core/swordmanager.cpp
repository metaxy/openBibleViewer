#ifdef BUILD_WITH_SWORD
#include "swordmanager.h"
#include <encfiltmgr.h>

SwordManager::SwordManager()
{
    m_library = NULL;
}
SwordManager::~SwordManager()
{
    if(m_library != NULL) {
        delete m_library;
    }
}

void SwordManager::init()
{
    m_library = new sword::SWMgr(0, 0, false, new sword::EncodingFilterMgr( sword::ENC_UTF8 ), true);
}
sword::SWMgr * SwordManager::getManager() const
{
    return m_library;
}
#endif
