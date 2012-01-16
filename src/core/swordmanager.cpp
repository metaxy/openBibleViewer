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
    m_library = sword::SWMgr(0, 0, false, new sword::EncodingFilterMgr( sword::ENC_UTF8 ), true);
}
