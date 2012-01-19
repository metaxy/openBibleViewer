#ifdef BUILD_WITH_SWORD
#include "swordmanager.h"
#include <encfiltmgr.h>
#include <markupfiltmgr.h>
#include <swmgr.h>
#include <swmodule.h>
using namespace::sword;

SwordManager::SwordManager()
{
    m_library = new SWMgr(new MarkupFilterMgr(FMT_PLAIN));
}
SwordManager::~SwordManager()
{
    if(m_library != NULL) {
        delete m_library;
    }
}

sword::SWMgr * SwordManager::getManager() const
{

    return m_library;
}
#endif
